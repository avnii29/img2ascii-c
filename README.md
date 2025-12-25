# img2ascii-c

A terminal-based C program that converts images into colored ASCII art using brightness mapping and ANSI escape codes.

The project loads an image, resizes it to fit the terminal, maps pixel brightness to ASCII characters, and renders the result with optional color and edge enhancement.

---

## Features

- Load images (PNG, JPEG, etc.)
- Resize images for terminal display (aspect-ratio aware)
- Brightness-to-ASCII character mapping
- ANSI 256-color terminal output
- Modular C codebase
- (Optional) Sobel edge detection for enhanced detail

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

---

## Build

Requirements:
- GCC or compatible C compiler
- Unix-like terminal with ANSI color support

Usage
./img2ascii-c examples/sample.jpg


Planned Options

-w <width> : Set output width
--no-color : Disable ANSI color output
--no-edges : Disable edge detection

How It Works 

1. Load image into an RGB pixel buffer.
2. Resize image to fit terminal dimensions.
3. Convert RGB values to brightness and HSV.
4. Map brightness to ASCII characters.
5. Map color to ANSI escape codes.
6. Print the result line by line to the terminal.


License

This project is licensed under the MIT License.
You are free to use, modify, and distribute this software, provided that the original copyright notice and license are included in any copies or substantial portions of the software.

Acknowledgements

stb_image.h by Sean Barrett
Used for image loading and decoding.
https://github.com/nothings/stb

Inspired by terminal-based ASCII art generators and image-to-text visualizations.
