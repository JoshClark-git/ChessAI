#include "Globals.h"
#include "PreComputed.h"
#include "PieceList.h"
#include "Board.h"
#include "Hash.h"
#include "move.h"
#include "Evaluate.h"
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

const float endGameMultipler = 1/1645;
int centerManhattanDistance[64];
int straightMovesDiff[64][64];

long long int emptyBoard = 0;

array<string,189982> hashArray;
array<string,189982> movesArray;
int skipped = 0;
int accum = 0;
int totalPut = 0;
int totalMem = 0;
int moveNum = 0;
int numOfInsertions = 0;
int maxMoves = 0;

unsigned long long int ZobristTable[8][8][12][64][9];
unsigned long long int OpeningTable[8][8][12][2];

unsigned long long int blackHash;
unsigned long long int whiteHash;

const int tableSize = 2048000;
array<HashTuple,tableSize> HashTable;
unordered_map<unsigned long long int, HashTuple> HashMap;

unordered_map<string, int> GameLog;

//mt19937 mt(01234567);

int totalCount = 0;

const int white = 16;
const int black = 8;

int turnColour = white;

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

unsigned long long int h = 0;

typedef unsigned long long int UINT64;
UINT64 getRandom(const UINT64 &begin = 0, const UINT64 &end = 100) {
    return begin >= end ? 0 : begin + (UINT64) ((end - begin)*rand()/(double)RAND_MAX);
};

long long int myPow(int ignore,int exp){
  return static_cast<uint64_t>(1) << exp;
}


long long int posToBitBoard(array<int,2> pos){
  return myPow(2,((pos[0]) * 8) + pos[1]);
}

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

void PreCompute(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8;j++){
      int maxFileDst = max(3 - j, j - 4);
      int maxRankDst = max(3 - i, i - 4);
      centerManhattanDistance[i*8+j] = maxFileDst + maxRankDst;
    }
  }
  for(int i = 0; i < 64; i++){
    for(int j = 0; j < 64; j++){
      straightMovesDiff[i][j] = abs(i-j)/8 + abs(i-j)%8;
    }
  }
}
/*
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
  if (EnPaisantProperties[0] < 9){
    return EnPaisantProperties[0];
  }
  if(EnPaisantProperties[1] < 9){
    return EnPaisantProperties[1];
  }
  return 9;
}

// Computes the hash value of a given board
unsigned long long int computeHash(int** board, Board myBoard){
    unsigned long long int h = 0;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (board[i][j]){
              int pieceHash = getPieceHash(board[i][j]);
              if(pieceHash == 6){
                cout << getEnPaisantHash(pieceHash,myBoard) << endl;
              }
              h ^= ZobristTable[i][j][getCastleHash(myBoard)][getEnPaisantHash(pieceHash,myBoard)][getPlayerHash(pieceHash)];
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
              h ^= OpeningTable[i][j][getPlayerHash(pieceHash)];
              if(i == 6 && j == 4){
                //cout << "initremovalVal: " << OpeningTable[i][j][getPlayerHash(pieceHash)] << endl;
              }
            }
        }
    }
    //cout << "oinit: " << h << endl;
    //cout << getPieceHash(board[6][4]) << endl;
    //cout << getPlayerHash(getPieceHash(board[6][4])) << endl;
    //pieceHash = getPieceHash(board[6][4]);
    //h ^= OpeningTable[6][4][getPlayerHash(pieceHash)];
    //cout << "after Delete: " << h << endl;
    ///sleep(9999);
    return h;
}
*/
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

Board copyBoard(initBoard(),{false,false,false,false,false,false},{-1,-1});

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

