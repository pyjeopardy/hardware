draw_parts = 1;
2d = 0;

$fn=100;

// Neutrik NC3FAAV1
module xlr()
{
    color("blue")
    {
        rotate([180, 0, -90])
        import("extern/neutrik.stl");
    }
}

module frontplate()
{
    color("grey") {
        difference() {
            cube([161.5, 304.1, 2]);

            for(x = [30, 80, 130]) {
                for(y = [50-25, 100-25, 150-25, 200-25, 250-25, 300-25]) {
                    translate([x, y, -1])
                    cylinder(4, d=22);
                }
            }
        }
    }
}

if(2d) {
    projection() frontplate();
}
else {
    frontplate();
}

if(draw_parts && !2d) {
    for(x = [30, 80, 130]) {
        for(y = [50-25, 100-25, 150-25, 200-25, 250-25, 300-25]) {
            translate([x, y, 0])
            xlr();
        }
    }
}
