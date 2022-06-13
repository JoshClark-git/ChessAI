#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>
#include <unistd.h>
#include "PieceList.h"
//#include "Globals.h"

using namespace std;

class Board{
private:
  int** boardReprentation;
  array<bool,6> castleProperties;
  array<int,2> EnPaisantProperties;
public:
  array<PieceList,2> pawnPositions;
  array<PieceList,2> knightPositions;
  array<PieceList,2> bishopPositions;
  array<PieceList,2> rookPositions;
  array<PieceList,2> queenPositions;
  array<PieceList,2> kingPositions;

  void updateStacks();
  int** initPieceMap();
  void setupPieceLists();

  void deletePieceLists();


  Board(int** board,array<bool,6> castleProperties,array<int,2> EnPaisantProperties);

  void newBoard(int** board);

  void deleteBoard();

  array<int,2> getEnPaisantProperties();
  array<bool,6> getCastleProperties();
  void setEnPaisantProperties(array<int,2> properties);
  void setCastleProperties(array<bool,6> properties);
  void setBoard(int** board);
  int** getBoard();
  void restoreStacks();
  void restoreQualities();
  void boardPrinter(int** board);
  void checkRookMove(int rank, int file, int** board);
  void updateProperties(array<int,2> initPos, array<int,2> endPos);
  array<PieceList,6> getWhitePieces();
  array<PieceList,6> getBlackPieces();

  void updatePiecePos(array<int,2> initPos,array<int,2> endPos, int pieceType, int pieceColour);

  void updatePieceRemoval(array<int,2> initPos,array<int,2> endPos, int pieceType, int pieceColour);

  void restorePiecePos(int rank, int file, int pieceType, int pieceColour);

  int applyMoveForward(array<int,2> initPos,array<int,2> endPos);
  void applyMoveReverse(array<int,2> initPos,array<int,2> endPos,int pieceType,int piece);
  int make_move(array<int,2> initPos,array<int,2> endPos);
  void unmake_move(array<int,2> initPos,array<int,2> endPos, int piece);
};

#endif
