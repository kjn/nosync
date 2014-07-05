nosync
======

nosync is a small preload library that can be used to disable
synchronization of file's content with storage devices on GNU/Linux.
It works by overriding implementations of certain standard functions
like `fsync` or `open`.

nosync is free software.  You can redistribute and/or modify it under
the terms of Apache License Version 2.0.

The author of nosync is Mikolaj Izdebski.


Installation
------------

    make install prefix=/target/location


Usage
-----

    LD_PRELOAD=/target/location/lib/nosync/nosync.so command


Implementation
--------------

nosync works by overloading the following functions:

  * `fdatasync`,
  * `fsync`,
  * `msync`,
  * `open`,
  * `open64`,
  * `sync`,
  * `sync_file_range`.
