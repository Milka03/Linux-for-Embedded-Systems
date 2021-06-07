#!/bin/bash
cd QemuVirt64
./build.sh
cd ..
wget https://downloads.openwrt.org/releases/19.07.7/targets/armvirt/64/openwrt-19.07.7-armvirt-64-Image
wget https://downloads.openwrt.org/releases/19.07.7/targets/armvirt/64/openwrt-19.07.7-armvirt-64-Image-initramfs
wget https://downloads.openwrt.org/releases/19.07.7/targets/armvirt/64/openwrt-19.07.7-armvirt-64-root.ext4.gz
wget https://downloads.openwrt.org/releases/19.07.7/targets/armvirt/64/openwrt-sdk-19.07.7-armvirt-64_gcc-7.5.0_musl.Linux-x86_64.tar.xz
gzip -c -d openwrt-19.07.7-armvirt-64-root.ext4.gz > root.ext4
truncate -s \>512M root.ext4
/sbin/resize2fs root.ext4
python3 -m http.server