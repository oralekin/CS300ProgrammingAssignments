#! /bin/sh

find src/ | entr -cns "make run input=samples/$1.in | diff -y samples/$1.out -"
