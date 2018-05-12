#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <webots/robot.h>

#include "../util/motors.h"
#include "../util/ground_sensors.h"
#include "../util/consts.h"

#include "../common/barcode.h"

#define MOTOR_SPEED     2.0

// rotate 135° left, turn LED 4 on, wait for 3 seconds
// 2, 2, 4, 4, 6, 2 (18, 36, 22)
#define TEST_STEPS 21
const color_t test_values[TEST_STEPS] =
        {white, black, white, white, black, white, white,
         white, black, white, white, black, white, black,
         black, white, black, white, black, black, black};

int main()
{
    wb_robot_init();
    motors_init();
    ground_init();
    
    motors_set_speed(MOTOR_SPEED, MOTOR_SPEED);
    
    const clock_t ticks_per_step = sync();
    clock_t step_start = clock() + ticks_per_step / 2;
    
    unsigned step = 0;
    unsigned test_count = 0;
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        if(clock() - step_start >= ticks_per_step)
        {
            step_start = clock();
            color_t input = get_color();
            bool ok = input == test_values[step];
            
            printf("%s = %s", col2str(input), col2str(test_values[step]));
            printf(" -> %s\n", ok ? "ok" : "failed");
            
            ++step;
            if(ok)
                ++test_count;
        }
        
        if(step == TEST_STEPS)
            break;
    }
    
    motors_stop();
    wb_robot_cleanup();
    
    printf("%s ", test_count == TEST_STEPS ? "Test passed!" : "Test failed!");
    printf("(%d/%d)\n", test_count, TEST_STEPS);
    printf("motor speed: %.1f rad/s\n", MOTOR_SPEED);
    printf("ticks/step: %ld\n", ticks_per_step);
    
    return EXIT_SUCCESS;
}
