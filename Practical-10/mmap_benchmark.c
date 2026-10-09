#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(void)
{
    int fd = open("test.dat", O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    if (st.st_size == 0) {
        printf("File is empty.\n");
        close(fd);
        return 0;
    }

    char *data = mmap(NULL, st.st_size,
                      PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    /* Read through the mapped memory without printing the file. */
    volatile unsigned long long sum = 0;
    for (off_t i = 0; i < st.st_size; i += 4096)
        sum += (unsigned char)data[i];

    printf("mmap: processed %lld bytes\n", (long long)st.st_size);
    printf("Checksum: %llu\n", sum);

    munmap(data, st.st_size);
    close(fd);
    return 0;
}
