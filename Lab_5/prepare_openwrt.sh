wget http://10.0.2.2:8000/kmod-drv-mpc8xxx_4.14.221-1_aarch64_generic.ipk
opkg update
opkg install kmod-drv-mpc8xxx_4.14.221-1_aarch64_generic.ipk
modprobe gpio-mpc8xxx
opkg install libgpiod
opkg install gpiod-tools
gpiodetect
opkg install python
wget http://10.0.2.2:8000/python3-gpiod_1.3-1_aarch64_generic.ipk
opkg install python3-gpiod_1.3-1_aarch64_generic.ipk
opkg install alsa-utils
opkg install pciutils
opkg install kmod-sound-hda-intel
opkg install mpd-full
opkg install mpc
opkg install python3-pip
pip3 install python-mpd2
wget http://10.0.2.2:8000/mpd.conf
wget http://10.0.2.2:8000/BlindingLights.mp3
wget http://10.0.2.2:8000/EyeOfTheTiger.mp3
rm /etc/mpd.conf
mv mpd.conf /etc/mpd.conf
mkdir /root/music
cp BlindingLights.mp3 /root/music/
cp EyeOfTheTiger.mp3 /root/music/
/etc/init.d/mpd restart
opkg install python3-flask
opkg install screen
wget http://10.0.2.2:8000/webservice.py
wget http://10.0.2.2:8000/music_player.py
wget http://10.0.2.2:8000/start_script
chmod +x webservice.py
chmod +x music_player.py
cp ./start_script /etc/init.d/start_script
chmod 777 ./etc/init.d/start_script
/etc/init.d/start_script enable