# img2ascii
Convert images and GIFs to ascii characters.

**A GIF of a Tyrannosaurus:**\
![demo](https://github.com/user-attachments/assets/65a44fe7-c582-4fc0-a622-fddd5bfd4195)

## Prerequisites
- c++20
- giflib
- boost [https://www.boost.org/doc/user-guide/getting-started.html]
- wget
  - only for URLs

## Usage
Make executable:
```bash
cd img2ascii
make
```

Show usage:
```bash
./img2ascii
```
or
```bash
./img2ascii --help
```

Converting an image to ascii:
```bash
./img2ascii image.png
```

### URLs
Downloading images requires `wget` to be installed. Downloading the image will prompt confirmation before downloading:
```bash
./img2ascii 'https://example.com/image.png'
```

> [!TIP]
> Args can still be used if you use URL.

### Chroma Keying
Blocking out a color (like a green screen) can be used with the `chroma` command:
```bash
./img2ascii image.png --chroma='0,255,0'
```
optionally use thresholds:
```bash
./img2ascii image.png --chroma='0,255,0' --chroma-threshold=0.7
```

Alternatively, color names can be used, e.g. `--chroma=green`:
- green
- red
- blue
- white
- black

### Scaling
Large images can fill the entire terminal; the `scale` command can be used to prevent this:
```bash
./img2ascii image.png --scale=0.1
```

### Colorizing Characters
Use `colored` arg to colorize ascii characters:
```
./img2ascii image.png --colored
```

> [!TIP]
> This also works with GIFs.

### GIFs
#### Duration of Animation
Specify number of times to loop GIF:
```bash
./img2ascii image.gif --gif-cycles=2
```

Specified time in seconds:
```bash
./img2ascii image.gif --gif-time=3
```

Or loop a GIF forever (cancel with ctrl+C):
```bash
./img2ascii image.gif --gif-loop
```

#### Speed of GIFs
The speed of a GIF can be changed via the `gif-speed` arg:
```bash
./img2ascii image.gif --gif-speed=0.5
```

## Supported Images
- png
- jpg
- gif
