#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int size;
    int processID;
    bool isFree;
} MemoryBlock;

MemoryBlock memory[15]; // hardcode max 15 blocks, this can be adjusted
int blockCount = 0;
int curPID = 1000;

void log_action(const char *message) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "./logger_location", sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        write(fd, message, strlen(message));
    }
    close(fd);
}

void initializeMemory() {
    int numHoles, memSum = 0;
    printf("Enter the number of holes to allocate: ");
    if (scanf("%d", &numHoles) != 1 || numHoles <= 0 || numHoles > 15) {
        printf("Invalid number of holes. Must be between 1 and 15.\n");
        log_action("Memory error: Invalid number of holes during initialization");
        while (getchar() != '\n');
        return;
    }

    int holes[numHoles];
    printf("Enter hole sizes: ");
    for (int i = 0; i < numHoles; i++) {
        if (scanf("%d", &holes[i]) != 1 || holes[i] <= 0) {
            printf("Invalid hole size. Must be a positive integer.\n");
            log_action("Memory ERROR: Invalid hole size during initialization");
            while (getchar() != '\n');
            return;
        }
        memSum += holes[i];
    }
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Memory: Initialized %d KB of memory", memSum);
    log_action(logMsg);

    blockCount = numHoles;
    for (int i = 0; i < numHoles; i++) {
        memory[i].size = holes[i];
        memory[i].isFree = true;
        memory[i].processID = -1;
    }
    printf("Memory initialized with %d holes.\n", numHoles);
    snprintf(logMsg, sizeof(logMsg), "Memory: Initialized with %d holes", numHoles);
    log_action(logMsg);

}

void allocateMemory() {
    if (blockCount == 0) {
        printf("Memory not initialized. Please initialize memory first.\n");
        log_action("Memory ERROR: Allocation attempted before initialization");
        return;
    }

    int size, processID;
    printf("Process assigned PID %d\n", curPID);
    processID = curPID; // assign and inc for next alloc cycle
    curPID++;
    printf("Size (KB): ");
    if (scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid size. Must be a positive integer.\n");
        log_action("Memory ERROR: Invalid size entered during allocation");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < blockCount; i++) {
        if (memory[i].isFree && memory[i].size >= size) {
            int remaining = memory[i].size - size;
            memory[i].isFree = false;
            memory[i].processID = processID;
            memory[i].size = size;
 
            if (remaining > 0) {
                if (blockCount >= 15) {
                    printf("Block table full, cannot split hole.\n");
                    log_action("Memory ERROR: Block table full during allocation split");
                    return;
                }
                // in order to create a new block directly after we need
                // to iterate backwards and shift our memory to make room
                for (int j = blockCount; j > i + 1; j--)
                    memory[j] = memory[j - 1];
                blockCount++;
                memory[i + 1].size = remaining;
                memory[i + 1].isFree = true;
                memory[i + 1].processID = -1;
            }
 
            printf("Allocated %d KB to PID %d.\n", size, processID);
            char logMsg[64];
            snprintf(logMsg, sizeof(logMsg), "Memory: Allocated %d KB to PID %d", size, processID);
            log_action(logMsg);
            return;
        }
    }

    // if fail
    printf("Unable to find block large enough for %d KB.\n", size);
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Memory ERROR: No block large enough for %d KB", size);
    log_action(logMsg);
}

void deallocateMemory() {
    if (blockCount == 0) {
        printf("Memory not initialized. Please initialize memory first.\n");
        log_action("Memory ERROR: Deallocation attempted before initialization");
        return;
    }

    int pid;
    printf("Enter PID to free: ");
    if (scanf("%d", &pid) != 1 || pid < 1000) {
        printf("Invalid PID.\n");
        log_action("Memory ERROR: Invalid PID entered during deallocation");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < blockCount; i++) {
        if (!memory[i].isFree && memory[i].processID == pid) {
            int freedSize = memory[i].size;
            memory[i].isFree = true;
            memory[i].processID = -1;
            printf("Freed PID %d (%d KB).\n", pid, freedSize);
 
            for (int j = 0; j < blockCount - 1; j++) {
                if (memory[j].isFree && memory[j + 1].isFree) {
                    memory[j].size += memory[j + 1].size;
                    for (int k = j + 1; k < blockCount - 1; k++)
                        memory[k] = memory[k + 1];
                    blockCount--;
                    j--;
                }
            }
 
            char logMsg[64];
            snprintf(logMsg, sizeof(logMsg), "Memory: Deallocated %d KB from PID %d", freedSize, pid);
            log_action(logMsg);
            return;
        }
    }

    // also if fail
    printf("PID %d not found.\n", pid);
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Memory ERROR: PID %d not found during deallocation", pid);
    log_action(logMsg);
}

void compactMemory() {
    if (blockCount == 0) {
        printf("Memory not initialized. Please initialize memory first.\n");
        log_action("Memory ERROR: Compaction attempted before initialization");
        return;
    }

    MemoryBlock temp[15];
    int count = 0, freeTotal = 0;
    for (int i = 0; i < blockCount; i++) {
        if (!memory[i].isFree)
            temp[count++] = memory[i];
        else
            freeTotal += memory[i].size;
    }

    if (freeTotal == 0) {
        printf("No free memory to compact.\n");
        log_action("Memory: Compaction skipped, no free memory");
        return;
    }

    blockCount = 0;
    for (int i = 0; i < count; i++)
        memory[blockCount++] = temp[i];
    if (freeTotal > 0) {
        memory[blockCount].size = freeTotal;
        memory[blockCount].isFree = true;
        memory[blockCount].processID = -1;
        blockCount++;
    }

    printf("Successfully merged %d KB into one free block.\n", freeTotal);
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Memory: Compacted %d KB into one free block", freeTotal);
    log_action(logMsg);
}

void printMemoryMap() {
    if (blockCount == 0) {
        printf("Memory not initialized. Please initialize memory first.\n");
        log_action("Memory ERROR: Display attempted before initialization");
        return;
    }

    printf("\n====Memory Map====\n");
    for (int i = 0; i < blockCount; i++) {
        if (memory[i].isFree)
            printf("  [FREE] %d KB\n", memory[i].size);
        else
            printf("  [PID %d] %d KB\n", memory[i].processID, memory[i].size);
    }
    printf("==================\n");
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Memory: Printed Memory Map (%d blocks)", blockCount);
    log_action(logMsg);
}



int main() {
    int choice;
    do {
        printf("\n=== Contiguous Memory Allocation + Compaction ===\n");
        printf("1. Initialize Memory\n2. Allocate Process\n3. Deallocate Process\n4. Display Memory Map\n5. Compact Memory\n0. Exit\nChoice: ");
        scanf("%d", &choice);

        if      (choice == 1) initializeMemory();
        else if (choice == 2) allocateMemory();
        else if (choice == 3) deallocateMemory();
        else if (choice == 4) printMemoryMap();
        else if (choice == 5) compactMemory();
        else if (choice == 0) printf("Exiting menu.\n");
        else printf("Invalid choice.\n");
    } while (choice != 0);

    return 0;
}
