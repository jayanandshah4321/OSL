#include<stdio.h>
#include<limits.h>

typedef struct{
    int pid;
    int AT;
    int BT;
    int RT;
    int CT;
    int WT;
    int TT;

} Process;

void SJFprem(Process arr[],int n){
    int count=0,time=0;
    while(count!=n){
        int min_rt=1000;
        int index=-1;
        for(int i=0;i<n;i++){
            if(arr[i].AT<=time && arr[i].RT>0 && arr[i].RT<min_rt){
                min_rt=arr[i].RT;
                index=i;
            }
        }
        time++;
        if(index==-1){
            //koi bhi process ready nahi hai
            continue;
        }

        arr[index].RT--;

        //check if the process is completed or not
        if(arr[index].RT==0){
            arr[index].CT=time;
            arr[index].TT=time-arr[index].AT;//turn aroud time
            arr[index].WT= arr[index].TT-arr[index].BT;//waiting time
            count++;
        }
    }
}
void roundRobin(Process arr[],int n,int quantum){
    int time = 0, count= 0;
    while (count != n) {
        int allIdle = 1; // Flag to check if all processes are idle

        // Loop through all processes in a round-robin manner
        for (int i = 0; i < n; i++) {
            // Process only if it has remaining time and has arrived
            if (arr[i].RT > 0 && arr[i].AT <= time) {
                allIdle = 0; // At least one process is available to run

                // If the process can complete within the quantum
                if (arr[i].RT <= quantum) {
                    time += arr[i].RT;
                    arr[i].RT = 0;
                    arr[i].CT = time;
                    arr[i].TT = time - arr[i].AT;
                    arr[i].WT = arr[i].TT - arr[i].BT;
                    count++;
                } else {
                    // Process for the given quantum and reduce remaining time
                    time += quantum;
                    arr[i].RT -= quantum;
                }
            }
        }

        // If no process was ready, increment time
        if (allIdle) {
            time++;
        }
    }
}

void printProcessData(Process processes[], int n) {
    float totalWaitingTime = 0, totalTurnaroundTime = 0;

    // Table header
    printf("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    
    // Loop through each process to display its data
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].AT,
               processes[i].BT,
               processes[i].CT,
               processes[i].TT,
               processes[i].WT);
        totalWaitingTime += processes[i].WT;
        totalTurnaroundTime += processes[i].TT;
    }

    // Display average waiting and turnaround times
    printf("Average Waiting Time: %.2f\n", totalWaitingTime / n);
    printf("Average Turnaround Time: %.2f\n", totalTurnaroundTime / n);
}

int main() {
    int n, choice, quantum;
    Process processes[100];

    // Get number of processes from user
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Input arrival time and burst time for each process
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter arrival time and burst time for process %d: ", processes[i].pid);
        scanf("%d%d", &processes[i].AT, &processes[i].BT);
        processes[i].RT = processes[i].BT; // Initialize remaining time
    }

    // Display menu and loop until user chooses to exit
    do {
        printf("\nCPU Scheduling Menu:\n");
        printf("1. Shortest Job First (Preemptive)\n");
        printf("2. Round Robin\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Execute the chosen scheduling algorithm
        switch (choice) {
            case 1:
                // Reset remaining time for SJF Preemptive
                for (int i = 0; i < n; i++) {
                    processes[i].RT = processes[i].BT;
                }
                SJFprem(processes, n);
                printf("\nResults for SJF (Preemptive) Scheduling:\n");
                printProcessData(processes, n);
                break;

            case 2:
                // Get quantum for Round Robin scheduling
                printf("Enter time quantum: ");
                scanf("%d", &quantum);

                // Reset remaining time for Round Robin
                for (int i = 0; i < n; i++) {
                    processes[i].RT = processes[i].BT;
                }
                roundRobin(processes, n, quantum);
                printf("\nResults for Round Robin Scheduling:\n");
                printProcessData(processes, n);
                break;

            case 3:
                // Exit the program
                printf("Exiting program.\n");
                break;

            default:
                // Invalid choice handling
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 3); // Repeat until user selects "Exit"

    return 0;
}
