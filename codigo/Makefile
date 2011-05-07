CC=gcc
CFLAGS=-c -Wall

TARGET=saveWorld
OBJS=save_world.o problem.o

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

save_world.o:	save_world.c problem.h
	$(CC) $(CFLAGS) $<

problem.o:	problem.c problem.h
	$(CC) $(CFLAGS) $<

clean:
	rm *.o $(TARGET)
