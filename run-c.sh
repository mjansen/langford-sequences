#! /bin/bash

n=$1

gcc -D N=$1 -O3 langford-in-c.cc -o langford$1-in-c

time ./langford$1-in-c > l$1-c
