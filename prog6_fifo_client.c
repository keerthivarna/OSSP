#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define SERVER_FIFO "/tmp/server_fifo"

typedef struct {
    pid_t client_pid;
    char message[256];
} Request;

typedef struct {
    char response[256];
} Response;

int main() {
    Request request;
    Response response;

    pid_t pid = getpid();

    char client_fifo[100];

    snprintf(client_fifo,
             sizeof(client_fifo),
             "/tmp/client_%d_fifo",
             pid);

    // Create client FIFO
    if (mkfifo(client_fifo, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("Enter message: ");
    fgets(request.message, sizeof(request.message), stdin);

    request.client_pid = pid;

    // Open server FIFO
    int server_fd = open(SERVER_FIFO, O_WRONLY);

    if (server_fd == -1) {
        perror("open server FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    // Send request
    write(server_fd, &request, sizeof(Request));

    close(server_fd);

    // Open client FIFO for response
    int client_fd = open(client_fifo, O_RDONLY);

    if (client_fd == -1) {
        perror("open client FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    // Read response
    read(client_fd, &response, sizeof(Response));

    printf("Server response: %s\n", response.response);

    close(client_fd);

    // Remove client FIFO
    unlink(client_fifo);

    return 0;
}
