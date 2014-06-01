RM=/bin/rm
CC=/usr/bin/gcc
OUTPUT_PATH=./
CFLAGS=-g -Wall -O3
MAKE=/usr/bin/make

all: position_logger

position_logger:
        ${CC} ${CFLAGS} -o ${OUTPUT_PATH}/position_logger position_logger.c -lgps -lm -lpaho-mqtt3c -lpthread