#include<iostream>
#include<string>
#include<cstdlib>
#include <algorithm>
#include <map>
#include <vector>
#include <experimental/random>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <chrono>
#include <stack>
#include <limits>
#include <cmath>

using namespace std;

const int pawn = 1;
const int king = 2;
const int knight = 3;
const int bishop = 4;
const int rook = 5;
const int queen = 6;

const int black = 8;
const int white = 16;


const int pawnValue = 100;
const int knightValue = 310;
const int bishopValue = 335;
const int rookValue = 500;
const int queenValue = 900;

long long int emptyBoard = 0;

int totalMem = 0;
struct Tuple{
    bool accessed;
    int value;
    int depth;
};

struct MoveList{
  int size;
  array<int,4>* moves;
};

struct BitBoard{
  long long int attackingSquares;
  long long int kingDangerSquares;
  long long int checkers;
  long long int checkRay;
  int numCheckers;
};

long long int posToBitBoard(array<int,2> pos){
  return (long long int)pow(2,((pos[0]) * 8) + pos[1]);
}



unsigned long long int ZobristTable[8][8][12][64][8][2];
const int tableSize = 21474836;
Tuple HashTable[21474836];
mt19937 mt(01234567);

int totalCount = 0;

int turnColour = black;

stack<bool> castlePropertiesStack;
stack<int> EnPaisantPropertiesStack;

bool whiteKingMoved = true;
bool blackKingMoved = true;

bool whiteKingRookMoved = false;
bool whiteQueenRookMoved = false;
bool blackKingRookMoved = false;
bool blackQueenRookMoved = false;

int blackEnPaisant = 3;
int whiteEnPaisant = 9;


const int pawnMap[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
                         50, 50, 50, 50, 50, 50, 50, 50,
                         10, 10, 20, 30, 30, 20, 10, 10,
                         8,  8, 10, 25, 25,  10,  8,  8,
                         6,  6,  8, 25, 25,  8,  6,  6,
                         8,  5,  7,  5,  5, 10,  5,  8,
                         5, 10, 10,-20,-20, 10, 10,  5,
                         0,  0,  0,  0,  0,  0,  0,  0 };
const int knightMap[64] = {-50,-40,-30,-30,-30,-30,-40,-50,
                           -40,-20,  0,  0,  0,  0,-20,-40,
                           -30,  0, 10, 15, 15, 10,  0,-30,
                           -30,  5, 15, 20, 20, 15,  5,-30,
                           -30,  0, 15, 20, 20, 15,  0,-30,
                           -30,  5, 10, 15, 15, 10,  5,-30,
                           -40,-20,  0,  5,  5,  0,-20,-40,
                           -50,-40,-30,-30,-30,-30,-40,-50};

const int bishopMap[64] = {-20,-10,-10,-10,-10,-10,-10,-20,
                           -10,  0,  0,  0,  0,  0,  0,-10,
                           -10,  0,  5, 10, 10,  5,  0,-10,
                           -10,  5,  5, 10, 10,  5,  5,-10,
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
                            10, 10,-15,-25,-25,-15, 10, 10,
                            10, 30, 25,-25,-20, -5, 30, 20};
const int blackKingMapMG[64] = {-30,-40,-40,-50,-50,-40,-40,-30,
                           -30,-40,-40,-50,-50,-40,-40,-30,
                           -30,-40,-40,-50,-50,-40,-40,-30,
                           -30,-40,-40,-50,-50,-40,-40,-30,
                           -20,-30,-30,-40,-40,-30,-30,-20,
                           -10,-20,-20,-20,-20,-20,-20,-10,
                            10, 10,-15,-25,-25,-15, 10, 10,
                            20, 30, -5,-10,-15, 25, 30, 20};
const int kingMapEG[64] = {-50,-40,-30,-20,-20,-30,-40,-50,
                           -30,-20,-10,  0,  0,-10,-20,-30,
                           -30,-10, 20, 30, 30, 20,-10,-30,
                           -30,-10, 30, 40, 40, 30,-10,-30,
                           -30,-10, 30, 40, 40, 30,-10,-30,
                           -30,-10, 20, 30, 30, 20,-10,-30,
                           -30,-30,  0,  0,  0,  0,-30,-30,
                           -50,-30,-30,-30,-30,-30,-30,-50};



class PieceList{
  private:
    int numPieces;
    array<int,2>* occupiedSquares;
    int** pieceMap;
  public:
    PieceList(){
      this->pieceMap;
      this->numPieces = 0;
      this->occupiedSquares;
      //this->occupiedSquares = new array<int,2>[8];
      //totalMem++;
    }
    array<int,2>* getOccupiedSquares(){
      return this->occupiedSquares;
    }
    int getSize(){
      return this->numPieces;
    }
    void setOccupiedSquares(int maxSize){
      this->occupiedSquares = new array<int,2>[maxSize];
    }
    void setPieceMap(int** pieceMap){
      this->pieceMap = pieceMap;
    }
    int** getPieceMap(){
      return this->pieceMap;
    }
    bool duplicates(){
      for(int i = 0; i < this->numPieces - 1; i++){
        array<int,2> curr = this->occupiedSquares[i];
        for(int j = i+1; j < this->numPieces;j++){
          if(curr[0] == this->occupiedSquares[j][0] && curr[1] == this->occupiedSquares[j][1]){
            return true;
          }
        }
      }
      return false;
    }

    void addPieceAtSquare(int rank, int file){
      array<int,2> newSquare = {rank,file};
      this->occupiedSquares[this->numPieces] = newSquare;
  		this->pieceMap[rank][file] = this->numPieces;
  		this->numPieces++;
      /*
      if(duplicates()){
        cout << "wtf" << endl;
        cout << rank << " " << file << endl;
        for(int i = 0; i < this->numPieces;i++){
          cout << this->occupiedSquares[i][0] << " " << this->occupiedSquares[i][1] << endl;
        }
        sleep(10);
      }
      */
    }

    void removePiece(array<int,2> initPos, array<int,2> endPos){
      int pieceIndex = this->pieceMap[endPos[0]][endPos[1]];
      this->occupiedSquares[pieceIndex] = this->occupiedSquares[this->numPieces - 1]; // move last element in array to the place of the removed element
  		this->pieceMap[this->occupiedSquares[pieceIndex][0]][this->occupiedSquares[pieceIndex][1]] = pieceIndex; // update map to point to the moved element's new location in the array
      //this->pieceMap[endPos[0]][endPos[1]] = 9;
      //this->pieceMap[endPos[0]][endPos[1]] = -1;
  		this->numPieces--;
      /*
      cout << "pieceMap" << endl;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          cout << this->pieceMap[i][j] << " ";
        }
        cout << endl;
      }
      */
    }

    void movePiece(array<int,2> initPos, array<int,2> endPos){
      int pieceIndex = this->pieceMap[initPos[0]][initPos[1]]; // get the index of this element in the occupiedSquares array
      //cout << "move piece index: " << pieceIndex << endl;
  		this->occupiedSquares[pieceIndex] = endPos;
  		this->pieceMap[endPos[0]][endPos[1]] = pieceIndex;
      //this->pieceMap[initPos[0]][initPos[1]] = 9;
      /*
      if(duplicates()){
        cout << "wtf" << endl;
        cout << initPos[0] << " " << initPos[1] << " to: "<< endPos[0] << " " << endPos[1] << endl;
        for(int i = 0; i < this->numPieces;i++){
          cout << this->occupiedSquares[i][0] << " " << this->occupiedSquares[i][1] << endl;
        }
        sleep(10);
      }
      //this->pieceMap[initPos[0]][initPos[1]]= -1;
      */
    }
};

class Board{
private:
  int** boardReprentation;
  array<bool,6> castleProperties;
  array<int,2> EnPaisantProperties;
public:
  //array<PieceList,2> pawnPositions;
  //PieceList pawnPositions[2];
  array<PieceList,2> pawnPositions;
  array<PieceList,2> knightPositions;
  array<PieceList,2> bishopPositions;
  array<PieceList,2> rookPositions;
  array<PieceList,2> queenPositions;
  array<PieceList,2> kingPositions;
  //PieceList* allPositions;
  //array<array<PieceList,2>,1> allPositions;

  void updateStacks(){
    for(int i = 0; i<6;i++){
      castlePropertiesStack.push(this->castleProperties[i]);
    }
    EnPaisantPropertiesStack.push(this->EnPaisantProperties[0]);
    EnPaisantPropertiesStack.push(this->EnPaisantProperties[1]);

  }
  int** initPieceMap(){
    int** map = new int*[8];
    //totalMem++;
    for(int i = 0; i < 8; i++){
      map[i] = new int[8];
      //totalMem++;
      for(int j = 0; j < 8; j++){
        map[i][j] = 9;
      }
    }
    return map;
  }
  void setupPieceLists(){
    this->pawnPositions[0].setPieceMap(initPieceMap());
    this->pawnPositions[0].setOccupiedSquares(8);
    this->pawnPositions[1].setPieceMap(initPieceMap());
    this->pawnPositions[1].setOccupiedSquares(8);
    this->knightPositions[0].setPieceMap(initPieceMap());
    this->knightPositions[0].setOccupiedSquares(10);
    this->knightPositions[1].setPieceMap(initPieceMap());
    this->knightPositions[1].setOccupiedSquares(10);
    this->bishopPositions[0].setPieceMap(initPieceMap());
    this->bishopPositions[0].setOccupiedSquares(10);
    this->bishopPositions[1].setPieceMap(initPieceMap());
    this->bishopPositions[1].setOccupiedSquares(10);
    this->rookPositions[0].setPieceMap(initPieceMap());
    this->rookPositions[0].setOccupiedSquares(10);
    this->rookPositions[1].setPieceMap(initPieceMap());
    this->rookPositions[1].setOccupiedSquares(10);
    this->queenPositions[0].setPieceMap(initPieceMap());
    this->queenPositions[0].setOccupiedSquares(9);
    this->queenPositions[1].setPieceMap(initPieceMap());
    this->queenPositions[1].setOccupiedSquares(9);
    this->kingPositions[0].setPieceMap(initPieceMap());
    this->kingPositions[0].setOccupiedSquares(1);
    this->kingPositions[1].setPieceMap(initPieceMap());
    this->kingPositions[1].setOccupiedSquares(1);
  }


  Board(int** board,array<bool,6> castleProperties,array<int,2> EnPaisantProperties){
    this->boardReprentation = board;
    this->castleProperties = castleProperties;
    this->EnPaisantProperties = EnPaisantProperties;
    setupPieceLists();
    updateStacks();
  }

