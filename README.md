# PRIMG

A C program to print an image to the terminal using ASCII block characters (▀▄█).

## Screenshots

![Space Render](imgs/space.png "Space")

![Mountain Range](imgs/mountain.png "Mountains")

![Low Resolution Planets](imgs/planets-low.png "Planets in a small window")

![High Resolution Planets](imgs/planets-high.png "Planets in a bigger window")


## Building & Installation

Run `make` inside the `src/` directory, which will build the executable in `bin/`.

To install run `sudo make install` which will install the program in
`/usr/local/bin/` by default, but it can be changed by modifying
the `INSTALL_PATH` environment variable

## Usage

The most basic usage of the program is to call it with a filename.
The image can be a PNG, JPG, and possibly a BMP. GIFs are not supported yet.

`primg example.png`

More options shall come.

