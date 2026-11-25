/*
    Project title: Deadlock Detection and Avoidance System
    Operating Systems (CSE234)
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

const int PROCESS_COUNT = 5;
const int RESOURCE_COUNT = 3;

vector<vector<int>> allocation = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

vector<vector<int>> maximum = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

vector<vector<int>> need(PROCESS_COUNT, vector<int>(RESOURCE_COUNT, 0));
vector<int> available = {3, 3, 2};

// Reads a non-negative integer. Returns false if input is invalid.
bool readNonNegative(const string& prompt, int& value) {
    cout << prompt;
    if (!(cin >> value) || value < 0) {
        cout << "Invalid input. Enter a non-negative integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// Need = Maximum - Allocation
void calculateNeed() {
    for (int i = 0; i < PROCESS_COUNT; i++) {
        for (int j = 0; j < RESOURCE_COUNT; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

void displayMatrix(const string& title, const vector<vector<int>>& matrix) {
    cout << "\n" << title << "\n";
    cout << left << setw(12) << "Process";
    for (int j = 0; j < RESOURCE_COUNT; j++) {
        cout << setw(8) << ("R" + to_string(j));
    }
    cout << "\n";

    for (int i = 0; i < PROCESS_COUNT; i++) {
        cout << left << setw(12) << ("P" + to_string(i));
        for (int j = 0; j < RESOURCE_COUNT; j++) {
            cout << setw(8) << matrix[i][j];
        }
        cout << "\n";
    }
}

void displayResources() {
    cout << "\n========== SYSTEM RESOURCE ALLOCATION ==========\n";
    displayMatrix("Allocation Matrix", allocation);
    displayMatrix("Maximum Matrix", maximum);
    displayMatrix("Need Matrix (Maximum - Allocation)", need);

    cout << "\nAvailable Resources: ";
    for (int value : available) cout << value << " ";
    cout << "\n";
}

bool safetyAlgorithm(bool showSteps, vector<int>& safeSequence) {
    vector<int> work = available;
    vector<bool> finished(PROCESS_COUNT, false);
    safeSequence.clear();

    bool progress = true;
    while (progress) {
        progress = false;

        for (int i = 0; i < PROCESS_COUNT; i++) {
            if (finished[i]) continue;

            bool canFinish = true;
            for (int j = 0; j < RESOURCE_COUNT; j++) {
                if (need[i][j] > work[j]) {
                    canFinish = false;
                    break;
                }
            }

            if (canFinish) {
                if (showSteps) {
                    cout << "P" << i << " can finish and release its allocated resources.\n";
                }

                for (int j = 0; j < RESOURCE_COUNT; j++) {
                    work[j] += allocation[i][j];
                }

                finished[i] = true;
                safeSequence.push_back(i);
                progress = true;
            }
        }
    }

    return static_cast<int>(safeSequence.size()) == PROCESS_COUNT;
}

void checkSystemSafety() {
    cout << "\n========== BANKER'S SAFETY ALGORITHM ==========\n";
    vector<int> safeSequence;

    if (safetyAlgorithm(true, safeSequence)) {
        cout << "\nSYSTEM STATUS: SAFE\nSafe sequence: ";
        for (int i = 0; i < static_cast<int>(safeSequence.size()); i++) {
            cout << "P" << safeSequence[i];
            if (i + 1 < static_cast<int>(safeSequence.size())) cout << " -> ";
        }
        cout << "\n";
    } else {
        cout << "\nSYSTEM STATUS: UNSAFE\n";
        cout << "No safe sequence exists for the current state.\n";
    }
}

void requestResources() {
    cout << "\n========== RESOURCE REQUEST ==========\n";

    int process;
    if (!readNonNegative("Enter process number (0-4): ", process)) return;

    if (process >= PROCESS_COUNT) {
        cout << "Invalid process number.\n";
        return;
    }

    vector<int> request(RESOURCE_COUNT);
    cout << "Enter requested units for each resource:\n";

    for (int j = 0; j < RESOURCE_COUNT; j++) {
        if (!readNonNegative("R" + to_string(j) + ": ", request[j])) return;
    }

    // A request cannot exceed the process's declared remaining need.
    for (int j = 0; j < RESOURCE_COUNT; j++) {
        if (request[j] > need[process][j]) {
            cout << "REQUEST REJECTED: request exceeds the process's remaining Need.\n";
            return;
        }
    }

    // A request cannot exceed the currently available resources.
    for (int j = 0; j < RESOURCE_COUNT; j++) {
        if (request[j] > available[j]) {
            cout << "REQUEST NOT GRANTED: insufficient available resources.\n";
            return;
        }
    }

    // Tentatively allocate the requested resources.
    for (int j = 0; j < RESOURCE_COUNT; j++) {
        available[j] -= request[j];
        allocation[process][j] += request[j];
        need[process][j] -= request[j];
    }

    vector<int> safeSequence;

    if (safetyAlgorithm(false, safeSequence)) {
        cout << "REQUEST GRANTED: system remains in a SAFE state.\n";
        cout << "Safe sequence: ";
        for (int i = 0; i < static_cast<int>(safeSequence.size()); i++) {
            cout << "P" << safeSequence[i];
            if (i + 1 < static_cast<int>(safeSequence.size())) cout << " -> ";
        }
        cout << "\n";
    } else {
        // Roll back if the tentative allocation makes the state unsafe.
        for (int j = 0; j < RESOURCE_COUNT; j++) {
            available[j] += request[j];
            allocation[process][j] -= request[j];
            need[process][j] += request[j];
        }
        cout << "REQUEST DENIED: it would make the system UNSAFE.\n";
        cout << "The tentative allocation has been rolled back.\n";
    }
}

void detectDeadlock() {
    cout << "\n========== DEADLOCK DETECTION ==========\n";
    cout << "Enter a current resource state for this detection test.\n";

    int processCount, resourceCount;
    if (!readNonNegative("Number of processes: ", processCount) || processCount == 0) {
        cout << "Process count must be positive.\n";
        return;
    }
    if (!readNonNegative("Number of resource types: ", resourceCount) || resourceCount == 0) {
        cout << "Resource count must be positive.\n";
        return;
    }
    if (processCount > 20 || resourceCount > 10) {
        cout << "Use at most 20 processes and 10 resource types for this demonstration.\n";
        return;
    }

    vector<int> work(resourceCount);
    vector<vector<int>> currentAllocation(processCount, vector<int>(resourceCount));
    vector<vector<int>> currentRequest(processCount, vector<int>(resourceCount));

    cout << "\nEnter Available vector:\n";
    for (int j = 0; j < resourceCount; j++) {
        if (!readNonNegative("R" + to_string(j) + ": ", work[j])) return;
    }

    cout << "\nEnter current Allocation matrix, one row per process.\n";
    for (int i = 0; i < processCount; i++) {
        cout << "P" << i << ": ";
        for (int j = 0; j < resourceCount; j++) {
            if (!(cin >> currentAllocation[i][j]) || currentAllocation[i][j] < 0) {
                cout << "Invalid allocation value.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
        }
    }

    cout << "\nEnter current outstanding Request matrix, one row per process.\n";
    for (int i = 0; i < processCount; i++) {
        cout << "P" << i << ": ";
        for (int j = 0; j < resourceCount; j++) {
            if (!(cin >> currentRequest[i][j]) || currentRequest[i][j] < 0) {
                cout << "Invalid request value.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
        }
    }

    // In the detection algorithm, a process holding no resources is initially finished.
    vector<bool> finished(processCount, false);
    for (int i = 0; i < processCount; i++) {
        bool holdingResource = false;
        for (int j = 0; j < resourceCount; j++) {
            if (currentAllocation[i][j] > 0) holdingResource = true;
        }
        finished[i] = !holdingResource;
    }

    bool progress = true;
    while (progress) {
        progress = false;

        for (int i = 0; i < processCount; i++) {
            if (finished[i]) continue;

            bool canFinish = true;
            for (int j = 0; j < resourceCount; j++) {
                if (currentRequest[i][j] > work[j]) {
                    canFinish = false;
                    break;
                }
            }

            if (canFinish) {
                for (int j = 0; j < resourceCount; j++) {
                    work[j] += currentAllocation[i][j];
                }
                finished[i] = true;
                progress = true;
            }
        }
    }

    vector<int> deadlockedProcesses;
    for (int i = 0; i < processCount; i++) {
        if (!finished[i]) deadlockedProcesses.push_back(i);
    }

    if (deadlockedProcesses.empty()) {
        cout << "\nRESULT: No deadlock detected. All processes can finish.\n";
    } else {
        cout << "\nRESULT: DEADLOCK DETECTED.\nProcesses unable to finish: ";
        for (int i = 0; i < static_cast<int>(deadlockedProcesses.size()); i++) {
            cout << "P" << deadlockedProcesses[i];
            if (i + 1 < static_cast<int>(deadlockedProcesses.size())) cout << ", ";
        }
        cout << "\n";
    }
}

void showMenu() {
    cout << "\n====================================================\n";
    cout << "       DEADLOCK DETECTION & AVOIDANCE SYSTEM\n";
    cout << "====================================================\n";
    cout << "1. Display Resource Allocation\n";
    cout << "2. Check System Safety (Banker's Algorithm)\n";
    cout << "3. Request Resources\n";
    cout << "4. Detect Deadlock\n";
    cout << "5. Exit\n";
    cout << "====================================================\n";
}

int main() {
    calculateNeed();

    while (true) {
        showMenu();

        int choice;
        if (!readNonNegative("Enter your choice: ", choice)) continue;

        switch (choice) {
            case 1:
                displayResources();
                break;
            case 2:
                checkSystemSafety();
                break;
            case 3:
                requestResources();
                break;
            case 4:
                detectDeadlock();
                break;
            case 5:
                cout << "Exiting Deadlock Detection & Avoidance System.\n";
                return 0;
            default:
                cout << "Invalid choice. Select 1 to 5.\n";
        }
    }
}