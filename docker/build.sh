#!/bin/bash

if [ ! -e OpenAPISetup.exe ] ; then
    echo "OpenAPISetup.exe 를 이 폴더에 복사하시오"
    exit 1
fi

if [ ! -e KiwoomRestfulCpp.exe ] ; then
    echo "KiwoomRestfulCpp.exe 를 이 폴더에 복사하시오"
    exit 1
fi

docker build -t kiwoom .
