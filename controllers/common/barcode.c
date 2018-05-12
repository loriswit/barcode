#include <stdio.h>
#include <time.h>

#include <webots/robot.h>

#include "../util/ground_sensors.h"
#include "../util/consts.h"

#include "barcode.h"
#include "helpers.h"

#define COLOR_THRESHOLD 700
#define SYNC_STEPS  5

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

clock_t sync()
{
    unsigned step = 0;
    clock_t start = 0;
    clock_t ticks[SYNC_STEPS];
    color_t last_input = white;
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        color_t input = get_color();
        if(input != last_input)
        {
            if(!step)
                println("Synchronising...");
            else
                ticks[step - 1] = clock() - start;
            
            ++step;
            start = clock();
            last_input = input;
        }
        
        if(step > SYNC_STEPS)
        {
            clock_t average = 0;
            for(size_t i = 0; i < SYNC_STEPS; ++i)
                average += ticks[i];
            
            clock_t ticks_per_step = average / SYNC_STEPS;
            println("Done! (ticks/step: %ld)", ticks_per_step);
            return ticks_per_step;
        }
    }
    
    println("Failed!");
    return -1;
}
