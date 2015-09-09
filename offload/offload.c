/*
 * See:
 * https://software.intel.com/en-us/articles/effective-use-of-the-intel-compilers-offload-features
 */
#include <stdio.h>
#include "offload.h"

void mandatory_offload()
{
    _Offload_status x;

    OFFLOAD_STATUS_INIT(x);
    #pragma offload target(mic) status(x) mandatory
    {
        if (_Offload_get_device_number() < 0) {
            printf("mandatory offload ran on CPU\n");
        } else {
            printf("mandatory offload ran on MIC\n");
        }
    }
    if (x.result == OFFLOAD_SUCCESS) {
        printf("mandatory offload was successful\n");
    } else {
        printf("mandatory offload failed\n");
    }
}

int main()
{
    mandatory_offload();
    return 0;
}
