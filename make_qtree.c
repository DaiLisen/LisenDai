/*/*
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
    if (map_width == section_width){
        unsigned char check_pixel = depth_map[0];
        for (int i = 0; i < map_width*map_width; i++){
            if (depth_map[i] != check_pixel){
                return 256;
            }
        }
        return check_pixel;
    }else{
        int start_value = y * map_width + x;
        unsigned char check_pixel = depth_map[start_value];
        int index_count = 0;
        int check_count = section_width*section_width;
        while (check_count > 2){
            if (depth_map[start_value] != check_pixel){
                return 256;
            }
            index_count++;
            if (index_count == section_width){
                start_value = start_value - section_width + map_width;
            }
            check_count--;
            start_value++;
        }
        return check_pixel;
    }
}

qNode *recursive_quad(unsigned char *depth_map, int map_width, int x, int y, int section_width){
    qNode* node = malloc(sizeof(qNode));
    if (homogenous(depth_map, map_width, x, y, section_width) != 256){
        node->leaf = 1;
        node->size = section_width;
        node->x = x;
        node->y = y;
        node->gray_value = depth_map[y*map_width + x];
    }else{
        node->leaf = 0;
        node->size = section_width;
        node->x = x;
        node->y = y;
        node->gray_value = 256;
        section_width = section_width / 2;
        node->child_NW = recursive_quad(depth_map, map_width, x, y, section_width);
        node->child_NE = recursive_quad(depth_map, map_width, x + section_width, y, section_width);
        node->child_SE = recursive_quad(depth_map, map_width, x + section_width, y + section_width, section_width);
        node->child_SW = recursive_quad(depth_map, map_width, x, y+section_width, section_width);
    }
    return node;
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {
    int start_x = 0;
    int start_y = 0;
    qNode* quadtree = recursive_quad(depth_map, map_width, start_x, start_y, map_width);
    return quadtree;
}


void free_qtree(qNode *qtree_node) {
    if(qtree_node) {
        if(!qtree_node->leaf){
            free_qtree(qtree_node->child_NW);
            free_qtree(qtree_node->child_NE);
            free_qtree(qtree_node->child_SE);
            free_qtree(qtree_node->child_SW);
        }
        free(qtree_node);
    }
}
