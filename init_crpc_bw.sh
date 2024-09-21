#!/bin/bash
# sudo tc qdisc add dev eth0 root tbf rate 3gbit burst 15mb latency 1ms

#!/bin/bash

# Check if there is at least one argument
if [ $# -lt 1 ]; then
    echo "Usage: $0 <argument>"
    exit 1
fi

# Get the first argument
arg=$1
delayVal=0.25
slow=${2:-$delayVal}
# sudo apt-get install -y bwm-ng
# data_size=${arg*2}
# echo "datasize is: ${data_size}"
# Perform different actions based on the argument

# tc qdisc add dev eth0 root handle 1:0 netem delay ${slow}ms


if [ $arg = 0 ]; then
    tc qdisc add dev eth0 root netem delay ${slow}ms
    echo "slowvalue is: ${slow}"
    echo "no bandwidthlimit added"
else 
    tc qdisc add dev eth0 root netem delay ${slow}ms rate ${arg}gbit
    # sudo tc qdisc add dev eth0 root tbf rate ${arg}gbit burst 50mb latency 50ms
    # sudo tc qdisc add dev eth0 root tbf rate ${arg}gbit burst 31250000 latency 50ms limit 12500000
    # tc qdisc add dev eth0 parent 1:1 handle 10: tbf rate ${arg}gbps burst ${arg}mb latency 1ms
    echo "slowvalue is: ${slow}"
fi
# if [ $arg = 1 ]; then
#     # Command for condition 1
#     echo "Executing command for condition 1"
#     echo 
#     sudo tc qdisc add dev eth0 root netem delay 0.25ms rate 1gbit
#     # Add your command here
# elif [ $arg = 2 ]; then
#     # Command for condition 2
#     echo "Executing command for condition 2"
#     # Add your command here
#     sudo tc qdisc add dev eth0 root netem delay 0.25ms rate 2gbit
#     # sudo tc qdisc add dev eth0 root tbf rate 2gbit burst 15mb latency 1ms
# elif [ $arg = 3 ]; then
#     # Command for condition 2
#     echo "Executing command for condition 2"
#     # Add your command here
#     sudo tc qdisc add dev eth0 root netem delay 0.25ms rate 3gbit
#     # sudo tc qdisc add dev eth0 root tbf rate 3gbit burst 15mb latency 1ms
# elif [ $arg = 5 ]; then
#     # Command for condition 3
#     echo "Executing command for condition 3"
#     # Add your command here
#     sudo tc qdisc add dev eth0 root netem delay 0.25ms rate 5gbit
#     # sudo tc qdisc add dev eth0 root tbf rate 5gbit burst 20mb latency 1ms
# elif [ $arg = 10 ]; then
#     # Command for condition 3
#     echo "Executing command for condition 3"
#     # Add your command here
#     sudo tc qdisc add dev eth0 root netem delay 0.25ms rate 10gbit
#     # sudo tc qdisc add dev eth0 root tbf rate 10gbit burst 40mb latency 1ms
# else
#     # Default action if the argument doesn't match any condition
#     echo "Unknown argument: $arg"
#     sudo tc qdisc add dev eth0 root netem delay 0.25ms rate 10gbit
#     sudo tc qdisc add dev eth0 root tbf rate 10gbit burst 40mb latency 1ms
# fi