OBJS	= TPFinal2.o LibreriaRobot.o heaps.o
SOURCE	= TPFinal2.c LibreriaRobot.c heaps.c
HEADER	= LibreriaRobot.h heaps.h
OUT	= robot
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

TPFinal2.o: TPFinal2.c
	$(CC) $(FLAGS) TPFinal2.c 

LibreriaRobot.o: LibreriaRobot.c
	$(CC) $(FLAGS) LibreriaRobot.c 

heaps.o: heaps.c
	$(CC) $(FLAGS) heaps.c 


clean:
	rm -f $(OBJS) $(OUT)