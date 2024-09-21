#include "dynamic_routing_manager.h"
#include <random>
#include <cmath>

namespace janus
{
    void DRLatencyQueue::add(uint64_t id, std::chrono::system_clock::time_point start_time) {
        if (q.size() >= maxSize) {
            removeOldest();
        }
        q.push(LatencyRequestData(id, start_time));
        addToHeaps({HIGH_INT_VALUE, id}); // Initially adding a high placeholder value
    }

    void DRLatencyQueue::updateEndTime(uint64_t id, std::chrono::system_clock::time_point end_time) {
        LatencyRequestData& request = q.back();
        if (request.id == id) {
            int old_latency = (request.end_time != std::chrono::system_clock::time_point::max())
                                ? chrono::duration_cast<chrono::microseconds>(request.end_time - request.start_time).count()
                                : HIGH_INT_VALUE;

            if (old_latency != HIGH_INT_VALUE) {
                removeLatencyFromHeaps({old_latency, id});
            }

            request.end_time = end_time;
            int latency = chrono::duration_cast<chrono::microseconds>(request.end_time - request.start_time).count();

            if (latency != HIGH_INT_VALUE) {
                addToHeaps({latency, id});
            }
        }
        else{
            verify(0);
        }
    }

    double DRLatencyQueue::findMedian() {
        if (q.size() == 0) {
            std::cerr << "Queue is empty!" << std::endl;
            return 0;
        }
        // display();
        if (q.size() % 2 == 0) {
            if (maxHeap.size() == minHeap.size()) {
                return (maxHeap.top().first + minHeap.top().first) / 2.0;
            } else {
                return maxHeap.top().first; // maxHeap always has one more element than minHeap
            }
        } else {
            return maxHeap.top().first;
        }
    }

    void DRLatencyQueue::removeOldest() {
        LatencyRequestData oldest = q.front();
        q.pop();

        int latency = (oldest.end_time != std::chrono::system_clock::time_point::max())
                          ? chrono::duration_cast<chrono::microseconds>(oldest.end_time - oldest.start_time).count()
                          : HIGH_INT_VALUE;
        if (latency <= maxHeap.top().first) {
            removeFromHeap(maxHeap, {latency, oldest.id});
        } else {
            removeFromHeap(minHeap, {latency, oldest.id});
        }
    }

    void DRLatencyQueue::addToHeaps(std::pair<int, uint64_t> value) {
        if (value.first != HIGH_INT_VALUE) {
            if (maxHeap.empty() || value.first <= maxHeap.top().first) {
                maxHeap.push(value);
            } else {
                minHeap.push(value);
            }
            rebalanceHeaps();
        }
    }

    void DRLatencyQueue::removeLatencyFromHeaps(std::pair<int, uint64_t> latencyId) {
        if (latencyId.first != HIGH_INT_VALUE) {
            if (!maxHeap.empty() && latencyId.first <= maxHeap.top().first) {
                removeFromHeap(maxHeap, latencyId);
            } else if (!minHeap.empty() && latencyId.first >= minHeap.top().first) {
                removeFromHeap(minHeap, latencyId);
            }
        }
    }

    void DRLatencyQueue::removeFromHeap(std::priority_queue<std::pair<int, uint64_t>>& heap, std::pair<int, uint64_t> value) {
        std::priority_queue<std::pair<int, uint64_t>> tempHeap = heap;
        std::priority_queue<std::pair<int, uint64_t>> newHeap;

        while (!tempHeap.empty()) {
            if (tempHeap.top() != value) {
                newHeap.push(tempHeap.top());
            }
            tempHeap.pop();
        }

        heap = newHeap;

        rebalanceHeaps();
    }

    void DRLatencyQueue::removeFromHeap(std::priority_queue<std::pair<int, uint64_t>, std::vector<std::pair<int, uint64_t>>, std::greater<std::pair<int, uint64_t>>>& heap, std::pair<int, uint64_t> value) {
        std::priority_queue<std::pair<int, uint64_t>, std::vector<std::pair<int, uint64_t>>, std::greater<std::pair<int, uint64_t>>> tempHeap = heap;
        std::priority_queue<std::pair<int, uint64_t>, std::vector<std::pair<int, uint64_t>>, std::greater<std::pair<int, uint64_t>>> newHeap;

        while (!tempHeap.empty()) {
            if (tempHeap.top() != value) {
                newHeap.push(tempHeap.top());
            }
            tempHeap.pop();
        }

        heap = newHeap;

        rebalanceHeaps();
    }

