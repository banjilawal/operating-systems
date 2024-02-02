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

const int LOCATION_NOT_READ = -255;
const int READ_COMPLETED = 1;
const int WRITE_COMPLETED = 0;
const int EMPTY_ADDRESS = -1;

const int COLUMNS = 2;
const int BUFFER_SIZE = 16;


const char* printCode (int code) {
    if (code == EMPTY_ADDRESS)
        return "EMPTY_ADDRESS";
    else if (code == WRITE_COMPLETED)
        return "WRITE_COMPLETED";
    else if (code == READ_COMPLETED)
        return "READ_COMPLETED";
    else if (code == LOCATION_NOT_READ)
        return "LOCATION NOT READ";
    else
        return "INVALID CODE";
}

int randomInt (int floor, int ceiling) {
    return rand() % (ceiling - floor + 1) + floor;
}

void writer (int storage[], int writesLog[], size_t storageSize, int location, int *flagPointer) {
    if (*flagPointer != READ_COMPLETED) {
        printf("flag location %p set to %s Wait for the flag to change\n",flagPointer, printCode(*flagPointer));
        return;
    }
    int index = 0;
//    printf("flag location:%p\n", flagPointer);
    location = rand() % storageSize;
    int number = randomInt(FLOOR, CEILING);

    for (index = 0; index < storageSize; ++index) {
        if (writesLog[index] == EMPTY_ADDRESS) {
            storage[location] = number;
            writesLog[index] = location;
            *flagPointer = WRITE_COMPLETED;
            printf("wrote %d to location -> %d\n", storage[location],  location);
            return;
        }
    }
    printf("Could not find an empty slot for recording the transaction\n");
}

void flushWritesLog (int writeLog[], size_t size) {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        writeLog[i] = EMPTY_ADDRESS;
    }

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (writeLog[i] != EMPTY_ADDRESS) {
            printf("writeLog flush failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void flushReadsLog (int readLog[][COLUMNS], size_t size) {
    for (int i = 0; i < size; ++i) {
        readLog[i][0] = LOCATION_NOT_READ;
        readLog[i][1] = INT_MIN;
    }

    // Assuring the readsLog has been flushed.
    for (int i = 0; i < size; ++i) {
        if (readLog[i][0] != LOCATION_NOT_READ) {
            printf("flushing readLog failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void printStorageContent (int storage[], int writeLog[], int readLog[][COLUMNS], size_t size) {
    for (int i = 0; i < size; ++i) {
        printf("store[%d]=%d\t writeLog[%d]:", i, storage[i], i);
        if (writeLog[i] == EMPTY_ADDRESS ) {
            printf("%s\treadLog[%d]->", printCode(EMPTY_ADDRESS), i);
        } else {
            printf("%d\treadLog[%d]->", writeLog[i], i);
        }
        if (readLog[i][0] == LOCATION_NOT_READ) {
            printf("%s\n", printCode(readLog[i][0]));
        } else {
            printf("%d\n", readLog[i][0]);
        }
    }
}

int main () {
    srand(time(NULL));
    int flag = READ_COMPLETED;
    int *flagPointer = &flag;

    int storage[BUFFER_SIZE];
    int writesLog[BUFFER_SIZE];
    int readsLog[BUFFER_SIZE][COLUMNS];

    flushReadsLog(readsLog, BUFFER_SIZE );
    printf("readsLog log has been initialize.\n");

    flushWritesLog(writesLog, BUFFER_SIZE);
    printf("writesLog has been initialized.\n");

    int writeCount = rand() % (BUFFER_SIZE) + 1;
    printf("flagAddress:%p contains %d \twriteCount:%d\n", flagPointer, *flagPointer, writeCount);

    printStorageContent(storage, writesLog, readsLog, BUFFER_SIZE);

//    printf("initial storage state\n");
//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        printf("storage[%d]:%d\n", i, storage[i]);
//    }
    for (int i = 0; i <= writeCount; ++i) {
        writer(storage, writesLog, BUFFER_SIZE, i, &flag);
    }
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        printf("storage[%d]:%d\t writesLog[%d]=%d\n", i, storage[i], i, writesLog[i]);
    }
    printf("writeCount=%d\n", writeCount);


//
//    printf("random number=%d\n", randomInt(FLOOR, CEILING));

//    flushWrites(writesLog, BUFFER_SIZE);
//    flushReads(*readsLog, BUFFER_SIZE);

//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        for (int j = 0; j < COLUMNS ; ++j) {
//            printf(" [%d][%d]:%d", i, j, readsLog[i * COLUMNS + j]);
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
////    readManager(storage, BUFFER_SIZE, writeLocations, readsLog);
//    printf("READS\n-------------------\n");
//    for (int i = 0; i < BUFFER_SIZE; ++i) {
//        for (int j = 0; j < COLUMNS ; ++j) {
//            printf(" readsLog[%d][%d]:%d", i, j, readsLog[i][j]);
//        }
//        printf("\n");
//    }
////    printf("final cursor:%d\n", cursor);
    printf("%p flag=%d %d\n",flagPointer,  *flagPointer, flag);
    for (int i = 0; i <= writeCount; ++i) {
        writer(storage, writesLog, BUFFER_SIZE, i, &flag);
    }
    return 0;
}
