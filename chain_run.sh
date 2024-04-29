#!/bin/bash

# List containers named container1, container2, or container3

# Function to clean up and kill background processes
cleanup() {
  echo "Cleaning up and killing background processes..."
  # Kill all background processes in the current process group
  kill -- -$$
  exit
}

# Trap the exit signal and call the cleanup function
trap 'cleanup' EXIT

stop_containers(){
existing_containers=$(sudo docker ps -a -q --filter "name=chain_l|chain_f1|chain_f2")

echo "$existing_containers"

# If any found, stop and remove them
if [[ -n "$existing_containers" ]]; then
  echo "Stopping and removing existing containers..."
  sudo docker stop $existing_containers
  sudo docker rm $existing_containers
fi

echo "1*****"
}

stop_containers

start_containers(){
# Start containers (replace with your container names/IDs and images)
sudo docker run -d --name chain_l --net my_network --ip 172.19.0.11 -it --cap-add=NET_ADMIN --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup -v /home/users/kkumar/rolis_new/rolis:/root/rolis crpc_rolis
sudo docker run -d --name chain_f1 --net my_network --ip 172.19.0.12 -it --cap-add=NET_ADMIN --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup -v /home/users/kkumar/rolis_new/rolis:/root/rolis crpc_rolis
sudo docker run -d --name chain_f2 --net my_network --ip 172.19.0.13 -it --cap-add=NET_ADMIN --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup -v /home/users/kkumar/rolis_new/rolis:/root/rolis crpc_rolis
}

start_containers

build_code(){
echo "building the code"
sudo docker exec -i chain_l /root/rolis/init_crpc.sh
echo "now going to run the crpc system"
}

build_code

# Define arrays containing parameter values
num_threads=(1)   # Replace these values with your actual num_threads values
crpc_options=(4)         # Replace these values with your actual crpc_options values
# num_threads=(15)   # Replace these values with your actual num_threads values
# crpc_options=(4) 
bw_options=(0) # for bw 5, #threads=15, the bw for dynamic doesn't reach more than 3.1gbps and the leader cpu is fully utilized
# batch_options=(200 500 1000 2000 3000)
MAX_RETRIES=3


echo "cp1_chain_run"
# evaluation_file="crpc_evaluation/evaluation.txt"
for bw_option in "${bw_options[@]}"; do
  # Get the current date and time
  current_datetime=$(date +"%Y%m%d_%H%M%S")
  
  # Create the output folder with the current date and time
  # sub_output_folder="evalslowdynamic_threads_output_${bw_option}_gbps_${current_datetime}"
  sub_output_folder="evalSlow_tp_threads/eval_${bw_option}_gbps_${current_datetime}"
  output_folder="crpc_evaluation/${sub_output_folder}"
  mkdir "$output_folder"

