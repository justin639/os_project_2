CC = gcc
CCFLAGS =

SRCS = OS41_2021-2_2017311133_±Ë¡ÿºÆ_P2.c

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
