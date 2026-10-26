# wol
A simple Wake-on-LAN utility written in C++. Compatible with Linux, macOS, and Windows.

## Requirements

### Linux
- [Git](https://git-scm.com) ≥ 2.54.0
- [Clang](https://clang.llvm.org/) ≥ 21.0.0
- [lld](https://lld.llvm.org) ≥ 21.0.0
- [CMake](https://cmake.org) ≥ 3.31

### macOS
- macOS 26 Tahoe or higher (previous versions may work, but have not been tested)
- CMake ≥ 3.31
- Latest version of [Xcode](https://developer.apple.com/xcode/) or [Xcode Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools) (includes Git and Clang)

### Windows
- Windows 10 or higher
- [MSYS2](https://www.msys2.org/) [CLANG64 environment](https://www.msys2.org/docs/environments/)
- Git ≥ 2.54.0 (install package `mingw-w64-clang-x86_64-git` or install directly in Windows)
- Clang ≥ 21.0.0 (install package `mingw-w64-clang-x86_64-clang`)
- lld ≥ 21.0.0 (install package `mingw-w64-clang-x86_64-lld`)
- CMake ≥ 3.31 (install package `mingw-w64-clang-x86_64-cmake` or install directly in Windows)

**Note:** Add `MSYS2_PATH_TYPE=inherit` to your Windows environment variables if you want to use programs in your Windows PATH in MSYS2 environments.

### Other

It may be possible to compile and run this program on other UNIX/UNIX-like operating systems such as BSD, provided that all the requirements in the Linux section are met. However, this scenario has not been tested.

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

**Note:** This installation method is not supported on Windows. Manually copy the `wol.exe` executable into your desired directory instead. You may wish to strip it of debug symbols by running `strip build/wol.exe` beforehand.

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

This program is based on work by the following authors:

- Andrei Maiboroda ([gumb0](https://github.com/gumb0))
- Mohammad Mostafizur Rahman ([AsifMinar](https://github.com/AsifMinar))
