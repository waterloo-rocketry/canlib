#!/bin/sh
set -x
${CLANG_TIDY:-clang-tidy} *.h message/*.h message/*.c util/*.h util/*.c mcp2515/*.h mcp2515/*.c dspic33e/*.h pic18f26k83/*.h --warnings-as-errors="*,-clang-diagnostic-c23-extensions" --checks="clang-*,misc-*" --extra-arg-before="-std=c99" --extra-arg-before="-pedantic" --extra-arg-before="-I." --extra-arg-before="-DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_ARMING" --extra-arg-before="-DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_ROCKET"
