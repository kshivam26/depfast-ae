import numpy as np
import matplotlib.pyplot as plt
import sys

def read_throughput(filename):
    with open(filename, 'r') as file:
        data = file.readlines()

    results = {}
    for line in data[1:]:  # Skip the header
        parts = line.strip().split(',')
        nodes = int(parts[0])
        throughput = float(parts[-1])
        if nodes not in results:
            results[nodes] = []
        results[nodes].append(throughput)

    # Calculate the median throughput and the range (min, max) for each number of nodes
    medians = {}
    error_bars = {}
    for nodes, throughputs in results.items():
        median = np.median(throughputs)
        medians[nodes] = median
        error_bars[nodes] = [median - min(throughputs), max(throughputs) - median]  # Min and max error
    return medians, error_bars

def plot_comparison(chaining_file, broadcast_file):
    # Read data from files
    chaining_medians, chaining_errors = read_throughput(chaining_file)
    broadcast_medians, broadcast_errors = read_throughput(broadcast_file)

    # Prepare data for plotting
    nodes = sorted(set(chaining_medians.keys()).union(broadcast_medians.keys()))
    chaining_values = [chaining_medians.get(node, 0) for node in nodes]
    broadcast_values = [broadcast_medians.get(node, 0) for node in nodes]
    chaining_error_values = np.array([[chaining_errors.get(node, [0, 0])[0] for node in nodes], 
                                      [chaining_errors.get(node, [0, 0])[1] for node in nodes]])
    broadcast_error_values = np.array([[broadcast_errors.get(node, [0, 0])[0] for node in nodes], 
                                       [broadcast_errors.get(node, [0, 0])[1] for node in nodes]])

    # Set up the bar width
    bar_width = 0.35
    index = np.arange(len(nodes))

    # Create the plot
    fig, ax = plt.subplots()
    bars1 = ax.bar(index - bar_width/2, chaining_values, bar_width, label='Chaining',
                   yerr=chaining_error_values, capsize=5)
    bars2 = ax.bar(index + bar_width/2, broadcast_values, bar_width, label='Broadcast',
                   yerr=broadcast_error_values, capsize=5)

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_xlabel('# Nodes')
    ax.set_ylabel('Throughput (req/s)')
    # ax.set_title('Throughput Comparison by Number of Nodes')
    ax.set_xticks(index)
    ax.set_xticklabels(nodes)
    ax.legend()

    plt.tight_layout()
    
    # Save the plot before showing it
    plt.savefig('microbench_crpc.png', format='png')
    plt.savefig('microbench_crpc.pdf', format='pdf')

    # Display the plot
    plt.show()

# Example usage with command-line arguments
if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: python script.py chaining_results.txt broadcast_results.txt")
        sys.exit(1)
    plot_comparison(sys.argv[1], sys.argv[2])
