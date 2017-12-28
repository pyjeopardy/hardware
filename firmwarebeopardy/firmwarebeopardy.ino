// configuration

// the used pins are 0 - 12 and A1 - A4
// there is a connector for A5 on the board, but it did not work and we do not need it
//
// the pins are sorted so that the numbers written on the controller case are correct
// if you change the cabling you might need to sort them again
//int buzzer_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A0, A1, A2, A3, A4, };
int buzzer_pins[] = {5, 4, 3, 2, 0, 1, A0, A1, A2, A3, A4, 7, 12, 11, 10, 9, 8, 6 };
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
  Serial.begin(19200);

  input.reserve(200);

  for(int i = 0; i < buzzer_pins_count; i ++)
  {
    pinMode(buzzer_pins[i], INPUT_PULLUP);
    buzzer_last_values[i] = LOW;
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
    waiting = false;
  }

  if(input_complete)
  {
    if(input.equals("R")) //reset
    {
      if (buzzer_first_pressed == 0) // no buzzer pressed - Ready 4 Game
      { 
        waiting = true;
        Serial.print("A\n");
      } 
      else   // buzzer pressed - gives idiot who is not releasing it
      { 
        Serial.print(buzzer_first_pressed);
        Serial.print("\n");
      }
    }
    else if(input.equals("A")) // async modus
    {
      Serial.print("A\n");
    }
    else if(input.equals("S") or input.equals("Q")) // sync modus
    {
      Serial.print("ERROR - Sync Mode not supported\n");
    }
    else
      Serial.print("ERROR - unknown command\n");

    input_complete = false;
    input = "";
  }
}


void update_input()
{
  while(Serial.available())
  {
    /*char tmp = Serial.read();

    if (tmp == '\n')
    {
      input_complete = true;
      return;
    }
    else
      input += tmp; */
    if (Serial.available() > 0) {
      char tmp = Serial.read();
      if (tmp != '\r' and tmp != '\n') {
        input += tmp;
        input_complete = true;
      }
    }
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

        if(value == HIGH && waiting && buzzer_first_pressed == 0)
          buzzer_first_pressed = i+1;
      }
    }
  }
}
