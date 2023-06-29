#include "logger_simulator\flash_memory.h"
#include "logger_simulator\mem_logger.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#define READ_LOG_ID_ARR_LEN    4

// Check written logs 5 should be erased, 8194 should be written on 1st page, 8225 on second and 300 is still retrivale after 2 pages erase
uint16_t read_log_ids_arr[READ_LOG_ID_ARR_LEN] = {5, 8194u, 8225u, 254u};
// Sample usage
int main() {
    // Initialize flash memory module
    if (flash_memory_init() != FLASH_ERR_OK)
    {
        printf("Failed to init flash memory\r\n");
    }

    log_entry_t log_entry_write = { .logId = 0, .data = {0xAA, 0, 0, 0, 0, 0xFF}};
    log_entry_t log_entry_read = {0};

    uint16_t logs_to_write = 8228;

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
    
    for (uint8_t idx = 0; idx < READ_LOG_ID_ARR_LEN; idx++)
    {
        if (!memory_logger_read(read_log_ids_arr[idx], &log_entry_read))
        {
            printf("failed to read log id %u\r\n", read_log_ids_arr[idx]);
        }
        else
        {
            // data with idx 1 and 2 will show LSB of log ID for testing with last read value of 255
            printf("Read log id %u data %02X %02X %02X %02X \r\n", log_entry_read.logId, log_entry_read.data[0], 
                                        log_entry_read.data[1], log_entry_read.data[2], log_entry_read.data[5]);
        }
    }


    
    // Deinitialize flash memory module
    // flash_memory_deinit();

    printf("Program executed succesfully\n");
    
    return 0;
}
