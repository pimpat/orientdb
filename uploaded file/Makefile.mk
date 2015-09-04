MAKE = make
CPP = g++
CC = gcc
BUILD_FLAG = -w -c $(TP_LIB)
DEBUG_FLAG = -w -c -g $(TP_LIB)
TP_LIB = -L/usr/local/lib/ -ltransport -lzmq -lsqlite3 -I/usr/local/include 
C_WRAPPER_FLAG = -shared
DEPEN_LIB = dep/lib/libdmp_wrapper.so
