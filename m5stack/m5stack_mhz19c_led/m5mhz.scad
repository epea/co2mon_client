module m3(){
    circle(d=3,$fn=20);
}

module base(){
    difference(){
        // base
        square([5+35+44+5,5+37+5+15]);
        
        // mhz mount hole
        translate([5,5+37]) m3();
        translate([7,5+37]) m3();
        translate([5,5+37-1.5]) square([2,3]);
        
        // m5stack
        translate([5+35,5]) m3();
        translate([5+35+44,5]) m3();
        
        // MHZ
        translate([10,5+37+10]) m3();
        translate([25,5+37+10]) m3();
        translate([15,2]) square([10,2]);
        
        // LED
        translate([50,5+37+16]) m3();
        translate([70,5+37+16]) m3();
    }
    
}

base();