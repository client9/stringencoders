#!/bin/sh

FILE=stringencoders-v3.11.0
rm -rf ${FILE}
git clone --depth=1 git@github.com:client9/stringencoders.git ${FILE}
(cd ${FILE} && ./bootstrap.sh)
find $FILE -name ".git" | xargs rm -rf

rm -f ${FILE}/Doxyfile
rm -f ${FILE}/makerelease.sh
rm -rf ${FILE}/doxy

tar -czvf ${FILE}.tar.gz ${FILE}
rm -rf ${FILE}
echo "DONE"
ls -lh ${FILE}.tar.gz
