#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// Function to purely check if a max_load is possible with given number of servers
// OPTIMIZATION: Avoids dynamic vector allocation during binary search. Strictly checks feasibility.

bool isPossible(const vector<long long>& tasks, int numServers, long long maxLoad) {
    long long currentServerLoad = 0;
    int requiredServers = 1;

    for (long long task : tasks) {
        
        // If adding this task exceeds the maxLoad, allocate to a new server
        if (currentServerLoad + task > maxLoad) {
            requiredServers++;
            currentServerLoad = task;
            
            // If required servers exceed available servers, this maxLoad is invalid
            if (requiredServers > numServers) {
                return false;
            }
        } else {
            currentServerLoad += task; // Add task to current server
        }
    }
    return true;
}

// Function to find the optimal maximum load using Binary Search
void optimizeCloudResources(const vector<long long>& tasks, int numServers) {
    if (numServers > tasks.size() || numServers <= 0) {
        cout << "Invalid input: Number of servers must be between 1 and the total number of tasks.\n";
        return;
    }

    
    long long low = *max_element(tasks.begin(), tasks.end());
    long long high = accumulate(tasks.begin(), tasks.end(), 0LL);
    long long optimalMaxLoad = high; //it starts as the total load

    // Binary Search on Answer - O(N log)
    while (low <= high) {
        long long mid = low + (high - low) / 2;

        if (isPossible(tasks, numServers, mid)) {
            optimalMaxLoad = mid; // Valid configuration, store it
            high = mid - 1;       // Try to find a even smaller "maximum load"
        } else {
            low = mid + 1;     // We need to increase the allowed load
        }
    }

    
    vector<vector<long long>> bestAllocation;
    vector<long long> currentServerTasks;
    long long currentServerLoad = 0;

    for (long long task : tasks) {
        if (currentServerLoad + task > optimalMaxLoad) {
            bestAllocation.push_back(currentServerTasks);
            currentServerTasks.clear();
            currentServerLoad = task;
            currentServerTasks.push_back(task);
        } else {
            currentServerLoad += task;
            currentServerTasks.push_back(task);
        }
    }
    if (!currentServerTasks.empty()) {
        bestAllocation.push_back(currentServerTasks);
    }

    // Output the formatted physical results
    cout << "\n============================================\n";
    cout << "   Cloud Resource Allocation Optimization   \n";
    cout << "============================================\n\n";

    cout << "Optimal Load Strategy Results:\n";
    cout << "--------------------------------\n";
    cout << ">> Optimal Maximum Load Value: " << optimalMaxLoad << "\n\n";
    cout << "Task Allocation Strategy:\n";
    
    for (int i = 0; i < bestAllocation.size(); i++) {
        cout << "Server " << i + 1 << " [Load: ";
        long long serverLoad = 0;
        for (long long task : bestAllocation[i]) {
            serverLoad += task;
        }
        cout << serverLoad << "] -> Tasks assigned: { ";
        for (int j = 0; j < bestAllocation[i].size(); j++) {
            cout << bestAllocation[i][j];
            if (j < bestAllocation[i].size() - 1) cout << ", ";
        }
        cout << " }\n";
    }
    cout << "\n============================================\n";
    cout << "Outcome: Efficient scaling and reduced latency achieved!\n";
    cout << "============================================\n";
}

int main() {
    // OPTIMIZATION: Fast I/O for significant performance boost during standard input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    
    cout << "Enter the total number of tasks: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Number of tasks must be greater than 0.\n";
        return 0;
    }

    // OPTIMIZATION: Using long long array
    vector<long long> tasks(n);
    cout << "Enter the load for each task (space-separated): ";
    for (int i = 0; i < n; i++) {
        cin >> tasks[i];
    }
    
    cout << "Enter the number of servers available: ";
    cin >> m;

    optimizeCloudResources(tasks, m);

    return 0;
}
