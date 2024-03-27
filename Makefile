CC = g++
CPPFLAGS = -Wall -Ofast

EXEC = tictactoe
TARGET = tictactoe.cpp

default: install

install:
	@$(CC) $(CPPFLAGS) $(TARGET) -o $(EXEC)

