#include<stdio.h>
#include <limits.h>
#define MAX 100

int search(int frames[],int n,int page){
    for(int i=0;i<n;i++){
        if(frames[i]==page){
            return 1;
        }
    }
    return 0;
}

void fcfs(int pages[],int n,int frames[],int frameSize){
    int index=0,pageFault=0;
    for(int i=0;i<n;i++){
        printf("Page %d: ",pages[i]);
        if(!search(frames,frameSize,pages[i])){
            printf("Page fault occurred.\n");
            frames[index]=pages[i];
            index=(index+1)%frameSize;
            pageFault++;
        }
        else{
            printf("No page fault.\n");
        }
        printf("Page Frame: ");
        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }
        printf("\n");
        printf("FCFS Page Faults: %d\n", pageFault);
    }
}
void lru(int pages[],int n,int frames[],int frameSize){
    int time[MAX],pageFaults = 0, least, index = 0;
    
    for(int i=0;i<n;i++){
        printf("Page %d: ", pages[i]);
        if(!search(frames,frameSize,pages[i])){
            printf("Page fault occurred.\n");
            if(index<frameSize){
                //no tension
                frames[index]=pages[i];
                time[index]=i;
                index++;
            }
            else{
                // All frames are full, need to replace a page
                least = 0;
                for(int j=0;j<frameSize;j++)
                {
                    if(time[j]<time[least]){
                        least=j;
                    }
                }
                frames[least]=pages[i];
                time[least]=i;
            }
            pageFaults++;
        }
        else{
            //no page fault
            printf("No page fault.\n");
            for (int j = 0; j < frameSize; j++) {
                if (frames[j] == pages[i]) {
                    time[j] = i;  // Update the access time
                    break;
                }
            }
        }

        // Display the current state of the frames
        printf("Page Frame: ");
        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }
        printf("\n");
    }
    printf("LRU Page Faults: %d\n", pageFaults);
}

int helperOptimal(int pages[],int n,int frames[],int frameSize,int index){
    int a=frames[0],b=frames[1],c=frames[2],d=frames[3];
    int count=4,i=index;
    while(i<n && count>1){
        if(pages[i]==a){
            a=-1;count--;
        }
        else if(pages[i]==b){
            b=-1;count--;
        }
        else if(pages[i]==c){
            c=-1;count--;
        }
        else if(pages[i]==d){
            d=-1;count--;
        }
        i++;
    }

    if(a!=-1){
        return 0;
    }
    else if(b!=-1){
        return 1;
    }
    else if(c!=-1){
        return 2;
    }
    else if(d!=-1){
        return 3;
    }
    return 0;
}
void optimal(int pages[],int n,int frames[],int frameSize){
    int pagefaults=0;
    for(int i=0;i<n;i++){
        printf("Page %d: ", pages[i]);
        if(!search(frames,frameSize,pages[i])){
            pagefaults++;
            printf("Page fault occurred.\n");
            if(i<frameSize){
                //means space is there
                frames[i]=pages[i];
            }
            else{
                int temp=helperOptimal(pages,n,frames,frameSize,i+1);
                frames[temp]=pages[i];
            }
        }
        else{
            //nothing chill
             printf("No page fault.\n");
        }
        printf("Page Frame: ");
        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }
        printf("\n");
        
    }
    printf("Optimal Page Faults: %d\n", pagefaults);
}

int main(){
    int n,frameSize,choice;
    printf("enter the numbers pages:");
    scanf("%d", &n);
    
    int pages[MAX];
    printf("Enter the reference string: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }
    
    printf("Enter frame size (minimum 3): ");
    scanf("%d", &frameSize);
    int frames[frameSize];

    do {
        // Display menu
        printf("\nPage Replacement Algorithms\n");
        printf("1. FCFS\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Reset frames for each algorithm
        for (int i = 0; i < frameSize; i++) {
            frames[i] = -1; // Initialize frames to -1 (empty)
        }

        // Process user choice
        switch (choice) {
            case 1:
                printf("\n--- FCFS Page Replacement ---\n");
                fcfs(pages, n, frames, frameSize);
                break;
            case 2:
                printf("\n--- LRU Page Replacement ---\n");
                lru(pages, n, frames, frameSize);
                break;
            case 3:
                printf("\n--- Optimal Page Replacement ---\n");
                optimal(pages, n, frames, frameSize);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please select a valid option.\n");
        }
    } while (choice != 4);

    return 0;
}