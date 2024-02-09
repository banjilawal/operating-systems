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
const int WRITE_COMPLETED = 522;
const int EMPTY_ADDRESS = -1;
const int ADDRESS_SET = 800;
const int ADDRESS_UNSET = -800;

const int COLUMNS = 2;
const int BUFFER_SIZE = 16;


const char* printCode (int code) {
    if (code == LOCATION_NOT_READ)
        return "LOCATION NOT READ";
    else if (code == READ_COMPLETED)
        return "READ_COMPLETED";
    else if (code == WRITE_COMPLETED)
        return "WRITE_COMPLETED";
    else if (code == EMPTY_ADDRESS)
        return "EMPTY_ADDRESS";
    else if (code == ADDRESS_SET)
        return "ADDRESS_SET";
    else if (code == ADDRESS_UNSET)
        return "ADDRESS_UNSET";
    else
        return "INVALID CODE";
}

int randomInt (int floor, int ceiling) {
    return rand() % (ceiling - floor + 1) + floor;
}

void writer (int storage[], int transactionLog[], size_t storageSize, int *flagPointer, int *address) {
    int tries = 0;
    int maxTries = BUFFER_SIZE / 3;
//    printf("entered writer with state %s\n", printCode(*flagPointer));
    if (*flagPointer != READ_COMPLETED) {
        printf("flag location %p not set to READ_COMPLETED. No WRITES can be made\n", flagPointer, printCode(*flagPointer));
        return;
    }
    int index = 0;
//    printf("flag location:%p\n", flagPointer);
    *address = rand() % storageSize;
    int number = randomInt(FLOOR, CEILING);
    while (transactionLog[*address] != ADDRESS_UNSET && tries < maxTries) {
        ++tries;
        *address = rand() % storageSize;
    }
    if (tries == maxTries) {
        printf("no locations can be overwritten. Try later.\n");
        return;
    }
    else {
        storage[*address] = number;
        transactionLog[*address] = ADDRESS_SET;
        *flagPointer = WRITE_COMPLETED;
//        printf("wrote %d to location -> %d\n", storage[*address],  *address);
//        printf("transactionLog[%d]:%s\n", *address, printCode(transactionLog[*address]));
    }
//
//
//    for (index = 0; index < storageSize; ++index) {
//        if (transactionLog[index] == EMPTY_ADDRESS) {
//            storage[location] = number;
//            transactionLog[index] = location;
//            *flagPointer = WRITE_COMPLETED;
//            printf("wrote %d to location -> %d\n", storage[location],  location);
//            return;
//        }
//    }
//    printf("Could not find an empty slot for recording the transaction\n");
}


void reader (int storage[], int transactionLog[], int reads[], size_t storageSize, int *flagPointer, int *address) {
//    printf("\tentered reader with state %s\n", printCode(*flagPointer));
    if (*flagPointer != WRITE_COMPLETED ) {
        printf("\tflag location %p not set to WRITE_COMPLETED. No READS can be made\n", flagPointer, printCode(*flagPointer));
        return;
    }
//    printf("\ttransactionLog[%d]:%s\n", *address, printCode(transactionLog[*address]));
    if (transactionLog[*address] != ADDRESS_SET) {
        printf("\tI/O operation at  %d. Cannot read right now\n", *address);
        return;
    }
    else {
        reads[*address] = storage[*address];
        transactionLog[*address] = ADDRESS_UNSET;
        *flagPointer = READ_COMPLETED;
    }
}

