#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int size;
    int processID;
    bool isFree;
} MemoryBlock;

MemoryBlock memory[15]; // hardcode max 15 blocks, this can be adjusted
int blockCount = 0;
int curPID = 1000;

void initializeMemory() {
    int numHoles;
    printf("Enter the number of holes to allocate: ");
    scanf("%d", &numHoles);
    int holes[numHoles];
    printf("Enter hole sizes: ");
    for (int i = 0; i < numHoles; i++)
        scanf("%d", &holes[i]);
    blockCount = numHoles;
    for (int i = 0; i < numHoles; i++) {
        memory[i].size = holes[i];
        memory[i].isFree = true;
        memory[i].processID = -1;
    }
    printf("Memory initialized with %d holes.\n", numHoles);
}

void allocateMemory() {
    int size, processID;
    printf("Process assigned PID %d\n", curPID);
    processID = curPID; // assign and inc for next alloc cycle
    curPID++;
    printf("Size (KB): ");
    scanf("%d", &size);

    for (int i = 0; i < blockCount; i++) {
        if (memory[i].isFree && memory[i].size >= size) {
            int remaining = memory[i].size - size;
            memory[i].isFree = false;
            memory[i].processID = processID;
            memory[i].size = size;

            if (remaining > 0) {
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
            return;
        }
    }

    // if fail
    printf("Unable to find block large enough for %d KB.\n", size);
}

void deallocateMemory() {
    int pid;
    printf("Enter PID to free: ");
    scanf("%d", &pid);

    for (int i = 0; i < blockCount; i++) {
        if (!memory[i].isFree && memory[i].processID == pid) {
            memory[i].isFree = true;
            memory[i].processID = -1;
            printf("Freed PID %d.\n", pid);

            // Merge adjacent free blocks
            for (int j = 0; j < blockCount - 1; j++) {
                if (memory[j].isFree && memory[j + 1].isFree) {
                    memory[j].size += memory[j + 1].size;
                    for (int k = j + 1; k < blockCount - 1; k++)
                        memory[k] = memory[k + 1];
                    blockCount--;
                    j--;
                }
            }
            return;
        }
    }

    // also if fail
    printf("PID %d not found.\n", pid);
}

void compactMemory() {
    MemoryBlock temp[15];
    int count = 0, freeTotal = 0;
    for (int i = 0; i < blockCount; i++) {
        if (!memory[i].isFree)
            temp[count++] = memory[i];
        else
            freeTotal += memory[i].size;
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
}

void printMemoryMap() {
    printf("\n====Memory Map====\n");
    for (int i = 0; i < blockCount; i++) {
        if (memory[i].isFree)
            printf("  [FREE] %d KB\n", memory[i].size);
        else
            printf("  [PID %d] %d KB\n", memory[i].processID, memory[i].size);
    }
    printf("==================\n");
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
