FROM ubuntu:20.04

RUN apt-get update && apt-get install -y cpio locales sudo
RUN rm /bin/sh && ln -s -f /bin/bash /bin/sh

RUN locale-gen en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LC_ALL en_US.UTF-8
ENV DEBIAN_FRONTEND=noninteractive

# Install X11 & XFCE environment
RUN apt-get update && apt-get install -y --no-install-recommends \
   dbus-x11 psmisc xdg-utils x11-xserver-utils x11-utils \
   xfce4 xfce4-panel xfce4-session \
   xfwm4 xfwm4-theme-breeze \
   xfce4-taskmanager xfce4-terminal xfce4-xkb-plugin \
   xfce4-settings xfce4-pulseaudio-plugin xfce4-eyes-plugin \
   xfce4-goodies \
   libxv1 mesa-utils mesa-utils-extra 

# Install things needed to remotely access the container
# XRDP (now disabled):
# RUN apt-get update && apt-get install -y xrdp 
# VNC
RUN apt-get update && apt-get install -y tigervnc-standalone-server \
  net-tools

# Install things needed to work with Buildroot
RUN apt-get update && apt-get install -y \
 git subversion mercurial cvs rsync \
 wget gcc g++ gdb make \
 device-tree-compiler \
 cmake ccache ninja-build \
 python3-posix-ipc \
 libncurses-dev unzip bc mc \
 libasound2-dev libpulse-dev \
 vde2 \
 u-boot-tools

ENV USER_NAME dev
ENV PROJECT dev

ARG host_uid=1002
ARG host_gid=1002
RUN groupadd -g $host_gid $USER_NAME && \
    useradd -g $host_gid -m -s /bin/bash -u $host_uid $USER_NAME
RUN echo -e -n "test12\ntest12\n" | passwd dev
RUN adduser dev sudo

USER $USER_NAME
RUN echo "tigervncserver :7 -xstartup /usr/bin/xfce4-session -localhost no" > /home/$USER_NAME/runvnc

ENV SHARED_DIR /home/$USER_NAME/host
RUN mkdir -p $SHARED_DIR 
RUN mkdir -p /home/$USER_NAME/.vnc
RUN chown $USER_NAME.$USER_NAME $SHARED_DIR
WORKDIR /home/$USER_NAME
RUN echo test12 | /usr/bin/tigervncpasswd -f > /home/$USER_NAME/.vnc/passwd
RUN chmod 0600 /home/$USER_NAME/.vnc/passwd
EXPOSE 5907
