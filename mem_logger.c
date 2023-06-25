#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 256
#define MAX_PAGES 256
#define LOG_SIZE 8

typedef struct {
    unsigned short logId;
    unsigned char data[6];
} LogEntry;

LogEntry logs[MAX_PAGES * PAGE_SIZE / LOG_SIZE];
int totalLogs = 0;

void writeLog(unsigned short logId, unsigned char data[6]) {
    if (totalLogs >= MAX_PAGES * PAGE_SIZE / LOG_SIZE) {
        printf("Memory is full. Erasing oldest log.\n");
        for (int i = 0; i < totalLogs - 1; i++) {
            logs[i] = logs[i + 1];
        }
        totalLogs--;
    }

    logs[totalLogs].logId = logId;
    for (int i = 0; i < 6; i++) {
        logs[totalLogs].data[i] = data[i];
    }
    totalLogs++;

    printf("Log added successfully.\n");
}

void readLog(unsigned short logId) {
    // Create a hash table to store log IDs and their corresponding indices in the logs array
    unsigned int hashTable[MAX_PAGES * PAGE_SIZE / LOG_SIZE];
    for (int i = 0; i < totalLogs; i++) {
        hashTable[logs[i].logId] = i;
    }

    // Check if the log ID exists in the hash table
    if (hashTable[logId] != 0) {
        int logIndex = hashTable[logId];
        printf("Log ID: %u\n", logs[logIndex].logId);
        printf("Data: ");
        for (int j = 0; j < 6; j++) {
            printf("%u ", logs[logIndex].data[j]);
        }
        printf("\n");
    } else {
        printf("Log not found.\n");
    }
}

void eraseLog(unsigned short logId) {
    int logIndex = -1;

    for (int i = 0; i < totalLogs; i++) {
        if (logs[i].logId == logId) {
            logIndex = i;
            break;
        }
    }

    if (logIndex != -1) {
        for (int i = logIndex; i < totalLogs - 1; i++) {
            logs[i] = logs[i + 1];
        }
        totalLogs--;
        printf("Log erased successfully.\n");
    } else {
        printf("Log not found.\n");
    }
}

void printMemoryUtilization() {
    int totalMemory = MAX_PAGES * PAGE_SIZE;
    int allocatedBlocks = totalLogs * LOG_SIZE;
    int availableMemory = totalMemory - allocatedBlocks;
    
    printf("Total Memory: %d bytes\n", totalMemory);
    printf("Allocated Blocks: %d\n", allocatedBlocks);
    printf("Available Memory: %d bytes\n", availableMemory);
}

