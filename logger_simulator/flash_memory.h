#ifndef _FLASH_MEMORY_H_
#define _FLASH_MEMORY_H_

#include "stdint.h"

#define BLOCK_SIZE      4
#define PAGE_SIZE       256
#define MAX_PAGES       256
#define MEMORY_SIZE     (MAX_PAGES * PAGE_SIZE)

typedef enum 
{
    FLASH_ERR_OK,
    FLASH_ERR_OVERFLOW,
    FLASH_ERR_NULL,
    FLASH_ERR_NOT_INITIALIZED,
    FLASH_ERR_FAILED,
} flash_err_t;

flash_err_t flash_memory_init(void);
flash_err_t flash_memory_deinit(void);

flash_err_t flash_memory_write(uint16_t address, const uint32_t* data, uint16_t num_blocks);
flash_err_t flash_memory_read(uint16_t address, uint32_t* data, uint16_t num_blocks);
flash_err_t flash_memory_erase(uint16_t page_number);

#endif
