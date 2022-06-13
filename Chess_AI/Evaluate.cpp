#include <iostream>
#include <array>
#include <unistd.h>
#include "Globals.h"
#include "move.h"
#include "Board.h"

const int pawnValue = 100;
const int knightValue = 310;
const int bishopValue = 335;
const int rookValue = 500;
const int queenValue = 900;

const int pawnMap[64] = {800,800,800,800,800,800,800,800,
                          50, 50, 50, 50, 50, 50, 50, 50,
                          10, 10, 20, 30, 30, 20, 10, 10,
                          5,  5, 10, 25, 25, 10,  5,  5,
                          0,  0,  5, 20, 20,  5,  0,  0,
                          3, -5,  0,  0,  0,  0, -5,  3,
                          5, 10, 10,-20,-20, 10, 10,  5,
                          0,  0,  0,  0,  0,  0,  0,  0};
const int knightMap[64] = {-50,-30,-30,-30,-30,-30,-30,-50,
                           -30,-20,  0,  0,  0,  0,-20,-30,
                           -30,  0, 10, 15, 15, 10,  0,-30,
                           -30,  5, 15, 20, 20, 15,  5,-30,
                           -30,  0, 15, 20, 20, 15,  0,-30,
                           -30,  5, 10, 15, 15, 10,  5,-30,
                           -30,-20,  0,  5,  5,  0,-20,-30,
                           -50,-30,-30,-30,-30,-30,-30,-50};

const int bishopMap[64] = {-20,-10,-10,-10,-10,-10,-10,-20,
                           -10,  0,  0,  0,  0,  0,  0,-10,
                           -10,  0,  5, 10, 10,  5,  0,-10,
                           -10,  0,  5, 10, 10,  5,  0,-10,
                           -10,  0, 10, 10, 10, 10,  0,-10,
                           -10, 10, 10, 10, 10, 10, 10,-10,
                           -10,  5,  0,  0,  0,  0,  5,-10,
                           -20,-10,-10,-10,-10,-10,-10,-20,};

const int rookMap[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
                          5, 10, 10, 10, 10, 10, 10,  5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                           0,  0,  0,  5,  5,  0,  0,  0};
const int queenMap[64] = {-20,-10,-10, -5, -5,-10,-10,-20,
                          -10, -5,  0,  0,  0,  0, -5,-10,
                          -10,  0,  0,  0,  0,  0,  0,-10,
                           -5,  0,  0,  0,  0,  0,  0, -5,
                            0,  0,  0,  0,  0,  0,  0, -5,
                          -10,  0,  0,  0,  0,  0,  0,-10,
                          -10,  0,  0,  0,  0,  0,  0,-10,
                          -20,-10,  0,  0,  0,  0,-10,-20};
const int whiteKingMapMG[64] = {-30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -20,-30,-30,-40,-40,-30,-30,-20,
                                -10,-20,-20,-20,-20,-20,-20,-10,
                                 20, 20, -5, -5, -5, -5, 20, 20,
                                 20, 30, 10,-20,-20,  0, 30, 20};
const int blackKingMapMG[64] = {-30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -20,-30,-30,-40,-40,-30,-30,-20,
                                -10,-20,-20,-20,-20,-20,-20,-10,
                                 20, 20, -5, -5, -5, -5, 20, 20,
                                 20, 30,  0,-20,-20, 10, 30, 20};
const int kingMapEG[64] = {-50,-40,-30,-20,-20,-30,-40,-50,
                           -30,-20,-10,  0,  0,-10,-20,-30,
                           -30,-10, 20, 30, 30, 20,-10,-30,
                           -30,-10, 30, 40, 40, 30,-10,-30,
                           -30,-10, 30, 40, 40, 30,-10,-30,
                           -30,-10, 20, 30, 30, 20,-10,-30,
                           -30,-30,  0,  0,  0,  0,-30,-30,
                           -50,-30,-30,-30,-30,-30,-30,-50};

