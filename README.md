# wol
A simple Wake-on-LAN utility written in C++. Compatible with BSD, Linux, and macOS.

## Requirements

### All
- `git` (if cloning the repository)
- `clang` (or `gcc`)
- `make`

### macOS
- [Xcode Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools)

## Compilation and Installation

### Quick Compile and Install

```
git clone https://github.org/digichelle/wol
cd wol
make all
make install
```

### Manual Compilation

```
clang++ -Wall -Os -std=c++23 -o wol wol.cpp
```

**Note:** `g++` can be used instead of `clang++`, if desired.

### Clean After Compilation/Installation

```
make clean
```

## Usage

### Basic Usage

```
wol aa:bb:cc:dd:ee:ff
```

### Specify Broadcast IP Address and/or Port

```
wol -b 255.255.255.255 -p 9 aa:bb:cc:dd:ee:ff
wol -b 255.255.255.255 aa:bb:cc:dd:ee:ff
wol -p 9 aa:bb:cc:dd:ee:ff
```

### Get Help

```
wol --help
```

### Get Version

```
wol --version
```

## License

### MIT License

Copyright © 2026 Michelle Catherine Marcó

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Special Thanks

- Andrei Maiboroda ([gumb0](https://github.com/gumb0))
- Mohammad Mostafizur Rahman ([AsifMinar](https://github.com/AsifMinar))
