#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "memory.h"

#define IS_ENOUGH_MEMORY(addr, size)   ((addr + size) <= MEMORY_SIZE)
static const char* flash_memory_file = "flash_memory.bin";
static bool is_initialized = false;

// Flash memory module initialization
flash_err_t flash_memory_init() {
    // Clean up existing file if it exists
    FILE* file = fopen(flash_memory_file, "rb");
    if (file) {
        fclose(file);
        remove(flash_memory_file);
    }
    else
    {
        // if not, create a new file
        file = fopen(flash_memory_file, "wb");
        if (file) {
            fclose(file);
            is_initialized = true;
        }
        else
        {
            return FLASH_ERR_FAILED;
        }
    }
    
    return FLASH_ERR_OK;
}

// Flash memory module deinitialization
flash_err_t flash_memory_deinit() {
    remove(flash_memory_file);
    is_initialized = false;
    return FLASH_ERR_OK;
}

// Mock framework functions
flash_err_t flash_memory_write(uint16_t address, const uint8_t* data, uint16_t size) {

    if (!is_initialized) {
        printf("Error: Flash memory module is not initialized.\n");
        return FLASH_ERR_NOT_INITIALIZED;
    }

    if (!data) {
        printf("Error: Null pointer passed for data.\n");
        return FLASH_ERR_NULL;
    }

    if (!IS_ENOUGH_MEMORY(address, size))
    {
        printf("Error: Flash has not enough memory.\n");
        return FLASH_ERR_OVERFLOW;
    }
    
    FILE* file = fopen(flash_memory_file, "r+b");
    if (file) {
        fseek(file, address, SEEK_SET);
        fwrite(data, sizeof(uint8_t), size, file);
        fclose(file);
    }
    else
    {
        return FLASH_ERR_FAILED;
    }

    return FLASH_ERR_OK;
}

flash_err_t flash_memory_read(uint16_t address, uint8_t* data, uint16_t size) {
    if (!is_initialized) {
        printf("Error: Flash memory module is not initialized.\n");
        return FLASH_ERR_NOT_INITIALIZED;
    }
    
    if (!data) {
        printf("Error: Null pointer passed for data.\n");
        return FLASH_ERR_NULL;
    }

    if (!IS_ENOUGH_MEMORY(address, size))
    {
        printf("Error: Flash has not enough memory.\n");
        return FLASH_ERR_OVERFLOW;
    }
    
    FILE* file = fopen(flash_memory_file, "rb");
    if (file) {
        fseek(file, address, SEEK_SET);
        fread(data, sizeof(uint8_t), size, file);
        fclose(file);
    }
    else
    {
        return FLASH_ERR_FAILED;
    }

    return FLASH_ERR_OK;
}

flash_err_t flash_memory_erase(uint16_t page_number) {
    if (!is_initialized) {
        printf("Error: Flash memory module is not initialized.\n");
        return FLASH_ERR_NOT_INITIALIZED;
    }
    
    FILE* file = fopen(flash_memory_file, "r+b");
    if (file) {
        fseek(file, page_number * PAGE_SIZE, SEEK_SET);
        uint8_t erase_data[PAGE_SIZE] = { 0xFF }; // Data to erase the page
        fwrite(erase_data, sizeof(uint8_t), PAGE_SIZE, file);
        fclose(file);
    }
    else
    {
        return FLASH_ERR_FAILED;
    }

    return FLASH_ERR_OK;
}
