Led Matrix Accelerometer Gyroscope Project
==========================================

Hardware
--------

- [Adafruit 64x64 RGB LED Matrix 2.5mm Pitch](https://www.adafruit.com/product/3649)
- [Adafruit Matrix Bonnet](https://www.adafruit.com/product/3211)
- [MPU6050](https://www.amazon.de/dp/B0CLS2JY43?ref=ppx_yo2ov_dt_b_fed_asin_title)
- Raspberry Pi Zero 2W

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
