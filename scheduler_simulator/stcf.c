#include <stdio.h>

#include "process.h"
#include "stcf.h"


int stcf_find_shortest_arrived_process(Process processes[], int num_processes, int current_time)
{
    int shortest_process_index = -1;
    int min_remaining_burst_time = __INT_MAX__;

    for(int i = 0; i < num_processes; i++)
    {

    //     printf("Time %d: Checking P%s, arrived=%d, remaining=%d\n", 
    //    current_time, processes[i].pid, 
    //    processes[i].arrival_time <= current_time, 
    //    processes[i].remaining_burst_time);

        if(processes[i].remaining_burst_time < min_remaining_burst_time && processes[i].completed_flag == 0 && processes[i].arrival_time <= current_time)
        {
            shortest_process_index = i;
            min_remaining_burst_time = processes[i].remaining_burst_time;
        }
    }

    return shortest_process_index;
}

int stcf_find_nearest_arriving_process(Process processes[], int num_processes, int current_time)
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

    return nearest_index;

}


void stcf_schedule(Process processes[], int num_processes)
{
    int current_time = 0;
    int completed = 0;

    int current_running = -1;  //* Index of currently running process
    

    while (completed < num_processes)
    {
        // find shortest job among arrived processes
        int shortest_idx = stcf_find_shortest_arrived_process(processes, num_processes, current_time);
        
        
        if(shortest_idx == -1)
        {
            int nearest_idx = stcf_find_nearest_arriving_process(processes, num_processes, current_time);
            // skip current time to the very next arriving process' arrival time
            printf("CPU idle from %d to %d\n", current_time, processes[nearest_idx].arrival_time);
            current_time = processes[nearest_idx].arrival_time; 
            continue;
        }
        
        //* preempt if needed
        if(shortest_idx != current_running)
        {
            current_running = shortest_idx;
            printf("Context switch to %s\n", processes[shortest_idx].pid);
        }

        // run one tick
        printf("[Time %d] %s is running (Remaining: %d)\n", current_time, processes[current_running].pid, processes[current_running].remaining_burst_time);
        processes[shortest_idx].remaining_burst_time--;




        // mark it completed if it has completed
        if(processes[shortest_idx].remaining_burst_time == 0) {
            // Mark completed
            processes[shortest_idx].completed_flag = 1;
            completed++;
        }

        current_time++;
    }
    
}