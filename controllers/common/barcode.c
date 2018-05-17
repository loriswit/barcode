#include <stdio.h>

#include <webots/robot.h>

#include "../util/ground_sensors.h"
#include "../util/leds.h"
#include "../util/consts.h"

#include "barcode.h"
#include "helpers.h"

#define COLOR_THRESHOLD 700
#define SYNC_STEPS  5

#define REMOTE_MULTIPLIER   0.9

color_t get_color()
{
    double ground_value =
            ground_get_value(GROUND_LEFT) +
            ground_get_value(GROUND_CENTER) +
            ground_get_value(GROUND_RIGHT);
    
    ground_value /= GROUND_COUNT;
    return ground_value < COLOR_THRESHOLD;
}

char * col2str(color_t color)
{
    return color == black ? "black" : "white";
}

double sync()
{
    unsigned step = 0;
    double start = 0;
    double times[SYNC_STEPS];
    color_t last_input = white;
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        color_t input = get_color();
        if(input != last_input)
        {
            if(!step)
            {
                println("Synchronising...");
                leds_set(true);
            }
            else
                times[step - 1] = now() - start;
            
            ++step;
            start = now();
            last_input = input;
        }
        
        if(step > SYNC_STEPS)
        {
            double average = 0;
            for(size_t i = 0; i < SYNC_STEPS; ++i)
                average += times[i];
            
            double time_per_step = average / SYNC_STEPS;
            if(wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL)
                time_per_step *= REMOTE_MULTIPLIER;
            
            println("Done! (time/step: %.3f s)", time_per_step);
            leds_set(false);
            return time_per_step;
        }
    }
    
    println("Failed!");
    leds_set(false);
    return -1;
}
