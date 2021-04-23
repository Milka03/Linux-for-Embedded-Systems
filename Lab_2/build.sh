#!/bin/bash
BR_NAME=buildroot-2021.02
BR_FILE=${BR_NAME}.tar.bz2
BR_DL=../${BR_FILE}
set -e
if [ ! -f ${BR_DL} ] || ! ( bzip2 -q -t ${BR_DL}); then
  (  
     cd ..
     rm -f ${BR_FILE}
     wget https://buildroot.org/downloads/${BR_FILE}
  )
fi
tar -xjf ${BR_DL}
cp .config ${BR_NAME}/.config
cp -r timer ${BR_NAME}/package 
cp Config.in ${BR_NAME}/package/Config.in

cd buildroot-2021.02
for i in ../patches/* ; do
   patch -p1 < $i
done
make


