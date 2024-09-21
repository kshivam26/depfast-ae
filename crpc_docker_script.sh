#!/bin/bash


# Define a range of values for -o, -b, and -x (number of nodes)
o_values=(100)
b_values=(50)
x_values=(25 21 17 13 9 5 3) # 25 21 17 13 9 5 3 # Adjust this array to the actual node counts you want to test
# # Detailed output file for each run
detailed_output_file="output_first_node.txt"

# Headers for the output files
echo "number_of_nodes, max_pending_requests, batch_size, run_id, throughput, 50%_lat, 90%_lat, 99%_lat, 99.9%_lat, bw_util" > chaining_output_10B_docker.txt
echo "number_of_nodes, max_pending_requests, batch_size, run_id, throughput, 50%_lat, 90%_lat, 99%_lat, 99.9%_lat, bw_util" > broadcast_output_10B_docker.txt

# Function to kill a process after a timeout
kill_after_timeout() {
    sleep $1
    kill $2 2>/dev/null
}

kill_all_processes() {
    local procs=("$@")
    echo "going to sleep for 75seconds"
    sleep 75
    echo "back up from sleep"
    for pid in "${procs[@]}"; do
        kill -9 $pid 2>/dev/null
    done
    echo "all processes killed"
}

# Iterate over each method: chaining and broadcast
for method in chaining broadcast ## add back broadcast
do
    # Depending on the method, set the flag and choose the output file
    flag=""
    output_file="broadcast_output_10B_docker.txt"
    if [ "$method" = "chaining" ]; then
        flag="-f"
        output_file="chaining_output_10B_docker.txt"
    fi

    # Iterate over each value of -x
    for x in "${x_values[@]}"
    do      
        # # Iterate over each value of -o
        for o in "${o_values[@]}"
        do
            # Iterate over each value of -b
            for b in "${b_values[@]}"
            do
                # Perform the benchmark 3 times for each method, -x, -o, and -b
                for i in {1..3}
                do
                    ./crpc_docker_setup.sh $x
                    declare -a pids

                    # Iterate through each node
                    for index in $(seq 1 $x)
                    do
                        container_name="micro_${index}"
                        echo "Running $method test on node $index with -x $x, -o $o, -b $b, iteration $i"

                        command="/root/depfast/build/crpc_benchmark_docker -w 1 -n 60 -e 1 -t $index -x $x $flag -o $o -b $b"
                        build_command="python3 /root/depfast/waf configure -J build"
                        if [ "$index" -eq 1 ]; then
                            # Capture output only from the first node
                            # sudo docker exec -i "$container_name" $build_command
                            echo "inside loop 1"
                            sudo docker exec -i "$container_name" $command > "output_first_node.txt" &
                        else
                            echo "inside loop 2"
                            sudo docker exec -d "$container_name" $command &
                        fi
                        pids+=($!)  # Store PID of the background process
                    done

                    sleep 5
                    sudo docker exec -i micro_1 bwm-ng --interfaces eth0 -t 1000 -o csv > bwmng_output.csv &
                    # Allow the benchmarks to run for 75 seconds before stopping them
                    echo "Allowing benchmarks to run for 75 seconds..."
                    sleep 70

                    # Stop all containers
                    for index in $(seq 1 $x)
                    do
                        container_name="micro_${index}"
                        echo "Stopping container $container_name"
                        sudo docker stop "$container_name"
                    done

                    # sleep 10

                    # Optionally restart the containers if they need to be used immediately again
                    # for index in $(seq 1 $x)
                    # do
                    #     container_name="micro_${index}"
                    #     echo "Starting container $container_name"
                    #     sudo docker start "$container_name"
                    # done

                    # Process output from the first node
                    if [ -f "output_first_node.txt" ]; then
                        # throughput=$(awk -F'throughput: ' '/throughput:/ {print $2}' "output_first_node.txt" | awk '{print $1}')
                        # echo "$x, $o, $b, $i, $throughput" >> "$output_file"
                        

                        throughput=$(awk -F'throughput: ' '/throughput:/ {print $2}' $detailed_output_file | awk '{print $1}')
                        lat_50=$(awk -F'50.0% LATENCY: ' '/50.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{gsub(";", "", $1); print $1}')
                        lat_90=$(awk -F'90.0% LATENCY: ' '/90.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{gsub(";", "", $1); print $1}')
                        lat_99=$(awk -F'99.0% LATENCY: ' '/99.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{gsub(";", "", $1); print $1}')
                        lat_999=$(awk -F'99.9% LATENCY: ' '/99.9% LATENCY:/ {print $2}' $detailed_output_file | awk '{gsub(";", "", $1); print $1}')

                        bw_output=$(python3 extract_bw_utilization.py .)
                        echo "bw_output: $bw_output"
                        # Append the results to the respective output file
                        echo "$x, $o, $b, $i, $throughput, $lat_50, $lat_90, $lat_99, $lat_999, $bw_output" >> $output_file
                        echo "Recorded throughput for iteration $i: $throughput"
                    fi
                done
            done
        done
    done
