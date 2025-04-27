#!/bin/bash

nums=(84000 102000 120000 138000)

echo "Start"
for flag in "-O0" "-O1" "-O2" "-O3" "-Ofast" "-Os" "-Og"
do
    echo "________________Optimization flag: $flag ________________"
    gcc "$flag" times.c -o timesf
    for i in "${nums[@]}"
    do
        echo "N = $i"
        for ((j=1; j<6; j++))
        do
            echo -n "$j. "
            ./timesf $i
        done
    done
done
