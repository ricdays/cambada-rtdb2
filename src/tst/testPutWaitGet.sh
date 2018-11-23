#!/bin/bash


# init
rm -rf /tmp/rtdb2_storage
rm -rf /tmp/{producer,consumer}*.txt

# uncomment next line to test progression using the new WAIT
export WAIT_ENABLED=1

# comment/uncomment consumers/producers as desired

# first arguments are the frequencies in Hz
# (there are more arguments allowed, look inside code)
cd ../../bin
./testPutter 10.0 > /tmp/producer1.txt &
sleep 0.1 # hack to give above putter some time for writing ZERO timestamp ...
./testPutter 33.3 > /tmp/producer2.txt &
./testGetter  3.0 > /tmp/consumer1.txt &
./testGetter 10.0 > /tmp/consumer2.txt &
./testGetter 33.0 > /tmp/consumer3.txt &
./testGetter 50.0 > /tmp/consumer4.txt &

sleep 30

echo "shutting down ..."
(
    killall testGetter
    killall testPutter
) 2>&1 >/dev/null

# to analyze output:
#cat /tmp/{producer,consumer}*.txt | analyzePutWaitGet.py

