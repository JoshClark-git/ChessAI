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
#include <bitset>
#include <unordered_map>
#include <fstream>
#include <list>
#include <random>
using namespace std;


array<string,1340146> bookMoves;
clock_t start;
clock_t endClock;
float maxTime;
int maxDepth = 2;

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

array<string,189982> hashArray;
array<string,189982> movesArray;


int totalMem = 0;
struct HashTuple{
  int value;
  int depth;
  int storedType;
};
struct Tuple{
    bool accessed;
    int value;
    int depth;
    //int** board;
    int storedType;
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
  long long int pinners;
  int numCheckers;
};

long long int myPow(int ignore,int exp){
  return static_cast<uint64_t>(1) << exp;
}


long long int posToBitBoard(array<int,2> pos){
  return myPow(2,((pos[0]) * 8) + pos[1]);
}

unsigned long long int ZobristTable[8][8][12][64][8][2];
unsigned long long int OpeningTable[8][8][12][2];
//const int tableSize =27480213;
const int tableSize = 97480217;
Tuple HashTable[tableSize];
unordered_map<unsigned long long int, HashTuple> HashMap;
unordered_map<string, int> GameLog;

mt19937 mt(01234567);

int totalCount = 0;

int turnColour = black;

stack<bool> castlePropertiesStack;
stack<int> EnPaisantPropertiesStack;

bool whiteKingMoved = false;
bool blackKingMoved = false;

bool whiteKingRookMoved = true;
bool whiteQueenRookMoved = true;
bool blackKingRookMoved = true;
bool blackQueenRookMoved = true;

int blackEnPaisant = 9;
int whiteEnPaisant = 9;


