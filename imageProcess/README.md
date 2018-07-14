# image_process
It is the c++ library for image processing. 
I try to write them like openCV, so they are lots of object. 
Each class has a corresponding header and its cpp file. 
in the following, i will breifly introduce each class and its main function

## PIC
For image (pixel value is unsigned char <0-255>).
Main method: resize, crop, color space change, contrast enhancement, denoising(Non-local Mean), homographic transform

## FILTER
For filter kernel, namely mask (pixel value is float)
Main method: create Guassion filter kernel

## POINT_2D
Point with two coordinate

## utility 
it is not the class, it just the header store common function for each class
Main function: get two picture's dissimilarity (the distance between two picture)
               solve linear equation (Ax=b, given matrix A and vector b, it get you vector x)
               determine the points is in the region or not
