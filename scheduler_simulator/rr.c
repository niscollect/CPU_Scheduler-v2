#include <stdio.h>

#include "process.h"
#include "rr.h"


// Initialize empty queue
void init_queue(ReadyQueue* q) 
{
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

// Add process to back of queue
void enqueue(ReadyQueue* q, Process* p)
{
    if (q->size >= MAX_PROCESSES) return;  // Queue full
    
    q->queue[q->rear] = p;
    q->rear = (q->rear + 1) % MAX_PROCESSES;  //* Wrap around!
    p->in_queue = 1;
    q->size++;
}
// Remove process from front of queue
Process* dequeue(ReadyQueue* q) {
    if (q->size == 0) return NULL;  // Queue empty
    
    Process* p = q->queue[q->front];
    q->front = (q->front + 1) % MAX_PROCESSES;  //* Wrap around!
    q->size--;
    p->in_queue = 0;
    return p;
}

// Check if queue is empty
int is_empty(ReadyQueue* q) {
    return q->size == 0;
}

void check_new_arrivals(Process processes[], int num_processes, int current_time, ReadyQueue* q)
{
    for(int i = 0; i < num_processes; i++)
    {
        if (processes[i].arrival_time == current_time && 
            processes[i].completed_flag == 0 && 
            processes[i].in_queue == 0) {  // Need this flag to avoid double-adding
            
            enqueue(q, &processes[i]);
            printf("[Time %d] %s arrived and added to queue\n", current_time, processes[i].pid);
        }
    }
}

int find_next_arrival_time(Process processes[], int num_processes, int current_time)
{
    int nearest_index = -1;
    int min_diff = __INT_MAX__;
    
    for(int i = 0; i < num_processes; i++)
    {
        int diff =  processes[i].arrival_time - current_time;

        if(diff < min_diff && processes[i].completed_flag == 0)
        {
            min_diff = diff;
            nearest_index = i;
        }
    }

    // not very much needed
    if(nearest_index == -1)
    {
        printf("Some error");
        return 1;
    }

    return processes[nearest_index].arrival_time;
}



void rr_schedule(Process processes[], int num_processes, int quantum)
{
    int current_time = 0;
    int completed = 0;


    ReadyQueue ready_queue;
    init_queue(&ready_queue);

    
    //@note      RR Process:
    //* Dequeue process from front of queue
    //* Run it for quantum ticks (or until finished)
    //* If not finished: Enqueue it back to rear
    //* Repeat - dequeue next process from front

    // @note
    //* processes[] array comes from main.c with all processes
    //* But we only want to put arrived processes in the ready queue
    //* Coz not all processes are ready at time 0


    while(completed < num_processes)
    {
        //* 1. Check for NEW ARRIVALS at current_time
        // for(int i = 0; i < num_processes; i++)
        // {
        //     if (processes[i].arrival_time == current_time && 
        //         processes[i].completed_flag == 0) {
        //         enqueue(&ready_queue, &processes[i]);
        //     }
        // }
        check_new_arrivals(processes, num_processes, current_time, &ready_queue);

        //* 2. If queue is empty, handle idle time
        if(is_empty(&ready_queue) == 1) // empty queue
        {
            int next_arrival = find_next_arrival_time(processes, num_processes, current_time);
            printf("[Time %d-%d] CPU idle\n", current_time, next_arrival);
            current_time = next_arrival;    
        }
        //* 3. If not, dequeue and run for qunatum
        else
        {
            Process* p = dequeue(&ready_queue);

            for(int t = 0; t < quantum && p->remaining_burst_time > 0; t++)
            {
                printf("[Time %d] %s is running (Remaining: %d)\n", current_time, p->pid, p->remaining_burst_time);
                p->remaining_burst_time--;
                current_time++;

                check_new_arrivals(processes, num_processes, current_time, &ready_queue);
            }

            if(p->remaining_burst_time <= 0)
            {
                p->completed_flag = 1;
                completed++;
            }
            else
            {
                enqueue(&ready_queue, p);
            }
            
        }
    }


}