bool insufficientMaterial(Board myBoard){
  if(myBoard.pawnPositions[0].getSize() == 0 && myBoard.pawnPositions[1].getSize() == 0 &&
    myBoard.rookPositions[0].getSize() == 0 && myBoard.rookPositions[1].getSize() == 0 &&
    myBoard.queenPositions[0].getSize() == 0 && myBoard.queenPositions[1].getSize() == 0){
      if(((myBoard.knightPositions[0].getSize() == 0 && myBoard.bishopPositions[0].getSize() < 2) ||
      (myBoard.bishopPositions[0].getSize() == 0 && myBoard.knightPositions[0].getSize() < 2)) &&
      ((myBoard.knightPositions[1].getSize() == 0 && myBoard.bishopPositions[1].getSize() < 2) ||
      (myBoard.bishopPositions[1].getSize() == 0 && myBoard.knightPositions[1].getSize() < 2))){
        return true;
      }
  }
  return false;
}

int pieceValue(int piece){
  piece = piece % 8;
  if(piece == 1){
    return pawnValue;
  }
  if(piece == 3){
    return knightValue;
  }
  if(piece == 4){
    return bishopValue;
  }
  if(piece == 5){
    return rookValue;
  }
  if(piece == 6){
    return queenValue;
  }
  return 0;
}

bool whitePassedPawn(int rank, int file, int** board){
  if(rank == 0){
    return true;
  }
  for(int i = rank-1; i > 0;i--){
    for(int j = -1;j < 2; j++){
      if(file + j < 0 || file + j > 7){
        continue;
      }
      if(board[i][file+j] == 9){
        return false;
      }
    }
  }
  return true;
}

bool blackPassedPawn(int rank, int file, int** board){
  if(rank == 7){
    return true;
  }
  for(int i = rank+1; i < 7;i++){
    for(int j = -1;j < 2; j++){
      if(file + j < 0 || file + j > 7){
        continue;
      }
      if(board[i][file+j] == 17){
        return false;
      }
    }
  }
  return true;
}

bool whiteProtectedPawn(int rank, int file, int** board){
  if(rank == 0){
    return false;
  }
  if(file == 0){
    for(int i = rank - 1; i < rank + 2;i++){
      if(board[i][file+1] == 17){
        return true;
      }
    }
  }
  else if(file == 7){
    for(int i = rank - 1; i < rank + 2;i++){
      if(board[i][file-1] == 17){
        return true;
      }
    }
  }
  else{
    for(int i = rank - 1; i < rank + 2;i++){
      if(board[i][file+1] == 17 || board[i][file-1] == 17){
        return true;
      }
    }
  }
  return false;
  for(int i = rank; i < 7; i++){
    if(board[i][file+1] == 17){
      return true;
    }
    if(board[i][file+1] > 0 && board[i][file+1] < 16){
      break;
    }
  }
  for(int i = rank; i < 7; i++){
    if(board[i][file-1] == 17){
      return true;
    }
    if(board[i][file-1] > 0 && board[i][file-1] < 16){
      break;
    }
  }
  return false;
}

bool blackProtectedPawn(int rank, int file, int** board){
  if(rank == 7){
    return false;
  }
  //cout << "enter Black protected" << endl;
  //cout << "file: " << file << endl;
  if(file == 0){
    for(int i = rank-1;i < rank+2; i++){
      if(board[i][file+1] == 9){
        //cout << "leaving Black protected" << endl;
        return true;
      }
    }
  }
  else if(file == 7){
    for(int i = rank-1;i < rank+2; i++){
      if(board[i][file-1] == 9){
        //cout << "leaving Black protected" << endl;
        return true;
      }
    }
  }
  else{
    for(int i = rank-1;i < rank+2; i++){
      //cout << "rank: " << i << endl;
      if(board[i][file+1] == 9 || board[i][file-1] == 9){
        //cout << "leaving Black protected" << endl;
        return true;
      }
    }
  }
  //cout << "leaving Black protected" << endl;
  return false;
  for(int i = rank; i > 0; i--){
    if(board[i][file+1] == 9){
      return true;
    }
    if(board[i][file+1] > 16){
      break;
    }
  }
  for(int i = rank; i > 0; i--){
    if(board[i][file-1] == 9){
      return true;
    }
    if(board[i][file+1] > 16){
      break;
    }
  }
  return false;
}


