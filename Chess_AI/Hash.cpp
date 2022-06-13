#include <iostream>
#include <stdlib.h>
#include "Globals.h"
#include "Hash.h"
#include "Board.h"
using namespace std;

mt19937 mt(01234567);

unsigned long long int randomInt(){
    uniform_int_distribution<unsigned long long int>
                                 dist(0, UINT64_MAX);
    return dist(mt);
}

void initTable(){
    int accum = 0;
    for (int i = 0; i<8; i++){
      for (int j = 0; j<8; j++){
        for (int k = 0; k<12; k++){
          for(int l = 0; l<64;l++){
            for(int m = 0;m < 9; m++){
              ZobristTable[i][j][k][l][m] = randomInt();
            }
          }
        }
      }
    }
    blackHash = randomInt();
    whiteHash = randomInt();
}



int getPieceHash(int piece){
  if(piece > 16){
    return piece - 11;
  }
  return piece-9;
}

int getPlayerHash(int ply){
  if(ply%2 == 1){
    return 1;
  }
  return 0;
  /*
  if(turnColour == black){
    return 1;
  }
  return 0;
  */
}

int getCastleHash(Board myBoard){
  array<bool,6> castleProperties = myBoard.getCastleProperties();
  int result = 0;
  if(castleProperties[0]){
    result ^= 1;
  }
  if(castleProperties[1]){
    result ^= 2;
  }
  if(castleProperties[2]){
    result ^= 4;
  }
  if(castleProperties[3]){
    result ^= 8;
  }
  if(castleProperties[4]){
    result ^= 16;
  }
  if(castleProperties[5]){
    result ^= 32;
  }
  return result;
}

int getEnPaisantHash(int piece, Board myBoard){
  array<int,2> EnPaisantProperties = myBoard.getEnPaisantProperties();
  if (EnPaisantProperties[0] < 9){
    return EnPaisantProperties[0];
  }
  if(EnPaisantProperties[1] < 9){
    return EnPaisantProperties[1];
  }
  return 8;
}

// Computes the hash value of a given board
unsigned long long int computeHash(int** board, Board myBoard){
    unsigned long long int h = 0;
    //int playerHash;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (board[i][j]){
              int pieceHash = getPieceHash(board[i][j]);
              h ^= ZobristTable[i][j][pieceHash][getCastleHash(myBoard)][getEnPaisantHash(pieceHash,myBoard)];
            }
        }
    }
    if(turnColour == black){
      h ^= blackHash;
    }
    return h;
}


int mateScore(int value, int ply){
  if(abs(value) > 10000){
    if(value > 0){
      //return (numeric_limits<int>::max() - (ply));
      return value - ply;
    }
    else{
      return -(-value - ply);
      return -(numeric_limits<int>::max() - (ply));
    }
  }
  return value;
}

void updateHashTable(unsigned long long int hash, int depth, int plySearched,int storedType, int value, array<int,4> move){
  auto got = HashMap.find (hash);
  if(got == HashMap.end()){
    HashTuple newInsert;
    //array<int,4> insertMove = {move[0],move[1],move[2],move[3]};
    newInsert.depth = depth;
    newInsert.storedType = storedType;
    newInsert.value = mateScore(value,plySearched);
    newInsert.visited = false;
    newInsert.thisMove = true;
    newInsert.move = move;
    HashMap.insert(make_pair(hash,newInsert));
    numOfInsertions++;
  }

  else if(got->second.depth <= depth){
    got->second.depth = depth;
    got->second.storedType = storedType;
    got->second.value = mateScore(value,plySearched);
    got->second.thisMove = true;
    got->second.move = move;
  }
  else{
    got->second.thisMove = true;
  }

}

