#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif
#include "ascii.h"

// ASCII characters from darkest to brightest - from reference image
static const char ASCII_CHARS[] = " .-=+*X#$&@";
static const int ASCII_CHARS_LEN = sizeof(ASCII_CHARS) - 1;

// Extended character set for more detail
static const char ASCII_CHARS_DETAILED[] = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
static const int ASCII_CHARS_DETAILED_LEN = sizeof(ASCII_CHARS_DETAILED) - 1;

char brightness_to_ascii(unsigned char brightness, int use_detailed) {
    if (use_detailed) {
        int index = (brightness * ASCII_CHARS_DETAILED_LEN) / 256;
        return ASCII_CHARS_DETAILED[index];
    } else {
        int index = (brightness * ASCII_CHARS_LEN) / 256;
        return ASCII_CHARS[index];
    }
}

// Sobel edge detection - returns edge magnitude
unsigned char* apply_edge_detection(unsigned char* data, int width, int height, int channels) {
    unsigned char* edges = (unsigned char*)malloc(width * height);
    if (!edges) return NULL;
    
    // Sobel kernels (as shown in reference images)
    int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int gx = 0, gy = 0;
            
            // Apply convolution
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int px = x + kx;
                    int py = y + ky;
                    
                    // Handle borders
                    if (px < 0) px = 0;
                    if (px >= width) px = width - 1;
                    if (py < 0) py = 0;
                    if (py >= height) py = height - 1;
                    
                    int idx = (py * width + px) * channels;
                    
                    // Convert to grayscale
                    unsigned char r = data[idx];
                    unsigned char g = (channels > 1) ? data[idx + 1] : r;
                    unsigned char b = (channels > 2) ? data[idx + 2] : r;
                    unsigned char gray = (unsigned char)(0.2126f * r + 0.7152f * g + 0.0722f * b);
                    
                    gx += sobel_x[ky + 1][kx + 1] * gray;
                    gy += sobel_y[ky + 1][kx + 1] * gray;
                }
            }
            
            // Calculate gradient magnitude
            int magnitude = (int)sqrt((float)(gx * gx + gy * gy));
            if (magnitude > 255) magnitude = 255;
            
            edges[y * width + x] = (unsigned char)magnitude;
        }
    }
    
    return edges;
}

int get_terminal_width(void) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
#endif
    return 80;  // Default fallback
}

void print_ascii_art(unsigned char* data, int width, int height, int channels, AsciiMode mode) {
    int terminal_width = get_terminal_width();
    int padding = (terminal_width > width) ? (terminal_width - width) / 2 : 0;
    
    // Apply edge detection if needed
    unsigned char* edges = NULL;
    if (mode == MODE_EDGES || mode == MODE_HYBRID) {
        edges = apply_edge_detection(data, width, height, channels);
        if (!edges) mode = MODE_BRIGHTNESS; // Fallback if edge detection fails
    }
    
    for (int y = 0; y < height; y++) {
        // Add padding for center alignment
        for (int p = 0; p < padding; p++) {
            printf(" ");
        }
        
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * channels;
            
            // Calculate brightness (grayscale)
            unsigned char r = data[idx];
            unsigned char g = (channels > 1) ? data[idx + 1] : r;
            unsigned char b = (channels > 2) ? data[idx + 2] : r;
            
            // Enhanced luminance formula (as per reference)
            float luminance = 0.2126f * r + 0.7152f * g + 0.0722f * b;
            
            unsigned char brightness;
            int use_detailed = 0;
            
            switch (mode) {
                case MODE_EDGES:
                    // Pure edge mode - show only edges
                    brightness = edges[y * width + x];
                    use_detailed = 1;
                    break;
                    
                case MODE_HYBRID:
                    // Hybrid mode - combine brightness and edges
                    {
                        unsigned char edge_val = edges[y * width + x];
                        // Weight: 70% brightness, 30% edges for detail enhancement
                        float combined = luminance * 0.7f + edge_val * 0.3f;
                        
                        // Apply contrast enhancement
                        float contrast = 1.4f;
                        combined = ((combined / 255.0f - 0.5f) * contrast + 0.5f) * 255.0f;
                        
                        if (combined < 0) combined = 0;
                        if (combined > 255) combined = 255;
                        
                        brightness = (unsigned char)combined;
                        use_detailed = 1;
                    }
                    break;
                    
                case MODE_BRIGHTNESS:
                default:
                    // Standard brightness mode with contrast boost
                    {
                        float contrast = 1.3f;
                        luminance = ((luminance / 255.0f - 0.5f) * contrast + 0.5f) * 255.0f;
                        
                        if (luminance < 0) luminance = 0;
                        if (luminance > 255) luminance = 255;
                        
                        brightness = (unsigned char)luminance;
                        use_detailed = 0; // Use simple character set
                    }
                    break;
            }
            
            // Get ASCII character for this brightness
            char ascii_char = brightness_to_ascii(brightness, use_detailed);
            
            // Print with ANSI 24-bit true color
            if (channels >= 3) {
                // Color boost for vibrant output
                int r_boost = (r < 230) ? r + (255 - r) / 8 : r;
                int g_boost = (g < 230) ? g + (255 - g) / 8 : g;
                int b_boost = (b < 230) ? b + (255 - b) / 8 : b;
                printf("\033[38;2;%d;%d;%dm%c", r_boost, g_boost, b_boost, ascii_char);
            } else {
                printf("%c", ascii_char);
            }
        }
        printf("\033[0m\n");  // Reset color at end of line
    }
    
    // Cleanup
    if (edges) free(edges);
}
