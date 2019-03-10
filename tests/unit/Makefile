CC=gcc

objects  =unit_tests.o
objects +=build_can_message.o
objects +=can_common.o
objects +=can_common_tests.o
objects +=can_buffering_layer.o
objects +=can_rcv_buffer.o

VPATH +=../../
VPATH +=../../util/

DEFINES+=-DBOARD_UNIQUE_ID=3
DEFINES+=-DTARGET_LOCAL

program_name=unit_tests

INCLUDES+=-I../../
INCLUDES+=-I../../util/
INCLUDES+=-I.

all: $(objects)
	@$(CC) $(INCLUDES) $(DEFINES) $(objects) -o $(program_name)
	@./$(program_name)

.PHONY:clean
clean:
	-rm $(objects)

%.o: %.c
	$(CC) $(INCLUDES) $(DEFINES) -c $< -o $@