const int pawnMap[64] = {800,800,800,800,800,800,800,800,
                          50, 50, 50, 50, 50, 50, 50, 50,
                          10, 10, 20, 30, 30, 20, 10, 10,
                          5,  5, 10, 25, 25, 10,  5,  5,
                          0,  0,  5, 20, 20,  5,  0,  0,
                          3, -5,-10,  0,  0,-10, -5,  3,
                          5, 10, 10,-20,-20, 10, 10,  5,
                          0,  0,  0,  0,  0,  0,  0,  0};
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
      totalMem++;
    }
    void deleteOccupiedSquares(){
      delete this->occupiedSquares;
      this->numPieces = 0;
      totalMem--;
    }
    void setPieceMap(int** pieceMap){
      this->pieceMap = pieceMap;
    }
    void deletePieceMap(){
      int** pieceMap = this->pieceMap;
      for(int i = 0; i < 8; i++){
        delete pieceMap[i];
      }
      delete pieceMap;
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

  void deletePieceLists(){
    this->pawnPositions[0].deletePieceMap();
    this->pawnPositions[0].deleteOccupiedSquares();
    this->pawnPositions[1].deletePieceMap();
    this->pawnPositions[1].deleteOccupiedSquares();
    this->knightPositions[0].deletePieceMap();
    this->knightPositions[0].deleteOccupiedSquares();
    this->knightPositions[1].deletePieceMap();
    this->knightPositions[1].deleteOccupiedSquares();
    this->bishopPositions[0].deletePieceMap();
    this->bishopPositions[0].deleteOccupiedSquares();
    this->bishopPositions[1].deletePieceMap();
    this->bishopPositions[1].deleteOccupiedSquares();
    this->rookPositions[0].deletePieceMap();
    this->rookPositions[0].deleteOccupiedSquares();
    this->rookPositions[1].deletePieceMap();
    this->rookPositions[1].deleteOccupiedSquares();
    this->queenPositions[0].deletePieceMap();
    this->queenPositions[0].deleteOccupiedSquares();
    this->queenPositions[1].deletePieceMap();
    this->queenPositions[1].deleteOccupiedSquares();
    this->kingPositions[0].deletePieceMap();
    this->kingPositions[0].deleteOccupiedSquares();
    this->kingPositions[1].deletePieceMap();
    this->kingPositions[1].deleteOccupiedSquares();
  }


  Board(int** board,array<bool,6> castleProperties,array<int,2> EnPaisantProperties){
    this->boardReprentation = board;
    this->castleProperties = castleProperties;
    this->EnPaisantProperties = EnPaisantProperties;
    setupPieceLists();
    updateStacks();
  }

  void newBoard(int** board){
    this->boardReprentation = board;
  }

  void deleteBoard(){
    for(int i =0; i<8;i++){
      delete this->boardReprentation[i];
    }
    delete this->boardReprentation;
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
        sleep(99999999);
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
        cout << initPos[0] << " " << initPos[1] << endl;
        cout << endPos[0] << " " << endPos[1] << endl;
        boardPrinter(this->boardReprentation);
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
    //boardPrinter(board);
    int pieceType = board[initPos[0]][initPos[1]];
    //cout << "pieceType: " << pieceType << endl;
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
    int** board = this->boardReprentation;
    //cout << "enter make" << endl;
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
    //cout << "enter apply" << endl;
    int piece = this->applyMoveForward(initPos,endPos);
    //castling

    if(turnColour == white){
      turnColour = black;
    }
    else{
      turnColour = white;
    }

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

    if(abs(endPos[1] - initPos[1]) > 1 && (board[initPos[0]][initPos[1]] % 8 == 2)){
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
    else if(abs(endPos[1] - initPos[1]) == 1 && (board[endPos[0]][endPos[1]] % 8 == 1) && piece == 0){
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
typedef unsigned long long int UINT64;
UINT64 getRandom(const UINT64 &begin = 0, const UINT64 &end = 100) {
    return begin >= end ? 0 : begin + (UINT64) ((end - begin)*rand()/(double)RAND_MAX);
};

void initOpeningTable(){
  for (int i = 0; i<8; i++){
    for (int j = 0; j<8; j++){
      for (int k = 0; k<12; k++){
        for(int l = 0; l<2; l++){
          OpeningTable[i][j][k][l] = randomInt();
        }
      }
    }
  }
}

void initTable(){
    int accum = 0;
    for (int i = 0; i<8; i++){
      for (int j = 0; j<8; j++){
        for (int k = 0; k<12; k++){
          for(int l = 0; l<64;l++){
            for(int m = 0;m < 8; m++){
              for(int n = 0; n<2;n++){
                ZobristTable[i][j][k][l][m][n] = randomInt();
                //ZobristTable[i][j][k][l][m][n] = getRandom(0, ULLONG_MAX);
                HashTable[accum].accessed = false;
                HashTable[accum].depth = -1;
                HashTable[accum].value = -numeric_limits<int>::max();
                accum++;
              }
            }
          }
        }
      }
    }
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

unsigned long long int computeOpeningHash(int** board, Board myBoard){
    unsigned long long int h = 0;
    int pieceHash;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if(board[i][j]){
              pieceHash = getPieceHash(board[i][j]);
              h ^= OpeningTable[i][j][pieceHash][getPlayerHash(pieceHash)];
              if(i == 6 && j == 4){
                //cout << "initremovalVal: " << OpeningTable[i][j][pieceHash][getPlayerHash(pieceHash)] << endl;
              }
            }
        }
    }
    //cout << "oinit: " << h << endl;
    //cout << getPieceHash(board[6][4]) << endl;
    //cout << getPlayerHash(getPieceHash(board[6][4])) << endl;
    //pieceHash = getPieceHash(board[6][4]);
    //h ^= OpeningTable[6][4][pieceHash][getPlayerHash(pieceHash)];
    //cout << "after Delete: " << h << endl;
    ///sleep(9999);
    return h;
}

void bitBoardPrinter(long long int board){
  long long int val;
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      val = ((board & myPow(2,(i * 8) + j) ? 0 : 1) + 1) % 2;
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

bool newGameLog(int** board){
  for(int i = 0; i < 8; i++){
    if(board[6][i] != 17){
      return false;
    }
  }
  return true;
}

void deleteBoard(int** board){
  for(int i = 0; i < 8; i++){
    delete board[i];
  }
  delete board;
}

void copyBoard(int** goodBoard, int** toBeCopied){
  for(int i = 0;i < 8; i++){
    for(int j = 0; j < 8; j++){
      toBeCopied[i][j] = goodBoard[i][j];
    }
  }
}

array<PieceList,6> piecePos(Board myBoard){
  if(turnColour == white){
    return myBoard.getWhitePieces();
  }
  else{
    return myBoard.getBlackPieces();
  }
}

int whitePawnCounter(int** board){
  int accum = 0;
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(board[i][j] == 17){
        accum++;
      }
    }
  }
  return accum;
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
  auto currTurnColour = turnColour;
  if(board[rank][file] > 16){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  //Up
  long long int squareVal = myPow(2,(rank-1) * 8 + file);
  if(rank != 0 && (squareVal & checkers) == checkers && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank-1,file};
    accum++;
  }
  //Down
  squareVal = myPow(2,(rank+1) * 8 + file);
  if(rank != 7 && (squareVal & checkers) == checkers && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank+1,file};
    accum++;
  }
  //Right
  squareVal = myPow(2,(rank) * 8 + file+1);
  if(file != 7 && (squareVal & checkers) == checkers && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank,file+1};
    accum++;
  }
  //Left
  squareVal = myPow(2,(rank) * 8 + file-1);
  if(file != 0 && (squareVal & checkers) == checkers && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank,file-1};
    accum++;
  }
  //Up-Right
  squareVal = myPow(2,(rank-1) * 8 + file+1);
  if(rank != 0 && file != 7 && (squareVal & checkers) == checkers && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank-1,file+1};
    accum++;
  }
  //Down-Right
  squareVal = myPow(2,(rank+1) * 8 + file+1);
  if(rank != 7 && file != 7 && (squareVal & checkers) == checkers && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank+1,file+1};
    accum++;
  }
  //Down-Left
  squareVal = myPow(2,(rank+1) * 8 + file-1);
  if(rank != 7 && file != 0 && (squareVal & checkers) == checkers && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank+1,file-1};
    accum++;
  }
  //Up-Left
  squareVal = myPow(2,(rank-1) * 8 + file-1);
  if(rank != 0 && file != 0 && (squareVal & checkers) == checkers && ((squareVal & kingDangerSquares) == 0)){
    possMoves.moves[accum] = {rank,file,rank-1,file-1};
    accum++;
  }

  turnColour = currTurnColour;
}

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
      std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPieceList[i][0],oppPieceList[i][1],board);
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
    if(!whiteKingMoved && !whiteKingRookMoved && whiteCastleKingSide(board, myBoard)){
      if(file != 4){
        cout << "WrongCastling" << endl;
        sleep(1);
      }
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
    boardPrinter(board);
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

void actualPins(int** board,Board myBoard, BitBoard& bBoard){
  long long int pinnedPieces = bBoard.pinners;
  long long int result = 0;
  long long int squareVal;
  array<int,2> kingPosition;
  if(turnColour == white){
    kingPosition = myBoard.kingPositions[1].getOccupiedSquares()[0];
  }
  else{
    kingPosition = myBoard.kingPositions[0].getOccupiedSquares()[0];
  }
  //array<int,2> kingPosition = kingPos(board, myBoard);

  int actualRank = kingPosition[0];
  int actualFile = kingPosition[1];

  int rank = actualRank;
  int file = actualFile;


  //Queen || rook || king

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  squareVal = myPow(2,(rank-1) * 8 + file);
  if(rank != 0 && ((squareVal & pinnedPieces) == squareVal)){
    result |= squareVal;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  squareVal = myPow(2,(rank+1) * 8 + file);
  if(rank != 0 && ((squareVal & pinnedPieces) == squareVal)){
    result |= squareVal;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  squareVal = myPow(2,(rank) * 8 + file+1);
  if(rank != 0 && ((squareVal & pinnedPieces) == squareVal)){
    result |= squareVal;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  squareVal = myPow(2,(rank) * 8 + file-1);
  if(rank != 0 && ((squareVal & pinnedPieces) == squareVal)){
    result |= squareVal;
  }
  file = actualFile;
  rank = actualRank;


  //Queen or Bishop or pawn or king

  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  squareVal = myPow(2,(rank-1) * 8 + file+1);
  if(rank != 0 && ((squareVal & pinnedPieces) == squareVal)){
    result |= squareVal;
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  squareVal = myPow(2,(rank+1) * 8 + file+1);
  if(rank != 0 && ((squareVal & pinnedPieces) == squareVal)){
    result |= squareVal;
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  squareVal = myPow(2,(rank+1) * 8 + file-1);
  if(rank != 0 && ((squareVal & pinnedPieces) == squareVal)){
    result |= squareVal;
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  squareVal = myPow(2,(rank-1) * 8 + file-1);
  if(rank != 0 && ((squareVal & pinnedPieces) == squareVal)){
    result |= squareVal;
  }
  bBoard.pinners = result;
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

bool playerInCheck(Board myBoard){
  int** board = myBoard.getBoard();
  array<int,2> kingPosition = kingPos(myBoard);

  int actualRank = kingPosition[0];
  int actualFile = kingPosition[1];

  int rank = actualRank;
  int file = actualFile;

  //knightMoves
  //Up-Right
  if(file != 7 && rank > 1 && board[rank-2][file+1] % 8 == 3 && ((board[rank-2][file+1] ^ turnColour) > board[rank-2][file+1])){

    return true;
  }
  //Right-Up
  if(file < 6 && rank != 0 && board[rank-1][file+2] % 8 == 3 && ((board[rank-1][file+2] ^ turnColour) > board[rank-1][file+2])){

    return true;
  }
  //Right-Down
  if(file < 6 && rank != 7 && board[rank+1][file+2] % 8 == 3 && ((board[rank+1][file+2] ^ turnColour) > board[rank+1][file+2])){

    return true;
  }
  //Down-Right
  if(file != 7 && rank < 6 && board[rank+2][file+1] % 8 == 3 && ((board[rank+2][file+1] ^ turnColour) > board[rank+2][file+1])){

    return true;
  }
  //Down-Left
  if(file != 0 && rank < 6 && board[rank+2][file-1] % 8 == 3 &&  ((board[rank+2][file-1] ^ turnColour) > board[rank+2][file-1])){

    return true;
  }
  //Left-Down
  if(file > 1 && rank != 7 && board[rank+1][file-2] % 8 == 3 && ((board[rank+1][file-2] ^ turnColour) > board[rank+1][file-2])){

    return true;
  }
  //Left-Up
  if(file > 1 && rank != 0 && board[rank-1][file-2] % 8 == 3 && ((board[rank-1][file-2] ^ turnColour) > board[rank-1][file-2])){

    return true;
  }
  //Up-Left
  if(file !=0 && rank > 1 && board[rank-2][file-1] % 8 == 3 && ((board[rank-2][file-1] ^ turnColour) > board[rank-2][file-1])){

    return true;
  }


  //Queen || rook || king

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  if(rank != 0 && rank == actualRank && board[rank-1][file] % 8 == 2){

    return true;
  }
  if(rank != 0  && (board[rank-1][file] % 8 == 5 || board[rank-1][file] % 8 == 6) && ((board[rank-1][file] ^ turnColour) > board[rank-1][file])){

    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  if(rank != 7 && rank == actualRank && board[rank+1][file] % 8 == 2){

    return true;
  }
  if(rank != 7  && (board[rank+1][file] % 8 == 5 || board[rank+1][file] % 8 == 6) && ((board[rank+1][file] ^ turnColour) > board[rank+1][file])){

    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  if(file != 7 && file == actualFile && board[rank][file+1] % 8 == 2){

    return true;
  }
  if(file != 7  && (board[rank][file+1] % 8 == 5 || board[rank][file+1] % 8 == 6) && ((board[rank][file+1] ^ turnColour) > board[rank][file+1])){

    return true;
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  if(file != 0 && file == actualFile && board[rank][file-1] % 8 == 2){

    return true;
  }
  if(file != 0  && (board[rank][file-1] % 8 == 5 || board[rank][file-1] % 8 == 6) && ((board[rank][file-1] ^ turnColour) > board[rank][file-1])){

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

    return true;
  }
  if(rank != 0 && file != 7 && (board[rank-1][file+1] % 8 == 4 || board[rank-1][file+1] % 8 == 6) && ((board[rank-1][file+1] ^ turnColour) > board[rank-1][file+1])){

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

    return true;
  }
  if(rank != 7 && file != 7 && (board[rank+1][file+1] % 8 == 4 || board[rank+1][file+1] % 8 == 6) && ((board[rank+1][file+1] ^ turnColour) > board[rank+1][file+1])){

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

    return true;
  }
  if(rank != 7 && file != 0 && (board[rank+1][file-1] % 8 == 4 || board[rank+1][file-1] % 8 == 6) && ((board[rank+1][file-1] ^ turnColour) > board[rank+1][file-1])){

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

    return true;
  }
  if(rank != 0 && file != 0 && (board[rank-1][file-1] % 8 == 4 || board[rank-1][file-1] % 8 == 6) && ((board[rank-1][file-1] ^ turnColour) > board[rank-1][file-1])){

    return true;
  }

  return false;
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

bool isPinned(int rank, int file, long long int pinners){
  long long int squareVal = myPow(2,(rank*8) + file);
  if((squareVal & pinners) == squareVal){
    return true;
  }
  return false;
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
/*
array<int,2> findSmallest(int rank, int file, Board myBoard,int ** board){
  array<int,2> target = {rank,file};
  int smallest = 10;
  vector<array<int,2>> moves;
  //vector<array<int,2>> piecePositions;
  array<vector<array<int,2>>,6> sortedPieces = sortedPiecePosition(board,myBoard);
  for(int i = 0; i < 6; i++){
    for(int j = 0; j<sortedPieces[i].size();j++){
      moves = pieceMoves(sortedPieces[i][j][0], sortedPieces[i][j][1],board,myBoard);
      if(find(moves.begin(),moves.end(),target) != moves.end()){
        return sortedPieces[i][j];
      }
    }
  }
  return {-1,-1};

}
*/
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

MoveList getMoves(Board myBoard, int** board){
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
  /*
  cout << "attacking squares" << endl;
  bitBoardPrinter(oppBitBoard.attackingSquares);
  cout << "king danger squares" << endl;
  bitBoardPrinter(oppBitBoard.kingDangerSquares);
  cout << "checkers" << endl;
  bitBoardPrinter(oppBitBoard.checkers);
  cout << "checkRay" << endl;
  bitBoardPrinter(oppBitBoard.checkRay);
  cout << "pinners" << endl;
  bitBoardPrinter(oppBitBoard.pinners);
  */
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
  /*
  for(int i = 0; i < index; i++){
    cout << actualResult.moves[i][0] << " " << actualResult.moves[i][1] << " to " << actualResult.moves[i][2] << " " << actualResult.moves[i][3] << endl;
  }
  */
  return actualResult;

}

int whiteKingZoneValue(int kingRank, int kingFile, Board myBoard){
  int currTurnColour = turnColour;
  int** board = myBoard.getBoard();
  int value = 0;
  int blackValue = 0;
  int valueOfAttack;
  //vector<array<int,2>> attackerList;
  int numPieces = 0;
  array <int,2> currMove;
  array <int,2> currPiece;
  array<int,2> prev = {-1,-1};
  turnColour = black;
  MoveList blackMoveList = getMoves(myBoard, board);
  array<int,4>* blackMoves = blackMoveList.moves;
  for(int i = 0; i < blackMoveList.size;i++){
     currMove = {blackMoves[i][2],blackMoves[i][3]};
     if(((currMove[0] - 2) <= kingRank && (currMove[0] + 2) >= kingRank) && ((currMove[1] - 2) <= kingFile && (currMove[1] + 2) >= kingFile)){
       currPiece = {blackMoves[i][0], blackMoves[i][1]};
       valueOfAttack = valueOfAttacker[(board[currPiece[0]][currPiece[1]] % 8) - 1 ];
       blackValue += valueOfAttack;
       if(currPiece[0] != prev[0] || currPiece[1] != prev[1]){
         numPieces++;
       }
       prev = currPiece;
     }
  }
  blackValue = (int) ((blackValue * attackWeight[numPieces])/100);
  if(blackValue == 0){
    turnColour = currTurnColour;
    delete blackMoveList.moves;
    totalMem--;
    return 0;
  }

  turnColour = white;
  numPieces = 0;
  MoveList whiteMoveList = getMoves(myBoard, board);
  array<int,4>* whiteMoves = whiteMoveList.moves;
  prev = {-1,-1};
  for(int i = 0; i < whiteMoveList.size;i++){
     currMove = {whiteMoves[i][2],whiteMoves[i][3]};
     if((currMove[0] - 2 <= kingRank && currMove[0] + 2 >= kingRank) && (currMove[1] - 2 <= kingFile && currMove[1] + 2 >= kingFile)){
       currPiece = {whiteMoves[i][0], whiteMoves[i][1]};
       valueOfAttack = valueOfAttacker[(board[currPiece[0]][currPiece[1]] % 8) - 1 ];
       value += valueOfAttack;
       if(currPiece[0] != prev[0] || currPiece[1] != prev[1]){
         numPieces++;
       }
       prev = currPiece;
     }
  }
  value = (int) ((value * attackWeight[numPieces])/100);

  turnColour = currTurnColour;

  delete whiteMoveList.moves;
  totalMem--;
  delete blackMoveList.moves;
  totalMem--;
  //cout << "whiteVal: " << value << endl;
  //cout << "blackVal: " << blackValue << endl;
  if((value - blackValue) < 0){
    return value - blackValue;
  }
  return 0;


}

int blackKingZoneValue(int kingRank, int kingFile, Board myBoard){
  int currTurnColour = turnColour;
  int** board = myBoard.getBoard();
  int value = 0;
  int blackValue = 0;
  int valueOfAttack;
  //vector<array<int,2>> attackerList;
  int numPieces = 0;
  turnColour = white;
  MoveList whiteMoveList = getMoves(myBoard, board);
  array<int,4>* whiteMoves = whiteMoveList.moves;
  array <int,2> currMove;
  array <int,2> currPiece;
  array<int,2> prev = {-1,-1};
  for(int i = 0; i < whiteMoveList.size;i++){
     currMove = {whiteMoves[i][2],whiteMoves[i][3]};
     if((currMove[0] - 2 <= kingRank && currMove[0] + 2 >= kingRank) && (currMove[1] - 2 <= kingFile && currMove[1] + 2 >= kingFile)){
       currPiece = {whiteMoves[i][0], whiteMoves[i][1]};
       valueOfAttack = valueOfAttacker[(board[currPiece[0]][currPiece[1]] % 8) - 1 ];
       value += valueOfAttack;
       if(currPiece[0] != prev[0] || currPiece[1] != prev[1]){
         numPieces++;
       }
       prev = currPiece;
     }
  }
  value = (int) ((value * attackWeight[numPieces])/100);
  if(value == 0){
    turnColour = currTurnColour;
    delete whiteMoveList.moves;
    totalMem--;
    return 0;
  }

  numPieces = 0;
  prev = {-1,-1};
  turnColour = black;
  MoveList blackMoveList = getMoves(myBoard, board);
  array<int,4>* blackMoves = blackMoveList.moves;
  for(int i = 0; i < blackMoveList.size;i++){
     currMove = {blackMoves[i][2],blackMoves[i][3]};
     if(((currMove[0] - 2) <= kingRank && (currMove[0] + 2) >= kingRank) && ((currMove[1] - 2) <= kingFile && (currMove[1] + 2) >= kingFile)){
       currPiece = {blackMoves[i][0], blackMoves[i][1]};
       valueOfAttack = valueOfAttacker[(board[currPiece[0]][currPiece[1]] % 8) - 1 ];
       blackValue += valueOfAttack;
       if(currPiece[0] != prev[0] || currPiece[1] != prev[1]){
         numPieces++;
       }
       prev = currPiece;
     }
  }
  blackValue = (int) ((blackValue * attackWeight[numPieces])/100);

  turnColour = currTurnColour;

  delete whiteMoveList.moves;
  totalMem--;
  delete blackMoveList.moves;
  totalMem--;
  //cout << "whiteVal: " << value << endl;
  //cout << "blackVal: " << blackValue << endl;
  if((blackValue - value) < 0){
    return blackValue - value;
  }
  return 0;


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
  return accum * 5;
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
  return accum * 5;
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
  return accum * 5;
}

int queenMobility(int rank, int file, int** board){
  return rookMobility(rank, file, board) + bishopMobility(rank,file,board);
}

/*
int pieceMobility(int rank, in file, int** board){
  int piece = board[rank][file] % 8;
  if(piece == 1){
    return 0
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
bool endGame(Board myBoard){
  int whiteValue = 0;
  int blackValue = 0;
  int** board = myBoard.getBoard();
  array<PieceList,6> whitePiecePositons = myBoard.getWhitePieces();
  array<PieceList,6> blackPiecePositons = myBoard.getBlackPieces();
  for(int i = 0; i < 6;i ++){
    array<int,2>* whitePositions = whitePiecePositons[i].getOccupiedSquares();
    for(int j = 0; j < whitePiecePositons[i].getSize();j++){
      whiteValue += pieceValue(board[whitePositions[j][0]][whitePositions[j][1]]);
    }
  }
  for(int i = 0; i < 6;i ++){
    array<int,2>* blackPositions = blackPiecePositons[i].getOccupiedSquares();
    for(int j = 0; j < blackPiecePositons[i].getSize();j++){
      blackValue += pieceValue(board[blackPositions[j][0]][blackPositions[j][1]]);
    }
  }
  if((blackValue + whiteValue) <= 3500){
    return true;
  }
  return false;
}
/*
bool drawnMaterial(Board myBoard, int** board){
  int whiteValue = 0;
  int blackValue = 0;
  int** board = myBoard.getBoard();
  array<PieceList,6> whitePiecePositons = myBoard.getWhitePieces();
  array<PieceList,6> blackPiecePositons = myBoard.getBlackPieces();
  if(whitePiecePositions[0].getSize() != 0 || blackPiecePositions[0].getSize() != 0){
    return false;
  }
  return true;
}
*/
int whitePiecePositionValue(int piece, int rank,int file, Board myBoard, bool isEndGame){
  int** board = myBoard.getBoard();
  switch (piece){
    case 1:{
      //bool kingSidePawn = whiteKingSidePawn(rank,file,myBoard);
      int val = pawnMap[rank*8 + file];
      if(isEndGame && rank < 4){
        if(whitePassedPawn(rank,file,board)){
          if(whiteProtectedPawn(rank,file,board)){
            //val = abs(val)*(8-rank);
            val = 25*(8-rank);
          }
          else{
            //val = abs(val)*(int)(8-rank);
            val = 17*(8-rank);
          }
        }
      }
      return val;
      if(!(endGame(myBoard)) && whiteKingSidePawn(rank,file,myBoard)){
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
      if(whitePassedPawn(rank,file,board)){
        if(whiteProtectedPawn(rank,file,board)){
          val = abs(val)*(8-rank);
        }
        else{
          val = abs(val)*(int)(8-rank)/4;
        }
      }
      /*
      if(!kingSidePawn && blackKingSidePawn(rank,file,myBoard)){
        val = abs(val)*(8-rank)/8;
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
      */
      return val;
    }
    case 3:
      if(isEndGame){
        return (knightMap[rank*8 + file] + knightMobility(rank,file,board))/2;
      }
      //return knightMap[rank*8 + file] + 10*knightMobility(rank,file,board);
      return knightMap[rank*8 + file] + knightMobility(rank,file,board);
    case 4:
      if(isEndGame){
        return (bishopMap[rank*8 + file]+ bishopMobility(rank,file,board))/2;
      }
      //return bishopMap[rank*8 + file] + 10*bishopMobility(rank,file,board);
      return bishopMap[rank*8 + file]+ bishopMobility(rank,file,board);
    case 5:
      if(isEndGame){
        return (rookMap[rank*8 + file] + rookMobility(rank,file,board))/2;
      }
      //return rookMap[rank*8 + file] + 10*rookMobility(rank,file,board);
      return rookMap[rank*8 + file] + rookMobility(rank,file,board);
    case 6:
      if(isEndGame){
        return (queenMap[rank*8 + file])/2;
      }
      return queenMap[rank*8 + file];
    case 2:
      if(isEndGame){
        int val = kingMapEG[rank*8 + file];
        return val/2;
        //return val + whiteKingZoneValue(rank,file,myBoard);
        //return 0;
      }
      else{
        bool kingMoved = myBoard.getCastleProperties()[0];
        int val = whiteKingMapMG[rank*8 + file];
        if(kingMoved){
          if(rank < 6){
            val -= 100;
          }
          else{
            if(board[rank-1][file] != 17){
              val -= 25;
              if(board[rank-2][file] != 17){
                val -= 75;
              }
            }
            if(file != 7 &&board[rank-1][file+1] != 17){
              val -= 25;
            }
            if(file != 0 &&board[rank-1][file-1] != 17){
              val -= 25;
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
      //bool kingSidePawn = blackKingSidePawn(rank,file,myBoard);
      int val = pawnMap[63 - (rank*8 + file)];
      if(isEndGame){
        if(blackPassedPawn(rank,file,board) && rank > 3){
          if(blackProtectedPawn(rank,file,board)){
            //val = abs(val)*(8-rank);
            val = 25*rank;
          }
          else{
            //val = abs(val)*(int)(8-rank)/4;
            val = 17*rank;
          }
        }
      }
      return val;
      if(!(endGame(myBoard)) && blackKingSidePawn(rank,file,myBoard)){
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
      if(blackPassedPawn(rank,file,board)){
        if(blackProtectedPawn(rank,file,board)){
          val = abs(val)*(rank+1);
        }
        else{
          val = abs(val)*(int)(rank+1)/4;
        }
      }
      /*
      if(!kingSidePawn && whiteKingSidePawn(rank,file,myBoard)){
        val = abs(val)*rank+1/8;
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
      */
    return val;
  }
    case 3:
      if(isEndGame){
        return (knightMap[63 - (rank*8 + file)] + knightMobility(rank,file,board))/2;
      }
      //return knightMap[63 - (rank*8 + file)] + 10*knightMobility(rank,file,board);
      return knightMap[63 - (rank*8 + file)] + knightMobility(rank,file,board);
    case 4:
      if(isEndGame){
        return (bishopMap[63 - (rank*8 + file)] + bishopMobility(rank,file,board))/2;
      }
      return bishopMap[63 - (rank*8 + file)] + bishopMobility(rank,file,board);
    case 5:
      if(isEndGame){
        return (rookMap[63 - (rank*8 + file)] + rookMobility(rank,file,board))/2;
      }
      return rookMap[63 - (rank*8 + file)] + rookMobility(rank,file,board);
    case 6:
      if(isEndGame){
        return (queenMap[63 - (rank*8 + file)])/2;
      }
      return queenMap[63 - (rank*8 + file)];
    case 2:
      if(isEndGame){
        int val = kingMapEG[63 - (rank*8 + file)];
      //return val + blackKingZoneValue(rank,file,myBoard);
        return val/2;
      }
      else{
        bool kingMoved = myBoard.getCastleProperties()[3];
        int val = blackKingMapMG[63 - (rank*8 + file)];
        if(kingMoved){
        if(rank > 1){
          val -= 100;
        }
        else{
          if(board[rank+1][file] != 9){
            val -= 25;
            if(board[rank+2][file] != 9){
              val -= 75;
            }
          }
          if(file != 7 &&board[rank+1][file+1] != 9){
            val -= 25;
          }
          if(file != 0 &&board[rank+1][file-1] != 9){
            val -= 25;
          }
        }
      }
        return val;
      //return val + blackKingZoneValue(rank,file,myBoard);
      }
    }
  cout << "black" << endl;
  cout << rank << " " << file << endl;
  boardPrinter(myBoard.getBoard());
  cout << "nonon" << endl;
  sleep(90999);
  return -1;
}

array<int,2> findSmallest(int rank, int file, Board myBoard, int ** board){
  //vector<array<int,2>> moves;
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
  /*
  cout << "attacking squares" << endl;
  bitBoardPrinter(oppBitBoard.attackingSquares);
  cout << "king danger squares" << endl;
  bitBoardPrinter(oppBitBoard.kingDangerSquares);
  cout << "checkers" << endl;
  bitBoardPrinter(oppBitBoard.checkers);
  cout << "checkRay" << endl;
  bitBoardPrinter(oppBitBoard.checkRay);
  cout << "pinners" << endl;
  bitBoardPrinter(oppBitBoard.pinners);
  */
  //sleep(10);
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
        //cout << "piece: " << pieceList[i][0] << " " << pieceList[i][1] << endl;
        for(int j = prev; j < index;j++){

          //cout << "move " << actualResult.moves[j][2] << " " << actualResult.moves[j][3] << endl;
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
  /*
  for(int i = 0; i < index; i++){
    cout << actualResult.moves[i][0] << " " << actualResult.moves[i][1] << " to " << actualResult.moves[i][2] << " " << actualResult.moves[i][3] << endl;
  }
  */
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


    if(currTurnColour != turnColour){
      cout << "wtf" << endl;
      sleep(10);
    }

    //move loses exchange


  return guessScore;



}
/*
int predictedVal(int initRank, int initFile,int moveRank, int moveFile, Board myBoard, int** board){
  //cout << "predict: " << initRank << " " << initFile << " to: " << moveRank << " " << moveFile << endl;
  int guessScore = 0;
  int initPieceValue = pieceValue(board[initRank][initFile]);
  int targetPieceValue = pieceValue(board[moveRank][moveFile]);

  if(targetPieceValue != 0){
    //guessScore = see(moveRank,moveFile,myBoard,board);
    guessScore = targetPieceValue;
    array<int,2> oppPiece = {moveRank,moveFile};
    //remove pieceVal
    if(turnColour == white){
      guessScore += blackPiecePositionValue(board[moveRank][moveFile] % 8, moveRank,moveFile,myBoard);
      //guessScore += pieceValue(board[moveRank][moveFile] % 8);
    }
    else{
      guessScore += whitePiecePositionValue(board[moveRank][moveFile] % 8, moveRank,moveFile,myBoard);
      //guessScore += pieceValue(board[moveRank][moveFile] % 8);
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
    if(initRank == 7 && initFile == 4){
    }
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


    guessScore -= see(moveRank,moveFile,myBoard,myBoard.getBoard());


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
  /*
  cout << "attacking squares" << endl;
  bitBoardPrinter(oppBitBoard.attackingSquares);
  cout << "king danger squares" << endl;
  bitBoardPrinter(oppBitBoard.kingDangerSquares);
  cout << "checkers" << endl;
  bitBoardPrinter(oppBitBoard.checkers);
  cout << "checkRay" << endl;
  bitBoardPrinter(oppBitBoard.checkRay);
  cout << "pinners" << endl;
  bitBoardPrinter(oppBitBoard.pinners);
  */
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
  /*
  for(int i = 0; i < index; i++){
    cout << actualResult.moves[i][0] << " " << actualResult.moves[i][1] << " to " << actualResult.moves[i][2] << " " << actualResult.moves[i][3] << endl;
  }
  */
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
    return actualResult;
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
  /*
  for(int i = 0; i < index; i++){
    cout << actualResult.moves[i][0] << " " << actualResult.moves[i][1] << " to " << actualResult.moves[i][2] << " " << actualResult.moves[i][3] << endl;
  }
  */
  return actualResult;
}

struct ordering {
    bool operator ()(pair<array<int,4>, int> const& a,
                     pair<array<int,4>, int> const& b) {
        return a.second > b.second;
    }
};

array<pair<array<int,4>,int>,218> moveOrdering(Board myBoard,int** board){
  //std::vector<vector<int>> result;
  //std::vector<array<int,2>> piecePositons = piecePos(myBoard);
  bool isEndGame = endGame(myBoard);
  MoveList allMoves = getMoves(board,myBoard);
  array<int,4>* allMovesList = allMoves.moves;
  array<pair<array<int,4>,int>,218> moveResult;
  //vector<pair<array<int,4>,int>> moveResult(allMoves.size);
  for(int i = 0; i < allMoves.size; i++){
    moveResult[i] = make_pair(allMovesList[i], predictedVal(allMovesList[i][0],allMovesList[i][1],allMovesList[i][2],allMovesList[i][3], myBoard,board,isEndGame));
  }
  array<int,4> empty = {-1,-1,-1,-1};
  for(int j = allMoves.size; j < 218; j++){
    moveResult[j] = make_pair(empty,-numeric_limits<int>::max());
  }
  /*
  for (int i = 0; i < piecePositons.size(); i++) {
    std::vector<array<int,2>> possMoves = pieceMoves(piecePositons[i][0],piecePositons[i][1],myBoard.getBoard(),myBoard);
    for(int j = 0; j< possMoves.size();j++){
      vector<int> val = {predictedVal(piecePositons[i][0],piecePositons[i][1],possMoves[j][0],possMoves[j][1], myBoard,board)};
      val.push_back(piecePositons[i][0]);
      val.push_back(piecePositons[i][1]);
      val.push_back(possMoves[j][0]);
      val.push_back(possMoves[j][1]);
      result.push_back(val);
    }
  }
  */
  /*
  std::sort(result.begin(), result.end(),
          [](const std::vector<int>& a, const std::vector<int>& b) {
  return a[0] > b[0];
});

  */
  sort(moveResult.begin(),moveResult.begin()+allMoves.size, ordering());
  /*
  for(int i = 0; i < moveResult.size();i++){
    cout << "val: " << moveResult[i].second << " piece: " << moveResult[i].first[0] << " " << moveResult[i].first[1] << " move: " << moveResult[i].first[2] << " " << moveResult[i].first[3] << endl;
  }
  */
  delete allMoves.moves;
  totalMem--;
  //sort(result.begin(),result.end(), std::greater<>());
  /*
  for(int i = 0; i < moveResult.size();i++){
    cout << "val: " << moveResult[i].second << " piece: " << moveResult[i].first[0] << " " << moveResult[i].first[1] << " move: " << moveResult[i].first[2] << " " << moveResult[i].first[3] << endl;
  }
  */
  //sleep(10);


  return moveResult;
}

int evaluate(Board myBoard,int** board){
  int whiteValue = 0;
  int blackValue = 0;
  int blackPosValue = 0;
  int whitePosValue = 0;
  int posValue = 0;
  int pieceVal;
  bool isEndGame = endGame(myBoard);
  array<PieceList,6> whitePieces = myBoard.getWhitePieces();
  for(int j = 0;j < 6; j++){
    array<int,2>* whitePiecePositons = whitePieces[j].getOccupiedSquares();
    for(int i = 0;i < whitePieces[j].getSize(); i++){
      pieceVal = pieceValue(board[whitePiecePositons[i][0]][whitePiecePositons[i][1]]);
      posValue = whitePiecePositionValue(board[whitePiecePositons[i][0]][whitePiecePositons[i][1]] % 8,whitePiecePositons[i][0],whitePiecePositons[i][1],myBoard,isEndGame);
      //cout << "piece: " << whitePiecePositons[i][0] << " " << whitePiecePositons[i][1] << endl;
      //cout << "posValue: " << posValue << endl;
      whiteValue += posValue + pieceVal;
    }
  }
  //cout << endl;
  array<PieceList,6> blackPieces = myBoard.getBlackPieces();
  for(int j = 0;j < 6; j++){
    array<int,2>* blackPiecePositons = blackPieces[j].getOccupiedSquares();
    for(int i = 0;i < blackPieces[j].getSize(); i++){
      pieceVal = pieceValue(board[blackPiecePositons[i][0]][blackPiecePositons[i][1]]);
      posValue = blackPiecePositionValue(board[blackPiecePositons[i][0]][blackPiecePositons[i][1]] % 8,blackPiecePositons[i][0],blackPiecePositons[i][1],myBoard,isEndGame);
      //cout << "piece: " << blackPiecePositons[i][0] << " " << blackPiecePositons[i][1] << endl;
      //cout << "posValue: " << posValue << endl;
      blackValue += posValue + pieceVal;
    }
  }
   //return whiteValue - blackValue;
  //return whiteValue - blackValue;
  int seeVal;
  int removeValue = 0;
  BitBoard oppBitBoard = oppAttackingSquares(board,myBoard);
  MoveList captureMoves = getCaptureMoves(board,myBoard,oppBitBoard);
  for(int i = 0; i < captureMoves.size;i++){
    //cout << board[captureMoves.moves[i][0]][captureMoves.moves[i][1]] << " " << captureMoves.moves[i][0] << " " << captureMoves.moves[i][1] << " to: " << captureMoves.moves[i][2] << " " << captureMoves.moves[i][3] << endl;
    seeVal = evalSee(captureMoves.moves[i][0], captureMoves.moves[i][1],captureMoves.moves[i][2],captureMoves.moves[i][3], myBoard, board);
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
  return whiteValue - blackValue;

}

unsigned long long int makeOpeningHash(array<int,2> initPos, array<int,2> endPos, int currPiece, int oppPiece, unsigned long long int currHash,Board myBoard){
  int playerHash = getPlayerHash(currPiece);
  //cout << "initHash: " << currHash << endl;
  //cout << "i: " << initPos[0] << " " << "j: " << initPos[1] << " " << "piece: " << currPiece << " " << "player: " << playerHash << endl;
  currHash ^= OpeningTable[initPos[0]][initPos[1]][currPiece][playerHash];
  //cout << "removahlHashVal: " << OpeningTable[initPos[0]][initPos[1]][currPiece][playerHash] << endl;
  //cout << "after Removal: " << currHash << endl;
  currHash ^= OpeningTable[endPos[0]][endPos[1]][currPiece][playerHash];
  //cout << "addHashVal: " << OpeningTable[endPos[0]][endPos[1]][currPiece][playerHash] << endl;
  //cout << "after addition: " << currHash << endl;;
  //castling
  if(currPiece % 6 == 1 && abs(initPos[1] - endPos[1]) > 1){
    if((endPos[1] - initPos[1]) > 0){
      //kingSide
      if(currPiece == 7){
        currHash ^= OpeningTable[7][7][10][playerHash];
        currHash ^= OpeningTable[7][5][10][playerHash];
      }
      else{
        currHash ^= OpeningTable[0][7][4][playerHash];
        currHash ^= OpeningTable[0][5][4][playerHash];
      }
    }
    //castling queenSide
    else{
      if(currPiece == 7){
        currHash ^= OpeningTable[7][0][10][playerHash];
        currHash ^= OpeningTable[7][3][10][playerHash];
      }
      else{
        currHash ^= OpeningTable[0][0][4][playerHash];
        currHash ^= OpeningTable[0][3][4][playerHash];
      }
    }
  }

  else if(oppPiece > 8 && oppPiece < 29){
    currHash ^= OpeningTable[endPos[0]][endPos[1]][getPieceHash(oppPiece)][playerHash];
  }
  // Promototed Capture
  else if(oppPiece > 29){
    // errase prev hash and put queen
    currHash ^= OpeningTable[endPos[0]][endPos[1]][currPiece][playerHash];
    currHash ^= OpeningTable[endPos[0]][endPos[1]][currPiece+5][playerHash];
    //remove oppPiece
    currHash ^= OpeningTable[endPos[0]][endPos[1]][getPieceHash(oppPiece/10)][playerHash];

  }
  //Promotion, no capture
  else if(oppPiece == 1){
    //remove prev placed pawn with queen
    currHash ^= OpeningTable[endPos[0]][endPos[1]][currPiece][playerHash];
    currHash ^= OpeningTable[endPos[0]][endPos[1]][currPiece+5][playerHash];
  }
  //EnPaisant capture
  else if((currPiece % 6 == 0) && abs(endPos[1] - initPos[1]) == 1){
    if(currPiece == 6){
      //remove black pawn
      currHash ^= OpeningTable[endPos[0] + 1][endPos[1]][0][playerHash];
    }
    else{
      //remove white pawn
      currHash ^= OpeningTable[endPos[0] - 1][endPos[1]][6][playerHash];
    }
  }

  //cout << "end: " << currHash << endl;
  //sleep(99999);
  return currHash;
}

unsigned long long int makeMoveHash(array<int,2> initPos, array<int,2> endPos, int currPiece, int oppPiece, unsigned long long int currHash,Board myBoard){
  int castleHash = getCastleHash(myBoard);
  int enPaisantHash = getEnPaisantHash(currPiece,myBoard);
  //int playerHash = (getPlayerHash(currPiece) + 1) % 2;
  int playerHash = getPlayerHash(currPiece);

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
  //int playerHash = (getPlayerHash(currPiece) + 1) % 2;
  int playerHash = getPlayerHash(currPiece);

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
int totalPut = 0;
/*
template<std::size_t R, std::size_t L, std::size_t N>
std::bitset<N> project_range(std::bitset<N> b)
{
    static_assert(R <= L && L <= N, "invalid bitrange");
    b >>= R;            // drop R rightmost bits
    b <<= (N - L + R);  // drop L-1 leftmost bits
    b >>= (N - L);      // shift back into place
    return b;
}
*/
template<size_t bits>
std::bitset<bits> subset(std::bitset<bits> set, int min, int max)
{
    const int ignore_hi = bits-max;
    return (set << ignore_hi) >> (ignore_hi + min);
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
    newResult = MoveGenerator(depth - 1,myBoard);
    result += newResult;
    /*
    if(!newMoveResultsInCheck(myBoard)){
      newResult = MoveGenerator(depth - 1,myBoard);
      result += newResult;
    }
    */
    myBoard.unmake_move(move,piece,captured);

    if(depth==6){
      cout << piece[0] << " " << piece[1] << "to: " << move[0] << " " << move[1] << endl;
      cout << "num moves: " << newResult << endl;
      //cout << myBoard.getWhitePieces()[0].getSize() << endl;
      //sleep(3);
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

bool sameBoard(int** currBoard, int** hashBoard){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(currBoard[i][j] != hashBoard[i][j]){
        return false;
      }
    }
  }
  return true;
}

int mateScore(int value, int depth){
  if(abs(value) > 100000){
    if(value > 0){
      return (numeric_limits<int>::max() - (100000+depth));
    }
    else{
      return (-numeric_limits<int>::max() + (100000-depth));
    }
  }
  return value;
}

string boardToFEN(int** board){
  int spaces = 0;
  string result = "";
  string currentFile = "";
  for(int i = 0; i < 8;i++){
    for(int j = 0; j< 8;j++){
      if(board[i][j] == 9){
        if(spaces == 0){
          currentFile += "p";
        }
        else{
          currentFile += to_string(spaces) + "p";
          spaces = 0;
        }
      }
      else if(board[i][j] == 10){
        if(spaces == 0){
          currentFile += "k";
        }
        else{
          currentFile += to_string(spaces) + "k";
          spaces = 0;
        }
      }
      else if(board[i][j] == 11){
        if(spaces == 0){
          currentFile += "n";
        }
        else{
          currentFile += to_string(spaces) + "n";
          spaces = 0;
        }
      }
      else if(board[i][j] == 12){
        if(spaces == 0){
          currentFile += "b";
        }
        else{
          currentFile += to_string(spaces) + "b";
          spaces = 0;
        }
      }
      else if(board[i][j] == 13){
        if(spaces == 0){
          currentFile += "r";
        }
        else{
          currentFile += to_string(spaces) + "r";
          spaces = 0;
        }
      }
      else if(board[i][j] == 14){
        if(spaces == 0){
          currentFile += "q";
        }
        else{
          currentFile += to_string(spaces) + "q";
          spaces = 0;
        }
      }
      else if(board[i][j] == 17){
        if(spaces == 0){
          currentFile += "P";
        }
        else{
          currentFile += to_string(spaces) + "P";
          spaces = 0;
        }
      }
      else if(board[i][j] == 18){
        if(spaces == 0){
          currentFile += "K";
        }
        else{
          currentFile += to_string(spaces) + "K";
          spaces = 0;
        }
      }
      else if(board[i][j] == 19){
        if(spaces == 0){
          currentFile += "N";
        }
        else{
          currentFile += to_string(spaces) + "N";
          spaces = 0;
        }
      }
      else if(board[i][j] == 20){
        if(spaces == 0){
          currentFile += "B";
        }
        else{
          currentFile += to_string(spaces) + "B";
          spaces = 0;
        }
      }
      else if(board[i][j] == 21){
        if(spaces == 0){
          currentFile += "R";
        }
        else{
          currentFile += to_string(spaces) + "R";
          spaces = 0;
        }
      }
      else if(board[i][j] == 22){
        if(spaces == 0){
          currentFile += "Q";
        }
        else{
          currentFile += to_string(spaces) + "Q";
          spaces = 0;
        }
      }
      else{
        spaces++;
      }
    }
    if(spaces != 0){
      currentFile += to_string(spaces);
    }
    result += currentFile + "/";
    currentFile = "";
    spaces = 0;
  }
  string final =  result.substr(0, result.size()-1);
  return final;
}

bool newGame(int** board){
  for(int i = 0; i < 8; i++){
    if(board[1][i] != 9){
      return false;
    }
  }
  return true;
}
/*
string moveToPGN(array<int,4> move, int** board){
  array<int,2> prev = {move[0],move[1]};
  array<int,2> curr = {move[2],move[3]};
}
*/

array<int,4> PGNtoMove(string move, int** board, Board myBoard){
  move.erase(remove(move.begin(), move.end(), '+'), move.end());
  int initFile, initRank,endFile,endRank,capturedPiece;
  //pawn Move
  if(islower(move[0])){
    initFile = (int)move[0] - 97;
    auto found = move.find('=');
    if (found != string::npos){
      int length = move.size();
      while(move[length-1] != '='){
        move.pop_back();
        length--;
      }
      move.pop_back();
    }
    //capture dxe4
    if(move[1] == 'x'){
      endFile = (int)move[2] - 97;
      endRank = 8 - (move[3] - '0');
      if(turnColour == white){
        initRank = endRank + 1;
      }
      else{
        initRank = endRank - 1;
      }
    }
    else{
      endFile = initFile;
      endRank = 8 - (move[1] - '0');
      if(turnColour == white){
        if(board[endRank+1][endFile] != 0){
          initRank = endRank+1;
        }
        else{
          initRank = endRank+2;
        }
      }
      else{
        if(board[endRank-1][endFile] != 0){
          initRank = endRank-1;
        }
        else{
          initRank = endRank-2;
        }
      }
    }
  }
  move.erase(remove(move.begin(), move.end(), 'x'), move.end());
  //QueenMove
  if(move[0] == 'Q'){
    if(turnColour == white){
      initRank = myBoard.queenPositions[1].getOccupiedSquares()[0][0];
      initFile = myBoard.queenPositions[1].getOccupiedSquares()[0][1];
    }
    else{
      initRank = myBoard.queenPositions[0].getOccupiedSquares()[0][0];
      initFile = myBoard.queenPositions[0].getOccupiedSquares()[0][1];
    }
    endRank = 8 - (move[2] - '0');
    endFile = (int)move[1] - 97;
  }

  else if(move[0] == 'N'){
    //Nc3
    if(move.length() == 3){
      endFile = (int)move[1] - 97;
      endRank = 8 - (move[2] - '0');
      if(turnColour == white){
        initRank = myBoard.knightPositions[1].getOccupiedSquares()[0][0];
        initFile = myBoard.knightPositions[1].getOccupiedSquares()[0][1];
        array<int,2> initPos = {endRank,endFile};
        vector<array<int,2>> knightMoves = knightTurn(initRank, initFile, board);
        auto index = find (knightMoves.begin(), knightMoves.end(), initPos);
        board[initRank][initFile] = 0;
        capturedPiece = board[endRank][endFile];
        board[endRank][endFile] = 19;
        if (index == knightMoves.end() || newMoveResultsInCheck(myBoard)){
          board[initRank][initFile] = 19;
          initRank = myBoard.knightPositions[1].getOccupiedSquares()[1][0];
          initFile = myBoard.knightPositions[1].getOccupiedSquares()[1][1];
        }
        board[initRank][initFile] = 19;
        board[endRank][endFile] = capturedPiece;

      }
      else{
        initRank = myBoard.knightPositions[0].getOccupiedSquares()[0][0];
        initFile = myBoard.knightPositions[0].getOccupiedSquares()[0][1];
        array<int,2> initPos = {endRank,endFile};
        vector<array<int,2>> knightMoves = knightTurn(initRank, initFile, board);
        auto index = find(knightMoves.begin(), knightMoves.end(), initPos);
        board[initRank][initFile] = 0;
        capturedPiece = board[endRank][endFile];
        board[endRank][endFile] = 11;
        if (index == knightMoves.end() || newMoveResultsInCheck(myBoard)){
          board[initRank][initFile] = 11;
          initRank = myBoard.knightPositions[0].getOccupiedSquares()[1][0];
          initFile = myBoard.knightPositions[0].getOccupiedSquares()[1][1];
        }
        board[initRank][initFile] = 11;
        board[endRank][endFile] = capturedPiece;
      }
    }
    //Nge7
    else{
      //reference to file
      if((int)move[1] > 60){
        initFile = (int)move[1] - 97;
        if(turnColour == white){
          if(initFile == myBoard.knightPositions[1].getOccupiedSquares()[0][1]){
            initRank = myBoard.knightPositions[1].getOccupiedSquares()[0][0];
          }
          else{
            initRank = myBoard.knightPositions[1].getOccupiedSquares()[1][0];
          }
        }
        else{
          if(initFile == myBoard.knightPositions[0].getOccupiedSquares()[0][1]){
            initRank = myBoard.knightPositions[0].getOccupiedSquares()[0][0];
          }
          else{
            initRank = myBoard.knightPositions[0].getOccupiedSquares()[1][0];
          }
        }
      }
      //N3d2
      else{
        initRank = 8 - (move[1] - '0');

        if(turnColour == white){
          if(initRank == myBoard.knightPositions[1].getOccupiedSquares()[0][0]){
            initFile = myBoard.knightPositions[1].getOccupiedSquares()[0][1];
          }
          else{
            initFile = myBoard.knightPositions[1].getOccupiedSquares()[1][1];
          }
        }
        else{
          if(initRank == myBoard.knightPositions[0].getOccupiedSquares()[0][0]){
            initFile = myBoard.knightPositions[0].getOccupiedSquares()[0][1];
          }
          else{
            initFile = myBoard.knightPositions[0].getOccupiedSquares()[1][1];
          }
        }
      }
      endFile = (int)move[2] - 97;
      endRank = 8 - (move[3] - '0');
    }
  }

  else if(move[0] == 'B'){
    //Bg4
    if(move.length() == 3){
      endFile = (int)move[1] - 97;
      endRank = 8 - (move[2] - '0');
      if(turnColour == white){
        initRank = myBoard.bishopPositions[1].getOccupiedSquares()[0][0];
        initFile = myBoard.bishopPositions[1].getOccupiedSquares()[0][1];

        array<int,2> initPos = {endRank,endFile};
        vector<array<int,2>> bishopMoves = bishopTurn(initRank, initFile, board);

        auto index = find (bishopMoves.begin(), bishopMoves.end(), initPos);
        if (index == bishopMoves.end()){
          initRank = myBoard.bishopPositions[1].getOccupiedSquares()[1][0];
          initFile = myBoard.bishopPositions[1].getOccupiedSquares()[1][1];

        }
      }
      else{
        initRank = myBoard.bishopPositions[0].getOccupiedSquares()[0][0];
        initFile = myBoard.bishopPositions[0].getOccupiedSquares()[0][1];
        array<int,2> initPos = {endRank,endFile};
        vector<array<int,2>> bishopMoves = bishopTurn(initRank, initFile, board);
        auto index = find(bishopMoves.begin(), bishopMoves.end(), initPos);
        if (index == bishopMoves.end()){
          initRank = myBoard.bishopPositions[0].getOccupiedSquares()[1][0];
          initFile = myBoard.bishopPositions[0].getOccupiedSquares()[1][1];
        }
      }
    }
  }

  else if(move[0] == 'R'){
    //Re1
    if(move.length() == 3){
      endFile = (int)move[1] - 97;
      endRank = 8 - (move[2] - '0');
      if(turnColour == white){
        initRank = myBoard.rookPositions[1].getOccupiedSquares()[0][0];
        initFile = myBoard.rookPositions[1].getOccupiedSquares()[0][1];
        array<int,2> initPos = {endRank,endFile};
        vector<array<int,2>> rookMoves = rookTurn(initRank, initFile, board);
        auto index = find (rookMoves.begin(), rookMoves.end(), initPos);
        board[initRank][initFile] = 0;
        capturedPiece = board[endRank][endFile];
        board[endRank][endFile] = 21;
        if (index == rookMoves.end() || newMoveResultsInCheck(myBoard)){
          board[initRank][initFile] = 21;
          initRank = myBoard.rookPositions[1].getOccupiedSquares()[1][0];
          initFile = myBoard.rookPositions[1].getOccupiedSquares()[1][1];
        }
        board[initRank][initFile] = 21;
        board[endRank][endFile] = capturedPiece;
      }
      else{
        initRank = myBoard.rookPositions[0].getOccupiedSquares()[0][0];
        initFile = myBoard.rookPositions[0].getOccupiedSquares()[0][1];
        array<int,2> initPos = {endRank,endFile};
        vector<array<int,2>> rookMoves = rookTurn(initRank, initFile, board);
        auto index = find(rookMoves.begin(), rookMoves.end(), initPos);
        board[initRank][initFile] = 0;
        capturedPiece = board[endRank][endFile];
        board[endRank][endFile] = 13;
        if (index == rookMoves.end() || newMoveResultsInCheck(myBoard)){
          board[initRank][initFile] = 13;
          initRank = myBoard.rookPositions[0].getOccupiedSquares()[1][0];
          initFile = myBoard.rookPositions[0].getOccupiedSquares()[1][1];
        }
        board[initRank][initFile] = 13;
        board[endRank][endFile] = capturedPiece;
      }
    }
    //Rce7
    else{
      //reference to file
      if((int)move[1] > 60){
        initFile = (int)move[1] - 97;
        if(turnColour == white){
          if(initFile == myBoard.rookPositions[1].getOccupiedSquares()[0][1]){
            initRank = myBoard.rookPositions[1].getOccupiedSquares()[0][0];
          }
          else{
            initRank = myBoard.rookPositions[1].getOccupiedSquares()[1][0];
          }
        }
        else{
          if(initFile == myBoard.rookPositions[0].getOccupiedSquares()[0][1]){
            initRank = myBoard.rookPositions[0].getOccupiedSquares()[0][0];
          }
          else{
            initRank = myBoard.rookPositions[0].getOccupiedSquares()[1][0];
          }
        }
      }
      //N3d2
      else{
        initRank = 8 - (move[1] - '0');

        if(turnColour == white){
          if(initRank == myBoard.rookPositions[1].getOccupiedSquares()[0][0]){
            initFile = myBoard.rookPositions[1].getOccupiedSquares()[0][1];
          }
          else{
            initFile = myBoard.rookPositions[1].getOccupiedSquares()[1][1];
          }
        }
        else{
          if(initRank == myBoard.rookPositions[0].getOccupiedSquares()[0][0]){
            initFile = myBoard.rookPositions[0].getOccupiedSquares()[0][1];
          }
          else{
            initFile = myBoard.rookPositions[0].getOccupiedSquares()[1][1];
          }
        }
      }
      endFile = (int)move[2] - 97;
      endRank = 8 - (move[3] - '0');
    }
  }

  else if(move[0] == 'K'){
    if(turnColour == white){
      initRank = myBoard.kingPositions[1].getOccupiedSquares()[0][0];
      initFile = myBoard.kingPositions[1].getOccupiedSquares()[0][1];
    }
    else{
      initRank = myBoard.kingPositions[0].getOccupiedSquares()[0][0];
      initFile = myBoard.kingPositions[0].getOccupiedSquares()[0][1];
    }
    endFile = (int)move[1] - 97;
    endRank = 8 - (move[2] - '0');
  }

  else if(move[0] == 'O'){
    if(move.size() == 3){
      if(turnColour == white){
        initFile = 4;
        initRank = 7;
        endFile = 6;
        endRank = 7;
      }
      else{
        initFile = 4;
        initRank = 0;
        endFile = 6;
        endRank = 0;
      }
    }
    else{
      if(turnColour == white){
        initFile = 4;
        initRank = 7;
        endFile = 2;
        endRank = 7;
      }
      else{
        initFile = 4;
        initRank = 0;
        endFile = 2;
        endRank = 0;
      }
    }
  }

  return {initRank,initFile,endRank,endFile};
}
/*
vector<array<int,2>> blackOpeningMove(string hash, str moveNum,int** board, Board myBoard){
  int currLine = 0;
  int i;
  vector<array<int,2>> result;
  string str;
  string gameFileStr;
  string strMoveNum;
  string hashStr = to_string(hash);
  ifstream hashFile("MasterHashs.txt");
  ifstream gameFile("MasterGames.txt");
  while(getline(hashFile,str)){
    auto found = str.find(hashStr);
    //get gameFile str
    if(found != string::npos){
      i = 0;
      while(i != currLine){
        getline(gameFile,gameFileStr);
        i++;
      }

    }
  }
}

array<int,4> blackOpening(int moveNum, int actualMoveNum,ifstream currentBook){

}
*/
void updateHashTable(unsigned long long int hash, int depth,int storedType, int value){
  unordered_map<unsigned long long int,HashTuple>::const_iterator got = HashMap.find (hash);
  if(got == HashMap.end()){
    HashTuple newInsert;
    //array<int,4> insertMove = {move[0],move[1],move[2],move[3]};
    newInsert.depth = depth;
    newInsert.storedType = storedType;
    newInsert.value = value;
    HashMap.insert(make_pair(hash,newInsert));
  }
  else if(got->second.depth <= depth){
    //HashMap.erase(hash);
    HashTuple newInsert;
    newInsert.depth = depth;
    newInsert.storedType = storedType;
    newInsert.value = value;
    //newInsert.move = insertMove;
    HashMap.insert(make_pair(hash,newInsert));
  }
}
/*
void updateHashTable(unsigned long long int hash, int depth,int storedType, int value){
  if(HashTable[hash % tableSize].depth <= depth){
    totalPut++;
    HashTable[hash % tableSize].accessed = true;
    HashTable[hash % tableSize].depth = depth;
    HashTable[hash % tableSize].storedType = storedType;
    //HashTable[hash % tableSize].board = board;
    HashTable[hash % tableSize].value = value;
  }
}
*/
array<int,5> Search(int depth, Board myBoard,int alpha, int beta, unsigned long long int hash, array<int,4> prevMove){
  int** board = myBoard.getBoard();
  if(depth == 0){
    return {-1,-1,-1,-1,evaluate(myBoard,board)};
  }

  unordered_map<unsigned long long int,HashTuple>::const_iterator got = HashMap.find (hash);
  if(got != HashMap.end() && got->second.depth >= depth){
    //cout << "here" << endl;
    //boardPrinter(myBoard.getBoard());
    //sleep(999);
    int realValue = mateScore(got->second.value,depth);

    if(got->second.storedType == 1){
      skipped++;
      return {-1,-1,-1,-1,realValue};
    }
    if(got->second.storedType == 2 && realValue < alpha){
      skipped++;
      return {-1,-1,-1,-1,realValue};
    }
    if(got->second.storedType == 0 && realValue > beta){
      skipped++;
      return {-1,-1,-1,-1,realValue};
    }
  }


/*
  if(HashTable[hash % tableSize].accessed && HashTable[hash % tableSize].depth >= depth){
    int realValue = mateScore(HashTable[hash % tableSize].value,depth);
    //cout << "type: " << HashTable[hash % tableSize].storedType << endl;

    if(HashTable[hash % tableSize].storedType == 1){
      skipped++;
      return {-1,-1,-1,-1,realValue};
    }
    if(turnColour == white){
      if(HashTable[hash % tableSize].storedType == 2 && realValue < alpha){
        skipped++;
        return {-1,-1,-1,-1,realValue};
      }
      if(HashTable[hash % tableSize].storedType == 0 && realValue > beta){
        skipped++;
        return {-1,-1,-1,-1,realValue};
      }
    }
    else{
      if(HashTable[hash % tableSize].storedType == 0 && realValue > beta){
        skipped++;
        return {-1,-1,-1,-1,realValue};
      }
      if(HashTable[hash % tableSize].storedType == 2 && realValue < alpha){
        skipped++;
        return {-1,-1,-1,-1,realValue};
      }
    }
  }
*/
  unsigned long long int OGHash = hash;
  accum++;
  array<int,4> bestMove = {-1,-1,-1,-1};
  array<pair<array<int,4>,int>,218> moves = moveOrdering(myBoard,board);

  if(depth == maxDepth){
    array<int,4> temp = prevMove, temp1;
    for(int i = 0; i < 218;i++){
      if(temp[0] == prevMove[0] && temp[1] == prevMove[1] && temp[2] == prevMove[2] && temp[3] == prevMove[3] && i != 0){
        break;
      }
      temp1 = moves[i].first;
      moves[i].first = temp;
      temp = temp1;
    }
  }
  if(depth == 2222){
    cout << "post" << endl;
    for(int i = 0; i < 5; i ++){
      cout << "piece: " << moves[i].first[0] << " " << moves[i].first[1] << " to: " << moves[i].first[2] << " " << moves[i].first[3] << endl;
    }
  }

  if(moves[0].first[0] == -1){
    if(playerInCheck(myBoard)){
      if(turnColour == white){
        return {-1,-1,-1,-1,-numeric_limits<int>::max() + (100000-depth)};
      }
      else{
        return {-1,-1,-1,-1,numeric_limits<int>::max() - (100000+depth)};
      }
    }
    else{
      return {-1,-1,-1,-1,0};
    }
  }
  array<int,5> value;
  array<int,5> best;
  array<int,2> piecePositon;
  array<int,2> move;
  int piece;
  int storedType;

  if(turnColour == white){
    storedType = 2;
  }
  else{
    storedType = 0;
  }

  int i = 0;
  clock_t tempEnd;
  string boardFEN;
  unsigned long long int prevHash;
  if(turnColour == white){
    best = {prevMove[0],prevMove[1],prevMove[2],prevMove[3],-numeric_limits<int>::max()};
    while(moves[i].first[0] != -1){
      //storedType = 2;
      /*
      cout << "white turn: " << depth << endl;
      cout << "initAlpha: " << alpha << endl;
      cout << "initBeta: " << beta << endl;
      */
      piecePositon = {moves[i].first[0],moves[i].first[1]};
      move = {moves[i].first[2], moves[i].first[3]};
      /*
      if(depth == maxDepth){
        cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
        cout << "move is: " << move[0] << " " << move[1] << endl;
      }
      */
      /*
      cout << "white turn: " << depth << endl;
      cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
      cout << "move is: " << move[0] << " " << move[1] << endl;
      cout << "depth: " << depth << endl;
      */
      int actualPiece = getPieceHash(board[piecePositon[0]][piecePositon[1]]);
      piece = myBoard.make_move(piecePositon,move);
      hash = makeMoveHash(piecePositon,move,actualPiece,piece,hash,myBoard);
      //cout << "enterBlack"<< endl;
      if(maxDepth == depth){
        boardFEN = boardToFEN(myBoard.getBoard());
        auto got = GameLog.find (boardFEN);
        if(got != GameLog.end() && got->second > 1){
          value = {moves[i].first[0],moves[i].first[1],moves[i].first[2],moves[i].first[3],0};
        }
        else{
          value = Search(depth-1,myBoard, alpha, beta, hash,prevMove);
        }
      }
      else{
        value = Search(depth-1,myBoard, alpha, beta, hash,prevMove);
      }
      //cout << "exitBlack" << endl;
      /*
      auto got = GameLog.find (hash);
      if(got != GameLog.end()){
        got->second++;
      }
      else{
        GameLog.insert(make_pair(hash,1));
        got = GameLog.find(hash);
      }
      if(got->second < 3){
        value = Search(depth-1,myBoard, alpha, beta, hash,prevMove);
      }
      else{
        value = {moves[i].first[0],moves[i].first[1],moves[i].first[2],moves[i].first[3],0};
      }
      if(got != GameLog.end()){
        got->second--;
      }
      else{
        auto got = GameLog.find (hash);
        got->second--;
      }
      */
      hash = unmakeMoveHash(move,piecePositon,actualPiece,piece,hash,myBoard);
      myBoard.unmake_move(move,piecePositon,piece);
      tempEnd = clock();
      if(((float) tempEnd - start)/CLOCKS_PER_SEC > maxTime){
        break;
      }

      if(depth == 111){
        cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
        cout << "move is: " << move[0] << " " << move[1] << endl;
        cout << "value is: " << value[4] << endl;
      }
      if(value[4] > best[4]){
        best[0] = piecePositon[0];
        best[1] = piecePositon[1];
        best[2] = move[0];
        best[3] = move[1];
        best[4] = value[4];
        //cout << "new best" << endl;
      }
      //cout << "value is: " << value[4] << endl;
      if(best[4] > alpha){
        storedType = 1;
        alpha = best[4];
        //cout << "new alpha" << endl;

        //cout << "new alpha: " << alpha << endl;
      }

      /*
      if(HashTable[hash % tableSize].accessed){
        if(HashTable[hash % tableSize].depth < depth && sameBoard(board,HashTable[hash % tableSize].board)){
          HashTable[hash % tableSize].depth = depth;
          HashTable[hash % tableSize].value = alpha;
        }

      }
      else{
        totalPut++;
        HashTable[hash % tableSize].accessed = true;
        HashTable[hash % tableSize].depth = depth;
        HashTable[hash % tableSize].value = alpha;
        HashTable[hash % tableSize].board = board;
        HashTable[hash % tableSize].storedType = storedType;
      }
      prevHash = hash;
      */
      //cout << "move" << endl;
      if(beta <= alpha){
        storedType = 2;
        //cout << "breaking" << endl;
        break;
        //updateHashTable(hash,depth,board,0,beta);
        //best[4] = beta;
        //return best;
      }
      i++;

    }
  }
  else{
    best = {prevMove[0],prevMove[1],prevMove[2],prevMove[3],numeric_limits<int>::max()};
    while(moves[i].first[0] != -1){
      //storedType = 0;

      ////cout << "black turn: " << depth << endl;
      //cout << "initAlpha: " << alpha << endl;
      //cout << "initBeta: " << beta << endl;

      piecePositon = {moves[i].first[0],moves[i].first[1]};
      move = {moves[i].first[2], moves[i].first[3]};
      /*
      if(depth == maxDepth){
        cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
        cout << "move is: " << move[0] << " " << move[1] << endl;
      }
      */
      //cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
      //cout << "move is: " << move[0] << " " << move[1] << endl;
      int actualPiece = getPieceHash(board[piecePositon[0]][piecePositon[1]]);
      piece = myBoard.make_move(piecePositon,move);

      hash = makeMoveHash(piecePositon,move,actualPiece,piece,hash,myBoard);
      //cout << "enterBlack" << endl;
      if(depth == maxDepth){
        boardFEN = boardToFEN(myBoard.getBoard());
        auto got = GameLog.find (boardFEN);
        if(got != GameLog.end() && got->second > 1){
          value = {moves[i].first[0],moves[i].first[1],moves[i].first[2],moves[i].first[3],0};
          //value = Search(depth-1,myBoard, alpha, beta, hash,prevMove);
        }
        else{
          value = Search(depth-1,myBoard, alpha, beta, hash,prevMove);
        }
      }
      else{
        value = Search(depth-1,myBoard, alpha, beta, hash,prevMove);
      }
      //cout << "exitBlack" << endl;
      /*
      auto got = GameLog.find (hash);
      if(got != GameLog.end()){
        got->second++;
      }
      else{
        GameLog.insert(make_pair(hash,1));
        got = GameLog.find(hash);
      }
      if(got->second < 3){
        value = Search(depth-1,myBoard, alpha, beta, hash,prevMove);
      }
      else{
        value = {moves[i].first[0],moves[i].first[1],moves[i].first[2],moves[i].first[3],0};
      }
      got->second--;
      */
      hash = unmakeMoveHash(move,piecePositon,actualPiece,piece,hash,myBoard);
      myBoard.unmake_move(move,piecePositon,piece);
      tempEnd = clock();
      if(((float) tempEnd - start)/CLOCKS_PER_SEC > maxTime){
        break;
      }
      //cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
      //cout << "move is: " << move[0] << " " << move[1] << endl;
      //cout << "value is: " << value[4] << endl;
      //cout << "got value" << endl;
      //value[4] = value[4] * -1;
      if(value[4] < best[4]){
        best[0] = piecePositon[0];
        best[1] = piecePositon[1];
        best[2] = move[0];
        best[3] = move[1];
        best[4] = value[4];
      }

      if(depth == 99999){
        cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
        cout << "move is: " << move[0] << " " << move[1] << endl;
        cout << "value is: " << value[4] << endl;
        cout << "beta is: " << beta << endl;
      }
      /*
      else{
        cout << "black" << endl;
        cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
        cout << "move is: " << move[0] << " " << move[1] << endl;
        cout << "value is: " << value[4] << endl;
        cout << "beta is: " << beta << endl;
        cout << "depth: " << depth << endl;
      }
      */
      //beta = min(beta,value[4]);

      //cout << "value is: " << value[4] << endl;
      if(best[4] < beta){
        storedType = 1;
        beta = best[4];
        //cout << "new beta: " << beta << endl;
      }
      /*
      if(!newMoveResultsInCheck(myBoard)){
        value = Search(depth-1,myBoard, alpha, beta, hash);
        //cout << "got value" << endl;
        //value[4] = value[4] * -1;
        if(value[4] < best[4]){
          best[0] = piecePositon[0];
          best[1] = piecePositon[1];
          best[2] = move[0];
          best[3] = move[1];
          best[4] = value[4];
        }
        if(depth == 10){
          cout << "piece is: " << piecePositon[0] << " " << piecePositon[1] << endl;
          cout << "move is: " << move[0] << " " << move[1] << endl;
          cout << "value is: " << value[4] << endl;
          cout << "beta is: " << beta << endl;
        }
        //beta = min(beta,value[4]);

        //cout << "value is: " << value[4] << endl;
        if(value[4] < beta){
          beta = value[4];
          //cout << "new beta: " << beta << endl;
        }
      }
      */
      /*
      if(hashAlreadyAccessed(hash,100)){
        bitset<64> hash64(hash);
        if(HashTable[(int)subset(hash64,48,64).to_ulong()].depth > depth){
          HashTable[(int)subset(hash64,48,64).to_ulong()].value = value[4];
          HashTable[(int)subset(hash64,48,64).to_ulong()].depth = depth;
        }
      }
      */
      /*
      if(HashTable[hash % tableSize].accessed){

        if(HashTable[hash % tableSize].depth < depth && sameBoard(board,HashTable[hash % tableSize].board)){
          HashTable[hash % tableSize].depth = depth;
          HashTable[hash % tableSize].value = beta;
        }

      }
      else{
        totalPut++;
        HashTable[hash % tableSize].accessed = true;
        HashTable[hash % tableSize].depth = depth;
        HashTable[hash % tableSize].value = beta;
        HashTable[hash % tableSize].board = board;
        HashTable[hash % tableSize].storedType = storedType;
      }
      */
      /*
      else{
        totalPut++;
        newHash(hash,depth,value[4]);
      }
      */
      /*
      if(HashTable[hash % tableSize].accessed){
        if(HashTable[hash % tableSize].depth <= depth){
          HashTable[hash % tableSize].value = value[4];
        }
      }
      else{
        totalPut++;
        HashTable[hash % tableSize].accessed = true;
        HashTable[hash % tableSize].depth = depth;
        HashTable[hash % tableSize].value = value[4];
      }
      */


      if(beta <= alpha){
        storedType = 0;
        //updateHashTable(hash,depth,board,2,alpha);
        //best[4] = alpha;
        //return best;
        break;
      }
      i++;
    }
  }
  //delete moves.moves;
  //delete moves.moveValues;
  updateHashTable(hash,depth,storedType,best[4]);
  return best;
}

string initProperties(string startFen){
  whiteKingRookMoved = true;
  whiteQueenRookMoved = true;
  blackKingRookMoved = true;
  blackQueenRookMoved = true;
  blackKingMoved = false;
  whiteKingMoved = false;
  // rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq 4
  size_t pos = 0;
  string delimiter = " ";
  pos = startFen.find(delimiter);
  string boardFen = startFen.substr(0,pos);
  startFen.erase(0,pos + delimiter.length());
  pos = startFen.find(delimiter);

  string turnColourFEN = startFen.substr(0,pos);

  if(turnColourFEN == "w"){
    turnColour = white;
  }
  else{
    turnColour = black;
  }
  startFen.erase(0,pos +delimiter.length());

  pos = startFen.find(delimiter);
  string castlePropertiesFEN = startFen.substr(0,pos);
  for(int i = 0; i < castlePropertiesFEN.length();i++){
    if(castlePropertiesFEN[i] == 'K'){
      whiteKingRookMoved = false;
    }
    if(castlePropertiesFEN[i] == 'Q'){
      whiteQueenRookMoved = false;
    }
    if(castlePropertiesFEN[i] == 'k'){
      blackKingRookMoved = false;
    }
    if(castlePropertiesFEN[i] == 'q'){
      blackQueenRookMoved = false;
    }
  }
  if(whiteKingRookMoved && whiteQueenRookMoved){
    whiteKingMoved = true;
  }
  if(blackKingRookMoved && blackQueenRookMoved){
    blackKingMoved = true;
  }
  startFen.erase(0,pos +delimiter.length());

  pos = startFen.find(delimiter);
  string stringEnPaisantFEN = startFen.substr(0,pos);
  char EnPaisantFEN = stringEnPaisantFEN[0];
  startFen.erase(0,pos +delimiter.length());

  if(EnPaisantFEN != '-'){
    if(turnColour == white){
      whiteEnPaisant =  EnPaisantFEN - '0';
    }
    else{
      blackEnPaisant = EnPaisantFEN - '0';
    }
  }

  pos = startFen.find(delimiter);
  string maxTimeFEN = startFen.substr(0,pos);

  if(maxTimeFEN == "1"){
    maxTime = 1.0;
  }
  else if(maxTimeFEN == "3"){
    maxTime = 3.0;
  }
  else{
    maxTime = 10.0;
  }

  startFen.erase(0,pos +delimiter.length());

  return boardFen;
}

int runner(char* startFen){
  string str(startFen);
  srand((unsigned)time(0));
  initOpeningTable();
  // rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq 4
  string strStartFen = initProperties(startFen);
  cout << "turnColour: " << turnColour << endl;
  /*
  ifstream file("./Book/Catalan3Bb4.pgn");
  string str;
  //string file_contents;
  ofstream myfile;
  myfile.open("MasterBook.txt", ios_base::app);
  while (std::getline(file, str))
  {
    if(str[0] != '['){
      myfile << str << endl;
    }
  }
  myfile.close();
  cout << "done" << endl;

  return 0;
  sleep(999);
  */
/*
  string move = "exd5 Nxd5";
  string moveNum = " 9.";
  string actualNum = "9.";
  ifstream file("CurrentBook.txt");
  string str;
  //string file_contents;
  //ofstream myfile;
  //myfile.open("obscureBook.txt");
  //vector<string> possibleMoves;
  //ofstream myfile;
  //myfile.open("CurrentBook.txt");
  string gameString;
  bool sameGame;
  int accum = 0;
  while (std::getline(file, str))
  {
    //cout << accum << endl;
    sameGame = true;
    if(str.find(moveNum) != string::npos){
      //cout << str << endl;
      int index = str.find(actualNum) + actualNum.size();
      //cout << str.find(actualNum) << endl;
      //cout << str[index+2] << endl;

      for(int i = 0; i < move.length();i++){
        if(str[index+i] != move[i]){
          sameGame = false;
          break;
        }
      }

      if(sameGame){
        //cout << "passed" << endl;
        //sleep(999);
        //myfile << str << endl;
        gameString = str + '\n';
        //cout << str << endl;
        //numLines = 0;
        while(!str.empty() && getline(file,str)){
          //myfile << str << endl;
          gameString += str + '\n';
          break;
          //numLines++;
          //cout << str << endl;
        }
        bookMoves[accum] = gameString;
        accum++;
        if(accum%10000 == 0){
          cout << accum << endl;
        }
        //possibleMoves.push_back(gameString);
      }
      //cout << endl;
      //myfile << str << endl;
    }
      //cout << str << endl;
      //myfile << str;
      //myfile << "\n";
  }

  cout << accum << endl;
  //sleep(9999);
  //myfile.close();
  //cout << "done" << endl;
  string selectedMove = bookMoves[rand() % accum];
  string movePos = actualNum + move;
  cout << selectedMove << endl;
  int actualMovePos = selectedMove.find(movePos) + movePos.length() + 1;
  cout << actualMovePos << endl;
  string moveString;
  while(selectedMove[actualMovePos] != ' '){
    moveString += selectedMove[actualMovePos];
    actualMovePos++;
  }
  int index = selectedMove.find(movePos) + movePos.length() + 1;
  cout << moveString << endl;
  cout << index << endl;
  ofstream myfile;
  myfile.open("CurrentBook.txt");
  bool keepGame;
  for(int i = 0; i < accum;i++)
  {
    keepGame = true;
    for(int j = 0; j < moveString.length();j++){
      if(bookMoves[i][index+j] != moveString[j]){
        keepGame = false;
        break;
      }
    }
    if(keepGame){
      myfile << bookMoves[i] << endl;
      //gameString = str;
      //cout << str << endl;
      //while(!str.empty() && getline(file,str)){
      //  myfile << str << endl;
        //gameString += str;
        //cout << str << endl;
      //}
    }
  }
  cout << "done" << endl;
  myfile.close();
  return 0;
*/

    start = clock();
    int** board = initBoard();
    /*
    unordered_map<string,int> moveMap;
    int moveNum = 1;
    int lineNum = 0;
    int gameNum = 3;
    string strMoveNum = " " +to_string(moveNum) + ".";
    string strGameNum = to_string(gameNum);
    string outputStr = "";
    string str;
    string subStr = "";
    bool repeat = true;
    string moveStr = "";
    //ifstream hashFile("MasterHashs.txt");
    //ifstream gameFile("MasterGames.txt");
    ifstream file("Newe4Book.txt");
    ifstream endfile("Newe4Book.txt");

    //ifstream hashFile("MasterHash.txt");
    //Bacrot Done
    ofstream currentfile;
    currentfile.open("CurrentBook.txt");
    /*
    ofstream e4file;
    e4file.open("Newe4Book.txt");
    ofstream d4file;
    d4file.open("Newd4Book.txt");
    ofstream obscurefile;
    obscurefile.open("NewobscureBook.txt");
    */
    //myfile.open("MasterHashs.txt",ios_base::app);
    //myfile.open("NewMasterBook.txt");
    int count = 0;
    /*
    while(getline(file,str)){
      auto found = str.find(" 1.");
      if(found != string::npos){
        while((int)str[found] != 32){
          found++;
        }
        found++;
        while((int)str[found] != 32){
          found++;
        }
        found++;
        while((int)str[found] != 32){
          moveStr += str[found];
          found++;
        }
        if((int)moveStr[0] == 0){
          continue;
        }
        auto moveFound = moveMap.find(moveStr);
        if(moveFound == moveMap.end()){
          moveMap.insert(make_pair(moveStr,1));
        }
        else{
          moveMap[moveFound->first]++;
        }
        moveStr = "";
        count++;
      }
    }
    for(auto it=moveMap.begin();it!=moveMap.end();it++) {
      cout<< (it->first) << " : " << it->second << endl;
    }
    cout << count << endl;
    random_device rd;
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen {seed};
    uniform_int_distribution<long long unsigned> distribution(0,count);

    std::uint64_t choice = distribution(gen);
    //int choice = Random::get(0,count);
    cout << choice << endl;
    int current = 0;
    auto it = moveMap.begin();
    while(it != moveMap.end()){
      current += it->second;
      if(current >= choice){
        break;
      }
      it++;
    }
    cout << it->first << endl;
    string finalMove = it->first;
    int accum = 0;
    string oppMove = " 1.e4 ";
    string cmplStr = oppMove + finalMove;
    cout << cmplStr << endl;

    while(getline(endfile,str)){
      if(str.find(cmplStr) != string::npos){
        while((int)str[0] != 0){
          currentfile << str << endl;
          getline(endfile,str);
        }
        currentfile << endl;
      }
    }
    /*
    while (std::getline(file, str)){
      if((int)str[0] != 0 && count < 3){
        myfile << " " + str << endl;
        count++;
        accum++;
      }
      else{
        while((int)str[0] != 0){
          getline(file,str);
          accum++;
        }
        count = 0;
        myfile << endl;
      }
      if(accum % 25000 == 0){
        cout << accum << endl;
      }
    }
    accum = 0;
    count = 0;
    while (std::getline(oldfile, str)){
      if((int)str[0] != 0 && count < 3){
        myfile << " " + str << endl;
        count++;
        accum++;
      }
      else{
        while((int)str[0] != 0){
          getline(oldfile,str);
          accum++;
        }
        count = 0;
        myfile << endl;
      }
      if(accum % 25000 == 0){
        cout << accum << endl;
      }
    }
    */
    /*
    while(getline(file,str)){
      auto found = str.find(" 1.e4");
      if(found != string::npos){
        while((int)str[0] != 0){
          e4file << str << endl;
          getline(file,str);
        }
        e4file << endl;
      }
      else{
        found = str.find(" 1.d4");
        if(found != string::npos){
          while((int)str[0] != 0){
            d4file << str << endl;
            getline(file,str);
          }
          d4file << endl;
        }
        else if((int)str[0] != 0){
          while((int)str[0] != 0){
            obscurefile << str << endl;
            getline(file,str);
          }
          obscurefile << endl;
        }
      }
    }
    */
    cout << "done" << endl;

    clock_t tempEnd = clock();
    cout << (((float) tempEnd - start)/CLOCKS_PER_SEC) << endl;
    //return 0;
    array<bool,6> castleProperties = {whiteKingMoved,whiteKingRookMoved,whiteQueenRookMoved,blackKingMoved,blackKingRookMoved,blackQueenRookMoved};
    array<int,2> EnPaisantProperties = {whiteEnPaisant,blackEnPaisant};
    Board myBoard(board,castleProperties,EnPaisantProperties);

    cout << "done board" << endl;

    //const string startFen = "5r2/pp4p1/2kp4/2p3B1/2P1PPp1/6K1/PP5r/4R3 b - - 5 29";
    loadBoardFromFen(strStartFen, board, myBoard);
    //turnColour = black;
    //unsigned long long int openingHash = computeOpeningHash(board, myBoard);
    int j;
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
    boardPrinter(board);
    //cout << openingHash << endl;
    string hashStr;
    string gameStr;
    int index = 0;
    //string strHash = to_string(openingHash);
    //blackOpeningMove(strHash,strMoveNum)
    /*
    while(getline(hashFile,hashStr)){
      auto found = hashStr.find(strHash);
      index++;
      if(index%20000 == 0){
        cout << index << endl;
      }
    }
    */
    //sleep(9999);
    /*
    while(getline(file,str)){
      if(str[0] != '['){
        myfile << str << endl;
      }
    }
    */
/*
    while(getline(file,str)){
      if(str[0] == '1' && str[1] == '.'){
        str = "Game:" + strGameNum + " " + str;
        gameNum++;
        strGameNum = to_string(gameNum);
      }
      myfile << str << endl;
    }
*/
/*
    while (std::getline(file, str))
    {
      //cout << "newline: " << str << endl;
      //cout << lineNum << endl;
      if(((int)str[0] == 0 || lineNum == 4) && !repeat){
        //continue;
        repeat = true;
        //cout << "Game:" + strGameNum << endl;
        outputStr = "Game:" + strGameNum + outputStr;
        myfile << outputStr << endl;
        myfile << endl;
        outputStr = "";
        //myBoard.deleteBoard();
        deleteBoard(board);
        myBoard.deletePieceLists();
        board = initBoard();
        //Board myBoard(board,castleProperties,EnPaisantProperties);
        myBoard.newBoard(board);
        myBoard.setupPieceLists();
        loadBoardFromFen(startFen, board, myBoard);
        //myBoard.newBoard(board);
        turnColour = white;
        openingHash = computeOpeningHash(board, myBoard);
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
        moveNum = 1;
        strMoveNum = to_string(moveNum);
        gameNum++;
        strGameNum = to_string(gameNum);
        int breaker = 0;

        while((int)str[0] != 0 && breaker < 50){
          getline(file, str);
          breaker++;
        }
        //cout << "newGame: " << str << endl;
        lineNum = 0;

        continue;
      }
      for(int i = 0; i < str.size();i++){
        //.e4
        if(str[i] == '.'){
          repeat = false;
          //cout << strMoveNum << endl;
          turnColour = white;
          j = i+1;
          while((int)str[j] != 32){
            subStr += str[j];
            j++;
          }
          //cout << "subStr: " << subStr << endl;
          auto found = subStr.find('=');
          if(found != string::npos){
            subStr = "";
            lineNum = 3;
            //cout << "break" << endl;
            break;
          }
          outputStr += " " + strMoveNum + ". ";
          array<int,4> res = PGNtoMove(subStr, board, myBoard);
          array<int,2> initPos = {res[0],res[1]};
          array<int,2> endPos = {res[2],res[3]};
          string moveStr = to_string(res[0]) + to_string(res[1]) + to_string(res[2]) + to_string(res[3]);
          //cout << res[0] << " " << res[1] << " to: " << res[2] << " " << res[3] << endl;
          int pieceHash = getPieceHash(board[res[0]][res[1]]);
          int capturedPiece = myBoard.make_move(initPos,endPos);
          openingHash = makeOpeningHash(initPos,endPos,pieceHash,capturedPiece,openingHash,myBoard);
          outputStr += moveStr + " " + to_string(openingHash) + " ";
          //cout << outputStr << endl;
          //sleep(999);
          subStr = "";
        }
        else if(str[i] == ' '){
          int intValue = (int)str[i+1];
          // " e6"
          if(intValue > 58){
            repeat = false;
            turnColour = black;
            j = i+1;
            while((int)str[j] != 32 && (int)str[j] != 0){
              subStr += str[j];
              j++;
            }
            //cout << "subStr: " << subStr << endl;
            auto found = subStr.find('=');
            if(found != string::npos){
              subStr = "";
              lineNum = 3;
              //cout << "break" << endl;
              break;
            }
            array<int,4> res = PGNtoMove(subStr, board, myBoard);
            array<int,2> initPos = {res[0],res[1]};
            array<int,2> endPos = {res[2],res[3]};
            string moveStr = to_string(res[0]) + to_string(res[1]) + to_string(res[2]) + to_string(res[3]);
            //cout << res[0] << " " << res[1] << " to: " << res[2] << " " << res[3] << endl;
            int pieceHash = getPieceHash(board[res[0]][res[1]]);
            int capturedPiece = myBoard.make_move(initPos,endPos);
            openingHash = makeOpeningHash(initPos,endPos,pieceHash,capturedPiece,openingHash,myBoard);
            outputStr += moveStr + " " + to_string(openingHash);
            //cout << outputStr << endl;
            subStr = "";
            moveNum++;
            strMoveNum = to_string(moveNum);
          }
        }
      }
      lineNum++;
    }
*/
/*
    while(getline(file,str)){
      if(((int)str[0] == 0 || lineNum == 4) && !repeat){
        repeat = true;
        if((int)outputStr[1] == 0){
          continue;
        }
        outputStr = "Game:" + strGameNum + outputStr;
        myfile << outputStr << endl;
        myfile << endl;
        outputStr = "";
        gameNum++;
        strGameNum = to_string(gameNum);
        moveNum = 0;
        strMoveNum = to_string(moveNum);
        lineNum = 0;
        int breaker = 0;
        while((int)str[0] != 0 && breaker < 50){
          getline(file, str);
          breaker++;
        }
        continue;
      }
      repeat = false;
      auto found = str.find('=');
      if(found != string::npos){
        lineNum = 4;
        //cout << "break" << endl;
        continue;
      }
      outputStr = outputStr + " " + str;
      lineNum++;
    }
*/

    //outputStr = "Game:" + strGameNum  + outputStr;
    //myfile << outputStr << endl;
    //myfile << endl;
    //cout << outputStr << endl;
    cout << "done" << endl;
    //sleep(99999);
    /*
    array<int,4> res = PGNtoMove("e4", board, myBoard);
    array<int,2> initPos = {res[0],res[1]};
    array<int,2> endPos = {res[2],res[3]};
    cout << res[0] << " " << res[1] << " to: " << res[2] << " " << res[3] << endl;
    int pieceHash = getPieceHash(board[res[0]][res[1]]);
    int capturedPiece = myBoard.make_move(initPos,endPos);
    openingHash = makeOpeningHash(initPos,endPos,pieceHash,capturedPiece,openingHash,myBoard);
    cout << "new Hash: " << openingHash << endl;
    sleep(999);
    */
    //array<int,4> thisMove = PGNtoMove("Qxd5",board,myBoard);
    //cout << thisMove[0] << " " << thisMove[1] << " to: " << thisMove[2] << " " << thisMove[3] << endl;
    //sleep(9999);
    cout << "endGame?: " << endGame(myBoard) << endl;


    boardPrinter(board);
    //cout << endGame(myBoard) << endl;

    initTable();
    //int** goodBoard = myBoard.getBoard();
    unsigned long long int h = computeHash(board, myBoard);
    cout << "init hash: " << h << endl;

    bitset<64> hash64(h);
    cout << evaluate(myBoard,board) << endl;

    //cout << project_range<6,8>(hash64).to_ulong() << endl;
    /*
    int x = 0;
    cout << x << endl;
    adder(x);
    cout << x << endl;
    */
    //cout << MoveGenerator(4,myBoard) << endl;
    int i = 2;
    if(endGame(myBoard)){
      i = 2;
    }
    start = clock();
    cout << "maxTime: " << maxTime << endl;
    //clock_t tempEnd = clock();
    maxDepth = 800;
    cout << "entering: " << i-1 << endl;
    array<int,5> result = Search(i-1,myBoard, -numeric_limits<int>::max(),numeric_limits<int>::max(),h,{0,0,0,0});
    cout << "Piece is: " << result[0] << " " << result[1] << endl;
    cout << "Move is: " << result[2] << " " << result[3] << endl;
    cout << "Score is: " << result[4] << endl;
    cout << "skipped is: " << skipped << endl;
    cout << "accum is: " << accum << endl;
    tempEnd = clock();
    cout << (((float) tempEnd - start)/CLOCKS_PER_SEC) << endl;
    //sleep(999999);
    //sleep(9999999);
    //sleep(999999);
    int boundAlpha = result[4] - 100;
    int boundBeta = result[4] + 100;
    bool prevBeta = false;
    bool prevAlpha = false;
    array<int,4> prev = {result[0],result[1],result[2],result[3]};
    int prevResult;

    while(((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime && abs(result[4]) < 100000){
    //while(i < 10){
      prevResult = result[4];
      prev = {result[0],result[1],result[2],result[3]};
      maxDepth = i;
      skipped = 0;
      accum = 0;
      cout << endl;
      cout << "entering: " << i << endl;
      cout << "alphaBound: " << boundAlpha << endl;
      cout << "betaBound: " << boundBeta << endl;
      result = Search(i,myBoard,boundAlpha,boundBeta,h,prev);
      tempEnd = clock();

      if(((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime && (result[4] >= boundBeta || result[4] <= boundAlpha) && abs(result[4]) < 100000){
        //prev = {result[0],result[1],result[2],result[3]};
        cout << "Piece is: " << result[0] << " " << result[1] << endl;
        cout << "Move is: " << result[2] << " " << result[3] << endl;
        cout << "Score is: " << result[4] << endl;
        if(result[4] >= boundBeta){
          boundBeta = numeric_limits<int>::max();
          boundAlpha = result[4]-25;
        }
        else{
          boundAlpha = -numeric_limits<int>::max();
          boundBeta = result[4]+25;
        }

        cout << "entering: " << i << endl;
        cout << "alphaBound: " << boundAlpha << endl;
        cout << "betaBound: " << boundBeta << endl;
        result = Search(i,myBoard,boundAlpha,boundBeta,h,prev);
        tempEnd = clock();
      }

      else if(result[4] < boundBeta && result[4] > boundAlpha){
        cout << "getting stopped early vals OLD" << endl;
        prevResult = result[4];
        prev = {result[0],result[1],result[2],result[3]};
      }

      //prevResult = result[4];
      //prev = {result[0],result[1],result[2],result[3]};

  /*
      if(result[4] >= boundBeta){
        if(prevBeta){
          boundBeta = result[4] + 100;
        }
        else{
          boundBeta = result[4] + 100;
        }
        boundAlpha = result[4] - 25;
        prevBeta = true;
        prevBeta = false;
      }
      else if(result[4] <= boundAlpha){
        if(prevAlpha){
          boundAlpha = result[4] - 100;
        }
        else{
          boundAlpha = result[4] - 100;
        }
        boundBeta = result[4] + 25;
        prevAlpha = true;
        prevBeta = false;
      }
      else{
        boundBeta = result[4] + 25;
        boundAlpha = result[4] - 25;
        prevAlpha = false;
        prevBeta = false;
      }
      */
      boundBeta = result[4] + 25;
      boundAlpha = result[4] - 25;
      cout << "Piece is: " << result[0] << " " << result[1] << endl;
      cout << "Move is: " << result[2] << " " << result[3] << endl;
      cout << "Score is: " << result[4] << endl;
      cout << "skipped is: " << skipped << endl;
      cout << "accum is: " << accum << endl;
      tempEnd = clock();
      cout << (((float) tempEnd - start)/CLOCKS_PER_SEC) << endl;
      i++;
    }
    cout << "out" << endl;
    if(abs(result[4]) < 9999 || ((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime){
      prevResult = result[4];
    }

/*
    cout << "Piece is: " << result[0] << " " << result[1] << endl;
    cout << "Move is: " << result[2] << " " << result[3] << endl;
    cout << "Score is: " << result[4] << endl;

    cout << "skipped is: " << skipped << endl;
    */
    cout << accum << endl;
    cout << totalPut << endl;

    //cout << MoveGenerator(6,myBoard) << endl;

    //int result = MoveGenerator(6, myBoard);
    //cout << result << endl;
    //cout << totalMem << endl;
    //cout << maxMoves << endl;
    /*
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
    actualPins(board,myBoard,result);
    cout << "pinners" << endl;
    cout << (result.pinners == 0) << endl;
    bitBoardPrinter(result.pinners);
    cout << result.numCheckers << endl;
    int index = 0;
    //kingMovesCheck(7, 0, board, index, actualResult, result.kingDangerSquares);
    pawnMoves(3, 1, board, myBoard, index,actualResult);
    //rookMovesPinned(7, 1, board, index, actualResult,myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1]);
    //pawnMovesPinned(6, 0, board, index, actualResult,myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1]);
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
    // /clock_t tempEnd = clock();
    printf ("time: %0.8f sec\n", ((float) tempEnd - start)/CLOCKS_PER_SEC);

    cout << "HashMap size: " << HashMap.max_size() << endl;
    cout << "totalMem: " << totalMem << endl;
    string pieceRank = to_string(prev[0]);
    if(pieceRank == "0"){
      pieceRank = "9";
    }
    cout << "final Piece is: " << prev[0] << " " << prev[1] << endl;
    cout << "final Move is: " << prev[2] << " " << prev[3] << endl;
    string pieceFile = to_string(prev[1]);
    string moveRank = to_string(prev[2]);
    string moveFile = to_string(prev[3]);
    string finalScore;
    if(prevResult > 30000){
      finalScore = "5000";
    }
    else if(prevResult < -30000){
      finalScore = "-5000";
    }
    else{
      finalScore = to_string(prevResult);
    }
    string finalMoveStr;
    if(prevResult < 0){
      cout << "init: " << finalScore << endl;
      finalScore = finalScore.erase(0,1);
      finalMoveStr = pieceRank + pieceFile + moveRank + moveFile + '0' + finalScore;
    }
    else{
      finalMoveStr = pieceRank + pieceFile + moveRank + moveFile + finalScore;
    }
    int finalMoveInt = stoi(finalMoveStr);
    whiteKingMoved = false;
    blackKingMoved = false;
    cout << "score: " << prevResult << endl;
    return finalMoveInt;
    //return 0;
}

int newRunner(char* startFen){
  string str(startFen);
  //srand(0);
  initOpeningTable();
  // rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq 4
  string strStartFen = initProperties(startFen);
  cout << "fen: " << strStartFen << endl;
  cout << "turnColour: " << turnColour << endl;
  /*
  ifstream file("./Book/Catalan3Bb4.pgn");
  string str;
  //string file_contents;
  ofstream myfile;
  myfile.open("MasterBook.txt", ios_base::app);
  while (std::getline(file, str))
  {
    if(str[0] != '['){
      myfile << str << endl;
    }
  }
  myfile.close();
  cout << "done" << endl;

  return 0;
  sleep(999);
  */
/*
  string move = "exd5 Nxd5";
  string moveNum = " 9.";
  string actualNum = "9.";
  ifstream file("CurrentBook.txt");
  string str;
  //string file_contents;
  //ofstream myfile;
  //myfile.open("obscureBook.txt");
  //vector<string> possibleMoves;
  //ofstream myfile;
  //myfile.open("CurrentBook.txt");
  string gameString;
  bool sameGame;
  int accum = 0;
  while (std::getline(file, str))
  {
    //cout << accum << endl;
    sameGame = true;
    if(str.find(moveNum) != string::npos){
      //cout << str << endl;
      int index = str.find(actualNum) + actualNum.size();
      //cout << str.find(actualNum) << endl;
      //cout << str[index+2] << endl;

      for(int i = 0; i < move.length();i++){
        if(str[index+i] != move[i]){
          sameGame = false;
          break;
        }
      }

      if(sameGame){
        //cout << "passed" << endl;
        //sleep(999);
        //myfile << str << endl;
        gameString = str + '\n';
        //cout << str << endl;
        //numLines = 0;
        while(!str.empty() && getline(file,str)){
          //myfile << str << endl;
          gameString += str + '\n';
          break;
          //numLines++;
          //cout << str << endl;
        }
        bookMoves[accum] = gameString;
        accum++;
        if(accum%10000 == 0){
          cout << accum << endl;
        }
        //possibleMoves.push_back(gameString);
      }
      //cout << endl;
      //myfile << str << endl;
    }
      //cout << str << endl;
      //myfile << str;
      //myfile << "\n";
  }

  cout << accum << endl;
  //sleep(9999);
  //myfile.close();
  //cout << "done" << endl;
  string selectedMove = bookMoves[rand() % accum];
  string movePos = actualNum + move;
  cout << selectedMove << endl;
  int actualMovePos = selectedMove.find(movePos) + movePos.length() + 1;
  cout << actualMovePos << endl;
  string moveString;
  while(selectedMove[actualMovePos] != ' '){
    moveString += selectedMove[actualMovePos];
    actualMovePos++;
  }
  int index = selectedMove.find(movePos) + movePos.length() + 1;
  cout << moveString << endl;
  cout << index << endl;
  ofstream myfile;
  myfile.open("CurrentBook.txt");
  bool keepGame;
  for(int i = 0; i < accum;i++)
  {
    keepGame = true;
    for(int j = 0; j < moveString.length();j++){
      if(bookMoves[i][index+j] != moveString[j]){
        keepGame = false;
        break;
      }
    }
    if(keepGame){
      myfile << bookMoves[i] << endl;
      //gameString = str;
      //cout << str << endl;
      //while(!str.empty() && getline(file,str)){
      //  myfile << str << endl;
        //gameString += str;
        //cout << str << endl;
      //}
    }
  }
  cout << "done" << endl;
  myfile.close();
  return 0;
*/

    start = clock();
    int** board = initBoard();
    /*
    unordered_map<string,int> moveMap;
    int moveNum = 1;
    int lineNum = 0;
    int gameNum = 3;
    string strMoveNum = " " +to_string(moveNum) + ".";
    string strGameNum = to_string(gameNum);
    string outputStr = "";
    string str;
    string subStr = "";
    bool repeat = true;
    string moveStr = "";
    //ifstream hashFile("MasterHashs.txt");
    //ifstream gameFile("MasterGames.txt");
    ifstream file("Newe4Book.txt");
    ifstream endfile("Newe4Book.txt");

    //ifstream hashFile("MasterHash.txt");
    //Bacrot Done
    ofstream currentfile;
    currentfile.open("CurrentBook.txt");
    /*
    ofstream e4file;
    e4file.open("Newe4Book.txt");
    ofstream d4file;
    d4file.open("Newd4Book.txt");
    ofstream obscurefile;
    obscurefile.open("NewobscureBook.txt");
    */
    //myfile.open("MasterHashs.txt",ios_base::app);
    //myfile.open("NewMasterBook.txt");
    int count = 0;
    /*
    while(getline(file,str)){
      auto found = str.find(" 1.");
      if(found != string::npos){
        while((int)str[found] != 32){
          found++;
        }
        found++;
        while((int)str[found] != 32){
          found++;
        }
        found++;
        while((int)str[found] != 32){
          moveStr += str[found];
          found++;
        }
        if((int)moveStr[0] == 0){
          continue;
        }
        auto moveFound = moveMap.find(moveStr);
        if(moveFound == moveMap.end()){
          moveMap.insert(make_pair(moveStr,1));
        }
        else{
          moveMap[moveFound->first]++;
        }
        moveStr = "";
        count++;
      }
    }
    for(auto it=moveMap.begin();it!=moveMap.end();it++) {
      cout<< (it->first) << " : " << it->second << endl;
    }
    cout << count << endl;
    random_device rd;
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen {seed};
    uniform_int_distribution<long long unsigned> distribution(0,count);

    std::uint64_t choice = distribution(gen);
    //int choice = Random::get(0,count);
    cout << choice << endl;
    int current = 0;
    auto it = moveMap.begin();
    while(it != moveMap.end()){
      current += it->second;
      if(current >= choice){
        break;
      }
      it++;
    }
    cout << it->first << endl;
    string finalMove = it->first;
    int accum = 0;
    string oppMove = " 1.e4 ";
    string cmplStr = oppMove + finalMove;
    cout << cmplStr << endl;

    while(getline(endfile,str)){
      if(str.find(cmplStr) != string::npos){
        while((int)str[0] != 0){
          currentfile << str << endl;
          getline(endfile,str);
        }
        currentfile << endl;
      }
    }
    /*
    while (std::getline(file, str)){
      if((int)str[0] != 0 && count < 3){
        myfile << " " + str << endl;
        count++;
        accum++;
      }
      else{
        while((int)str[0] != 0){
          getline(file,str);
          accum++;
        }
        count = 0;
        myfile << endl;
      }
      if(accum % 25000 == 0){
        cout << accum << endl;
      }
    }
    accum = 0;
    count = 0;
    while (std::getline(oldfile, str)){
      if((int)str[0] != 0 && count < 3){
        myfile << " " + str << endl;
        count++;
        accum++;
      }
      else{
        while((int)str[0] != 0){
          getline(oldfile,str);
          accum++;
        }
        count = 0;
        myfile << endl;
      }
      if(accum % 25000 == 0){
        cout << accum << endl;
      }
    }
    */
    /*
    while(getline(file,str)){
      auto found = str.find(" 1.e4");
      if(found != string::npos){
        while((int)str[0] != 0){
          e4file << str << endl;
          getline(file,str);
        }
        e4file << endl;
      }
      else{
        found = str.find(" 1.d4");
        if(found != string::npos){
          while((int)str[0] != 0){
            d4file << str << endl;
            getline(file,str);
          }
          d4file << endl;
        }
        else if((int)str[0] != 0){
          while((int)str[0] != 0){
            obscurefile << str << endl;
            getline(file,str);
          }
          obscurefile << endl;
        }
      }
    }
    */
    cout << "done" << endl;

    clock_t tempEnd = clock();
    cout << (((float) tempEnd - start)/CLOCKS_PER_SEC) << endl;
    //return 0;
    array<bool,6> castleProperties = {whiteKingMoved,whiteKingRookMoved,whiteQueenRookMoved,blackKingMoved,blackKingRookMoved,blackQueenRookMoved};
    array<int,2> EnPaisantProperties = {whiteEnPaisant,blackEnPaisant};
    Board myBoard(board,castleProperties,EnPaisantProperties);

    cout << "done board" << endl;

    //const string startFen = "5r2/pp4p1/2kp4/2p3B1/2P1PPp1/6K1/PP5r/4R3 b - - 5 29";
    loadBoardFromFen(strStartFen, board, myBoard);
    //turnColour = black;
    //unsigned long long int openingHash = computeOpeningHash(board, myBoard);
    int j;
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
    boardPrinter(board);
    //cout << openingHash << endl;
    string hashStr;
    string gameStr;
    int index = 0;
    //string strHash = to_string(openingHash);
    //blackOpeningMove(strHash,strMoveNum)
    /*
    while(getline(hashFile,hashStr)){
      auto found = hashStr.find(strHash);
      index++;
      if(index%20000 == 0){
        cout << index << endl;
      }
    }
    */
    //sleep(9999);
    /*
    while(getline(file,str)){
      if(str[0] != '['){
        myfile << str << endl;
      }
    }
    */
/*
    while(getline(file,str)){
      if(str[0] == '1' && str[1] == '.'){
        str = "Game:" + strGameNum + " " + str;
        gameNum++;
        strGameNum = to_string(gameNum);
      }
      myfile << str << endl;
    }
*/
/*
    while (std::getline(file, str))
    {
      //cout << "newline: " << str << endl;
      //cout << lineNum << endl;
      if(((int)str[0] == 0 || lineNum == 4) && !repeat){
        //continue;
        repeat = true;
        //cout << "Game:" + strGameNum << endl;
        outputStr = "Game:" + strGameNum + outputStr;
        myfile << outputStr << endl;
        myfile << endl;
        outputStr = "";
        //myBoard.deleteBoard();
        deleteBoard(board);
        myBoard.deletePieceLists();
        board = initBoard();
        //Board myBoard(board,castleProperties,EnPaisantProperties);
        myBoard.newBoard(board);
        myBoard.setupPieceLists();
        loadBoardFromFen(startFen, board, myBoard);
        //myBoard.newBoard(board);
        turnColour = white;
        openingHash = computeOpeningHash(board, myBoard);
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
        moveNum = 1;
        strMoveNum = to_string(moveNum);
        gameNum++;
        strGameNum = to_string(gameNum);
        int breaker = 0;

        while((int)str[0] != 0 && breaker < 50){
          getline(file, str);
          breaker++;
        }
        //cout << "newGame: " << str << endl;
        lineNum = 0;

        continue;
      }
      for(int i = 0; i < str.size();i++){
        //.e4
        if(str[i] == '.'){
          repeat = false;
          //cout << strMoveNum << endl;
          turnColour = white;
          j = i+1;
          while((int)str[j] != 32){
            subStr += str[j];
            j++;
          }
          //cout << "subStr: " << subStr << endl;
          auto found = subStr.find('=');
          if(found != string::npos){
            subStr = "";
            lineNum = 3;
            //cout << "break" << endl;
            break;
          }
          outputStr += " " + strMoveNum + ". ";
          array<int,4> res = PGNtoMove(subStr, board, myBoard);
          array<int,2> initPos = {res[0],res[1]};
          array<int,2> endPos = {res[2],res[3]};
          string moveStr = to_string(res[0]) + to_string(res[1]) + to_string(res[2]) + to_string(res[3]);
          //cout << res[0] << " " << res[1] << " to: " << res[2] << " " << res[3] << endl;
          int pieceHash = getPieceHash(board[res[0]][res[1]]);
          int capturedPiece = myBoard.make_move(initPos,endPos);
          openingHash = makeOpeningHash(initPos,endPos,pieceHash,capturedPiece,openingHash,myBoard);
          outputStr += moveStr + " " + to_string(openingHash) + " ";
          //cout << outputStr << endl;
          //sleep(999);
          subStr = "";
        }
        else if(str[i] == ' '){
          int intValue = (int)str[i+1];
          // " e6"
          if(intValue > 58){
            repeat = false;
            turnColour = black;
            j = i+1;
            while((int)str[j] != 32 && (int)str[j] != 0){
              subStr += str[j];
              j++;
            }
            //cout << "subStr: " << subStr << endl;
            auto found = subStr.find('=');
            if(found != string::npos){
              subStr = "";
              lineNum = 3;
              //cout << "break" << endl;
              break;
            }
            array<int,4> res = PGNtoMove(subStr, board, myBoard);
            array<int,2> initPos = {res[0],res[1]};
            array<int,2> endPos = {res[2],res[3]};
            string moveStr = to_string(res[0]) + to_string(res[1]) + to_string(res[2]) + to_string(res[3]);
            //cout << res[0] << " " << res[1] << " to: " << res[2] << " " << res[3] << endl;
            int pieceHash = getPieceHash(board[res[0]][res[1]]);
            int capturedPiece = myBoard.make_move(initPos,endPos);
            openingHash = makeOpeningHash(initPos,endPos,pieceHash,capturedPiece,openingHash,myBoard);
            outputStr += moveStr + " " + to_string(openingHash);
            //cout << outputStr << endl;
            subStr = "";
            moveNum++;
            strMoveNum = to_string(moveNum);
          }
        }
      }
      lineNum++;
    }
*/
/*
    while(getline(file,str)){
      if(((int)str[0] == 0 || lineNum == 4) && !repeat){
        repeat = true;
        if((int)outputStr[1] == 0){
          continue;
        }
        outputStr = "Game:" + strGameNum + outputStr;
        myfile << outputStr << endl;
        myfile << endl;
        outputStr = "";
        gameNum++;
        strGameNum = to_string(gameNum);
        moveNum = 0;
        strMoveNum = to_string(moveNum);
        lineNum = 0;
        int breaker = 0;
        while((int)str[0] != 0 && breaker < 50){
          getline(file, str);
          breaker++;
        }
        continue;
      }
      repeat = false;
      auto found = str.find('=');
      if(found != string::npos){
        lineNum = 4;
        //cout << "break" << endl;
        continue;
      }
      outputStr = outputStr + " " + str;
      lineNum++;
    }
*/

    //outputStr = "Game:" + strGameNum  + outputStr;
    //myfile << outputStr << endl;
    //myfile << endl;
    //cout << outputStr << endl;
    cout << "done" << endl;
    //sleep(99999);
    /*
    array<int,4> res = PGNtoMove("e4", board, myBoard);
    array<int,2> initPos = {res[0],res[1]};
    array<int,2> endPos = {res[2],res[3]};
    cout << res[0] << " " << res[1] << " to: " << res[2] << " " << res[3] << endl;
    int pieceHash = getPieceHash(board[res[0]][res[1]]);
    int capturedPiece = myBoard.make_move(initPos,endPos);
    openingHash = makeOpeningHash(initPos,endPos,pieceHash,capturedPiece,openingHash,myBoard);
    cout << "new Hash: " << openingHash << endl;
    sleep(999);
    */
    //array<int,4> thisMove = PGNtoMove("Qxd5",board,myBoard);
    //cout << thisMove[0] << " " << thisMove[1] << " to: " << thisMove[2] << " " << thisMove[3] << endl;
    //sleep(9999);
    cout << "endGame?: " << endGame(myBoard) << endl;


    boardPrinter(board);
    //cout << endGame(myBoard) << endl;

    initTable();
    //int** goodBoard = myBoard.getBoard();
    unsigned long long int h = computeHash(board, myBoard);
    cout << "init hash: " << h << endl;

    bitset<64> hash64(h);
    cout << evaluate(myBoard,board) << endl;

    fstream gameLogFile("GameLog.txt");
    ofstream outfile;

    if(newGame(myBoard.getBoard())){
      outfile.open("GameLog.txt");
    }
    else{
      outfile.open("GameLog.txt", std::ios_base::app);
    }
    outfile << strStartFen << endl;
    outfile.close();
    string gameFEN;

    //unsigned long long int hashVal;
    //char* end;
    while (getline (gameLogFile, gameFEN)) {
      //const char* c = gameHash.c_str();
      //hashVal = strtoul(c,&end,10);
      //hashVal = stoull(c,nullptr,10);
      auto got = GameLog.find (gameFEN);
      if(got != GameLog.end()){
        got->second++;
      }
      else{
        GameLog.insert(make_pair(gameFEN,1));
      }
}

    Board myBoardCopy = myBoard;

    //cout << project_range<6,8>(hash64).to_ulong() << endl;
    /*
    int x = 0;
    cout << x << endl;
    adder(x);
    cout << x << endl;
    */
    //cout << MoveGenerator(4,myBoard) << endl;
    int i = 2;
    start = clock();
    cout << "maxTime: " << maxTime << endl;
    //clock_t tempEnd = clock();
    maxDepth = 800;
    cout << "entering: " << i-1 << endl;
    array<int,5> result = Search(i-1,myBoard, -numeric_limits<int>::max(),numeric_limits<int>::max(),h,{0,0,0,0});
    cout << "Piece is: " << result[0] << " " << result[1] << endl;
    cout << "Move is: " << result[2] << " " << result[3] << endl;
    cout << "Score is: " << result[4] << endl;
    cout << "skipped is: " << skipped << endl;
    cout << "accum is: " << accum << endl;
    tempEnd = clock();
    cout << (((float) tempEnd - start)/CLOCKS_PER_SEC) << endl;
    //sleep(999999);
    //sleep(9999999);
    //sleep(999999);
    int boundAlpha = result[4] - 100;
    int boundBeta = result[4] + 100;
    bool prevBeta = false;
    bool prevAlpha = false;
    array<int,4> prev = {result[0],result[1],result[2],result[3]};
    int prevResult;
    while(((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime && abs(result[4]) < 100000){
    //while(i < 10){
      prevResult = result[4];
      prev = {result[0],result[1],result[2],result[3]};
      maxDepth = i;
      skipped = 0;
      accum = 0;
      cout << endl;
      cout << "entering: " << i << endl;
      cout << "alphaBound: " << boundAlpha << endl;
      cout << "betaBound: " << boundBeta << endl;
      result = Search(i,myBoard,boundAlpha,boundBeta,h,prev);
      tempEnd = clock();

      if(((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime && (result[4] >= boundBeta || result[4] <= boundAlpha) && abs(result[4]) < 100000){
        //prev = {result[0],result[1],result[2],result[3]};
        //prevResult = result[4];
        cout << "Piece is: " << result[0] << " " << result[1] << endl;
        cout << "Move is: " << result[2] << " " << result[3] << endl;
        cout << "Score is: " << result[4] << endl;
        if(result[4] >= boundBeta){
          boundBeta = numeric_limits<int>::max();
          boundAlpha = result[4]-25;
        }
        else{
          boundAlpha = -numeric_limits<int>::max();
          boundBeta = result[4]+25;
        }

        cout << "entering: " << i << endl;
        cout << "alphaBound: " << boundAlpha << endl;
        cout << "betaBound: " << boundBeta << endl;
        result = Search(i,myBoard,boundAlpha,boundBeta,h,prev);
        tempEnd = clock();
      }

      if(result[4] < boundBeta && result[4] > boundAlpha || (abs(result[4]) > 10000 && ((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime)){
        cout << "getting stopped early vals NEW" << endl;
        prevResult = result[4];
        prev = {result[0],result[1],result[2],result[3]};
      }

      //prevResult = result[4];
      //prev = {result[0],result[1],result[2],result[3]};

  /*
      if(result[4] >= boundBeta){
        if(prevBeta){
          boundBeta = result[4] + 100;
        }
        else{
          boundBeta = result[4] + 100;
        }
        boundAlpha = result[4] - 25;
        prevBeta = true;
        prevBeta = false;
      }
      else if(result[4] <= boundAlpha){
        if(prevAlpha){
          boundAlpha = result[4] - 100;
        }
        else{
          boundAlpha = result[4] - 100;
        }
        boundBeta = result[4] + 25;
        prevAlpha = true;
        prevBeta = false;
      }
      else{
        boundBeta = result[4] + 25;
        boundAlpha = result[4] - 25;
        prevAlpha = false;
        prevBeta = false;
      }
      */
      boundBeta = result[4] + 25;
      boundAlpha = result[4] - 25;
      cout << "Piece is: " << result[0] << " " << result[1] << endl;
      cout << "Move is: " << result[2] << " " << result[3] << endl;
      cout << "Score is: " << result[4] << endl;
      cout << "skipped is: " << skipped << endl;
      cout << "accum is: " << accum << endl;
      tempEnd = clock();
      cout << (((float) tempEnd - start)/CLOCKS_PER_SEC) << endl;
      i++;
    }
    if(abs(result[4]) < 9999 || ((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime){
      prevResult = result[4];
    }


/*
    cout << "Piece is: " << result[0] << " " << result[1] << endl;
    cout << "Move is: " << result[2] << " " << result[3] << endl;
    cout << "Score is: " << result[4] << endl;

    cout << "skipped is: " << skipped << endl;
    */
    cout << accum << endl;
    cout << totalPut << endl;

    //cout << MoveGenerator(6,myBoard) << endl;

    //int result = MoveGenerator(6, myBoard);
    //cout << result << endl;
    //cout << totalMem << endl;
    //cout << maxMoves << endl;
    /*
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
    actualPins(board,myBoard,result);
    cout << "pinners" << endl;
    cout << (result.pinners == 0) << endl;
    bitBoardPrinter(result.pinners);
    cout << result.numCheckers << endl;
    int index = 0;
    //kingMovesCheck(7, 0, board, index, actualResult, result.kingDangerSquares);
    pawnMoves(3, 1, board, myBoard, index,actualResult);
    //rookMovesPinned(7, 1, board, index, actualResult,myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1]);
    //pawnMovesPinned(6, 0, board, index, actualResult,myBoard.getWhitePieces()[5].getOccupiedSquares()[0][0],myBoard.getWhitePieces()[5].getOccupiedSquares()[0][1]);
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
    // /clock_t tempEnd = clock();
    printf ("time: %0.8f sec\n", ((float) tempEnd - start)/CLOCKS_PER_SEC);

    cout << "HashMap size: " << HashMap.max_size() << endl;
    cout << "totalMem: " << totalMem << endl;
    string pieceRank = to_string(prev[0]);
    if(pieceRank == "0"){
      pieceRank = "9";
    }
    cout << "final Piece is: " << prev[0] << " " << prev[1] << endl;
    cout << "final Move is: " << prev[2] << " " << prev[3] << endl;
    string pieceFile = to_string(prev[1]);
    string moveRank = to_string(prev[2]);
    string moveFile = to_string(prev[3]);
    string finalScore;
    if(prevResult > 30000){
      finalScore = "5000";
    }
    else if(prevResult < -30000){
      finalScore = "-5000";
    }
    else{
      finalScore = to_string(prevResult);
    }
    string finalMoveStr;
    if(prevResult < 0){
      finalScore = finalScore.erase(0,1);
      finalMoveStr = pieceRank + pieceFile + moveRank + moveFile + '0' + finalScore;
    }
    else{
      finalMoveStr = pieceRank + pieceFile + moveRank + moveFile + finalScore;
    }
    int finalMoveInt = stoi(finalMoveStr);
    whiteKingMoved = false;
    blackKingMoved = false;
    cout << "score: " << prevResult << endl;
    cout << endl;
    cout << "myBoardCopy board:" << endl;
    int piece = myBoardCopy.make_move({prev[0],prev[1]},{prev[2],prev[3]});
    outfile.open("GameLog.txt", std::ios_base::app);
    outfile << boardToFEN(myBoardCopy.getBoard()) << endl;
    //outfile;

    //outfile.open("GameLog.txt", std::ios_base::app); // append instead of overwrite
    /*
    int actualPiece = getPieceHash(board[prev[0]][prev[1]]);
    int piece = myBoardCopy.make_move({prev[0],prev[1]},{prev[2],prev[3]});
    unsigned long long int hash = makeMoveHash({prev[0],prev[1]},{prev[2],prev[3]},actualPiece,piece,h,myBoardCopy);

    outfile << to_string(hash) << endl;
    */
    return finalMoveInt;
    //return 0;
}
/*
int main(){
  char* fen;
  char* turn;
  char* castle;
  char* ep;
  char* time;

  //cin >> fen >> turn >> castle >> ep >> time;
  //fen += " " + turn + " " + castle + " " + ep + " " + time;

  //turnColour = colour;
  //whiteKingMoved = whiteMoved;
  //blackKingMoved = blackMoved;

  string fenCustom = "rnbqkb1r/1p2pppp/p2p1n2/8/3NP3/2N5/PPP2PPP/R1BQKB1R w -KQkq - 3";
  runner(fenCustom);
  return 0;
}
*/

extern "C"{
  int My_Function(char* fen){
    return runner(fen);
  }
  int New_Function(char* fen){
    return newRunner(fen);
  }
}
