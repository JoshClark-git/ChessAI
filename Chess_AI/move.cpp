#include <array>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include "Globals.h"
#include "move.h"
#include "Evaluate.h"
using namespace std;

bool isPinned(int rank, int file, long long int pinners){
  long long int squareVal = myPow(2,(rank*8) + file);
  if((squareVal & pinners) == squareVal){
    return true;
  }
  return false;
}

bool enPaisantLegal(int rank, int file, int captureFile,int** board, int kingRank, int kingFile){

  if (rank != kingRank){
    return true;
  }
  if(board[rank][file] == 17 && board[rank-1][captureFile] != 0){
    return true;
  }
  if(board[rank][file] == 9 && board[rank+1][captureFile] != 0){
    return true;
  }
  int currPieceColour;
  if(board[rank][file] > 16){
    currPieceColour = white;
  }
  else{
    currPieceColour = black;
  }
  board[rank][file] = 0;
  board[rank][captureFile] = 0;
  for(int i = kingFile+1; i < 8; i++){
    int currPiece = board[rank][i];
    if((currPiece % 8 == 5 || currPiece % 8 == 6) && ((currPiece ^ currPieceColour) > currPiece)){
      if(currPieceColour == white){
        board[rank][file] = 17;
        board[rank][captureFile] = 9;
      }
      else{
        board[rank][file] = 9;
        board[rank][captureFile] = 17;
      }
      return false;
    }
    else if(currPiece != 0){
      if(currPieceColour == white){
        board[rank][file] = 17;
        board[rank][captureFile] = 9;
      }
      else{
        board[rank][file] = 9;
        board[rank][captureFile] = 17;
      }
      return true;
    }
  }
  for(int i = kingFile-1; i > -1; i--){
    int currPiece = board[rank][i];
    if((currPiece % 8 == 5 || currPiece% 8 == 6) && ((currPiece ^ currPieceColour) > currPiece)){
      if(currPieceColour == white){
        board[rank][file] = 17;
        board[rank][captureFile] = 9;
      }
      else{
        board[rank][file] = 9;
        board[rank][captureFile] = 17;
      }
      return false;
    }
    else if(currPiece != 0){
      if(currPieceColour == white){
        board[rank][file] = 17;
        board[rank][captureFile] = 9;
      }
      else{
        board[rank][file] = 9;
        board[rank][captureFile] = 17;
      }
      return true;
    }
  }
  if(currPieceColour == white){
    board[rank][file] = 17;
    board[rank][captureFile] = 9;
  }
  else{
    board[rank][file] = 9;
    board[rank][captureFile] = 17;
  }
  return true;

}

//General possible moves

void pawnMoves(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves){
  blackEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.pop();
  whiteEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.push(blackEnPaisant);
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }

  if(turnColour==white){
    if(board[rank-1][file] == 0){
      possMoves.moves[accum] = {rank,file,rank-1,file};
      accum++;
      if(rank == 6 && board[rank-2][file] == 0){
        possMoves.moves[accum] = {rank,file,rank-2,file};
        accum++;
      }
    }
    //Up-right take
    if(rank != 0 && file != 7 && ((rank == 3 && (whiteEnPaisant - file) == 1))){
      if(enPaisantLegal(rank, file, file+1, board, myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank-1,file+1};
        accum++;
      }
    }
    //Up-right take
    else if(rank != 0 && file != 7 && ((board[rank-1][file+1] != 0 && (board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1]))){
      possMoves.moves[accum] = {rank,file,rank-1,file+1};
      accum++;
    }
    //Up-left take
    if(rank != 0 && file != 0 && ((rank == 3 && (file - whiteEnPaisant) == 1))){
      if(enPaisantLegal(rank, file, file-1, board, myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank-1,file-1};
        accum++;
      }
    }
    else if(rank != 0 && file != 0 && ((board[rank-1][file-1] != 0 && (board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1]))){
      possMoves.moves[accum] = {rank,file,rank-1,file-1};
      accum++;
    }
  }
  else{
    if(board[rank+1][file] == 0){
      possMoves.moves[accum] = {rank,file,rank+1,file};
      accum++;
      if(rank == 1 && board[rank+2][file] == 0){
        possMoves.moves[accum] = {rank,file,rank+2,file};
        accum++;
      }
    }
    //Down-right take
    if(rank != 7 && file != 7 && ((board[rank+1][file+1] != 0 && (board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1]))){
      possMoves.moves[accum] = {rank,file,rank+1,file+1};
      accum++;
    }
    else if(rank != 7 && file != 7 && ((rank == 4 && ((blackEnPaisant - file) == 1)))){
      if(enPaisantLegal(rank, file, file+1, board, myBoard.getBlackPieces()[5].getOccupiedSquares()[0][0],myBoard.getBlackPieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank+1,file+1};
        accum++;
      }
    }
    //Down-left take
    if(rank != 7 && file != 0 && ((rank == 4 && ((file - blackEnPaisant) == 1)))){
      if(enPaisantLegal(rank, file, file-1, board, myBoard.getBlackPieces()[5].getOccupiedSquares()[0][0],myBoard.getBlackPieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank+1,file-1};
        accum++;
      }
    }
    if(rank != 7 && file != 0 && ((board[rank+1][file-1] != 0 && (board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1]))){
      possMoves.moves[accum] = {rank,file,rank+1,file-1};
      accum++;
    }
  }
  turnColour = currTurnColour;
}
void knightMoves(int rank, int file, int** board, int &accum, MoveList possMoves){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  if(file != 7 && rank > 1 && ((board[rank-2][file+1] ^ turnColour) > board[rank-2][file+1])){
    possMoves.moves[accum] = {rank,file,rank-2,file+1};
    accum++;
    //possTurns.push_back({rank-2,file+1});
  }
  //Right-Up
  if(file < 6 && rank != 0 && ((board[rank-1][file+2] ^ turnColour) > board[rank-1][file+2])){
    possMoves.moves[accum] = {rank,file,rank-1,file+2};
    accum++;
  }
  //Right-Down
  if(file < 6 && rank != 7 && ((board[rank+1][file+2] ^ turnColour) > board[rank+1][file+2])){
    possMoves.moves[accum] = {rank,file,rank+1,file+2};
    accum++;
  }
  //Down-Right
  if(file != 7 && rank < 6 && (((int)board[rank+2][file+1] ^ turnColour) > (int)board[rank+2][file+1])){
    possMoves.moves[accum] = {rank,file,rank+2,file+1};
    accum++;
  }
  //Down-Left
  if(file != 0 && rank < 6 && (((int)board[rank+2][file-1] ^ turnColour) > (int)board[rank+2][file-1])){
    possMoves.moves[accum] = {rank,file,rank+2,file-1};
    accum++;
  }
  //Left-Down
  if(file > 1 && rank != 7 && (((int)board[rank+1][file-2] ^ turnColour) > (int)board[rank+1][file-2])){
    possMoves.moves[accum] = {rank,file,rank+1,file-2};
    accum++;
  }
  //Left-Up
  if(file > 1 && rank != 0 && (((int)board[rank-1][file-2] ^ turnColour )> (int)board[rank-1][file-2])){
    possMoves.moves[accum] = {rank,file,rank-1,file-2};
    accum++;
  }
  //Up-Left
  if(file !=0 && rank > 1 && (((int)board[rank-2][file-1] ^ turnColour) > (int)board[rank-2][file-1])){
    possMoves.moves[accum] = {rank,file,rank-2,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void bishopMoves(int rank, int file, int** board, int &accum, MoveList possMoves){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
    accum++;
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
    accum++;
    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
    accum++;
    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
    accum++;
    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
    accum++;
  }
  turnColour = currTurnColour;

}
void rookMoves(int rank, int file, int** board, int &accum, MoveList possMoves){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
    accum++;
    rank--;
  }
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
    accum++;
    rank++;
  }
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
    accum++;
    file++;
  }
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
    accum++;
    file--;
  }
  if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void queenMoves(int rank, int file, int** board, int &accum, MoveList possMoves){
  //cout << "in queen Moves" << endl;

  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;

  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
    accum++;
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
    accum++;
    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
    accum++;
    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
    accum++;
    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
    accum++;
    rank--;
  }
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
    accum++;
    rank++;
  }
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
    accum++;
    file++;
  }
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
    accum++;
    file--;
  }
  if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
    accum++;
  }
  //possMoves.size = accum;
  turnColour = currTurnColour;
  //return possMoves;
}

//Capture possible moves

