/*
 * PROJ1-1: YOUR TASK A CODE HERE
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

int squared_euclidean(int feature_width, int feature_height, 
		unsigned char *left_feature, unsigned char *right_feature) {
	int distance = 0;
	int current_length = 0;
	int feature_length = (2 * feature_width + 1) * (2 * feature_height + 1);
	while(current_length < feature_length){
		int diff = (*left_feature - *right_feature);
		left_feature++; right_feature++;
		distance += diff * diff;
		current_length++;
	}
	return distance;
}


int Two2OneD(int x, int y, int width){
	return width * y + x;
}

void extract_box(unsigned char *box, unsigned char *pic, int searchX, int searchY, int box_width, int box_height, int pic_width, int pic_height) {
	int curr_size = 0, colI = 0;
	int currX = searchX - (box_width - 1)/2;
	int currY = searchY - (box_height - 1)/2; 
	int size = box_width * box_height;
	unsigned char *original_pic = pic;
	pic +=  Two2OneD(currX, currY, pic_width);
	for( ; curr_size < size ; curr_size++){
		if( colI  >  box_width - 1){
			colI  = 0;
			//pic += (pic_width - currX) + searchX + 1;  
			currX = searchX - (box_width - 1)/2; 
			currY++;
			pic = original_pic + Two2OneD(currX, currY, pic_width);
		}			
		if( (currX < 0) | (currY < 0) | (currY > pic_height)) {
			*box = 0;
		}
		*box = *pic;
		colI++; currX++; pic++; box++;
	}
}

// x and y are coords of  pixels of center of search space 
unsigned char scanFeatureRight(unsigned char *left_feature, unsigned char *right_image, int right_image_width, int right_image_height, int feature_width, int feature_height, int x, int y, int max_displacement){
	int curr_feature = 0;
	int max_features = (2 * max_displacement + 1) * (2 * max_displacement + 1);
	int startX = x - max_displacement;
	int startY = y - max_displacement; 
	int matchX; int matchY; int distance;
	int smallestDistance = 0;
	unsigned char smallestNorm = 0;
	unsigned char currNorm;
	int enter = 1;
	unsigned char testNORM = 0;
	unsigned char right_feature[(2 * feature_width + 1) * (2*feature_height+1)];
	for( ; curr_feature < max_features; curr_feature++){ 
		if( startX > x + (max_displacement)){
			startX = x - max_displacement;
			startY++;
		}
		if( (startX  - feature_width < 0) | (startY > right_image_height)) {
			startX++;
			continue;
		}
		if( ((startX + feature_width) > right_image_width - 1) | ((startY + feature_height) > right_image_height - 1)){
			startX++;
			continue;
		}  
		testNORM =  normalized_displacement(x - startX, y - startY, max_displacement); 
		extract_box(right_feature, right_image, startX, startY, 2 * feature_width + 1,
			 2 * feature_height + 1,  right_image_width, right_image_height);
	 	distance = squared_euclidean(feature_width, feature_height, left_feature, right_feature);
		if( startY - feature_height < 0 ){
			startX++;
			continue;
		}
		if( (enter) | (distance < smallestDistance)){
			smallestDistance = distance;
			matchX = startX; matchY = startY;
			smallestNorm = normalized_displacement(x - matchX, y - matchY, max_displacement);
			enter = 0;
		 } else if(distance == smallestDistance){
			currNorm = normalized_displacement(x - startX, y - startY, max_displacement); 
			if ( currNorm < smallestNorm ){
				matchX = startX; matchY = startY;
				smallestNorm = currNorm;
		 	}
		 }
		startX++;	
	}
	return normalized_displacement(x - matchX, y - matchY, max_displacement);
}
void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {
	//Variables to track the size of dimensions
	//-----------------------------------------
	//  x * y of the feature
	int feature_box_width = 2 * feature_width + 1;
	int feature_box_height = 2 * feature_height + 1;
	int	image_size = image_width * image_height;
	int x = 0, y = 0, curr_size = 0;
	// for (int i = 0; i < image_size; i++){
	// 	printf("%d", left[i]);
	// }
	// printf("END OF IMAGE\n\n\n");	
	for( ; curr_size < image_size; curr_size++){
		if( x > image_width - 1){
			y++;
			x = 0;
		}
		if( ((x - feature_width) < 0) | ((y - feature_height) < 0) ) {
			*depth_map = 0;
		}
		else if( ( (x + feature_width) > image_width - 1 ) | ((y + feature_height) > image_height - 1)){		
			*depth_map = 0;
		}
		else if( ( (y - feature_height) < 0 )){
			*depth_map = 0;
		}
		else{  
			unsigned char leftFeature[feature_box_width * feature_box_height];
			extract_box(leftFeature, left, x, y, feature_box_width, feature_box_height,
					image_width,  image_height);
			unsigned char norm_distance = scanFeatureRight(leftFeature, right, image_width, image_height, feature_width, feature_height, x, y, maximum_displacement);
			*depth_map = norm_distance;
		}
		x++; depth_map++;
	}	

}


