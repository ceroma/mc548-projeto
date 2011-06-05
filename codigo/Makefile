CC=gcc
CFLAGS=-c -Wall

TARGET=saveWorld
OBJS=save_world.o problem.o grasp.o tabu.o

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

save_world.o:	save_world.c problem.h tabu.h
	$(CC) $(CFLAGS) $<

problem.o:	problem.c problem.h
	$(CC) $(CFLAGS) $<

grasp.o:	grasp.c grasp.h problem.h
	$(CC) $(CFLAGS) $<

tabu.o:		tabu.c tabu.h problem.h
	$(CC) $(CFLAGS) $<

clean:
	rm *.o $(TARGET)
