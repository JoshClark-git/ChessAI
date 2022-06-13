#ifndef MOVE_H
#define MOVE_H

#include <array>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include "Board.h"
#include "Evaluate.h"
#include "Globals.h"
using namespace std;

bool isPinned(int rank, int file, long long int pinners);

bool enPaisantLegal(int rank, int file, int captureFile,int** board, int kingRank, int kingFile);

//General possible moves

void pawnMoves(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves);
void knightMoves(int rank, int file, int** board, int &accum, MoveList possMoves);
void bishopMoves(int rank, int file, int** board, int &accum, MoveList possMoves);
void rookMoves(int rank, int file, int** board, int &accum, MoveList possMoves);
void queenMoves(int rank, int file, int** board, int &accum, MoveList possMoves);

//Capture possible moves

void pawnMovesCapture(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves);
void knightMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves);
void bishopMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves);
void rookMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves);
void queenMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves);
void kingMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves, long long int kingDangerSquares);

//Allowed moves if piece is pinned

void pawnMovesPinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);
void bishopMovesPinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);
void rookMovesPinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);
void queenMovesPinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);

//CapturePinnedPiece

void pawnMovesCapturePinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);
void bishopMovesCapturePinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);
void rookMovesCapturePinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);
void queenMovesCapturePinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);

//In check moves

void pawnMovesCheck(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void knightMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void bishopMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void rookMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void queenMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void kingMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int kingDangerSquares);

//Capture in check moves

void pawnMovesCaptureCheck(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void knightMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void bishopMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void rookMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void queenMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece);
void kingMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int kingDangerSquares);

// Check if current piece (rank,file) is pinned

bool actualPinnerUpRight(int rank, int file, int** board,long long int target);
bool actualPinnerUpLeft(int rank, int file, int** board,long long int target);
bool actualPinnerDownRight(int rank, int file, int** board,long long int target);
bool actualPinnerDownLeft(int rank, int file, int** board,long long int target);
bool actualPinnerUp(int rank, int file, int** board,long long int target);
bool actualPinnerDown(int rank, int file, int** board,long long int target);
bool actualPinnerLeft(int rank, int file, int** board,long long int target);
bool actualPinnerRight(int rank, int file, int** board,long long int target);

bool oppPawnAttacks(int rank, int file, int** board);

// Attacking Squares
void pawnMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard);
void knightMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard);
void bishopMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard, array<int,2> normalKingPos);
void rookMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard, array<int,2> normalKingPos);
void queenMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard, array<int,2> normalKingPos);
void kingMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard);


bool whiteCastleKingSide(int rank, int file, int** board, long long int kingDangerSquares);

bool whiteCastleQueenSide(int rank, int file, int** board, long long int kingDangerSquares);

bool blackCastleKingSide(int rank, int file, int** board, long long int kingDangerSquares);

bool blackCastleQueenSide(int rank, int file, int** board, long long int kingDangerSquares);

void updateCastlePropertiesStack();

void kingMoves(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int kingDangerSquares);

void pinnedPieceMove(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);

void pinnedPieceCaptureMove(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile);

void checkedPieceMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, long long int kingDangerSquares, bool slidingPiece);

void checkedPieceCaptureMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, long long int kingDangerSquares, bool slidingPiece);

void pieceMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int kingDangerSquares);

void pieceCaptureMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int kingDangerSquares);

void oppAttackingMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard, array<int,2> normalKingPos);

BitBoard oppAttackingSquares(int** board, Board myBoard);

MoveList getMoves(int ** board, Board myBoard);

MoveList getCaptureMoves(int** board, Board myBoard,BitBoard oppBitBoard);

array<pair<array<int,4>,int>,218> moveOrdering(Board myBoard,int** board);

int MoveGenerator(int depth, Board myBoard);


#endif
