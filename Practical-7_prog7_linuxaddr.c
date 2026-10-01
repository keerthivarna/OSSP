#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_initialized = 10;
int global_uninitialized;

void sample_function()
{
    printf("Inside sample_function()\n");
}

int main()
{
    static int static_variable = 20;
    int stack_variable = 30;
    int *heap_variable = malloc(sizeof(int));

    if (heap_variable == NULL)
    {
        perror("malloc");
        return 1;
    }

    *heap_variable = 40;

    printf("Code address              : %p\n", (void *)sample_function);
    printf("Global initialized        : %p\n", (void *)&global_initialized);
    printf("Global uninitialized      : %p\n", (void *)&global_uninitialized);
    printf("Static variable           : %p\n", (void *)&static_variable);
    printf("Heap variable             : %p\n", (void *)heap_variable);
    printf("Stack variable            : %p\n", (void *)&stack_variable);

    free(heap_variable);

    return 0;
}
