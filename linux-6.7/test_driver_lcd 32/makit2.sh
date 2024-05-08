#!/bin/bash

echo "-----------making $1----------------"

make MODULE=$1 ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- 