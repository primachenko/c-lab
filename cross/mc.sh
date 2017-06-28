#!/bin/bash

CFLAGS="-I/home/user/cross/build/include -I/home/user/cross/build/lib/glib-2.0/include -I/home/user/cross/build/lib/libffi-3.2.1/include" \
LDFLAGS="-L/home/user/cross/build/lib " \
PKG_CONFIG_PATH="/home/user/cross/build/lib/pkgconfig" \
CC="arm-linux-gnueabi-gcc" \
./configure \
--with-screen=ncurses \
--host=arm-linux-gnueabi \
--prefix=/home/user/cross/build \
--with-ncurses-libs=/home/user/cross/build/lib \
--with-ncurses-includes=/home/user/cross/build/include
