#!/bin/bash

if [ ! -f test.node ]
then
	./build.sh
fi

node main.js

