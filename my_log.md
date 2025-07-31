v1 of the scheduler was very limited. 
In this version, we are introducing STCF and RR implementation.

<hr>

# STCF
### Implementing STCF

Goal:
A scheduler that starts ticking, and running processes, until a new process gets runnable. No sooner does a new process gets runnable than the scheduler stops the running process and checks through all the processes to find which has the least remaining_burst_time, and schedules it.

Btw, since it's STCF which is PSJF, i.e., Preemptive Shortest Job First.
So, the skeleton is very much like SJF.

When we start with the skeleton of the SJF and try transitioning it to STCF, we arrive at a problem.
`How do I know when a new process has arrived or gotten runnable?` 
Do I check in every tick? Or should I sort beforehand?
~ If I check at every tick, it's already horrible
~ If I sort beforehand, it's fundamentally flawed. **STCF is a _preemptive_ algorithm**. And preemptive algorithms deal with **dynamically changing system state** — particularly, **new arrivals over time**.

So the typical solution is to check at every tick. Yes.

STCF is SJF with check point at each tick.

<hr>
So the basic 5 step implementation:

1) Find the shortest job among arrived processes
```c
int shortest_idx = stcf_find_shortest_arrived_process(processes, num_processes, current_time);
```

2) Preempt if needed
```c
if(/* condition of preemption */)
{
	current_running = shortest_idx;
	printf("Context switch to %s\n", processes[shortest_idx].pid);
}
```

3) Run one tick
```c
printf("[Time %d] %s is running (Remaining: %d)\n", current_time, processes[current_running].pid, processes[current_running].remaining_burst_time);
processes[shortest_idx].remaining_burst_time--;
```

4) If the process completes on this tick, mark it as complete

5) increment time


Upon completing it, and then taking a crooked example, you'd realize a piece is missing, and that's the idle CPU case. If there's not index found, that means that's no fulfilling process to run, and so the CPU must sit idle for the time till next fulfilling process is caught.
And we need to check this right after trying to find the index.

<hr>

<hr> 

# RR 

### Implementing RR 

Goal: A scheduling algorithm that runs each job for a fixed time slice (quantum), then switches to the next job in a circular queue. This ensures fair CPU sharing among all processes.

**Key Challenge: Queue Management** Unlike FIFO/SJF/STCF where we just scan arrays, RR needs a proper queue data structure. Why? Because processes need to:

- Enter at the back when they arrive
- Get picked from the front for execution
- Return to the back if their quantum expires before completion

Circular queue is not the only solution, but it's the best and standard.

**Circular Queue Implementation** I implemented a circular queue using:

```c
typedef struct {
    Process* queue[MAX_PROCESSES];
    int front;   // where to dequeue from
    int rear;    // where to enqueue to  
    int size;    // current queue size
} ReadyQueue;
```

The magic is in the wrap-around: `(index + 1) % MAX_PROCESSES` makes it circular. When we reach the end of the array, it wraps back to index 0.

**Core RR Logic Flow:**

1. **Check for new arrivals** - any process arriving at current_time gets enqueued
2. **Handle idle CPU** - if queue empty, jump to next arrival time
3. **Dequeue front process** - it is who runs next
4. **Execute for quantum** - run process tick by tick, checking for new arrivals each tick
5. **Handle completion or re-enqueue** - finished processes are marked done, others go to back of queue

**Tricky Part: Arrivals During Quantum** Initially I missed this - new processes can arrive while another process is running its quantum. So I had to check for arrivals inside the quantum execution loop:

```c
for(int t = 0; t < quantum && p->remaining_burst_time > 0; t++) {
    // Run process for 1 tick
    current_time++;
    check_new_arrivals(processes, num_processes, current_time, &ready_queue);
}
```

**Process State Tracking** Added `in_queue` flag to Process struct to prevent double-adding processes to the queue. Set to 1 when enqueued, 0 when dequeued.

**Result:** Clean round-robin scheduling with proper time-sharing and queue management!
