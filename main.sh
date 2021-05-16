#!/bin/bash

#Configuration Parameters

DT="1000000"

RF="1000"

port="51717"

hostname="raza"

#Compiling the codes

gcc client.c -lm -o client
gcc server.c -o server
gcc logfilecode.c -o log
gcc signal.c -o signal

#Executing the processes

xterm -e ./server 51717 &
sleep 2

xterm -e ./client $hostname $port $DT $RF &
sleep 2

xterm -e ./signal &
sleep 1

xterm -e ./log
sleep 1
