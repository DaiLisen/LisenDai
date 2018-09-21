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


void calc_depth(unsigned char *depth_map, unsigned char *left,
                unsigned char *right, int image_width, int image_height,
                int feature_width, int feature_height, int maximum_displacement) {
    int x = 0;
    int y = 0;
    int lx = 0;  //low x
    int hx = 0;  //high x
    int ly = 0;
    int hy = 0;
    for (int i = 0; i < image_width * image_height; i++) {  //i是左边image的第i个值，对应坐标就是小绿                                       点的坐标green
        y = i / image_width;   //行
        x = i % image_width;   //列  （y，x）绿点坐标
        
        if (maximum_displacement == 0) {
            depth_map[i] = 0;
        }
        else if (x - feature_width < 0 || x + feature_width > image_width - 1 || y - feature_height < 0 || y + feature_height > image_height - 1) { //on the edge of the image
            depth_map[i] = 0;
        }
        else {
            if (x - maximum_displacement < 0) {   //when ==0, it is on the edge.the x=0.so it should be <0.
                lx = 0;
            }
            else {
                lx = x - maximum_displacement;//low xy和high xy确定了粉色框的边界 通过max displacement
            }
            if ( x + maximum_displacement > image_width - 1) {
                hx = image_width - 1;
            }
            else {
                hx = x + maximum_displacement;
            }
            if (y - maximum_displacement < 0) {
                ly = 0;
            }
            else {
                ly = y - maximum_displacement;
            }
            if (y + maximum_displacement > image_height - 1) {
                hy = image_height - 1;
            }
            else {
                hy = y + maximum_displacement;
            }
            int st_x = -1;
            int st_y = -1;
            unsigned int euclidean = -1;
            for (int j = ly; j <= hy; j++) {  //j k是求和的界限
                for (int k = lx; k <= hx; k++) {
                    int sum = 0;  //减去feature——width之后就是针对小红点的红色方框的边界值
                    if (k - feature_width < 0 || k + feature_width > image_width - 1 || j - feature_height < 0 || j + feature_height > image_height - 1) {
                        continue;  //跳出循环进入下一个，k++ when feature is out of the iamge
                    }
                    for (int f = -feature_height; f <= feature_height; f++) {
                        for (int m = -feature_width; m <= feature_width; m++) {
                            sum += (left[(y + f) * image_width + (x + m)] - right[(j + f) * image_width + (k + m)])^2;  //(left[]-right[])^2求和 找在1d memory中这个点对应的值
                        }
                    }
                    if (sum < euclidean) {  //寻找最小值，及其对应的坐标
                        euclidean = sum;
                        st_x = k;
                        st_y = j;//the smallest ywe want to get
                    }
                    else if (sum == euclidean) {  //两个点对应结果相同时，看哪个归一化最小
                        if (normalized_displacement(x - k, y - j, maximum_displacement) < normalized_displacement(x - st_x, y - st_y, maximum_displacement)) {
                            st_y = j;
                            st_x = k;
                        }
                    }
                } //for(k)
            }  //for(j)
            depth_map[i] = normalized_displacement(x - st_x, y - st_y, maximum_displacement);
        }
    }  //for
}
