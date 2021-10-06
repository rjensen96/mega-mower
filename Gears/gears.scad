
module gear() {
    include <gearlib.scad>;
}

module grub_screw_hole() {
    translate([0, 0, 25]) {
        rotate([90, 0, 0]) {
            cylinder(r=2.8, h=40, $fn=200);
    
        }
    }
}


difference() {
    gear();
    grub_screw_hole();
}
