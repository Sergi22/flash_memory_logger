#include "flash_memory.h"
#include "stdbool.h"
#include "stdint.h"


// Sample usage
int main() {
    // Initialize flash memory module
    flash_memory_init();
    
    // Simulate writing data to flash memory
    uint8_t write_data[] = { 0xAA, 0xBB, 0xCC, 0xDD };
    flash_memory_write(0, write_data, sizeof(write_data));
    
    // Simulate erasing a page
    flash_memory_erase(0);
    
    // Simulate reading data from flash memory
    uint8_t read_data[sizeof(write_data)];
    flash_memory_read(0, read_data, sizeof(read_data));
    
    // Compare read data with the original written data
    bool is_data_equal = true;
    for (uint8_t i = 0; i < sizeof(write_data); i++) {
        printf("Write vs read data: %02X vs %02X\r\n", write_data[i], read_data[i]);
        if (read_data[i] != write_data[i]) {
            is_data_equal = false;
            break;
        }
    }
    
    if (is_data_equal) {
        printf("Mock framework test passed!\n");
    } else {
        printf("Mock framework test failed!\n");
    }
    
    // Deinitialize flash memory module
    flash_memory_deinit();

    printf("Program executed succesfully\n");
    
    return 0;
}
