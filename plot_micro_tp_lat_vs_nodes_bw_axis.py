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
    # Increase the height of the plots in figsize
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8, 3.5))  # Increased height from 3 to 4

    # Node values and index setup
    node_values = chaining_data['number_of_nodes'].values
    index = np.arange(len(node_values))

    # Throughput line graph with error bars
    ax1.errorbar(index, chaining_data['throughput'].values, yerr=chaining_errors[['error_min', 'error_max']].values.T,
                 fmt='-o', label='Chaining TPUT', capsize=5, color='g')
    ax1.errorbar(index, broadcast_data['throughput'].values, yerr=broadcast_errors[['error_min', 'error_max']].values.T,
                 fmt='-o', label='Broadcast TPUT', capsize=5, color='b')
    ax1.set_xlabel('# Nodes')
    ax1.set_ylabel('Throughput (req/s)')
    ax1.set_xticks(index)
    ax1.set_xticklabels(node_values)
    ax1.grid(True, axis='y', linestyle='--', alpha=0.7)

    # Create a second y-axis for bandwidth
    ax1b = ax1.twinx()
    ax1b.plot(index, np.array(chaining_bw_util), '-^', linestyle='--', color='darkgreen', label='Chaining B/W')
    ax1b.plot(index, np.array(broadcast_bw_util), '-^', linestyle='--', color='darkblue', label='Broadcast B/W')
    ax1b.set_ylabel('B/W Util (Gbps)')

    # Set the limits for the y-axes to prevent overlapping lines
    throughput_max = np.max([chaining_data['throughput'].max(), broadcast_data['throughput'].max()])
    bandwidth_max = np.max([np.array(chaining_bw_util).max(), np.array(broadcast_bw_util).max()])
    ax1.set_ylim(0, throughput_max * 1.1)  # 10% higher than the max value
    ax1b.set_ylim(0, bandwidth_max * 1.1)  # 10% higher than the max value

    # Combined legend for both axes placed above the plot to the left
    lines, labels = ax1.get_legend_handles_labels()
    lines2, labels2 = ax1b.get_legend_handles_labels()
    ax1.legend(lines + lines2, labels + labels2, loc='upper left', bbox_to_anchor=(0, 1.25), ncol=2, fontsize='small', frameon=False)

    # Latency line graph using median run data
    ax2.plot(index, chaining_data['50%_lat'].values, label='Chaining P50', marker='o', linestyle='--', color='g')
    ax2.plot(index, chaining_data['90%_lat'].values, label='Chaining P90', marker='o', color='g')
    ax2.plot(index, broadcast_data['50%_lat'].values, label='Broadcast P50', marker='o', linestyle='--', color='b')
    ax2.plot(index, broadcast_data['90%_lat'].values, label='Broadcast P90', marker='o', color='b')
    ax2.set_xlabel('# Nodes')
    ax2.set_ylabel('Latency (ms)')
    ax2.set_xticks(index)
    ax2.set_xticklabels(node_values)
    ax2.legend(loc='upper left', bbox_to_anchor=(0, 1.25), ncol=2, fontsize='small', frameon=False)
    ax2.grid(True, axis='y', linestyle='--', alpha=0.7)

    # Adjust layout to reduce white space
    plt.subplots_adjust(top=0.85, bottom=0.15, left=0.1, right=0.9, hspace=0.2, wspace=0.3)

    plt.tight_layout()
    plt.savefig('micro_tp_lat_large.png')
    plt.savefig("micro_tp_lat_large.pdf", format='pdf')
    plt.show()



# import numpy as np  # Ensure NumPy is imported

# def plot_data(chaining_data, broadcast_data, chaining_errors, broadcast_errors, chaining_bw_util, broadcast_bw_util):
#     fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8, 3))  # Adjusted figure size

#     # Node values and index setup
#     node_values = chaining_data['number_of_nodes'].values
#     index = np.arange(len(node_values))

#     # Throughput line graph with error bars
#     ax1.errorbar(index, chaining_data['throughput'].values, yerr=chaining_errors[['error_min', 'error_max']].values.T,
#                  fmt='-o', label='Chaining Throughput', capsize=5, color='g')
#     ax1.errorbar(index, broadcast_data['throughput'].values, yerr=broadcast_errors[['error_min', 'error_max']].values.T,
#                  fmt='-o', label='Broadcast Throughput', capsize=5, color='b')
#     ax1.set_xlabel('# Nodes')
#     ax1.set_ylabel('Throughput (req/s)')
#     ax1.set_xticks(index)
#     ax1.set_xticklabels(node_values)
#     ax1.legend(loc='upper center', bbox_to_anchor=(0.5, 1.15), ncol=2)
#     ax1.grid(True, axis='y', linestyle='--', alpha=0.7)

#     # Convert Pandas Series to Numpy arrays before plotting bandwidth utilization
#     chaining_bw_util_np = np.array(chaining_bw_util)
#     broadcast_bw_util_np = np.array(broadcast_bw_util)
#     ax1.plot(index, chaining_bw_util_np, '-^', linestyle='--', color='darkgreen', label='Chaining BW Util')
#     ax1.plot(index, broadcast_bw_util_np, '-^', linestyle='--', color='darkblue', label='Broadcast BW Util')
#     for i, (bw_chaining, bw_broadcast) in enumerate(zip(chaining_bw_util_np, broadcast_bw_util_np)):
#         ax1.annotate(f'{bw_chaining:.2f}%', (index[i], bw_chaining), textcoords="offset points", xytext=(0,10), ha='center', color='darkgreen')
#         ax1.annotate(f'{bw_broadcast:.2f}%', (index[i], bw_broadcast), textcoords="offset points", xytext=(0,-15), ha='center', color='darkblue')

#     # Latency line graph using median run data
#     ax2.plot(index, chaining_data['50%_lat'].values, label='Chaining P50', marker='o', linestyle='--', color='g')
#     ax2.plot(index, chaining_data['90%_lat'].values, label='Chaining P90', marker='o', color='g')
#     ax2.plot(index, broadcast_data['50%_lat'].values, label='Broadcast P50', marker='o', linestyle='--', color='b')
#     ax2.plot(index, broadcast_data['90%_lat'].values, label='Broadcast P90', marker='o', color='b')
#     ax2.set_xlabel('# Nodes')
#     ax2.set_ylabel('Latency (ms)')
#     ax2.set_xticks(index)
#     ax2.set_xticklabels(node_values)
#     ax2.legend(loc='upper center', bbox_to_anchor=(0.5, 1.15), ncol=2)
#     ax2.grid(True, axis='y', linestyle='--', alpha=0.7)

#     plt.tight_layout()
#     plt.savefig('micro_tp_lat_large.png')
#     plt.savefig("micro_tp_lat_large.pdf", format='pdf')
#     plt.show()



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