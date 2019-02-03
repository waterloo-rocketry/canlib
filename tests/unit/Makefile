CC=gcc

objects  =unit_tests.o
objects +=build_can_message.o
objects +=can_common.o
objects +=can_common_tests.o

VPATH +=../../

DEFINES+=-DBOARD_UNIQUE_ID=3

program_name=unit_tests

INCLUDES+=-I../../
INCLUDES+=-I.

all: $(objects)
	@$(CC) $(INCLUDES) $(DEFINES) $(objects) -o $(program_name)
	@./$(program_name)

.PHONY:clean
clean:
	-rm $(objects)

%.o: %.c
	@$(CC) $(INCLUDES) $(DEFINES) -c $< -o $@
