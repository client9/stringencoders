#!/bin/sh
FILE=stringencoders-v3.1.0
rm -rf $FILE
mkdir $FILE
cp . $FILE

find $FILE -name '.svn' | xargs rm -rf

rm -f ${FILE}/index.html
rm -f ${FILE}/Doxyfile
rm -f ${FILE}/makerelease.sh
rm -rf ${FILE}/doxy

tar -czvf ${FILE}.tar.gz ${FILE}

