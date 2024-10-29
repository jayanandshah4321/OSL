#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// Function to sort the array
void sortArray(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Sort the array
    sortArray(arr, n);

    // Fork the process
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {  // Child process
        // Prepare arguments for execve
        char *args[n + 2];
        args[0] = "./child"; // Program to run in child process
        for (int i = 0; i < n; i++) {
            args[i + 1] = (char *)malloc(10);
            sprintf(args[i + 1], "%d", arr[i]);
        }
        args[n + 1] = NULL; // Null-terminate the argument list

        // Use execve to execute child program
        execve(args[0], args, NULL);
        perror("execve failed"); // If execve fails
    } else {  // Parent process
        wait(NULL); // Wait for child to finish
        printf("Parent process finished.\n");
    }

    return 0;
}
