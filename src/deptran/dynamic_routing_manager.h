#include "__dep__.h"
#include <iostream>
#include <queue>
#include <functional>
// #include <chrono>

namespace janus
{
    const int HIGH_INT_VALUE = 1e9; // A high placeholder value for end_time

    struct LatencyRequestData {
        uint64_t id;
        std::chrono::system_clock::time_point start_time;
        std::chrono::system_clock::time_point end_time;

        LatencyRequestData(uint64_t identifier, std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end = std::chrono::system_clock::time_point::max()) : id(identifier), start_time(start), end_time(end) {}
    };

    class DRLatencyQueue {
    public:
        DRLatencyQueue(size_t n): maxSize(n){}
        DRLatencyQueue(){}
        void add(uint64_t id, std::chrono::system_clock::time_point start_time);
        void updateEndTime(uint64_t id, std::chrono::system_clock::time_point end_time);
        double findMedian();
        void display();
        LatencyRequestData getLastRequest();

    private:
        std::queue<LatencyRequestData> q;
        size_t maxSize;
        std::priority_queue<std::pair<int, uint64_t>> maxHeap; // {latency, id}
        std::priority_queue<std::pair<int, uint64_t>, std::vector<std::pair<int, uint64_t>>, std::greater<std::pair<int, uint64_t>>> minHeap; // {latency, id}
        uint64_t requestCounter;

        void removeOldest();
        void addToHeaps(std::pair<int, uint64_t> value);
        void removeLatencyFromHeaps(std::pair<int, uint64_t> latencyId);
        void removeFromHeap(std::priority_queue<std::pair<int, uint64_t>>& heap, std::pair<int, uint64_t> value);
        void removeFromHeap(std::priority_queue<std::pair<int, uint64_t>, std::vector<std::pair<int, uint64_t>>, std::greater<std::pair<int, uint64_t>>>& heap, std::pair<int, uint64_t> value);
        void rebalanceHeaps();
    };

    class DynamicRoutingManager
    {
    public:
        DynamicRoutingManager() {}
        DynamicRoutingManager(const DynamicRoutingManager &) {}
        
        map<int, DRLatencyQueue> latency_queues;
        vector<int> resultVector;
        int timeout_period = 1000000;
        bool no_change = false;
        bool loop_var = true;
        bool started = false;
        bool groupIsChanged = false;
        int num_no_change = 0;
        std::unordered_set<int> groupFast, groupSlow;
        SpinLock groups_lock_;
        void add_request_start_time(uint64_t req_id, uint64_t node_id);
        void add_request_end_time(uint64_t req_id, uint64_t node_id);
        vector<int> getNodesOrder();
        void incrementTimeout();
        void resetTimeout();
        void computeGroups();
        void printSet(unordered_set<int> s); // delete later maybe
        // double get_dir_prob();


        void increaseTimeoutPeriod(){
            // Log_info("$$$$ increaseTimeoutPeriod; increasing timeout_period");
            timeout_period = std::min(timeout_period*2, 64000000);
        }

        void resetTimeoutPeriod(){
            // Log_info("$$$$ increaseTimeoutPeriod; resetting timeout_period");
            timeout_period = 1000000;
            resultVector.clear();
        }   


        void init_probing_queues(const std::vector<int>& nodeIds){
            for (int i = 0; i < nodeIds.size(); i++)
            {
                DRLatencyQueue q(7);
                latency_queues[nodeIds[i]] = q;                
            }
        }

        // Method to initialize groups based on node_ids
        void initializeGroups(const std::vector<int>& nodeIds) {
            // Assuming an equal initial division for demonstration purposes
            size_t halfSize = nodeIds.size() / 2;

            // Initialize groupFast with the first half of node_ids
            // groupFast.clear();
            // groupFast.insert(nodeIds.begin(), nodeIds.begin() + halfSize);

            // // Initialize groupSlow with the second half of node_ids
            // groupSlow.clear();
            // groupSlow.insert(nodeIds.begin() + halfSize, nodeIds.end());


            // delete below maybe?
             // Initialize groupFast with the first half of node_ids
            groupSlow.clear();
            groupSlow.insert(nodeIds.begin(), nodeIds.begin() + halfSize);

            // Initialize groupSlow with the second half of node_ids
            groupFast.clear();
            groupFast.insert(nodeIds.begin() + halfSize, nodeIds.end());
        }

        // Method to check if group memberships have changed
        bool membershipChanged(const std::unordered_set<int>& previousGroupFast,
                       const std::unordered_set<int>& previousGroupSlow) {
            // Check if membership has changed from the previous computation
            groupIsChanged = !(previousGroupFast == groupFast && previousGroupSlow == groupSlow);
            return groupIsChanged;
        }

    };
} // namespace janus