#!/bin/bash
shopt -s nullglob
Commandcpp=clang++
#MYLIBF=lib/libtidy
MYLIBS=tidy
#MYINCLUDES=lib/libtidy/include

#cd ..
files=""

for i in *.cpp; do
    files="$files $i"
done
#echo $files
#echo $Commandcpp -std=c++17 $files -I$MYINCLUDES -L$MYLIBF -l$MYLIBS -o main
$Commandcpp -std=c++17 $files -l$MYLIBS -o main
mv main build/
#cp lib/libtidy/tidy.so build/