done

echo "Benchmarking complete."












































# # Output file for summarized results
# output_file="benchmark_results_bcast3.txt"


# # Header for the output file
# # echo "number_of_nodes, max_pending_requests, batch_size, run_id, throughput" > $output_file
# echo "number_of_nodes, max_pending_requests, batch_size, run_id, throughput, 50%_lat, 90%_lat, 99%_lat, 99.9%_lat" > $output_file

# # Function to kill a process after a timeout
# kill_after_timeout() {
#     sleep $1
#     kill $2 2>/dev/null
# }

# # Iterate over each value of -x
# for x in "${x_values[@]}"
# do
#     # Iterate over each value of -o
#     for o in "${o_values[@]}"
#     do
#         # Iterate over each value of -b
#         for b in "${b_values[@]}"
#         do
#             # Perform the benchmark 5 times for the current combination of -x, -o, and -b
#             for i in {1..3}
#             do
#                 echo "Running test with -x $x, -o $o, -b $b, iteration $i"
#                 # Run the command and direct output to a file
#                 ./build/crpc_benchmark -s 127.0.0.1:11000 -w 1 -n 60 -e 1 -x $x -o $o -b $b > $detailed_output_file &
#                 # Get PID of the last background process
#                 pid=$!

#                 # Start timeout function in background
#                 kill_after_timeout 75 $pid &
#                 timeout_pid=$!

#                 # Wait for the benchmark process to finish
#                 wait $pid 2>/dev/null
#                 # Kill timeout if the process has finished before the timeout
#                 kill $timeout_pid 2>/dev/null

#                 # # Read the throughput from the detailed output file
#                 # throughput=$(awk -F'throughput: ' '/throughput:/ {print $2}' $detailed_output_file | awk '{print $1}')

#                 # # Append the results to the output file
#                 # echo "$x, $o, $b, $i, $throughput" >> $output_file

#                 # Read the throughput from the detailed output file
#                 throughput=$(awk -F'throughput: ' '/throughput:/ {print $2}' $detailed_output_file | awk '{print $1}')

#                 # Read latencies
#                 lat_50=$(awk -F'50.0% LATENCY: ' '/50.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{print $1}')
#                 lat_90=$(awk -F'90.0% LATENCY: ' '/90.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{print $1}')
#                 lat_99=$(awk -F'99.0% LATENCY: ' '/99.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{print $1}')
#                 lat_999=$(awk -F'99.9% LATENCY: ' '/99.9% LATENCY:/ {print $2}' $detailed_output_file | awk '{print $1}')

#                 # Append the results to the output file
#                 echo "$x, $o, $b, $i, $throughput, $lat_50, $lat_90, $lat_99, $lat_999" >> $output_file

#             done
#         done
#     done
# done

# echo "Benchmarking complete."