CC = g++
CPPFLAGS = -Wall -Ofast

default: install

install:
	@$(CC) $(CPPFLAGS) tictactoe.cpp -o tictactoe
	@$(CC) $(CPPFLAGS) MinMaxAlphaBeta.cpp -o alphabeta