  array<int,2> getEnPaisantProperties(){
    return this->EnPaisantProperties;
  }
  array<bool,6> getCastleProperties(){
    return this->castleProperties;
  }
  int** getBoard(){
    return this->boardReprentation;
  }
  void restoreStacks(){
    for(int i = 0;i < 6;i++){
      castlePropertiesStack.pop();
    }
    EnPaisantPropertiesStack.pop();
    EnPaisantPropertiesStack.pop();

  }
  void restoreQualities(){
    for(int i = 5;i >= 0;i--){
      this->castleProperties[i] = castlePropertiesStack.top();
      castlePropertiesStack.pop();
    }
    for(int i = 0; i < 6; i++){
      castlePropertiesStack.push(this->castleProperties[i]);
    }
    this->EnPaisantProperties[1] = EnPaisantPropertiesStack.top();
    EnPaisantPropertiesStack.pop();
    this->EnPaisantProperties[0] = EnPaisantPropertiesStack.top();
    EnPaisantPropertiesStack.push(this->EnPaisantProperties[1]);
  }
  void boardPrinter(int** board){
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        if(board[i][j] == 0){
          cout << "00" << ' ';
        }
        else if(board[i][j] > 15){
          cout << "W";
          if(board[i][j] % 8 == 1){
            cout << "P" << ' ';
          }
          else if(board[i][j] % 8 == 2){
            cout << "K" << ' ';
          }
          else if(board[i][j] % 8 == 3){
            cout << "N" << ' ';
          }
          else if(board[i][j] % 8 == 4){
            cout << "B" << ' ';
          }
          else if(board[i][j] % 8 == 5){
            cout << "R" << ' ';
          }
          else{
            cout << "Q" << ' ';
          }
        }
        else{
          cout << "B";
          if(board[i][j] % 8 == 1){
            cout << "P" << ' ';
          }
          else if(board[i][j] % 8 == 2){
            cout << "K" << ' ';
          }
          else if(board[i][j] % 8 == 3){
            cout << "N" << ' ';
          }
          else if(board[i][j] % 8 == 4){
            cout << "B" << ' ';
          }
          else if(board[i][j] % 8 == 5){
            cout << "R" << ' ';
          }
          else{
            cout << "Q" << ' ';
          }
        }
      }
      cout << endl;
    }
  }
  void checkRookMove(int rank, int file, int** board){
    if(board[rank][file] % 8 != 5){
      return;
    }
    if(rank == 0 && file == 0){
      this->castleProperties[5] = true;
    }
    else if(rank == 0 && file == 7){
      this->castleProperties[4] = true;
    }
    else if(rank == 7 && file == 0){
      this->castleProperties[2] = true;
    }
    else if(rank == 7 && file == 7){
      this->castleProperties[1] = true;
    }
  }
  void updateProperties(array<int,2> initPos, array<int,2> endPos){
    int** board = this->getBoard();
    //RookMoved
    checkRookMove(initPos[0],initPos[1],board);
    //KingMoved
    if(board[initPos[0]][initPos[1]] % 8 == 2){
      if(turnColour == white){
        this->castleProperties[0] = true;
      }
      else{
        this->castleProperties[3] = true;
      }
    }
    //EnPaisant
    if((board[initPos[0]][initPos[1]] % 8 == 1) && abs(endPos[0] - initPos[0]) > 1){
      if(turnColour == white){
        this->EnPaisantProperties[1] = endPos[1];
        this->EnPaisantProperties[0] = 9;
      }
      else{
        this->EnPaisantProperties[0] = endPos[1];
        this->EnPaisantProperties[1] = 9;
      }
    }
    else{
      this->EnPaisantProperties[1] = 9;
      this->EnPaisantProperties[0] = 9;
    }
  }
  /*
  vector<array<int,2>> getWhitePieces(){
    vector<array<int,2>> result;
    array<int,2>* pawns = this->pawnPositions[1].getOccupiedSquares();
    array<int,2>* knights = this->knightPositions[1].getOccupiedSquares();
    array<int,2>* bishops = this->bishopPositions[1].getOccupiedSquares();
    array<int,2>* rooks = this->rookPositions[1].getOccupiedSquares();
    array<int,2>* queens = this->queenPositions[1].getOccupiedSquares();
    array<int,2> king = this->kingPositions[1].getOccupiedSquares()[0];

    for(int i = 0; i < this->pawnPositions[1].getSize();i++){
      result.push_back({pawns[i][0],pawns[i][1]});
    }
    for(int i = 0; i < this->knightPositions[1].getSize();i++){
        result.push_back({knights[i][0],knights[i][1]});
    }
    for(int i = 0; i < this->bishopPositions[1].getSize();i++){
        result.push_back({bishops[i][0],bishops[i][1]});
    }
    for(int i = 0; i < this->rookPositions[1].getSize();i++){
        result.push_back({rooks[i][0],rooks[i][1]});
    }
    for(int i = 0; i < this->queenPositions[1].getSize();i++){
        result.push_back({queens[i][0],queens[i][1]});
    }

    result.push_back({king[0],king[1]});
    return result;
  }
  */
  array<PieceList,6> getWhitePieces(){
    return {this->pawnPositions[1],this->knightPositions[1],this->bishopPositions[1],
    this->rookPositions[1],this->queenPositions[1],this->kingPositions[1]};
  }
  array<PieceList,6> getBlackPieces(){
    return {this->pawnPositions[0],this->knightPositions[0],this->bishopPositions[0],
    this->rookPositions[0],this->queenPositions[0],this->kingPositions[0]};
  }

  void updatePiecePos(array<int,2> initPos,array<int,2> endPos, int pieceType, int pieceColour){
    //cout << "updating: " << initPos[0] << " " << initPos[1] << " to: " << endPos[0] << " " << endPos[1] << endl;
    switch(pieceType){
      case 1:
      /*
        if(initPos[0] == 7 && initPos[1] == 0 && endPos[0] == 6 && endPos[1] == 0){
          cout << "init black pawns:" << endl;
          for(int i = 0; i < this->pawnPositions[0].getSize();i++){
            cout << this->pawnPositions[0].getOccupiedSquares()[i][0] << " " << this->pawnPositions[0].getOccupiedSquares()[i][1] << endl;
          }
          cout << "init white pawns:" << endl;
          for(int i = 0; i < this->pawnPositions[1].getSize();i++){
            cout << this->pawnPositions[1].getOccupiedSquares()[i][0] << " " << this->pawnPositions[1].getOccupiedSquares()[i][1] << endl;
          }
        }
        */
        this->pawnPositions[pieceColour].movePiece(initPos,endPos);
          /*
        cout << "black pawns:" << endl;
        for(int i = 0; i < this->pawnPositions[0].getSize();i++){
          cout << this->pawnPositions[0].getOccupiedSquares()[i][0] << " " << this->pawnPositions[0].getOccupiedSquares()[i][1] << endl;
        }
        cout << "white pawns:" << endl;
        for(int i = 0; i < this->pawnPositions[1].getSize();i++){
          cout << this->pawnPositions[1].getOccupiedSquares()[i][0] << " " << this->pawnPositions[1].getOccupiedSquares()[i][1] << endl;
        }
        */
        break;
      case 2:
        this->kingPositions[pieceColour].movePiece(initPos,endPos);
        break;
      case 3:
        this->knightPositions[pieceColour].movePiece(initPos,endPos);
        break;
      case 4:
        this->bishopPositions[pieceColour].movePiece(initPos,endPos);
        break;
      case 5:
        this->rookPositions[pieceColour].movePiece(initPos,endPos);
        break;
      case 6:
        this->queenPositions[pieceColour].movePiece(initPos,endPos);
        break;
      default:
        cout << pieceType << endl;
        cout << initPos[0] << " " << initPos[1] << " to: " << endPos[0] << " " << endPos[1] << endl;
        cout << "updatePiece" << endl;
        sleep(10);
    }
  }

  void updatePieceRemoval(array<int,2> initPos,array<int,2> endPos, int pieceType, int pieceColour){
    //cout << "removing: " << pieceType << " " << pieceColour << " at: " << endPos[0] << " " << endPos[1] << endl;
     switch(pieceType){
      case 1:
        this->pawnPositions[pieceColour].removePiece(initPos,endPos);
        /*
        cout << "black pawns:" << endl;
        for(int i = 0; i < this->pawnPositions[0].getSize();i++){
          cout << this->pawnPositions[0].getOccupiedSquares()[i][0] << " " << this->pawnPositions[0].getOccupiedSquares()[i][1] << endl;
        }
        cout << "white pawns:" << endl;
        for(int i = 0; i < this->pawnPositions[1].getSize();i++){
          cout << this->pawnPositions[1].getOccupiedSquares()[i][0] << " " << this->pawnPositions[1].getOccupiedSquares()[i][1] << endl;
        }
        */
        break;
      case 2:
        this->kingPositions[pieceColour].removePiece(initPos,endPos);
        break;
      case 3:
        this->knightPositions[pieceColour].removePiece(initPos,endPos);
        break;
      case 4:
        this->bishopPositions[pieceColour].removePiece(initPos,endPos);
        break;
      case 5:
        this->rookPositions[pieceColour].removePiece(initPos,endPos);
        break;
      case 6:
        this->queenPositions[pieceColour].removePiece(initPos,endPos);
        break;
      default:
        cout << pieceType << endl;
        cout << "remove" << endl;
        sleep(10);
    }
  }

  void restorePiecePos(int rank, int file, int pieceType, int pieceColour){
    //cout << "restoring: " << pieceType << " " << pieceColour << " at: " << rank << " " << file << endl;
    switch(pieceType){
      case 1:
        this->pawnPositions[pieceColour].addPieceAtSquare(rank,file);
        /*
        cout << "black pawns:" << endl;
        for(int i = 0; i < this->pawnPositions[0].getSize();i++){
          cout << this->pawnPositions[0].getOccupiedSquares()[i][0] << " " << this->pawnPositions[0].getOccupiedSquares()[i][1] << endl;
        }
        cout << "white pawns:" << endl;
        for(int i = 0; i < this->pawnPositions[1].getSize();i++){
          cout << this->pawnPositions[1].getOccupiedSquares()[i][0] << " " << this->pawnPositions[1].getOccupiedSquares()[i][1] << endl;
        }
        */
        break;
      case 2:
        this->kingPositions[pieceColour].addPieceAtSquare(rank,file);
        break;
      case 3:
        this->knightPositions[pieceColour].addPieceAtSquare(rank,file);
        break;
      case 4:
        this->bishopPositions[pieceColour].addPieceAtSquare(rank,file);
        break;
      case 5:
        this->rookPositions[pieceColour].addPieceAtSquare(rank,file);
        break;
      case 6:
        this->queenPositions[pieceColour].addPieceAtSquare(rank,file);
        break;
      default:
        cout << pieceType << endl;
        cout << "restore" << endl;
        sleep(10);
    }
  }

  int applyMoveForward(array<int,2> initPos,array<int,2> endPos){

    int ** board = this->boardReprentation;

    int piece = board[endPos[0]][endPos[1]];


    int pieceColour;
    //enPaisant capture
    if(board[initPos[0]][initPos[1]] % 8 == 1 && initPos[1] != endPos[1] && board[endPos[0]][endPos[1]] == 0){
      //cout << "Ep!!!" << endl;
      if(turnColour == white){
        //std::vector<array<int,2>> blackPiecePos = this->getBlackPieces();
        board[endPos[0] + 1][endPos[1]] = 0;
        array<int,2> capturedPawn;
        capturedPawn[0] = endPos[0] + 1;
        capturedPawn[1] = endPos[1];
        updatePieceRemoval(initPos,capturedPawn,1, 0);
        //cout << "removing :" << capturedPawn[0] << " " << capturedPawn[1] << endl;
        /*
        auto it = find(blackPiecePos.begin(), blackPiecePos.end(), capturedPawn);
        if (it != blackPiecePos.end()) {
          int index = it - blackPiecePos.begin();
          swap(blackPiecePos[index], blackPiecePos.back());
          blackPiecePos.pop_back();
          this->setBlackPieces(blackPiecePos);
        }
        else{
          cout << "LogError removing black piece via EnPaisant" << endl;
          this->boardPrinter(board);
          for (int i = 0; i < blackPiecePos.size(); i++) {
            cout << blackPiecePos[i][0] << " " << blackPiecePos[i][1] << endl;
          }
          sleep(100);
        }
        */
      }
      else{
        //std::vector<array<int,2>> whitePiecePos = this->getWhitePieces();
        board[endPos[0] - 1][endPos[1]] = 0;
        array<int,2> capturedPawn;
        capturedPawn[0] = endPos[0] - 1;
        capturedPawn[1] = endPos[1];
        updatePieceRemoval(initPos,capturedPawn,1, 1);
        //cout << "removing :" << initPos[0] << " " << initPos[1] << endl;
        /*
        if (it != whitePiecePos.end()) {
          int index = it - whitePiecePos.begin();
          swap(whitePiecePos[index], whitePiecePos.back());
          whitePiecePos.pop_back();
          this->setWhitePieces(whitePiecePos);

        }
        else{
          cout << "LogError removing white piece via EnPaisant" << endl;
          cout << endPos[0] << " " << endPos[1] << endl;
          cout << "whitePos: " << endl;
          for(int i=0;i<whitePiecePos.size();i++){
            cout << whitePiecePos[i][0] << " " << whitePiecePos[i][1] << endl;
          }
          sleep(100);
        }
        */
      }
    }
    if(piece != 0){
      if(piece > 16){
        pieceColour = 1;
      }
      else{
        pieceColour = 0;
      }
      this->updatePieceRemoval(initPos,endPos,piece % 8,pieceColour);
    }
    int pieceType = board[initPos[0]][initPos[1]];
    //int pieceColour;
    if(pieceType > 16){
      pieceColour = 1;
    }
    else{
      pieceColour = 0;
    }
    this->updatePiecePos(initPos,endPos,pieceType % 8,pieceColour);
    board[endPos[0]][endPos[1]] = board[initPos[0]][initPos[1]];
    board[initPos[0]][initPos[1]] = 0;

    //Pawn -> Queen
    if(board[endPos[0]][endPos[1]] % 8 == 1 && (endPos[0] == 0 || endPos[0] == 7)){
      //cout << "q->p" << endl;
      board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] + 5;
      updatePieceRemoval(initPos,endPos, 1 , pieceColour);
      /*
      cout << "black Pawns:" << endPos[0] << " " << endPos[1] << endl;
      for(int i = 0; i < this->pawnPositions[0].getSize();i++){
        cout << this->pawnPositions[0].getOccupiedSquares()[i][0] << " " << this->queenPositions[0].getOccupiedSquares()[i][1] << endl;
      }
      (*/
      restorePiecePos(endPos[0], endPos[1], 6, pieceColour);
      /*
      cout << "black Queens: " << endl;
      for(int i = 0; i < this->queenPositions[0].getSize();i++){
        cout << this->queenPositions[0].getOccupiedSquares()[i][0] << " " << this->queenPositions[0].getOccupiedSquares()[i][1] << endl;
      }
      */
      if(piece != 0){
        piece = piece * 10;
      }
      else{
        piece = 1;
      }
    }
    return piece;
  }
  void applyMoveReverse(array<int,2> initPos,array<int,2> endPos,int pieceType,int piece){
    int ** board = this->boardReprentation;
    //int pieceType = board[initPos[0]][initPos[1]];
    int pieceColour;
    if(pieceType > 16){
      pieceColour = 1;
    }
    else{
      pieceColour = 0;
    }
    this->updatePiecePos(initPos,endPos,pieceType % 8,pieceColour);

    board[endPos[0]][endPos[1]] = board[initPos[0]][initPos[1]];
    if(piece > 8 && piece < 29){
      board[initPos[0]][initPos[1]] = piece;
    }
    else{
      board[initPos[0]][initPos[1]] = 0;
    }

  }
  int make_move(array<int,2> initPos,array<int,2> endPos){
    updateProperties(initPos,endPos);
    updateStacks();
    int** board = this->getBoard();
    //castling

    if((board[initPos[0]][initPos[1]] % 8 == 2) && abs(endPos[1] - initPos[1]) > 1){
      if((endPos[1] - initPos[1]) > 0){
        //kingSide
        if(turnColour == white){
          board[7][5] = board[7][7];
          board[7][7] = 0;
          updatePiecePos({7,7},{7,5}, 5, 1);
          //this->updateWhitePiecePositions({7,7},{7,5});
        }
        else{
          board[0][5] = board[0][7];
          board[0][7] = 0;
          updatePiecePos({0,7},{0,5}, 5, 0);
        }
      }
      //castling queenSide
      else{
        if(turnColour == white){
          board[7][3] = board[7][0];
          board[7][0] = 0;
          updatePiecePos({7,0},{7,3}, 5, 1);
        }
        else{
          board[0][3] = board[0][0];
          board[0][0] = 0;
          updatePiecePos({0,0},{0,3}, 5, 0);
        }
      }
    }
    int piece = this->applyMoveForward(initPos,endPos);
    //castling
    /*
    if(turnColour == white){
      turnColour = black;
    }
    else{
      turnColour = white;
    }
    */
    return piece;
  }
  void unmake_move(array<int,2> initPos,array<int,2> endPos, int piece){
    if(turnColour == white){
      turnColour = black;
    }
    else{
      turnColour = white;
    }
    //castling
    int** board = this->getBoard();

    if((board[initPos[0]][initPos[1]] % 8 == 2) && abs(endPos[1] - initPos[1]) > 1){
      if((endPos[1] - initPos[1]) < 0){
        //kingSide
        if(turnColour == white){
          board[7][7] = board[7][5];
          board[7][5] = 0;
          updatePiecePos({7,5},{7,7}, 5, 1);
        }
        else{
          board[0][7] = board[0][5];
          board[0][5] = 0;
          updatePiecePos({0,5},{0,7}, 5, 0);
        }
      }
      //castling queenSide
      else{
        if(turnColour == white){
          board[7][0] = board[7][3];
          board[7][3] = 0;
          updatePiecePos({7,3},{7,0}, 5, 1);
        }
        else{
          board[0][0] = board[0][3];
          board[0][3] = 0;
          updatePiecePos({0,3},{0,0}, 5, 0);
        }
      }
    }

    //this->applyMoveReverse(initPos,endPos);
    restoreStacks();
    restoreQualities();
    //capture
    int pieceType = board[initPos[0]][initPos[1]];
    if(piece > 8 && piece < 29){
      //this->boardReprentation[initPos[0]][initPos[1]] = piece;
      //int pieceType = piece;
      int pieceColour;
      if(piece > 16){
        pieceColour = 1;
      }
      else{
        pieceColour = 0;
      }
      //cout << "normal" << endl;
      this->restorePiecePos(initPos[0],initPos[1],piece % 8, pieceColour);
    }
    this->applyMoveReverse(initPos,endPos,pieceType,piece);

    // Promototed Capture
    if(piece > 29){
      int pieceColour;
      int currPieceColour;
      if(piece/10 > 16){
        pieceColour = 1;
        currPieceColour = 0;
      }
      else{
        pieceColour = 0;
        currPieceColour = 1;
      }
      this->boardReprentation[initPos[0]][initPos[1]] = piece/10;
      board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] - 5;
      //cout << "promoted capture" << endl;
      this->restorePiecePos(initPos[0],initPos[1],(piece/10) % 8, pieceColour);
      // remove queen at endPos
      this->updatePieceRemoval(initPos,endPos, 6, currPieceColour);
      // add pawn back
      //cout << "promoted capture pawn back" << endl;
      this->restorePiecePos(endPos[0],endPos[1],1, currPieceColour);
      //restorePiecePos(initPos,piece/10);

    }
    //Promotion, no capture
    else if(piece == 1){
      //cout << "promoted No capture" << endl;
      //cout << "pieceType: " << pieceType << endl;
      board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] - 5;
      if(turnColour == white){
        //board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] - 5;
        this->updatePieceRemoval(initPos,endPos, 6, 1);
        this->restorePiecePos(endPos[0],endPos[1],1, 1);
      }
      else{
        this->updatePieceRemoval(initPos,endPos, 6, 0);
        this->restorePiecePos(endPos[0],endPos[1],1, 0);
      }
    }
    //EnPaisant capture
    else if((board[endPos[0]][endPos[1]] % 8 == 1) && abs(endPos[1] - initPos[1]) == 1 && piece == 0){
      //cout << "EP!!! restore" << endl;
      if(turnColour == white){
        this->boardReprentation[initPos[0] + 1][initPos[1]] = 9;
        this->restorePiecePos(initPos[0] + 1,initPos[1],1, 0);
        //cout << "restoring: " << initPos[0] + 1 << " " <<  initPos[1] << endl;
        //restorePiecePos({initPos[0]+1,initPos[1]},9);
      }
      else{
        this->boardReprentation[initPos[0] - 1][initPos[1]] = 17;
        this->restorePiecePos(initPos[0] - 1,initPos[1],1, 1);
        //cout << "restoring: " << initPos[0] - 1 << " " <<  initPos[1] << endl;
        //restorePiecePos({initPos[0]-1,initPos[1]},17);
      }
    }
  }
};

