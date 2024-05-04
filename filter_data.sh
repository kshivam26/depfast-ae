#!/bin/bash

# Check if a file name is provided
if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <csv_file>"
  exit 1
fi

# File path from the first command-line argument
input_file="$1"

# Output file derived from input file name
output_file="${input_file%.*}_filtered.csv"

# Check if the file exists
if [ ! -f "$input_file" ]; then
  echo "Error: File not found."
  exit 1
fi

# Prepare the output file with the header
echo "server_threads,client_threads,QPS,50% latency,90% latency,99% latency,99.9% latency" > "$output_file"

# Read the CSV file, skipping the header
tail -n +2 "$input_file" | {
  # Read lines in groups of three
  while read -r line1 && read -r line2 && read -r line3; do
    # Extract the QPS values from each line
    qps1=$(echo "$line1" | cut -d',' -f3)
    qps2=$(echo "$line2" | cut -d',' -f3)
    qps3=$(echo "$line3" | cut -d',' -f3)

    # Put QPS values and lines into an array
    declare -a qpsArray=("$qps1" "$qps2" "$qps3")
    declare -a lineArray=("$line1" "$line2" "$line3")

    # Sort QPS values and get the median
    IFS=$'\n' sortedQps=($(sort -n <<<"${qpsArray[*]}"))
    medianQps=${sortedQps[1]}

    # Find which line has the median QPS and write it to the output file
    for i in "${!qpsArray[@]}"; do
      if [[ "${qpsArray[$i]}" == "$medianQps" ]]; then
        echo "${lineArray[$i]}" >> "$output_file"
        break
      fi
    done
  done
}

exit 0
