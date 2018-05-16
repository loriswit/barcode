#ifndef BARCODE_BARCODE_H
#define BARCODE_BARCODE_H

#include <stdbool.h>

#define color_t bool
#define black   true
#define white   false

/**
 * Returns the current color detected by the ground sensors.
 *
 * @return <b>black</b> if the color is below the threshold, <b>white</b> if not.
 */
color_t get_color();

/**
 * Returns a string containing the name of a color.
 *
 * @param color The input color
 * @return "black" or "white" according to the input color
 */
char * col2str(color_t color);

/**
 * Synchronises the robot with the bars spacing.
 *
 * @return The average elapsed ticks per bar.
 */
double sync();

#endif //BARCODE_BARCODE_H