bool isolatedPawn(int rank, int file, PieceList pawnList){
  array<int,2>* pawnPos = pawnList.getOccupiedSquares();
  for(int i = 0; i < pawnList.getSize();i++){
    if(pawnPos[i][1]-1 == file || pawnPos[i][1]+1 == file){
      return false;
    }
  }
  return true;
}

bool endGame(Board myBoard){
  int whiteValue = 0;
  int blackValue = 0;
  int** board = myBoard.getBoard();
  array<PieceList,6> whitePiecePositons = myBoard.getWhitePieces();
  array<PieceList,6> blackPiecePositons = myBoard.getBlackPieces();
  for(int i = 1; i < 5;i ++){
    array<int,2>* whitePositions = whitePiecePositons[i].getOccupiedSquares();
    for(int j = 0; j < whitePiecePositons[i].getSize();j++){
      whiteValue += pieceValue(board[whitePositions[j][0]][whitePositions[j][1]]);
    }
  }
  for(int i = 1; i < 5;i ++){
    array<int,2>* blackPositions = blackPiecePositons[i].getOccupiedSquares();
    for(int j = 0; j < blackPiecePositons[i].getSize();j++){
      blackValue += pieceValue(board[blackPositions[j][0]][blackPositions[j][1]]);
    }
  }
  if((blackValue + whiteValue) <= 3290){
    return true;
  }
  return false;
}

int whitePiecePositionValue(int piece, int rank,int file, Board myBoard, bool isEndGame){
  int** board = myBoard.getBoard();
  switch (piece){
    case 1:{
      int val = pawnMap[rank*8 + file];
      /*
      if(isEndGame){
        if(whitePassedPawn(rank,file,board) && rank < 4){
          val = 50*(8-rank);
        }
      }

      if(isolatedPawn(rank,file,myBoard.pawnPositions[1])){
        val -= 20;
      }
      */
      return val;
    }
    case 3:
      return knightMap[rank*8 + file];
    case 4:
      return bishopMap[rank*8 + file];
    case 5:
      return rookMap[rank*8 + file];
    case 6:
      return queenMap[rank*8 + file];
    case 2:
      if(isEndGame){
        return kingMapEG[rank*8 + file];
      }
      else{
        bool kingMoved = myBoard.getCastleProperties()[0];
        int val = whiteKingMapMG[rank*8 + file];
        if(kingMoved){
          if(rank < 6){
            val -= 150;
          }
            else if(myBoard.queenPositions[0].getSize()> 0){
            if(board[rank-1][file] == 0){
              val -= 25;
              if(board[rank-2][file] == 0){
                val -= 75;
              }
            }
            if(file < 6 &&board[rank-1][file+1] == 0){
              val -= 10;
            }
            if(file > 1 &&board[rank-1][file-1] == 0){
              val -= 10;
            }
          }
        }
        return val;
        //return val + whiteKingZoneValue(rank,file,myBoard);
      }
  }
  cout << piece << endl;
  cout << "white" << endl;
  cout << "nonon" << endl;
  sleep(90999);
  return -1;
}

