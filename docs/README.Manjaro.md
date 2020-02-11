![Kodi Logo](resources/banner_slim.png)

# Arch/Manjaro build guide
This guide has been tested with Manjaro 18.1.5 Juhraya x86_x64. Please read it in full before you proceed to familiarize yourself with the build procedure.

The recommended method to build/install Kodi on Arch Linux or Manjaro (and other Arch clones), is to build from a PKGBUILD rather than building/installing by hand.  The primary reason to build this way is that it allows all files to be managed by pacman which is adventitious for numerous reasons.  

* To build the official Kodi package, use a utility such as [asp](https://wiki.archlinux.org/index.php/Arch_Build_System#Retrieve_PKGBUILD_source_using_Git) which will pull the needed files.
* To build a dev version of Kodi, download a PKGBUILD the [AUR](https://wiki.archlinux.org/index.php/Arch_User_Repository) which hosts [kodi-devel](https://aur.archlinux.org/packages/kodi-devel/).

The remainder of this article shows steps to manually build and install.

Several other distributions have **[specific build guides](README.md)** and a general **[Linux build guide](README.Linux.md)** is also available.

## Table of Contents
1. **[Document conventions](#1-document-conventions)**
2. **[Get the source code](#2-get-the-source-code)**
3. **[Install the required packages](#3-install-the-required-packages)**  
4. **[Build Kodi](#4-build-kodi)**

## 1. Document conventions
This guide assumes you are using `terminal`, also known as `console`, `command-line` or simply `cli`. Commands need to be run at the terminal, one at a time and in the provided order.

This is a comment that provides context:
```
this is a command
this is another command
and yet another one
```

**Example:** Clone Kodi's current master branch:
```
git clone https://github.com/xbmc/xbmc kodi
```

Commands that contain strings enclosed in angle brackets denote something you need to change to suit your needs.
```
git clone -b <branch-name> https://github.com/xbmc/xbmc kodi
```

**Example:** Clone Kodi's current Krypton branch:
```
git clone -b Krypton https://github.com/xbmc/xbmc kodi
```

Several different strategies are used to draw your attention to certain pieces of information. In order of how critical the information is, these items are marked as a note, tip, or warning. For example:
 
**NOTE:** Linux is user friendly... It's just very particular about who its friends are.  
**TIP:** Algorithm is what developers call code they do not want to explain.  
**WARNING:** Developers don't change light bulbs. It's a hardware problem.

**[back to top](#table-of-contents)**

## 2. Get the source code
Make sure `git` is installed:
```
sudo pacman -S git
```

Change to your `home` directory:
```
cd $HOME
```

Clone Kodi's current master branch:
```
git clone https://github.com/xbmc/xbmc kodi
```

**[back to top](#table-of-contents)**

## 3. Install the required packages

Kodi needs several third party packages, most of them can be installed from the official Arch/Manjaro repository

```
sudo pacman --needed --asdeps -S alsa-lib autoconf automake avahi bluez-libs bzip2 cmake curl \
dbus ffmpeg flatbuffers fmt fontconfig freetype2 fribidi gawk giflib glu gperf jre8-openjdk libao \
libass libbluray libcap libcdio libcec libcrossguid libjpeg-turbo libltdl libmariadbclient \
libmicrohttpd libmpcdec libnfs libplist libpng libpulse libssh libsystemd libtool libusb \
libva libvdpau libxml2 libxmu libxrandr libxslt libxt lsb-release lzo mesa nasm openssl pcre \
python2 python2-pillow rapidjson shairplay smbclient sqlite swig taglib tinyxml yasm zlib
```

In addition to those, Kodi needs a few packages that are currently unavailable in the official repo, but they can be installed from the AUR([Arch User Repo](https://aur.archlinux.org/)).
```
cwiid fstrcmp
```
Those can either be installed manually[[1](https://wiki.archlinux.org/index.php/Arch_User_Repository#Installing_packages)] or by using a AUR Helpers[[2](https://wiki.archlinux.org/index.php/AUR_helpers)].

**Optional:** For developers and anyone else who compiles frequently it is recommended to use ccache
```
sudo pacman -S ccache
```
**[back to top](#table-of-contents)**

## 4. Build Kodi
See the general **[Linux build guide](README.Linux.md)** for reference.

**[back to top](#table-of-contents)**
