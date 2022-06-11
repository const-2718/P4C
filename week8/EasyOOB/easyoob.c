#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int my_get_int(char *message)
{
    char buf[32];
    printf(message);
    fgets(buf, sizeof(buf), stdin);
    return atoi(buf);
}

void get_shell(void)
{
    system("/bin/sh");
}

int main(void)
{
    char buf[256];
    char *ptr;
    int size, index;
    int (*get_int)(char *message) = my_get_int;

    while (true)
    {
        size = get_int("size: ");
        index = get_int("index: ");

        ptr = malloc(size);
        gets(&ptr[index]);
    }

    return 0;
}