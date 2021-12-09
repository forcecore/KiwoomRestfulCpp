#!/bin/bash

echo Wine version
wine --version
printf "\n\n"

echo Winetricks version
winetricks --version

# Copy files from the host (as these may be upgraded)
cp /docker/xinitrc /root/.xinitrc
mkdir -p /root/.config/openbox
cp /docker/autostart /root/.config/openbox/autostart
cp /docker/KiwoomRestfulCpp.exe /kiwoom/drive_c

printf "123456\n123456\n\n" | vncpasswd
xinit openbox-session -- /usr/sbin/Xvnc :1 -PasswordFile /root/.vnc/passwd
