/*
 * Project 1-1: Quadtree Compression
 *
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    int i = 0;
    unsigned char *start = depth_map;
    int pixel_count = section_width^2;
    while(pixel_count >= 3){
        if(*depth_map == *start){
            if(++i == section_width){
                depth_map = depth_map + map_width - section_width;
            }
            depth_map++;
            pixel_count--;
        }
        else
            return -1;
    }
    return *start;
}

qNode *create_tree(int x, int y, unsigned char *depth_map, int map_width, int section_width){
    qNode* node = malloc(sizeof(qNode));
    if(!node)
        return NULL;
    node->x = x;
    node->y = y;
    node->size = section_width;
    
    if(homogenous(depth_map, map_width, x, y, section_width)== -1){
        node->leaf = 0;
        node->gray_value = -1;
        node->child_NE = create_tree(x + section_width / 2, y, depth_map, map_width, section_width/2);
        node->child_NW = create_tree(x, y, depth_map, map_width, section_width/2);
        node->child_SE= create_tree(x + section_width/2, y + section_width/2, depth_map, map_width, section_width/2);
        node->child_SW = create_tree(x, y + section_width/2, depth_map, map_width, section_width/2);
    }
    else{
        node->gray_value = *(depth_map + y * map_width + x);
    }
    return node;
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {
    int x = 0;
    int y = 0;
    qNode *node = create_tree(x, y, depth_map, map_width, map_width);
    return node;
    return NULL;
    /* YOUR CODE HERE */
}

void free_qtree(qNode *qtree_node) {
    if(qtree_node){
        if(qtree_node->leaf)
            free(qtree_node);
        else{
            free_qtree(qtree_node->child_NE);
            free_qtree(qtree_node->child_SE);
            free_qtree(qtree_node->child_NW);
            free_qtree(qtree_node->child_SW);
        }
    }
    /* YOUR CODE HERE */
}
