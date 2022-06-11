#ifndef _EASYBOF2_H_
#define _EASYBOF2_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// #include <alloca.h>

typedef enum {
    Unknown = 0,
    RawText = 1,
    Strings = 2
} FILE_TYPE;

typedef struct __attribute__((__packed__)) {
    uint32_t mgid;
    uint32_t size;
    uint32_t type;
} FILE_HEAD;

bool is_file_exists(char *filepath);
int process_file(char *filepath);
static inline int process_file_rawtext(int fd, void *buffer);
static inline int process_file_strings(int fd, void *buffer);

#endif