void pawnMovesCapture(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves){
  blackEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.pop();
  whiteEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.push(blackEnPaisant);
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }

  if(turnColour==white){
    //Up-right take
    if(rank != 0 && file != 7 && ((rank == 3 && (whiteEnPaisant - file) == 1))){
      if(enPaisantLegal(rank, file, file+1, board, myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank-1,file+1};
        accum++;
      }
    }
    //Up-right take
    else if(rank != 0 && file != 7 && ((board[rank-1][file+1] != 0 && (board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1]))){
      possMoves.moves[accum] = {rank,file,rank-1,file+1};
      accum++;
    }
    //Up-left take
    if(rank != 0 && file != 0 && ((rank == 3 && (file - whiteEnPaisant) == 1))){
      if(enPaisantLegal(rank, file, file-1, board, myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank-1,file-1};
        accum++;
      }
    }
    else if(rank != 0 && file != 0 && ((board[rank-1][file-1] != 0 && (board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1]))){
      possMoves.moves[accum] = {rank,file,rank-1,file-1};
      accum++;
    }
  }
  else{
    //Down-right take
    if(rank != 7 && file != 7 && ((board[rank+1][file+1] != 0 && (board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1]))){
      possMoves.moves[accum] = {rank,file,rank+1,file+1};
      accum++;
    }
    else if(rank != 7 && file != 7 && ((rank == 4 && ((blackEnPaisant - file) == 1)))){
      if(enPaisantLegal(rank, file, file+1, board, myBoard.getBlackPieces()[5].getOccupiedSquares()[0][0],myBoard.getBlackPieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank+1,file+1};
        accum++;
      }
    }
    //Down-left take
    if(rank != 7 && file != 0 && ((rank == 4 && ((file - blackEnPaisant) == 1)))){
      if(enPaisantLegal(rank, file, file-1, board, myBoard.getBlackPieces()[5].getOccupiedSquares()[0][0],myBoard.getBlackPieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank+1,file-1};
        accum++;
      }
    }
    if(rank != 7 && file != 0 && ((board[rank+1][file-1] != 0 && (board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1]))){
      possMoves.moves[accum] = {rank,file,rank+1,file-1};
      accum++;
    }
  }
  turnColour = currTurnColour;
}
void knightMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  if(file != 7 && rank > 1 && (board[rank-2][file+1] != 0) && ((board[rank-2][file+1] ^ turnColour) > board[rank-2][file+1])){
    possMoves.moves[accum] = {rank,file,rank-2,file+1};
    accum++;
    //possTurns.push_back({rank-2,file+1});
  }
  //Right-Up
  if(file < 6 && rank != 0 && (board[rank-1][file+2] != 0) && ((board[rank-1][file+2] ^ turnColour) > board[rank-1][file+2])){
    possMoves.moves[accum] = {rank,file,rank-1,file+2};
    accum++;
  }
  //Right-Down
  if(file < 6 && rank != 7 && (board[rank+1][file+2] != 0) && ((board[rank+1][file+2] ^ turnColour) > board[rank+1][file+2])){
    possMoves.moves[accum] = {rank,file,rank+1,file+2};
    accum++;
  }
  //Down-Right
  if(file != 7 && rank < 6 && (board[rank+2][file+1] != 0) && (((int)board[rank+2][file+1] ^ turnColour) > (int)board[rank+2][file+1])){
    possMoves.moves[accum] = {rank,file,rank+2,file+1};
    accum++;
  }
  //Down-Left
  if(file != 0 && rank < 6 && (board[rank+2][file-1] != 0) && (((int)board[rank+2][file-1] ^ turnColour) > (int)board[rank+2][file-1])){
    possMoves.moves[accum] = {rank,file,rank+2,file-1};
    accum++;
  }
  //Left-Down
  if(file > 1 && rank != 7 && (board[rank+1][file-2] != 0) && (((int)board[rank+1][file-2] ^ turnColour) > (int)board[rank+1][file-2])){
    possMoves.moves[accum] = {rank,file,rank+1,file-2};
    accum++;
  }
  //Left-Up
  if(file > 1 && rank != 0 && (board[rank-1][file-2] != 0) && (((int)board[rank-1][file-2] ^ turnColour )> (int)board[rank-1][file-2])){
    possMoves.moves[accum] = {rank,file,rank-1,file-2};
    accum++;
  }
  //Up-Left
  if(file !=0 && rank > 1 && (board[rank-2][file-1] != 0) && (((int)board[rank-2][file-1] ^ turnColour) > (int)board[rank-2][file-1])){
    possMoves.moves[accum] = {rank,file,rank-2,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void bishopMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
    accum++;
  }
  turnColour = currTurnColour;

}
void rookMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  std::vector<array<int,2>> possTurns;
  int actualRank = rank;
  int actualFile = file;

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void queenMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves){
  rookMovesCapture(rank,file,board,accum,possMoves);
  bishopMovesCapture(rank,file,board,accum,possMoves);
}
void kingMovesCapture(int rank, int file, int** board, int &accum, MoveList possMoves, long long int kingDangerSquares){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  //Up
  if(rank != 0 && board[rank-1][file] != 0 && !(myPow(2,(rank-1) * 8 + file) & kingDangerSquares) && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possMoves.moves[accum] = {rank,file,rank-1,file};
    accum++;
  }
  //Down
  if(rank != 7 && board[rank+1][file] != 0 && !(myPow(2,(rank+1) * 8 + file) & kingDangerSquares) && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possMoves.moves[accum] = {rank,file,rank+1,file};
    accum++;
  }
  //Right

  if(file != 7 && board[rank][file+1] != 0 && !(myPow(2,(rank) * 8 + file+1) & kingDangerSquares) &&  ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possMoves.moves[accum] = {rank,file,rank,file+1};
    accum++;
  }
  //Left
  if(file != 0 && board[rank][file-1] != 0 && !(myPow(2,(rank) * 8 + file-1) & kingDangerSquares) && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possMoves.moves[accum] = {rank,file,rank,file-1};
    accum++;
  }
  //Up-Right
  if(rank != 0 && board[rank-1][file+1] != 0 && file != 7 && !(myPow(2,(rank-1) * 8 + file+1) & kingDangerSquares) && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possMoves.moves[accum] = {rank,file,rank-1,file+1};
    accum++;
  }
  //Down-Right
  if(rank != 7 && board[rank+1][file+1] != 0 && file != 7 && !(myPow(2,(rank+1) * 8 + file+1) & kingDangerSquares) && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possMoves.moves[accum] = {rank,file,rank+1,file+1};
    accum++;
  }
  //Down-Left
  if(rank != 7 && board[rank+1][file-1] != 0 && file != 0 && !(myPow(2,(rank+1) * 8 + file-1) & kingDangerSquares) && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possMoves.moves[accum] = {rank,file,rank+1,file-1};
    accum++;
  }
  //Up-Left
  if(rank != 0 && board[rank-1][file-1] != 0 && file != 0 && !(myPow(2,(rank-1) * 8 + file-1) & kingDangerSquares) && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possMoves.moves[accum] = {rank,file,rank-1,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}

//Allowed moves if piece is pinned

void pawnMovesPinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  blackEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.pop();
  whiteEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.push(blackEnPaisant);
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }

  if(turnColour==white){
    if(board[rank-1][file] == 0 && (kingFile == file)){
      possMoves.moves[accum] = {rank,file,rank-1,file};
      accum++;
      if(rank == 6 && board[rank-2][file] == 0){
        possMoves.moves[accum] = {rank,file,rank-2,file};
        accum++;
      }
    }
    //Up-right take
    if(rank != 0 && file != 7 && ((kingRank > rank) && (file > kingFile)) && ((board[rank-1][file+1] != 0 && (board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1]) || (rank == 3 && (whiteEnPaisant - file) == 1))){
      if(enPaisantLegal(rank, file, file+1, board, kingRank,kingFile)){
        possMoves.moves[accum] = {rank,file,rank-1,file+1};
        accum++;
      }
    }
    //Up-left take
    if(rank != 0 && file != 0 && ((kingRank > rank) && (file < kingFile)) && ((board[rank-1][file-1] != 0 && (board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1]) || (rank == 3 && (file - whiteEnPaisant) == 1))){
      if(enPaisantLegal(rank, file, file-1, board, kingRank,kingFile)){
        possMoves.moves[accum] = {rank,file,rank-1,file-1};
        accum++;
      }
    }
  }
  else{
    if(board[rank+1][file] == 0 &&  (kingFile == file)){
      possMoves.moves[accum] = {rank,file,rank+1,file};
      accum++;
      if(rank == 1 && board[rank+2][file] == 0){
        possMoves.moves[accum] = {rank,file,rank+2,file};
        accum++;
      }
    }
    //Down-right take
    if(rank != 7 && file != 7 && ((kingRank < rank) && (file > kingFile)) && ((board[rank+1][file+1] != 0 && (board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1]) || (rank == 4 && ((blackEnPaisant - file) == 1)))){
      if(enPaisantLegal(rank, file, file+1, board, kingRank,kingFile)){
        possMoves.moves[accum] = {rank,file,rank+1,file+1};
        accum++;
      }
    }
    //Down-left take
    if(rank != 7 && file != 0 && ((kingRank < rank) && (file < kingFile)) && ((board[rank+1][file-1] != 0 && (board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1]) || (rank == 4 && ((file - blackEnPaisant) == 1)))){
      if(enPaisantLegal(rank, file, file-1, board, kingRank,kingFile)){
        possMoves.moves[accum] = {rank,file,rank+1,file-1};
        accum++;
      }
    }
  }
  turnColour = currTurnColour;
}
void bishopMovesPinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  //Up-Right / Down-Left diagonal
  if((kingRank > actualRank && kingFile < actualFile) || (kingRank < actualRank && kingFile > actualFile)){
    while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
      accum++;
      rank--;
      file++;
    }
    if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
      accum++;
    }
    file = actualFile;
    rank = actualRank;
    while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
      accum++;
      rank++;
      file--;
    }
    if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
      accum++;
    }
  }
  file = actualFile;
  rank = actualRank;
  //Down-right - Up-Left
  if((kingRank < actualRank && kingFile < actualFile) || (kingRank > actualRank && kingFile > actualFile)){
    while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
      accum++;
      rank++;
      file++;
    }
    if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
      accum++;
    }
    file = actualFile;
    rank = actualRank;
    while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
      accum++;
      rank--;
      file--;
    }
    if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
      accum++;
    }
  }
  turnColour = currTurnColour;

}
void rookMovesPinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;

  //Up
  if(kingFile == file){
    while(rank != 0 && board[rank-1][file] == 0){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
      accum++;
      rank--;
    }
    if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
      accum++;
    }
    file = actualFile;
    rank = actualRank;
    while(rank != 7 && board[rank+1][file] == 0){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
      accum++;
      rank++;
    }
    if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
      accum++;
    }
  }
  file = actualFile;
  rank = actualRank;
  //Right / Left
  if(kingRank == rank){
    while(file != 7 && board[rank][file+1] == 0){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
      accum++;
      file++;
    }
    if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
      accum++;
    }
    file = actualFile;
    rank = actualRank;
    while(file != 0 && board[rank][file-1] == 0){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
      accum++;
      file--;
    }
    if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
      accum++;
    }
  }
  turnColour = currTurnColour;
}
void queenMovesPinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  bishopMovesPinned(rank,file, board, accum, possMoves, kingRank,kingFile);
  rookMovesPinned(rank,file, board, accum, possMoves, kingRank,kingFile);
}

