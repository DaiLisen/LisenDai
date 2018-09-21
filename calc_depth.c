/*
 * Project 1-1: Computing a Displacement Map
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
                                      int maximum_displacement) {
    
    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement) / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;
    
}

int squared_euclidean(int width, int height, unsigned char *a, unsigned char *b) {
    int d = 0;
    int counter = 0;
    int size = (2 * width + 1) * (2 * height + 1);
    while(counter++ < size){
        d += (*a++ - *b++)^2;
    }
    return d;
}

int low_bondary(int x, int maximum_displacement){
    int x_min;
    if(x < maximum_displacement)
        x_min = 0;
    else
        x_min = x - maximum_displacement;
    return x_min;
}

int high_bondary(int x, int maximum_displacement, int image_width){
    int x_max;
    if (x + maximum_displacement > image_width - 1) {
        x_max = image_width - 1;
    }
    else{
        x_max = x + maximum_displacement;
    }
    return x_max;
}

void calc_depth(unsigned char *depth_map, unsigned char *left,
                unsigned char *right, int image_width, int image_height,
                int feature_width, int feature_height, int maximum_displacement) {
    int x = 0;
    int y = 0;
    int x_max = 0;
    int x_min = 0;
    int y_max = 0;
    int y_min = 0;
    int i;
    int image_size = image_width * image_width;
    for(i = 0; i < image_size; i++){
        y = i / image_width;
        x = i % image_width; // (y, x)
        if (maximum_displacement == 0)
            *depth_map = 0;
        else if (x < feature_width || y < feature_height || x + feature_width >= image_width || y + feature_height >= image_width)
            *depth_map = 0;
        else{
            x_min = low_bondary(x, maximum_displacement);
            y_min = low_bondary(y, maximum_displacement);
            x_max = high_bondary(x, maximum_displacement, image_width);
            y_max = high_bondary(y, maximum_displacement, image_width);
        }
        int st_x = -1;
        int st_y = -1;
        unsigned int euclidean = -1;
        for (int j = y_min; j <= y_max; j++) {
            for (int k = x_min; k <= x_max; k++) {
                int sum = 0;
                if (k - feature_width < 0 || k + feature_width > image_width - 1 || j - feature_height < 0 || j + feature_height > image_height - 1) {
                    continue;
                }
                for (int f = -feature_height; f <= feature_height; f++) {
                    for (int m = -feature_width; m <= feature_width; m++) {
                        sum += (left[(y + f) * image_width + (x + m)] - right[(j + f) * image_width + (k + m)])^2;
                    }
                }
                if (sum < euclidean) {
                    euclidean = sum;
                    st_x = k;
                    st_y = j;
                }
                else if (sum == euclidean) {
                    if (normalized_displacement(x - k, y - j, maximum_displacement) < normalized_displacement(x - st_x, y - st_y, maximum_displacement)) {
                        st_y = j;
                        st_x = k;
                    }
                }
            } 
        }
        depth_map[i] = normalized_displacement(x - st_x, y - st_y, maximum_displacement);
    }
    /* YOUR CODE HERE */
}
