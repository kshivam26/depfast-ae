import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

def read_data(filename):
    data = pd.read_csv(filename)
    data.columns = data.columns.str.strip()
    return data

def process_data(data):
    # Group by 'number_of_nodes' and calculate median values and the range for errors
    grouped = data.groupby('number_of_nodes')
    median_data = grouped.median().reset_index()
    min_data = grouped.min().reset_index()
    max_data = grouped.max().reset_index()

    # Calculate error bars: the range from min to max for each node count
    errors = {
        node: [[median_data.at[idx, 'throughput'] - min_data.at[idx, 'throughput']],
               [max_data.at[idx, 'throughput'] - median_data.at[idx, 'throughput']]]
        for idx, node in enumerate(median_data['number_of_nodes'])
    }
    errors_df = pd.DataFrame(errors).T.explode(0).explode(1)
    errors_df.columns = ['error_min', 'error_max']

    return median_data, errors_df

def plot_data(chaining_data, broadcast_data, chaining_errors, broadcast_errors):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(7, 3))  # Adjusted figure size

    # Bar width and offset
    bar_width = 0.35
    node_values = chaining_data['number_of_nodes'].values  # Ensuring node values are consistent
    index = np.arange(len(node_values))

    # Throughput bar graph
    ax1.bar(index - bar_width/2, chaining_data['throughput'].values, bar_width, label='Chaining', 
            yerr=chaining_errors[['error_min', 'error_max']].values.T, capsize=5, color='g')
    ax1.bar(index + bar_width/2, broadcast_data['throughput'].values, bar_width, label='Broadcast', 
            yerr=broadcast_errors[['error_min', 'error_max']].values.T, capsize=5, color='b')
    ax1.set_xlabel('# Nodes')
    ax1.set_ylabel('Throughput (req/s)')
    ax1.set_xticks(index)
    ax1.set_xticklabels(node_values)
    ax1.legend()
    ax1.grid(True, axis='y', linestyle='--', alpha=0.7)  # Enable grid

    # Latency line graph using median run data
    ax2.plot(index, chaining_data['50%_lat'].values, label='Chaining P50', marker='o', linestyle='--', color='g')
    ax2.plot(index, chaining_data['90%_lat'].values, label='Chaining P90', marker='o', color='g')
    ax2.plot(index, broadcast_data['50%_lat'].values, label='Broadcast P50', marker='o', linestyle='--', color='b')
    ax2.plot(index, broadcast_data['90%_lat'].values, label='Broadcast P90', marker='o', color='b')
    ax2.set_xlabel('# Nodes')
    ax2.set_ylabel('Latency (ms)')
    ax2.set_xticks(index)
    ax2.set_xticklabels(node_values)
    ax2.legend()
    ax2.grid(True, axis='y', linestyle='--', alpha=0.7)  # Enable grid

    plt.tight_layout()
    plt.savefig('micro_tp_lat_large.png')
    plt.savefig("micro_tp_lat_large.pdf", format='pdf')
    plt.show()

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: python script.py chaining_results.txt broadcast_results.txt")
        sys.exit(1)
    chaining_filename = sys.argv[1]
    broadcast_filename = sys.argv[2]

    chaining_data = read_data(chaining_filename)
    broadcast_data = read_data(broadcast_filename)

    chaining_median_data, chaining_errors = process_data(chaining_data)
    broadcast_median_data, broadcast_errors = process_data(broadcast_data)

    plot_data(chaining_median_data, broadcast_median_data, chaining_errors, broadcast_errors)