int blackPiecePositionValue(int piece, int rank,int file, Board myBoard, bool isEndGame){
  int** board = myBoard.getBoard();
  switch (piece){
    case 1:{
      int val = pawnMap[63 - (rank*8 + file)];
      /*
      if(isEndGame){
        if(blackPassedPawn(rank,file,board)){
          val = 50*rank;
        }
      }

      if(isolatedPawn(rank,file,myBoard.pawnPositions[0])){
        val -= 20;
      }
      */
      return val;
  }
    case 3:
      return knightMap[63 - (rank*8 + file)];
    case 4:
      return bishopMap[63 - (rank*8 + file)];
    case 5:
      return rookMap[63 - (rank*8 + file)];
    case 6:
      return queenMap[63 - (rank*8 + file)];
    case 2:
      if(isEndGame){
        return kingMapEG[63 - (rank*8 + file)];
      }
      else{
        bool kingMoved = myBoard.getCastleProperties()[3];
        int val = blackKingMapMG[63 - (rank*8 + file)];
        if(kingMoved){
          if(rank > 1){
            val -= 150;
          }
          else if(myBoard.queenPositions[1].getSize()> 0){

              if(board[rank+1][file] == 0){
                val -= 25;
                if(board[rank+2][file] == 0){
                  val -= 75;
                }
              }
              if(file < 6 &&board[rank+1][file+1] == 0){
                val -= 10;
              }
              if(file > 1 &&board[rank+1][file-1] == 0){
                val -= 10;
              }
          }
      }
        return val;
      //return val + blackKingZoneValue(rank,file,myBoard);
      }
    }
  cout << "black" << endl;
  cout << piece << endl;
  sleep(90999);
  return -1;
}

int whitePawnPositionValue(int rank, int file, Board myBoard, float myEndGameWeight){
  int** board = myBoard.getBoard();
  int val = pawnMap[rank*8 + file];
  /*
  if(myEndGameWeight > 0.0){
    if(whitePassedPawn(rank,file,board) && rank < 4){
      val += (int)(50*(8-rank) * myEndGameWeight);
    }
  }

  if(isolatedPawn(rank,file,myBoard.pawnPositions[1])){
    val -= 20;
  }
  */
  return val;

}

int blackPawnPositionValue(int rank, int file, Board myBoard, float myEndGameWeight){
  int** board = myBoard.getBoard();
  int val = pawnMap[63 - (rank*8 + file)];
  /*
  if(myEndGameWeight > 0.0){
    if(blackPassedPawn(rank,file,board)){
      val += (int)(50*rank * myEndGameWeight);
    }
  }

  if(isolatedPawn(rank,file,myBoard.pawnPositions[0])){
    val -= 20;
  }
  */
  return val;
}

array<int,2> findSmallest(int rank, int file, Board myBoard, int ** board){
  long long int target = (long long int) pow(2,rank*8 + file);
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
  int index = 0;
  int prev = 0;
  if(oppBitBoard.numCheckers == 2){
    kingMovesCheck(pieces[5].getOccupiedSquares()[0][0],pieces[5].getOccupiedSquares()[0][1],board,index,actualResult,oppBitBoard.kingDangerSquares);
    for(int i = 0;i < index;i++){
      if(actualResult.moves[i][2] == rank && actualResult.moves[i][3] == file){
        delete actualResult.moves;
        totalMem--;
        return pieces[5].getOccupiedSquares()[0];
      }
    }
  }
  else if(oppBitBoard.numCheckers == 1){
    for(int k = 0; k < 6; k++){
      array<int,2>* pieceList = pieces[k].getOccupiedSquares();
      for(int i = 0; i < pieces[k].getSize();i++){
        if(!isPinned(pieceList[i][0], pieceList[i][1],oppBitBoard.pinners)){
          checkedPieceMove(pieceList[i][0], pieceList[i][1], board, myBoard, index, actualResult,oppBitBoard.checkers,oppBitBoard.checkRay,oppBitBoard.kingDangerSquares,true);
          for(int j = prev; j < index;j++){
            if(actualResult.moves[j][2] == rank && actualResult.moves[j][3] == file){
              delete actualResult.moves;
              totalMem--;
              return pieceList[i];
            }
            prev = actualResult.size;
          }
        }
      }
    }
  }
  else{
    int kingRank= pieces[5].getOccupiedSquares()[0][0];
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
        for(int j = prev; j < index;j++){
          if(actualResult.moves[j][2] == rank && actualResult.moves[j][3] == file){
            delete actualResult.moves;
            totalMem--;
            return pieceList[i];
          }
          prev = index;
        }
      }
    }
  }
  delete actualResult.moves;
  totalMem--;
  return {-1,-1};
}