//CapturePinnedPiece

void pawnMovesCapturePinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  blackEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.pop();
  whiteEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.push(blackEnPaisant);
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }

  if(turnColour==white){
    //Up-right take
    if(rank != 0 && file != 7 && ((kingRank > rank) && (file > kingFile)) && ((board[rank-1][file+1] != 0 && (board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1]) || (rank == 3 && (whiteEnPaisant - file) == 1))){
      if(enPaisantLegal(rank, file, file+1, board, kingRank,kingFile)){
        possMoves.moves[accum] = {rank,file,rank-1,file+1};
        accum++;
      }
    }
    //Up-left take
    if(rank != 0 && file != 0 && ((kingRank > rank) && (file < kingFile)) && ((board[rank-1][file-1] != 0 && (board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1]) || (rank == 3 && (file - whiteEnPaisant) == 1))){
      if(enPaisantLegal(rank, file, file-1, board, kingRank,kingFile)){
        possMoves.moves[accum] = {rank,file,rank-1,file-1};
        accum++;
      }
    }
  }
  else{
    //Down-right take
    if(rank != 7 && file != 7 && ((kingRank < rank) && (file > kingFile)) && ((board[rank+1][file+1] != 0 && (board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1]) || (rank == 4 && ((blackEnPaisant - file) == 1)))){
      if(enPaisantLegal(rank, file, file+1, board, kingRank,kingFile)){
        possMoves.moves[accum] = {rank,file,rank+1,file+1};
        accum++;
      }
    }
    //Down-left take
    if(rank != 7 && file != 0 && ((kingRank < rank) && (file < kingFile)) && ((board[rank+1][file-1] != 0 && (board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1]) || (rank == 4 && ((file - blackEnPaisant) == 1)))){
      if(enPaisantLegal(rank, file, file-1, board, kingRank,kingFile)){
        possMoves.moves[accum] = {rank,file,rank+1,file-1};
        accum++;
      }
    }
  }
  turnColour = currTurnColour;
}
void bishopMovesCapturePinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  //Up-Right / Down-Left diagonal
  if((kingRank > actualRank && kingFile < actualFile) || (kingRank < actualRank && kingFile > actualFile)){
    while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
      rank--;
      file++;
    }
    if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
      accum++;
    }
    file = actualFile;
    rank = actualRank;
    while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
      rank++;
      file--;
    }
    if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
      accum++;
    }
  }
  file = actualFile;
  rank = actualRank;
  //Down-right - Up-Left
  if((kingRank < actualRank && kingFile < actualFile) || (kingRank > actualRank && kingFile > actualFile)){
    while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
      rank++;
      file++;
    }
    if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
      accum++;
    }
    file = actualFile;
    rank = actualRank;
    while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
      rank--;
      file--;
    }
    if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
      accum++;
    }
  }
  turnColour = currTurnColour;

}
void rookMovesCapturePinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;

  //Up
  if(kingFile == file){
    while(rank != 0 && board[rank-1][file] == 0){
      rank--;
    }
    if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
      accum++;
    }
    file = actualFile;
    rank = actualRank;
    while(rank != 7 && board[rank+1][file] == 0){
      rank++;
    }
    if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
      accum++;
    }
  }
  file = actualFile;
  rank = actualRank;
  //Right / Left
  if(kingRank == rank){
    while(file != 7 && board[rank][file+1] == 0){
      file++;
    }
    if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
      accum++;
    }
    file = actualFile;
    rank = actualRank;
    while(file != 0 && board[rank][file-1] == 0){
      file--;
    }
    if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
      accum++;
    }
  }
  turnColour = currTurnColour;
}
void queenMovesCapturePinned(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  bishopMovesCapturePinned(rank,file, board, accum, possMoves, kingRank,kingFile);
  rookMovesCapturePinned(rank,file, board, accum, possMoves, kingRank,kingFile);
}

//In check moves

