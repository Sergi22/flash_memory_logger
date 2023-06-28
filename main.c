#include "logger_simulator\flash_memory.h"
#include "logger_simulator\mem_logger.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"


// Sample usage
int main() {
    // Initialize flash memory module
    if (flash_memory_init() != FLASH_ERR_OK)
    {
        printf("Failed to init flash memory\r\n");
    }

    log_entry_t log_entry_write = { .logId = 0, .data = {0xAA, 0, 0, 0, 0, 0xFF}};
    log_entry_t log_entry_read = {0};

    uint16_t logs_to_write = 8195;

    do{

        if (!memory_logger_write(&log_entry_write))
        {
            printf("Failed to write log\r\n");
            break;
        }

        log_entry_write.logId++;

        // For visual detection, while checking binary file
        memset(&log_entry_write.data[1], (log_entry_write.logId & 0xFF), 4);
    } while (--logs_to_write);
    
    if (memory_logger_read(3, &log_entry_read))
    {
        printf("read log ID %u, data %04X\r\n", log_entry_read.logId, (uint32_t*)&log_entry_read.data[0]);
    }

    if (!memory_logger_read(8194, &log_entry_read))
    {
        printf("failed to read log id 8194\r\n");
    }
    
    // Deinitialize flash memory module
    // flash_memory_deinit();

    printf("Program executed succesfully\n");
    
    return 0;
}
