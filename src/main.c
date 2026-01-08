#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "ascii.h"

int main(int argc, char** argv) {
    const char* image_path = "assets/sample.jpg";
    int target_width = 80;  // Balanced size for detail visibility
    AsciiMode mode = MODE_HYBRID;  // Default to hybrid mode for best results
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'w' && i + 1 < argc) {
            target_width = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--mode") == 0 && i + 1 < argc) {
            i++;
            if (strcmp(argv[i], "brightness") == 0) {
                mode = MODE_BRIGHTNESS;
            } else if (strcmp(argv[i], "edges") == 0) {
                mode = MODE_EDGES;
            } else if (strcmp(argv[i], "hybrid") == 0) {
                mode = MODE_HYBRID;
            }
        } else if (argv[i][0] != '-') {
            image_path = argv[i];
        }
    }
    
    int width, height, channels;
    unsigned char* data = load_image(image_path, &width, &height, &channels);

    if (!data) {
        printf("Failed to load image: %s\n", image_path);
        return 1;
    }

    printf("Loaded image: %dx%d (%d channels)\n", width, height, channels);
    
    // Calculate target height with artistic aspect ratio
    // Slightly adjusted ratio for a classier look (0.45 instead of 0.5)
    int target_height = (int)((float)height / width * target_width * 0.45f);
    
    const char* mode_name = (mode == MODE_BRIGHTNESS) ? "Brightness" : 
                           (mode == MODE_EDGES) ? "Edges" : "Hybrid";
    
    printf("Resizing to: %dx%d for terminal display\n", target_width, target_height);
    printf("Mode: %s\n", mode_name);
    printf("Generating ASCII art...\n\n");
    
    // Resize image for ASCII art
    unsigned char* resized = resize_image(data, width, height, 
                                         target_width, target_height, channels);
    
    if (!resized) {
        printf("Failed to resize image\n");
        free_image(data);
        return 1;
    }
    
    // Print ASCII art with the resized image
    print_ascii_art(resized, target_width, target_height, channels, mode);
    
    // Cleanup
    free(resized);
    free_image(data);
    
    return 0;
}



