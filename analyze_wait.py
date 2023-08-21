import sys
import re
import statistics

def calculate_differences(values):
    differences = [values[i] - values[i - 1] for i in range(1, len(values))]
    return differences

def categorize_into_buckets_wait_values(differences):
    buckets = {}
    
    for i in range(0, 50000, 5000):
        label = f"{i}-{i+5000}"
        buckets[label] = 0
    
    for diff in differences:
        for i in range(0, 50000, 5000):
            if i <= diff < i + 5000:
                label = f"{i}-{i+5000}"
                buckets[label] += 1
                break
            
    return buckets

def extract_tid_latencies(log_lines):
    tid_latencies = {}
    
    for line in log_lines:
        if 'tid' in line and 'latency' in line:
            parts = line.split(';')
            tid = parts[0].split(': ')[1].strip()
            latency = int(parts[1].split(': ')[1].strip())
            
            if tid in tid_latencies:
                tid_latencies[tid].append(latency)
            else:
                tid_latencies[tid] = [latency]
    
    return tid_latencies

def bucketize_latencies(latencies):
    bucket_ranges = {
        "<10000": (0, 10000),
        "10000-20000": (10000, 20000),
        "20000-30000": (20000, 30000),
        "30000-40000": (30000, 40000),
        "40000-50000": (40000, 50000),
        "50000-60000": (50000, 60000),
        "60000-70000": (60000, 70000),
        "70000-80000": (70000, 80000),
        "80000-90000": (80000, 90000),
        "90000-100000": (90000, 100000),
        ">100000": (100000, float('inf'))
    }



    
    bucket_counts = {bucket: 0 for bucket in bucket_ranges}
    
    for latency in latencies:
        for bucket, (lower, upper) in bucket_ranges.items():
            if lower <= latency < upper:
                bucket_counts[bucket] += 1
                break
    
    return bucket_counts

def read_logfile(filename, wait_pattern, throughput_pattern, timestamp_pattern, latency_pattern):
    # Read the file and extract values using regular expressions    
    timestamps = []
    wait_values = []
    throughput_values = []
    tid_latencies = {}
    
    with open(filename, 'r') as file:
        lines = file.readlines()
        for line in lines:
            wait_match = re.search(wait_pattern, line)
            throughput_match = re.search(throughput_pattern, line)
            timestamp_match = re.search(timestamp_pattern, line)
            latency_match = re.search(latency_pattern, line)
        
            if wait_match:
                wait_value = int(wait_match.group(1))
                wait_values.append(wait_value)
            elif throughput_match:
                throughput_value = float(throughput_match.group(1))
                throughput_values.append(throughput_value)
                # total_throughput += throughput_value
            elif timestamp_match:
                timestamp = int(timestamp_match.group(1))
                timestamps.append(timestamp)
            elif latency_match:
                tid = latency_match.group(1)
                latency = int(latency_match.group(2))
                
                if tid in tid_latencies:
                    tid_latencies[tid].append(latency)
                else:
                    tid_latencies[tid] = [latency]

    return timestamps, wait_values, throughput_values, tid_latencies

def calculate_differences(timestamps):
    differences = [timestamps[i] - timestamps[i - 1] for i in range(1, len(timestamps))]
    return differences

def categorize_into_buckets(differences):
    buckets = {}
    
    for i in range(0, 61, 2):
        label = f"{i}-{i+2}"
        buckets[label] = 0
    
    for diff in differences:
        for i in range(0, 61, 2):
            if i <= diff < i + 2:
                label = f"{i}-{i+2}"
                buckets[label] += 1
                break
            
    return buckets


def main():
    if len(sys.argv) != 2:
        print("Usage: python script_name.py <file_option>")
        exit()

    file_option = sys.argv[1]

    # Define file paths based on user input
    if file_option == "0":
        input_file_path = "logfile_no_crpc.txt"
        output_file_path = "analysis_60c1s21r1p_no_crpc.txt"
    elif file_option == "1":
        input_file_path = "logfile_crpc.txt"
        output_file_path = "analysis_60c1s21r1p_crpc.txt"
    else:
        print("Invalid file option.")
        exit()

    wait_pattern = r"time of sp_quorum->Wait\(\): (\d+)"        ## for sp->quorum wait in microseconds
    throughput_pattern = r"Throughput: ([\d.]+)"                ## for throughput returned at the end of run 
    timestamp_pattern = r'Current time in milliseconds since the epoch:\s*(\d+)'    ## time when requests are received by the leader; difference in milliseconds
    latency_pattern = r'tid: (\d+); latency: (\d+)'                                 ## time between subsequent requests sent by the client in microseconds
    timestamps_values, wait_values, throughput_values, tid_latencies = read_logfile(input_file_path, wait_pattern, throughput_pattern, timestamp_pattern, latency_pattern)

    total_throughput = sum(throughput_values)

    # differences = calculate_differences(wait_values)
    bucket_counts = categorize_into_buckets_wait_values(wait_values)
    total_count = sum(bucket_counts.values())

    result = f"File: {input_file_path}\n\n"
    result += f"Wait values in micro-seconds\n"
    # print("Bucket Ranges and Counts:")
    for bucket, count in bucket_counts.items():
        percentage = (count / total_count) * 100
        result += f"{bucket}: {count} ({percentage:.2f}%)\n"
    result+= f"\n"
            
    # Calculate the number of occurrences, mean, and median for wait values
    num_wait_occurrences = len(wait_values)
    wait_mean = statistics.mean(wait_values)
    wait_median = statistics.median(wait_values)

    # Calculate the number of occurrences, mean, and median for throughput values
    num_throughput_occurrences = len(throughput_values)
    throughput_mean = statistics.mean(throughput_values)
    throughput_median = statistics.median(throughput_values)
    
    # Prepare the result string

    result += f"Wait - Number of occurrences: {num_wait_occurrences}\n"
    result += f"Wait - Mean: {wait_mean}\n"
    result += f"Wait - Median: {wait_median}\n"
    result+= f"\n"
    result+= f"######################################################\n"
    result+= f"\n"
    result += f"Throughput - Number of occurrences: {num_throughput_occurrences}\n"
    result += f"Throughput - Mean: {throughput_mean}\n"
    result += f"Throughput - Median: {throughput_median}\n"
    result += f"Total Throughput: {total_throughput}\n"

    result+= f"######################################################\n"
    result+= f"\n"

    differences = calculate_differences(timestamps_values)
    bucket_counts = categorize_into_buckets(differences)
    total_count = sum(bucket_counts.values())
    
    result += f"\nDifference between receiving any two requests\n"
    for bucket, count in bucket_counts.items():
        percentage = (count / total_count) * 100
        result += f"{bucket}: {count} ({percentage:.2f}%)\n"

    result+= f"######################################################\n"
    result+= f"\n"
    
    result+= f"\nlatency between subsequent requests\n"
    for tid, latencies in tid_latencies.items():
        bucket_counts = bucketize_latencies(latencies)
        total_latencies = sum(bucket_counts.values())
        
        result+= f"\nBucketized Statistics for tid {tid}:\n"
        for bucket, count in bucket_counts.items():
            percentage = (count / total_latencies) * 100
            result+=f"{bucket}: {count} ({percentage:.2f}%)\n"

    # Write the result to the output file
    with open(output_file_path, "w") as output_file:
        output_file.write(result)

    print("Results written to", output_file_path)



if __name__ == "__main__":
    main()