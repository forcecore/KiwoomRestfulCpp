#!/bin/bash

if [ ! -e $HOME/.ssh/NPKI ] ; then
    echo "공동인증서를 ~/.ssh/NPKI 로 복사하시오"
    exit 1
fi

docker run \
    -i \
    -v $HOME/.ssh/NPKI:/kiwoom/drive_c/users/root/AppData/LocalLow/NPKI \
    --net=host \
    --env WINEARCH=win32 \
    --env WINEPREFIX=/kiwoom \
    --env LANG=ko_KR.utf8 \
    --env LC_ALL=ko_KR.utf8 \
    --name kiwoom_run \
    -t kiwoom \
    /bin/bash -c '( printf "123456\n123456\n\n" | vncpasswd ) && xinit openbox-session -- /usr/sbin/Xvnc :1 -PasswordFile /root/.vnc/passwd'
