# memwatch #

## Overview ##
Interactive memory viewer for Linux.

## Features ##
 * General statistics on the use of memory.
 * Process list, sorted by memory usage.
 * Show only processes with specified PID.

## Install ##

```
$ git clone https://bitbucket.org/PascalRD/memwatch
$ cd memwatch && mkdir build && cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr ../
$ make
$ su
$ make install
```

On Gentoo Linux memwatch is available in portage: sys-process/memwatch
