#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#define ARRAY_SIZE 16

const int FLOOR = -100;
const int CEILING = 100;
const int TOTAL_PRODUCER_THREADS = 3;
const int TOTAL_CONSUMER_THREADS = 4;

const int BUFFER_SIZE = 10;

const char LOWEST_ASCII_CHAR = '!';
const char HIGHEST_ASCII_CHAR = '~';

struct ThreadArgs {
    char string[ARRAY_SIZE];
    int numbers[ARRAY_SIZE];
    bool flagUnset;
};
typedef struct ThreadArgs ThreadArgs;

void *producer(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs *)args;

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        threadArgs->string[i] = LOWEST_ASCII_CHAR +
                                (rand() % ((int)HIGHEST_ASCII_CHAR - (int)LOWEST_ASCII_CHAR + 1)) +
                                (int)LOWEST_ASCII_CHAR;
    }

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        threadArgs->numbers[i] = rand() % (CEILING - FLOOR + 1) + FLOOR;
    }

    if (threadArgs->flagUnset)
        threadArgs->flagUnset = false;
    else
        threadArgs->flagUnset = true;
    return NULL;
}

void displayNumbers (int array[], size_t size) {
    for (int i = 0; i < size; ++i) {
        printf("%d", array[i]);
    }
    printf("\n");
}

char *intArrayToString(int array[], int size) {
    char *str = (char *)malloc((size + 1) * sizeof(char));
    for (int i = 0; i < size; ++i) {
        str[i] = (char)array[i] + LOWEST_ASCII_CHAR;
    }
    str[size] = '\0';
    return str;
}

int main() {
    srand(time(NULL));
    pthread_t threadId;
//    printf("threadId=%lu\n", threadId); // Use %lu for pthread_t
    ThreadArgs threadArgs = {{}, {}, false};
    pthread_create(&threadId, NULL, producer, (void *)&threadArgs);
    pthread_join(threadId, NULL);

    printf("PRODUCER OUTPUT\n");
    printf("---------------------\n");
    printf("threadId address:%p\n", &threadId);
    printf("threadId:%d\n", threadId);
    printf("string:%s\n", threadArgs.string);

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("num[%d]:%d\n", i, threadArgs.numbers[i]);
    }


//    char *numbersString  = intArrayToString(threadArgs.numbers, BUFFER_SIZE);
//    displayNumbers(threadArgs.numbers, ARRAY_SIZE);
//    printf("threadId:%lu\nstring:%s\nnumbers:%s\nflagUnset:%d\n",
//           threadId, threadArgs.string, numbersString, threadArgs.flagUnset);
//
//    // Don't forget to free the dynamically allocated memory
//    free(numbersString);

    return 0;
}
