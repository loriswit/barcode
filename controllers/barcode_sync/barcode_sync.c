#include <stdio.h>
#include <stdlib.h>
#include <webots/robot.h>
#include <time.h>

#include "../util/motors.h"
#include "../util/ground_sensors.h"
#include "../util/consts.h"

#define MOTOR_SPEED     1
#define COLOR_THRESHOLD 500

#define color_t bool
#define black   true
#define white   false

#define SYNC_STEPS  5

clock_t sync(color_t input)
{
    static int step = -1;
    static clock_t ticks[SYNC_STEPS];
    static clock_t start;
    static color_t last_input = white;
    
    if(input != last_input)
    {
        if(step >= 0)
        {
            ticks[step] = clock() - start;
            printf("step %d: %ld ticks\n", step + 1, ticks[step]);
        }
        
        ++step;
        start = clock();
        last_input = input;
    }
    
    if(step == SYNC_STEPS)
    {
        clock_t average = 0;
        for(size_t i = 0; i < SYNC_STEPS; ++i)
            average += ticks[i];
        
        return average / SYNC_STEPS;
    }
    else
        return -1;
}

color_t get_input()
{
    double ground_value =
            ground_get_value(GROUND_LEFT) +
            ground_get_value(GROUND_CENTER) +
            ground_get_value(GROUND_RIGHT);
    
    ground_value /= GROUND_COUNT;
    return ground_value < COLOR_THRESHOLD;
}

int main()
{
    wb_robot_init();
    motors_init();
    ground_init();
    
    motors_set_speed(MOTOR_SPEED, MOTOR_SPEED);
    
    bool synced = false;
    clock_t step_start = 0;
    clock_t ticks_per_step = 0;
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        color_t input = get_input();
        
        if(!synced)
            ticks_per_step = sync(input);
        
        if(!synced && ticks_per_step > 0)
        {
            synced = true;
            step_start = clock() + ticks_per_step / 2;
            printf("ticks/step: %ld\n", ticks_per_step);
        }
        
        if(synced)
        {
            clock_t step_time = clock() - step_start;
            if(step_time >= ticks_per_step)
            {
                step_start = clock();
                printf("%s\n", input == black ? "black" : "white");
            }
        }
    }
    
    motors_stop();
    wb_robot_cleanup();
    
    return EXIT_SUCCESS;
}
