
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
CMD_DIR := command
OUT_DIR := out

MAIN =	main.c
SRC =	serial.c \
		kString.c \
		kSerial.c \
		kFile.c \
		kCommand.c \
		kLogger.c
CMD =	kCommandHelp.c \
		kCommandTarget.c \
		kCommandTwi.c

SRCS := $(MAIN) \
		$(SRC) \
		$(CMD)
INCS := -I $(INC_DIR) \
		-I $(CMD_DIR)
OBJS := $(MAIN:%.c=$(OBJ_DIR)$(SP)%.o) \
		$(SRC:%.c=$(OBJ_DIR)$(SP)%.o) \
		$(CMD:%.c=$(OBJ_DIR)$(SP)%.o)

$(TARGET): $(OBJS)
	$(CC) -o $(OUT_DIR)$(SP)$(TARGET) $(OBJS) $(INCS)
	@echo build program ... done ($(TARGET).exe)

$(OBJ_DIR)$(SP)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

$(OBJ_DIR)$(SP)%.o: $(SRC_DIR)$(SP)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

$(OBJ_DIR)$(SP)%.o: $(CMD_DIR)$(SP)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)


.PHONY: clean

clean:
	@if not exist $(OBJ_DIR) ($(MD) $(OBJ_DIR))
	@if not exist $(OUT_DIR) ($(MD) $(OUT_DIR))
	$(RM) $(OBJ_DIR)$(SP)*.o
	$(RM) $(OUT_DIR)$(SP)*.exe
