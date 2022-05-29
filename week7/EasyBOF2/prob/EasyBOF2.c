/*
 * NAME: EasyBOF2
 * GOAL: Pop a shell!
*/

// ubuntu 18.04, gcc 7.5.0
// gcc -o EasyBOF2 EasyBOF2.c -no-pie -fno-stack-protector -z norelro -z execstack -m32

#include "EasyBOF2.h"

void print_usage(char *arg0)
{
    fprintf(stderr, "%s [file to process]\n", arg0);
}

int main(int argc, char *argv[], char *envp[])
{
    char *filepath = argv[1];

    if (argc < 2) {
        print_usage(argv[0]);
        return -1;
    }

    if (is_file_exists(filepath) != true) {
        fprintf(stderr, "file not exists: %s\n", filepath);
        return -1;
    }

    return process_file(filepath);
}

bool is_file_exists(char *filepath)
{
    return access(filepath, R_OK) == 0;
}

int process_file(char *filepath)
{
    int fd = open(filepath, O_RDONLY);

    if (fd == -1) {
        fprintf(stderr, "failed to open file: %s\n", filepath);
        return -1;
    }

    FILE_HEAD head;

    if (read(fd, &head, sizeof(FILE_HEAD)) != sizeof(FILE_HEAD)) {
        fprintf(stderr, "failed to read header.\n");
        return -1;
    }

    if (head.mgid != 0xe4ff) {
        fprintf(stderr, "invalid magic id: 0x%04x\n", head.mgid);
        return -1;
    }

    void *buffer = alloca(head.size);

    switch ((FILE_TYPE)(head.type))
    {
    case RawText:
        if (process_file_rawtext(fd, buffer) == -1)
            fprintf(stderr, "rawtext processing failed.\n");
        break;
        
    case Strings:
        if (process_file_strings(fd, buffer) == -1)
            fprintf(stderr, "strings proessing failed.\n");
        break;

    case Unknown:
    default:
        fprintf(stderr, "unknown file type.\n");
    }

    close(fd);
    return 0;
}

int process_file_rawtext(int fd, void *buffer)
{
    int32_t size;

    if (read(fd, &size, sizeof(int32_t)) != sizeof(int32_t)) {
        fprintf(stderr, "failed to read file body.\n");
        return -1;
    }

    if (read(fd, buffer, size) != size) {
        fprintf(stderr, "failed to read file body.\n");
        return -1;
    }

    return 0;
}

int process_file_strings(int fd, void *buffer)
{
    FILE *fp = fdopen(fd, "r");
    int index = 0;
    char *strings = (char *)(buffer);

    while (true)
    {
        char *s = fgets(&strings[index], 32, fp);
        if (s == NULL)
            break;

        index += strlen(s) + 1;
    }

    return 0;
}