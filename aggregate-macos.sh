#!/bin/zsh

echo "Copying libgal.a to libraries folder"
cp ./gallib/build/src/*.a ./lib

echo "Copying header files from gallib to includes folder"
cp ./gallib/src/*.h ./include

echo "Renaming gallib header to libgal.h"
mv ./include/gal.h ./include/libgal.h

echo "Copying libgll.a to libraries folder"
cp ./libdbll/build/src/*.a ./lib

echo "Copying libdbll header files from libdbll to includes folder"
cp ./libdbll/src/*.h ./include

echo "Renaming libdbll.h to libgll.h"
cp ./include/libdbll.h ./include/libgll.h

echo "Copying test cases to testcases folder"
cp ./test_case_generation/*.tc ./testcases