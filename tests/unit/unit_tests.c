#include <stdio.h>
#include <stdbool.h>
#include "build_can_message.h"
#include "can_common_tests.h"
#include "can_buffering_layer.h"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

/* Yes this macro is bad. Don't at me */
#define RUN_UNIT_TEST(test_name)                                     \
    number_of_tests++;                                               \
    if(!test_name()) {                                               \
        number_of_failures++;                                        \
        printf(#test_name " %sFAILURE%s\n", COLOR_RED, COLOR_RESET); \
        retval = 1;                                                  \
    } else {                                                         \
        printf(#test_name " %sPASSED%s\n", COLOR_GREEN, COLOR_RESET);\
    }


int main()
{
    int retval = 0;
    int number_of_tests = 0;
    int number_of_failures = 0;
    printf("CANLIB Unit Tests\n");

    //someday move this block into a macro or unit test framework or something
    RUN_UNIT_TEST(test_build_can_message);
    RUN_UNIT_TEST(test_can_common_functions);
    RUN_UNIT_TEST(test_debug_macro);
    RUN_UNIT_TEST(test_can_buffering_layer_rx);
    RUN_UNIT_TEST(test_can_buffering_layer_tx);

    printf("%i Tests, %i passed, %i failed\n",
           number_of_tests,
           number_of_tests - number_of_failures,
           number_of_failures);
    return retval;
}