# Iterate through each combination of parameters
  for thread_value in "${num_threads[@]}"; do
      for option_value in "${crpc_options[@]}"; do        
        # Delete the input file
        latency_input_file="crpc_evaluation/output_latency.txt"
        rm "$latency_input_file"
        echo "File $latency_input_file deleted. cp1"

        for ((i = 1; i <= 3; i++)); do
          throughput_value=""  # Initialize throughput value
          retries=0
          # Retry the iteration until throughput_value is obtained
          while [[ -z $throughput_value ]]; do
            sudo docker exec -i chain_l /root/rolis/init_crpc_bw.sh "$bw_option"
            sudo docker exec -i chain_f1 /root/rolis/init_crpc_bw.sh "$bw_option"
            sudo docker exec -i chain_f2 /root/rolis/init_crpc_bw.sh "$bw_option" "40"
            echo "now executing the system with $thread_value and $option_value paramters"

            # Define the output file path inside the output directory based on thread_value and option_value
            output_file="${output_folder}/leader_output_${bw_option}_${thread_value}_${option_value}.txt"

            # Execute commands with the parameters
            timeout 4m sudo docker exec -i chain_l /root/rolis/cpuset_b0.sh "$thread_value" "$option_value" >& $output_file &
            pid1=$!
            echo "$pid1"

            timeout 4m sudo docker exec -i chain_f1 /root/rolis/cpuset_b1.sh "$thread_value" "$option_value" >&  crpc_evaluation/f1.txt &
            pid2=$!
            echo "$pid2"

            timeout 4m sudo docker exec -i chain_f2 /root/rolis/cpuset_b2.sh "$thread_value" "$option_value" 1000 0 >& crpc_evaluation/f2.txt&
            pid3=$!
            echo "$pid3"

            timeout 4m sudo docker exec -i chain_l bwm-ng --interfaces eth0 -t 1000 -o csv > ${output_folder}/bwmng_output.csv &
            pid4=$!
            echo "$pid4"

            timeout 4m crpc_evaluation/retrieve_cpu_util.sh 5 ${thread_value+1} ${output_folder} &
            pid5=$!
            echo "$pid5"

            # Wait for these processes to complete
            # wait $pid1 || wait $pid2 || wait $pid3
            # Wait for either pid1, pid2, or pid3 to finish/abort
            while : ; do
                if ! kill -0 $pid1 2>/dev/null; then
                    echo "pid1 ($pid1) finished"
                    break
                elif ! kill -0 $pid2 2>/dev/null; then
                    echo "pid2 ($pid2) finished"
                    break
                elif ! kill -0 $pid3 2>/dev/null; then
                    echo "pid3 ($pid3) finished"
                    break
                fi
                sleep 1
            done
            echo "leader process done"
            kill -- -$pid5
            kill -- -$pid4
            kill -- -$pid1
            kill -- -$pid2
            kill -- -$pid3
            echo "Killed processes with pid: $pid2 and $pid3"

            # Get throughput value from the output file
            throughput_value=$(grep -oP 'throughput without warmup and cool-down: \K[\d.]+' "$output_file")
          
            # If throughput value is not found, print a message and retry the iteration
            # Increment retries if throughput value is not found
            if [[ -z $throughput_value ]]; then
                ((retries++))
                if (( retries >= MAX_RETRIES )); then
                    echo "Max retries reached. Thread: $thread_value, Option: $option_value"
                    break
                else
                    echo "Throughput value not found. Retrying for thread: $thread_value, option: $option_value (Retry: $retries)"
                fi
            fi
            
            echo "throughput value: $throughput_value"
            stop_containers
            start_containers
            build_code
          done
        
          # calculate latency values
          input_file="$output_file"

          # Extract latency values and calculate percentiles
          # latency_values=($(grep -oP 'latency: \K\d+ microseconds' "$input_file" | sed 's/ microseconds//'))
          # latency_input_file="crpc_evaluation/output_latency.txt"
          readarray -t latency_values < "$latency_input_file"
          num_values=${#latency_values[@]}

          # Delete the input file
          rm "$latency_input_file"
          echo "File $latency_input_file deleted. cp2"

        # Check if there are any latency values
        if [ $num_values -eq 0 ]; then
            echo "No latency values found in the input file."
            exit 1
        fi

        # Calculate mean
        sum=0
        for latency in "${latency_values[@]}"; do
            # Strip leading zeros by forcing the base to be decimal
            latency=$((10#$latency))
            sum=$((sum + latency))
        done
        mean=$((sum / num_values))

        # Sort the array
        sorted_latencies=($(for latency in "${latency_values[@]}"; do echo "$latency"; done | sort -n))

        # Save sorted latencies to a file
        # printf "%s\n" "${sorted_latencies[@]}" > sorted_latencies.txt

        # Calculate percentiles with linear interpolation
        p50="${sorted_latencies[num_values / 2]}"
        p90="${sorted_latencies[num_values * 9 / 10]}"
        p95="${sorted_latencies[num_values * 95 / 100]}"
        p99_index=$((num_values * 99 / 100))
        p99_lower="${sorted_latencies[p99_index]}"
        p99_upper="${sorted_latencies[p99_index + 1]}"
        p99=$((p99_lower + (p99_upper - p99_lower) / 2))
        p999="${sorted_latencies[num_values * 999 / 1000]}"

          evaluation_file="${output_folder}/evaluation_data.txt"

        bw_output=$(python3 crpc_evaluation/extract_bw_utilization.py "$output_folder")
        cpu_output=$(python3 crpc_evaluation/extract_cpu_utilization.py "$output_folder")
            # Print the mid_point value
            echo "Mid-point value: $bw_output"
          # Write evaluation data to the evaluation file
          echo "${thread_value}, ${option_value}, ${throughput_value}, ${p50}, ${p90}, ${p95}, ${p99}, ${p999}", "${bw_output}", "${cpu_output}">> "$evaluation_file"
        done
        
      done
  done
  python3 crpc_evaluation/plot_graphs.py "$output_folder"
done

