#!/bin/sh
set -x
clang-tidy-21 message/*.h message/*.c util/*.h util/*.c mcp2515/*.h mcp2515/*.c dspic33epxxxgp50x/*.h pic18f26k83/*.h --warnings-as-errors="*,-clang-diagnostic-c23-extensions" --checks="clang-*,misc-*"