void pawnMovesCheck(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  blackEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.pop();
  whiteEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.push(blackEnPaisant);
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  long long int squareVal;
  long long int enPaissantSquare;

  if(turnColour==white){
    if(board[rank-1][file] == 0){
      squareVal = myPow(2,(rank-1) * 8 + file);
      if(slidingPiece && ((squareVal & checkRay) == squareVal)){
        possMoves.moves[accum] = {rank,file,rank-1,file};
        accum++;
      }
      if(rank == 6 && board[rank-2][file] == 0){
        squareVal = myPow(2,(rank-2) * 8 + file);
        if(slidingPiece && ((squareVal & checkRay) == squareVal)){
          possMoves.moves[accum] = {rank,file,rank-2,file};
          accum++;
        }
      }
    }
    //Up-right take
    squareVal = myPow(2,(rank-1) * 8 + file+1);
    enPaissantSquare = myPow(2,(rank) * 8 + file+1);
    //normal Capture
    if(rank != 0 && file != 7 && ((squareVal & checkers) == checkers)){
      possMoves.moves[accum] = {rank,file,rank-1,file+1};
      accum++;
    }
    // EP capture
    else if((rank == 3 && (whiteEnPaisant - file) == 1) && ((enPaissantSquare & checkers) == checkers|| (enPaissantSquare & checkRay) == enPaissantSquare)){
      if(enPaisantLegal(rank, file, file+1, board, myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank-1,file+1};
        accum++;
      }
    }
    //Up-Left take
    squareVal = myPow(2,(rank-1) * 8 + file-1);
    enPaissantSquare = myPow(2,(rank) * 8 + file-1);
    //normal Capture
    if(rank != 0 && file != 0 && ((squareVal & checkers) == checkers)){
      possMoves.moves[accum] = {rank,file,rank-1,file-1};
      accum++;
    }
    // EP capture
    else if((rank == 3 && (file - whiteEnPaisant) == 1) && ((enPaissantSquare & checkers) == checkers|| (squareVal & checkRay) == squareVal)){
      if(enPaisantLegal(rank, file, file-1, board, myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank-1,file-1};
        accum++;
      }
    }
  }
  else{
    if(board[rank+1][file] == 0){
      squareVal = myPow(2,(rank+1) * 8 + file);
      if(slidingPiece && ((squareVal & checkRay) == squareVal)){
        possMoves.moves[accum] = {rank,file,rank+1,file};
        accum++;
      }
      if(rank == 1 && board[rank+2][file] == 0){
        squareVal = myPow(2,(rank+2) * 8 + file);
        if(slidingPiece && ((squareVal & checkRay) == squareVal)){
          possMoves.moves[accum] = {rank,file,rank+2,file};
          accum++;
        }
      }
    }
    //Up-right take
    squareVal = myPow(2,(rank+1) * 8 + file+1);
    enPaissantSquare = myPow(2,(rank) * 8 + file+1);
    //normal Capture
    if(rank != 7 && file != 7 && ((squareVal & checkers) == checkers)){
      possMoves.moves[accum] = {rank,file,rank+1,file+1};
      accum++;
    }
    // EP capture
    else if((rank == 4 && (blackEnPaisant - file) == 1) && ((enPaissantSquare & checkers) == checkers|| (squareVal & checkRay) == squareVal)){
      if(enPaisantLegal(rank, file, file+1, board, myBoard.getBlackPieces()[5].getOccupiedSquares()[0][0],myBoard.getBlackPieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank+1,file+1};
        accum++;
      }
    }
    //Up-left take
    squareVal = myPow(2,(rank+1) * 8 + file-1);
    enPaissantSquare = myPow(2,(rank) * 8 + file-1);
    //normal Capture
    if(rank != 7 && file != 0 && ((squareVal & checkers) == checkers)){
      possMoves.moves[accum] = {rank,file,rank+1,file-1};
      accum++;
    }
    // EP capture
    else if((rank == 4 && (file - blackEnPaisant) == 1) && ((enPaissantSquare & checkers) == checkers|| (squareVal & checkRay) == squareVal)){
      if(enPaisantLegal(rank, file, file-1, board, myBoard.getBlackPieces()[5].getOccupiedSquares()[0][0],myBoard.getBlackPieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank+1,file-1};
        accum++;
      }
    }
  }
  turnColour = currTurnColour;
}
void knightMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  long long int squareVal;
  squareVal = myPow(2,(rank-2) * 8 + file+1);
  if(file != 7 && rank > 1 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank-2,file+1};
    accum++;
    //possTurns.push_back({rank-2,file+1});
  }
  //Right-Up
  squareVal = myPow(2,(rank-1) * 8 + file+2);
  if(file < 6 && rank != 0 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank-1,file+2};
    accum++;
  }
  //Right-Down
  squareVal = myPow(2,(rank+1) * 8 + file+2);
  if(file < 6 && rank != 7 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank+1,file+2};
    accum++;
  }
  //Down-Right
  squareVal = myPow(2,(rank+2) * 8 + file+1);
  if(file != 7 && rank < 6 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank+2,file+1};
    accum++;
  }
  //Down-Left
  squareVal = myPow(2,(rank+2) * 8 + file-1);
  if(file != 0 && rank < 6 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank+2,file-1};
    accum++;
  }
  //Left-Down
  squareVal = myPow(2,(rank+1) * 8 + file-2);
  if(file > 1 && rank != 7 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank+1,file-2};
    accum++;
  }
  //Left-Up
  squareVal = myPow(2,(rank-1) * 8 + file-2);
  if(file > 1 && rank != 0 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank-1,file-2};
    accum++;
  }
  //Up-Left
  squareVal = myPow(2,(rank-2) * 8 + file-1);
  if(file !=0 && rank > 1 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank-2,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void bishopMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  long long int squareVal;
  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    squareVal = myPow(2,(rank-1) * 8 + file+1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
      accum++;
    }
    rank--;
    file++;
  }
  squareVal = myPow(2,(rank-1) * 8 + file+1);
  if(rank != 0 && file != 7  && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    squareVal = myPow(2,(rank+1) * 8 + file+1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
      accum++;
    }
    rank++;
    file++;
  }
  squareVal = myPow(2,(rank+1) * 8 + file+1);
  if(rank != 7 && file != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    squareVal = myPow(2,(rank+1) * 8 + file-1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
      accum++;
    }
    rank++;
    file--;
  }
  squareVal = myPow(2,(rank+1) * 8 + file-1);
  if(rank != 7 && file != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    squareVal = myPow(2,(rank-1) * 8 + file-1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
      accum++;
    }
    rank--;
    file--;
  }
  squareVal = myPow(2,(rank-1) * 8 + file-1);
  if(rank != 0 && file != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
    accum++;
  }
  turnColour = currTurnColour;

}
void rookMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  long long int squareVal;

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    squareVal = myPow(2,(rank-1) * 8 + file);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
      accum++;
    }
    rank--;
  }
  squareVal = myPow(2,(rank-1) * 8 + file);
  if(rank != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    squareVal = myPow(2,(rank+1) * 8 + file);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
      accum++;
    }
    rank++;
  }
  squareVal = myPow(2,(rank+1) * 8 + file);
  if(rank != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    squareVal = myPow(2,(rank) * 8 + file+1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
      accum++;
    }
    file++;
  }
  squareVal = myPow(2,(rank) * 8 + file+1);
  if(file != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    squareVal = myPow(2,(rank) * 8 + file-1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
      accum++;
    }
    file--;
  }
  squareVal = myPow(2,(rank) * 8 + file-1);
  if(file != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void queenMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  bishopMovesCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  rookMovesCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
}
void kingMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int kingDangerSquares){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  //Up
  if(rank != 0  && !(myPow(2,(rank-1) * 8 + file) & kingDangerSquares) && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possMoves.moves[accum] = {rank,file,rank-1,file};
    accum++;
  }
  //Down
  if(rank != 7  && !(myPow(2,(rank+1) * 8 + file) & kingDangerSquares) && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possMoves.moves[accum] = {rank,file,rank+1,file};
    accum++;
  }
  //Right

  if(file != 7  && !(myPow(2,(rank) * 8 + file+1) & kingDangerSquares) &&  ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possMoves.moves[accum] = {rank,file,rank,file+1};
    accum++;
  }
  //Left
  if(file != 0  && !(myPow(2,(rank) * 8 + file-1) & kingDangerSquares) && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possMoves.moves[accum] = {rank,file,rank,file-1};
    accum++;
  }
  //Up-Right
  if(rank != 0 && file != 7 && !(myPow(2,(rank-1) * 8 + file+1) & kingDangerSquares) && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possMoves.moves[accum] = {rank,file,rank-1,file+1};
    accum++;
  }
  //Down-Right
  if(rank != 7 && file != 7 && !(myPow(2,(rank+1) * 8 + file+1) & kingDangerSquares) && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possMoves.moves[accum] = {rank,file,rank+1,file+1};
    accum++;
  }
  //Down-Left
  if(rank != 7 && file != 0 && !(myPow(2,(rank+1) * 8 + file-1) & kingDangerSquares) && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possMoves.moves[accum] = {rank,file,rank+1,file-1};
    accum++;
  }
  //Up-Left
  if(rank != 0 && file != 0 && !(myPow(2,(rank-1) * 8 + file-1) & kingDangerSquares) && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possMoves.moves[accum] = {rank,file,rank-1,file-1};
    accum++;
  }

  turnColour = currTurnColour;
}

//Capture in check moves

