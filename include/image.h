#ifndef IMAGE_H
#define IMAGE_H

unsigned char* load_image(const char* path, int* width, int* height, int* channels);
void free_image(unsigned char* data);
unsigned char* resize_image(unsigned char* data, int orig_width, int orig_height, 
                           int new_width, int new_height, int channels);

#endif
