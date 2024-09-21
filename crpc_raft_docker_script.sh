#!/bin/bash

# Server counts to iterate over
server_counts=(25 21 3 5 9 13 17)
# server_counts=(13)

# Values for -v option
v_values=(2 0)

# Initialize output files with new headers
echo "server_count, run_id, throughput, 50%_lat, 90%_lat, 99%_lat, 99.9%_lat, bw_util" > broadcast_testcommitissue_output_raft_10B_docker.txt
echo "server_count, run_id, throughput, 50%_lat, 90%_lat, 99%_lat, 99.9%_lat, bw_util" > chaining_testcommitissue_output_raft_10B_docker.txt

# Setup and run Docker containers for each configuration
for server_count in "${server_counts[@]}"; do
    # Iterate for each -v value
    for v_value in "${v_values[@]}"; do
        # Determine the output file based on the -v value
        if [ "$v_value" -eq 2 ]; then
            output_file="chaining_testcommitissue_output_raft_10B_docker.txt"
        else
            output_file="broadcast_testcommitissue_output_raft_10B_docker.txt"
        fi

        # Perform the run 3 times for each server count and -v value
        for run_id in {1..3}; do
            echo "Setting up $server_count Docker containers for run_id $run_id..."
            ./crpc_docker_cleanup.sh
            ./crpc_docker_setup.sh $server_count

            # # Wait for containers to be ready
            # echo "Waiting for containers to initialize..."
            # sleep 30

            declare -a pids
            echo "Running test with server_count $server_count, -v $v_value, run_id $run_id"

            # Iterate through each server/process
            for process_id in $(seq 1 $server_count); do
                container_name="micro_${process_id}"
                config_filename="config/1c1s${server_count}r1p.yml"

                echo "Executing command on $container_name for server p$process_id"

                # Execute the command inside the Docker container
                if [ "$process_id" -eq 1 ]; then
                    # Capture the output from the first container
                    sudo docker exec -i "$container_name" \
                    build/deptran_server -f config/none_fpga_raft.yml -f "$config_filename" -f config/rw.yml -f config/concurrent_1.yml -P p$process_id -v $v_value -d 60 > output_first_node.txt &
                else
                    sudo docker exec -d "$container_name" \
                    build/deptran_server -f config/none_fpga_raft.yml -f "$config_filename" -f config/rw.yml -f config/concurrent_1.yml -P p$process_id -v $v_value -d 60 &
                fi
                
                pids+=($!)  # Store PID of the background process
            done

            # Short delay before monitoring bandwidth
            echo "Monitoring bandwidth for 60 seconds on micro_1..."
            sleep 5
            sudo docker exec -i micro_1 bwm-ng --interfaces eth0 -t 1000 -o csv > bwmng_output.csv &

            # Allow the commands to execute for 70 seconds before stopping processes
            echo "Allowing commands to execute for 70 seconds..."
            sleep 70


            # Stop all containers
            for index in $(seq 1 $server_count)
            do
                container_name="micro_${index}"
                echo "Stopping container $container_name"
                sudo docker stop "$container_name"
            done

            # # Kill all background processes
            # echo "Stopping all processes..."
            # for pid in "${pids[@]}"; do
            #     kill -9 $pid 2>/dev/null
            # done


            echo "All processes for run_id $run_id with -v $v_value have been stopped."

            # Process output from the first node
            if [ -f "output_first_node.txt" ]; then
                throughput=$(awk -F'throughput: ' '/throughput:/ {print $2}' "output_first_node.txt" | awk '{print $1}')
                lat_50=$(awk -F'50.0% LATENCY: ' '/50.0% LATENCY:/ {print $2}' "output_first_node.txt" | awk '{gsub(";", "", $1); print $1}')
                lat_90=$(awk -F'90.0% LATENCY: ' '/90.0% LATENCY:/ {print $2}' "output_first_node.txt" | awk '{gsub(";", "", $1); print $1}')
                lat_99=$(awk -F'99.0% LATENCY: ' '/99.0% LATENCY:/ {print $2}' "output_first_node.txt" | awk '{gsub(";", "", $1); print $1}')
                lat_999=$(awk -F'99.9% LATENCY: ' '/99.9% LATENCY:/ {print $2}' "output_first_node.txt" | awk '{gsub(";", "", $1); print $1}')

                # Extract and print the bandwidth output
                bw_output=$(python3 extract_bw_utilization.py .)
                echo "bw_output: $bw_output"
                # Append the results to the respective output file
                echo "$server_count, $run_id, $throughput, $lat_50, $lat_90, $lat_99, $lat_999, $bw_output" >> $output_file
                echo "Recorded throughput for run_id $run_id: $throughput"
            fi
        done
    done
done

echo "Benchmarking complete for all server configurations."
