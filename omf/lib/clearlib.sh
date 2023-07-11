#!/bin/sh

DIR="../lib/"

#Get the libs path
if [ $# -eq 1 ]; then
	DIR=$1
fi

echo "Remove the libs provide by bsp4 in ${DIR}......"

set -v 
rm -rf ./${DIR}/libipc_core.*
rm -rf ./${DIR}/libssl.*
rm -rf ./${DIR}/libmbedx509.*
rm -rf ./${DIR}/libmbedtls.*
rm -rf ./${DIR}/libmbedcrypto.*
rm -rf ./${DIR}/libcurl.*
rm -rf ./${DIR}/libcrypto.*
rm -rf ./${DIR}/libz.*
