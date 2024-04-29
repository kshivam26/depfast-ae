#!/bin/bash

# Define the CSV file path
output_file="output_crpc_1MB.csv" 

# Write the CSV header
echo "server_threads,client_threads,QPS,50% latency,90% latency,99% latency,99.9% latency" > $output_file

# Server and client thread numbers
server_threads=(33 29 25 21 17 13 9 5 3)
client_threads=(1 10 20 30 50 100)

# Iterate over the server thread numbers
for st in "${server_threads[@]}"; do
  # Start the server in the background
  
  # Iterate over the client thread numbers
  for ct in "${client_threads[@]}"; do
    

    for i in {1..3}; do
      # Repeat the experiment 3 times for each combination
      ./build/crpc_benchmark -s 127.0.0.1:11000 -b 0 -w 1 -e 1 -x $st &
      # Get the server's PID so we can kill it later
      server_pid=$!

      sleep 5
      # Run the client and capture its output
      output=$(./build/crpc_benchmark -c 127.0.0.1:12001 -t $ct -w 1 -n 60 -f -b 0 -o 1)

      # Parse the output to extract the required metrics
      qps=$(echo $output | grep -oP 'QPS: \K[^;]+')
      lat_50=$(echo $output | grep -oP '50.0% LATENCY: \K[^;]+')
      lat_90=$(echo $output | grep -oP '90.0% LATENCY: \K[^;]+')
      lat_99=$(echo $output | grep -oP '99.0% LATENCY: \K[^;]+')
      lat_999=$(echo $output | grep -oP '99.9% LATENCY: \K[^;]+')

      # Write the results to the CSV file
      echo "$st,$ct,$qps,$lat_50,$lat_90,$lat_99,$lat_999" >> $output_file

      # Kill the server
      kill $server_pid
      wait $server_pid 2>/dev/null
    done

    
  done

  
done
