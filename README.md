blctl
=====

This tool is used to control device backlights on linux, through `/sys`
interface.

## Building

Just type `make` :)

Or, for an out-of-tree build:
```
mkdir build
cd build
make -f ../Makefile
```

You will need GNU make for this.

## Installing

`make install` will install everything to `/usr/local`. You can specify the
prefix with `make PREFIX=/usr install`.

## Usage

Take a look at the man page [here](doc/blctl.1.md).
