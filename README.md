
# CPU Scheduler Simulator v2
#OS

A comprehensive command-line CPU scheduling simulator implementing multiple scheduling algorithms with preemption support and advanced queue management. Built while studying Operating Systems concepts from OSTEP (Operating Systems: Three Easy Pieces).

## 🚀 What's New in v2

This is an enhanced version of my [CPU Scheduler Simulator v1]([link-to-your-v1-repo](https://github.com/niscollect/CPU_Scheduler.git)) with significant improvements:

- ✅ **Preemptive Scheduling**: STCF (Shortest Time-to-Completion First)
- ✅ **Round Robin**: Time-quantum based scheduling with circular queue
- ✅ **Advanced Data Structures**: Proper queue management and process state tracking
- ✅ **Dynamic Preemption**: Real-time scheduling decisions with every clock tick
- ✅ **Enhanced CLI**: Support for quantum parameters and better argument parsing

## 📋 Supported Algorithms

| Algorithm | Type | Description |
|-----------|------|-------------|
| **FIFO** | Non-preemptive | First In, First Out (First Come First Served) |
| **SJF** | Non-preemptive | Shortest Job First |
| **STCF** | Preemptive | Shortest Time-to-Completion First (Preemptive SJF) |
| **RR** | Preemptive | Round Robin with configurable time quantum |

## 🛠️ Usage

### Compilation
```bash
gcc -o scheduler main.c fifo.c sjf.c stcf.c rr.c
```

### Running the Simulator
```bash
# FIFO Scheduling
./scheduler --algorithm FIFO

# Shortest Job First
./scheduler --algorithm SJF

# Shortest Time-to-Completion First (Preemptive)
./scheduler --algorithm STCF

# Round Robin with quantum = 2
./scheduler --algorithm RR --quantum 2
```

## 📊 Sample Output

### FIFO Example
```
[Time 2] P1 is running (Remaining: 5)
[Time 3] P1 is running (Remaining: 4)
[Time 4] P1 is running (Remaining: 3)
[Time 5] P1 is running (Remaining: 2)
[Time 6] P1 is running (Remaining: 1)
[Time 7] P1 finished execution.
[Time 7] P2 is running (Remaining: 3)
...
```

### STCF Example (Preemptive)
```
Context switch to P1
[Time 2] P1 is running (Remaining: 5)
Context switch to P2
[Time 3] P2 is running (Remaining: 3)
[Time 4] P2 is running (Remaining: 2)
[Time 5] P2 is running (Remaining: 1)
Context switch to P3
[Time 6] P3 is running (Remaining: 1)
Context switch to P1
[Time 7] P1 is running (Remaining: 4)
...
```

### Round Robin Example
```
[Time 2] P1 arrived and added to queue
[Time 2] P1 is running (Remaining: 5)
[Time 3] P2 arrived and added to queue
[Time 3] P1 is running (Remaining: 4)
[Time 4] P3 arrived and added to queue
[Time 4] P2 is running (Remaining: 3)
[Time 5] P2 is running (Remaining: 2)
[Time 6] P3 is running (Remaining: 1)
[Time 7] P1 is running (Remaining: 3)
...
```

## 🏗️ Architecture

### Core Components

- **Process Structure**: Tracks PID, arrival time, burst time, completion status, and queue state
- **Scheduler Interface**: Modular design allowing easy addition of new algorithms
- **Circular Queue**: Efficient round-robin implementation with proper wrap-around
- **Tick-by-Tick Simulation**: Precise time-based execution with preemption support

### File Structure
```
├── main.c           # Command-line parsing and algorithm selection
├── process.h        # Process structure and definitions
├── fifo.c/fifo.h   # First In, First Out implementation
├── sjf.c/sjf.h     # Shortest Job First implementation  
├── stcf.c/stcf.h   # Shortest Time-to-Completion First implementation
├── rr.c/rr.h       # Round Robin implementation
└── README.md
```

## 🔧 Technical Implementation

### Key Features

**Preemptive Scheduling Logic**
```c
// STCF checks for preemption every tick
int shortest_idx = find_shortest_arrived_process(processes, num_processes, current_time);
if(shortest_idx != current_running) {
    current_running = shortest_idx;
    printf("Context switch to %s\n", processes[shortest_idx].pid);
}
```

**Circular Queue Management**
```c
// Efficient wrap-around using modulo arithmetic
q->rear = (q->rear + 1) % MAX_PROCESSES;
q->front = (q->front + 1) % MAX_PROCESSES;
```

**Dynamic Process State Tracking**
- `completed_flag`: Tracks process completion
- `in_queue`: Prevents double-adding to ready queue
- Real-time arrival handling during execution

## 📚 Learning Outcomes

Building this simulator provided hands-on experience with:

- **Process Scheduling Algorithms**: Understanding trade-offs between different approaches
- **Preemption Mechanics**: Implementing context switching and priority-based interruption
- **Data Structure Design**: Circular queues, process state management
- **Systems Programming**: C programming, modular architecture, command-line interfaces
- **Algorithm Analysis**: Comparing performance characteristics of different schedulers

## 🎯 Future Enhancements

- [ ] Multi-Level Feedback Queue (MLFQ)
- [ ] I/O burst handling and blocked process states
- [ ] Process workload file input
- [ ] Performance metrics calculation (turnaround time, response time, wait time)
- [ ] Gantt chart visualization

## 🤝 Contributing

Feel free to fork this repository and submit pull requests for improvements or additional scheduling algorithms!

## 📖 References

- [OSTEP - Operating Systems: Three Easy Pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/)
- [Process Scheduling - Wikipedia](https://en.wikipedia.org/wiki/Scheduling_(computing))

---

Built with ❤️ while learning Operating Systems fundamentals
