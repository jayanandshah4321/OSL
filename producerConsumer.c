#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Constants for buffer size and number of producers and consumers
#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

// Shared buffer and indices for producer-consumer logic
int buffer[BUFFER_SIZE];
int in = 0, out = 0;

// Semaphores and mutex for managing shared resources
sem_t empty, full;
pthread_mutex_t mutex;

// Producer function
void* producer(void *arg) {
    int id = *((int*)arg);  // Retrieve producer ID from argument

    // Producing 1000 items
    for (int i = 0; i < 1000; i++) {
        // Wait if buffer is full
        sem_wait(&empty);
        // Lock the mutex to access the buffer
        pthread_mutex_lock(&mutex);

        // Produce an item and add it to the buffer
        buffer[in] = i;
        printf("Producer %d produced %d\n", id, i);
        in = (in + 1) % BUFFER_SIZE;

        // Unlock the mutex and signal that a new item is available
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    return NULL;
}

// Consumer function
void* consumer(void *arg) {
    int id = *((int*)arg);  // Retrieve consumer ID from argument

    // Consuming 1000 items
    for (int i = 0; i < 1000; i++) {
        // Wait if buffer is empty
        sem_wait(&full);
        // Lock the mutex to access the buffer
        pthread_mutex_lock(&mutex);

        // Consume an item from the buffer
        int item = buffer[out];
        printf("Consumer %d consumed %d\n", id, item);
        out = (out + 1) % BUFFER_SIZE;

        // Unlock the mutex and signal that a space is available
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    return NULL;
}

int main() {
    // Array of producer and consumer IDs for thread identification
    int producer_ids[NUM_PRODUCERS], consumer_ids[NUM_CONSUMERS];
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);  // Indicates available spaces in the buffer
    sem_init(&full, 0, 0);             // Indicates filled slots in the buffer
    pthread_mutex_init(&mutex, NULL);

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i + 1;  // Assign unique ID to each producer
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1;  // Assign unique ID to each consumer
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // Wait for all producer threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    // Wait for all consumer threads to finish
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Clean up semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
