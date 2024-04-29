#!/bin/bash

# Variables
network_name="my_network"
subnet="172.19.0.0/16"
base_ip="172.19.0."
image_name="microbench"
start_ip=10
number_of_nodes=$1  # Number of nodes passed as the first script argument

# # Create Docker network if it doesn't already exist
# if ! docker network ls | grep -q "$network_name"; then
#     sudo docker network create --subnet $subnet $network_name
# fi

# Function to convert IP to a number
ip_to_num() {
    local IFS='.'
    read ip1 ip2 ip3 ip4 <<< "$1"
    echo "$((ip1 * 256 ** 3 + ip2 * 256 ** 2 + ip3 * 256 + ip4))"
}

# Function to convert number to IP
num_to_ip() {
    local num=$1
    local ip4=$((num % 256)); num=$((num / 256))
    local ip3=$((num % 256)); num=$((num / 256))
    local ip2=$((num % 256)); num=$((num / 256))
    local ip1=$((num))
    echo "$ip1.$ip2.$ip3.$ip4"
}

# Function to stop and remove all containers
cleanup_containers() {
    echo "Stopping and removing existing containers..."
    sudo docker ps -a --filter "name=micro_" --format "{{.Names}}" | xargs -r sudo docker stop
    sudo docker ps -a --filter "name=micro_" --format "{{.Names}}" | xargs -r sudo docker rm
    echo "Cleanup complete."
}

# Stop and remove all existing containers before setting up new ones
cleanup_containers


declare -a ips

# Main loop to create and run containers
for (( i=0; i<number_of_nodes; i++ )); do
    current_ip="${base_ip}$((start_ip + i + 1))"  # Directly calculate the IP string

    # Save IP for use in container command
    ips[i]=$current_ip

    # Container name
    container_name="micro_$((i+1))"  # Ensure correct increment

    echo "now running docker run"
    echo "container name $container_name"  # Debugging output

    # Run the Docker container with dynamic IP
    sudo docker run -d --name "$container_name" --net $network_name --ip "${ips[i]}" -it \
        --cap-add=NET_ADMIN --cap-add=SYS_ADMIN \
        -v /sys/fs/cgroup:/sys/fs/cgroup \
        -v /home/users/kkumar/micro_benchmark/depfast-ae:/root/depfast $image_name
done

# # Pass IPs to containers if needed (example command to update configuration inside container)
# for (( i=0; i<number_of_nodes; i++ )); do
#     container_name="chain_${i}"
#     docker exec "$container_name" some_command_to_update_config "${ips[@]}"
# done

echo "Deployment of $number_of_nodes nodes complete."