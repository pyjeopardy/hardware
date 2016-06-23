// configuration
int buzzer_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A0, A1, A2, A3, A4, A5, };
long debounce_delay = 50;

// internal variables
const int buzzer_pins_count = sizeof(buzzer_pins) / sizeof(int);

int buzzer_last_values[buzzer_pins_count];
int buzzer_debounce[buzzer_pins_count];

int buzzer_first_pressed = 0;

boolean waiting = false;

String input = "";
boolean input_complete = false;


void setup()
{
  Serial.begin(9600);

  input.reserve(200);

  for(int i = 0; i < buzzer_pins_count; i ++)
  {
    pinMode(buzzer_pins[i], INPUT_PULLUP);
    buzzer_last_values[i] = HIGH;
    buzzer_debounce[i] = 0;
  }
}


void loop()
{
  update_input();

  update_buzzer();

  if(waiting and buzzer_first_pressed != 0)
  {
    Serial.print(buzzer_first_pressed);
    Serial.print("\n");

    buzzer_first_pressed = 0;
    waiting = 0;
  }

  if(input_complete)
  {
    if(input.equals("reset"))
    {
      waiting = false;
      Serial.print("ready\n");
    }
    else if(input.equals("start"))
    {
      waiting = true;
      Serial.print("ready\n");
    }
    else
      Serial.print("unknown command\n");

    input_complete = false;
    input = "";
  }
}


void update_input()
{
  while(Serial.available())
  {
    char tmp = Serial.read();

    if (tmp == '\n')
    {
      input_complete = true;
      return;
    }
    else
      input += tmp;
  }
}


void update_buzzer()
{
  for(int i = 0; i < buzzer_pins_count; i ++)
  {
    if((millis() - buzzer_debounce[i]) > debounce_delay)
    {
      int value = digitalRead(buzzer_pins[i]);

      if(value != buzzer_last_values[i])
      {
        buzzer_last_values[i] = value;
        buzzer_debounce[i] = millis();

        if(value == LOW && waiting && buzzer_first_pressed == 0)
          buzzer_first_pressed = i+1;
      }
    }
  }
}
