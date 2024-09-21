import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys
import matplotlib.ticker as ticker

def read_data(filename):
    data = pd.read_csv(filename)
    data.columns = data.columns.str.strip()
    return data

def kilo_formatter(x, pos):
    """Converts numbers to thousands with 'K' notation."""
    return f'{int(x/1000)}K'

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
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8.5, 3.5))  # Adjusted figure size

    # Bar width and offset
    bar_width = 0.35
    node_values = chaining_data['number_of_nodes'].values  # Ensuring node values are consistent
    index = np.arange(len(node_values))

    # Throughput bar graph
    ax1.bar(index - bar_width/2, chaining_data['throughput'].values, bar_width, label='Chaining', 
            yerr=chaining_errors[['error_min', 'error_max']].values.T, capsize=5, color='g',alpha=0.7)
    ax1.bar(index + bar_width/2, broadcast_data['throughput'].values, bar_width, label='Broadcast', 
            yerr=broadcast_errors[['error_min', 'error_max']].values.T, capsize=5, color='b',alpha=0.7)
    ax1.set_xlabel('# Nodes', fontsize='x-large')
    ax1.set_ylabel('Throughput (req/s)', fontsize='x-large')
    ax1.set_xticks(index)
    ax1.set_xticklabels(node_values)
    ax1.tick_params(axis='both', which='both', labelsize='x-large')
    # ax1.legend(loc='upper left', bbox_to_anchor=(0, 1.25), ncol=2, fontsize='x-large', frameon=False)
    ax1.legend(fontsize='x-large')
    ax1.grid(True, axis='y', linestyle='--', alpha=0.7)  # Enable grid
    # lines, labels = ax1.get_legend_handles_labels()
    # ax1.legend(lines, labels, loc='upper center', bbox_to_anchor=(0.5, 1.3), ncol=2, fontsize='medium', frameon=False)

    ax1.yaxis.set_major_formatter(ticker.FuncFormatter(kilo_formatter))
    ax1.yaxis.set_major_locator(ticker.MaxNLocator(5))

    # Latency line graph using median run data
    ax2.plot(index, chaining_data['50%_lat'].values, label='Chaining P50', marker='o', linestyle='--', color='g',alpha=0.7)
    ax2.plot(index, chaining_data['90%_lat'].values, label='Chaining P90', marker='o', color='g',alpha=0.7)
    ax2.plot(index, broadcast_data['50%_lat'].values, label='Broadcast P50', marker='o', linestyle='--', color='b',alpha=0.7)
    ax2.plot(index, broadcast_data['90%_lat'].values, label='Broadcast P90', marker='o', color='b',alpha=0.7)
    ax2.set_xlabel('# Nodes', fontsize='x-large')
    ax2.set_ylabel('Latency (ms)', fontsize='x-large')
    ax2.set_xticks(index)
    ax2.set_xticklabels(node_values, fontsize='x-large')
    ax2.tick_params(axis='both', which='both', labelsize='x-large')
    # ax2.legend(loc='upper left', bbox_to_anchor=(0, 1.3), ncol=2, fontsize='x-large', frameon=False)
    ax2.legend(fontsize='x-large')
    ax2.grid(True, axis='y', linestyle='--', alpha=0.7)  # Enable grid

    plt.tight_layout()
    plt.savefig('micro_raft_tp_lat_10B.png')
    plt.savefig("micro_raft_tp_lat_10B.pdf", format='pdf')
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