unsigned long long int makeMoveHash(array<int,2> initPos, array<int,2> endPos, int currPiece, int oppPiece, unsigned long long int currHash,Board myBoard,int plySearched){
  int castleHash = getCastleHash(myBoard);
  int enPaisantHash = getEnPaisantHash(currPiece,myBoard);
  int playerHash = getPlayerHash(plySearched);
  if(playerHash){
    currHash ^= blackHash;
  }
  else{
    currHash ^= whiteHash;
  }

  currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece][castleHash][enPaisantHash];
  currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece][castleHash][enPaisantHash];
  //castling
  if(currPiece % 6 == 1 && abs(initPos[1] - endPos[1]) > 1){
    if((endPos[1] - initPos[1]) > 0){
      //kingSide
      if(currPiece == 7){
        currHash ^= ZobristTable[7][7][10][castleHash][enPaisantHash];
        currHash ^= ZobristTable[7][5][10][castleHash][enPaisantHash];
      }
      else{
        currHash ^= ZobristTable[0][7][4][castleHash][enPaisantHash];
        currHash ^= ZobristTable[0][5][4][castleHash][enPaisantHash];
      }
    }
    //castling queenSide
    else{
      if(currPiece == 7){
        currHash ^= ZobristTable[7][0][10][castleHash][enPaisantHash];
        currHash ^= ZobristTable[7][3][10][castleHash][enPaisantHash];
      }
      else{
        currHash ^= ZobristTable[0][0][4][castleHash][enPaisantHash];
        currHash ^= ZobristTable[0][3][4][castleHash][enPaisantHash];
      }
    }
  }
  //Capture
  else if(oppPiece > 8 && oppPiece < 29){
    currHash ^= ZobristTable[endPos[0]][endPos[1]][getPieceHash(oppPiece)][castleHash][enPaisantHash];
  }
  // Promototed Capture
  else if(oppPiece > 29){
    // errase prev hash and put queen
    currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece][castleHash][enPaisantHash];
    currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece+5][castleHash][enPaisantHash];
    //remove oppPiece
    currHash ^= ZobristTable[endPos[0]][endPos[1]][getPieceHash(oppPiece/10)][castleHash][enPaisantHash];

  }
  //Promotion, no capture
  else if(oppPiece == 1){
    //remove prev placed pawn with queen
    currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece][castleHash][enPaisantHash];
    currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece+5][castleHash][enPaisantHash];
  }
  //EnPaisant capture
  else if((currPiece % 6 == 0) && abs(endPos[1] - initPos[1]) == 1){
    if(currPiece == 6){
      //remove black pawn
      currHash ^= ZobristTable[endPos[0] + 1][endPos[1]][0][castleHash][enPaisantHash];
    }
    else{
      //remove white pawn
      currHash ^= ZobristTable[endPos[0] - 1][endPos[1]][6][castleHash][enPaisantHash];
    }
  }


  return currHash;
}

unsigned long long int unmakeMoveHash(array<int,2> initPos, array<int,2> endPos, int currPiece, int oppPiece, unsigned long long int currHash,Board myBoard,int plySearched){
  int castleHash = getCastleHash(myBoard);
  int enPaisantHash = getEnPaisantHash(currPiece,myBoard);

  int playerHash = getPlayerHash(plySearched);

  //castling
  if(currPiece % 6 == 1 && abs(initPos[1] - endPos[1]) > 1){
    if((endPos[1] - initPos[1]) < 0){
      //kingSide
      if(currPiece == 7){
        currHash ^= ZobristTable[7][5][10][castleHash][enPaisantHash];
        currHash ^= ZobristTable[7][7][10][castleHash][enPaisantHash];
      }
      else{
        currHash ^= ZobristTable[0][5][4][castleHash][enPaisantHash];
        currHash ^= ZobristTable[0][7][4][castleHash][enPaisantHash];
      }
    }
    //castling queenSide
    else{
      if(currPiece == 7){
        currHash ^= ZobristTable[7][3][10][castleHash][enPaisantHash];
        currHash ^= ZobristTable[7][0][10][castleHash][enPaisantHash];
      }
      else{
        currHash ^= ZobristTable[0][3][4][castleHash][enPaisantHash];
        currHash ^= ZobristTable[0][0][4][castleHash][enPaisantHash];
      }
    }
  }
  //Normal Capture
  else if(oppPiece > 8 && oppPiece < 29){
    currHash ^= ZobristTable[initPos[0]][initPos[1]][getPieceHash(oppPiece)][castleHash][enPaisantHash];
  }

  // Promototed Capture
  else if(oppPiece > 29){
    //restoreOpp
    currHash ^= ZobristTable[initPos[0]][initPos[1]][getPieceHash(oppPiece/10)][castleHash][enPaisantHash];
    //Remove Queen and double
    currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece+5][castleHash][enPaisantHash];
    currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece][castleHash][enPaisantHash];

  }
  // Promotion no Capture
  else if(oppPiece == 1){
    //remove prev placed pawn with queen
    currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece+5][castleHash][enPaisantHash];
    currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece][castleHash][enPaisantHash];
  }

  //EnPaisant capture
  else if((currPiece % 6 == 0) && abs(endPos[1] - initPos[1]) == 1){
    if(currPiece == 6){
      //remove black pawn
      currHash ^= ZobristTable[initPos[0] + 1][initPos[1]][0][castleHash][enPaisantHash];
    }
    else{
      //remove white pawn
      currHash ^= ZobristTable[initPos[0] - 1][initPos[1]][6][castleHash][enPaisantHash];
    }
  }

  currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece][castleHash][enPaisantHash];
  currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece][castleHash][enPaisantHash];
  if(playerHash){
    currHash ^= blackHash;
  }
  else{
    currHash ^= whiteHash;
  }

  return currHash;
}

// exit 6 at 1.16
