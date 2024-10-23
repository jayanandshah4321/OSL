#include<stdio.h>
#include<stdbool.h>

int m,n;

int allocation[10][10],max[10][10],need[10][10],available[10];

void calculateNeed(){
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            need[i][j]=max[i][j]-allocation[i][j];
        }
    }
}

bool isSafe() {
    int work[10], finish[10] = {0};
    for (int i = 0; i < m; i++) {
        work[i] = available[i];
    }
    int safeseq[10], index = 0;
    int count = 0;

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) {
                bool flag = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        flag = false;
                        break;
                    }
                }

                if (flag) {
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                    safeseq[index++] = i;
                    finish[i] = 1;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) {
            return false;  // If no progress is made, the system is not safe
        }
    }

    printf("System is in a safe state.\nSafe sequence: ");
    for (int i = 0; i < n; i++) {
        printf("P%d ", safeseq[i]);
    }
    printf("\n");

    return true;
}

bool requestResources(int process,int request[]){
    //check if req is less than need
    for(int i=0;i<m;i++){
        if(request[i]>need[process][i]){
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
    }
    //check if req is < available
    for(int i=0;i<m;i++){
        if(request[i]>available[i]){
            printf("Process P%d must wait; resources are not available.\n", process);
            return false;
        }
    }

    //assume karlo ki hamne ye req accept karli

    for(int i=0;i<m;i++){
        available[i]-=request[i];
        allocation[process][i]+=request[i];
        need[process][i]-=request[i];
    }

    //now lets check the system is in safe state or not

    if (isSafe()) {
        printf("Resources allocated successfully to P%d.\n", process);
        return true;
    }
    else{
        // Rollback if not safe
        printf("System is not in a safe state after allocation. Rolling back.\n");
        for (int i = 0; i < m; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
    }
     return false;

}

void displayNeedMatrix() {
    printf("Need Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}

int main(){
    printf("enter the no of processes");
    scanf("%d",&n);
    printf("Enter the number of resource types: ");
    scanf("%d", &m);

    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }
    printf("Enter the max matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter the available resources:\n");
    for (int i = 0; i < m; i++) {
        scanf("%d", &available[i]);
    }

    calculateNeed();


    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Show Need Matrix\n");
        printf("2. Check if the System is Safe\n");
        printf("3. Request Resources\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayNeedMatrix();
                break;
            case 2:
                if (!isSafe()) {
                    printf("System is not in a safe state.\n");
                }
                break;
            case 3: {
                int process, request[10];
                printf("Enter process number (0 to %d): ", n - 1);
                scanf("%d", &process);
                printf("Enter the request for resources:\n");
                for (int i = 0; i < m; i++) {
                    scanf("%d", &request[i]);
                }
                requestResources(process, request);
                break;
            }
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}