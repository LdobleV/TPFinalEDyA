OBJS	= TPFinal1.o LibreriaRobot.o LibreriaHash.o LibreriaLaberinto.o pilas.o glist.o
SOURCE	= TPFinal1.c LibreriaRobot.c LibreriaHash.c LibreriaLaberinto.c pilas.c glist.c
HEADER	= LibreriaRobot.h LibreriaHash.h LibreriaLaberinto.h pilas.h glist.h
OUT	= robot
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

TPFinal1.o: TPFinal1.c
	$(CC) $(FLAGS) TPFinal1.c 

LibreriaRobot.o: LibreriaRobot.c
	$(CC) $(FLAGS) LibreriaRobot.c 
	
LibreriaHash.o: LibreriaHash.c
	$(CC) $(FLAGS) LibreriaHash.c 

LibreriaLaberinto.o: LibreriaLaberinto.c
	$(CC) $(FLAGS) LibreriaLaberinto.c 

pilas.o: pilas.c
	$(CC) $(FLAGS) pilas.c 

glist.o: glist.c
	$(CC) $(FLAGS) glist.c 


clean:
	rm -f $(OBJS) $(OUT)