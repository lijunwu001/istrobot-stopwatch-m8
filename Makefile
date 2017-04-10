CC=avr-gcc
TARGET=build
MCU=atmega8
main: main.elf
	avr-objcopy -j .text -j .data -O ihex ${TARGET}/main.elf ${TARGET}/main.hex
	rm ${TARGET}/*.elf ${TARGET}/*.o

main.elf: main.o
	${CC} -g -mmcu=${MCU} -o ${TARGET}/main.elf ${TARGET}/main.o

main.o: main.c
	${CC} -g -Os -mmcu=${MCU} -o ${TARGET}/main.o -c main.c

all: main