int see(int moveRank, int moveFile, Board myBoard, int** board){
  int value = 0;
  array<int,2> piece = findSmallest(moveRank,moveFile,myBoard, board);
  if(piece[0] != -1){
    int capturedValue = pieceValue(board[moveRank][moveFile] % 8);
    array<int,2> endPos = {moveRank, moveFile};
    int capturedPiece = myBoard.make_move(piece, endPos);
    value = max(0,capturedValue - see(moveRank,moveFile,myBoard,myBoard.getBoard()));
    myBoard.unmake_move(endPos,piece,capturedPiece);
  }
  return value;
}

int evalSee(int initRank,int initFile,int endRank,int endFile, Board myBoard, int** board){
  int capturedValue = pieceValue(board[endRank][endFile]);
  int capturedPiece = myBoard.make_move({initRank,initFile}, {endRank,endFile});
  int value = max(0,capturedValue - see(endRank,endFile,myBoard,myBoard.getBoard()));
  myBoard.unmake_move({endRank,endFile},{initRank,initFile},capturedPiece);
  return value;
}

int predictedVal(int initRank, int initFile,int moveRank, int moveFile, Board myBoard, int** board, bool isEndGame){
  //cout << "predict: " << initRank << " " << initFile << " to: " << moveRank << " " << moveFile << endl;
  int guessScore = 0;
  int initPieceValue = pieceValue(board[initRank][initFile]);
  int targetPieceValue = pieceValue(board[moveRank][moveFile]);

  if(targetPieceValue != 0){
    guessScore = 10*targetPieceValue - initPieceValue;
  }

  //Promotion
  if(initPieceValue == 100){
    // EP
    if(initFile != moveFile){
      guessScore = 900;
    }
    if(moveRank == 7 || moveRank == 0){
      guessScore += 800;
    }
  }
  else{
    if(oppPawnAttacks(moveRank,moveFile,board)){
      guessScore -= 350;
    }
    if(oppPawnAttacks(initRank,initFile,board)){
      guessScore += 350;
    }
  }
  //return guessScore;
    //Encourage Development from initPosition
    int initPosVal;
    int endPosVal;



    int currTurnColour = turnColour;
    //Under attack currently (encrourage movement)
    if(turnColour == white){
      //cout << "enter predicted" << endl;
      initPosVal = whitePiecePositionValue(board[initRank][initFile] % 8,initRank,initFile,myBoard,isEndGame);
      endPosVal = whitePiecePositionValue(board[initRank][initFile] % 8, moveRank,moveFile,myBoard,isEndGame);
      guessScore += (endPosVal - initPosVal);
    }
    else{
      initPosVal = blackPiecePositionValue(board[initRank][initFile] % 8,initRank,initFile,myBoard,isEndGame);
      endPosVal = blackPiecePositionValue(board[initRank][initFile] % 8, moveRank,moveFile,myBoard,isEndGame);
      guessScore += (endPosVal - initPosVal);
    }


  return guessScore;



}

int mopUpEval(array<int,2> myKing, array<int,2> oppKing, int myEval, int oppEval, float endGameWeight){
  int mopUp = 0;
  if(endGameWeight>0 && myEval > oppEval + 200){
    mopUp = centerManhattanDistance[oppKing[0]*8 + oppKing[1]] * 10;
    mopUp += (14 - straightMovesDiff[myKing[0]*8 + myKing[1]][oppKing[0]*8 + oppKing[1]]) * 4;
    return (int) (mopUp * endGameWeight);
  }
  return 0;
}

