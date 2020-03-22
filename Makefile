
ifeq ($(OS), Windows_NT)

RM := del
MD := mkdir
CP := copy
SP := \\

else

RM := rm -fr
MD := mkdir -p
CP := cp
SP := /

endif


CC = gcc
AR = gcc
CFLAGS  = -Wall -g
CFLAGS += -std=c99
CFLAGS += -O3

TARGET  := ks
OBJ_DIR := obj
SRC_DIR := src
INC_DIR := inc
OUT_DIR := out

SRC =	main.c \
		serial.c \
		kFile.c \
		kCommand.c \
		kLogger.c

INCS := -I $(INC_DIR)

OBJS := $(SRC:%.c=$(OBJ_DIR)$(SP)%.o)

$(TARGET): $(OBJS)
	$(CC) -o $(OUT_DIR)$(SP)$(TARGET) $(OBJS)
	@echo build program ... done ($(TARGET).exe)

$(OBJ_DIR)$(SP)%.o: %.c
	$(CC) $(CFLAGS) $(CFGS) -c $< -o $@


.PHONY: clean

clean:
	@if not exist $(OBJ_DIR) ($(MD) $(OBJ_DIR))
	@if not exist $(OUT_DIR) ($(MD) $(OUT_DIR))
	$(RM) $(OBJ_DIR)$(SP)*.o
	$(RM) $(OUT_DIR)$(SP)*.exe
