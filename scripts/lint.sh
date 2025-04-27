#!/bin/sh
set -x
clang-tidy message/*.h message/*.c util/*.h util/*.c mcp2515/*.h mcp2515/*.c