float endGameWeight(int endGameValue){
  if(endGameValue < 1645){
    return 1 - (endGameValue/1645);
  }
  return 0;
}

int evaluate(Board myBoard,int** board,int alpha,int beta){
  int whiteValue = 0;
  int blackValue = 0;
  int whiteValueWOPawns = 0;
  int blackValueWOPawns = 0;
  int blackPosValue = 0;
  int whitePosValue = 0;
  int posValue;
  int pieceVal;
  bool isEndGame = false;
  array<PieceList,6> whitePieces = myBoard.getWhitePieces();
  array<PieceList,6> blackPieces = myBoard.getBlackPieces();

  for(int j = 1;j < 5; j++){
    array<int,2>* whitePiecePositons = whitePieces[j].getOccupiedSquares();
    for(int i = 0;i < whitePieces[j].getSize(); i++){
      pieceVal = pieceValue(board[whitePiecePositons[i][0]][whitePiecePositons[i][1]]);
      whiteValueWOPawns += pieceVal;
      posValue = whitePiecePositionValue(board[whitePiecePositons[i][0]][whitePiecePositons[i][1]]%8,whitePiecePositons[i][0],whitePiecePositons[i][1],myBoard,isEndGame);
      //cout << "piece: " << whitePiecePositons[i][0] << " " << whitePiecePositons[i][1] << endl;
      //cout << "posValue: " << posValue << endl;
      whiteValue += posValue + pieceVal;
    }
  }
  float whiteEndGameWeight = endGameWeight(whiteValueWOPawns);
  //cout << endl;
  for(int j = 1;j < 5; j++){
    array<int,2>* blackPiecePositons = blackPieces[j].getOccupiedSquares();
    for(int i = 0;i < blackPieces[j].getSize(); i++){
      pieceVal = pieceValue(board[blackPiecePositons[i][0]][blackPiecePositons[i][1]]);
      blackValueWOPawns += pieceVal;
      posValue = blackPiecePositionValue(board[blackPiecePositons[i][0]][blackPiecePositons[i][1]] % 8,blackPiecePositons[i][0],blackPiecePositons[i][1],myBoard,isEndGame);
      //cout << "piece: " << blackPiecePositons[i][0] << " " << blackPiecePositons[i][1] << endl;
      //cout << "posValue: " << posValue << endl;
      blackValue += posValue + pieceVal;
    }
  }
  float blackEndGameWeight = endGameWeight(blackValueWOPawns);

  if((blackValueWOPawns + whiteValueWOPawns) <= 3290){
    isEndGame = true;
  }



  int whitePawnVal = whitePieces[0].getSize() * 100;
  array<int,2>* whitePawnPositons = whitePieces[0].getOccupiedSquares();

  for(int i = 0;i < whitePieces[0].getSize(); i++){
    pieceVal = pieceValue(board[whitePawnPositons[i][0]][whitePawnPositons[i][1]]);
    //posValue = whitePiecePositionValue(board[whitePawnPositons[i][0]][whitePawnPositons[i][1]] % 8,whitePawnPositons[i][0],whitePawnPositons[i][1],myBoard,isEndGame);
    posValue = whitePawnPositionValue(whitePawnPositons[i][0],whitePawnPositons[i][1],myBoard,whiteEndGameWeight);

    whiteValue += posValue + pieceVal;
  }
  //White King
  whiteValue += whitePiecePositionValue(2,whitePieces[5].getOccupiedSquares()[0][0],whitePieces[5].getOccupiedSquares()[0][1],myBoard,isEndGame);

  int blackPawnVal = blackPieces[0].getSize() * 100;
  array<int,2>* blackPawnPositons = blackPieces[0].getOccupiedSquares();

  for(int i = 0;i < blackPieces[0].getSize(); i++){
    pieceVal = pieceValue(board[blackPawnPositons[i][0]][blackPawnPositons[i][1]]);
    //posValue = blackPiecePositionValue(board[blackPawnPositons[i][0]][blackPawnPositons[i][1]] % 8,blackPawnPositons[i][0],blackPawnPositons[i][1],myBoard,isEndGame);
    posValue = blackPawnPositionValue(blackPawnPositons[i][0],blackPawnPositons[i][1],myBoard,blackEndGameWeight);

    blackValue += posValue + pieceVal;
  }
  //blackKing
  blackValue += blackPiecePositionValue(2,blackPieces[5].getOccupiedSquares()[0][0],blackPieces[5].getOccupiedSquares()[0][1],myBoard,isEndGame);
   //return whiteValue - blackValue;
  //return whiteValue - blackValue;

  whiteValue += mopUpEval(whitePieces[5].getOccupiedSquares()[0],blackPieces[5].getOccupiedSquares()[0],whiteValueWOPawns + whitePawnVal,blackValueWOPawns + blackPawnVal,whiteEndGameWeight);
  blackValue += mopUpEval(blackPieces[5].getOccupiedSquares()[0],whitePieces[5].getOccupiedSquares()[0],blackValueWOPawns + blackPawnVal,whiteValueWOPawns + whitePawnVal,blackEndGameWeight);
  int seeVal;
  int removeValue = 0;
  BitBoard oppBitBoard = oppAttackingSquares(board,myBoard);
  MoveList captureMoves = getCaptureMoves(board,myBoard,oppBitBoard);
  for(int i = 0; i < captureMoves.size;i++){
    if(turnColour == white){
      if(whiteValue-(blackValue-removeValue) >= beta){
        delete captureMoves.moves;
        totalMem--;
        //return beta;
        return whiteValue-(blackValue-removeValue);
      }
    }
    else{
      if(blackValue-(whiteValue-removeValue) >= beta){
        delete captureMoves.moves;
        totalMem--;
        return blackValue-(whiteValue-removeValue);
        //return beta;
      }
    }
    //cout << board[captureMoves.moves[i][0]][captureMoves.moves[i][1]] << " " << captureMoves.moves[i][0] << " " << captureMoves.moves[i][1] << " to: " << captureMoves.moves[i][2] << " " << captureMoves.moves[i][3] << endl;
    seeVal = evalSee(captureMoves.moves[i][0], captureMoves.moves[i][1],captureMoves.moves[i][2],captureMoves.moves[i][3], myBoard, board);
    //cout << "value: " << seeVal << endl;
    removeValue = max(removeValue,seeVal);
  }

  if(turnColour == white){
    blackValue -= removeValue;
  }
  else{
    whiteValue -= removeValue;
  }
  /*
  if(turnColour == black){
    //turnColour = black;
    for(int j = 0;j < 6; j++){
      array<int,2>* whitePiecePositons = whitePieces[j].getOccupiedSquares();
      for(int i = 0;i < whitePieces[j].getSize(); i++){
        removeValue = max(removeValue,see(whitePiecePositons[i][0],whitePiecePositons[i][1],myBoard,board));
      }
    }
    whiteValue -= removeValue;
  }
  else{
    for(int j = 0;j < 6; j++){
      array<int,2>* blackPiecePositons = blackPieces[j].getOccupiedSquares();
      for(int i = 0;i < blackPieces[j].getSize(); i++){
        //turnColour = white;
        removeValue = max(removeValue,see(blackPiecePositons[i][0],blackPiecePositons[i][1],myBoard,board));
        //turnColour = black;
      }
    }
    blackValue -= removeValue;
  }
  */
  delete captureMoves.moves;
  totalMem--;
  //cout << "in eval"<< endl;
  //cout << "turnColour: " << turnColour<< endl;
  //cout << whiteValue << " " << blackValue << endl;
  if(turnColour == white){
    return whiteValue - blackValue;
  }
  return blackValue - whiteValue;

}
