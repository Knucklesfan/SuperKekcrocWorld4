OBJS	= background.o font.o main.o util.o message.o bgtextscene.o level.o player.o coin.o bluecoin.o titlescreen.o
SOURCE	= source/level.cpp source/background.cpp source/font.cpp source/main.cpp source/util.cpp source/message.cpp source/bgtextscene.cpp source/player.cpp
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

player.o: source/player.cpp
	$(CC) $(FLAGS) source/player.cpp 

message.o: source/message.cpp
	$(CC) $(FLAGS) source/message.cpp 
font.o: source/font.cpp
	$(CC) $(FLAGS) source/font.cpp 
	
bgtextscene.o: source/bgtextscene.cpp
	$(CC) $(FLAGS) source/bgtextscene.cpp 
	
main.o: source/main.cpp
	$(CC) $(FLAGS) source/main.cpp 

util.o: source/util.cpp
	$(CC) $(FLAGS) source/util.cpp 

level.o: source/level.cpp
	$(CC) $(FLAGS) source/level.cpp 

coin.o: source/coin.cpp
	$(CC) $(FLAGS) source/coin.cpp 

bluecoin.o: source/bluecoin.cpp
	$(CC) $(FLAGS) source/bluecoin.cpp 

titlescreen.o: source/titlescreen.cpp
	$(CC) $(FLAGS) source/titlescreen.cpp 

clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
