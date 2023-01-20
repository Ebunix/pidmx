# PiDMX
Simple desktop DMX controller

Not included in this repository is a build of Google's V8 engine. The header files are present, but the binaries are omitted due to their massive size.
You'll have to build the correct binaries yourself or grab them from a different place. 

# Cloning
Clone the repository using `git clone --recursive https://github.com/Ebunix/pidmx` to take care of the 

# PiDMX
For compiling PiDMX you'll need a static, monotithic build of Google's V8 Javascript engine. Building V8 is quite lengthy and not that straightforward, so I've provided pre-built binaries for x64 Linux [here](https://static.itsebu.net/v8.tar.gz) that you can download. The included `x64.debug` and `x64.release` folders need to be extracted into `js/libv8`, next to the already present `include` folder.

From the repository root directory:
```bash
mkdir build && cd build
cmake ..
make -j8
```

# Kernel module
This dummy driver module provides 16 in-memory DMX channels. These aren't transmitted anywhere, but you get 16 nodes under `/dev/dmx(n)` that you can write to. Currently these are entirely owned by root. Todo for the future is making axxess for these nodes group-based so you don't need to be root to write to them. For now as a (terrible) workaround you can `chmod 0666` the nodes that you need. This is definitely not the proper way to do it, but I'm not a kernel driver developer :)

From the repository root directory:
```bash
cd module
make
```

Load the DMX dummy driver by running `insmod dmxio.ko` as root.

Each device node represents a DMX 'universe'. The files are essentially 512-byte long memory buffers that you can read and write. Any read and write operation will always start at the first byte, with a maximum length of 512. Seeking these files doesn't do anything. Writing beyond the 512 byte length of the file will discard any additional data beyond the 512 byte limit.

To change the number of created DMX nodes, edit the `module/src/dmxmodule.c` file and change the `MAX_UNIVERSES` value at the top of the file to the desired number.
