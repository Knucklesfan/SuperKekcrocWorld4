OBJS	= background.o font.o main.o util.o message.o
SOURCE	= source/background.cpp source/font.cpp source/main.cpp source/util.cpp source/message.cpp
HEADER	= 
OUT	= skw
CC	 = g++
FLAGS	 = $(INC) -g -c -Wall -fsanitize=address -O0 -D _LINUX
LFLAGS	 = -lSDL2 -lSDL2_ttf -lSDL2_mixer -fsanitize=address
INC=-I include/


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

background.o: source/background.cpp
	$(CC) $(FLAGS) source/background.cpp 
	
message.o: source/message.cpp
	$(CC) $(FLAGS) source/message.cpp 
font.o: source/font.cpp
	$(CC) $(FLAGS) source/font.cpp 

main.o: source/main.cpp
	$(CC) $(FLAGS) source/main.cpp 

util.o: source/util.cpp
	$(CC) $(FLAGS) source/util.cpp 


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
