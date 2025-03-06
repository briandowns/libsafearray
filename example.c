#include <stdio.h>

#include "safe_array.h"

int
main(void)
{
    NEW_ARRAY(int)
    array_int ai;
    array_int_init(&ai, 8);
    array_int_append(&ai, 42);
    int x = array_int_last(&ai);
    printf("%d\n", x);
    array_int_free(&ai);

    return 0;
}
