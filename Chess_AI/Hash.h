#ifndef HASH_H // This needs to be unique in each header
#define HASH_H

#include <iostream>
#include <array>
#include <unistd.h>
#include "Board.h"

unsigned long long int randomInt();

void initTable();

int getPieceHash(int piece);

int mateScore(int value, int ply);

int getPlayerHash(int piece);

int getCastleHash(Board myBoard);

int getEnPaisantHash(int piece, Board myBoard);

// Computes the hash value of a given board
unsigned long long int computeHash(int** board, Board myBoard);

void updateHashTable(unsigned long long int hash, int depth,int plySearched,int storedType, int value, array<int,4> move);

unsigned long long int makeMoveHash(array<int,2> initPos, array<int,2> endPos, int currPiece, int oppPiece, unsigned long long int currHash,Board myBoard,int plySearched);

unsigned long long int unmakeMoveHash(array<int,2> initPos, array<int,2> endPos, int currPiece, int oppPiece, unsigned long long int currHash,Board myBoard,int plySearched);
/*
unsigned long long int computeOpeningHash(int** board, Board myBoard);
*/
#endif
