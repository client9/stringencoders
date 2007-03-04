#!/bin/bash
VERSION=3.0.0
rm -rf tmp
mkdir tmp
cd tmp
svn co https://wush.net/svn/dots101/base64 modp-base64-v${VERSION}
find . -name '.svn' | xargs rm -rf

rm -f modp-base64-v${VERSION}/index.html
rm -f modp-base64-v${VERSION}/performance-data.html
rm -f modp-base64-v${VERSION}/Doxyfile
rm -f modp-base64-v${VERSION}/makerelease.sh
rm -rf modp-base64-v${VERSION}/doxy

tar -cjvf modp-base64-v${VERSION}.tar.bz2 modp-base64-v${VERSION}
tar -czvf modp-base64-v${VERSION}.tar.gz modp-base64-v${VERSION}

