#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(void)
{
    int fd = open("test.dat", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    unsigned long long total = 0;
    unsigned long long sum = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++)
            sum += (unsigned char)buffer[i];

        total += (unsigned long long)bytes_read;
    }

    if (bytes_read == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    printf("read(): processed %llu bytes\n", total);
    printf("Checksum: %llu\n", sum);

    close(fd);
    return 0;
}
