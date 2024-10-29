#include <sys/types.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// Merge function to combine sorted halves
void merge(int arr[], int l, int mid, int h) {
    int temp[h-l+1];
    int i = l, j = mid + 1, k = 0;

    // Merge elements in sorted order
    while(i <= mid && j <= h) {
        if(arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }

    // Copy remaining elements from left and right halves
    while(i <= mid) temp[k++] = arr[i++];
    while(j <= h) temp[k++] = arr[j++];

    // Copy merged array back to original
    for(int x = l; x <= h; x++) arr[x] = temp[x - l];
}

// Merge Sort function
void mergeSort(int arr[], int l, int h) {
    if (l >= h) return;
    int mid = (l + h) / 2;
    mergeSort(arr, l, mid);        // Sort left half
    mergeSort(arr, mid + 1, h);    // Sort right half
    merge(arr, l, mid, h);         // Merge sorted halves
}

int main() {
    int choice;

    // Display menu options
    printf("\n********** MENU **********\n");
    printf("1. Normal Execution (Sorting)\n");
    printf("2. Demonstrate Zombie State\n");
    printf("3. Demonstrate Orphan State\n");
    printf("Enter Your Choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: { // Normal execution with sorting
            int arr[5], x;
            printf("\nEnter Five Integers to be Sorted:\n");
            for(int i = 0; i < 5; i++) {
                printf("Enter Integer %d: ", i + 1);
                scanf("%d", &arr[i]);
            }

            pid_t pid = fork(); // Create child process

            if (pid == -1) {
                perror("Error in forking process");
            } else if (pid == 0) { // Child Process
                printf("\nInside Child Process\n");
                mergeSort(arr, 0, 4);
                printf("Array Sorted by Child Process: ");
                for(int i = 0; i < 5; i++) printf("%d ", arr[i]);
                printf("\n");
                exit(0);
            } else { // Parent Process
                printf("\nInside Parent Process\n");
                mergeSort(arr, 0, 4);
                printf("Array Sorted by Parent Process: ");
                for(int i = 0; i < 5; i++) printf("%d ", arr[i]);
                printf("\nWaiting for Child Process to Terminate...\n");
                wait(NULL);
                printf("Child Process Terminated. Parent Exiting.\n");
            }
            break;
        }

        case 2: { // Demonstrate zombie process
            pid_t pid1 = fork();

            if (pid1 == -1) {
                perror("Error in forking process");
            } else if (pid1 == 0) { // Child Process
                printf("\nInside Child Process\n");
                printf("Child PID = %d\n", getpid());
                printf("Parent PID = %d\n", getppid());
                exit(0); // Child exits, becomes zombie
            } else { // Parent Process
                sleep(5); // Delay to allow child to enter zombie state
                printf("\nInside Parent Process\n");
                printf("Demonstrating Zombie Process\n");
                system("ps | grep defunct"); // List processes, showing zombie
            }
            break;
        }

        case 3: { // Demonstrate orphan process
            pid_t pid2 = fork();

            if (pid2 == -1) {
                perror("Error in forking process");
            } else if (pid2 == 0) { // Child Process
                sleep(2); // Wait for parent to terminate
                printf("\nInside Child Process (Now Orphan)\n");
                printf("Child PID = %d\n", getpid());
                printf("New Parent PID (adopted) = %d\n", getppid());
            } else { // Parent Process
                printf("\nInside Parent Process\n");
                printf("Parent PID = %d\n", getpid());
                printf("Child PID = %d\n", pid2);
                exit(0); // Parent exits, making child an orphan
            }
            break;
        }

        default:
            printf("Invalid Choice\n");
    }
}
