TARGET := texteditor
srcfiles = $(shell find ./ -maxdepth 1 -name "*.c")
objfiles = $(srcfiles:.c=.o)
CC := gcc
CFLAGS = -Wall#-pthread
LDFLAGS = -lform -lncurses#-ldl# -lpthread

#$(info VAR="$(objfiles)")

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(objfiles)
		$(CC) $^ -o $@ $(LDFLAGS)

$(objfiles): $(srcfiles)
		$(CC) $(CFLAGS) $^ -c

clean:
		rm -rf $(TARGET) *.o
