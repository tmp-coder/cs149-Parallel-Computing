#!/usr/bin/env bash

V=$1

for i in $(seq 1 32)
do
./mandelbrot -v $V -t $i | grep 'speed';
done