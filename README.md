# img2ascii-c

A terminal-based C program that converts images into colored ASCII art using brightness mapping and ANSI escape codes.

The project loads an image, resizes it to fit the terminal, maps pixel brightness to ASCII characters, and renders the result with optional color and edge enhancement.

---

## Features

- Load images (PNG, JPEG, etc.)
- Resize images for terminal display (aspect-ratio aware)
- **3 Rendering Modes:**
  - **Brightness Mode**: Classic brightness-to-ASCII mapping
  - **Edge Mode**: Sobel edge detection for outlines
  - **Hybrid Mode**: Combines brightness + edges for maximum detail (default)
- Character gradient based on reference standards: ` .-=+*X#$&@`
- Extended detailed character set for high-quality output
- ANSI 24-bit true color terminal output
- Center-aligned output
- Contrast enhancement for minute detail visibility
- Modular C codebase

---

## Demo

Example output (terminal):

@@@%%%###***+++===---:::...
*(Adding ss later)*

---

## Project Structure
```
├── src/
│ ├── main.c # Program entry point and CLI handling
│ ├── image.c # Image loading and resizing
│ ├── ascii.c # ASCII and color mapping logic
│ └── edges.c # Edge detection (optional)
│
├── include/
│ ├── image.h
│ ├── ascii.h
│ ├── edges.h
│ └── stb_image.h # Third-party image loader
│
├── examples/
│ └── sample.jpg
│
├── Makefile
├── README.md
└── LICENSE
```


## Build

Requirements:
- GCC or compatible C compiler
- Terminal with ANSI color support (Windows Terminal, PowerShell, or Unix-like terminal)

### Windows
```bash
# Build the project
build.bat

# Run with default sample image (hybrid mode)
.\img2ascii.exe

# Run with custom image
.\img2ascii.exe path\to\your\image.jpg

# Specify width
.\img2ascii.exe -w 100 path\to\your\image.jpg

# Choose rendering mode
.\img2ascii.exe --mode brightness    # Classic brightness mapping
.\img2ascii.exe --mode edges         # Edge detection only
.\img2ascii.exe --mode hybrid        # Combined (default, best quality)
```

### Linux/Mac
```bash
# Build using Makefile
make

# Run
./build/img2ascii assets/sample.jpg
```


Planned Options
```
-w <width> : Set output width
--no-color : Disable ANSI color output
--no-edges : Disable edge detection
```

## How It Works 

1. **Load Image**: Decode image into RGB pixel buffer using stb_image
2. **Resize**: Scale image to target dimensions (aspect-ratio aware)
3. **Edge Detection** (Hybrid/Edge modes): Apply Sobel convolution kernels
   - Horizontal gradient: `[[-1,0,1], [-2,0,2], [-1,0,1]]`
   - Vertical gradient: `[[-1,-2,-1], [0,0,0], [1,2,1]]`
   - Calculate magnitude: `sqrt(gx² + gy²)`
4. **Brightness Calculation**: Convert RGB to grayscale using luminance formula
   - `Y = 0.2126*R + 0.7152*G + 0.0722*B`
5. **Contrast Enhancement**: Apply contrast boost for detail visibility
6. **Character Mapping**: Map intensity to ASCII character gradient
   - Simple: ` .-=+*X#$&@`
   - Detailed: ` .'^\`",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$`
7. **Color Rendering**: Apply ANSI 24-bit true color codes
8. **Center Alignment**: Auto-detect terminal width and center output
9. **Print**: Render line by line to terminal


## License

This project is licensed under the MIT License.

## Acknowledgements

stb_image.h by Sean Barrett
Used for image loading and decoding.
https://github.com/nothings/stb

Inspired by terminal-based ASCII art generators and image-to-text visualizations.
