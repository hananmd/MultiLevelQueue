
# Multilevel Queue Scheduling Simulator (C)

A C implementation of CPU scheduling using **4 priority-based queues**, each with a distinct algorithm. The scheduler cycles through queues, assigning a **20-unit time slice** to each.

## 📌 Scheduling Logic

| Queue | Priority | Algorithm | Type |
| :--- | :---: | :--- | :---: |
| **Q0** | High | Round Robin (Quantum=10) | Preemptive |
| **Q1** | Medium-High | Shortest Job First (SJF) | Preemptive |
| **Q2** | Medium-Low | Shortest Job First (SJF) | Non-Preemptive |
| **Q3** | Low | First-Come, First-Served (FCFS) | Non-Preemptive |

## 🚀 Key Features
- **Dynamic Input**: Accepts PID, Burst Time, Arrival Time, and Priority (0–3).
- **Real-Time Logging**: Displays preemption, completion, and idle times.
- **Metrics**: Calculates Waiting Time & Turnaround Time for each process and averages.
- **Efficient Idle Handling**: Skips time to next arrival if all queues are empty.

## ⚙️ How to Run

```bash
# Compile
gcc -o scheduler main.c

# Run
./scheduler
```

## 📝 Usage
1. Enter number of processes.
2. Input details for each process (PID, Priority 0-3, Burst Time, Arrival Time).
3. View execution log and final performance statistics.

## 🛠 Tech Stack
- **Language**: C
- **Structures**: Linked Lists (Queues), Arrays (Process Storage)
- **Concepts**: Preemption, Context Switching, Time Slicing
