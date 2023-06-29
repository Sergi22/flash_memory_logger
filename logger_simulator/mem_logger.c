#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "flash_memory.h"
#include "mem_logger.h"
#include "string.h"

#define LOG_BLOCKS      2
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

// Log ID tracking
static uint16_t current_log_id = 0;
static uint16_t start_log_id = 0;

/**
 * @brief Write a log entry to flash memory.
 *
 * @param log_entry Pointer to the log entry to be written.
 * @return True if the write operation is successful, False otherwise.
 */
bool memory_logger_write(const log_entry_t* log_entry) {
    const log_t * log_ptr = (log_t *) log_entry;

    flash_err_t flash_result;

    // Check if there is enough memory to write the log entry
    if (used_blocks_count == total_blocks) {

        // move current block tracker to the beginning 
        if (current_block == total_blocks)
        {
            current_block = 0;
        }

        uint8_t erase_page = (current_block * BLOCK_SIZE) / PAGE_SIZE;

        // Erase the oldest log page to make room for new logs
        flash_result = flash_memory_erase(erase_page);
        if (flash_result != FLASH_ERR_OK)
        {
            printf("Failed to erase flash memory page num %u; Err %02X", erase_page, flash_result);
            return false;
        }
        
        // Recalculate used blocks count
        used_blocks_count -= BLOCKS_PER_PAGE;
        start_log_id = start_log_id + LOGS_PER_PAGE;
    }

    flash_result = flash_memory_write((current_block * BLOCK_SIZE), log_ptr->raw, LOG_BLOCKS);

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

/**
 * @brief Read a log entry from flash memory based on the log ID.
 *
 * @param log_id The ID of the log entry to be read.
 * @param log_entry Pointer to store the read log entry.
 * @return True if the read operation is successful, False otherwise.
 */
bool memory_logger_read(uint16_t log_id, log_entry_t* log_entry) {
    log_t * log_ptr = (log_t *) log_entry;

    // Check if requested log_id exists in memory
    if ((log_id <= current_log_id) && (log_id >= start_log_id)) {
            
        if (flash_memory_read((log_id % LOGS_PER_MEMORY) * log_size, log_ptr->raw, LOG_BLOCKS) == FLASH_ERR_OK) {
            return true;
        }
    }

    return false;
}
