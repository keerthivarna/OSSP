#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_variable = 100;

int main()
{
    int stack_variable = 200;
    int *heap_variable = malloc(sizeof(int));

    if (heap_variable == NULL)
    {
        perror("malloc");
        return 1;
    }

    *heap_variable = 300;

    printf("PID: %d\n", getpid());
    printf("Global variable address : %p\n", (void *)&global_variable);
    printf("Stack variable address  : %p\n", (void *)&stack_variable);
    printf("Heap variable address   : %p\n", (void *)heap_variable);

    printf("Process is running...\n");
    printf("Press Ctrl+C to terminate.\n");

    while (1)
    {
        sleep(1);
    }

    free(heap_variable);
    return 0;
}