void flushWriterLog (int writerLog[], size_t size) {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        writerLog[i] = ADDRESS_UNSET;
    }

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (writerLog[i] != ADDRESS_UNSET) {
            printf("writeLog flush failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void printWriteTransactions (int storage[], int transactionLog[], int reads[], size_t size) {
    for (int i = 0; i < size; ++i) {
        if (transactionLog[i] == ADDRESS_SET) {
            printf("store[%d]=%d\t", i, storage[i]);
            printf("\ttransactionLog[%d]:%s", i, printCode(transactionLog[i]));
            printf("\treads[%d]->%d\n", i, reads[i]);
        }
    }
}

void printReadTransactions (int storage[], int transactionLog[], int reads[], size_t size) {
    for (int i = 0; i < size; ++i) {
        if (transactionLog[i] != ADDRESS_UNSET) {
            printf("store[%d]=%d\t", i, storage[i]);
            printf("\ttransactionLog[%d]:%d", i, printCode(transactionLog[i]));
            printf("\treads[%d]->%d\n", i, reads[1]);
        }
    }
}

void printStorageContent (int storage[], int transactionLog[], int reads[], size_t size, bool writesOnly) {
    if (writesOnly) {
        printWriteTransactions(storage, transactionLog, reads, size);
    }
    else {
        for (int i = 0; i < size; ++i) {
            printf("store[%d]=%d\t transactionLog[%d]:", i, storage[i], i);
            if (transactionLog[i] == EMPTY_ADDRESS ) {
                printf("%s\ttransactionLog[%d]->", printCode(EMPTY_ADDRESS), i);
            } else {
                printf("%d\ttransactionLog[%d]->", printCode(transactionLog[i]), i);
            }
            printf("%d\n", reads[i]);
        }
    }
}

void printSystemState (int storage[], int transactionLog[], int reads[], size_t size) {
    for (int i = 0; i < size; ++i) {
        printf("storage[%d]=%d", i, storage[i]);
        printf("\ttransactionLog[%d]:%s", i, printCode(transactionLog[i]));
        printf("\tread[%d]->%d\n", i, reads[i]);
    }
}

void displayIOTransactions (int storage[], int transactionLog[], int reads[], size_t size) {
    printf("\nWRITING TRANSACTIONS\n");
    printf("----------------------------- \n");
    printWriteTransactions(storage, transactionLog, reads, size);

    printf("\nREADING TRANSACTIONS\n");
    printf("----------------------------- \n");
    printReadTransactions(storage, transactionLog, reads, BUFFER_SIZE);
}

void writingManager (size_t jobCount, int storage[], int writerLog[], size_t storageSize, int *flag, int *address) {
    if (jobCount > storageSize) {
        printf("The number of write jobs is larger than the storage space. This would overwrite data\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < jobCount; ++i) {
        writer(storage, writerLog, storageSize, flag, address);
    }
}

void readingManager (size_t jobCount, int storage[], int transactionLog[], int reads[], size_t storageSize, int *flag, int *address) {
    if (jobCount > storageSize) {
        printf("The number of read jobs is larger than the storage space. This would overwrite data\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < jobCount; ++i) {
        *address = rand() % storageSize;
        reader(storage, transactionLog, reads, storageSize, flag, address);
    }
}

void ioManager (size_t jobCount, int storage[], int transactionLog[], int reads[], size_t storageSize, int *flag, int *address) {
    int jobNumber = 1;
    for (int i = 0; i < jobCount; ++i) {

        writer(storage, transactionLog, storageSize, flag, address);
        printf("%d: transactionLog[%d]:%s. %d written to storage[%d]\n",
               jobNumber, *address, printCode(transactionLog[*address]),
               storage[*address],  *address);
//        printf("transactionLog[%d]:%s\n", *address, printCode(transactionLog[*address]));
        reader(storage, transactionLog, reads, storageSize, flag, address);
        printf("%d: transactionLog[%d]:%s. %d read from storage[%d]' into to (reads[%d]=%d)\n",
               (jobNumber + 1), *address, printCode(transactionLog[*address]), storage[*address], *address, *address, reads[*address]);
        jobNumber +=2;
        writer(storage, transactionLog, storageSize, flag, address);
        writer(storage, transactionLog, storageSize, flag, address);
    }
}

int main () {
    srand(time(NULL));
    int flag = READ_COMPLETED;
    int *flagPointer = &flag;

    int address = EMPTY_ADDRESS;
    int *addressPointer = &address;

    int storage[BUFFER_SIZE];
    int transactionLog[BUFFER_SIZE];
    int reads[BUFFER_SIZE];

    flushWriterLog(transactionLog, BUFFER_SIZE);
    printf("transactionLog has been flushed.\n");

    printf("\nINITIAL STORAGE SYSTEM STATE\n");
    printf("--------------------------\n");
    printSystemState(storage, transactionLog, reads, BUFFER_SIZE);

    int totalJobs = rand() % (BUFFER_SIZE) + 1;
    ioManager(totalJobs, storage, transactionLog, reads, BUFFER_SIZE, &flag, &address);
//    printStorageContent(storage, transactionLog, reads, BUFFER_SIZE, false);

    printf("\nSTORAGE SYSTEM STATE AFTER %d JOBS\n", totalJobs);
    printf("--------------------------------------------------\n");
    printSystemState(storage, transactionLog, reads, BUFFER_SIZE);
//
//    int writeCount = rand() % (BUFFER_SIZE) + 1;
//    printf("\nINITIAL FLAG STATE\n");
//    printf("--------------------------\n");
//    printf("FLAG_ADDRESS:%p\n", flagPointer);
//    printf("FLAG=%s\n", printCode(*flagPointer), writeCount);
//    printf("writeCount:%d\n", writeCount);
//    writingManager(writeCount, storage, transactionLog, BUFFER_SIZE, &flag, &address);
//    printf("\nREPORT OF %d WRITE OPERATIONS\n", writeCount);
//    printf("----------------------------- \n");
//
//    printf("\nFINAL STORAGE STATE\n");
//    printf("--------------------------\n");
//    printStorageContent(storage, transactionLog, reads, BUFFER_SIZE, true);
//
//    int readCount = rand() % (BUFFER_SIZE) + 1;
//    printf("readCount:%d\n", readCount);
////    reader(storage, transactionLog, reads, BUFFER_SIZE, &flag);
//    readingManager(readCount, storage, transactionLog, reads, BUFFER_SIZE, &flag, &address);
//    printf("\nREPORT OF %d READ OPERATIONS\n", readCount);
//    printf("----------------------------- \n");
//    printReadTransactions(storage, transactionLog, reads, BUFFER_SIZE);
    return 0;
}