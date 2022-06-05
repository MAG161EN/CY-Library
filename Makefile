CC = gcc
EXEC = CY-Librairy
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

WINDOWS = false

ifeq ($(WINDOWS), true) # Mingw only
	CLEAN = del /S *.o
	MRPROPER = del $(EXEC)
else 
	CLEAN = rm -f $(OBJ)
	MRPROPER = rm -rf $(EXEC)
endif

all : $(EXEC)

%.o : %.c
	$(CC) -o $@ -c $<

$(EXEC) : $(OBJ)
	$(CC) -o $@ $^

clean :
	$(CLEAN)

mrproper : clean
	$(MRPROPER)
