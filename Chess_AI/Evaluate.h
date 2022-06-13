#ifndef EVALUATE_H
#define EVALUATE_H

#include <iostream>
#include <array>
#include <unistd.h>
#include "Globals.h"
#include "Board.h"

bool insufficientMaterial(Board myBoard);

int pieceValue(int piece);

bool whiteKingSidePawn( int rank,int file, Board myBoard);

bool blackKingSidePawn( int rank,int file, Board myBoard);

bool doubledPawn(int rank, int file, Board myBoard);

bool whitePassedPawn(int rank, int file, int** board);

bool blackPassedPawn(int rank, int file, int** board);

bool whiteProtectedPawn(int rank, int file, int** board);

bool blackProtectedPawn(int rank, int file, int** board);

bool isolatedPawn(int rank, int file, PieceList pawnList);

bool endGame(Board myBoard);

int whitePiecePositionValue(int piece, int rank,int file, Board myBoard, bool isEndGame);

int blackPiecePositionValue(int piece, int rank,int file, Board myBoard, bool isEndGame);

array<int,2> findSmallest(int rank, int file, Board myBoard, int ** board);

int see(int moveRank, int moveFile, Board myBoard, int** board);

int evalSee(int initRank,int initFile,int endRank,int endFile, Board myBoard, int** board);

int predictedVal(int initRank, int initFile,int moveRank, int moveFile, Board myBoard, int** board, bool isEndGame);

int mopUpEval(array<int,2> myKing, array<int,2> oppKing, int myEval, int oppEval, float endGameWeight);

float endGameWeight(int endGameValue);

int evaluate(Board myBoard,int** board,int alpha,int beta);


#endif
