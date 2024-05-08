#!/bin/bash

echo "-----------making $1----------------"

make MODULE=$1 ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu-