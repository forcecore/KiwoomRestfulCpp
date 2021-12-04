FROM archlinux:latest

# Add multilib for wine
# Locale disable by default. Let's enable it.
# Also choose faster mirrors, we have a lot to download...
RUN printf "\n\n[multilib]\nInclude = /etc/pacman.d/mirrorlist\n" >> /etc/pacman.conf && \
    sed -i 's/NoExtract/# NoExtract/g' /etc/pacman.conf && \
    pacman -Syy && \
    pacman -S --noconfirm reflector && \
    reflector --country "South Korea" --sort rate --latest 10 --number 5 --save /etc/pacman.d/mirrorlist

# Install wine. Wine is so huge so we want to split the run command.
# winetricks must up-to-date so not using winetricks package.
RUN pacman --noconfirm -S wine && \
    curl https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks > /bin/winetricks && \
    chmod +x /bin/winetricks

# Install VNC stuff
RUN pacman --noconfirm -S glibc \
        xorg-xinit tigervnc xterm openbox ttf-dejavu ttf-liberation \
        xorg-server-xvfb \
        lib32-gnutls gnutls cabextract unzip && \
    echo "ko_KR.UTF-8 UTF-8" >> /etc/locale.gen && locale-gen

# Setup wine env
ENV WINEARCH=win32 WINEPREFIX=/kiwoom
WORKDIR /kiwoom/drive_c

# Need fonts to see hangul
RUN winetricks corefonts && winetricks cjkfonts
# vcrun2019, as we are running an MFC application.
# -q for silent install.
# But even with -q, the installation will fail without the X server.
# We use xvfb-run!
RUN xvfb-run winetricks -q vcrun2019
RUN xvfb-run winetricks -q vcrun6

# Install OpenAPISetup.exe, with the answer preset ans.iss.
# ans.iss can be created by running
# wine OpenAPISetup.exe /r /f1"c:\\ans.iss"
ENV LANG=ko_KR.utf8 LC_ALL=ko_KR.utf8
COPY ./KiwoomRestfulCpp.exe ./OpenAPISetup.exe ./ans.iss /kiwoom/drive_c/
RUN xvfb-run wine OpenAPISetup.exe /s /f1"c:\\ans.iss"

# If you only run OpenAPISetup.exe, you get
    # 0024:err:ole:com_get_class_object class {a1574a0d-6bfa-4bd7-9020-ded88711818d} not registered
    # 0024:err:ole:com_get_class_object class {a1574a0d-6bfa-4bd7-9020-ded88711818d} not registered
    # 0024:err:ole:com_get_class_object no class object {a1574a0d-6bfa-4bd7-9020-ded88711818d} could be created for context 0x3
# when you run the KiwoomRestfulCpp.exe.
# The solution is to run regsvr32 to register the active x component manually.
# Running the command once doesn't work for some reason,
# (will not be commiteed into wine's system.reg)
# so we run regsvr32 TWICE :)
# Also, if you merge these commands with OpenAPISetup.exe, it will
# also make regsvr32 not commit into system.reg.
# Even if you see this message without xvfb-run,
# regsvr32: Successfully registered DLL 'C:/OpenAPI/khopenapi.ocx'
# It didn't work and not commited into system.reg :)
# As you can see, the setup is very brittle.
WORKDIR /kiwoom/drive_c/OpenAPI
RUN LANG=C LC_ALL=C xvfb-run regsvr32 C:/OpenAPI/khopenapi.ocx && \
    sleep 1 && \
    LANG=C LC_ALL=C xvfb-run regsvr32 C:/OpenAPI/khopenapi.ocx && \
    sleep 1

COPY ./xinitrc /root/.xinitrc
COPY ./autostart /root/.config/openbox/autostart
WORKDIR /kiwoom/drive_c
