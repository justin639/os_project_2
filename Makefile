CC = gcc
CCFLAGS =

SRCS = project_2.c

TARGET = os-sims

OBJS := $(patsubst %.c, %.o, $(SRCS))

all: $(TARGET)

%.o:%.c
	$(CC) $(CCFLAGS) $< -c -o $@

$(TARGET): $(OBJS)
	   $(CC) $(CCFLAGS) $^ -o $@

.PHONY=clean

clean: 
		rm -f $(OBJS) $(TARGET)
