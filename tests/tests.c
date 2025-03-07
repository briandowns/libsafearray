#include <strings.h>

#include "crosscheck.h"
#include "../safe_array.h"

void
cc_setup()
{}

void
cc_tear_down()
{}

cc_result_t
test_array_new(void)
{
    NEW_ARRAY(int)
    array_int ai;
    array_int_init(&ai, 8);
    CC_
    CC_SUCCESS;
}

int
main(void)
{
    CC_INIT;

    CC_RUN(test_array_new);

    CC_COMPLETE;
}
