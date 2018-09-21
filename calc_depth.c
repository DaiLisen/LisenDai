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



void depth_finder(int x, int y, int x_min, int x_max, int y_min, int y_max, unsigned char *depth_map, unsigned char *left, unsigned char *right, int image_width, int image_height, int feature_width, int feature_height,int maximum_displacement){
    int x0 = -1;
    int y0 = -1;
    int sum = 0;
    int h = - feature_height;
    int w = - feature_width;
    unsigned int euclidean = -1;
    while(y_min++ <= y_max){
        while(x_min++ <= x_max){
            if (x_min < feature_width || y_min - feature_height < 0 || x_min + feature_width > image_width - 1 || y_min + feature_height > image_height - 1) {
                continue;
            }
            while(h++ <= feature_height) {
                while(w++ <= feature_width) {
                    sum += (left[(y + h) * image_width + (x + w)] - right[(y_min + w) * image_width + (x_min + w)]) * (left[(y + h) * image_width + (x + w)] - right[(y_min + w) * image_width + (x_min + w)]);
                }
            }
            if (sum < euclidean) {
                euclidean = sum;
                x0 = x_min - 1;
                y0 = y_min - 1;
            }
            else if (sum == euclidean) {
                if (normalized_displacement(x - x_min, y - y_min, maximum_displacement) < normalized_displacement(x - y0, y - y0, maximum_displacement)) {
                    y0 = y_min;
                    x0 = x_min;
                }
            }
        }
    }
    *depth_map = normalized_displacement(x - x0, y - y0, maximum_displacement);
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
        depth_finder(x, y, x_min, x_max, y_min, y_max, depth_map, left, right, image_width, image_height,  feature_width, feature_height, maximum_displacement);
        depth_map++;
    }
    /* YOUR CODE HERE */
}
