#ifndef GLOBALS_H // This needs to be unique in each header
#define GLOBALS_H
#include<iostream>
#include<string>
#include <iterator>
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
struct HashTuple{
  //unsigned long long int hash;
  array<int,4> move;
  int value;
  int depth;
  int storedType;
  bool visited;
  bool lastMove;
  bool thisMove;
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

struct ordering {
    bool operator ()(pair<array<int,4>, int> const& a,
                     pair<array<int,4>, int> const& b) {
        return a.second > b.second;
    }
};


long long int myPow(int ignore,int exp);
long long int posToBitBoard(array<int,2> pos);

extern bool whiteKingMoved;
extern bool blackKingMoved;

extern bool whiteKingRookMoved;
extern bool whiteQueenRookMoved;
extern bool blackKingRookMoved;
extern bool blackQueenRookMoved;

extern unordered_map<unsigned long long int, HashTuple> HashMap;
extern const int tableSize;
extern array<HashTuple,2048000> HashTable;
extern stack<bool> castlePropertiesStack;
extern stack<int> EnPaisantPropertiesStack;
extern const int black;
extern const int white;
extern int moveNum;
extern int totalMem;
extern int maxMoves;
extern int numOfInsertions;
extern int blackEnPaisant;
extern int whiteEnPaisant;
extern int turnColour;
extern int numOfInsertions;
extern unsigned long long int ZobristTable[8][8][12][64][9]; // board x piece x caslte x EP
extern unsigned long long int blackHash;
extern unsigned long long int whiteHash;

extern const float endGameMultipler;
extern int centerManhattanDistance[64];
extern int straightMovesDiff[64][64];

#endif
