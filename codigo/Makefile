CC=gcc
CFLAGS=-c -Wall

TARGET=saveWorld
OBJS=save_world.o

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

save_world.o:	save_world.c
	$(CC) $(CFLAGS) $<

clean:
	rm *.o $(TARGET)
