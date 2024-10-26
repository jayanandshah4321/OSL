DISK SCHEDULING ALGO
ASS:8

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Function to sort the request array
void sort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// SSTF (Shortest Seek Time First) algorithm implementation using sorted array
void SSTF(int arr[], int head, int n) {
    int seek_time = 0, closest, i, pos;
    int sorted[MAX]; // to store a sorted copy of the array
    int visited[MAX] = {0}; // to mark processed requests

    // Copy and sort the array
    for (i = 0; i < n; i++) sorted[i] = arr[i];
    sort(sorted, n);

    printf("SSTF Order of servicing requests: %d", head);
    for (int count = 0; count < n; count++) {
        closest = 1e9;
        // Find the closest unprocessed request to the current head position
        for (i = 0; i < n; i++) {
            if (!visited[i] && abs(sorted[i] - head) < closest) {
                closest = abs(sorted[i] - head);
                pos = i;
            }
        }
        // Update seek time and mark the request as processed
        seek_time += abs(sorted[pos] - head);
        head = sorted[pos];
        visited[pos] = 1;
        printf(" -> %d", head);
    }
    printf("\nTotal seek time: %d\n", seek_time);
}

// SCAN algorithm implementation
void SCAN(int arr[], int head, int n, int disk_size, int direction) {
    int seek_time = 0, i, j;
    arr[n] = head; // add head position to the array
    n++;
    arr[n] = direction == 1 ? disk_size - 1 : 0; // add end based on direction
    n++;

    // Sort the array of requests
    sort(arr, n);

    // Find the position of the initial head in the sorted array
    int head_index;
    for (i = 0; i < n; i++) {
        if (arr[i] == head) {
            head_index = i;
            break;
        }
    }

    printf("SCAN Order of servicing requests: ");
    // Based on the direction, serve requests
    if (direction == 1) {
        for (i = head_index; i < n; i++) printf("%d -> ", arr[i]);
        for (i = head_index - 1; i >= 0; i--) printf("%d -> ", arr[i]);
        seek_time+=abs(arr[n-1]-arr[head_index]);
    } else {
        for (i = head_index; i >= 0; i--) printf("%d -> ", arr[i]);
        for (i = head_index + 1; i < n; i++) printf("%d -> ", arr[i]);
        seek_time+=abs(arr[0]-arr[head_index]);
    }
    seek_time+=abs(arr[n-1]-arr[0]);
    printf("\nTotal seek time: %d\n", seek_time);
}

// C-LOOK algorithm implementation
void C_LOOK(int arr[], int head, int n, int disk_size) {
    int seek_time = 0, i, j;
    arr[n] = head; // add head position to the array
    n++;

    // Sort the array of requests
    sort(arr, n);

    // Find the position of the initial head in the sorted array
    int head_index;
    for (i = 0; i < n; i++) {
        if (arr[i] == head) {
            head_index = i;
            break;
        }
    }

    printf("C-LOOK Order of servicing requests: ");
    // Serve requests circularly starting from the head position
    for (i = head_index; i < n; i++) printf("%d -> ", arr[i]);
    for (i = 0; i < head_index; i++) printf("%d -> ", arr[i]);

    seek_time = abs(head - arr[n-1]) + abs(arr[n - 1] - arr[0])+abs(arr[0]-arr[head_index-1]);
    printf("\nTotal seek time: %d\n", seek_time);
}

int main() {
    int arr[MAX], n, head, disk_size, choice, direction;

    // Input section
    printf("Enter number of requests: ");
    scanf("%d", &n);
    printf("Enter request queue: ");
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter disk size: ");
    scanf("%d", &disk_size);

    // Menu-driven section
    while (1) {
        printf("\nDisk Scheduling Algorithms:\n");
        printf("1. SSTF\n2. SCAN\n3. C-LOOK\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                SSTF(arr, head, n); // Call SSTF
                break;
            case 2:
                printf("Enter direction (1 for high, 0 for low): ");
                scanf("%d", &direction);
                SCAN(arr, head, n, disk_size, direction); // Call SCAN
                break;
            case 3:
                C_LOOK(arr, head, n, disk_size); // Call C-LOOK
                break;
            case 4:
                exit(0); // Exit
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
