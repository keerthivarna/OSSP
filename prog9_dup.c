#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int fd, newfd;
    char buffer[100];

    fd = open("sample.txt", O_RDONLY);

    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    newfd = dup(fd);

    if (newfd < 0)
    {
        perror("dup");
        close(fd);
        return 1;
    }

    printf("Original file descriptor: %d\n", fd);
    printf("Duplicated file descriptor: %d\n", newfd);

    int n = read(newfd, buffer, sizeof(buffer) - 1);

    if (n < 0)
    {
        perror("read");
        close(fd);
        close(newfd);
        return 1;
    }

    buffer[n] = '\0';
    printf("File content: %s\n", buffer);

    close(fd);
    close(newfd);

    return 0;
}
