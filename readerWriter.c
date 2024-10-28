#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t resource_access;    // Controls access to the shared resource (writers only)
sem_t read_count_access;  // Controls access to `read_count`
int read_count = 0;       // Number of readers currently accessing the resource

void *reader(void *arg) {
    int reader_id = *((int *)arg);

    while (1) {
        // Step 1: Reader requests access to modify `read_count`
        sem_wait(&read_count_access);
        read_count++;
        if (read_count == 1) {
            // First reader locks the resource for other readers and writers
            sem_wait(&resource_access);
        }
        sem_post(&read_count_access);

        // Step 2: Reading section
        printf("Reader %d is reading\n", reader_id);
        sleep(rand() % 3);  // Simulate reading time

        // Step 3: Reader releases access to `read_count`
        sem_wait(&read_count_access);
        read_count--;
        if (read_count == 0) {
            // Last reader releases the resource
            sem_post(&resource_access);
        }
        sem_post(&read_count_access);

        sleep(1);  // Wait before next read attempt
    }
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);

    while (1) {
        // Step 1: Writer requests access to the resource
        sem_wait(&resource_access);

        // Step 2: Writing section
        printf("Writer %d is writing\n", writer_id);
        sleep(rand() % 3);  // Simulate writing time

        // Step 3: Writer releases the resource
        sem_post(&resource_access);

        sleep(1);  // Wait before next write attempt
    }
}

int main() {
    pthread_t readers[5], writers[3];
    int reader_ids[5], writer_ids[3];

    // Initialize semaphores
    sem_init(&resource_access, 0, 1);
    sem_init(&read_count_access, 0, 1);

    // Create reader threads
    for (int i = 0; i < 5; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < 3; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Join threads (in a real application, we'd want a way to stop gracefully)
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&resource_access);
    sem_destroy(&read_count_access);

    return 0;
}
