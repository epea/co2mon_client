module m3(){
    circle(d=3,$fn=20);
}

module m3_3d(height){
    cylinder(h=height,d=3,$fn=20);
}

module m4_3d(height){
    cylinder(h=height,d=4,$fn=20);
}

module m3head_3d(){
    cylinder(h=5,d=7,$fn=20);
}

module m2_3d(height){
    cylinder(h=20,d=2,$fn=20);
}

module m3_mount(height){
    difference(){
        cylinder( h=height,r=5 ,$fn=20);
        cylinder( h=height,r=1.5,$fn=20 );
    }
}

module r_courner_3d(height,r_outer,width){
    difference(){
        cylinder( h=height,r=r_outer , $fn=20);
        cylinder( h= height-width , r = r_outer - width  , $fn=20);
        translate([-r_outer,0,0]) cube([r_outer,r_outer,height]);
        translate([-r_outer,-r_outer,0]) cube([r_outer*2,r_outer,height]);
    }
    

}

base_width = 94;
base_height = 65;
courner_r = 5;

mhz_case_hole_y = 55;

module bottom_base(){
    union(){
        difference(){
            // base
            square([base_width,base_height]);
            
            // R
            translate([0,base_height- courner_r]) square([courner_r,courner_r]);
            translate([base_width-courner_r,base_height- courner_r]) square([courner_r,courner_r]);
        }
        translate([courner_r,base_height- courner_r])  circle(r=courner_r,$fn=20);
        translate([base_width-courner_r,base_height- courner_r])  circle(r=courner_r,$fn=20);
    }
}

module bottom(){
    difference(){
        // base
        bottom_base();
        
        // pcb mount hole
        translate([80,43]) m3();
        translate([80,43-1.5]) square([9,3]);
        translate([89,43]) m3();
        
        // MHZ
        translate([69,mhz_case_hole_y]) m3();
        translate([84,mhz_case_hole_y]) m3();
        translate([69,2]) square([10,2]);
       
        // m5stack
        translate([5,5]) m3();
        translate([49,5]) m3();

        // LED
        translate([45,5+37+16+3]) m3();
        translate([10,5+37+16+3]) m3();

    }
}

bottom();

mhz_case_height = 19;
module mhz_case_base(){
    union(){
        difference(){
            cube([37+2,65,mhz_case_height]);
            translate([37+2 -courner_r,65-courner_r,0]) cube([courner_r,courner_r,mhz_case_height]);
            translate([0,4,0]) cube([37,59,mhz_case_height-1]);
        }
        translate([37+2 -courner_r,65-courner_r,0]) r_courner_3d(mhz_case_height,courner_r,2);
    }
}

module mhz_case(){
    difference(){
    union(){
        difference(){
            mhz_case_base();
            
            translate([14,mhz_case_hole_y,0]) m3_3d(mhz_case_height);
            translate([29,mhz_case_hole_y,0]) m3_3d(mhz_case_height);
            
            for ( y = [12 : 10:47] ){
                translate([10,y,0]) m2_3d(mhz_case_height);
                translate([25,y,0]) m2_3d(mhz_case_height);
            }

            for( x = [0:10:30] ){
                translate([x,4,mhz_case_height-2]) cube([5,2,3]);
            }

            for( y = [8:10:48] ){
                translate([37,y,3]) cube([2,1,mhz_case_height-5]);
            }
        }
        translate([14,mhz_case_hole_y,0]) m3_mount(mhz_case_height);
        translate([29,mhz_case_hole_y,0]) m3_mount(mhz_case_height);
        translate([0,53,0]) cube([2,11,mhz_case_height]);
        translate([14.1,2.1,-2]) cube([9.8,1.8,2.5]);
    }
        translate([14,mhz_case_hole_y,mhz_case_height-5]) m3head_3d();
        translate([29,mhz_case_hole_y,mhz_case_height-5]) m3head_3d();
    }
}
color([0,0,0]) translate([55,0]) mhz_case();

led_case_height = mhz_case_height;
module led_case_base(){
    union(){
        difference(){
            cube([54,13,led_case_height]);
            translate([2,0,0])cube([50,2,led_case_height]);
            translate([1,0,0])cube([52,1,led_case_height]);
            translate([1,0,0]) cube([52,12,led_case_height-1]);
            translate([0,13-courner_r,0]) cube([courner_r,courner_r,led_case_height]);
        }
        translate([courner_r,13-courner_r,0]) 
            rotate( [0,0,90] )
            
            r_courner_3d( led_case_height , courner_r , 1);
    }
}

module led_case(){
    difference(){
        union(){
            difference(){
                led_case_base();
                
                translate([10,8,0]) m3_3d(led_case_height);
                translate([45,8,0]) m3_3d(led_case_height);
            
                translate([29,15,8]) rotate([90,0,0])  m4_3d(led_case_height);
            }
            
            
            translate([10,8,0]) m3_mount(led_case_height);
            translate([45,8,0]) m3_mount(led_case_height);
        }
        
        translate([10,8,led_case_height-5]) m3head_3d();
        translate([45,8,led_case_height-5]) m3head_3d();
    }
}
color([0.5,0.5,0.5]) translate([0,52])led_case();