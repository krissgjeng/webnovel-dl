#!/bin/bash
shopt -s nullglob
Commandcpp=clang++
#MYLIBF=lib/libtidy
MYLIBS="-ltidy -lcurl -lzip -lstdc++fs"
#MYINCLUDES=lib/libtidy/include
#echo $MYLIBS
mkdir build
#cd ..
files=""

for i in *.cpp; do
    files="$files $i"
done
#echo $files
#echo $Commandcpp -std=c++17 $files -I$MYINCLUDES -L$MYLIBF -l$MYLIBS -o main
$Commandcpp -std=c++17 $MYLIBS $files -lstdc++fs -o main
mv main build/
cp lib/libcurl/cacert.pem build/
#cp lib/libtidy/tidy.so build/
