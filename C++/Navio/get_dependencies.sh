#!/bin/bash

ARCH=`dpkg --print-architecture`

confirm_guard() {
    echo "Would you like to install missing dependencies (requires sudo)? [y/n]"
    while true; do
        read answer
        case $answer in
            [Yy]* ) return ;;
            [Nn]* ) exit ;;
            * ) echo "Wrong answer" ;;
        esac
    done
}

if [[ "$ARCH" == *"arm"* ]]; then
    if ! dpkg -s pigpio &> /dev/null; then
        confirm_guard
        sudo apt-get update
        sudo apt-get install pigpio
    fi
else
    git submodule update --init --recursive;
    pushd pigpio;
    make CROSS_PREFIX=arm-linux-gnueabihf-
    popd;
fi
