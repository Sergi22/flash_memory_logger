
#include "stdint.h"

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

void flash_memory_init(void);
void flash_memory_deinit(void);

flash_err_t flash_memory_write(uint16_t address, const uint8_t* data, uint16_t size);
flash_err_t flash_memory_read(uint16_t address, uint8_t* data, uint16_t size);
flash_err_t flash_memory_erase(uint16_t page_number);
