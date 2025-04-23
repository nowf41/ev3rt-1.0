#!/bin/bash
# THIS FILE IS PUBLIC DOMAIN.
# OTHER FILES EXCEPT THIS FILE ARE **NOT** PUBLIC DOMAIN.

# install packages
sudo apt update
sudo apt install build-essential ruby -y
sudo gem install shell
sudo bash ev3rt-prepare-ubuntu.sh


# PATH
touch ~/.bashrc
echo "export PATH=\"\$PATH:/opt/gcc-arm-none-eabi-6-2017-q1-update/bin\"" >> /.bashrc
source ~/.bashrc