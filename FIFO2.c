#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

void count_content(const char *sentence, int *char_count, int *word_count, int *line_count) {
    *char_count = *word_count = *line_count = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        (*char_count)++;
        if (sentence[i] == '\n') {
            (*line_count)++;
        }
        if (sentence[i] == ' ' || sentence[i] == '\n' || sentence[i] == '\t') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            (*word_count)++;
        }
    }
}

int main() {
    char sentence[256];
    char result[256];
    int char_count, word_count, line_count;

    while (1) {
        // Read sentence from FIFO1
        int fd1 = open(FIFO1, O_RDONLY);
        read(fd1, sentence, sizeof(sentence));
        close(fd1);

        // Check for exit condition
        if (strncmp(sentence, "exit", 4) == 0) {
            break;
        }

        // Count characters, words, and lines
        count_content(sentence, &char_count, &word_count, &line_count);

        // Prepare the result message
        snprintf(result, sizeof(result), "Characters: %d, Words: %d, Lines: %d", char_count, word_count, line_count);

        // Write the result to FIFO2
        int fd2 = open(FIFO2, O_WRONLY);
        write(fd2, result, strlen(result) + 1);
        close(fd2);
    }

    return 0;
}
