import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# This script reads a .txt file containing performance data, calculates the median throughput
# for each slowness level based on multiple runs, and plots throughput vs slowness. The plot is 
# saved as a PDF file named "tp_vs_slowness.pdf".

# Input: A .txt file named 'chaining_output_raft_1KB_docker_slowness_new.txt' with the following format:
# number_of_nodes, run_id, throughput, 50%_lat, 90%_lat, 99%_lat, 99.9%_lat, bw_util, slowness
# 13, 1, 31403.050000, 4.832121, 5.257172, 5.501106, 5.554343, 0.25, 1
# 13, 2, 31013.500000, 4.852722, 5.288566, 5.538779, 5.590607, 0.25, 2
# ...

# Output: A plot showing median throughput vs slowness, saved as 'tp_vs_slowness.pdf'.

# Define the path to the input text file
file_path = 'chaining_output_raft_1KB_docker_slowness_new.txt'

# Read the data from the text file
data = pd.read_csv(file_path, delimiter=',', skipinitialspace=True)

# Function to get the median row for each group
def median_row(group):
    median_idx = group['throughput'].median()
    return group.loc[(group['throughput'] - median_idx).abs().idxmin()]

# Calculate the median throughput for each slowness level
median_throughput = data.groupby('slowness').apply(median_row).reset_index(drop=True)

# Convert to numpy arrays to avoid multi-dimensional indexing issue
slowness = median_throughput['slowness'].to_numpy()
throughput = median_throughput['throughput'].to_numpy()

# Sort the data by slowness for plotting
sorted_indices = np.argsort(slowness)
slowness = slowness[sorted_indices]
throughput = throughput[sorted_indices]

# Generate equally spaced x values for plotting
x_values = np.arange(len(slowness))

# Plot throughput vs slowness with equally spaced x-axis
plt.figure(figsize=(9,6))
plt.plot(x_values, throughput, marker='o')
plt.xlabel('Slowness levels', fontsize='x-large')
plt.ylabel('Throughput', fontsize='x-large')
# plt.title('Median Throughput vs Slowness')
# plt.grid(True)

# Set x-ticks to be equally spaced but labeled with actual slowness values appended with 'x'
slowness_labels = [f'{int(level)}x' for level in slowness]
plt.xticks(x_values, slowness_labels, fontsize='x-large')
plt.yticks(fontsize='x-large')

# Save the plot as a PDF file
plt.savefig('tp_vs_slowness.pdf')

# Show the plot
plt.show()
