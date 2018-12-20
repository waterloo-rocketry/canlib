#include <stdio.h>
#include <stdbool.h>
#include "build_can_message.h"

int main() {
    int retval = 0;
    int number_of_tests = 0;
    int number_of_failures = 0;
    printf("CANLIB Unit Tests\n");

    //someday move this block into a macro or unit test framework or something
    number_of_tests++;
    if(!test_build_can_message()) {
        // unit tests return false if the test failed
        number_of_failures++;
        printf("test_build_can_message FAILURE\n");
        retval = 1;
    } else {
        printf("test_build_can_message PASSED\n");
    }

    printf("%i Tests, %i passed, %i failed\n",
           number_of_tests,
           number_of_tests - number_of_failures,
           number_of_failures);
    return retval;
}