void pawnMovesCaptureCheck(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  blackEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.pop();
  whiteEnPaisant = EnPaisantPropertiesStack.top();
  EnPaisantPropertiesStack.push(blackEnPaisant);
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  long long int squareVal;
  long long int enPaissantSquare;

  if(turnColour==white){
    //Up-right take
    squareVal = myPow(2,(rank-1) * 8 + file+1);
    enPaissantSquare = myPow(2,(rank) * 8 + file+1);
    //normal Capture
    if(rank != 0 && file != 7 && ((squareVal & checkers) == checkers)){
      possMoves.moves[accum] = {rank,file,rank-1,file+1};
      accum++;
    }
    // EP capture
    else if((rank == 3 && (whiteEnPaisant - file) == 1) && ((enPaissantSquare & checkers) == checkers)){
      if(enPaisantLegal(rank, file, file+1, board, myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank-1,file+1};
        accum++;
      }
    }
    //Up-Left take
    squareVal = myPow(2,(rank-1) * 8 + file-1);
    enPaissantSquare = myPow(2,(rank) * 8 + file-1);
    //normal Capture
    if(rank != 0 && file != 0 && ((squareVal & checkers) == checkers)){
      possMoves.moves[accum] = {rank,file,rank-1,file-1};
      accum++;
    }
    // EP capture
    else if((rank == 3 && (file - whiteEnPaisant) == 1) && ((enPaissantSquare & checkers) == checkers)){
      if(enPaisantLegal(rank, file, file-1, board, myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank-1,file-1};
        accum++;
      }
    }
  }
  else{
    //Up-right take
    squareVal = myPow(2,(rank+1) * 8 + file+1);
    enPaissantSquare = myPow(2,(rank) * 8 + file+1);
    //normal Capture
    if(rank != 7 && file != 7 && ((squareVal & checkers) == checkers)){
      possMoves.moves[accum] = {rank,file,rank+1,file+1};
      accum++;
    }
    // EP capture
    else if((rank == 4 && (blackEnPaisant - file) == 1) && ((enPaissantSquare & checkers) == checkers)){
      if(enPaisantLegal(rank, file, file+1, board, myBoard.getBlackPieces()[5].getOccupiedSquares()[0][0],myBoard.getBlackPieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank+1,file+1};
        accum++;
      }
    }
    //Up-left take
    squareVal = myPow(2,(rank+1) * 8 + file-1);
    enPaissantSquare = myPow(2,(rank) * 8 + file-1);
    //normal Capture
    if(rank != 7 && file != 0 && ((squareVal & checkers) == checkers)){
      possMoves.moves[accum] = {rank,file,rank+1,file-1};
      accum++;
    }
    // EP capture
    else if((rank == 4 && (file - blackEnPaisant) == 1) && ((enPaissantSquare & checkers) == checkers)){
      if(enPaisantLegal(rank, file, file-1, board, myBoard.getBlackPieces()[5].getOccupiedSquares()[0][0],myBoard.getBlackPieces()[5].getOccupiedSquares()[0][1])){
        possMoves.moves[accum] = {rank,file,rank+1,file-1};
        accum++;
      }
    }
  }
  turnColour = currTurnColour;
}
void knightMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  long long int squareVal;
  squareVal = myPow(2,(rank-2) * 8 + file+1);
  if(file != 7 && rank > 1 && (((squareVal & checkers) == checkers))){
    possMoves.moves[accum] = {rank,file,rank-2,file+1};
    accum++;
    //possTurns.push_back({rank-2,file+1});
  }
  //Right-Up
  squareVal = myPow(2,(rank-1) * 8 + file+2);
  if(file < 6 && rank != 0 && (((squareVal & checkers) == checkers))){
    possMoves.moves[accum] = {rank,file,rank-1,file+2};
    accum++;
  }
  //Right-Down
  squareVal = myPow(2,(rank+1) * 8 + file+2);
  if(file < 6 && rank != 7 && (((squareVal & checkers) == checkers))){
    possMoves.moves[accum] = {rank,file,rank+1,file+2};
    accum++;
  }
  //Down-Right
  squareVal = myPow(2,(rank+2) * 8 + file+1);
  if(file != 7 && rank < 6 && (((squareVal & checkers) == checkers))){
    possMoves.moves[accum] = {rank,file,rank+2,file+1};
    accum++;
  }
  //Down-Left
  squareVal = myPow(2,(rank+2) * 8 + file-1);
  if(file != 0 && rank < 6 && (((squareVal & checkers) == checkers))){
    possMoves.moves[accum] = {rank,file,rank+2,file-1};
    accum++;
  }
  //Left-Down
  squareVal = myPow(2,(rank+1) * 8 + file-2);
  if(file > 1 && rank != 7 && (((squareVal & checkers) == checkers))){
    possMoves.moves[accum] = {rank,file,rank+1,file-2};
    accum++;
  }
  //Left-Up
  squareVal = myPow(2,(rank-1) * 8 + file-2);
  if(file > 1 && rank != 0 && (((squareVal & checkers) == checkers))){
    possMoves.moves[accum] = {rank,file,rank-1,file-2};
    accum++;
  }
  //Up-Left
  squareVal = myPow(2,(rank-2) * 8 + file-1);
  if(file !=0 && rank > 1 && (((squareVal & checkers) == checkers))){
    possMoves.moves[accum] = {rank,file,rank-2,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void bishopMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  long long int squareVal;
  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  squareVal = myPow(2,(rank-1) * 8 + file+1);
  if(rank != 0 && file != 7  && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  squareVal = myPow(2,(rank+1) * 8 + file+1);
  if(rank != 7 && file != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  squareVal = myPow(2,(rank+1) * 8 + file-1);
  if(rank != 7 && file != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  squareVal = myPow(2,(rank-1) * 8 + file-1);
  if(rank != 0 && file != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
    accum++;
  }
  turnColour = currTurnColour;

}
void rookMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  long long int squareVal;

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  squareVal = myPow(2,(rank-1) * 8 + file);
  if(rank != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  squareVal = myPow(2,(rank+1) * 8 + file);
  if(rank != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  squareVal = myPow(2,(rank) * 8 + file+1);
  if(file != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  squareVal = myPow(2,(rank) * 8 + file-1);
  if(file != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void queenMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
  bishopMovesCaptureCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  rookMovesCaptureCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
}
void kingMovesCaptureCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int kingDangerSquares){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  //Up
  long long int squareVal = myPow(2,(rank-1) * 8 + file);
  if(rank != 0 && (squareVal & checkers) != 0 && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank-1,file};
    accum++;
  }
  //Down
  squareVal = myPow(2,(rank+1) * 8 + file);
  if(rank != 7 && (squareVal & checkers) != 0 && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank+1,file};
    accum++;
  }
  //Right
  squareVal = myPow(2,(rank) * 8 + file+1);
  if(file != 7 && (squareVal & checkers) != 0 && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank,file+1};
    accum++;
  }
  //Left
  squareVal = myPow(2,(rank) * 8 + file-1);
  if(file != 0 && (squareVal & checkers) != 0 && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank,file-1};
    accum++;
  }
  //Up-Right
  squareVal = myPow(2,(rank-1) * 8 + file+1);
  if(rank != 0 && file != 7 && (squareVal & checkers) != 0 && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank-1,file+1};
    accum++;
  }
  //Down-Right
  squareVal = myPow(2,(rank+1) * 8 + file+1);
  if(rank != 7 && file != 7 && (squareVal & checkers) != 0 && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank+1,file+1};
    accum++;
  }
  //Down-Left
  squareVal = myPow(2,(rank+1) * 8 + file-1);
  if(rank != 7 && file != 0 && (squareVal & checkers) != 0 && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank+1,file-1};
    accum++;
  }
  //Up-Left
  squareVal = myPow(2,(rank-1) * 8 + file-1);
  if(rank != 0 && file != 0 && ((squareVal & checkers) != 0) && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank-1,file-1};
    accum++;
  }

  turnColour = currTurnColour;
}

// Check if current piece (rank,file) is pinned

bool actualPinnerUpRight(int rank, int file, int** board,long long int target){
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  if(rank == 7 || file == 0){
    return false;
  }
  long long int squareVal = myPow(2,(rank+1) * 8 + file-1);
  if((squareVal & target) == squareVal){
    return true;
  }
  return false;

}
bool actualPinnerUpLeft(int rank, int file, int** board,long long int target){
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  if(rank == 7 || file == 7){
    return false;
  }
  long long int squareVal = myPow(2,(rank+1) * 8 + file+1);
  if((squareVal & target) == squareVal){
    return true;
  }
  return false;

}
bool actualPinnerDownRight(int rank, int file, int** board,long long int target){
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  if(rank == 0 || file == 0){
    return false;
  }
  long long int squareVal = myPow(2,(rank-1) * 8 + file-1);
  if((squareVal & target) == squareVal){
    return true;
  }
  return false;

}
bool actualPinnerDownLeft(int rank, int file, int** board,long long int target){
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  if(rank == 0 || file == 7){
    return false;
  }
  long long int squareVal = myPow(2,(rank-1) * 8 + file+1);
  if((squareVal & target) == squareVal){
    return true;
  }
  return false;

}
bool actualPinnerUp(int rank, int file, int** board,long long int target){
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  if(rank == 7){
    return false;
  }
  long long int squareVal = myPow(2,(rank+1) * 8 + file);
  if((squareVal & target) == squareVal){
    return true;
  }
  return false;

}
bool actualPinnerDown(int rank, int file, int** board,long long int target){
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  if(rank == 0){
    return false;
  }
  long long int squareVal = myPow(2,(rank-1) * 8 + file);
  if((squareVal & target) == squareVal){
    return true;
  }
  return false;

}
bool actualPinnerLeft(int rank, int file, int** board,long long int target){
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  if(file == 7){
    return false;
  }
  long long int squareVal = myPow(2,(rank) * 8 + file+1);
  if((squareVal & target) == squareVal){
    return true;
  }
  return false;

}
bool actualPinnerRight(int rank, int file, int** board,long long int target){
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  if(file == 0){
    return false;
  }
  long long int squareVal = myPow(2,(rank) * 8 + file-1);
  if((squareVal & target) == squareVal){
    return true;
  }
  return false;

}

bool oppPawnAttacks(int rank, int file, int** board){
  if(turnColour == white){
    if(rank > 0 && file > 0 && board[rank-1][file-1] == 9){
      return true;
    }
    if(rank > 0 && file < 7 &&board[rank-1][file+1] == 9){
      return true;
    }
  }
  else{
    if(rank < 7 && file > 0 && board[rank+1][file-1] == 17){
      return true;
    }
    if(rank < 7 && file < 7 && board[rank+1][file+1] == 17){
      return true;
    }
  }
  return false;
}

