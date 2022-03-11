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
   int burst;
   int waiting;
   int turn_around;
} process;

// Simple sorting algorithm for the processes
void selection_sort(process list[100], int count)
{
   int i, j, mi;
   process m;
   for (i = 0; i < count - 1; i++)
   {
      /* find the minimum */
      mi = i;
      for (j = i + 1; j < count; j++)
         if (list[j].burst < list[mi].burst)
               mi = j;

      m = list[mi];

      /* move elements to the right */
      for (j = mi; j > i; j--)
         list[j] = list[j-1];

      list[i] = m;
   }
}

int main(void) {
   char *filename = "processes.txt";
   FILE *fp = fopen(filename, "r");

   if (fp == NULL) {
      printf("Error: could not open file %s", filename);
      return 1;
   }

   char name[256];
   int pID, burst;

   // Skips a line
   fscanf(fp, "%*[^\n]\n");

   process processes[100];

   int i = 0;

   // Storing everything in an array of structs
   while(fscanf(fp, "%s%*c %d%*c %d", name, &pID, &burst) != EOF) {

      processes[i].name[0] = name[0];
      processes[i].pID = pID;
      processes[i].burst = burst;

      i++;
   }

   int waiting = 0;
   int turn_around = 0;
   int total_waiting_time = 0;
   int total_turn_around_time = 0;

   selection_sort(processes, i);

   // i iterated over all the structs in the array, so n is the number of structs
   int n = i;

   for (i = 0; i<n; i++) {
      processes[i].waiting = waiting;
      waiting += processes[i].burst;

      turn_around += processes[i].burst;
      processes[i].turn_around = turn_around;

      total_waiting_time += processes[i].waiting;
      total_turn_around_time += processes[i].turn_around;
   }

   printf("Process       Burst       Waiting       Turn around\n");
   for (int i = 0; i<n; i++) {
      printf("%-4d          %-4d        %-4d          %-4d\n", processes[i].pID, processes[i].burst, processes[i].waiting, processes[i].turn_around);
   }

   float average_turn_around_time = (float) total_turn_around_time/(float) n;
   float average_waiting_time = (float) total_waiting_time/ (float) n; 

   printf("\nAverage Waiting time = %f\n", average_waiting_time);
   printf("Average turn around time = %f", average_turn_around_time);

   // close the file
   fclose(fp);

   return 0;
}

