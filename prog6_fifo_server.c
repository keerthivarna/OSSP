#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

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

    // Create server FIFO
    if (mkfifo(SERVER_FIFO, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for clients...\n");

    // Open server FIFO
    int server_fd = open(SERVER_FIFO, O_RDWR);
    if (server_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        ssize_t bytes_read = read(server_fd, &request, sizeof(Request));

        if (bytes_read > 0) {
            printf("Received from client %d: %s",
                   request.client_pid, request.message);

            snprintf(response.response,
                     sizeof(response.response),
                     "Server received your message: %s",
                     request.message);

            char client_fifo[100];

            snprintf(client_fifo,
                     sizeof(client_fifo),
                     "/tmp/client_%d_fifo",
                     request.client_pid);

            int client_fd = open(client_fifo, O_WRONLY);

            if (client_fd == -1) {
                perror("open client FIFO");
                continue;
            }

            write(client_fd, &response, sizeof(Response));

            close(client_fd);

            printf("Response sent to client %d\n",
                   request.client_pid);
        }
    }

    close(server_fd);
    unlink(SERVER_FIFO);

    return 0;
}
