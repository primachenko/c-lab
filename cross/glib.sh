#!/bin/bash
echo "glib_cv_stack_grows=no" > config.cache
echo "glib_cv_uscore=no" >> config.cache
echo "ac_cv_func_posix_getpwuid_r=yes" >> config.cache
echo "ac_cv_func_posix_getgrgid_r=yes" >> config.cache
CC="arm-linux-gnueabi-gcc" \
CPPFLAGS="-I/home/user/cross/build/include -I/home/user/cross/build/lib/libffi-3.2.1/include" \
LDFLAGS="-L/home/user/cross/build/lib" \
LIBFFI_LIBS="-L/home/user/cross/build/lib -lffi" \
LIBFFI_CFLAGS="-I/home/user/cross/build/lib/libffi-3.2.1/include" \
./configure --cache-file=config.cache --with-libiconv=no --host=arm-linux-gnueabi --prefix=/home/user/cross/build -C --enable-static --enable-shared=yes