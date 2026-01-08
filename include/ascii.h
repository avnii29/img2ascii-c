#ifndef ASCII_H
#define ASCII_H

// ASCII art modes
typedef enum {
    MODE_BRIGHTNESS,    // Standard brightness mapping
    MODE_EDGES,        // Edge detection only
    MODE_HYBRID        // Brightness + edges combined
} AsciiMode;

// Convert brightness (0-255) to ASCII character
char brightness_to_ascii(unsigned char brightness, int use_detailed);

// Print colored ASCII representation of an image
void print_ascii_art(unsigned char* data, int width, int height, int channels, AsciiMode mode);

// Get terminal width for centering
int get_terminal_width(void);

// Apply edge detection to image
unsigned char* apply_edge_detection(unsigned char* data, int width, int height, int channels);

#endif
