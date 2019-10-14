#!/usr/bin/env bash

xterm -e "yarpserver --write" &
# xterm -e "cd Downloads/IR_assignment/iCub/ ; python3 box_mover.py"
xterm -e "cd Downloads/IR_assignment/iCub/ ; iCub_SIM" &
sleep 5
xterm -e "yarpdev --device opencv_grabber --name /test/video" &
sleep 5
xterm -e "yarp connect /test/video /icubSim/texture/screen"
xterm -e "yarpview --name /view/left" &
sleep 3
xterm -e "cd Downloads/IR_assignment/iCub/ ; ./icubSim"
