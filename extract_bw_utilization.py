import csv
from datetime import datetime, timedelta
import os
import sys
from decimal import Decimal, ROUND_HALF_UP

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 2:
    print("Usage: python3 your_python_script.py input_folder")
    sys.exit(1)

# Get input folder path from command-line arguments
input_folder = sys.argv[1]



# Reading data from the file
input_file_path = os.path.join(input_folder, "bwmng_output.csv")
output_file_path = os.path.join(input_folder, "bwmng_output_gbps.csv")

# input_file_path = 'bwmng_output.csv'  # Replace with the actual path to your file
# output_file_path = 'bwmng_output_gbps.csv'  # Replace with the desired path for the output file

# Initialize the dictionary to store counts for each bucket
bucket_counts = {}

values = []

try: 
    with open(input_file_path, 'r') as input_file:
        reader = csv.reader(input_file, delimiter=';')

        # Initialize the timestamp based on the first row
        first_row = next(reader)
        start_timestamp = float(first_row[0])

        for row in reader:
            if row[1] == 'eth0':
                # Assuming there is always a value after 'eth0'
                bytes_per_second = float(row[2])
                # print (bytes_per_second)
                giga_bits_per_second = '{:.2f}'.format((bytes_per_second * 8) / 1e9)[:-1]
                values.append(giga_bits_per_second)
                # print(giga_bits_per_second)
                # Use giga_bits_per_second directly as the key for the dictionary
                if giga_bits_per_second != '0.0':
                    # Use giga_bits_per_second directly as the key for the dictionary
                    bucket_counts[giga_bits_per_second] = bucket_counts.get(giga_bits_per_second, 0) + 1
            
# for value in values:
#     print(value)s,
    

# # Output bucket counts
# for key, count in sorted(bucket_counts.items(), key=lambda x: float(x[0])):
#     start_range = float(key)
#     end_range = round(start_range + 0.1, 1)
#     print(f'Bucket {start_range}-{end_range}: {count} values')

    # print(bucket_counts)
    # Find the bucket with the highest count
    if len(bucket_counts) > 1:
        max_count_bucket = max(bucket_counts, key=bucket_counts.get)
    else:
        max_count_bucket = 0


    
    # Calculate the mid-point of the bucket with the highest count
    mid_point = Decimal(max_count_bucket) + Decimal('0.05')

    # Round the result to two decimal places using ROUND_HALF_UP
    mid_point = mid_point.quantize(Decimal('0.01'), rounding=ROUND_HALF_UP)


    print(mid_point)
    
except Exception as e:
    print(f"Error: {e}")

finally:
    # Remove the CSV file after processing
    try:
        os.remove(input_file_path)
    except OSError as e:
        print(f"Error deleting file: {e}")

# Exit with a success code
sys.exit(0)