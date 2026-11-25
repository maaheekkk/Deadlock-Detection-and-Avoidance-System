
# ⚙️ Deadlock Detection and Avoidance System

A C++ based simulation of operating-system resource management that demonstrates **deadlock avoidance, deadlock detection, safe-state evaluation, and resource-request handling** using the Banker's Algorithm and resource-allocation techniques.

---

## 📌 Project Overview

Deadlocks are a common problem in operating systems where multiple processes compete for resources and become unable to proceed due to circular dependencies.

This project simulates resource allocation among processes and demonstrates how an operating system can evaluate resource requests, maintain safe states, and identify potential deadlocks.

The system implements the **Banker's Algorithm** for deadlock avoidance and a separate deadlock detection algorithm to analyze resource-allocation states.

---

## 🎯 Objectives

- Understand the concept of deadlocks in operating systems.
- Implement the Banker's Algorithm for deadlock avoidance.
- Calculate the Need matrix from Allocation and Maximum matrices.
- Determine whether a system is in a safe or unsafe state.
- Simulate resource requests and evaluate their safety.
- Implement rollback when a tentative allocation results in an unsafe state.
- Detect deadlocked processes using a separate detection algorithm.

---

## ✨ Features

- 📊 Allocation, Maximum, Need and Available resource structures.
- 🛡️ Banker's Algorithm for safety checking.
- 🔍 Safe sequence identification.
- 📥 Resource request simulation.
- ↩️ Rollback mechanism for unsafe tentative allocations.
- 🚨 Deadlock detection using Work and Finish.
- 🖥️ Interactive console-based interface.

---

## 🧠 Concepts Used

| Concept | Description |
|---|---|
| Deadlock | Processes waiting indefinitely for resources held by one another. |
| Allocation Matrix | Resources currently allocated to each process. |
| Maximum Matrix | Maximum resources each process may require. |
| Need Matrix | Additional resources required for completion. |
| Available Vector | Resources currently available in the system. |
| Banker's Algorithm | Determines whether an allocation keeps the system in a safe state. |
| Safe Sequence | An order in which all processes can finish. |
| Deadlock Detection | Identifies processes that cannot finish in the simulated current state. |

---

## 📐 Need Matrix Calculation

The Need matrix is calculated using:

\[
Need = Maximum - Allocation
\]

It represents the additional resources required by each process to complete execution.

---

## 🏦 Banker's Algorithm

The Banker's Algorithm is used to determine whether the system can allocate resources without entering an unsafe state.

### Algorithm

1. Initialize `Work` with the `Available` vector.
2. Find a process whose `Need` is less than or equal to `Work`.
3. Assume the process completes execution.
4. Release its allocated resources back into `Work`.
5. Repeat until all processes finish or no eligible process can be found.
6. If all processes can finish, the system is in a **safe state**.

A safe sequence is generated when the system is safe.

---

## 🔄 Resource Request Handling

When a process requests additional resources:

1. Check whether the request exceeds the process's remaining Need.
2. Check whether the requested resources are currently Available.
3. Temporarily allocate the requested resources.
4. Run the safety algorithm.
5. If the system remains safe, the request is accepted.
6. Otherwise, the allocation is rolled back.

This ensures that resource allocation does not leave the simulated system in an unsafe state.

---

## 🚨 Deadlock Detection

Deadlock detection is implemented separately from deadlock avoidance.

The detection algorithm uses:

- `Work` – currently available resources during the detection simulation.
- `Finish` – indicates whether a process can finish.
- `Allocation` – resources currently held by processes.
- `Request` – resources each process is still waiting for.

### Algorithm

1. Initialize `Work = Available`.
2. Initialize the Finish array.
3. Find an unfinished process whose Request can be satisfied by Work.
4. Simulate its completion and release its allocated resources.
5. Mark the process as finished.
6. Repeat until no further process can finish.
7. Processes remaining unfinished are reported as deadlocked in the simulated state.

---

## 📊 Sample Input

The sample Banker's Algorithm state uses five processes and three resource types.

### Allocation Matrix

| Process | R0 | R1 | R2 |
|---|---:|---:|---:|
| P0 | 0 | 1 | 0 |
| P1 | 2 | 0 | 0 |
| P2 | 3 | 0 | 2 |
| P3 | 2 | 1 | 1 |
| P4 | 0 | 0 | 2 |

### Maximum Matrix

| Process | R0 | R1 | R2 |
|---|---:|---:|---:|
| P0 | 7 | 5 | 3 |
| P1 | 3 | 2 | 2 |
| P2 | 9 | 0 | 2 |
| P3 | 2 | 2 | 2 |
| P4 | 4 | 3 | 3 |

### Available Resources

```text
[3, 3, 2]
```

### Expected Safe Sequence

```text
P1 → P3 → P4 → P0 → P2
```

---

## 🧪 Testing Scenarios

| Test | Input / Scenario | Expected Result |
|---|---|---|
| Safety Check | Sample resource allocation | Safe state and safe sequence |
| Valid Request | P1 requests `[1, 0, 2]` | Request granted if the resulting state remains safe |
| Invalid Request | P0 requests `[8, 0, 0]` | Rejected because it exceeds P0's Need |
| No Deadlock | Detection scenario where processes can finish | No deadlock detected |
| Deadlock | Detection scenario where processes cannot finish | Remaining processes identified |

---

## 🛠️ Technologies Used

- **Language:** C++
- **Standard:** C++11 or later
- **Libraries:** STL, `vector`, `iostream`, `iomanip`, `string`, `limits`
- **Platform:** Windows / Linux / macOS

---

## ▶️ How to Run

### 1. Clone the repository

```bash
git clone https://github.com/your-username/Deadlock-Detection-and-Avoidance-System.git
```

### 2. Navigate into the project folder

```bash
cd Deadlock-Detection-and-Avoidance-System
```

### 3. Compile the program

```bash
g++ -std=c++11 -Wall -Wextra main.cpp -o deadlock_simulator
```

### 4. Run the program

**Linux / macOS**

```bash
./deadlock_simulator
```

**Windows**

```bash
deadlock_simulator.exe
```


---

## ⚠️ Limitations

- This project is an educational simulation and does not manage actual operating-system processes.
- Resource allocation is simulated using predefined or user-provided data.
- The program does not implement actual process execution or operating-system resource scheduling.
- Deadlock avoidance and detection are demonstrated algorithmically.

---

## 🚀 Future Scope

- Dynamic input for process and resource configurations.
- Graphical user interface for visualization.
- Real-time animation of resource allocation.
- Resource-allocation graph visualization.
- Enhanced validation and interactive simulations.

---


## 📚 Academic Project

Developed as part of the Operating Systems coursework.

**Course:** CSE-234  
**Project:** Deadlock Detection and Avoidance System

---

> An unsafe state does not necessarily mean the system is deadlocked. Deadlock avoidance attempts to prevent unsafe allocations, whereas deadlock detection analyzes a current simulated state to identify processes that cannot finish.
