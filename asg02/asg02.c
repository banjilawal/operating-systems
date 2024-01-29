//
// Created by griot on 1/24/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

const int NOT_READ = 0;
const int CAN_OVERWRITE = 1;
const int EMPTY_ADDRESS = -1;

const int COLUMNS = 2;
const int BUFFER_SIZE = 16;
const double NEGATIVE_NUMBER_RATIO = 0.234;

int randomInt () {
    int number = rand();
    double outcome = (double) rand() / RAND_MAX;
    if (outcome <= NEGATIVE_NUMBER_RATIO) {
        number *= 1;
    }
    return number;
}

void writer (int data, int *storage, size_t size, int *writes,  int (*reads[2]), bool *isWriting) {
    int index = 0;
    while ((*(reads + index)[0]) != CAN_OVERWRITE || index < size) {
        ++index;
        printf("Cannot overwrite %d in address[%d]\n", *(storage + index));
    }
    *isWriting = true;
    *(storage + index) = data;
    *(writes + index) = data;
    *isWriting = false;
}

void reader (int *storage, size_t size, int *writes, int (*reads[2]), bool *isWriting) {
    int index = 0;
    while ((*reads + index)[0] == CAN_OVERWRITE || index < size) {
        ++index;
        printf("storage[%d] has already been read for %d\n", index, storage[index]);
    }
    if (index == size || *isWriting) {
        printf("No writes have been made\n");
    } else {
        *(reads + index)[1] = *(storage + index);
        *(reads + index)[0] = CAN_OVERWRITE;
    }
}

//void producer (int array[], int size, int *currentCursor, int *previousCursor, int writes[], int reads[size][2]) {
////    printf("\tentry cursor-->%d\n", *currentCursor);
//    (*previousCursor) = *currentCursor;
//    int number = rand();
//    double outcome = (double) rand() / RAND_MAX;
//    if (outcome <= NEGATIVE_NUMBER_RATIO) {
//        number *= 1;
//    }
//    array[*currentCursor] = number;
//    writes[]
//    reads[*currentCursor][1] = INT_MIN;
////    printf("\t\tadded %d at %d\n", number, *currentCursor);
//    (*currentCursor)++;
////    printf("\texit cursor-->%d\n", *currentCursor);
//}

//void consumer (
//        int storage[],
//        int size,
//        int writeLocations[],
//        int reads[size][2]
//) {
//    int counter = 0;
//    int address =  rand() % size;
//    while (writeLocations[address] < 0 && counter < size) {
//        ++counter;
////        printf("address %d contains %d no data. Nothing to read\n", address, writeLocations[address]);
//        address =  rand() % size;
//    }
//    printf("found %d at address %d\n", storage[address], address);
//    reads[address][1] = storage[address];
//    printf("\t\tconsumer transaction! read[%d]=%d\n", address, reads[address][1]);
//    writeLocations[address] = -1;
//    printf("\t\t\tmarked %d as free with %d\n", address, writeLocations[address]);
////    for (int i = 0; i < size; ++i) {
////        if (reads[i][0] >= 0) {
////            reads[i][1] = writeLocations[location];
////            storage[location] = INT_MIN;
////            writeLocations[location] = -1;
////        }
////    }
//}

void flushWrites (int *writes, size_t size) {
    for (int i = 0; i < size; ++i) {
        *(writes + i) = EMPTY_ADDRESS;
    }
}

void flushReads (int **reads, size_t rows) {
    for (int i = 0; i < rows; ++i) {
        reads[i][0] = NOT_READ;
        reads[i][1] = INT_MIN;
    }
//    for (int i = 0; i < ; ++i) {
//
//    }
//       *(reads + i)[0] = NOT_READ;
//       *(reads + i)[1] = INT_MIN;
//    }
//    printf("Confirming readlog is erase\n");
//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        printf("[%d][1]=%d\n", i, array[i][1]);
//    }
//
}

void readManager (int readCount, int *storage, size_t size, int *writes, int (*reads[2]), bool *isWriting) {
    for (int i = 0; i < readCount; ++i) {
        printf("ReadManager run %d\n", i);
        reader(storage, size, writes, reads, isWriting);
//        consumer(storage, size, writeLocations, reads);
    }
}


void writeManager (int writeCount, int *storage, size_t size, int *writes, int (*reads[2]), bool *isWriting) {
    for (int i = 0; i < writeCount; ++i) {
        printf("Write Operation #:%d\n", i);
        writer(randomInt(), storage, size, writes, reads, isWriting);
    }
}
//void writeManager (
//        int size,
//        int totalWrites,
//        int storage[],
//        int writeLocations[size],
//        int reads[size][2]
//
//) {
//    int currentCursor = 0;
//    int previousCursor = 0;
//    for (int i = 0; i < totalWrites; ++i) {
//
//        producer(storage, size, &currentCursor, &previousCursor, writeLocations, );
////        printf("write#:%d of %d previous_cursor:%d current_cursor:%d\n",
////               i, totalWrites, previousCursor, currentCursor);
//        writeLocations[i] = previousCursor;
//    }
//}

int main () {
    srand(time(NULL));
    int storage[BUFFER_SIZE];
    int writes[BUFFER_SIZE];
    int reads[BUFFER_SIZE][COLUMNS];

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        reads[i][0] = NOT_READ;
        reads[i][1] = INT_MIN;
    }

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        writes[i] = EMPTY_ADDRESS;
    }

//    flushWrites(writes, BUFFER_SIZE);
//    flushReads(*reads, BUFFER_SIZE);

//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        for (int j = 0; j < COLUMNS ; ++j) {
//            printf(" [%d][%d]:%d", i, j, reads[i * COLUMNS + j]);
//
//        }
//        printf("\n");
//
//    }

//    writeManager(storage, BUFFER_SIZE, (BUFFER_SIZE / 2), writeLocations);
//
//    printf("Current Storage State\n---------------------------\n");
//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        printf("storage[%d]: %d\n", i, storage[i]);
//    }
//    printf("\nWrite Location Updates\n-------------------\n");
//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        if (writeLocations[i] > -1) {
//            printf("wrote %d to address %d\n", storage[i], writeLocations[i]);
//        }
//    }
//
//    readManager(storage, BUFFER_SIZE, writeLocations, reads);
    printf("READS\n-------------------\n");
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        for (int j = 0; j < COLUMNS ; ++j) {
            printf(" reads[%d][%d]:%d", i, j, reads[i][j]);
        }
        printf("\n");
    }
////    printf("final cursor:%d\n", cursor);
    return 0;
}
