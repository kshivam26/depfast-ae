import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

def read_data(filename):
    data = pd.read_csv(filename)
    data.columns = data.columns.str.strip()
    return data

def process_data(data):
    # Group by 'number_of_nodes' and calculate median values
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

    # Include bw_util data corresponding to median throughput
    bw_util_data = median_data['bw_util']

    return median_data, errors_df, bw_util_data

def plot_data(chaining_data, broadcast_data, chaining_errors, broadcast_errors, chaining_bw_util, broadcast_bw_util):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(7, 3))  # Adjusted figure size

    # Node values and index setup
    node_values = chaining_data['number_of_nodes'].values  # Ensuring node values are consistent
    index = np.arange(len(node_values))

    # Throughput line graph
    ax1.errorbar(index, chaining_data['throughput'].values, yerr=chaining_errors[['error_min', 'error_max']].values.T, 
                 fmt='-o', label='Chaining', capsize=5, color='g')
    ax1.errorbar(index, broadcast_data['throughput'].values, yerr=broadcast_errors[['error_min', 'error_max']].values.T, 
                 fmt='-o', label='Broadcast', capsize=5, color='b')
    
    # Annotate with bw_util values with dynamic positioning and color coding
    for idx, (bw_util_chaining, bw_util_broadcast, tp_chaining, tp_broadcast) in enumerate(zip(
            chaining_bw_util, broadcast_bw_util, chaining_data['throughput'].values, broadcast_data['throughput'].values)):
        
        # Always position broadcast above the point to avoid clashing with x-axis labels
        vertical_offset_broadcast = 10  # Fixed value above the point
        vertical_offset_chaining = -15 if tp_chaining < tp_broadcast else 10  # Avoid overlap dynamically

        ax1.annotate(f'{bw_util_chaining:.2f}', (index[idx], tp_chaining), 
                     textcoords="offset points", xytext=(0, vertical_offset_chaining), ha='center', color='green')
        ax1.annotate(f'{bw_util_broadcast:.2f}', (index[idx], tp_broadcast), 
                     textcoords="offset points", xytext=(0, vertical_offset_broadcast), ha='center', color='blue')
    
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
    plt.savefig('micro_tp_lat_line_1KB.png')
    plt.savefig("micro_tp_lat_line_1KB.pdf", format='pdf')
    plt.show()

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: python script.py chaining_results.txt broadcast_results.txt")
        sys.exit(1)
    chaining_filename = sys.argv[1]
    broadcast_filename = sys.argv[2]

    chaining_data = read_data(chaining_filename)
    broadcast_data = read_data(broadcast_filename)

    chaining_median_data, chaining_errors, chaining_bw_util = process_data(chaining_data)
    broadcast_median_data, broadcast_errors, broadcast_bw_util = process_data(broadcast_data)

    plot_data(chaining_median_data, broadcast_median_data, chaining_errors, broadcast_errors, chaining_bw_util, broadcast_bw_util)