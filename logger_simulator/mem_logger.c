#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "flash_memory.h"
#include "mem_logger.h"
#include "string.h"

#define PAGE_SIZE       256
#define MAX_PAGES       256
#define LOG_BLOCKS      2
#define LOG_SIZE        8
#define BLOCKS_PER_PAGE (PAGE_SIZE / BLOCK_SIZE)
#define LOGS_PER_MEMORY (MEMORY_SIZE / (BLOCK_SIZE * LOG_BLOCKS))
#define LOGS_PER_PAGE   (BLOCKS_PER_PAGE / LOG_BLOCKS)

typedef union 
{
    log_entry_t log_entry;
    uint32_t    raw[LOG_BLOCKS];
} log_t;

// Memory utilization tracking
static uint16_t current_block      = 0;
static uint16_t used_blocks_count  = 0;
const static uint16_t total_blocks = MEMORY_SIZE / BLOCK_SIZE;
const static uint8_t  log_size     = LOG_BLOCKS * BLOCK_SIZE;

static uint16_t current_log_id = 0;
static uint16_t start_log_id = 0;

// Write a log entry to flash memory
bool memory_logger_write(const log_entry_t* log_entry) {
    const log_t * log_ptr = (log_t *) log_entry;

    // printf("Writing LOG ID %02X", log_ptr->log_entry.logId);
    flash_err_t flash_result;

    // Check if there is enough memory to write the log entry
    if (used_blocks_count == total_blocks) {

        uint8_t erase_page = (current_block * BLOCK_SIZE) % PAGE_SIZE;
        // Erase the oldest log page to make room for new logs
        flash_result = flash_memory_erase(erase_page);
        if (flash_result != FLASH_ERR_OK)
        {
            printf("Failed to erase flash memory page num %u; Err %02X", erase_page, flash_result);
            return false;
        }

        // move current block tracker to the begining 
        if (current_block == total_blocks)
        {
            current_block = 0;
        }
        
        // Recalculate used blocks count
        used_blocks_count -= BLOCKS_PER_PAGE;
        start_log_id = start_log_id + LOGS_PER_PAGE;
    }

    if ((current_block * BLOCK_SIZE) <= 32 )
    {
        printf("Current adr write %u\r\n", (current_block * BLOCK_SIZE));
    }
    flash_result = flash_memory_write((current_block * BLOCK_SIZE), log_ptr->raw, LOG_BLOCKS);
    // Write the log entry to flash memory
    if (flash_result == FLASH_ERR_OK) {
        current_block += LOG_BLOCKS;
        used_blocks_count += LOG_BLOCKS;
        current_log_id = log_ptr->log_entry.logId;
        return true;
    } else {
        printf("Failed to write flash memory; Err %02X\r\n", flash_result);
    }

    return false;
}

bool memory_logger_read(uint16_t log_id, log_entry_t* log_entry) {
    log_t * log_ptr = (log_t *) log_entry;

    // Check if requested log_id exist in memory
    if ((log_id <= current_log_id) && (log_id >= start_log_id)) {
            // Write the log entry to flash memory
        if (flash_memory_read((log_id % LOGS_PER_MEMORY) * log_size, log_ptr->raw, LOG_BLOCKS) == FLASH_ERR_OK) {
            return true;
        }
    }

    return false;
}

