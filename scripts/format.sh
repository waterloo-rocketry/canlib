#!/bin/sh
set -x
clang-format -i message/*.h message/*.c util/*.h util/*.c stm32h7/*.h stm32h7/*.c pic18f26k83/*.h pic18f26k83/*.c mcp2515/*.h mcp2515/*.c dspic33epxxxgp50x/*.h dspic33epxxxgp50x/*.c
