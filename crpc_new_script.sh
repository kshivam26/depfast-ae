#!/bin/bash

# Define a range of values for -o, -b, and -x (number of nodes)
o_values=(100)
b_values=(50)
x_values=(25 21 17 13 9 5 3)  # Adjust this array to the actual node counts you want to test
# # Detailed output file for each run
detailed_output_file="output_new.txt"

# Headers for the output files
echo "number_of_nodes, max_pending_requests, batch_size, run_id, throughput, 50%_lat, 90%_lat, 99%_lat, 99.9%_lat" > chaining_output_10KB.txt
echo "number_of_nodes, max_pending_requests, batch_size, run_id, throughput, 50%_lat, 90%_lat, 99%_lat, 99.9%_lat" > broadcast_output_10KB.txt

# Function to kill a process after a timeout
kill_after_timeout() {
    sleep $1
    kill $2 2>/dev/null
}

# Iterate over each method: chaining and broadcast
for method in chaining broadcast ## add back broadcast
do
    # Depending on the method, set the flag and choose the output file
    flag=""
    output_file="broadcast_output_10KB.txt"
    if [ "$method" = "chaining" ]; then
        flag="-f"
        output_file="chaining_output_10KB.txt"
    fi

    # Iterate over each value of -x
    for x in "${x_values[@]}"
    do
        # Iterate over each value of -o
        for o in "${o_values[@]}"
        do
            # Iterate over each value of -b
            for b in "${b_values[@]}"
            do
                # Perform the benchmark 5 times for each method, -x, -o, and -b
                for i in {1..3}
                do
                    echo "Running $method test with -x $x, -o $o, -b $b, iteration $i"
                    # Run the command and direct output to a file
                    ./build/crpc_benchmark -s 127.0.0.1:11000 -w 3 -n 60 -e 1 -x $x $flag -o $o -b $b > $detailed_output_file &
                    # Get PID of the last background process
                    pid=$!

                    # Start timeout function in background
                    kill_after_timeout 75 $pid &
                    timeout_pid=$!

                    # Wait for the benchmark process to finish
                    wait $pid 2>/dev/null
                    # Kill timeout if the process has finished before the timeout
                    kill $timeout_pid 2>/dev/null

                    # Read the throughput and latencies from the detailed output file
                    throughput=$(awk -F'throughput: ' '/throughput:/ {print $2}' $detailed_output_file | awk '{print $1}')
                    lat_50=$(awk -F'50.0% LATENCY: ' '/50.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{gsub(";", "", $1); print $1}')
                    lat_90=$(awk -F'90.0% LATENCY: ' '/90.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{gsub(";", "", $1); print $1}')
                    lat_99=$(awk -F'99.0% LATENCY: ' '/99.0% LATENCY:/ {print $2}' $detailed_output_file | awk '{gsub(";", "", $1); print $1}')
                    lat_999=$(awk -F'99.9% LATENCY: ' '/99.9% LATENCY:/ {print $2}' $detailed_output_file | awk '{gsub(";", "", $1); print $1}')

                    # Append the results to the respective output file
                    echo "$x, $o, $b, $i, $throughput, $lat_50, $lat_90, $lat_99, $lat_999" >> $output_file
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