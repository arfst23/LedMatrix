Led Matrix Project
==================

Hardware
--------

- 4 x [Adafruit 64x64 RGB LED Matrix 2mm Pitch](https://www.adafruit.com/product/5362)
- [Adafruit Matrix Bonnet](https://www.adafruit.com/product/3211)
- Raspberry Pi Zero 2W

3D print the stand `matrix.FCStd`

Raspberry Pi Install
--------------------

- OS
- ssh access
- firewall
- wifi config
- mount ro
- disable audio

    aplay -l
    cd /etc/modprobe.d
    sudo vi alsa-blacklist.conf
    blacklist snd_bcm2835

    sudo vi /boot/cmdline.txt
    isolcpus=3

- led matrix library and config

    ./build

- build software

    make
