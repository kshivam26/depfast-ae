import re
import statistics




def main():
    filename = "logfile_no_crpc.txt"
    timestamps_values = read_logfile(filename)
    
    differences = calculate_differences(timestamps_values)
    bucket_counts = categorize_into_buckets(differences)
    total_count = sum(bucket_counts.values())
    
    print("Bucket Ranges and Counts:")
    for bucket, count in bucket_counts.items():
        percentage = (count / total_count) * 100
        print(f"{bucket}: {count} ({percentage:.2f}%)")

if __name__ == "__main__":
    main()

