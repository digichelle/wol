# wol
A simple Wake-on-LAN utility written in C++. Compatible with Linux and macOS.

## Requirements

### Linux
- Git
- Clang
- lld
- CMake
- make

### macOS
- CMake
- [Xcode Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools)

## Compilation and Installation

### Initial Setup

```
git clone https://github.org/digichelle/wol
cd wol
cmake -S . -B build --fresh
```

### Compile

```
cmake --build build
```

### Install

```
sudo cmake --install build --strip
```

### Clean After Compilation and/or Installation

```
cmake --build build --target clean
```

## Usage

### Basic Usage

```
wol aa:bb:cc:dd:ee:ff
```

### Specify Broadcast IP Address and/or Port

```
wol -i 255.255.255.255 -p 9 aa:bb:cc:dd:ee:ff
wol -i 255.255.255.255 aa:bb:cc:dd:ee:ff
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
