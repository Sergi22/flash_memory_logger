#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "string.h"
#include "flash_memory.h"

// Private defines
#define IS_ENOUGH_MEMORY(addr, size)   ((addr + (size * BLOCK_SIZE)) <= MEMORY_SIZE)

// Private variables
static const char* flash_memory_file = "flash_memory.bin";
static bool is_initialized = false;

/**
 * @brief Initializes the flash memory module.
 *
 * @return Flash error code indicating the success or failure of the initialization.
 */
flash_err_t flash_memory_init(void) {
    // Clean up existing file if it exists
    FILE* file = fopen(flash_memory_file, "r");
    if (file) {
        fclose(file);
        remove(flash_memory_file);
    }

    // if not, create a new file
    file = fopen(flash_memory_file, "wb");
    if (file) {
        is_initialized = true;
        fclose(file);
        printf("Flash memory initialized\r\n");
    }
    else
    {
        return FLASH_ERR_FAILED;
    }
    
    return FLASH_ERR_OK;
}

/**
 * @brief Deinitializes the flash memory module.
 *
 * @return Flash error code indicating the success or failure of the deinitialization.
 */
flash_err_t flash_memory_deinit(void) {
    remove(flash_memory_file);
    is_initialized = false;
    return FLASH_ERR_OK;
}

/**
 * @brief Writes data to the specified address in flash memory.
 *
 * @param address The address in flash memory to write the data to.
 * @param data The data to be written.
 * @param num_blocks The number of blocks of the data to be written.
 * @return Flash error code indicating the success or failure of the write operation.
 */
flash_err_t flash_memory_write(uint16_t address, const uint32_t* data, uint16_t num_blocks) {
    
    if (is_initialized == false) {
        printf("Error: Flash memory module is not initialized.\n");
        return FLASH_ERR_NOT_INITIALIZED;
    }

    if (!data) {
        printf("Error: Null pointer passed for data.\n");
        return FLASH_ERR_NULL;
    }

    if (!IS_ENOUGH_MEMORY(address, num_blocks))
    {
        printf("Error: Flash has not enough memory.\n");
        return FLASH_ERR_OVERFLOW;
    }
    
    // Calculate block number and offset within the block
    // uint16_t block_number = address / BLOCK_SIZE;
    // uint16_t block_offset = address % BLOCK_SIZE;

    FILE* file = fopen(flash_memory_file, "rb+");
    if (file) {
        fseek(file, address, SEEK_SET);
        fwrite(data, sizeof(uint32_t), num_blocks, file);
        fclose(file);
    }
    else {
        return FLASH_ERR_FAILED;
    }

    return FLASH_ERR_OK;
}

/**
 * @brief Reads data from the specified address in flash memory.
 *
 * @param address The address in flash memory to read the data from.
 * @param data The buffer to store the read data.
 * @param num_blocks The number of blocks of data to be read.
 * @return Flash error code indicating the success or failure of the read operation.
 */
flash_err_t flash_memory_read(uint16_t address, uint32_t* data, uint16_t num_blocks) {
    if (!is_initialized) {
        printf("Error: Flash memory module is not initialized.\n");
        return FLASH_ERR_NOT_INITIALIZED;
    }
    
    if (!data) {
        printf("Error: Null pointer passed for data.\n");
        return FLASH_ERR_NULL;
    }

    if (!IS_ENOUGH_MEMORY(address, num_blocks))
    {
        printf("Error: Flash has not enough memory.\n");
        return FLASH_ERR_OVERFLOW;
    }
    
    FILE* file = fopen(flash_memory_file, "rb");
    if (file) {
        fseek(file, address, SEEK_SET);
        fread(data, sizeof(uint32_t), num_blocks, file);
        fclose(file);
    }
    else
    {
        return FLASH_ERR_FAILED;
    }

    return FLASH_ERR_OK;
}

/**
 * @brief Erases a page in flash memory.
 *
 * @param page_number The page number to be erased.
 * @return Flash error code indicating the success or failure of the erase operation.
 */
flash_err_t flash_memory_erase(uint16_t page_number) {
    if (!is_initialized) {
        printf("Error: Flash memory module is not initialized.\n");
        return FLASH_ERR_NOT_INITIALIZED;
    }
    
    FILE* file = fopen(flash_memory_file, "rb+");
    if (file) {
        fseek(file, page_number * PAGE_SIZE, SEEK_SET);
        uint8_t erase_data[PAGE_SIZE] = {0xFF}; // Data to erase the page
        memset(erase_data, 0xFF, PAGE_SIZE);
        fwrite(&erase_data, sizeof(uint8_t), PAGE_SIZE, file);
        fseek(file, page_number * PAGE_SIZE, SEEK_SET);
        fclose(file);
    }
    else
    {
        return FLASH_ERR_FAILED;
    }

    return FLASH_ERR_OK;
}
