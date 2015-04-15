/* 
 * File:   HW5.c
 * Author: Eric
 *
 * Created on April 15, 2015, 4:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include<xc.h> // processor SFR definitions
#include<sys/attribs.h> // __ISR macro
#include "i2c_display.h"
#include "i2c_master_int.h"
#include "accel.h"

short accels[3]; // accelerations for the 3 axes

short mags[3]; // magnetometer readings for the 3 axes

short temp;

int main() {
    setup();
    
// read the accelerometer from all three axes

// the accelerometer and the pic32 are both little endian by default (the lowest address has the LSB)

// the accelerations are 16-bit twos compliment numbers, the same as a short
    
    
    char msg[10];

    int xScale;
    int yScale;
    int i;
    
    
   
    while (1) {   
        // read the accelerometer from all three axes

// the accelerometer and the pic32 are both little endian by default (the lowest address has the LSB)

// the accelerations are 16-bit twos compliment numbers, the same as a short

        acc_read_register(OUT_X_L_A, (unsigned char *) accels, 6);

// need to read all 6 bytes in one transaction to get an update.

        acc_read_register(OUT_X_L_M, (unsigned char *) mags, 6);

// read the temperature data. Its a right justified 12 bit two's compliment number

        acc_read_register(TEMP_OUT_L, (unsigned char *) &temp, 2);
        xScale = accels[0]+32000;
        xScale = xScale/64000.0 * 128;
        xScale = 128 - xScale;
        
        yScale = accels[1]+ 32000;
        yScale = yScale/64000.0 * 64;
        yScale = 64-yScale;
        display_clear();
        if(xScale < 64){
            for(i = 64; i>= xScale; i--){
                display_pixel_set(31,i, 1);
                display_pixel_set(32,i, 1); 
                display_pixel_set(33,i, 1); 
            }  
        }
        if(xScale >= 64){
            for(i = 64; i<= xScale; i++){
                display_pixel_set(31,i, 1); 
                display_pixel_set(32,i, 1); 
                display_pixel_set(33,i, 1); 
            }  
        }
        
        if(yScale < 32){
            for(i = 32; i >= yScale; i--){
                display_pixel_set(i,63, 1);
                display_pixel_set(i,64, 1); 
                display_pixel_set(i,65, 1); 
            }
        }
        if(yScale >= 32){
            for(i = 32; i<= yScale; i++){
                display_pixel_set(i,63, 1);
                display_pixel_set(i,64, 1); 
                display_pixel_set(i,65, 1); 
            }
            
        }
        display_draw();

      //  sprintf(msg,"X: %d  ", xScale);
       // write_string(msg,10,1);
    /*
     6 for sda
     7 for cs
     bottom left for sdo */
   
        
    }
    
    
    
}

