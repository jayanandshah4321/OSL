#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int main() {
    char sentence[256];
    char result[256];

    // Create FIFOs if they don't exist
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    while (1) {
        // Get sentence from user
        printf("Enter a sentence (or 'exit' to quit): ");
        fgets(sentence, sizeof(sentence), stdin);

        // Check for exit condition
        if (strncmp(sentence, "exit", 4) == 0) {
            break;
        }

        // Write the sentence to FIFO1
        int fd1 = open(FIFO1, O_WRONLY);
        write(fd1, sentence, strlen(sentence) + 1);
        close(fd1);

        // Read the result from FIFO2 and display it
        int fd2 = open(FIFO2, O_RDONLY);
        read(fd2, result, sizeof(result));
        printf("Result from Process 2: %s\n", result);
        close(fd2);
    }

    // Clean up FIFOs
    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}