    void DRLatencyQueue::rebalanceHeaps() {
        if (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        } else if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    void DRLatencyQueue::display() {
        std::queue<LatencyRequestData> tempQueue = q; // Create a temporary copy of the queue

        while (!tempQueue.empty()) {
            const LatencyRequestData& request = tempQueue.front();
            std::cout << "Request ID: " << request.id << " latency: " << chrono::duration_cast<chrono::microseconds>(request.end_time - request.start_time).count() << ", Start time: "
                    << std::chrono::duration_cast<std::chrono::microseconds>(request.start_time.time_since_epoch()).count()
                    << ", End time: ";
            if (request.end_time != std::chrono::system_clock::time_point::max()) {
                std::cout << std::chrono::duration_cast<std::chrono::microseconds>(request.end_time.time_since_epoch()).count();
            } else {
                std::cout << "Not available";
            }
            std::cout << std::endl;

            tempQueue.pop();
        }
    }

    LatencyRequestData DRLatencyQueue::getLastRequest(){
        if (q.size() > 0){
            auto value = q.back();
            return value;
        }
        else { 
            return LatencyRequestData(-1, std::chrono::system_clock::time_point::max(), std::chrono::system_clock::time_point::max());
        }
    }

    void DynamicRoutingManager::add_request_start_time(uint64_t req_id, uint64_t node_id)
    {        
        // case: when the previous probing request has received it's reply
        auto request = latency_queues[node_id].getLastRequest();
        if(request.id == -1 || request.end_time != std::chrono::system_clock::time_point::max()){
            latency_queues[node_id].add(req_id, chrono::system_clock::now());
        }
        else{
            return;
        }
    }


    void DynamicRoutingManager::add_request_end_time(uint64_t req_id, uint64_t node_id)
    {
        if (latency_queues[node_id].getLastRequest().id == req_id)
        {
            latency_queues[node_id].updateEndTime(req_id, chrono::system_clock::now());
            return;
        }
    }

    void DynamicRoutingManager::computeGroups(){
        std::vector<std::pair<int, int>> allLatenciesWithId;
        std::unordered_set<int> tempGroupFast, tempGroupSlow;

        // Step 1: Retrieve median latency values and associated queue_id from each heap queue
        for (auto& latency_queue_pair : latency_queues) {
            auto last_request = latency_queue_pair.second.getLastRequest();
            if(last_request.end_time == std::chrono::system_clock::time_point::max()){
                // Log_info("$$$$ inside DynamicRoutingManager::computeGroups(); no response yet for request_id:%d", last_request.id);
                latency_queue_pair.second.updateEndTime(last_request.id, chrono::system_clock::now());
            }
            int medianLatency = latency_queue_pair.second.findMedian();
            int nodeId = latency_queue_pair.first;
            allLatenciesWithId.emplace_back(medianLatency, nodeId);
            // Log_info("!$$$$ inside DynamicRoutingManager::computeGroups(); added medianLatency: %d, for nodeid: %d", medianLatency, nodeId);
            // Log_info("!$$$$ inside DynamicRoutingManagers::computeGroups(); current latency contents for nodeid: %d", nodeId);
            // latency_queue_pair.second.display();
        }

        // Log_info("$$$$ inside DynamicRoutingManager::computeGroups(); all latencies: ");
        // kshivam: delete later
        // for (auto latId: allLatenciesWithId){
        //     // Log_info("$$$$ inside DynamicRoutingManager::computeGroups(); node_id: %d, latency: %d", latId.second, latId.first);
        // }

        // Step 2: Sort all latencies based on values
        std::sort(allLatenciesWithId.begin(), allLatenciesWithId.end());

        // Step 3: Divide into two groups
        size_t halfSize = allLatenciesWithId.size() / 2;

        // Step 4: Save results in two unordered sets based on queue_id

        for (size_t i = 0; i < halfSize; ++i) {
            tempGroupFast.insert(allLatenciesWithId[i].second);
            // Log_info("tempGroupFast: %d with id: %d", allLatenciesWithId[i].first, allLatenciesWithId[i].second);
        }

        for (size_t i = halfSize; i < allLatenciesWithId.size(); ++i) {
            tempGroupSlow.insert(allLatenciesWithId[i].second);
            // Log_info("tempGroupSlow: %d with id: %d", allLatenciesWithId[i].first, allLatenciesWithId[i].second);
        }

        auto isChanged = membershipChanged(tempGroupFast, tempGroupSlow);

        if (isChanged){
            // resultVector.clear();
            Log_info("inside DynamicRoutingManager::computeGroups(); group membership has changed");
            Log_info("old groupFast is: ");
            printSet(groupFast);
            Log_info("new groupFast is: "); 
            printSet(tempGroupFast);
            groups_lock_.lock();
            groupFast = tempGroupFast;
            groupSlow = tempGroupSlow;
            groups_lock_.unlock();
            resetTimeoutPeriod();
        }
        else {
            increaseTimeoutPeriod();
            // Log_info("$$$$ inside DynamicRoutingManager::computeGroups(); membership has NOT changed");
        }
    }

    void DynamicRoutingManager::printSet(unordered_set<int> s){
        for (const auto& element : s) {
            Log_info("!$$$$ element: %d", element);
            // std::cout << element << " ";
        }
    }

    vector<int> DynamicRoutingManager::getNodesOrder(){
        if (resultVector.size() > 0){
            return resultVector;
        }
        // Log_info("^^^^^ taking other route");
        groups_lock_.lock();
        resultVector.insert(resultVector.end(), groupFast.begin(), groupFast.end());
        resultVector.insert(resultVector.end(), groupSlow.begin(), groupSlow.end());
        groups_lock_.unlock();
        return resultVector;
    }

    void DynamicRoutingManager::resetTimeout(){
        timeout_period = 1000000;
        // no_change = false;
        // Log_info("^^^^ reset timeout called");
        // resultVector.clear();
    }

    void DynamicRoutingManager::incrementTimeout(){
        timeout_period = std::min(timeout_period*2, 64000000);
    }

} // namespace janus