#include <stdio.h>
#include <stdlib.h>

// Must be in exactly **one .c file**
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "image.h"   // <-- main.c will also include this

unsigned char* load_image(
    const char* path,
    int* width,
    int* height,
    int* channels
) {
    unsigned char* data = stbi_load(path, width, height, channels, 0);
    if (!data) {
        fprintf(stderr, "stb_image error: %s\n", stbi_failure_reason());
        return NULL;
    }
    return data;
}

void free_image(unsigned char* data) {
    if (data) stbi_image_free(data);
}

// Simple bilinear resize
unsigned char* resize_image(unsigned char* data, int orig_width, int orig_height, 
                           int new_width, int new_height, int channels) {
    unsigned char* resized = (unsigned char*)malloc(new_width * new_height * channels);
    if (!resized) return NULL;
    
    float x_ratio = (float)orig_width / new_width;
    float y_ratio = (float)orig_height / new_height;
    
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            int px = (int)(x * x_ratio);
            int py = (int)(y * y_ratio);
            
            // Clamp to image bounds
            if (px >= orig_width) px = orig_width - 1;
            if (py >= orig_height) py = orig_height - 1;
            
            int src_idx = (py * orig_width + px) * channels;
            int dst_idx = (y * new_width + x) * channels;
            
            for (int c = 0; c < channels; c++) {
                resized[dst_idx + c] = data[src_idx + c];
            }
        }
    }
    
    return resized;
}