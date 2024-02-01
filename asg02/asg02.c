//
// Created by griot on 1/24/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

const int FLOOR = -1024;
const int CEILING = 1024;

const int LOCATION_NOT_READ = 0;
const int LOCATION_FREE_FOR_WRITING = 1;
const int EMPTY_ADDRESS = -1;

const int COLUMNS = 2;
const int BUFFER_SIZE = 16;
const double NEGATIVE_NUMBER_RATIO = 0.234;

int randomInt (int floor, int ceiling) {
//    srand(time(NULL));
    return rand() % (ceiling - floor + 1) + floor;
}

void writer (int storage[], size_t storageSize, int location) {
    location = rand() % storageSize;
    storage[location] =  randomInt(FLOOR, CEILING);
    printf("write %d to location -> %d\n", storage[location],  location);
}

int main () {
    srand(time(NULL));
    int writeCount = rand() % (BUFFER_SIZE) + 1;
    int storage[BUFFER_SIZE];
    int writtenLocationsLog[BUFFER_SIZE];
    int readLocationsLog[BUFFER_SIZE][COLUMNS];

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        readLocationsLog[i][0] = LOCATION_NOT_READ;
        readLocationsLog[i][1] = INT_MIN;
    }
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (readLocationsLog[i][0] != LOCATION_NOT_READ) {
            printf("readLocationsLog log has not been initialized\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("readLocationsLog log has been initialize.\n");

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        writtenLocationsLog[i] = EMPTY_ADDRESS;
    }
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (writtenLocationsLog[i] != EMPTY_ADDRESS) {
            printf("error initializing writeLocationsLog\n");
        }
    }
    printf("writtenLocationsLog has been initialized\n");

     printf("initial storage state\n");
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        printf("storage[%d]:%d\n", i, storage[i]);
    }
    for (int i = 0; i <= BUFFER_SIZE / 2; ++i) {
        writer(storage, BUFFER_SIZE, i);
    }
    for (int i = 0; i < writeCount; ++i) {
        printf("storage[%d]:%d\n", i, storage[i]);
    }
    printf("writeCount=%d\n", writeCount);


//
//    printf("random number=%d\n", randomInt(FLOOR, CEILING));

//    flushWrites(writtenLocationsLog, BUFFER_SIZE);
//    flushReads(*readLocationsLog, BUFFER_SIZE);

//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        for (int j = 0; j < COLUMNS ; ++j) {
//            printf(" [%d][%d]:%d", i, j, readLocationsLog[i * COLUMNS + j]);
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
////    readManager(storage, BUFFER_SIZE, writeLocations, readLocationsLog);
//    printf("READS\n-------------------\n");
//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        for (int j = 0; j < COLUMNS ; ++j) {
//            printf(" readLocationsLog[%d][%d]:%d", i, j, readLocationsLog[i][j]);
//        }
//        printf("\n");
//    }
////    printf("final cursor:%d\n", cursor);
    return 0;
}
