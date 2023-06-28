#ifndef _MEM_LOGGER_H_
#define _MEM_LOGGER_H_

#include <stdbool.h>
#include "stdint.h"

typedef struct {
    unsigned short logId;
    unsigned char data[6];
} log_entry_t;

bool memory_logger_write(const log_entry_t* log_entry);
bool memory_logger_read(uint16_t log_id, log_entry_t* log_entry);
#endif
