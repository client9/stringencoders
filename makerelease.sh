#!/bin/sh

FILE=stringencoders-v3.10.3
rm -rf ${FILE}
svn export http://stringencoders.googlecode.com/svn/trunk ${FILE}

find $FILE -name '.svn' | xargs rm -rf

rm -f ${FILE}/Doxyfile
rm -f ${FILE}/makerelease.sh
rm -rf ${FILE}/doxy

tar -czvf ${FILE}.tar.gz ${FILE}
rm -rf ${FILE}
echo "DONE"
ls -lh ${FILE}.tar.gz
