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
const int READ_COMPLETED = 0;
const int WRITE_COMPLETED = 11111;
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

void writer (int storage[], int writerLog[], size_t storageSize, int *flagPointer) {
    printf("\n\tentered writer with state %s\n", printCode(*flagPointer));
    if (*flagPointer != READ_COMPLETED) {
        printf("flag location %p not set to READ_COMPLETED. No WRITES can be made\n", flagPointer, printCode(*flagPointer));
        return;
    }
    int index = 0;
//    printf("flag location:%p\n", flagPointer);
    int location = rand() % storageSize;
    int number = randomInt(FLOOR, CEILING);

    for (index = 0; index < storageSize; ++index) {
        if (writerLog[index] == EMPTY_ADDRESS) {
            storage[location] = number;
            writerLog[index] = location;
            *flagPointer = WRITE_COMPLETED;
            printf("wrote %d to location -> %d\n", storage[location],  location);
            return;
        }
    }
    printf("Could not find an empty slot for recording the transaction\n");
}


void reader (int storage[], int writerLog[], int readerLog[][COLUMNS ], size_t storageSize, int *flagPointer) {
    printf("\n\tentered reader with state %s\n", printCode(*flagPointer));
    if (*flagPointer != WRITE_COMPLETED ) {
        printf("flag location %p not set to WRITE_COMPLETED. No READS can be made\n", flagPointer, printCode(*flagPointer));
        return;
    }
    for (int i = 0; i < storageSize; ++i) {
        if (writerLog[i] >= 0) {
            int memoryAddress = writerLog[i];
            readerLog[memoryAddress][1] = storage[memoryAddress];
            writerLog[i] = EMPTY_ADDRESS;
            printf("Read %d at address %d\n", readerLog[memoryAddress], memoryAddress);
        }
    }
    printf("Nothing new has been written.");
    *flagPointer = READ_COMPLETED;
}

void flushWriterLog (int writerLog[], size_t size) {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        writerLog[i] = EMPTY_ADDRESS;
    }

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (writerLog[i] != EMPTY_ADDRESS) {
            printf("writeLog flush failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void flushReaderLog (int readerLog[][COLUMNS], size_t size) {
    for (int i = 0; i < size; ++i) {
        readerLog[i][0] = LOCATION_NOT_READ;
        readerLog[i][1] = INT_MIN;
    }

    // Assuring the readsLog has been flushed.
    for (int i = 0; i < size; ++i) {
        if (readerLog[i][0] != LOCATION_NOT_READ) {
            printf("flushing readLog failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void printWriteTransactions (int storage[], int writeLog[], int readLog[][COLUMNS], size_t size) {
    int writeLocation = EMPTY_ADDRESS;
    for (int i = 0; i < size; ++i) {

        if (writeLog[i] != EMPTY_ADDRESS ) {
            writeLocation = writeLog[i];
            printf("store[%d]=%d\t", writeLocation, storage[writeLocation]);
            printf("\twriteLog[%d]:%d", i, writeLog[i]);
            printf("\treadLog[%d]->%d\n", writeLocation, readLog[writeLocation]);
        }
    }
}

void printStorageContent (int storage[], int writeLog[], int readLog[][COLUMNS], size_t size, bool writesOnly) {
    if (writesOnly) {
        printWriteTransactions(storage, writeLog, readLog, size);
    }
    else {
        for (int i = 0; i < size; ++i) {
            printf("store[%d]=%d\t writeLog[%d]:", i, storage[i], i);
            if (writeLog[i] == EMPTY_ADDRESS ) {
                printf("%s\twriteLog[%d]->", printCode(EMPTY_ADDRESS), i);
            } else {
                printf("%d\twriteLog[%d]->", writeLog[i], i);
            }
            if (readLog[i][0] == LOCATION_NOT_READ) {
                printf("%s\n", printCode(readLog[i][0]));
            } else {
                printf("%d\n", readLog[i][0]);
            }
        }
    }
}
void writingManager (size_t jobCount, int storage[], int writerLog[], size_t storageSize, int *flag) {
    if (jobCount > storageSize) {
        printf("The number of is larger than the storage space.  This would overwrite data\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < jobCount; ++i) {
        writer(storage, writerLog, storageSize, flag);
    }
}

int main () {
    srand(time(NULL));
    int flag = READ_COMPLETED;
    int *flagPointer = &flag;

    int storage[BUFFER_SIZE];
    int writerLog[BUFFER_SIZE];
    int readerLog[BUFFER_SIZE][COLUMNS];

    flushReaderLog(readerLog, BUFFER_SIZE);
    printf("readerLog log has been initialize.\n");

    flushWriterLog(writerLog, BUFFER_SIZE);
    printf("writerLog has been initialized.\n");

    printf("\nINITIAL STORAGE STATE\n");
    printf("--------------------------\n");
    printStorageContent(storage, writerLog, readerLog, BUFFER_SIZE, false);

    int writeCount = rand() % (BUFFER_SIZE) + 1;
    printf("\nIINITIAL FLAG STATE\n");
    printf("--------------------------\n");
    printf("FLAG_ADDRESS:%p\nFLAG=%s\nwriteCount:%d\n", flagPointer, printCode(*flagPointer), writeCount);

    writingManager(writeCount, storage, writerLog, BUFFER_SIZE, &flag);
    printf("\nREPORT OF %d WRITE OPERATIONS\n");
    printf("----------------------------- \n", writeCount);

    printf("\nFINAL STORAGE STATE\n");
    printf("--------------------------\n");
    printStorageContent(storage, writerLog, readerLog, BUFFER_SIZE, true);

    reader(storage, writerLog, readerLog, BUFFER_SIZE, &flag);
    writingManager(writeCount, storage, writerLog, BUFFER_SIZE, &flag);
    printf("\nREPORT OF %d WRITE OPERATIONS\n");
    printf("----------------------------- \n", writeCount);
    return 0;
}
