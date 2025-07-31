#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fifo.h"
#include "sjf.h"
#include "stcf.h"
#include "rr.h"  // future addition

#define NUM_PROCESSES 3

int main(int argc, char* argv[])
{
    
    //* default algorithm
    char *algorithm = "FIFO"; 
    //* deafault quantum = 1
    int quantum = 1;     
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--algorithm") == 0) {
            algorithm = argv[++i];
        }
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--quantum") == 0) {
            quantum = atoi(argv[++i]);
        }
    }
    

    
    Process processes[NUM_PROCESSES] = { {"P1", 2, 5, 0, 0}, {"P2", 3, 3, 0, 0}, {"P3", 4, 1, 0, 0} };


    if (strcmp(algorithm, "FIFO") == 0)
        fifo_schedule(processes, NUM_PROCESSES);
    else if (strcmp(algorithm, "SJF") == 0)
        sjf_schedule(processes, NUM_PROCESSES);
    else if (strcmp(algorithm, "STCF") == 0)
        stcf_schedule(processes, NUM_PROCESSES);
    else if (strcmp(algorithm, "RR") == 0)
        rr_schedule(processes, NUM_PROCESSES, quantum);
    else
        printf("Unknown scheduling algorithm.\n");

    return 0;
}