// Attacking Squares
void pawnMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }

  if(turnColour == white){
    if(file > 0){
      long long int squareVal = myPow(2,(rank-1) * 8 + file-1);
      bBoard.attackingSquares |= squareVal;
      bBoard.kingDangerSquares |= squareVal;
      if((squareVal & kingPos) == kingPos){
        squareVal = myPow(2,(rank) * 8 + file);
        bBoard.checkers |= squareVal;
        bBoard.numCheckers++;
      }
    }
    if(file < 7){
      long long int squareVal = myPow(2,(rank-1) * 8 + file+1);
      bBoard.attackingSquares |= squareVal;
      bBoard.kingDangerSquares |= squareVal;
      if((squareVal & kingPos) == kingPos){
        squareVal = myPow(2,(rank) * 8 + file);
        bBoard.checkers |= squareVal;
        bBoard.numCheckers++;
      }
    }
  }
  else{
    if(file > 0){
      long long int squareVal = myPow(2,(rank+1) * 8 + file-1);
      bBoard.attackingSquares |= squareVal;
      bBoard.kingDangerSquares |= squareVal;
      if((squareVal & kingPos) == kingPos){
        squareVal = myPow(2,(rank) * 8 + file);
        bBoard.checkers |= squareVal;
        bBoard.numCheckers++;
      }
    }
    if(file < 7){
      long long int squareVal = myPow(2,(rank+1) * 8 + file+1);
      bBoard.attackingSquares |= squareVal;
      bBoard.kingDangerSquares |= squareVal;
      if((squareVal & kingPos) == kingPos){
        squareVal = myPow(2,(rank) * 8 + file);
        bBoard.checkers |= squareVal;
        bBoard.numCheckers++;
      }
    }
  }
  turnColour = currTurnColour;
}
void knightMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  long long int initialSquare = myPow(2,(rank) * 8 + file);
  if(file != 7 && rank > 1){
    long long int squareVal = myPow(2,(rank-2) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      bBoard.checkers |= initialSquare;
      bBoard.numCheckers++;
    }
  }
  //Right-Up
  if(file < 6 && rank != 0){
    long long int squareVal = myPow(2,(rank-1) * 8 + file+2);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      bBoard.checkers |= initialSquare;
      bBoard.numCheckers++;
    }
  }
  //Right-Down
  if(file < 6 && rank != 7){
    long long int squareVal = myPow(2,(rank+1) * 8 + file+2);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      bBoard.checkers |= initialSquare;
      bBoard.numCheckers++;
    }
  }
  //Down-Right
  if(file != 7 && rank < 6){
    long long int squareVal = myPow(2,(rank+2) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      bBoard.checkers |= initialSquare;
      bBoard.numCheckers++;
    }
  }
  //Down-Left
  if(file != 0 && rank < 6){
    long long int squareVal = myPow(2,(rank+2) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      bBoard.checkers |= initialSquare;
      bBoard.numCheckers++;
    }
  }
  //Left-Down
  if(file > 1 && rank != 7){
    long long int squareVal = myPow(2,(rank+1) * 8 + file-2);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      bBoard.checkers |= initialSquare;
      bBoard.numCheckers++;
    }
  }
  //Left-Up
  if(file > 1 && rank != 0){
    long long int squareVal = myPow(2,(rank-1) * 8 + file-2);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      bBoard.checkers |= initialSquare;
      bBoard.numCheckers++;
    }
  }
  //Up-Left
  if(file !=0 && rank > 1){
    long long int squareVal = myPow(2,(rank-2) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      bBoard.checkers |= initialSquare;
      bBoard.numCheckers++;
    }
  }
  turnColour = currTurnColour;
}
void bishopMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard, array<int,2> normalKingPos){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  //Up-Right
  long long int squareVal;
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    squareVal = myPow(2,(rank-1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }
    rank--;
    file++;
  }
  if(rank != 0 && file != 7){
    squareVal = myPow(2,(rank-1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = myPow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 1 && file != 6){
        bBoard.kingDangerSquares |= myPow(2,(rank-2) * 8 + file+2);
      }
    }
    else{
      squareVal = myPow(2,(rank-1) * 8 + file+1);
      if(actualPinnerUpRight(normalKingPos[0], normalKingPos[1],board,squareVal)){
        bBoard.pinners |= squareVal;
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    squareVal = myPow(2,(rank+1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }
    rank++;
    file++;
  }
  if(rank != 7 && file != 7){
    squareVal = myPow(2,(rank+1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = myPow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 6 && file != 6){
        bBoard.kingDangerSquares |= myPow(2,(rank+2) * 8 + file+2);
      }
    }
    else{
      if(actualPinnerDownRight(normalKingPos[0], normalKingPos[1],board,squareVal)){
        bBoard.pinners |= squareVal;
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    squareVal = myPow(2,(rank+1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank++;
    file--;
  }
  if(rank != 7 && file != 0){
    squareVal = myPow(2,(rank+1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = myPow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 6 && file != 1){
        bBoard.kingDangerSquares |= myPow(2,(rank+2) * 8 + file-2);
      }
    }
    else{
      if(actualPinnerDownLeft(normalKingPos[0], normalKingPos[1],board,squareVal)){
        bBoard.pinners |= squareVal;
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    squareVal = myPow(2,(rank-1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank--;
    file--;
  }
  if(rank != 0 && file != 0){
    squareVal = myPow(2,(rank-1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = myPow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 1 && file != 1 && board[rank-1][file-1] % 8 == 2){
        bBoard.kingDangerSquares |= myPow(2,(rank-2) * 8 + file-2);
      }
    }
    else{
      if(actualPinnerUpLeft(normalKingPos[0], normalKingPos[1],board,squareVal)){
        bBoard.pinners |= squareVal;
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  turnColour = currTurnColour;

}
void rookMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard, array<int,2> normalKingPos){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  int actualRank = rank;
  int actualFile = file;
  long long int squareVal;

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    squareVal = myPow(2,(rank-1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank--;
  }
  if(rank != 0){
    squareVal = myPow(2,(rank-1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = myPow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 1){
        bBoard.kingDangerSquares |= myPow(2,(rank-2) * 8 + file);
      }
    }
    else{
      squareVal = myPow(2,(rank-1) * 8 + file);
      if(actualPinnerUp(normalKingPos[0], normalKingPos[1],board,squareVal)){
        bBoard.pinners |= squareVal;
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    squareVal = myPow(2,(rank+1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank++;
  }


  if(rank != 7){
    squareVal = myPow(2,(rank+1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = myPow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 6){
        bBoard.kingDangerSquares |= myPow(2,(rank+2) * 8 + file);
      }
    }
    else{
      squareVal = myPow(2,(rank+1) * 8 + file);
      if(actualPinnerDown(normalKingPos[0], normalKingPos[1],board,squareVal)){
        bBoard.pinners |= squareVal;
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    squareVal = myPow(2,(rank) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    file++;
  }
  if(file != 7){
    squareVal = myPow(2,(rank) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = myPow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(file != 6 && board[rank][file+1] % 8 == 2){
        bBoard.kingDangerSquares |= myPow(2,(rank) * 8 + file+2);
      }
    }
    else{
      squareVal = myPow(2,(rank) * 8 + file+1);
      if(actualPinnerRight(normalKingPos[0], normalKingPos[1],board,squareVal)){
        bBoard.pinners |= squareVal;
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    squareVal = myPow(2,(rank) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    file--;
  }
  if(file != 0){
    squareVal = myPow(2,(rank) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = myPow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(file != 1 && board[rank][file-1] % 8 == 2){
        bBoard.kingDangerSquares |= myPow(2,(rank) * 8 + file-2);
      }
    }
    else{
      squareVal = myPow(2,(rank) * 8 + file-1);
      if(actualPinnerLeft(normalKingPos[0], normalKingPos[1],board,squareVal)){
        bBoard.pinners |= squareVal;
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  turnColour = currTurnColour;
}
void queenMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard, array<int,2> normalKingPos){
  rookMoves(rank,file,board,kingPos,bBoard,normalKingPos);
  bishopMoves(rank,file,board,kingPos,bBoard,normalKingPos);
}
void kingMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  long long int squareVal;
  //Up
  if(rank != 0){
    squareVal = myPow(2,(rank-1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Down
  if(rank != 7){
    squareVal = myPow(2,(rank+1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Right
  if(file != 7){
    squareVal = myPow(2,(rank) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Left
  if(file != 0){
    squareVal = myPow(2,(rank) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Up-Right
  if(rank != 0 && file != 7){
    squareVal = myPow(2,(rank-1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Down-Right
  if(rank != 7 && file != 7){
    squareVal = myPow(2,(rank+1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Down-Left
  if(rank != 7 && file != 0){
    squareVal = myPow(2,(rank+1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Up-Left
  if(rank != 0 && file != 0){
    squareVal = myPow(2,(rank-1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  turnColour = currTurnColour;
}


bool whiteCastleKingSide(int rank, int file, int** board, long long int kingDangerSquares){
  if(board[7][5] != 0 || board[7][6] != 0 || board[7][7] != 21){
    return false;
  }
  if((myPow(2,(rank) * 8 + file) & kingDangerSquares) || (myPow(2,(rank) * 8 + file+1) & kingDangerSquares) || (myPow(2,(rank) * 8 + file+2) & kingDangerSquares)){
    return false;
  }
  return true;
}

bool whiteCastleQueenSide(int rank, int file, int** board, long long int kingDangerSquares){
  if(board[7][1] != 0 || board[7][2] != 0 || board[7][3] != 0 || board[7][0] != 21){
    return false;
  }
  if((myPow(2,(rank) * 8 + file) & kingDangerSquares) || (myPow(2,(rank) * 8 + file-1) & kingDangerSquares) || (myPow(2,(rank) * 8 + file-2) & kingDangerSquares)){
    return false;
  }
  return true;
}

bool blackCastleKingSide(int rank, int file, int** board, long long int kingDangerSquares){
  if(board[0][5] != 0 || board[0][6] != 0 || board[0][7] != 13){
    return false;
  }
  if((myPow(2,(rank) * 8 + file) & kingDangerSquares) || (myPow(2,(rank) * 8 + file+1) & kingDangerSquares) || (myPow(2,(rank) * 8 + file+2) & kingDangerSquares)){
    return false;
  }
  return true;
}

bool blackCastleQueenSide(int rank, int file, int** board, long long int kingDangerSquares){
  if(board[0][1] != 0 || board[0][2] != 0 || board[0][3] != 0 || board[0][0] != 13){
    return false;
  }
  if((myPow(2,(rank) * 8 + file) & kingDangerSquares) || (myPow(2,(rank) * 8 + file-1) & kingDangerSquares) || (myPow(2,(rank) * 8 + file-2) & kingDangerSquares)){
    return false;
  }
  return true;
}

void updateCastlePropertiesStack(){
  blackQueenRookMoved = castlePropertiesStack.top();
  castlePropertiesStack.pop();
  blackKingRookMoved = castlePropertiesStack.top();
  castlePropertiesStack.pop();
  blackKingMoved = castlePropertiesStack.top();
  castlePropertiesStack.pop();
  whiteQueenRookMoved = castlePropertiesStack.top();
  castlePropertiesStack.pop();
  whiteKingRookMoved = castlePropertiesStack.top();
  castlePropertiesStack.pop();
  whiteKingMoved = castlePropertiesStack.top();

  castlePropertiesStack.push(whiteKingRookMoved);
  castlePropertiesStack.push(whiteQueenRookMoved);
  castlePropertiesStack.push(blackKingMoved);
  castlePropertiesStack.push(blackKingRookMoved);
  castlePropertiesStack.push(blackQueenRookMoved);

}

void kingMoves(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int kingDangerSquares){
  updateCastlePropertiesStack();
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }

  //Up
  if(rank != 0  && !(myPow(2,(rank-1) * 8 + file) & kingDangerSquares) && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possMoves.moves[accum] = {rank,file,rank-1,file};
    accum++;
  }
  //Down
  if(rank != 7  && !(myPow(2,(rank+1) * 8 + file) & kingDangerSquares) && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possMoves.moves[accum] = {rank,file,rank+1,file};
    accum++;
  }
  //Right

  if(file != 7  && !(myPow(2,(rank) * 8 + file+1) & kingDangerSquares) &&  ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possMoves.moves[accum] = {rank,file,rank,file+1};
    accum++;
  }
  //Left
  if(file != 0  && !(myPow(2,(rank) * 8 + file-1) & kingDangerSquares) && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possMoves.moves[accum] = {rank,file,rank,file-1};
    accum++;
  }
  //Up-Right
  if(rank != 0 && file != 7 && !(myPow(2,(rank-1) * 8 + file+1) & kingDangerSquares) && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possMoves.moves[accum] = {rank,file,rank-1,file+1};
    accum++;
  }
  //Down-Right
  if(rank != 7 && file != 7 && !(myPow(2,(rank+1) * 8 + file+1) & kingDangerSquares) && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possMoves.moves[accum] = {rank,file,rank+1,file+1};
    accum++;
  }
  //Down-Left
  if(rank != 7 && file != 0 && !(myPow(2,(rank+1) * 8 + file-1) & kingDangerSquares) && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possMoves.moves[accum] = {rank,file,rank+1,file-1};
    accum++;
  }
  //Up-Left
  if(rank != 0 && file != 0 && !(myPow(2,(rank-1) * 8 + file-1) & kingDangerSquares) && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possMoves.moves[accum] = {rank,file,rank-1,file-1};
    accum++;
  }
  //castling
  if(turnColour == white){
    //if(!whiteKingMoved && !whiteKingRookMoved && whiteCastleKingSide(board, myBoard)){
    if(!whiteKingMoved && !whiteKingRookMoved && whiteCastleKingSide(rank,file,board,kingDangerSquares)){
      if(file != 4){
        cout << "WrongCastling" << endl;
        sleep(1);
      }
      possMoves.moves[accum] = {rank,file,rank,file+2};
      accum++;
    }
    if(!whiteKingMoved && !whiteQueenRookMoved && whiteCastleQueenSide(rank,file,board,kingDangerSquares)){
    //if(!whiteKingMoved && !whiteQueenRookMoved && whiteCastleQueenSide(board, myBoard)){
      possMoves.moves[accum] = {rank,file,rank,file-2};
      accum++;
    }
  }
  else{
    //(!blackKingMoved && !blackKingRookMoved && blackCastleKingSide(board, myBoard)){
    if(!blackKingMoved && !blackKingRookMoved && blackCastleKingSide(rank,file,board,kingDangerSquares)){
      possMoves.moves[accum] = {rank,file,rank,file+2};
      accum++;
    }
    //if(!blackKingMoved && !blackQueenRookMoved && blackCastleQueenSide(board, myBoard)){
    if(!blackKingMoved && !blackQueenRookMoved && blackCastleQueenSide(rank,file,board,kingDangerSquares)){
      possMoves.moves[accum] = {rank,file,rank,file-2};
      accum++;
    }
  }
  turnColour = currTurnColour;
}

void pinnedPieceMove(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  if(board[rank][file] % 8 == 1){
    pawnMovesPinned(rank,file,board,accum,possMoves,kingRank,kingFile);
  }
  else if(board[rank][file] % 8 == 2){
  }
  else if(board[rank][file] % 8 == 3){
  }
  else if(board[rank][file] % 8 == 4){
    bishopMovesPinned(rank,file,board,accum,possMoves,kingRank,kingFile);
  }
  else if(board[rank][file] % 8 == 5){
    rookMovesPinned(rank,file,board,accum,possMoves,kingRank,kingFile);
  }
  else if(board[rank][file] % 8 == 6){
    queenMovesPinned(rank,file,board,accum,possMoves,kingRank,kingFile);
  }
  else{
    cout << "pieceMovesPinned" << endl;
    sleep(99999);
  }
}

void pinnedPieceCaptureMove(int rank, int file, int** board, int &accum, MoveList possMoves, int kingRank, int kingFile){
  if(board[rank][file] % 8 == 1){
    pawnMovesCapturePinned(rank,file,board,accum,possMoves,kingRank,kingFile);
  }
  else if(board[rank][file] % 8 == 2){
  }
  else if(board[rank][file] % 8 == 3){
  }
  else if(board[rank][file] % 8 == 4){
    bishopMovesCapturePinned(rank,file,board,accum,possMoves,kingRank,kingFile);
  }
  else if(board[rank][file] % 8 == 5){
    rookMovesCapturePinned(rank,file,board,accum,possMoves,kingRank,kingFile);
  }
  else if(board[rank][file] % 8 == 6){
    queenMovesCapturePinned(rank,file,board,accum,possMoves,kingRank,kingFile);
  }
  else{
    cout << "pieceMovesCapturePinned" << endl;
    sleep(99999);
  }
}

void checkedPieceMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, long long int kingDangerSquares, bool slidingPiece){
  if(board[rank][file] % 8 == 1){
    pawnMovesCheck(rank,file,board,myBoard,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else if(board[rank][file] % 8 == 2){
    kingMovesCheck(rank,file,board,accum,possMoves,kingDangerSquares);
  }
  else if(board[rank][file] % 8 == 3){
    knightMovesCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else if(board[rank][file] % 8 == 4){
    bishopMovesCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else if(board[rank][file] % 8 == 5){
    rookMovesCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else if(board[rank][file] % 8 == 6){
    queenMovesCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else{
    cout << "pieceMovesCheck" << endl;
    sleep(99999);
  }
}

void checkedPieceCaptureMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, long long int kingDangerSquares, bool slidingPiece){
  if(board[rank][file] % 8 == 1){
    pawnMovesCaptureCheck(rank,file,board,myBoard,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else if(board[rank][file] % 8 == 2){
    kingMovesCaptureCheck(rank,file,board,accum,possMoves,checkers,kingDangerSquares);
  }
  else if(board[rank][file] % 8 == 3){
    knightMovesCaptureCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else if(board[rank][file] % 8 == 4){
    bishopMovesCaptureCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else if(board[rank][file] % 8 == 5){
    rookMovesCaptureCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else if(board[rank][file] % 8 == 6){
    queenMovesCaptureCheck(rank,file,board,accum,possMoves,checkers,checkRay,slidingPiece);
  }
  else{
    cout << "pieceMovesCaptureCheck" << endl;
    sleep(99999);
  }
}

void pieceMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int kingDangerSquares){
  if(board[rank][file] % 8 == 1){
    pawnMoves(rank,file,board,myBoard,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 2){
    kingMoves(rank,file,board,myBoard,accum,possMoves, kingDangerSquares);
  }
  else if(board[rank][file] % 8 == 3){
    knightMoves(rank,file,board,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 4){
    bishopMoves(rank,file,board,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 5){
    rookMoves(rank,file,board,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 6){
    queenMoves(rank,file,board,accum,possMoves);
  }
  else{
    cout << "here>" << endl;
    cout << "pieceMoves" << endl;
    sleep(99999);
  }
}

void pieceCaptureMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves, long long int kingDangerSquares){
  if(board[rank][file] % 8 == 1){
    pawnMovesCapture(rank,file,board,myBoard,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 2){
    kingMovesCapture(rank,file,board,accum,possMoves, kingDangerSquares);
  }
  else if(board[rank][file] % 8 == 3){
    knightMovesCapture(rank,file,board,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 4){
    bishopMovesCapture(rank,file,board,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 5){
    rookMovesCapture(rank,file,board,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 6){
    queenMovesCapture(rank,file,board,accum,possMoves);
  }
  else{
    cout << "here>" << endl;
    cout << "pieceMoves" << endl;
    sleep(99999);
  }
}

void oppAttackingMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard, array<int,2> normalKingPos){
  if(board[rank][file] % 8 == 1){
    pawnMoves(rank,file,board,kingPos,bBoard);
  }
  else if(board[rank][file] % 8 == 2){
    kingMoves(rank,file,board,kingPos,bBoard);
  }
  else if(board[rank][file] % 8 == 3){
    knightMoves(rank,file,board,kingPos,bBoard);
  }
  else if(board[rank][file] % 8 == 4){
    bishopMoves(rank,file,board,kingPos,bBoard,normalKingPos);
  }
  else if(board[rank][file] % 8 == 5){
    rookMoves(rank,file,board,kingPos,bBoard,normalKingPos);
  }
  else if(board[rank][file] % 8 == 6){
    queenMoves(rank,file,board,kingPos,bBoard,normalKingPos);
  }
  else{
    cout << "pieceMoves" << endl;
    cout << rank << " " << file << endl;
    cout << turnColour << endl;
    sleep(99999);
  }
}

BitBoard oppAttackingSquares(int** board, Board myBoard){
  int initTurnColour = turnColour;
  BitBoard result;
  result.attackingSquares = 0;
  result.kingDangerSquares = 0;
  result.checkers = 0;
  result.checkRay = 0;
  result.numCheckers = 0;
  result.pinners = 0;
  array<PieceList,6> pieces;
  long long int kingPos;
  array<int,2> normalKingPos;
  if(turnColour == white){
    pieces = myBoard.getBlackPieces();
    normalKingPos = myBoard.getWhitePieces()[5].getOccupiedSquares()[0];
    kingPos = posToBitBoard(normalKingPos);
    turnColour = black;
  }
  else{
    pieces = myBoard.getWhitePieces();
    normalKingPos = myBoard.getBlackPieces()[5].getOccupiedSquares()[0];
    kingPos = posToBitBoard(normalKingPos);
    turnColour = white;
  }
  for(int k = 0; k < 6; k++){
    array<int,2>* pieceList = pieces[k].getOccupiedSquares();
    for(int i = 0; i < pieces[k].getSize();i++){
      //boardPrinter(board);
      //cout << endl;
      oppAttackingMoves(pieceList[i][0], pieceList[i][1], board, kingPos, result, normalKingPos);
    }
  }
  turnColour = initTurnColour;
  return result;
}

MoveList getMoves(int ** board, Board myBoard){
  //vector<array<int,2>> moves;
  array<int,4> val;
  int pieceColour;
  array<PieceList,6> pieces;
  if(turnColour == white){
    pieces = myBoard.getWhitePieces();
  }
  else{
    pieces = myBoard.getBlackPieces();
  }
  int numPieces = 0;
  for(int a = 0; a < 6;a++){
    numPieces += pieces[a].getSize();
  }
  MoveList actualResult;
  actualResult.moves = new array<int,4>[218];
  actualResult.size = 0;
  totalMem++;
  BitBoard oppBitBoard = oppAttackingSquares(board,myBoard);

  //actualPins(board,myBoard,oppBitBoard);

  //sleep(10);
  int index = 0;
  if(oppBitBoard.numCheckers == 2){
    kingMovesCheck(pieces[5].getOccupiedSquares()[0][0],pieces[5].getOccupiedSquares()[0][1],board,index,actualResult,oppBitBoard.kingDangerSquares);
  }
  else if(oppBitBoard.numCheckers == 1){
    for(int k = 0; k < 6; k++){
      array<int,2>* pieceList = pieces[k].getOccupiedSquares();
      for(int i = 0; i < pieces[k].getSize();i++){
        if(!isPinned(pieceList[i][0], pieceList[i][1],oppBitBoard.pinners)){
          checkedPieceMove(pieceList[i][0], pieceList[i][1], board, myBoard, index, actualResult,oppBitBoard.checkers,oppBitBoard.checkRay,oppBitBoard.kingDangerSquares,true);
        }
      }
    }
  }
  else{
    int kingRank = pieces[5].getOccupiedSquares()[0][0];
    int kingFile = pieces[5].getOccupiedSquares()[0][1];
    for(int k = 0; k < 6; k++){
      array<int,2>* pieceList = pieces[k].getOccupiedSquares();
      for(int i = 0; i < pieces[k].getSize();i++){
        if(isPinned(pieceList[i][0], pieceList[i][1],oppBitBoard.pinners)){
          pinnedPieceMove(pieceList[i][0], pieceList[i][1], board, index, actualResult, kingRank, kingFile);
        }
        else{
          pieceMove(pieceList[i][0], pieceList[i][1], board, myBoard, index, actualResult,oppBitBoard.kingDangerSquares);
        }
      }
    }
  }
  actualResult.size = index;
  maxMoves = max(maxMoves,index);

  return actualResult;

}

MoveList getCaptureMoves(int** board, Board myBoard,BitBoard oppBitBoard){
  //vector<array<int,2>> moves;
  array<int,4> val;
  int pieceColour;
  array<PieceList,6> pieces;
  if(turnColour == white){
    pieces = myBoard.getWhitePieces();
  }
  else{
    pieces = myBoard.getBlackPieces();
  }
  int numPieces = 0;
  for(int a = 0; a < 6;a++){
    numPieces += pieces[a].getSize();
  }
  MoveList actualResult;
  actualResult.moves = new array<int,4>[218];
  actualResult.size = 0;
  totalMem++;

  int index = 0;
  if(oppBitBoard.numCheckers == 2){
    //return actualResult;
    kingMovesCaptureCheck(pieces[5].getOccupiedSquares()[0][0], pieces[5].getOccupiedSquares()[0][1],board, index, actualResult, oppBitBoard.checkers, oppBitBoard.kingDangerSquares);
  }
  else if(oppBitBoard.numCheckers == 1){
    for(int k = 0; k < 6; k++){
      array<int,2>* pieceList = pieces[k].getOccupiedSquares();
      for(int i = 0; i < pieces[k].getSize();i++){
        if(!isPinned(pieceList[i][0], pieceList[i][1],oppBitBoard.pinners)){
          checkedPieceCaptureMove(pieceList[i][0], pieceList[i][1], board, myBoard, index, actualResult,oppBitBoard.checkers,oppBitBoard.checkRay,oppBitBoard.kingDangerSquares,true);
        }
      }
    }
  }
  else{
    int kingRank = pieces[5].getOccupiedSquares()[0][0];
    int kingFile = pieces[5].getOccupiedSquares()[0][1];
    for(int k = 0; k < 6; k++){
      array<int,2>* pieceList = pieces[k].getOccupiedSquares();
      for(int i = 0; i < pieces[k].getSize();i++){
        if(isPinned(pieceList[i][0], pieceList[i][1],oppBitBoard.pinners)){
          pinnedPieceCaptureMove(pieceList[i][0], pieceList[i][1], board, index, actualResult, kingRank, kingFile);
        }
        else{
          pieceCaptureMove(pieceList[i][0], pieceList[i][1], board, myBoard, index, actualResult,oppBitBoard.kingDangerSquares);
        }
      }
    }
  }
  actualResult.size = index;
  //maxMoves = max(maxMoves,index);
  return actualResult;
}

array<pair<array<int,4>,int>,218> moveOrdering(Board myBoard,int** board){
  bool isEndGame = endGame(myBoard);
  MoveList allMoves = getMoves(board,myBoard);
  array<int,4>* allMovesList = allMoves.moves;
  array<pair<array<int,4>,int>,218> moveResult;
  for(int i = 0; i < allMoves.size; i++){
    moveResult[i] = make_pair(allMovesList[i], predictedVal(allMovesList[i][0],allMovesList[i][1],allMovesList[i][2],allMovesList[i][3], myBoard,board,isEndGame));
  }
  array<int,4> empty = {-1,-1,-1,-1};
  for(int j = allMoves.size; j < 218; j++){
    moveResult[j] = make_pair(empty,-numeric_limits<int>::max());
  }

  sort(moveResult.begin(),moveResult.begin()+allMoves.size, ordering());

  delete allMoves.moves;
  totalMem--;


  return moveResult;
}

int MoveGenerator(int depth, Board myBoard){
  if(depth == 0){
    return 1;
  }
  int result = 0;
  int newResult = 0;
  array<int,2> piece;
  array<int,2> move;
  int index = 0;
  MoveList allMoves = getMoves(myBoard.getBoard(), myBoard);
  int pieceNum = 0;
  for(int index = 0; index < allMoves.size;index++){
    newResult = 0;
    piece = {allMoves.moves[index][0],allMoves.moves[index][1]};
    move = {allMoves.moves[index][2],allMoves.moves[index][3]};
    int captured = myBoard.make_move(piece,move);
    newResult = MoveGenerator(depth - 1,myBoard);
    result += newResult;
    myBoard.unmake_move(move,piece,captured);
  }
  delete allMoves.moves;
  totalMem--;
  return result;
}
