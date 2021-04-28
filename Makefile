
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
OBJ_DIR := build
OUT_DIR := out
SRC_DIR := source
CMD_DIR := cmd
KSL_DIR := kserial

MAIN =	main.c
SRC =	serial.c \
		kString.c \
		kFile.c \
		kLogger.c
CMD =	kCommand.c \
		kCommandHelp.c \
		kCommandTarget.c \
		kCommandTwi.c
KSL =	kSerial.c

SRCS := $(MAIN) \
		$(SRC) \
		$(CMD) \
		$(KSL)
INCS := -I $(SRC_DIR) \
		-I $(CMD_DIR) \
		-I $(KSL_DIR)
OBJS := $(MAIN:%.c=$(OBJ_DIR)$(SP)%.o) \
		$(SRC:%.c=$(OBJ_DIR)$(SP)%.o) \
		$(CMD:%.c=$(OBJ_DIR)$(SP)%.o) \
		$(KSL:%.c=$(OBJ_DIR)$(SP)%.o)

$(TARGET): $(OBJS)
	$(CC) -o $(OUT_DIR)$(SP)$(TARGET) $(OBJS) $(INCS)
	@echo build program ... done ($(TARGET).exe)

$(OBJ_DIR)$(SP)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

$(OBJ_DIR)$(SP)%.o: $(SRC_DIR)$(SP)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

$(OBJ_DIR)$(SP)%.o: $(CMD_DIR)$(SP)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

$(OBJ_DIR)$(SP)%.o: $(KSL_DIR)$(SP)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)


.PHONY: clean

clean:
	@if not exist $(OBJ_DIR) ($(MD) $(OBJ_DIR))
	@if not exist $(OUT_DIR) ($(MD) $(OUT_DIR))
	$(RM) $(OBJ_DIR)$(SP)*.o
	$(RM) $(OUT_DIR)$(SP)*.exe
