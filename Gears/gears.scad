
module gear() {
    include <gearlib.scad>;
}

module grub_screw_hole() {
    translate([0, 0, 24]) {
        rotate([90, 0, 0]) {
            cylinder(r=1.9, h=40, $fn=200);
    
        }
    }
}


difference() {
    gear();
    grub_screw_hole();
}