array<int,5> Search(int depth, Board myBoard,int alpha, int beta, unsigned long long int hash, array<int,4> prevMove){
  int** board = myBoard.getBoard();
  if(depth == 0){
    accum++;
    return {-1,-1,-1,-1,evaluate(myBoard,board,alpha,beta)};
  }
  accum++;
  bool OGvisited;
  auto got = HashMap.find (hash);

  if(got != HashMap.end()){
    got->second.thisMove = true;
    if(got->second.visited == true){
      //got->second.thisMove = true;
      //updateHashTable(hash,got->second.depth,got->second.depth,0,got->second.storedType,0,got->second.move);
      return {got->second.move[0],got->second.move[1],got->second.move[2],got->second.move[3],0};
    }

    if(got->second.depth >= depth){
      int realValue = mateScore(got->second.value,maxDepth - depth);
      if(got->second.storedType == 1){
        skipped++;
        return {got->second.move[0],got->second.move[1],got->second.move[2],got->second.move[3],realValue};
      }

      if(got->second.storedType == 2 && realValue <= alpha){
        skipped++;
        return {got->second.move[0],got->second.move[1],got->second.move[2],got->second.move[3],realValue};
      }

      if(got->second.storedType == 0 && realValue >= beta){
        skipped++;
        return {got->second.move[0],got->second.move[1],got->second.move[2],got->second.move[3],realValue};
      }
    }

    OGvisited = got->second.visited;
    got->second.visited = true;

  }

  if(insufficientMaterial(myBoard)){
    if(got != HashMap.end()){
      got->second.visited = OGvisited;
      //updateHashTable(hash,got->second.depth,got->second.depth,0,got->second.storedType,0,got->second.move);
    }
    return {-1,-1,-1,-1,0};
  }



  unsigned long long int OGHash = hash;
  array<int,4> bestMove = {-1,-1,-1,-1};
  array<pair<array<int,4>,int>,218> moves = moveOrdering(myBoard,board);

  if(depth == maxDepth && prevMove[0] != -1){
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

  if(moves[0].first[0] == -1){
    if(playerInCheck(myBoard)){
      if(got != HashMap.end()){
        got->second.visited = OGvisited;
      }
      return {-1,-1,-1,-1,-(numeric_limits<int>::max() - (maxDepth-depth))};
    }
    else{
      if(got != HashMap.end()){
        got->second.visited = OGvisited;
      }
      return {-1,-1,-1,-1,0};
    }
  }

  array<int,5> value;
  array<int,5> best;
  array<int,2> piecePositon;
  array<int,2> move;
  int piece;
  int storedType = 2;
  int i = 0;
  clock_t tempEnd;
  string boardFEN;
  unsigned long long int prevHash = hash;
  best = {prevMove[0],prevMove[1],prevMove[2],prevMove[3],alpha};
  while(moves[i].first[0] != -1){
    piecePositon = {moves[i].first[0],moves[i].first[1]};
    move = {moves[i].first[2], moves[i].first[3]};

    int actualPiece = getPieceHash(board[piecePositon[0]][piecePositon[1]]);
    piece = myBoard.make_move(piecePositon,move);

    hash = makeMoveHash(piecePositon,move,actualPiece,piece,hash,myBoard,maxDepth-(depth-1));

    value = Search(depth-1,myBoard, -beta, -alpha, hash,prevMove);
    value[4] = -value[4];
    hash = unmakeMoveHash(move,piecePositon,actualPiece,piece,hash,myBoard,maxDepth-(depth-1));
    myBoard.unmake_move(move,piecePositon,piece);
    tempEnd = clock();
    if(((float) tempEnd - start)/CLOCKS_PER_SEC > maxTime){
      break;
    }

    if(value[4] >= beta){
      storedType = 0;
      best[0] = piecePositon[0];
      best[1] = piecePositon[1];
      best[2] = move[0];
      best[3] = move[1];
      best[4] = value[4];

      break;
    }
    if(value[4] > alpha){
      storedType = 1;

      best[0] = piecePositon[0];
      best[1] = piecePositon[1];
      best[2] = move[0];
      best[3] = move[1];
      best[4] = value[4];

      alpha = best[4];
    }
    i++;

  }

  if(((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime){
    //storeEvaluation(hash,depth,storedType,best[4]);
    updateHashTable(hash,depth,maxDepth-depth,storedType,best[4],{best[0],best[1],best[2],best[3]});
  }

  if(got!= HashMap.end()){
    got->second.visited = OGvisited;
  }
  return best;
}

array<int,4> extractPrevMove(string startFen){
  cout << startFen << endl;
  int last = startFen[startFen.size()-1] -'0';
  cout << last << endl;
  int next = startFen[startFen.size()-2] -'0';
  int secondFirst = startFen[startFen.size()-3] -'0';
  int first = startFen[startFen.size()-4] -'0';
  cout << first << " " << secondFirst << endl;
  return {first,secondFirst,next,last};
}

string initProperties(string startFen){
  whiteKingRookMoved = true;
  whiteQueenRookMoved = true;
  blackKingRookMoved = true;
  blackQueenRookMoved = true;
  blackKingMoved = false;
  whiteKingMoved = false;
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
  cout << castlePropertiesFEN << endl;
  for(int i = 0; i < castlePropertiesFEN.length();i++){
    cout << castlePropertiesFEN[i] << endl;
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
      blackEnPaisant = 9;
    }
    else{
      blackEnPaisant = EnPaisantFEN - '0';
      whiteEnPaisant = 9;
    }
  }
  else{
    whiteEnPaisant = 9;
    blackEnPaisant = 9;
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

int newRunner(char* startFen){
  string str(startFen);

  string strStartFen = initProperties(startFen);
    int** board = initBoard();

    int count = 0;


    clock_t tempEnd = clock();
    //return 0;
    array<bool,6> castleProperties = {whiteKingMoved,whiteKingRookMoved,whiteQueenRookMoved,blackKingMoved,blackKingRookMoved,blackQueenRookMoved};
    array<int,2> EnPaisantProperties = {whiteEnPaisant,blackEnPaisant};
    Board myBoard(board,castleProperties,EnPaisantProperties);

    loadBoardFromFen(strStartFen, board, myBoard);
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


    boardPrinter(board);

    if(moveNum == 0){
      initTable();
      PreCompute();
      h = computeHash(board, myBoard);
    }
    else{
      boardPrinter(copyBoard.getBoard());
      array<int,4> prevMove = extractPrevMove(startFen);
      int actualPiece = getPieceHash(board[prevMove[2]][prevMove[3]]);

      int piece = copyBoard.make_move({prevMove[0],prevMove[1]},{prevMove[2],prevMove[3]});
      if(piece == 1 || piece>29){
        if(turnColour == black){
          actualPiece = 0;
        }
        else{
          actualPiece = 6;
        }
      }

      if(turnColour == 8){
        turnColour = 16;
      }
      else{
        turnColour = 8;
      }

      h = makeMoveHash({prevMove[0],prevMove[1]},{prevMove[2],prevMove[3]},actualPiece,piece,h,myBoard,0);
      auto got = HashMap.find (h);
      if(got == HashMap.end()){
        cout << "still not right" << endl;
        sleep(1);
      }


    }
    EnPaisantPropertiesStack.pop();
    EnPaisantPropertiesStack.pop();

    EnPaisantPropertiesStack.push(whiteEnPaisant);
    EnPaisantPropertiesStack.push(blackEnPaisant);



    bitset<64> hash64(h);

    //updateHashTable(h, 999,1, 0,true);

    string gameFEN;
    int** prevBoard;


    Board myBoardCopy = myBoard;


    int originalTurnColor = turnColour;
    int i = 2;
    start = clock();

    maxDepth = 1;
    array<int,5> result = Search(i-1,myBoard, -numeric_limits<int>::max(),numeric_limits<int>::max(),h,{-1,-1,-1,-1});
    tempEnd = clock();
    cout << (((float) tempEnd - start)/CLOCKS_PER_SEC) << endl;
    int boundAlpha = result[4] - 100;
    int boundBeta = result[4] + 100;
    bool prevBeta = false;
    bool prevAlpha = false;
    array<int,4> prev = {result[0],result[1],result[2],result[3]};
    int prevResult = result[4];
    while(((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime && abs(result[4]) < 100000){
    //while(i < 10){
      prevResult = result[4];
      prev = {result[0],result[1],result[2],result[3]};
      maxDepth = i;
      cout << endl;
      cout << "entering: " << i << endl;
      cout << "alphaBound: " << boundAlpha << endl;
      cout << "betaBound: " << boundBeta << endl;

      result = Search(i,myBoard,-numeric_limits<int>::max(),numeric_limits<int>::max(),h,prev);
      tempEnd = clock();

      if(((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime && (result[4] >= boundBeta || result[4] <= boundAlpha) && abs(result[4]) < 100000){

        cout << "Piece is: " << result[0] << " " << result[1] << endl;
        cout << "Move is: " << result[2] << " " << result[3] << endl;
        cout << "Score is: " << result[4] << endl;
        if(result[4] >= boundBeta){
          boundBeta = numeric_limits<int>::max();
          boundAlpha = result[4]-50;
        }
        else{
          boundAlpha = -numeric_limits<int>::max();
          boundBeta = result[4]+50;
        }

        cout << "entering: " << i << endl;
        cout << "alphaBound: " << boundAlpha << endl;
        cout << "betaBound: " << boundBeta << endl;

        result = Search(i,myBoard,-numeric_limits<int>::max(),numeric_limits<int>::max(),h,prev);
        tempEnd = clock();
      }

      if(result[4] < boundBeta && result[4] > boundAlpha || (abs(result[4]) > 10000 && ((float) tempEnd - start)/CLOCKS_PER_SEC < maxTime)){
        cout << "getting stopped early vals NEW" << endl;
        prevResult = result[4];
        prev = {result[0],result[1],result[2],result[3]};
      }

      boundBeta = result[4] + 50;
      boundAlpha = result[4] - 50;
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
    turnColour = originalTurnColor;


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
    cout << "finalScore: " << finalScore << endl;
    int piece = myBoardCopy.make_move({prev[0],prev[1]},{prev[2],prev[3]});

    turnColour = originalTurnColor;

    int** fBoard = initBoard();
    copyBoard.setEnPaisantProperties(EnPaisantProperties);
    copyBoard.setCastleProperties(castleProperties);
    copyBoard.setBoard(fBoard);

    //const string startFen = "5r2/pp4p1/2kp4/2p3B1/2P1PPp1/6K1/PP5r/4R3 b - - 5 29";
    loadBoardFromFen(strStartFen, fBoard, copyBoard);
    if(moveNum == 0){
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          if(fBoard[i][j] == 9){
            copyBoard.pawnPositions[0].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 10){
            copyBoard.kingPositions[0].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 11){
            copyBoard.knightPositions[0].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 12){
            copyBoard.bishopPositions[0].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 13){
            copyBoard.rookPositions[0].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 14){
            copyBoard.queenPositions[0].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 17){
            copyBoard.pawnPositions[1].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 18){
            copyBoard.kingPositions[1].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 19){
            copyBoard.knightPositions[1].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 20){
            copyBoard.bishopPositions[1].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 21){
            copyBoard.rookPositions[1].addPieceAtSquare(i,j);
          }
          else if(fBoard[i][j] == 22){
            copyBoard.queenPositions[1].addPieceAtSquare(i,j);
          }
        }
      }
    }

    int actualPiece = getPieceHash(fBoard[prev[0]][prev[1]]);
    array<int,2> fPiece = {prev[0],prev[1]};
    array<int,2> fMove = {prev[2],prev[3]};

    piece = copyBoard.make_move(fPiece,fMove);

    h = makeMoveHash(fPiece,fMove,actualPiece,piece,h,copyBoard,1);

    whiteKingMoved = false;
    whiteKingRookMoved = false;
    whiteQueenRookMoved = false;
    blackKingMoved = false;
    blackKingRookMoved = false;
    blackQueenRookMoved = false;

    moveNum++;
    auto got = HashMap.find(h);

    if(got!= HashMap.end()){
      got->second.visited = true;
      updateHashTable(h,99999,0,got->second.storedType,0,got->second.move);

    }

    vector<unsigned long long int> ancients;
    for(auto kv : HashMap){
      //cout << kv.first<< endl;
      auto got = HashMap.find (kv.first);
      if(!(got->second.thisMove || got->second.lastMove || got->second.visited)){
        ancients.push_back(kv.first);
      }

    }
    for(auto key: ancients){
      HashMap.erase(key);
    }
    for(auto kv : HashMap){
      auto got = HashMap.find (kv.first);
      got->second.lastMove = got->second.thisMove;
      got->second.thisMove = false;
    }

    return finalMoveInt;

}

extern "C"{
  int New_Function(char* fen){
    return newRunner(fen);
  }
  int My_Function(char* fen){
    return newRunner(fen);
  }
}
