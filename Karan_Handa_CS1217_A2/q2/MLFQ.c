#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*==========================================
Things to be done - 
- read file
- store each process in a struct, and put that struct in an array
==========================================*/

// This is a struct of processes.
typedef struct process
{
   char name[1]; // character array as opposed to a single char because issue with fscanf and chars
   int pID;
   int priority;
   int time;
   int tickets;
   int waiting;
   int turn_around;
   int weighted_waiting;
} process;

void bsortDesc(process list[100], int s)
{
   int i, j;
   process temp;
   
   for (i = 0; i < s; i++)
   {
      for (j = 0; j < (s-i); j++)
      {
         if (list[j].priority < list[j + 1].priority)
         {
               temp = list[j];
               list[j] = list[j + 1];
               list[j + 1] = temp;
         } 
      }
   }
}

int main(void) {
    char *filename = "weighted_processes.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    char name[256];
    int pID, time, priority, tickets;

    // Skips a line
    fscanf(fp, "%*[^\n]\n");

    process processes[100];

    int i = 0;

    // Storing everything in an array of structs
    while(fscanf(fp, "%s%*c %d%*c %d%*c %d%*c %d", name, &pID, &priority, &time, &tickets) != EOF) {

        processes[i].name[0] = name[0];
        processes[i].pID = pID;
        processes[i].time = time;
        processes[i].priority = priority;
        processes[i].tickets = tickets;

        i++;
    }

    int waiting = 0;
    int turn_around = 0;
    int total_weighted_waiting_time = 0;
    int total_turn_around_time = 0;

    // i iterated over all the structs in the array, so n is the number of structs
    int n = i;

    bsortDesc(processes, n);

    for (i = 0; i<n; i++) {
        processes[i].waiting = waiting;
        processes[i].weighted_waiting = processes[i].waiting * processes[i].priority;
        waiting += processes[i].time;

        turn_around += processes[i].time;
        processes[i].turn_around = turn_around;

        total_weighted_waiting_time += processes[i].weighted_waiting;
        total_turn_around_time += processes[i].turn_around;
    }

    printf("Process       Burst       Waiting       Turn around\n");
    for (int i = 0; i<n; i++) {
        printf("%-4d          %-4d        %-4d          %-4d\n", processes[i].pID, processes[i].time, processes[i].waiting, processes[i].turn_around);
    }

    float average_turn_around_time = (float) total_turn_around_time/(float) n;
    float weighted_average_waiting_time = (float) total_weighted_waiting_time/ (float) n; 

    printf("\nWeighted Average Waiting time = %f\n", weighted_average_waiting_time);
    printf("Average turn around time = %f", average_turn_around_time);

    // close the file
    fclose(fp);

    return 0;
}