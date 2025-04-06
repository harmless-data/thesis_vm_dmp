CC = bpf-gcc

CFLAGS = -O2 -Wall

SRC = hello.bpf.c
OBJ = $(SRC:.c=.o)

OUTPUT = EBPFHello.o

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUTPUT)