unsigned long long int randomInt(){
    uniform_int_distribution<unsigned long long int>
                                 dist(0, UINT64_MAX);
    return dist(mt);
}

void initTable(){
    int accum = 0;
    for (int i = 0; i<8; i++)
      for (int j = 0; j<8; j++)
        for (int k = 0; k<12; k++)
          for(int l = 0; l<64;l++)
            for(int m = 0;m < 8; m++)
              for(int n = 0; n<2;n++)
                ZobristTable[i][j][k][l][m][n] = randomInt();
                HashTable[accum].accessed = false;
                HashTable[accum].depth = -1;
                HashTable[accum].value = -numeric_limits<int>::max();
                accum++;

}

int getPieceHash(int piece){
  if(piece > 16){
    return piece - 11;
  }
  return piece-9;

}

int getPlayerHash(int piece){
  if(piece > 5){
    return 1;
  }
  return 0;
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
  if(piece > 5){
    return EnPaisantProperties[1];
  }
  return EnPaisantProperties[0];
}

// Computes the hash value of a given board
unsigned long long int computeHash(int** board, Board myBoard){
    unsigned long long int h = 0;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (board[i][j]){
              int pieceHash = getPieceHash(board[i][j]);
              h ^= ZobristTable[i][j][pieceHash][getCastleHash(myBoard)][getEnPaisantHash(pieceHash,myBoard)][getPlayerHash(pieceHash)];
            }
        }
    }
    return h;
}

void bitBoardPrinter(long long int board){
  long long int val;
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      val = ((board & (long long int)pow(2,(i * 8) + j) ? 0 : 1) + 1) % 2;
      cout << " " << val << " ";
    }
    cout << endl;
  }
}

void boardPrinter(int** board){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(board[i][j] == 0){
        cout << "00" << ' ';
      }
      else if(board[i][j] > 15){
        cout << "W";
        if(board[i][j] % 8 == 1){
          cout << "P" << ' ';
        }
        else if(board[i][j] % 8 == 2){
          cout << "K" << ' ';
        }
        else if(board[i][j] % 8 == 3){
          cout << "N" << ' ';
        }
        else if(board[i][j] % 8 == 4){
          cout << "B" << ' ';
        }
        else if(board[i][j] % 8 == 5){
          cout << "R" << ' ';
        }
        else{
          cout << "Q" << ' ';
        }
      }
      else{
        cout << "B";
        if(board[i][j] % 8 == 1){
          cout << "P" << ' ';
        }
        else if(board[i][j] % 8 == 2){
          cout << "K" << ' ';
        }
        else if(board[i][j] % 8 == 3){
          cout << "N" << ' ';
        }
        else if(board[i][j] % 8 == 4){
          cout << "B" << ' ';
        }
        else if(board[i][j] % 8 == 5){
          cout << "R" << ' ';
        }
        else{
          cout << "Q" << ' ';
        }
      }
    }
    cout << endl;
  }
}

int** initBoard(){
  int** board = new int*[8];
  totalMem++;
  for(int i = 0; i < 8; i++){
    board[i] = new int[8];
    totalMem++;
    for(int j = 0; j < 8; j++){
      board[i][j] = 0;
    }
  }
  return board;
}

array<PieceList,6> piecePos(Board myBoard){
  if(turnColour == white){
    return myBoard.getWhitePieces();
  }
  else{
    return myBoard.getBlackPieces();
  }
}

void loadBoardFromFen(string fen,int **board, Board myBoard){
  std::map<char, int> pieceTypeFromChar = {
    { 'p', pawn },
    { 'k', king },
    { 'n', knight },
    { 'r', rook },
    { 'q', queen },
    { 'b', bishop}
  };

  string fenBoard = fen.substr(0, fen.find(" "));
  int file = 0, rank = 0;

  for(char c : fenBoard){
    if(c == '/'){
      file = 0;
      rank+= 1;
    }
    else{
      if(isdigit(c)){
        file += (int) (c - '0');
      }
      else{
        int pieceColour = isupper(c) ? white : black;
        int pieceType = pieceTypeFromChar[tolower(c)];
        board[rank][file] = pieceColour | pieceType;
        pieceColour = (pieceColour / 8) - 1;
        //myBoard.restorePiecePos(rank,file, pieceType,pieceColour);
        //updatePiecePos(rank,file,myBoard,pieceType,pieceColour);

        /*
        if(pieceColour == white){
          whitePiecePos.push_back({rank,file});
        }
        else{
          blackPiecePos.push_back({rank,file});
        }
        */
        file++;
      }
    }
  }
}

std::vector<array<int,2>> knightTurn(int rank, int file, int** board){
  int currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  std::vector<array<int,2>> possTurns;
  //Up-Right
  if(file != 7 && rank > 1 && ((board[rank-2][file+1] ^ turnColour) > board[rank-2][file+1])){
    possTurns.push_back({rank-2,file+1});
  }
  //Right-Up
  if(file < 6 && rank != 0 && ((board[rank-1][file+2] ^ turnColour) > board[rank-1][file+2])){
    possTurns.push_back({rank-1,file+2});
  }
  //Right-Down
  if(file < 6 && rank != 7 && ((board[rank+1][file+2] ^ turnColour) > board[rank+1][file+2])){
    possTurns.push_back({rank+1,file+2});
  }
  //Down-Right
  if(file != 7 && rank < 6 && (((int)board[rank+2][file+1] ^ turnColour) > (int)board[rank+2][file+1])){
    possTurns.push_back({rank+2,file+1});
  }
  //Down-Left
  if(file != 0 && rank < 6 && (((int)board[rank+2][file-1] ^ turnColour) > (int)board[rank+2][file-1])){
    possTurns.push_back({rank+2,file-1});
  }
  //Left-Down
  if(file > 1 && rank != 7 && (((int)board[rank+1][file-2] ^ turnColour) > (int)board[rank+1][file-2])){
    possTurns.push_back({rank+1,file-2});
  }
  //Left-Up
  if(file > 1 && rank != 0 && (((int)board[rank-1][file-2] ^ turnColour )> (int)board[rank-1][file-2])){
    possTurns.push_back({rank-1,file-2});
  }
  //Up-Left
  if(file !=0 && rank > 1 && (((int)board[rank-2][file-1] ^ turnColour) > (int)board[rank-2][file-1])){
    possTurns.push_back({rank-2,file-1});
  }
  turnColour = currTurnColour;
  return possTurns;
}
/*
MoveList pawnMoves(int rank, int file, int** board, Board myBoard){
  MoveList possMoves;
  possMoves.size = 0;
  possMoves.moves = new array<int,4>[4];
  totalMem++;
  int accum = 0;
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
    if(rank != 0 && file != 7 && ((board[rank-1][file+1] != 0 && (board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1]) || (rank == 3 && (whiteEnPaisant - file) == 1))){
      possMoves.moves[accum] = {rank,file,rank-1,file+1};
      accum++;
    }
    //Up-left take
    if(rank != 0 && file != 0 && ((board[rank-1][file-1] != 0 && (board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1]) || (rank == 3 && (file - whiteEnPaisant) == 1))){
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
    if(rank != 7 && file != 7 && ((board[rank+1][file+1] != 0 && (board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1]) || (rank == 4 && ((blackEnPaisant - file) == 1)))){
      possMoves.moves[accum] = {rank,file,rank+1,file+1};
      accum++;
    }
    //Down-left take
    if(rank != 7 && file != 0 && ((board[rank+1][file-1] != 0 && (board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1]) || (rank == 4 && ((file - blackEnPaisant) == 1)))){
      possMoves.moves[accum] = {rank,file,rank+1,file-1};
      accum++;
    }
  }
  turnColour = currTurnColour;
  possMoves.size = accum;
  return possMoves;
}
MoveList knightMoves(int rank, int file, int** board){
  MoveList possMoves;
  possMoves.size = 0;
  possMoves.moves = new array<int,4>[8];
  totalMem++;
  int accum = 0;
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
  possMoves.size = accum;
  turnColour = currTurnColour;
  return possMoves;
}
MoveList bishopMoves(int rank, int file, int** board){
  MoveList possMoves;
  possMoves.size = 0;
  possMoves.moves = new array<int,4>[13];
  totalMem++;
  int accum = 0;
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
  turnColour = currTurnColour;
  possMoves.size = accum;
  return possMoves;
}
MoveList rookMoves(int rank, int file, int** board){
  MoveList possMoves;
  possMoves.size = 0;
  possMoves.moves = new array<int,4>[14];
  totalMem++;
  int accum = 0;
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
  possMoves.size = accum;
  turnColour = currTurnColour;
  return possMoves;
}
MoveList queenMoves(int rank, int file, int** board){
  MoveList possMoves;
  possMoves.size = 0;
  possMoves.moves = new array<int,4>[27];
  totalMem++;
  int accum = 0;
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
  possMoves.size = accum;
  turnColour = currTurnColour;
  return possMoves;
}
*/
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
    if(rank != 0 && file != 7 && ((board[rank-1][file+1] != 0 && (board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1]) || (rank == 3 && (whiteEnPaisant - file) == 1))){
      possMoves.moves[accum] = {rank,file,rank-1,file+1};
      accum++;
    }
    //Up-left take
    if(rank != 0 && file != 0 && ((board[rank-1][file-1] != 0 && (board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1]) || (rank == 3 && (file - whiteEnPaisant) == 1))){
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
    if(rank != 7 && file != 7 && ((board[rank+1][file+1] != 0 && (board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1]) || (rank == 4 && ((blackEnPaisant - file) == 1)))){
      possMoves.moves[accum] = {rank,file,rank+1,file+1};
      accum++;
    }
    //Down-left take
    if(rank != 7 && file != 0 && ((board[rank+1][file-1] != 0 && (board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1]) || (rank == 4 && ((file - blackEnPaisant) == 1)))){
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
  turnColour = currTurnColour;

}
void rookMoves(int rank, int file, int** board, int &accum, MoveList possMoves){
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
  std::vector<array<int,2>> possTurns;
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


void pawnMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int checkers, long long int checkRay, bool slidingPiece){
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
      squareVal = (long long int)pow(2,(rank-1) * 8 + file);
      if(slidingPiece && ((squareVal & checkRay) == squareVal)){
        possMoves.moves[accum] = {rank,file,rank-1,file};
        accum++;
      }
      if(rank == 6 && board[rank-2][file] == 0){
        squareVal = (long long int)pow(2,(rank-2) * 8 + file);
        if(slidingPiece && ((squareVal & checkRay) == squareVal)){
          possMoves.moves[accum] = {rank,file,rank-2,file};
          accum++;
        }
      }
    }
    //Up-right take
    squareVal = (long long int)pow(2,(rank-1) * 8 + file+1);
    enPaissantSquare = (long long int)pow(2,(rank) * 8 + file+1);
    if(rank != 0 && file != 7 && (((enPaissantSquare & checkers) == checkers) || ((rank == 3 && (whiteEnPaisant - file) == 1)) && (slidingPiece && ((squareVal & checkRay) == squareVal)))){
      possMoves.moves[accum] = {rank,file,rank-1,file+1};
      accum++;
    }
    //Up-left take
    squareVal = (long long int)pow(2,(rank-1) * 8 + file-1);
    enPaissantSquare = (long long int)pow(2,(rank) * 8 + file-1);
    if(rank != 0 && file != 0 && (((enPaissantSquare & checkers) == checkers) || ((rank == 3 && (file - whiteEnPaisant) == 1)) && (slidingPiece && ((squareVal & checkRay) == squareVal)))){
      possMoves.moves[accum] = {rank,file,rank-1,file-1};
      accum++;
    }
  }
  else{
    if(board[rank+1][file] == 0){
      squareVal = (long long int)pow(2,(rank+1) * 8 + file);
      if(slidingPiece && ((squareVal & checkRay) == squareVal)){
        possMoves.moves[accum] = {rank,file,rank+1,file};
        accum++;
      }
      if(rank == 1 && board[rank+2][file] == 0){
        squareVal = (long long int)pow(2,(rank+2) * 8 + file);
        if(slidingPiece && ((squareVal & checkRay) == squareVal)){
          possMoves.moves[accum] = {rank,file,rank+2,file};
          accum++;
        }
      }
    }
    squareVal = (long long int)pow(2,(rank+1) * 8 + file+1);
    enPaissantSquare = (long long int)pow(2,(rank) * 8 + file+1);
    if(rank != 7 && file != 7 && (((enPaissantSquare & checkers) == checkers) || ((rank == 4 && (blackEnPaisant - file) == 1)) && (slidingPiece && ((squareVal & checkRay) == squareVal)))){
      possMoves.moves[accum] = {rank,file,rank+1,file+1};
      accum++;
    }
    //Up-left take
    squareVal = (long long int)pow(2,(rank+1) * 8 + file-1);
    enPaissantSquare = (long long int)pow(2,(rank) * 8 + file-1);
    if(rank != 7 && file != 0 && (((enPaissantSquare & checkers) == checkers) || ((rank == 4 && (file - blackEnPaisant) == 1)) && (slidingPiece && ((squareVal & checkRay) == squareVal)))){
      possMoves.moves[accum] = {rank,file,rank+1,file-1};
      accum++;
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
  squareVal = (long long int)pow(2,(rank-2) * 8 + file+1);
  if(file != 7 && rank > 1 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank-2,file+1};
    accum++;
    //possTurns.push_back({rank-2,file+1});
  }
  //Right-Up
  squareVal = (long long int)pow(2,(rank-1) * 8 + file+2);
  if(file < 6 && rank != 0 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank-1,file+2};
    accum++;
  }
  //Right-Down
  squareVal = (long long int)pow(2,(rank+1) * 8 + file+2);
  if(file < 6 && rank != 7 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank+1,file+2};
    accum++;
  }
  //Down-Right
  squareVal = (long long int)pow(2,(rank+2) * 8 + file+1);
  if(file != 7 && rank < 6 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank+2,file+1};
    accum++;
  }
  //Down-Left
  squareVal = (long long int)pow(2,(rank+2) * 8 + file-1);
  if(file != 0 && rank < 6 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank+2,file-1};
    accum++;
  }
  //Left-Down
  squareVal = (long long int)pow(2,(rank+1) * 8 + file-2);
  if(file > 1 && rank != 7 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank+1,file-2};
    accum++;
  }
  //Left-Up
  if(file > 1 && rank != 0 && (((squareVal & checkers) == checkers) || (slidingPiece && ((squareVal & checkRay) == squareVal)))){
    possMoves.moves[accum] = {rank,file,rank-1,file-2};
    accum++;
  }
  //Up-Left
  squareVal = (long long int)pow(2,(rank-2) * 8 + file-1);
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
    squareVal = (long long int)pow(2,(rank-1) * 8 + file+1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
      accum++;
    }
    rank--;
    file++;
  }
  squareVal = (long long int)pow(2,(rank-1) * 8 + file+1);
  if(rank != 0 && file != 7  && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file+1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
      accum++;
    }
    rank++;
    file++;
  }
  squareVal = (long long int)pow(2,(rank+1) * 8 + file+1);
  if(rank != 7 && file != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file-1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
      accum++;
    }
    rank++;
    file--;
  }
  squareVal = (long long int)pow(2,(rank+1) * 8 + file-1);
  if(rank != 7 && file != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file-1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    squareVal = (long long int)pow(2,(rank-1) * 8 + file-1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file-1};
      accum++;
    }
    rank--;
    file--;
  }
  squareVal = (long long int)pow(2,(rank-1) * 8 + file-1);
  cout << "squareVal" << endl;
  bitBoardPrinter(squareVal);
  cout << "checkers" << endl;
  bitBoardPrinter(checkers);
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
    squareVal = (long long int)pow(2,(rank-1) * 8 + file);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
      accum++;
    }
    rank--;
  }
  squareVal = (long long int)pow(2,(rank-1) * 8 + file);
  if(rank != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank-1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
      accum++;
    }
    rank++;
  }
  squareVal = (long long int)pow(2,(rank+1) * 8 + file);
  if(rank != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank+1,file};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    squareVal = (long long int)pow(2,(rank) * 8 + file+1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
      accum++;
    }
    file++;
  }
  squareVal = (long long int)pow(2,(rank) * 8 + file+1);
  if(file != 7 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file+1};
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    squareVal = (long long int)pow(2,(rank) * 8 + file-1);
    if(slidingPiece && ((squareVal & checkRay) == squareVal)){
      possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
      accum++;
    }
    file--;
  }
  squareVal = (long long int)pow(2,(rank) * 8 + file-1);
  if(file != 0 && ((squareVal & checkers) == checkers)){
    possMoves.moves[accum] = {actualRank,actualFile,rank,file-1};
    accum++;
  }
  turnColour = currTurnColour;
}
void queenMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves){
  //cout << "in queen Moves" << endl;

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
void kingMovesCheck(int rank, int file, int** board, int &accum, MoveList possMoves, long long int bitBoard){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  //Up
  if(rank != 0  && !((long long int)pow(2,(rank-1) * 8 + file) & bitBoard) && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possMoves.moves[accum] = {rank,file,rank-1,file};
    accum++;
  }
  //Down
  if(rank != 7  && !((long long int)pow(2,(rank+1) * 8 + file) & bitBoard) && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possMoves.moves[accum] = {rank,file,rank+1,file};
    accum++;
  }
  //Right

  if(file != 7  && !((long long int)pow(2,(rank) * 8 + file+1) & bitBoard) &&  ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possMoves.moves[accum] = {rank,file,rank,file+1};
    accum++;
  }
  //Left
  if(file != 0  && !((long long int)pow(2,(rank) * 8 + file-1) & bitBoard) && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possMoves.moves[accum] = {rank,file,rank,file-1};
    accum++;
  }
  //Up-Right
  if(rank != 0 && file != 7 && !((long long int)pow(2,(rank-1) * 8 + file+1) & bitBoard) && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possMoves.moves[accum] = {rank,file,rank-1,file+1};
    accum++;
  }
  //Down-Right
  if(rank != 7 && file != 7 && !((long long int)pow(2,(rank+1) * 8 + file+1) & bitBoard) && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possMoves.moves[accum] = {rank,file,rank+1,file+1};
    accum++;
  }
  //Down-Left
  if(rank != 7 && file != 0 && !((long long int)pow(2,(rank+1) * 8 + file-1) & bitBoard) && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possMoves.moves[accum] = {rank,file,rank+1,file-1};
    accum++;
  }
  //Up-Left
  if(rank != 0 && file != 0 && !((long long int)pow(2,(rank-1) * 8 + file-1) & bitBoard) && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possMoves.moves[accum] = {rank,file,rank-1,file-1};
    accum++;
  }

  turnColour = currTurnColour;
}


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
      long long int squareVal = (long long int)pow(2,(rank-1) * 8 + file-1);
      bBoard.attackingSquares |= squareVal;
      bBoard.kingDangerSquares |= squareVal;
      if((squareVal & kingPos) == kingPos){
        squareVal = (long long int)pow(2,(rank) * 8 + file);
        bBoard.checkers |= squareVal;
        bBoard.numCheckers++;
      }
    }
    if(file < 7){
      long long int squareVal = (long long int)pow(2,(rank-1) * 8 + file+1);
      bBoard.attackingSquares |= squareVal;
      bBoard.kingDangerSquares |= squareVal;
      if((squareVal & kingPos) == kingPos){
        squareVal = (long long int)pow(2,(rank) * 8 + file);
        bBoard.checkers |= squareVal;
        bBoard.numCheckers++;
      }
    }
  }
  else{
    if(file > 0){
      long long int squareVal = (long long int)pow(2,(rank+1) * 8 + file-1);
      bBoard.attackingSquares |= squareVal;
      bBoard.kingDangerSquares |= squareVal;
      if((squareVal & kingPos) == kingPos){
        squareVal = (long long int)pow(2,(rank) * 8 + file);
        bBoard.checkers |= squareVal;
        bBoard.numCheckers++;
      }
    }
    if(file < 7){
      long long int squareVal = (long long int)pow(2,(rank+1) * 8 + file+1);
      bBoard.attackingSquares |= squareVal;
      bBoard.kingDangerSquares |= squareVal;
      if((squareVal & kingPos) == kingPos){
        squareVal = (long long int)pow(2,(rank) * 8 + file);
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
  if(file != 7 && rank > 1 && ((board[rank-2][file+1] ^ turnColour) > board[rank-2][file+1])){
    long long int squareVal = (long long int)pow(2,(rank-2) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(squareVal & kingPos == kingPos){
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
    }
  }
  //Right-Up
  if(file < 6 && rank != 0 && ((board[rank-1][file+2] ^ turnColour) > board[rank-1][file+2])){
    long long int squareVal = (long long int)pow(2,(rank-1) * 8 + file+2);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(squareVal & kingPos == kingPos){
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
    }
  }
  //Right-Down
  if(file < 6 && rank != 7 && ((board[rank+1][file+2] ^ turnColour) > board[rank+1][file+2])){
    long long int squareVal = (long long int)pow(2,(rank+1) * 8 + file+2);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(squareVal & kingPos == kingPos){
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
    }
  }
  //Down-Right
  if(file != 7 && rank < 6 && (((int)board[rank+2][file+1] ^ turnColour) > (int)board[rank+2][file+1])){
    long long int squareVal = (long long int)pow(2,(rank+2) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(squareVal & kingPos == kingPos){
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
    }
  }
  //Down-Left
  if(file != 0 && rank < 6 && (((int)board[rank+2][file-1] ^ turnColour) > (int)board[rank+2][file-1])){
    long long int squareVal = (long long int)pow(2,(rank+2) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(squareVal & kingPos == kingPos){
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
    }
  }
  //Left-Down
  if(file > 1 && rank != 7 && (((int)board[rank+1][file-2] ^ turnColour) > (int)board[rank+1][file-2])){
    long long int squareVal = (long long int)pow(2,(rank+1) * 8 + file-2);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(squareVal & kingPos == kingPos){
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
    }
  }
  //Left-Up
  if(file > 1 && rank != 0 && (((int)board[rank-1][file-2] ^ turnColour )> (int)board[rank-1][file-2])){
    long long int squareVal = (long long int)pow(2,(rank-1) * 8 + file-2);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(squareVal & kingPos == kingPos){
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
    }
  }
  //Up-Left
  if(file !=0 && rank > 1 && (((int)board[rank-2][file-1] ^ turnColour) > (int)board[rank-2][file-1])){
    long long int squareVal = (long long int)pow(2,(rank-2) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(squareVal & kingPos == kingPos){
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
    }
  }
  turnColour = currTurnColour;
}
void bishopMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard){
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
    squareVal = (long long int)pow(2,(rank-1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    squareVal = (long long int)pow(2,(rank-1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = (long long int)pow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 1 && file != 6){
        bBoard.kingDangerSquares |= (long long int)pow(2,(rank-2) * 8 + file+2);
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
    squareVal = (long long int)pow(2,(rank+1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = (long long int)pow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 6 && file != 6){
        bBoard.kingDangerSquares |= (long long int)pow(2,(rank+2) * 8 + file+2);
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
    squareVal = (long long int)pow(2,(rank+1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = (long long int)pow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 6 && file != 1){
        bBoard.kingDangerSquares |= (long long int)pow(2,(rank+2) * 8 + file-2);
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
    squareVal = (long long int)pow(2,(rank-1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    squareVal = (long long int)pow(2,(rank-1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = (long long int)pow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 1 && file != 1 && board[rank-1][file-1] % 8 == 2){
        bBoard.kingDangerSquares |= (long long int)pow(2,(rank-2) * 8 + file-2);
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  turnColour = currTurnColour;

}
void rookMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard){
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
    squareVal = (long long int)pow(2,(rank-1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank--;
  }
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    squareVal = (long long int)pow(2,(rank-1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = (long long int)pow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 1){
        bBoard.kingDangerSquares |= (long long int)pow(2,(rank-2) * 8 + file);
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
    squareVal = (long long int)pow(2,(rank+1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    rank++;
  }


  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = (long long int)pow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(rank != 6){
        bBoard.kingDangerSquares |= (long long int)pow(2,(rank+2) * 8 + file);
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
    squareVal = (long long int)pow(2,(rank) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    file++;
  }
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    squareVal = (long long int)pow(2,(rank) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = (long long int)pow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(file != 6 && board[rank][file+1] % 8 == 2){
        bBoard.kingDangerSquares |= (long long int)pow(2,(rank) * 8 + file+2);
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
    squareVal = (long long int)pow(2,(rank) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if(!bBoard.numCheckers){
      bBoard.checkRay |= squareVal;
    }

    file--;
  }
  if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    squareVal = (long long int)pow(2,(rank) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
    if((squareVal & kingPos) == kingPos){
      squareVal = (long long int)pow(2,(actualRank) * 8 + actualFile);
      bBoard.checkers |= squareVal;
      bBoard.numCheckers++;
      if(file != 1 && board[rank][file-1] % 8 == 2){
        bBoard.kingDangerSquares |= (long long int)pow(2,(rank) * 8 + file-2);
      }
    }
  }
  if(!bBoard.numCheckers){
    bBoard.checkRay = 0;
  }
  turnColour = currTurnColour;
}
void queenMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard){
  rookMoves(rank,file,board,kingPos,bBoard);
  bishopMoves(rank,file,board,kingPos,bBoard);
}
void kingMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  long long int squareVal;
  //Up
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    squareVal = (long long int)pow(2,(rank-1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Down
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Right
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    squareVal = (long long int)pow(2,(rank) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Left
  if(rank != 7  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    squareVal = (long long int)pow(2,(rank) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Up-Right
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    squareVal = (long long int)pow(2,(rank-1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Down-Right
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file+1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Down-Left
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    squareVal = (long long int)pow(2,(rank+1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  //Up-Left
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    squareVal = (long long int)pow(2,(rank-1) * 8 + file-1);
    bBoard.attackingSquares |= squareVal;
    bBoard.kingDangerSquares |= squareVal;
  }
  turnColour = currTurnColour;
}

std::vector<array<int,2>> bishopTurn(int rank, int file, int** board){
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
  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    possTurns.push_back({rank-1,file+1});
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possTurns.push_back({rank-1,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    possTurns.push_back({rank+1,file+1});
    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possTurns.push_back({rank+1,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    possTurns.push_back({rank+1,file-1});
    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possTurns.push_back({rank+1,file-1});
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    possTurns.push_back({rank-1,file-1});
    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possTurns.push_back({rank-1,file-1});
  }
  file = actualFile;
  rank = actualRank;
  turnColour = currTurnColour;
  return possTurns;
}
std::vector<array<int,2>> rookTurn(int rank, int file, int** board){
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
    possTurns.push_back({rank-1,file});
    rank--;
  }
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possTurns.push_back({rank-1,file});
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    possTurns.push_back({rank+1,file});
    rank++;
  }
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possTurns.push_back({rank+1,file});
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    possTurns.push_back({rank,file+1});
    file++;
  }
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possTurns.push_back({rank,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    possTurns.push_back({rank,file-1});
    file--;
  }
  if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possTurns.push_back({rank,file-1});
  }
  file = actualFile;
  rank = actualRank;
  turnColour = currTurnColour;
  return possTurns;
}
std::vector<array<int,2>> queenTurn(int rank, int file, int** board){
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

  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    possTurns.push_back({rank-1,file+1});
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possTurns.push_back({rank-1,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    possTurns.push_back({rank+1,file+1});
    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possTurns.push_back({rank+1,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    possTurns.push_back({rank+1,file-1});
    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possTurns.push_back({rank+1,file-1});
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    possTurns.push_back({rank-1,file-1});
    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possTurns.push_back({rank-1,file-1});
  }
  file = actualFile;
  rank = actualRank;
  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    possTurns.push_back({rank-1,file});
    rank--;
  }
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possTurns.push_back({rank-1,file});
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    possTurns.push_back({rank+1,file});
    rank++;
  }
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possTurns.push_back({rank+1,file});
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    possTurns.push_back({rank,file+1});
    file++;
  }
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possTurns.push_back({rank,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    possTurns.push_back({rank,file-1});
    file--;
  }
  if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possTurns.push_back({rank,file-1});
  }
  file = actualFile;
  rank = actualRank;
  turnColour = currTurnColour;
  return possTurns;
}
std::vector<array<int,2>> pawnTurn(int rank, int file, int** board, Board myBoard){
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
  std::vector<array<int,2>> possTurns;

  if(turnColour==white){
    if(board[rank-1][file] == 0){
      possTurns.push_back({rank-1,file});
      if(rank == 6 && board[rank-2][file] == 0){
        possTurns.push_back({rank-2,file});
      }
    }
    //Up-right take
    if(rank != 0 && file != 7 && ((board[rank-1][file+1] != 0 && (board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1]) || (rank == 3 && (whiteEnPaisant - file) == 1))){
      possTurns.push_back({rank-1,file+1});
    }
    //Up-left take
    if(rank != 0 && file != 0 && ((board[rank-1][file-1] != 0 && (board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1]) || (rank == 3 && (file - whiteEnPaisant) == 1))){
      possTurns.push_back({rank-1,file-1});
    }
  }
  else{
    if(board[rank+1][file] == 0){
      possTurns.push_back({rank+1,file});
      if(rank == 1 && board[rank+2][file] == 0){
        possTurns.push_back({rank+2,file});
      }
    }
    //Down-right take
    if(rank != 7 && file != 7 && ((board[rank+1][file+1] != 0 && (board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1]) || (rank == 4 && ((blackEnPaisant - file) == 1)))){
      possTurns.push_back({rank+1,file+1});
    }
    //Down-left take
    if(rank != 7 && file != 0 && ((board[rank+1][file-1] != 0 && (board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1]) || (rank == 4 && ((file - blackEnPaisant) == 1)))){
      possTurns.push_back({rank+1,file-1});
    }
  }
  turnColour = currTurnColour;
  return possTurns;
}
std::vector<array<int,2>> pawnTurnOnlyAttack(int rank, int file, int** board){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  std::vector<array<int,2>> possTurns;
  if(turnColour == white && rank != 1){
    return possTurns;
  }
  if(turnColour == black && rank != 6){
    return possTurns;
  }
  if(turnColour == white){
    if(file > 0){
      possTurns.push_back({rank-1,file-1});
    }
    if(file < 7){
      possTurns.push_back({rank-1,file+1});
    }
  }
  else{
    if(file > 0){
      possTurns.push_back({rank+1,file-1});
    }
    if(file < 7){
      possTurns.push_back({rank+1,file+1});
    }
  }


  /*
  if(turnColour==white){
    //Up-right take
    if(rank != 0 && file != 7 && board[rank-1][file+1] != 0 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
      possTurns.push_back({rank-1,file+1});
    }
    //Up-left take
    if(rank != 0 && file != 0 && board[rank-1][file-1] != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
      possTurns.push_back({rank-1,file-1});
    }
  }
  else{
    //Down-right take
    if(rank != 7 && file != 7 && board[rank+1][file+1] != 0 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
      possTurns.push_back({rank+1,file+1});
    }
    //Down-left take
    if(rank != 7 && file != 0 && board[rank+1][file-1] != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
      possTurns.push_back({rank+1,file-1});
    }
  }
  */
  turnColour = currTurnColour;

  return possTurns;
}
std::vector<array<int,2>> kingTurnMinusCastle(int rank, int file, int** board){
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  std::vector<array<int,2>> possTurns;
  //Up
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possTurns.push_back({rank-1,file});
  }
  //Down
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possTurns.push_back({rank+1,file});
  }
  //Right
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possTurns.push_back({rank,file+1});
  }
  //Left
  if(rank != 7  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possTurns.push_back({rank,file-1});
  }
  //Up-Right
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possTurns.push_back({rank-1,file+1});
  }
  //Down-Right
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possTurns.push_back({rank+1,file+1});
  }
  //Down-Left
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possTurns.push_back({rank+1,file-1});
  }
  //Up-Left
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possTurns.push_back({rank-1,file-1});
  }
  turnColour = currTurnColour;
  return possTurns;
}

std::vector<array<int,2>> pieceMovesForChecks(int rank, int file, int** board){
  if(board[rank][file] % 8 == 1){
    return pawnTurnOnlyAttack(rank,file,board);
  }
  else if(board[rank][file] % 8 == 2){
    return kingTurnMinusCastle(rank, file, board);
  }
  else if(board[rank][file] % 8 == 3){
    return knightTurn(rank,file,board);
  }
  else if(board[rank][file] % 8 == 4){
    return bishopTurn(rank,file,board);
  }
  else if(board[rank][file] % 8 == 5){
    return rookTurn(rank,file,board);
  }
  else{
    return queenTurn(rank,file,board);
  }
}

bool whiteCastleKingSide(int** board, Board myBoard){
  if(board[7][5] != 0 || board[7][6] != 0 || board[7][7] != 21){
    return false;
  }
  turnColour = black;
  array<PieceList,6> oppPiecePos = piecePos(myBoard);
  //std::vector<array<int,2>> oppPiecePos = piecePos(myBoard);
  for(int i = 0; i < 6; i++){
    array<int,2>* oppPieceList = oppPiecePos[i].getOccupiedSquares();
    for(int k = 0; k < oppPiecePos[i].getSize();k++){
      std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPieceList[k][0],oppPieceList[k][1],board);
      for(int j = 0; j < oppPieceMoves.size(); j++){
        if(oppPieceMoves[j][0] == 7 && (oppPieceMoves[j][1] == 5 || oppPieceMoves[j][1] == 6 || oppPieceMoves[j][1] == 4)){
          turnColour = white;
          return false;
        }
      }
    }
  }
  turnColour = white;
  return true;
}
bool whiteCastleQueenSide(int** board, Board myBoard){
  if(board[7][1] != 0 || board[7][2] != 0 || board[7][3] != 0 || board[7][0] != 21){
    return false;
  }
  turnColour = black;
  array<PieceList,6> oppPiecePos = piecePos(myBoard);
  for(int i = 0;i < 6; i++){
    array<int,2>* oppPieceList = oppPiecePos[i].getOccupiedSquares();
    for(int k = 0; k < oppPiecePos[i].getSize(); k++){
      std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPieceList[k][0],oppPieceList[k][1],board);
      for(int j = 0;j < oppPieceMoves.size(); j++){
        if(oppPieceMoves[j][0] == 7 && (oppPieceMoves[j][1] == 2 || oppPieceMoves[j][1] == 3 || oppPieceMoves[j][1] == 4)){
          turnColour = white;
          return false;
        }
      }
    }
  }
  turnColour = white;
  return true;
}
bool blackCastleKingSide(int** board, Board myBoard){
  if(board[0][5] != 0 || board[0][6] != 0 || board[0][7] != 13){
    return false;
  }
  turnColour = white;
  array<PieceList,6> oppPiecePos = piecePos(myBoard);
  for(int k = 0; k < 6; k++){
    array<int,2>* oppPieceList = oppPiecePos[k].getOccupiedSquares();
    for(int i = 0; i < oppPiecePos[k].getSize(); i++){
      std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPieceList[i][0],oppPieceList[i][1],board);
      for(int j = 0;j < oppPieceMoves.size(); j++){
        if(oppPieceMoves[j][0] == 0 && (oppPieceMoves[j][1] == 4 || oppPieceMoves[j][1] == 5 || oppPieceMoves[j][1] == 6)){
          turnColour = black;
          return false;
        }
      }
    }
  }
  turnColour = black;
  return true;
}
bool blackCastleQueenSide(int** board, Board myBoard){
  if(board[0][1] != 0 || board[0][2] != 0 || board[0][3] != 0 || board[0][0] != 13){
    return false;
  }
  turnColour = white;
  array<PieceList,6> oppPiecePos = piecePos(myBoard);
  for(int k = 0; k < 6; k++){
    array<int,2>* oppPieceList = oppPiecePos[k].getOccupiedSquares();
    for(int i = 0; i < oppPiecePos[k].getSize(); i++){
      std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPieceList[k][0],oppPieceList[k][1],board);
      for(int j = 0;j < oppPieceMoves.size(); j++){
        if(oppPieceMoves[j][0] == 0 && (oppPieceMoves[j][1] == 2 || oppPieceMoves[j][1] == 3 || oppPieceMoves[j][1] == 4)){
          turnColour = black;
          return false;
        }
      }
    }
  }
  turnColour = black;
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

std::vector<array<int,2>> kingTurn(int rank, int file, int** board, Board myBoard){
  updateCastlePropertiesStack();
  std::vector<array<int,2>> possTurns;
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }

  //Up
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possTurns.push_back({rank-1,file});
  }
  //Down
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possTurns.push_back({rank+1,file});
  }
  //Right
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possTurns.push_back({rank,file+1});
  }
  //Left
  if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possTurns.push_back({rank,file-1});
  }
  //Up-Right
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possTurns.push_back({rank-1,file+1});
  }
  //Down-Right
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possTurns.push_back({rank+1,file+1});
  }
  //Down-Left
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possTurns.push_back({rank+1,file-1});
  }
  //Up-Left
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possTurns.push_back({rank-1,file-1});
  }
  //castling
  if(turnColour == white){
    if(!whiteKingMoved && !whiteKingRookMoved && whiteCastleKingSide(board, myBoard)){
      possTurns.push_back({rank, file+2});
    }
    if(!whiteKingMoved && !whiteQueenRookMoved && whiteCastleQueenSide(board, myBoard)){
      possTurns.push_back({rank, file-2});
    }
  }
  else{
    if(!blackKingMoved && !blackKingRookMoved && blackCastleKingSide(board, myBoard)){
      possTurns.push_back({rank, file+2});
    }
    if(!blackKingMoved && !blackQueenRookMoved && blackCastleQueenSide(board, myBoard)){
      possTurns.push_back({rank, file-2});
    }
  }
  turnColour = currTurnColour;
  return possTurns;
}

void kingMoves(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves){
  updateCastlePropertiesStack();
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }

  //Up
  if(rank != 0  && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    possMoves.moves[accum] = {rank,file,rank-1,file};
    accum++;
  }
  //Down
  if(rank != 7  && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    possMoves.moves[accum] = {rank,file,rank+1,file};
    accum++;
  }
  //Right
  if(file != 7  && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    possMoves.moves[accum] = {rank,file,rank,file+1};
    accum++;
  }
  //Left
  if(file != 0  && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    possMoves.moves[accum] = {rank,file,rank,file-1};
    accum++;
  }
  //Up-Right
  if(rank != 0 && file != 7 && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    possMoves.moves[accum] = {rank,file,rank-1,file+1};
    accum++;
  }
  //Down-Right
  if(rank != 7 && file != 7 && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    possMoves.moves[accum] = {rank,file,rank+1,file+1};
    accum++;
  }
  //Down-Left
  if(rank != 7 && file != 0 && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    possMoves.moves[accum] = {rank,file,rank+1,file-1};
    accum++;
  }
  //Up-Left
  if(rank != 0 && file != 0 && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    possMoves.moves[accum] = {rank,file,rank-1,file-1};
    accum++;
  }
  //castling
  if(turnColour == white){
    if(!whiteKingMoved && !whiteKingRookMoved && whiteCastleKingSide(board, myBoard)){
      possMoves.moves[accum] = {rank,file,rank,file+2};
      accum++;
    }
    if(!whiteKingMoved && !whiteQueenRookMoved && whiteCastleQueenSide(board, myBoard)){
      possMoves.moves[accum] = {rank,file,rank,file-2};
      accum++;
    }
  }
  else{
    if(!blackKingMoved && !blackKingRookMoved && blackCastleKingSide(board, myBoard)){
      possMoves.moves[accum] = {rank,file,rank,file+2};
      accum++;
    }
    if(!blackKingMoved && !blackQueenRookMoved && blackCastleQueenSide(board, myBoard)){
      possMoves.moves[accum] = {rank,file,rank,file-2};
      accum++;
    }
  }
  turnColour = currTurnColour;
}

void pieceMove(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves){
  if(board[rank][file] % 8 == 1){
    pawnMoves(rank,file,board,myBoard,accum,possMoves);
  }
  else if(board[rank][file] % 8 == 2){
    kingMoves(rank,file,board,myBoard,accum,possMoves);
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
    cout << "pieceMoves" << endl;
    sleep(99999);
  }
}
/*
MoveList pieceMoves(int rank, int file, int** board, Board myBoard, int &accum, MoveList possMoves){
  if(board[rank][file] % 8 == 1){
    return pawnMoves(rank,file,board,myBoard);
  }
  else if(board[rank][file] % 8 == 2){
    return kingMoves(rank,file,board,myBoard);
  }
  else if(board[rank][file] % 8 == 3){
    return knightMoves(rank,file,board);
  }
  else if(board[rank][file] % 8 == 4){
    return bishopMoves(rank,file,board);
  }
  else if(board[rank][file] % 8 == 5){
    return rookMoves(rank,file,board);
  }
  else if(board[rank][file] % 8 == 6){
    return queenMoves(rank,file,board,accum,possMoves);
  }
  else{
    cout << "pieceMoves" << endl;
    sleep(99999);
    return queenMoves(rank,file,board);
  }
}

MoveList pieceMoves(int rank, int file, int** board, Board myBoard){
  if(board[rank][file] % 8 == 1){
    return pawnMoves(rank,file,board,myBoard);
  }
  else if(board[rank][file] % 8 == 2){
    return kingMoves(rank,file,board,myBoard);
  }
  else if(board[rank][file] % 8 == 3){
    return knightMoves(rank,file,board);
  }
  else if(board[rank][file] % 8 == 4){
    return bishopMoves(rank,file,board);
  }
  else if(board[rank][file] % 8 == 5){
    return rookMoves(rank,file,board);
  }
  else if(board[rank][file] % 8 == 6){
    return queenMoves(rank,file,board);
  }
  else{
    cout << "pieceMoves" << endl;
    sleep(99999);
    return queenMoves(rank,file,board);
  }
}
*/
/*
std::vector<array<int,2>> pieceMoves(int rank, int file, int** board, Board myBoard){
  if(board[rank][file] % 8 == 1){
    return pawnTurn(rank,file,board,myBoard);
  }
  else if(board[rank][file] % 8 == 2){
    return kingTurn(rank,file,board,myBoard);
  }
  else if(board[rank][file] % 8 == 3){
    return knightTurn(rank,file,board);
  }
  else if(board[rank][file] % 8 == 4){
    return bishopTurn(rank,file,board);
  }
  else if(board[rank][file] % 8 == 5){
    return rookTurn(rank,file,board);
  }
  else{
    return queenTurn(rank,file,board);
  }
}
*/

void oppAttackingMoves(int rank,int file, int** board ,long long int kingPos,BitBoard& bBoard){
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
    bishopMoves(rank,file,board,kingPos,bBoard);
  }
  else if(board[rank][file] % 8 == 5){
    rookMoves(rank,file,board,kingPos,bBoard);
  }
  else if(board[rank][file] % 8 == 6){
    queenMoves(rank,file,board,kingPos,bBoard);
  }
  else{
    cout << "pieceMoves" << endl;
    sleep(99999);
  }
}

bool newMoveResultsInCheck(Board myBoard){
  int newTurnColour;
  array<int,2> kingPosition;
  if(turnColour == white){
    kingPosition = myBoard.kingPositions[1].getOccupiedSquares()[0];
    newTurnColour = black;
  }
  else{
    kingPosition = myBoard.kingPositions[0].getOccupiedSquares()[0];
    newTurnColour = white;
  }
  int** board = myBoard.getBoard();
  //array<int,2> kingPosition = kingPos(board, myBoard);

  int actualRank = kingPosition[0];
  int actualFile = kingPosition[1];

  int rank = actualRank;
  int file = actualFile;

  //knightMoves
  //Up-Right
  if(file != 7 && rank > 1 && board[rank-2][file+1] % 8 == 3 && ((board[rank-2][file+1] ^ turnColour) > board[rank-2][file+1])){
    turnColour = newTurnColour;
    return true;
  }
  //Right-Up
  if(file < 6 && rank != 0 && board[rank-1][file+2] % 8 == 3 && ((board[rank-1][file+2] ^ turnColour) > board[rank-1][file+2])){
    turnColour = newTurnColour;
    return true;
  }
  //Right-Down
  if(file < 6 && rank != 7 && board[rank+1][file+2] % 8 == 3 && ((board[rank+1][file+2] ^ turnColour) > board[rank+1][file+2])){
    turnColour = newTurnColour;
    return true;
  }
  //Down-Right
  if(file != 7 && rank < 6 && board[rank+2][file+1] % 8 == 3 && ((board[rank+2][file+1] ^ turnColour) > board[rank+2][file+1])){
    turnColour = newTurnColour;
    return true;
  }
  //Down-Left
  if(file != 0 && rank < 6 && board[rank+2][file-1] % 8 == 3 &&  ((board[rank+2][file-1] ^ turnColour) > board[rank+2][file-1])){
    turnColour = newTurnColour;
    return true;
  }
  //Left-Down
  if(file > 1 && rank != 7 && board[rank+1][file-2] % 8 == 3 && ((board[rank+1][file-2] ^ turnColour) > board[rank+1][file-2])){
    turnColour = newTurnColour;
    return true;
  }
  //Left-Up
  if(file > 1 && rank != 0 && board[rank-1][file-2] % 8 == 3 && ((board[rank-1][file-2] ^ turnColour) > board[rank-1][file-2])){
    turnColour = newTurnColour;
    return true;
  }
  //Up-Left
  if(file !=0 && rank > 1 && board[rank-2][file-1] % 8 == 3 && ((board[rank-2][file-1] ^ turnColour) > board[rank-2][file-1])){
    turnColour = newTurnColour;
    return true;
  }


  //Queen || rook || king

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  if(rank != 0 && rank == actualRank && board[rank-1][file] % 8 == 2){
    turnColour = newTurnColour;
    return true;
  }
  if(rank != 0  && (board[rank-1][file] % 8 == 5 || board[rank-1][file] % 8 == 6) && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){
    turnColour = newTurnColour;
    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  if(rank != 7 && rank == actualRank && board[rank+1][file] % 8 == 2){
    turnColour = newTurnColour;
    return true;
  }
  if(rank != 7  && (board[rank+1][file] % 8 == 5 || board[rank+1][file] % 8 == 6) && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){
    turnColour = newTurnColour;
    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  if(file != 7 && file == actualFile && board[rank][file+1] % 8 == 2){
    turnColour = newTurnColour;
    return true;
  }
  if(file != 7  && (board[rank][file+1] % 8 == 5 || board[rank][file+1] % 8 == 6) && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){
    turnColour = newTurnColour;
    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  if(file != 0 && file == actualFile && board[rank][file-1] % 8 == 2){
    turnColour = newTurnColour;
    return true;
  }
  if(file != 0  && (board[rank][file-1] % 8 == 5 || board[rank][file-1] % 8 == 6) && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){
    turnColour = newTurnColour;
    return true;
  }
  file = actualFile;
  rank = actualRank;


  //Queen or Bishop or pawn or king

  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && rank == actualRank && file == actualFile && (board[rank-1][file+1] % 8 == 2 || (board[rank-1][file+1] == 9 && (board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1]))){
    turnColour = newTurnColour;
    return true;
  }
  if(rank != 0 && file != 7 && (board[rank-1][file+1] % 8 == 4 || board[rank-1][file+1] % 8 == 6) && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){
    turnColour = newTurnColour;
    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && rank == actualRank && file == actualFile && (board[rank+1][file+1] % 8 == 2 || (board[rank+1][file+1] == 17 && (board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1]))){
    turnColour = newTurnColour;
    return true;
  }
  if(rank != 7 && file != 7 && (board[rank+1][file+1] % 8 == 4 || board[rank+1][file+1] % 8 == 6) && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){
    turnColour = newTurnColour;
    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && rank == actualRank && file == actualFile && (board[rank+1][file-1] % 8 == 2 || (board[rank+1][file-1] == 17 && (board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1]))){
    turnColour = newTurnColour;
    return true;
  }
  if(rank != 7 && file != 0 && (board[rank+1][file-1] % 8 == 4 || board[rank+1][file-1] % 8 == 6) && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){
    turnColour = newTurnColour;
    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && rank == actualRank && file == actualFile && (board[rank-1][file-1] % 8 == 2 || (board[rank-1][file-1] == 9 && (board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1]))){
    turnColour = newTurnColour;
    return true;
  }
  if(rank != 0 && file != 0 && (board[rank-1][file-1] % 8 == 4 || board[rank-1][file-1] % 8 == 6) && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){
    turnColour = newTurnColour;
    return true;
  }
  turnColour = newTurnColour;
  return false;


}

bool onlyKings(Board myBoard){
  if(myBoard.getWhitePieces().size() == 1 && myBoard.getWhitePieces().size() == 1){
    return true;
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
/*
array<vector<array<int,2>>,6> sortedPiecePosition(int** board, Board myBoard){
  array<vector<array<int,2>>,6> result;
  if(turnColour == white){
    vector<array<int,2>> whitePiecePos = myBoard.getWhitePieces();
    for(int i = 0; i < whitePiecePos.size();i++){
      switch(board[whitePiecePos[i][0]][whitePiecePos[i][1]]){
        case 17:
          result[0].push_back({whitePiecePos[i][0],whitePiecePos[i][1]});
        case 19:
          result[1].push_back({whitePiecePos[i][0],whitePiecePos[i][1]});
        case 20:
          result[2].push_back({whitePiecePos[i][0],whitePiecePos[i][1]});
        case 21:
          result[3].push_back({whitePiecePos[i][0],whitePiecePos[i][1]});
        case 22:
          result[4].push_back({whitePiecePos[i][0],whitePiecePos[i][1]});
        case 18:
          result[5].push_back({whitePiecePos[i][0],whitePiecePos[i][1]});
      }
    }
  }
  else{
    vector<array<int,2>> blackPiecePos = myBoard.getBlackPieces();
    for(int i = 0; i < blackPiecePos.size();i++){
      switch(board[blackPiecePos[i][0]][blackPiecePos[i][1]]){
        case 9:
          result[0].push_back({blackPiecePos[i][0],blackPiecePos[i][1]});
        case 11:
          result[1].push_back({blackPiecePos[i][0],blackPiecePos[i][1]});
        case 12:
          result[2].push_back({blackPiecePos[i][0],blackPiecePos[i][1]});
        case 13:
          result[3].push_back({blackPiecePos[i][0],blackPiecePos[i][1]});
        case 14:
          result[4].push_back({blackPiecePos[i][0],blackPiecePos[i][1]});
        case 10:
          result[5].push_back({blackPiecePos[i][0],blackPiecePos[i][1]});
      }
    }
  }
  return result;
}
*/

array<int,2> kingPos(Board myBoard){
  if(turnColour == white){
    return myBoard.kingPositions[1].getOccupiedSquares()[0];
  }
  else{
    return myBoard.kingPositions[0].getOccupiedSquares()[0];
  }
}
int maxMoves = 0;

BitBoard oppAttackingSquares(int** board, Board myBoard){
  int initTurnColour = turnColour;
  BitBoard result;
  result.attackingSquares = 0;
  result.kingDangerSquares = 0;
  result.checkers = 0;
  result.checkRay = 0;
  result.numCheckers = 0;
  array<PieceList,6> pieces;
  long long int kingPos;
  if(turnColour == white){
    pieces = myBoard.getBlackPieces();
    kingPos = posToBitBoard(myBoard.getWhitePieces()[5].getOccupiedSquares()[0]);
    turnColour = black;
  }
  else{
    pieces = myBoard.getWhitePieces();
    kingPos = posToBitBoard(myBoard.getBlackPieces()[5].getOccupiedSquares()[0]);
    turnColour = white;
  }
  for(int k = 0; k < 6; k++){
    array<int,2>* pieceList = pieces[k].getOccupiedSquares();
    for(int i = 0; i < pieces[k].getSize();i++){
      oppAttackingMoves(pieceList[i][0], pieceList[i][1], board, kingPos, result);
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
  BitBoard oppBitBoard = oppAttackingSquares(board,myBoard);
  if(oppBitBoard.numCheckers == 2){

  }
  totalMem++;
  int index = 0;
  for(int k = 0; k < 6; k++){
    array<int,2>* pieceList = pieces[k].getOccupiedSquares();
    for(int i = 0; i < pieces[k].getSize();i++){
      pieceMove(pieceList[i][0], pieceList[i][1], board, myBoard, index, actualResult);
    }
  }
  actualResult.size = index;
  maxMoves = max(maxMoves,index);

  return actualResult;

}
/*
array<MoveList,16> getMoves(int ** board, Board myBoard){
  array<MoveList,16> result;
  //array<int,4> filler = {-1,-1,-1,-1};
  //fill_n(result, 218, filler);
  MoveList emptyList;
  emptyList.size = -1;
  for(int i = 0; i < 16; i++){
    result[i] = emptyList;
  }
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
  int index = 0;
  for(int k = 0; k < 6; k++){
    array<int,2>* pieceList = pieces[k].getOccupiedSquares();
    for(int i = 0; i < pieces[k].getSize();i++){
      MoveList moves = pieceMoves(pieceList[i][0], pieceList[i][1], board, myBoard);
      result[index] = moves;
      index++;
    }
  }
  //cout << "done result" << endl;
  return result;
}
*/
/*
vector<array<int,4>> getMoves(int ** board, Board myBoard){
  int pieceColour;
  vector<array<int,4>> result;
  vector<array<int,2>> moves;
  array<int,4> val;
  array<int,2> piece;
  if(turnColour == black){
    pieceColour = 0;
  }
  else{
    pieceColour = 6;
  }
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < myBoard.allPositions[pieceColour + i].getSize();j++){
      piece = myBoard.allPositions[pieceColour + i].getOccupiedSquares()[j];
      moves = knightTurn(piece[0],piece[1],board);
      for(int k = 0; k<moves.size();k++){
        val = {piece[0],piece[1],moves[k][0],moves[k][1]};
        result.push_back(val);
      }
    }
  }
  return result;
}
*/
vector<array<int,4>> getMoves(int** board){
  vector<array<int,4>> result;
  vector<array<int,2>> moves;
  if(turnColour == white){
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        if(board[i][j] > 16){
          moves = knightTurn(i,j,board);
          for(int k = 0; k<moves.size();k++){
            array<int,4> val = {i,j,moves[k][0],moves[k][1]};
            result.push_back(val);
          }
        }
      }
    }
  }
  else{
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        if(board[i][j] > 0 && board[i][j] < 16){
          moves = knightTurn(i,j,board);
          for(int k = 0; k<moves.size();k++){
            array<int,4> val = {i,j,moves[k][0],moves[k][1]};
            result.push_back(val);
          }
        }
      }
    }
  }
  return result;
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
    //boardPrinter(myBoard.getBoard());
    piece = {allMoves.moves[index][0],allMoves.moves[index][1]};
    move = {allMoves.moves[index][2],allMoves.moves[index][3]};
    //cout << depth << endl;
    //cout << piece[0] << " " << piece[1] << "to: " << move[0] << " " << move[1] << endl;
    int captured = myBoard.make_move(piece,move);

    if(!newMoveResultsInCheck(myBoard)){
      newResult = MoveGenerator(depth - 1,myBoard);
      result += newResult;
    }

    myBoard.unmake_move(move,piece,captured);
    if(depth==6){
      cout << piece[0] << " " << piece[1] << "to: " << move[0] << " " << move[1] << endl;
      cout << "num moves: " << newResult << endl;
      cout << endl;
      //boardPrinter(myBoard.getBoard());
    }
  }
  delete allMoves.moves;
  totalMem--;
  return result;
/*
  }
  while(allMoves[pieceNum].size != -1){
    for(int index = 0; index < allMoves[pieceNum].size;index++){
      newResult = 0;
      //boardPrinter(myBoard.getBoard());
      piece = {allMoves[pieceNum].moves[index][0],allMoves[pieceNum].moves[index][1]};
      move = {allMoves[pieceNum].moves[index][2],allMoves[pieceNum].moves[index][3]};
      //cout << depth << endl;
      //cout << piece[0] << " " << piece[1] << "to: " << move[0] << " " << move[1] << endl;
      int captured = myBoard.make_move(piece,move);
      if(!newMoveResultsInCheck(myBoard)){
        newResult = MoveGenerator(depth - 1,myBoard);
        result += newResult;
      }
      myBoard.unmake_move(move,piece,captured);
      if(depth==6){
        cout << piece[0] << " " << piece[1] << "to: " << move[0] << " " << move[1] << endl;
        cout << "num moves: " << newResult << endl;
        cout << endl;
        //boardPrinter(myBoard.getBoard());
      }
    }
    if(allMoves[pieceNum].size > -1){
      delete allMoves[pieceNum].moves;
      totalMem--;
    }
    pieceNum++;
  }
  delete allMoves;
  totalMem--;
  return result;
  */
}

/*

bool whiteKingSidePawn( int rank,int file, Board myBoard){
  int currTurnColour = turnColour;
  turnColour = white;
  array<int,2> kingPosition = kingPos(myBoard);
  if(kingPosition[1] > 4 && file > 4){
    turnColour = currTurnColour;
    return true;
  }
  else if(kingPosition[1] < 3 && file < 3){
    turnColour = currTurnColour;
    return true;
  }
  turnColour = currTurnColour;
  return false;

}

bool blackKingSidePawn( int rank,int file, Board myBoard){
  int currTurnColour = turnColour;
  turnColour = black;
  array<int,2> kingPosition = kingPos(myBoard);
  if(kingPosition[1] > 4 && file > 4){
    turnColour = currTurnColour;
    return true;
  }
  else if(kingPosition[1] < 3 && file < 3){
    turnColour = currTurnColour;
    return true;
  }
  turnColour = currTurnColour;
  return false;

}

bool doubledPawn(int rank, int file, Board myBoard){
  int** board = myBoard.getBoard();
  for(int i = 0; i>8;i++){
    if(i != rank && ((board[i][file] ^ turnColour) == board[i][file])){
      return true;
    }
  }
  return false;
}

bool whitePassedPawn(int rank, int file, Board myBoard){
  if(rank == 0){
    return true;
  }
  int** board = myBoard.getBoard();
  for(int i = rank-1; i < 7;i++){
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

bool blackPassedPawn(int rank, int file, Board myBoard){
  if(rank == 7){
    return true;
  }
  int** board = myBoard.getBoard();
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

bool whiteIsolatedPawn(int rank, int file, Board myBoard){
  int** board = myBoard.getBoard();
  if(file > 0){
    int leftFile = file - 1;
    if(file < 7){
      int rightFile = file + 1;
      for(int i = 1; i < 7; i++){
        if(board[i][leftFile] == 17 || board[i][rightFile] == 17){
          return false;
        }
      }
    }
    else{
      for(int i = 1; i < 7; i++){
        if(board[i][leftFile] == 17){
          return false;
        }
      }
    }
  }
  else{
    int rightFile = file + 1;
    for(int i = 1; i < 7; i++){
      if(board[i][rightFile] == 17){
        return false;
      }
    }
  }
  return true;
}

bool blackIsolatedPawn(int rank, int file, Board myBoard){
  int** board = myBoard.getBoard();
  if(file > 0){
    int leftFile = file - 1;
    if(file < 7){
      int rightFile = file + 1;
      for(int i = 1; i < 7; i++){
        if(board[i][leftFile] == 9 || board[i][rightFile] == 9){
          return false;
        }
      }
    }
    else{
      for(int i = 1; i < 7; i++){
        if(board[i][leftFile] == 9){
          return false;
        }
      }
    }
  }
  else{
    int rightFile = file + 1;
    for(int i = 1; i < 7; i++){
      if(board[i][rightFile] == 9){
        return false;
      }
    }
  }
  return true;
}

bool whiteSupportedPawn(int rank, int file, int** board){
  if((file > 0 && board[rank+1][file-1] == 17 ) || (file < 7 && board[rank+1][file+1] == 17)){
    return true;
  }
  else{
    return false;
  }
}

bool blackSupportedPawn(int rank, int file, int** board){
  if((file > 0 && board[rank-1][file-1] == 9 )|| (file < 7 &&board[rank-1][file+1] == 9)){
    return true;
  }
  else{
    return false;
  }
}

vector<array<int,2>> kingZone(int rank, int file, Board myBoard){
  vector<array<int,2>> squares;
  int moveableRank = 1;
  int moveableFile = 1;
  //Up
  for(int i = -2; i < 3; i++){
    if(rank - i < 0 || rank + i > 7){
      continue;
    }
    for(int j = -2; j < 3; j++){
      if(file - j < 0 || file + j > 7){
        continue;
      }
      squares.push_back({i,j});

    }
  }
  return squares;
}

int valueOfAttacker[6] = {10,0,20,20,40,80};
int attackWeight[8] = {0,0,50,75,88,94,97,99};

int kingZoneValue(vector<array<int,2>> kingZone, Board myBoard){
  int** board = myBoard.getBoard();
  int value = 0;
  vector<array<int,2>> attackerList;
  vector<array<int,2>> whitePiecePos = myBoard.getWhitePieces();
  vector<array<int,2>> blackPiecePos = myBoard.getBlackPieces();
  for(int i = 0; i<whitePiecePos.size();i++){
    int valueOfAttack = valueOfAttacker[board[whitePiecePos[i][0]][whitePiecePos[i][1]] % 8 - 1];
    vector<array<int,2>> moves = pieceMoves(whitePiecePos[i][0], whitePiecePos[i][1], board, myBoard);
    for(int j = 0; j<moves.size();j++){
      if(find(kingZone.begin(),kingZone.end(),moves[j]) != kingZone.end()){
        value += valueOfAttack;
        if(find(attackerList.begin(),attackerList.end(),whitePiecePos[i]) == attackerList.end()){
          attackerList.push_back(whitePiecePos[i]);
        }
      }
    }
  }
  value *= (attackWeight[attackerList.size()]/100);
  attackerList.clear();
  int blackValue = 0;
  for(int i = 0; i<blackPiecePos.size();i++){
    int valueOfAttack = valueOfAttacker[board[blackPiecePos[i][0]][blackPiecePos[i][1]] % 8 - 1];
    vector<array<int,2>> moves = pieceMoves(blackPiecePos[i][0], blackPiecePos[i][1], board, myBoard);
    for(int j = 0; j<moves.size();j++){
      if(find(kingZone.begin(),kingZone.end(),moves[j]) != kingZone.end()){
        blackValue += valueOfAttack;
        if(find(attackerList.begin(),attackerList.end(),blackPiecePos[i]) == attackerList.end()){
          attackerList.push_back(blackPiecePos[i]);
        }
      }
    }
  }
  blackValue *= (attackWeight[attackerList.size()]/100);

  if(turnColour == white){
    return value - blackValue;
  }

  return blackValue - value;


}

int kingSafety(int rank, int file, Board myBoard){
  vector<array<int,2>> zone = kingZone(rank,file,myBoard);

  return kingZoneValue(zone, myBoard);
}

int whiteOpenFile(int rank, int file, int** board){
  for(int i = 1; i < 7;i++){
    if(board[i][file] == 17){
      return 0;
    }
    if(board[i][file] == 9){
      return 30;
    }
  }
  return 50;
}

int blackOpenFile(int rank, int file, int** board){
  for(int i = 1; i < 7;i++){
    if(board[i][file] == 9){
      return 0;
    }
    if(board[i][file] == 17){
      return 30;
    }
  }
  return 50;
}

int bishopMobility(int rank, int file, int** board){
  int accum = 0;
  int actualRank = rank;
  int actualFile = file;
  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    accum++;
    rank--;
    file++;
  }
  if(rank != 0 && file != 7){
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    accum++;
    rank++;
    file++;
  }
  if(rank != 7 && file != 7){
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    accum++;
    rank++;
    file--;
  }
  if(rank != 7 && file != 0){
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    accum++;
    rank--;
    file--;
  }
  if(rank != 0 && file != 0){
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  return accum;
}

int knightMobility(int rank, int file, int** board){

  int accum = 0;
  //Up-Right
  if(file != 7 && rank > 1){
    accum++;
  }
  //Right-Up
  if(file < 6 && rank != 0){
    accum++;
  }
  //Right-Down
  if(file < 6 && rank != 7){
    accum++;
  }
  //Down-Right
  if(file != 7 && rank < 6){
    accum++;
  }
  //Down-Left
  if(file != 0 && rank < 6){
    accum++;
  }
  //Left-Down
  if(file > 1 && rank != 7){
    accum++;
  }
  //Left-Up
  if(file > 1 && rank != 0){
    accum++;
  }
  //Up-Left
  if(file !=0 && rank > 1){
    accum++;
  }
  return accum;
}

int rookMobility(int rank, int file, int** board){
  int accum = 0;
  int actualRank = rank;
  int actualFile = file;

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    accum++;
    rank--;
  }
  if(rank != 0){
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    accum++;
    rank++;
  }
  if(rank != 7){
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    accum++;
    file++;
  }
  if(file != 7){
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    accum++;
    file--;
  }
  if(file != 0){
    accum++;
  }
  file = actualFile;
  rank = actualRank;
  return accum;
}

int queenMobility(int rank, int file, int** board){
  return rookMobility(rank, file, board) + bishopMobility(rank,file,board);
}
*/
/*
int pawnCenterControl(int rank, int file, int** board){

}

int pieceMobility(int rank, in file, int** board){
  int piece = board[rank][file] % 8;
  if(piece == 1){
    return pawnCenterControl(rank,file,board);
  }
  if(piece == 3){
    return knightMobility(rank,file,board);
  }
  if(piece == 4){
    return bishopMobility(rank,file,board);
  }
  if(piece == 5){
    return rookMobility(rank,file,board);
  }
  if(piece == 6){
    return queenMobility(rank,file,board);
  }
  else{
    cout << "piece mobility with empty piece" << endl;
    sleep(100000);
    return -1;
  }
}
*/
/*
int whitePiecePositionValue(int piece, int rank,int file, Board myBoard){
  int** board = myBoard.getBoard();
  switch (piece){
    case 1:{
      bool kingSidePawn = whiteKingSidePawn(rank,file,myBoard);
      int val = pawnMap[rank*8 + file];
      if(!endGame(myBoard) && kingSidePawn){
        //if(rank != 6){
        //  val *= -1;
        //}
        if(whiteIsolatedPawn(rank,file,myBoard)){
          val -= 50;
          if(doubledPawn(rank,file,myBoard)){
            val -= 40;
          }
        }

      }
      if(whitePassedPawn(rank,file,myBoard) && !kingSidePawn){
        if(whiteSupportedPawn(rank,file,board)){
          val *= (8-rank)/2;
        }
        else{
          val *= (8-rank)/4;
        }
      }
      if(!kingSidePawn && blackKingSidePawn(rank,file,myBoard)){
        val *= (8-rank)/8;
      }
      if(whiteSupportedPawn(rank,file,board)){
        val += 10;
      }
      else{
        val -= 5;
      }
      if(whiteIsolatedPawn(rank,file,myBoard)){
        val -= 35;
        if(doubledPawn(rank,file,myBoard)){
          val -= 20;
        }
      }
      else{
        val += 5;
      }
      if(doubledPawn(rank,file,myBoard)){
        val -= 10;
      }
      return val;
    }
    case 3:
      return knightMap[rank*8 + file] + 10*knightMobility(rank,file,board);
    case 4:
      return bishopMap[rank*8 + file] + 10*bishopMobility(rank,file,board);
    case 5:
      return rookMap[rank*8 + file] + whiteOpenFile(rank,file,board) + 10*rookMobility(rank,file,board);
    case 6:
      return queenMap[rank*8 + file] + 5*queenMobility(rank,file,board);
    case 2:
      if(endGame(myBoard)){
        int val = kingMapEG[rank*8 + file];
        return val + kingSafety(rank,file,myBoard);
      }
      else{
        int val = whiteKingMapMG[rank*8 + file];
        return val + kingSafety(rank,file,myBoard);
      }
  }
  cout << piece << endl;
  cout << "white" << endl;
  cout << "nonon" << endl;
  sleep(90999);
  return -1;
}

int blackPiecePositionValue(int piece, int rank,int file, Board myBoard){
  int** board = myBoard.getBoard();
  switch (piece){
    case 1:{
      bool kingSidePawn = blackKingSidePawn(rank,file,myBoard);
      int val = pawnMap[63 - (rank*8 + file)];
      if(!endGame(myBoard) && kingSidePawn){
        //if(rank != 6){
        //  val *= -1;
        //}
        if(blackIsolatedPawn(rank,file,myBoard)){
          val -= 50;
          if(doubledPawn(rank,file,myBoard)){
            val -= 40;
          }
        }

      }
      if(blackPassedPawn(rank,file,myBoard) && !kingSidePawn){
        if(blackSupportedPawn(rank,file,board)){
          val *= rank+1/2;
        }
        else{
          val *= (int)rank+1/4;
        }
      }
      if(!kingSidePawn && whiteKingSidePawn(rank,file,myBoard)){
        val *= rank+1/8;
      }
      if(blackSupportedPawn(rank,file,board)){
        val += 10;
      }
      else{
        val -= 5;
      }
      if(blackIsolatedPawn(rank,file,myBoard)){
        val -= 35;
        if(doubledPawn(rank,file,myBoard)){
          val -= 20;
        }
      }
      else{
        val += 5;
      }
      if(doubledPawn(rank,file,myBoard)){
        val -= 10;
      }
    return val;
  }
    case 3:
      return knightMap[63 - (rank*8 + file)] + 10*knightMobility(rank,file,board);
    case 4:
      return bishopMap[63 - (rank*8 + file)] + 10*bishopMobility(rank,file,board);
    case 5:
      return rookMap[63 - (rank*8 + file)] + blackOpenFile(rank,file,board) + 10*rookMobility(rank,file,board);
    case 6:
      return queenMap[63 - (rank*8 + file)] + 5*queenMobility(rank,file,board);
    case 2:
    if(endGame(myBoard)){
      int val = kingMapEG[63 - (rank*8 + file)];
      return val + kingSafety(rank,file,myBoard);
    }
    else{
      int val = blackKingMapMG[63 - (rank*8 + file)];
      return val + kingSafety(rank,file,myBoard);
    }
  }
  cout << "black" << endl;
  cout << rank << " " << file << endl;
  boardPrinter(myBoard.getBoard());
  cout << "nonon" << endl;
  sleep(90999);
  return -1;
}

int see(int moveRank, int moveFile, Board myBoard, int** board){
  if(board[moveRank][moveFile] % 8 == 2){
    return 0;
  }
  int value = 0;
  array<int,2> piece = findSmallest(moveRank,moveFile,myBoard, board);
  if(piece[0] != -1){
    int capturedValue = pieceValue(board[moveRank][moveFile] % 8);
    array<int,2> endPos = {moveRank, moveFile};
    int capturedPiece = myBoard.make_move(piece, endPos);
    if(!newMoveResultsInCheck(myBoard)){
      value = max(0,capturedValue - see(moveRank,moveFile,myBoard,myBoard.getBoard()));
    }
    myBoard.unmake_move(endPos,piece,capturedPiece);
  }
  return value;

}

int predictedVal(int initRank, int initFile,int moveRank, int moveFile, Board myBoard, int** board){
  int guessScore = 0;
  int initPieceValue = pieceValue(board[initRank][initFile]);
  int targetPieceValue = pieceValue(board[moveRank][moveFile]);

  if(targetPieceValue != 0){
    guessScore = see(moveRank,moveFile,myBoard,board);
    array<int,2> oppPiece = {moveRank,moveFile};
    //remove pieceVal
    if(turnColour == white){
      guessScore += blackPiecePositionValue(board[moveRank][moveFile] % 8, moveRank,moveFile,myBoard);
    }
    else{
      guessScore += whitePiecePositionValue(board[moveRank][moveFile] % 8, moveRank,moveFile,myBoard);
    }
  }

  //Promotion
  if(initPieceValue == 100 && (moveRank == 7 || moveRank == 0)){
    guessScore += 800;
    if(turnColour == white){
      int piece = myBoard.make_move({initRank,initFile},{moveRank,moveFile});
      turnColour = black;
      guessScore -= see(moveRank,moveFile,myBoard,myBoard.getBoard());
      myBoard.unmake_move({moveRank,moveFile},{initRank,initFile},piece);
      turnColour = white;
    }
    else{
      int piece = myBoard.make_move({initRank,initFile},{moveRank,moveFile});
      turnColour = white;
      guessScore -= see(moveRank,moveFile,myBoard,myBoard.getBoard());
      myBoard.unmake_move({moveRank,moveFile},{initRank,initFile},piece);
      turnColour = black;
    }
  }
  else{
    //Encourage Development from initPosition
    int initPosVal;
    int endPosVal;



    int currTurnColour = turnColour;
    //Under attack currently (encrourage movement)
    if(turnColour == white){
      //cout << "enter predicted" << endl;
      initPosVal = whitePiecePositionValue(board[initRank][initFile] % 8,initRank,initFile,myBoard);
      endPosVal = whitePiecePositionValue(board[initRank][initFile] % 8, moveRank,moveFile,myBoard);
      guessScore += (endPosVal - initPosVal);
      turnColour = black;
    }
    else{
      initPosVal = blackPiecePositionValue(board[initRank][initFile] % 8,initRank,initFile,myBoard);
      endPosVal = blackPiecePositionValue(board[initRank][initFile] % 8, moveRank,moveFile,myBoard);
      guessScore += (endPosVal - initPosVal);
      turnColour = white;
    }
    int val = see(initRank,initFile,myBoard,board);
    guessScore += (int)val/2;

    if(turnColour == white){
      turnColour = black;
    }
    else{
      turnColour = white;
    }
    int piece = myBoard.make_move({initRank,initFile},{moveRank,moveFile});

    if(!newMoveResultsInCheck(myBoard)){
      guessScore -= see(moveRank,moveFile,myBoard,myBoard.getBoard());
      if(targetPieceValue){
        guessScore += targetPieceValue;
      }
    }

    myBoard.unmake_move({moveRank,moveFile},{initRank,initFile},piece);

    if(currTurnColour != turnColour){
      cout << "wtf" << endl;
      sleep(10);
    }

    //move loses exchange


  }
  return guessScore;



}
*/
/*
int evaluate(Board myBoard,int** board){
  int whiteValue = 0;
  int blackValue = 0;
  int blackPosValue = 0;
  int whitePosValue = 0;
  int posValue = 0;
  std::vector<array<int,2>> whitePiecePositons = myBoard.getWhitePieces();
  for(int i = 0;i < whitePiecePositons.size(); i++){
    whiteValue += pieceValue(board[whitePiecePositons[i][0]][whitePiecePositons[i][1]]);
    posValue = whitePiecePositionValue(board[whitePiecePositons[i][0]][whitePiecePositons[i][1]] % 8,whitePiecePositons[i][0],whitePiecePositons[i][1],myBoard);
    //cout <<"piece: " << whitePiecePositons[i][0] << " " << whitePiecePositons[i][1] << endl;
    //cout << "value: " << posValue << endl;
    whiteValue += posValue;
  }
  std::vector<array<int,2>> blackPiecePositons = myBoard.getBlackPieces();
  for(int i = 0;i < blackPiecePositons.size(); i++){
    blackValue += pieceValue(board[blackPiecePositons[i][0]][blackPiecePositons[i][1]]);
    posValue = blackPiecePositionValue(board[blackPiecePositons[i][0]][blackPiecePositons[i][1]] % 8,blackPiecePositons[i][0],blackPiecePositons[i][1],myBoard);
    //cout <<"piece: " << blackPiecePositons[i][0] << " "<<  blackPiecePositons[i][1] << endl;
    //cout << "value: " << posValue << endl;
    blackValue += posValue;
  }
  //cout << whiteValue << endl;
  //cout << blackValue << endl;
  array<int,2> move;
  int takeValue = 0;
  int removeValue = 0;
  if(turnColour == white){
    turnColour = black;
    for(int i = 0; i < whitePiecePositons.size();i++){
      removeValue = max(takeValue,see(whitePiecePositons[i][0],whitePiecePositons[i][1],myBoard,board));
    }
    whiteValue -= removeValue;
    turnColour = white;
  }
  else{
    turnColour = white;
    for(int i = 0; i < blackPiecePositons.size();i++){
      removeValue = max(takeValue,see(blackPiecePositons[i][0],blackPiecePositons[i][1],myBoard,board));
    }
    blackValue -= removeValue;
    turnColour = black;
  }
  return whiteValue - blackValue;

}

unsigned long long int makeMoveHash(array<int,2> initPos, array<int,2> endPos, int currPiece, int oppPiece, unsigned long long int currHash,Board myBoard){
  int castleHash = getCastleHash(myBoard);
  int enPaisantHash = getEnPaisantHash(currPiece,myBoard);
  int playerHash = (getPlayerHash(currPiece) + 1) % 2;

  currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece][castleHash][enPaisantHash][playerHash];
  currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece][castleHash][enPaisantHash][playerHash];
  //castling
  if(currPiece % 6 == 1 && abs(initPos[1] - endPos[1]) > 1){
    if((endPos[1] - initPos[1]) > 0){
      //kingSide
      if(currPiece == 7){
        currHash ^= ZobristTable[7][7][10][castleHash][enPaisantHash][playerHash];
        currHash ^= ZobristTable[7][5][10][castleHash][enPaisantHash][playerHash];
      }
      else{
        currHash ^= ZobristTable[0][7][4][castleHash][enPaisantHash][playerHash];
        currHash ^= ZobristTable[0][5][4][castleHash][enPaisantHash][playerHash];
      }
    }
    //castling queenSide
    else{
      if(currPiece == 7){
        currHash ^= ZobristTable[7][0][10][castleHash][enPaisantHash][playerHash];
        currHash ^= ZobristTable[7][3][10][castleHash][enPaisantHash][playerHash];
      }
      else{
        currHash ^= ZobristTable[0][0][4][castleHash][enPaisantHash][playerHash];
        currHash ^= ZobristTable[0][3][4][castleHash][enPaisantHash][playerHash];
      }
    }
  }

  else if(oppPiece > 8 && oppPiece < 29){
    currHash ^= ZobristTable[endPos[0]][endPos[1]][getPieceHash(oppPiece)][castleHash][enPaisantHash][playerHash];
  }
  // Promototed Capture
  else if(oppPiece > 29){
    // errase prev hash and put queen
    currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece][castleHash][enPaisantHash][playerHash];
    currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece+5][castleHash][enPaisantHash][playerHash];
    //remove oppPiece
    currHash ^= ZobristTable[endPos[0]][endPos[1]][getPieceHash(oppPiece/10)][castleHash][enPaisantHash][playerHash];

  }
  //Promotion, no capture
  else if(oppPiece == 1){
    //remove prev placed pawn with queen
    currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece][castleHash][enPaisantHash][playerHash];
    currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece+5][castleHash][enPaisantHash][playerHash];
  }
  //EnPaisant capture
  else if((currPiece % 6 == 0) && abs(endPos[1] - initPos[1]) == 1){
    if(currPiece == 6){
      //remove black pawn
      currHash ^= ZobristTable[endPos[0] + 1][endPos[1]][0][castleHash][enPaisantHash][playerHash];
    }
    else{
      //remove white pawn
      currHash ^= ZobristTable[endPos[0] - 1][endPos[1]][6][castleHash][enPaisantHash][playerHash];
    }
  }


  return currHash;
}

unsigned long long int unmakeMoveHash(array<int,2> initPos, array<int,2> endPos, int currPiece, int oppPiece, unsigned long long int currHash,Board myBoard){
  int castleHash = getCastleHash(myBoard);
  int enPaisantHash = getEnPaisantHash(currPiece,myBoard);
  int playerHash = (getPlayerHash(currPiece) + 1) % 2;

  //castling
  if(currPiece % 6 == 1 && abs(initPos[1] - endPos[1]) > 1){
    if((endPos[1] - initPos[1]) < 0){
      //kingSide
      if(currPiece == 7){
        currHash ^= ZobristTable[7][5][10][castleHash][enPaisantHash][playerHash];
        currHash ^= ZobristTable[7][7][10][castleHash][enPaisantHash][playerHash];
      }
      else{
        currHash ^= ZobristTable[0][5][4][castleHash][enPaisantHash][playerHash];
        currHash ^= ZobristTable[0][7][4][castleHash][enPaisantHash][playerHash];
      }
    }
    //castling queenSide
    else{
      if(currPiece == 7){
        currHash ^= ZobristTable[7][3][10][castleHash][enPaisantHash][playerHash];
        currHash ^= ZobristTable[7][0][10][castleHash][enPaisantHash][playerHash];
      }
      else{
        currHash ^= ZobristTable[0][3][4][castleHash][enPaisantHash][playerHash];
        currHash ^= ZobristTable[0][0][4][castleHash][enPaisantHash][playerHash];
      }
    }
  }
  //Normal Capture
  else if(oppPiece > 8 && oppPiece < 29){
    currHash ^= ZobristTable[initPos[0]][initPos[1]][getPieceHash(oppPiece)][castleHash][enPaisantHash][playerHash];
  }

  // Promototed Capture
  else if(oppPiece > 29){
    //restoreOpp
    currHash ^= ZobristTable[initPos[0]][initPos[1]][getPieceHash(oppPiece/10)][castleHash][enPaisantHash][playerHash];
    //Remove Queen and double
    currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece+5][castleHash][enPaisantHash][playerHash];
    currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece][castleHash][enPaisantHash][playerHash];

  }
  // Promotion no Capture
  else if(oppPiece == 1){
    //remove prev placed pawn with queen
    currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece+5][castleHash][enPaisantHash][playerHash];
    currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece][castleHash][enPaisantHash][playerHash];
  }

  //EnPaisant capture
  else if((currPiece % 6 == 0) && abs(endPos[1] - initPos[1]) == 1){
    if(currPiece == 6){
      //remove black pawn
      currHash ^= ZobristTable[initPos[0] + 1][initPos[1]][0][castleHash][enPaisantHash][playerHash];
    }
    else{
      //remove white pawn
      currHash ^= ZobristTable[initPos[0] - 1][initPos[1]][6][castleHash][enPaisantHash][playerHash];
    }
  }

  currHash ^= ZobristTable[initPos[0]][initPos[1]][currPiece][castleHash][enPaisantHash][playerHash];
  currHash ^= ZobristTable[endPos[0]][endPos[1]][currPiece][castleHash][enPaisantHash][playerHash];

  return currHash;
}
int skipped = 0;
int accum = 0;
*/

void adder(int &x){
  x++;
}

int main(){

    int** board = initBoard();

    array<bool,6> castleProperties = {whiteKingMoved,whiteKingRookMoved,whiteQueenRookMoved,blackKingMoved,blackKingRookMoved,blackQueenRookMoved};
    array<int,2> EnPaisantProperties = {whiteEnPaisant,blackEnPaisant};
    Board myBoard(board,castleProperties,EnPaisantProperties);

    cout << "done board" << endl;

    const string startFen = "8/8/8/1k6/3Pp4/8/K7/8 w KQkq - 0 1";
    loadBoardFromFen(startFen, board, myBoard);

    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        if(board[i][j] == 9){
          myBoard.pawnPositions[0].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 10){
          myBoard.kingPositions[0].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 11){
          myBoard.knightPositions[0].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 12){
          myBoard.bishopPositions[0].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 13){
          myBoard.rookPositions[0].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 14){
          myBoard.queenPositions[0].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 17){
          myBoard.pawnPositions[1].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 18){
          myBoard.kingPositions[1].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 19){
          myBoard.knightPositions[1].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 20){
          myBoard.bishopPositions[1].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 21){
          myBoard.rookPositions[1].addPieceAtSquare(i,j);
        }
        else if(board[i][j] == 22){
          myBoard.queenPositions[1].addPieceAtSquare(i,j);
        }
      }
    }
    cout << myBoard.pawnPositions[1].getSize() << endl;


    boardPrinter(board);

    clock_t start = clock();
    //cout << endGame(myBoard) << endl;

    initTable();
    //int** goodBoard = myBoard.getBoard();
    unsigned long long int h = computeHash(board, myBoard);
    cout << "init hash: " << h << endl;
    /*
    int x = 0;
    cout << x << endl;
    adder(x);
    cout << x << endl;
    */
    /*
    int result = MoveGenerator(6, myBoard);
    cout << result << endl;
    cout << totalMem << endl;
    cout << maxMoves << endl;
    */
    MoveList actualResult;
    actualResult.moves = new array<int,4>[218];
    actualResult.size = 0;
    cout << "turnColour " << turnColour <<  endl;
    BitBoard result = oppAttackingSquares(board,myBoard);
    cout << "turnColour " << turnColour <<  endl;
    //int** kingDanger = initBoard(result.kingDangerSquares);
    cout << "attacking squares" << endl;
    bitBoardPrinter(result.attackingSquares);
    cout << "king danger squares" << endl;
    bitBoardPrinter(result.kingDangerSquares);
    cout << "checkers" << endl;
    bitBoardPrinter(result.checkers);
    cout << "checkRay" << endl;
    bitBoardPrinter(result.checkRay);
    cout << result.numCheckers << endl;
    int index = 0;
    //kingMovesCheck(7, 0, board, index, actualResult, result.kingDangerSquares);
    pawnMovesCheck(4, 4, board, index, actualResult, result.checkers, result.checkRay, true);
    for(int i = 0; i < index;i++){
      cout << actualResult.moves[i][2] << " " << actualResult.moves[i][3] << endl;
    }

    //bitBoardPrinter((long long int) 257);
    /*
    array<int,5> result = Search(7,myBoard, -numeric_limits<int>::max(),numeric_limits<int>::max(),h);


    cout << "Piece is: " << result[0] << " " << result[1] << endl;
    cout << "Move is: " << result[2] << " " << result[3] << endl;
    cout << "Score is: " << result[4] << endl;
    cout << "skipped is: " << skipped << endl;
    cout << (float)skipped/accum << endl;
    */
    //cout << boards << endl;
    clock_t end = clock();
    printf ("time: %0.8f sec\n", ((float) end - start)/CLOCKS_PER_SEC);

    return 0;
}
