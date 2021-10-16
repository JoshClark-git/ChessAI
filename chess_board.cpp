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

int boards = 0;

//r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 depth = 5 ;  766.26200000 sec




using namespace std;

const int pawn = 1;
const int king = 2;
const int knight = 3;
const int bishop = 4;
const int rook = 5;
const int queen = 6;

const int black = 8;
const int white = 16;

int turnColour = white;

bool whiteKingMoved = true;
bool blackKingMoved = false;

bool whiteKingRookMoved = false;
bool whiteQueenRookMoved = false;
bool blackKingRookMoved = false;
bool blackQueenRookMoved = false;

int blackEnPaisant = 9;
int whiteEnPaisant = 9;

std::vector<array<int,2>> whitePiecePos;
std::vector<array<int,2>> blackPiecePos;

//std::vector<array<int,2>> whiteSlidingPiecePos;
//std::vector<array<int,2>> blackSlidingPiecePos;

std::vector<vector<array<int,2>>> whiteMoves;
std::vector<vector<array<int,2>>> blackMoves;

int** initBoard(){
  int** board = new int*[8];
  for(int i = 0; i < 8; i++){
    board[i] = new int[8];
    for(int j = 0; j < 8; j++){
      board[i][j] = 0;
    }
  }
  return board;
}

void loadBoardFromFen(string fen,int **board){
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
        if(pieceColour == white){
          whitePiecePos.push_back({rank,file});
        }
        else{
          blackPiecePos.push_back({rank,file});
        }
        file++;
      }
    }
  }
}
/*
bool isSlidingPiece(array<int,2> position){
  if(turnColour == white){
    auto it = find(whiteSlidingPiecePos.begin(),whiteSlidingPiecePos.end(),position);
    if(it == whiteSlidingPiecePos.end()){
      return false;
    }
    else{
      return true;
    }
  }
  else{
    auto it = find(blackSlidingPiecePos.begin(),blackSlidingPiecePos.end(),position);
    if(it == blackSlidingPiecePos.end()){
      return false;
    }
    else{
      return true;
    }
  }
}
*/
/*
void setupSlidingPiecePos(int **board){
  int rank;
  int file;
  for(int i = 0;i<whitePiecePos.size();i++){
    rank = whitePiecePos[i][0];
    file = whitePiecePos[i][1];
    int val = board[rank][file] % 8;
    if(val == 4 || val == 5 || val == 6){
      whiteSlidingPiecePos.push_back({rank,file});
    }
  }
  for(int i = 0;i<blackPiecePos.size();i++){
    rank = blackPiecePos[i][0];
    file = blackPiecePos[i][1];
    int val = board[rank][file] % 8;
    if(val == 4 || val == 5 || val == 6){
      blackSlidingPiecePos.push_back({rank,file});
    }
  }
}
*/
int** copyBoard(int** board){
  int** boardCopy = new int*[8];
  for(int i = 0; i< 8; i++){
    boardCopy[i] = new int[8];
    for(int j = 0; j<8; j++){
      boardCopy[i][j] = board[i][j];
    }
  }
  return boardCopy;
}
void duplicateBoard(int** goodBoard, int** oldBoard){
  for(int i = 0; i< 8; i++){
    for(int j = 0; j<8; j++){
      oldBoard[i][j] = goodBoard[i][j];
    }
  }
}

void deleteBoard(int** board){
  for(int i = 0; i< 8; i++){
    delete board[i];
  }
  delete board;
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


std::vector<array<int,2>> piecePos(int** board){
  if(turnColour == white){
    return whitePiecePos;
  }
  else{
    return blackPiecePos;
  }
  /*
  std::vector<array<int,2>> piecePositons;
  for(int i = 0;i<8;i++){
    for(int j = 0; j<8; j++){
      if(!((board[i][j] ^ turnColour) > board[i][j])){
        piecePositons.push_back({i,j});
      }
    }
  }
  return piecePositons;
  */
}

std::vector<array<int,2>> knightTurn(int rank, int file, int** board){
  auto currTurnColour = turnColour;
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
std::vector<array<int,2>> pawnTurn(int rank, int file, int** board){
  auto currTurnColour = turnColour;
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

  if(turnColour==white){
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

bool whiteCastleKingSide(int** board){
  if(board[7][5] != 0 || board[7][6] != 0 || board[7][7] != 21){
    return false;
  }
  turnColour = black;
  std::vector<array<int,2>> oppPiecePos = piecePos(board);
  for(int i = 0; i < oppPiecePos.size(); i++){
    std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPiecePos[i][0],oppPiecePos[i][1],board);
    for(int j = 0; j < oppPieceMoves.size(); j++){
      if(oppPieceMoves[j][0] == 7 && (oppPieceMoves[j][1] == 5 || oppPieceMoves[j][1] == 6 || oppPieceMoves[j][1] == 4)){
        turnColour = white;
        return false;
      }
    }
  }
  turnColour = white;
  return true;
}
bool whiteCastleQueenSide(int** board){
  if(board[7][1] != 0 || board[7][2] != 0 || board[7][3] != 0 || board[7][0] != 21){
    return false;
  }
  turnColour = black;
  std::vector<array<int,2>> oppPiecePos = piecePos(board);
  for(int i = 0; i < oppPiecePos.size(); i++){
    std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPiecePos[i][0],oppPiecePos[i][1],board);

    for(int j = 0;j < oppPieceMoves.size(); j++){
      if(oppPieceMoves[j][0] == 7 && (oppPieceMoves[j][1] == 2 || oppPieceMoves[j][1] == 3 || oppPieceMoves[j][1] == 4)){
        turnColour = white;
        return false;
      }
    }
  }
  turnColour = white;
  return true;
}
bool blackCastleKingSide(int** board){
  if(board[0][5] != 0 || board[0][6] != 0 || board[0][7] != 13){
    return false;
  }
  turnColour = white;
  std::vector<array<int,2>> oppPiecePos = piecePos(board);
  for(int i = 0; i < oppPiecePos.size(); i++){
    std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPiecePos[i][0],oppPiecePos[i][1],board);
    for(int j = 0;j < oppPieceMoves.size(); j++){
      if(oppPieceMoves[j][0] == 0 && (oppPieceMoves[j][1] == 4 || oppPieceMoves[j][1] == 5 || oppPieceMoves[j][1] == 6)){
        turnColour = black;
        return false;
      }
    }
  }
  turnColour = black;
  return true;
}
bool blackCastleQueenSide(int** board){
  if(board[0][1] != 0 || board[0][2] != 0 || board[0][3] != 0 || board[0][0] != 13){
    return false;
  }
  turnColour = white;
  std::vector<array<int,2>> oppPiecePos = piecePos(board);
  for(int i = 0; i < oppPiecePos.size(); i++){
    std::vector<array<int,2>> oppPieceMoves = pieceMovesForChecks(oppPiecePos[i][0],oppPiecePos[i][1],board);
    for(int j = 0;j < oppPieceMoves.size(); j++){
      if(oppPieceMoves[j][0] == 0 && (oppPieceMoves[j][1] == 2 || oppPieceMoves[j][1] == 3 || oppPieceMoves[j][1] == 4)){
        turnColour = black;
        return false;
      }
    }
  }
  turnColour = black;
  return true;
}


std::vector<array<int,2>> kingTurn(int rank, int file, int** board){
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
    if(!whiteKingMoved && !whiteKingRookMoved && whiteCastleKingSide(board)){
      possTurns.push_back({rank, file+2});
    }
    if(!whiteKingMoved && !whiteQueenRookMoved && whiteCastleQueenSide(board)){
      possTurns.push_back({rank, file-2});
    }
  }
  else{
    if(!blackKingMoved && !blackKingRookMoved && blackCastleKingSide(board)){
      possTurns.push_back({rank, file+2});
    }
    if(!blackKingMoved && !blackQueenRookMoved && blackCastleQueenSide(board)){
      possTurns.push_back({rank, file-2});
    }
  }
  turnColour = currTurnColour;
  return possTurns;
}

std::vector<array<int,2>> NewpieceMoves(array<int,2> initPos, int** board){
  if(turnColour == white){
    auto it = find(whitePiecePos.begin(), whitePiecePos.end(), initPos);
    if(it == whitePiecePos.end()){
      cout << "log Error NewpieceMoves" << endl;
      cout << initPos[0] << " " << initPos[1] << endl;
      sleep(100);
    }
    int index = it - whitePiecePos.begin();
    if(board[whitePiecePos[index][0]][whitePiecePos[index][1]] % 8 == 2){
      //Update caslting
      if(!whiteKingMoved && (!whiteKingRookMoved || !whiteQueenRookMoved)){
        whiteMoves[index] = kingTurn(initPos[0],initPos[1],board);
      }

    }
    //enPaisant check
    else if(board[whitePiecePos[index][0]][whitePiecePos[index][1]] % 8 == 1){
      array<int,2> enPaisantRight = {initPos[0]-1,initPos[1]+1};
      array<int,2> enPaisantLeft = {initPos[0]-1,initPos[1]-1};
      auto it = find(whitePiecePos.begin(),whitePiecePos.end(),initPos);
      int index = it - whitePiecePos.begin();
      if(find(whiteMoves[index].begin(),whiteMoves[index].end(), enPaisantLeft) != whiteMoves[index].end() && board[enPaisantLeft[0]][enPaisantLeft[1]] == 0 && whiteEnPaisant != enPaisantLeft[1]){
        whiteMoves[index] = pawnTurn(initPos[0],initPos[1],board);
      }
      else if(find(whiteMoves[index].begin(),whiteMoves[index].end(), enPaisantRight) != whiteMoves[index].end() && board[enPaisantRight[0]][enPaisantRight[1]] == 0 && whiteEnPaisant != enPaisantRight[1]){
        whiteMoves[index] = pawnTurn(initPos[0],initPos[1],board);
      }
    }
    return whiteMoves[index];
  }
  else{
    auto it = find(blackPiecePos.begin(), blackPiecePos.end(), initPos);
    if(it == blackPiecePos.end()){
      cout << "log Error NewpieceMoves" << endl;
      cout << initPos[0] << " " << initPos[1] << endl;
      sleep(100);
    }
    int index = it - blackPiecePos.begin();
    if(board[blackPiecePos[index][0]][blackPiecePos[index][1]] % 8 == 2 && !blackKingMoved){
      //Update caslting
      if(!blackKingRookMoved || !blackQueenRookMoved){
        blackMoves[index] = kingTurn(initPos[0],initPos[1],board);
      }
    }
    //enPaisant check
    else if(board[blackPiecePos[index][0]][blackPiecePos[index][1]] % 8 == 1){
      array<int,2> enPaisantRight = {initPos[0]+1,initPos[1]+1};
      array<int,2> enPaisantLeft = {initPos[0]+1,initPos[1]-1};
      auto it = find(blackPiecePos.begin(),blackPiecePos.end(),initPos);
      int index = it - blackPiecePos.begin();
      if(find(blackMoves[index].begin(),blackMoves[index].end(), enPaisantLeft) != blackMoves[index].end() && board[enPaisantLeft[0]][enPaisantLeft[1]] == 0 && blackEnPaisant != enPaisantLeft[1]){
        //cout << "enter" << endl;
        /*
        for(int i = 0;i< blackMoves[index].size();i++){
          cout << blackMoves[index][i][0] << " " << blackMoves[index][i][1] << endl;
        }
        //cout << "before: " << whiteMoves[index].size() << endl;
        */
        blackMoves[index] = pawnTurn(initPos[0],initPos[1],board);
        /*
        for(int i = 0;i< blackMoves[index].size();i++){
          cout << blackMoves[index][i][0] << " " << blackMoves[index][i][1] << endl;
        }
        */
        //cout << "after: " << whiteMoves[index].size() << endl;
      }
      else if(find(blackMoves[index].begin(),blackMoves[index].end(), enPaisantRight) != blackMoves[index].end() && board[enPaisantRight[0]][enPaisantRight[1]] == 0 && blackEnPaisant != enPaisantRight[1]){
        blackMoves[index] = pawnTurn(initPos[0],initPos[1],board);
      }
    }
    return blackMoves[index];
  }
  cout << "log Error NewpieceMoves neither colour" << endl;
  cout << initPos[0] << " " << initPos[1] << endl;
  sleep(100);
  return blackMoves[0];
}

std::vector<array<int,2>> pieceMoves(int rank, int file, int** board){
  if(board[rank][file] % 8 == 1){
    return pawnTurn(rank,file,board);
  }
  else if(board[rank][file] % 8 == 2){
    return kingTurn(rank,file,board);
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

void setupAttackingSquares(int** board){
  auto currTurnColour = turnColour;
  turnColour = white;
  for(int i = 0;i<whitePiecePos.size();i++){
    whiteMoves.push_back(pieceMoves(whitePiecePos[i][0],whitePiecePos[i][1],board));
  }
  turnColour = black;
  for(int i = 0;i<blackPiecePos.size();i++){
    blackMoves.push_back(pieceMoves(blackPiecePos[i][0],blackPiecePos[i][1],board));
  }
  turnColour = currTurnColour;
}

array<int,2> kingPos(int** board){
  if(turnColour == white){
    for(int i = 0; i < whitePiecePos.size(); i++){
      if(board[whitePiecePos[i][0]][whitePiecePos[i][1]] == 18){
        return {whitePiecePos[i][0],whitePiecePos[i][1]};
      }
    }
  }
  else{
    for(int i = 0; i < blackPiecePos.size(); i++){
      if(board[blackPiecePos[i][0]][blackPiecePos[i][1]] == 10){
        return {blackPiecePos[i][0],blackPiecePos[i][1]};
      }
    }
  }
  /*
  for(int i = 0; i<8;i++){
    for(int j = 0; j < 8; j++){
      if((board[i][j] % 8 == 2) && ((board[i][j] ^ turnColour) < board[i][j])){
        return {i,j};
      }
    }
  }
  */
  cout << "can't find king" << endl;
  cout << turnColour << endl;
  for(int i = 0; i<whitePiecePos.size(); i++){
    cout << whitePiecePos[i][0] << " " << whitePiecePos[i][1] << endl;
  }
  boardPrinter(board);
  sleep(100);
  return {-1,-1};
}

bool newMoveResultsInCheck(int** board){
  array<int,2> kingPosition = kingPos(board);

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

bool oldMoveResultsInCheck(int** board){
  array<int,2> kingPosition = kingPos(board);
  if(turnColour == white){
    for(int i = 0; i<blackPiecePos.size();i++){
      if(find(blackMoves[i].begin(),blackMoves[i].end(),kingPosition) != blackMoves[i].end()){
        return true;
      }
    }
  }
  else{
    for(int i = 0; i<whitePiecePos.size();i++){
      if(find(whiteMoves[i].begin(),whiteMoves[i].end(),kingPosition) != whiteMoves[i].end()){
        return true;
      }
    }
  }
  return false;
}
/*
bool moveResultsInCheck(int** board){
  array<int,2> kingPosition = kingPos(board);
  if(turnColour == white){
    turnColour = black;
  }
  else{
    turnColour = white;
  }
  std::vectorvector<<array<int,2>>> allPieceMoves;
  if(turnColour == white){
    allPieceMoves = whiteMoves;
  }
  else{
    allPieceMoves = blackMoves;
  }
  for(int i = 0; i<oppPiecePos.size(); i++){
    std::vector<array<int,2>> allPieceMoves;
    if(turnColour == white){
      allPieceMoves = whiteMoves;
    }
    else{
      allPieceMoves = blackMoves;
    }
    //std::vector<array<int,2>> allPieceMoves = pieceMovesForChecks(oppPiecePos[i][0],oppPiecePos[i][1],board);
    for(int j = 0; j < allPieceMoves.size(); j++){

      if(((board[oppPiecePos[]]     allPieceMoves[j][0] == kingPosition[0] && allPieceMoves[j][1] == kingPosition[1]){
        if(turnColour == white){
          turnColour = black;
        }
        else{
          turnColour = white;
        }
        return true;
      }
    }
  }
  if(turnColour == white){
    turnColour = black;
  }
  else{
    turnColour = white;
  }
  return false;
}
*/

void updateSlidingMove(int rank, int file,int** board){
  array<int,2> position = {rank,file};
  //white
  if(board[rank][file] > 16){
    auto it = find(whitePiecePos.begin(), whitePiecePos.end(), position);
    if(it == whitePiecePos.end()){
      cout << "log Error updateSlidingMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - whitePiecePos.begin();
    whiteMoves[index] = pieceMoves(rank,file,board);
  }
  //black
  else{
    auto it = find(blackPiecePos.begin(), blackPiecePos.end(), position);
    if(it == blackPiecePos.end()){
      cout << "log Error updateSlidingMoveB" << endl;
      cout << rank << " " << file << endl;
      boardPrinter(board);
      sleep(100);
    }
    int index = it - blackPiecePos.begin();
    blackMoves[index] = pieceMoves(rank,file,board);
  }
}
/*
void updateUpMove(int rank, int file, int** board){
  array<int,2> position = {rank,file};
  //white
  if(board[rank][file] > 16){
    auto it = find(whitePiecePos.begin(), whitePiecePos.end(), position);
    if(it == whitePiecePos.end()){
      cout << "log Error updateUpMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - whitePiecePos.begin();
    std::vector<array<int,2>> pieceMoves = whiteMoves[index];
    for(int i = 0; i < pieceMoves.size();i++){
      if(pieceMoves[i][0] >= rank && pieceMoves[i][1] == file){
        swap(pieceMoves[i],pieceMoves.back());
        pieceMoves.pop_back();
        i--;
      }
    }
    while(rank != 7 && board[rank+1][file] == 0){
      pieceMoves.push_back({rank+1,file});
      rank++;
    }

    if(rank != 7  && ((board[rank+1][file] ^ white) > board[rank+1][file])){
      pieceMoves.push_back({rank+1,file});
    }
    whiteMoves[index] = pieceMoves;
  }
  //black
  else{
    auto it = find(blackPiecePos.begin(), blackPiecePos.end(), position);
    if(it == blackPiecePos.end()){
      cout << "log Error updateUpMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - blackPiecePos.begin();
    std::vector<array<int,2>> pieceMoves = blackMoves[index];
    for(int i = 0; i < pieceMoves.size();i++){
      if(pieceMoves[i][0] >= rank && pieceMoves[i][1] == file){
        swap(pieceMoves[i],pieceMoves.back());
        pieceMoves.pop_back();
        i--;
      }
    }
    while(rank != 7 && board[rank+1][file] == 0){
      pieceMoves.push_back({rank+1,file});
      rank++;
    }

    if(rank != 7  && ((board[rank+1][file] ^ black) > board[rank+1][file])){
      pieceMoves.push_back({rank+1,file});
    }
    blackMoves[index] = pieceMoves;
  }
}
void updateDownMove(int rank, int file, int** board){
  array<int,2> position = {rank,file};
  //white
  if(board[rank][file] > 16){
    auto it = find(whitePiecePos.begin(), whitePiecePos.end(), position);
    if(it == whitePiecePos.end()){
      cout << "log Error updateUpMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - whitePiecePos.begin();
    std::vector<array<int,2>> pieceMoves = whiteMoves[index];
    for(int i = 0; i < pieceMoves.size();i++){
      if(pieceMoves[i][0] <= rank && pieceMoves[i][1] == file){
        swap(pieceMoves[i],pieceMoves.back());
        pieceMoves.pop_back();
        i--;
      }
    }
    while(rank != 0 && board[rank-1][file] == 0){
      pieceMoves.push_back({rank-1,file});
      rank--;
    }

    if(rank != 0  && ((board[rank-1][file] ^ white) > board[rank-1][file])){
      pieceMoves.push_back({rank-1,file});
    }
    whiteMoves[index] = pieceMoves;
  }
  //black
  else{
    auto it = find(blackPiecePos.begin(), blackPiecePos.end(), position);
    if(it == blackPiecePos.end()){
      cout << "log Error updateUpMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - blackPiecePos.begin();
    std::vector<array<int,2>> pieceMoves = blackMoves[index];
    for(int i = 0; i < pieceMoves.size();i++){
      if(pieceMoves[i][0] <= rank && pieceMoves[i][1] == file){
        swap(pieceMoves[i],pieceMoves.back());
        pieceMoves.pop_back();
        i--;
      }
    }
    while(rank != 0 && board[rank-1][file] == 0){
      pieceMoves.push_back({rank-1,file});
      rank--;
    }

    if(rank != 0  && ((board[rank-1][file] ^ black) > board[rank-1][file])){
      pieceMoves.push_back({rank-1,file});
    }
    blackMoves[index] = pieceMoves;
  }
}
void updateRightMove(int rank, int file, int** board){
  array<int,2> position = {rank,file};
  //white
  if(board[rank][file] > 16){
    auto it = find(whitePiecePos.begin(), whitePiecePos.end(), position);
    if(it == whitePiecePos.end()){
      cout << "log Error updateUpMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - whitePiecePos.begin();
    std::vector<array<int,2>> pieceMoves = whiteMoves[index];
    for(int i = 0; i < pieceMoves.size();i++){
      if(pieceMoves[i][0] == rank && pieceMoves[i][1] <= file){
        swap(pieceMoves[i],pieceMoves.back());
        pieceMoves.pop_back();
        i--;
      }
    }
    while(file != 0  && board[rank][file-1] == 0){
      pieceMoves.push_back({rank,file-1});
      file--;
    }

    if(file != 0  && ((board[rank][file-1] ^ white) > board[rank][file-1])){
      pieceMoves.push_back({rank,file-1});
    }
    whiteMoves[index] = pieceMoves;
  }
  //black
  else{
    auto it = find(blackPiecePos.begin(), blackPiecePos.end(), position);
    if(it == blackPiecePos.end()){
      cout << "log Error updateUpMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - blackPiecePos.begin();
    std::vector<array<int,2>> pieceMoves = blackMoves[index];
    for(int i = 0; i < pieceMoves.size();i++){
      if(pieceMoves[i][0] == rank && pieceMoves[i][1] <= file){
        swap(pieceMoves[i],pieceMoves.back());
        pieceMoves.pop_back();
        i--;
      }
    }
    while(file != 0  && board[rank][file-1] == 0){
      pieceMoves.push_back({rank,file-1});
      file--;
    }

    if(file != 0  && ((board[rank][file-1] ^ black) > board[rank][file-1])){
      pieceMoves.push_back({rank,file-1});
    }
    blackMoves[index] = pieceMoves;
  }
}
void updateLeftMove(int rank, int file, int** board){
  array<int,2> position = {rank,file};
  //white
  if(board[rank][file] > 16){
    auto it = find(whitePiecePos.begin(), whitePiecePos.end(), position);
    if(it == whitePiecePos.end()){
      cout << "log Error updateUpMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - whitePiecePos.begin();
    std::vector<array<int,2>> pieceMoves = whiteMoves[index];
    for(int i = 0; i < pieceMoves.size();i++){
      if(pieceMoves[i][0] == rank && pieceMoves[i][1] >= file){
        swap(pieceMoves[i],pieceMoves.back());
        pieceMoves.pop_back();
        i--;
      }
    }
    while(file != 7  && board[rank][file+1] == 0){
      pieceMoves.push_back({rank,file+1});
      file++;
    }

    if(file != 7  && ((board[rank][file+1] ^ white) > board[rank][file+1])){
      pieceMoves.push_back({rank,file+1});
    }
    whiteMoves[index] = pieceMoves;
  }
  //black
  else{
    auto it = find(blackPiecePos.begin(), blackPiecePos.end(), position);
    if(it == blackPiecePos.end()){
      cout << "log Error updateUpMove" << endl;
      cout << rank << " " << file << endl;
      sleep(100);
    }
    int index = it - blackPiecePos.begin();
    std::vector<array<int,2>> pieceMoves = blackMoves[index];
    for(int i = 0; i < pieceMoves.size();i++){
      if(pieceMoves[i][0] == rank && pieceMoves[i][1] >= file){
        swap(pieceMoves[i],pieceMoves.back());
        pieceMoves.pop_back();
        i--;
      }
    }
    while(file != 7  && board[rank][file+1] == 0){
      pieceMoves.push_back({rank,file+1});
      file++;
    }

    if(file != 7  && ((board[rank][file+1] ^ black) > board[rank][file+1])){
      pieceMoves.push_back({rank,file+1});
    }
    blackMoves[index] = pieceMoves;
  }
}
*/
void updateSlidingMoves(array<int,2> initPos,array<int,2> endPos,int** board){
  std::vector<array<int,2>> alreadyUpdated;
  //initPos

  int actualRank = initPos[0];
  int actualFile = initPos[1];

  int rank = initPos[0];
  int file = initPos[1];

  //Queen || rook || pawn || king

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  if(rank != 0 && (actualRank == rank) && (board[rank-1][file] == 9 || board[rank-1][file] % 8 == 2)){
    //updateSlidingMove(rank-1,file,board);
    updateSlidingMove(rank-1,file,board);
    alreadyUpdated.push_back({rank-1,file});
  }
  if(actualRank == 3 && (abs(actualRank - rank) == 1) && board[rank-1][file] == 9){
    //updateSlidingMove(rank-1,file,board);
    updateSlidingMove(rank-1,file,board);
    //updateSlidingMove(rank-1,file,board);
    alreadyUpdated.push_back({rank-1,file});
  }
  if(rank != 0  && (board[rank-1][file] % 8 == 5 || board[rank-1][file] % 8 == 6)){
    //updateSlidingMove(rank-1,file,board);
    updateSlidingMove(rank-1,file,board);
    //updateUpMove(rank-1,file,board);
    alreadyUpdated.push_back({rank-1,file});
  }
  file = actualFile;
  rank = actualRank;

  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  if(rank != 7 && (actualRank == rank) && (board[rank+1][file] == 17 || board[rank+1][file] % 8 == 2)){
    updateSlidingMove(rank+1,file,board);
    alreadyUpdated.push_back({rank+1,file});
  }
  if(actualRank == 4 && (abs(actualRank - rank) == 1) && board[rank+1][file] == 17){
    updateSlidingMove(rank+1,file,board);
    alreadyUpdated.push_back({rank+1,file});
  }
  if(rank != 7  && (board[rank+1][file] % 8 == 5 || board[rank+1][file] % 8 == 6)){
    //updateSlidingMove(rank+1,file,board);
    updateSlidingMove(rank+1,file,board);
    alreadyUpdated.push_back({rank+1,file});
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  if(file != 7 && (actualFile == file) && (board[rank][file+1] % 8 == 2 || (rank == 4 && board[rank][file+1] == 9) || (rank == 3 && board[rank][file+1] == 17))){
    if(board[rank][file+1] % 8 == 2 || !((file == blackEnPaisant) || (whiteEnPaisant == file))){
      updateSlidingMove(rank,file+1,board);
    }
    else if (board[rank][file+1] == 9){
      updateSlidingMove(rank+1,file,board);
    }
    else{
      updateSlidingMove(rank-1,file,board);
    }
    //updateSlidingMove(rank,file+1,board);
    alreadyUpdated.push_back({rank,file+1});
  }
  if(file != 7  && (board[rank][file+1] % 8 == 5 || board[rank][file+1] % 8 == 6)){
    updateSlidingMove(rank, file+1,board);
    //updateSlidingMove(rank,file+1,board);
    alreadyUpdated.push_back({rank,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  if(file != 0 && (actualFile == file) && (board[rank][file-1] % 8 == 2 || (rank == 4 && board[rank][file-1]== 9) || (rank == 3 && board[rank][file-1]== 17))){

    if(board[rank][file-1] % 8 == 2 || !(file == blackEnPaisant || whiteEnPaisant == file)){
      updateSlidingMove(rank,file-1,board);
    }
    else if (board[rank][file-1] == 9){
      updateSlidingMove(rank+1,file,board);
    }
    else{
      updateSlidingMove(rank-1,file,board);
    }
    alreadyUpdated.push_back({rank,file-1});
  }
  if(file != 0  && (board[rank][file-1] % 8 == 5 || board[rank][file-1] % 8 == 6)){
    updateSlidingMove(rank,file-1,board);
    //updateSlidingMove(rank,file-1,board);
    alreadyUpdated.push_back({rank,file-1});
  }
  file = actualFile;
  rank = actualRank;

  //Queen or Bishop || pawn || king
  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && (actualRank == rank) && (actualFile == file) && (board[rank-1][file+1] == 9 || board[rank-1][file+1] % 8 == 2)){
    updateSlidingMove(rank-1,file+1,board);
    alreadyUpdated.push_back({rank-1,file+1});
  }
  if(rank != 0 && file != 7 && (board[rank-1][file+1] % 8 == 4 || board[rank-1][file+1] % 8 == 6)){
    updateSlidingMove(rank-1,file+1,board);
    alreadyUpdated.push_back({rank-1,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && (actualRank == rank) && (actualFile == file) && ((board[rank+1][file+1] == 17) || board[rank+1][file+1] % 8 == 2)){
    updateSlidingMove(rank+1,file+1,board);
    alreadyUpdated.push_back({rank+1,file+1});
  }
  if(rank != 7 && file != 7 && (board[rank+1][file+1] % 8 == 4 || board[rank+1][file+1] % 8 == 6)){
    updateSlidingMove(rank+1,file+1,board);
    alreadyUpdated.push_back({rank+1,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && (actualRank == rank) && (actualFile == file) && (board[rank+1][file-1] == 17 || board[rank+1][file-1] % 8 == 2)){
    updateSlidingMove(rank+1,file-1,board);
    alreadyUpdated.push_back({rank+1,file-1});
  }
  if(rank != 7 && file != 0 && (board[rank+1][file-1] % 8 == 4 || board[rank+1][file-1] % 8 == 6)){
    updateSlidingMove(rank+1,file-1,board);
    alreadyUpdated.push_back({rank+1,file-1});
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && (actualRank == rank) && (actualFile == file) && (board[rank-1][file-1] == 9 || board[rank-1][file-1] % 8 == 2)){
    updateSlidingMove(rank-1,file-1,board);
    alreadyUpdated.push_back({rank-1,file-1});
  }
  if(rank != 0 && file != 0 && (board[rank-1][file-1] % 8 == 4 || board[rank-1][file-1] % 8 == 6)){
    updateSlidingMove(rank-1,file-1,board);
    alreadyUpdated.push_back({rank-1,file-1});
  }

  //endPos

  array<int,2> tempArr;

  actualRank = endPos[0];
  actualFile = endPos[1];

  rank = actualRank;
  file = actualFile;

  //Queen || rook || pawn || king

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  tempArr = {rank-1,file};
  if(rank != 0 && (actualRank == rank) && (board[rank-1][file] == 9 || board[rank-1][file] % 8 == 2) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank-1,file,board);
  }
  if(actualRank == 3 && (abs(actualRank - rank) == 1) && board[rank-1][file] == 9  && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank-1,file,board);
  }
  if(rank != 0  && (board[rank-1][file] % 8 == 5 || board[rank-1][file] % 8 == 6)  && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    //updateSlidingMove(rank-1,file,board);
    updateSlidingMove(rank-1,file,board);
    //updateUpMove(rank-1,file,board);
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  tempArr = {rank+1,file};
  if(rank != 7 && (actualRank == rank) && (board[rank+1][file] == 17 || board[rank+1][file] % 8 == 2) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank+1,file,board);
  }
  if(actualRank == 4 && (abs(actualRank - rank) == 1) && board[rank+1][file] == 17 && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank+1,file,board);
  }
  if(rank != 7  && (board[rank+1][file] % 8 == 5 || board[rank+1][file] % 8 == 6) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    //updateSlidingMove(rank+1,file,board);
    updateSlidingMove(rank+1,file,board);
    //auto it = find(whitePiecePos.begin(),whitePiecePos.end(),tempArr);

  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  tempArr = {rank,file+1};

  if(file != 7 && (actualFile == file) && (board[rank][file+1] % 8 == 2 || (rank == 4 && board[rank][file+1] == 9 && blackEnPaisant == file) || (rank == 3 && board[rank][file+1] == 17 && whiteEnPaisant == file)) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank,file+1,board);
  }
  if(file != 7  && (board[rank][file+1] % 8 == 5 || board[rank][file+1] % 8 == 6) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank, file+1,board);
    //updateSlidingMove(rank,file+1,board);
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  tempArr = {rank,file-1};
  if(file != 0 && (actualFile == file) && (board[rank][file-1] % 8 == 2 || (rank == 4 && board[rank][file-1]== 9 && blackEnPaisant == file) || (rank == 3 && board[rank][file-1]== 17 && whiteEnPaisant == file)) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank,file-1,board);
  }
  if(file != 0  && (board[rank][file-1] % 8 == 5 || board[rank][file-1] % 8 == 6) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank,file-1,board);
  }
  file = actualFile;
  rank = actualRank;

  //Queen or Bishop || pawn || king

  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  tempArr = {rank-1,file+1};
  if(rank != 0 && file != 7 && (actualRank == rank) && (actualFile == file) && (board[rank-1][file+1] == 9 || board[rank-1][file+1] % 8 == 2) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank-1,file+1,board);
  }
  if(rank != 0 && file != 7 && (board[rank-1][file+1] % 8 == 4 || board[rank-1][file+1] % 8 == 6) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank-1,file+1,board);
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  tempArr = {rank+1,file+1};
  if(rank != 7 && file != 7 && (actualRank == rank) && (actualFile == file) && ((board[rank+1][file+1] == 17) || board[rank+1][file+1] % 8 == 2) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank+1,file+1,board);
  }
  if(rank != 7 && file != 7 && (board[rank+1][file+1] % 8 == 4 || board[rank+1][file+1] % 8 == 6) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank+1,file+1,board);
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  tempArr = {rank+1,file-1};
  if(rank != 7 && file != 0 && (actualRank == rank) && (actualFile == file) && (board[rank+1][file-1] == 17 || board[rank+1][file-1] % 8 == 2) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank+1,file-1,board);
  }
  if(rank != 7 && file != 0 && (board[rank+1][file-1] % 8 == 4 || board[rank+1][file-1] % 8 == 6) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank+1,file-1,board);
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  tempArr = {rank-1,file-1};
  if(rank != 0 && file != 0 && (actualRank == rank) && (actualFile == file) && (board[rank-1][file-1] == 9 || board[rank-1][file-1] % 8 == 2) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank-1,file-1,board);
  }
  if(rank != 0 && file != 0 && (board[rank-1][file-1] % 8 == 4 || board[rank-1][file-1] % 8 == 6) && (find(alreadyUpdated.begin(),alreadyUpdated.end(),tempArr) == alreadyUpdated.end())){
    updateSlidingMove(rank-1,file-1,board);
  }
}
/*
void updateSlidingMoves(array<int,2> initPos,array<int,2> endPos,int** board){
  std::vector<array<int,2>> alreadyUpdated;
  //initPos

  int actualRank = initPos[0];
  int actualFile = initPos[1];

  int rank = initPos[0];
  int file = initPos[1];

  //Queen || rook || pawn || king

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  if(rank != 0 && (actualRank == rank) && (board[rank-1][file] == 9 || board[rank-1][file] % 8 == 2)){
    //updateSlidingMove(rank-1,file,board);
    updateSlidingMove(rank-1,file,board);
    alreadyUpdated.push_back({rank-1,file});
  }
  if(actualRank == 3 && (abs(actualRank - rank) == 1) && board[rank-1][file] == 9){
    //updateSlidingMove(rank-1,file,board);
    updateSlidingMove(rank-1,file,board);
    //updateSlidingMove(rank-1,file,board);
    alreadyUpdated.push_back({rank-1,file});
  }
  if(rank != 0  && (board[rank-1][file] % 8 == 5 || board[rank-1][file] % 8 == 6)){
    //updateSlidingMove(rank-1,file,board);
    updateSlidingMove(rank-1,file,board);
    //updateUpMove(rank-1,file,board);
    alreadyUpdated.push_back({rank-1,file});
  }
  file = actualFile;
  rank = actualRank;

  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  if(rank != 7 && (actualRank == rank) && (board[rank+1][file] == 17 || board[rank+1][file] % 8 == 2)){
    updateSlidingMove(rank+1,file,board);
    alreadyUpdated.push_back({rank+1,file});
  }
  if(actualRank == 4 && (abs(actualRank - rank) == 1) && board[rank+1][file] == 17){
    updateSlidingMove(rank+1,file,board);
    alreadyUpdated.push_back({rank+1,file});
  }
  if(rank != 7  && (board[rank+1][file] % 8 == 5 || board[rank+1][file] % 8 == 6)){
    //updateDownMove(rank+1,file,board);
    updateSlidingMove(rank+1,file,board);
    alreadyUpdated.push_back({rank+1,file});
  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  if(file != 7 && (actualFile == file) && (board[rank][file+1] % 8 == 2 || (rank == 4 && board[rank][file+1] == 9) || (rank == 3 && board[rank][file+1] == 17))){
    if(board[rank][file+1] % 8 == 2 || !((file == blackEnPaisant) || (whiteEnPaisant == file))){
      updateSlidingMove(rank,file+1,board);
    }
    else if (board[rank][file+1] == 9){
      updateSlidingMove(rank+1,file,board);
    }
    else{
      updateSlidingMove(rank-1,file,board);
    }
    //updateSlidingMove(rank,file+1,board);
    alreadyUpdated.push_back({rank,file+1});
  }
  if(file != 7  && (board[rank][file+1] % 8 == 5 || board[rank][file+1] % 8 == 6)){
    //updateRightMove(rank, file+1,board);
    updateSlidingMove(rank,file+1,board);
    alreadyUpdated.push_back({rank,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  if(file != 0 && (actualFile == file) && (board[rank][file-1] % 8 == 2 || (rank == 4 && board[rank][file-1]== 9) || (rank == 3 && board[rank][file-1]== 17))){

    if(board[rank][file-1] % 8 == 2 || !(file == blackEnPaisant || whiteEnPaisant == file)){
      updateSlidingMove(rank,file-1,board);
    }
    else if (board[rank][file-1] == 9){
      updateSlidingMove(rank+1,file,board);
    }
    else{
      updateSlidingMove(rank-1,file,board);
    }
    alreadyUpdated.push_back({rank,file-1});
  }
  if(file != 0  && (board[rank][file-1] % 8 == 5 || board[rank][file-1] % 8 == 6)){
    //updateLeftMove(rank,file-1,board);
    updateSlidingMove(rank,file-1,board);
    alreadyUpdated.push_back({rank,file-1});
  }
  file = actualFile;
  rank = actualRank;

  //Queen or Bishop || pawn || king
  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  if(rank != 0 && file != 7 && (actualRank == rank) && (actualFile == file) && (board[rank-1][file+1] == 9 || board[rank-1][file+1] % 8 == 2)){
    updateSlidingMove(rank-1,file+1,board);
    alreadyUpdated.push_back({rank-1,file+1});
  }
  if(rank != 0 && file != 7 && (board[rank-1][file+1] % 8 == 4 || board[rank-1][file+1] % 8 == 6)){
    updateSlidingMove(rank-1,file+1,board);
    alreadyUpdated.push_back({rank-1,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  if(rank != 7 && file != 7 && (actualRank == rank) && (actualFile == file) && ((board[rank+1][file+1] == 17) || board[rank+1][file+1] % 8 == 2)){
    updateSlidingMove(rank+1,file+1,board);
    alreadyUpdated.push_back({rank+1,file+1});
  }
  if(rank != 7 && file != 7 && (board[rank+1][file+1] % 8 == 4 || board[rank+1][file+1] % 8 == 6)){
    updateSlidingMove(rank+1,file+1,board);
    alreadyUpdated.push_back({rank+1,file+1});
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  if(rank != 7 && file != 0 && (actualRank == rank) && (actualFile == file) && (board[rank+1][file-1] == 17 || board[rank+1][file-1] % 8 == 2)){
    updateSlidingMove(rank+1,file-1,board);
    alreadyUpdated.push_back({rank+1,file-1});
  }
  if(rank != 7 && file != 0 && (board[rank+1][file-1] % 8 == 4 || board[rank+1][file-1] % 8 == 6)){
    updateSlidingMove(rank+1,file-1,board);
    alreadyUpdated.push_back({rank+1,file-1});
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  if(rank != 0 && file != 0 && (actualRank == rank) && (actualFile == file) && (board[rank-1][file-1] == 9 || board[rank-1][file-1] % 8 == 2)){
    updateSlidingMove(rank-1,file-1,board);
    alreadyUpdated.push_back({rank-1,file-1});
  }
  if(rank != 0 && file != 0 && (board[rank-1][file-1] % 8 == 4 || board[rank-1][file-1] % 8 == 6)){
    updateSlidingMove(rank-1,file-1,board);
    alreadyUpdated.push_back({rank-1,file-1});
  }

  //endPos

  array<int,2> tempArr;

  actualRank = endPos[0];
  actualFile = endPos[1];

  rank = actualRank;
  file = actualFile;

  //Queen || rook || pawn || king

  //Up
  while(rank != 0 && board[rank-1][file] == 0){
    rank--;
  }
  tempArr = {rank-1,file};
  if(rank != 0 && (actualRank == rank) && (board[rank-1][file] == 9 || board[rank-1][file] % 8 == 2)){
    updateSlidingMove(rank-1,file,board);
  }
  if(actualRank == 3 && (abs(actualRank - rank) == 1) && board[rank-1][file] == 9){
    updateSlidingMove(rank-1,file,board);
  }
  if(rank != 0  && (board[rank-1][file] % 8 == 5 || board[rank-1][file] % 8 == 6)){
    //updateSlidingMove(rank-1,file,board);
    updateSlidingMove(rank-1,file,board);
    //updateUpMove(rank-1,file,board);
  }
  file = actualFile;
  rank = actualRank;
  //Down
  while(rank != 7 && board[rank+1][file] == 0){
    rank++;
  }
  tempArr = {rank+1,file};
  if(rank != 7 && (actualRank == rank) && (board[rank+1][file] == 17 || board[rank+1][file] % 8 == 2)){
    updateSlidingMove(rank+1,file,board);
  }
  if(actualRank == 4 && (abs(actualRank - rank) == 1) && board[rank+1][file] == 17){
    updateSlidingMove(rank+1,file,board);
  }
  if(rank != 7  && (board[rank+1][file] % 8 == 5 || board[rank+1][file] % 8 == 6)){
    //updateDownMove(rank+1,file,board);
    updateSlidingMove(rank+1,file,board);
    //auto it = find(whitePiecePos.begin(),whitePiecePos.end(),tempArr);

  }
  file = actualFile;
  rank = actualRank;
  //Right
  while(file != 7 && board[rank][file+1] == 0){
    file++;
  }
  tempArr = {rank,file+1};

  if(file != 7 && (actualFile == file) && (board[rank][file+1] % 8 == 2 || (rank == 4 && board[rank][file+1] == 9 && blackEnPaisant == file) || (rank == 3 && board[rank][file+1] == 17 && whiteEnPaisant == file))){
    updateSlidingMove(rank,file+1,board);
  }
  if(file != 7  && (board[rank][file+1] % 8 == 5 || board[rank][file+1] % 8 == 6)){
    //updateRightMove(rank, file+1,board);
    updateSlidingMove(rank,file+1,board);
  }
  file = actualFile;
  rank = actualRank;
  //Left
  while(file != 0 && board[rank][file-1] == 0){
    file--;
  }
  tempArr = {rank,file-1};
  if(file != 0 && (actualFile == file) && (board[rank][file-1] % 8 == 2 || (rank == 4 && board[rank][file-1]== 9 && blackEnPaisant == file) || (rank == 3 && board[rank][file-1]== 17 && whiteEnPaisant == file))){
    updateSlidingMove(rank,file-1,board);
  }
  if(file != 0  && (board[rank][file-1] % 8 == 5 || board[rank][file-1] % 8 == 6)){
    //updateLeftMove(rank,file-1,board);
  }
  file = actualFile;
  rank = actualRank;

  //Queen or Bishop || pawn || king

  //Up-Right
  while(rank != 0 && file != 7 && board[rank-1][file+1] == 0){
    rank--;
    file++;
  }
  tempArr = {rank-1,file+1};
  if(rank != 0 && file != 7 && (actualRank == rank) && (actualFile == file) && (board[rank-1][file+1] == 9 || board[rank-1][file+1] % 8 == 2)){
    updateSlidingMove(rank-1,file+1,board);
  }
  if(rank != 0 && file != 7 && (board[rank-1][file+1] % 8 == 4 || board[rank-1][file+1] % 8 == 6)){
    updateSlidingMove(rank-1,file+1,board);
  }
  file = actualFile;
  rank = actualRank;
  //Down-right
  while(rank != 7 && file != 7 && board[rank+1][file+1] == 0){
    rank++;
    file++;
  }
  tempArr = {rank+1,file+1};
  if(rank != 7 && file != 7 && (actualRank == rank) && (actualFile == file) && ((board[rank+1][file+1] == 17) || board[rank+1][file+1] % 8 == 2)){
    updateSlidingMove(rank+1,file+1,board);
  }
  if(rank != 7 && file != 7 && (board[rank+1][file+1] % 8 == 4 || board[rank+1][file+1] % 8 == 6)){
    updateSlidingMove(rank+1,file+1,board);
  }
  file = actualFile;
  rank = actualRank;
  //Down-Left
  while(rank != 7 && file != 0 && board[rank+1][file-1] == 0){
    rank++;
    file--;
  }
  tempArr = {rank+1,file-1};
  if(rank != 7 && file != 0 && (actualRank == rank) && (actualFile == file) && (board[rank+1][file-1] == 17 || board[rank+1][file-1] % 8 == 2)){
    updateSlidingMove(rank+1,file-1,board);
  }
  if(rank != 7 && file != 0 && (board[rank+1][file-1] % 8 == 4 || board[rank+1][file-1] % 8 == 6)){
    updateSlidingMove(rank+1,file-1,board);
  }
  file = actualFile;
  rank = actualRank;
  //Up-Left
  while(rank != 0 && file != 0 && board[rank-1][file-1] == 0){
    rank--;
    file--;
  }
  tempArr = {rank-1,file-1};
  if(rank != 0 && file != 0 && (actualRank == rank) && (actualFile == file) && (board[rank-1][file-1] == 9 || board[rank-1][file-1] % 8 == 2)){
    updateSlidingMove(rank-1,file-1,board);
  }
  if(rank != 0 && file != 0 && (board[rank-1][file-1] % 8 == 4 || board[rank-1][file-1] % 8 == 6)){
    updateSlidingMove(rank-1,file-1,board);
  }
}
*/
void updateKnightMoves(array<int,2> initPos, array<int,2> endPos,int** board){
  std::vector<array<int,2>> alreadyUpdated;
  array<int,2> tempArr;
  //initPos
  int actualRank = initPos[0];
  int actualFile = initPos[1];

  int rank = initPos[0];
  int file = initPos[1];

  //Up-Right
  if(file != 7 && rank > 1 && board[rank-2][file+1] % 8 == 3){
    updateSlidingMove(rank-2,file+1,board);
    alreadyUpdated.push_back({rank-2,file+1});
  }
  //Right-Up
  tempArr = {rank-2,file+1};
  if(file < 6 && rank != 0 && board[rank-1][file+2] % 8 == 3 && !(find(alreadyUpdated.begin(), alreadyUpdated.end(), tempArr) != alreadyUpdated.end())){
    updateSlidingMove(rank-1,file+2,board);
    alreadyUpdated.push_back(tempArr);
  }
  //Right-Down
  tempArr = {rank+1,file+2};
  if(file < 6 && rank != 7 && board[rank+1][file+2] % 8 == 3 && !(find(alreadyUpdated.begin(), alreadyUpdated.end(), tempArr) != alreadyUpdated.end())){
    updateSlidingMove(rank+1,file+2,board);
    alreadyUpdated.push_back(tempArr);
  }
  //Down-Right
  tempArr = {rank+2,file+1};
  if(file != 7 && rank < 6 && board[rank+2][file+1] % 8 == 3 && !(find(alreadyUpdated.begin(), alreadyUpdated.end(), tempArr) != alreadyUpdated.end())){
    updateSlidingMove(rank+2,file+1,board);
    alreadyUpdated.push_back(tempArr);
  }
  //Down-Left
  tempArr = {rank+2,file-1};
  if(file != 0 && rank < 6 && board[rank+2][file-1] % 8 == 3 && !(find(alreadyUpdated.begin(), alreadyUpdated.end(), tempArr) != alreadyUpdated.end())){
    updateSlidingMove(rank+2,file-1,board);
    alreadyUpdated.push_back(tempArr);
  }
  //Left-Down
  tempArr = {rank+1,file-2};
  if(file > 1 && rank != 7 && board[rank+1][file-2] % 8 == 3 && !(find(alreadyUpdated.begin(), alreadyUpdated.end(), tempArr) != alreadyUpdated.end())){
    updateSlidingMove(rank+1,file-2,board);
    alreadyUpdated.push_back(tempArr);
  }
  //Left-Up
  tempArr = {rank-1,file-2};
  if(file > 1 && rank != 0 && board[rank-1][file-2] % 8 == 3 && !(find(alreadyUpdated.begin(), alreadyUpdated.end(), tempArr) != alreadyUpdated.end())){
    updateSlidingMove(rank-1,file-2,board);
    alreadyUpdated.push_back(tempArr);
  }
  //Up-Left
  tempArr = {rank-2,file-1};
  if(file !=0 && rank > 1 && board[rank-2][file-1] % 8 == 3 && !(find(alreadyUpdated.begin(), alreadyUpdated.end(), tempArr) != alreadyUpdated.end())){
    updateSlidingMove(rank-2,file-1,board);
    alreadyUpdated.push_back(tempArr);
  }

  //endPos

  actualRank = endPos[0];
  actualFile = endPos[1];

  rank = endPos[0];
  file = endPos[1];

  //Up-Right
  if(file != 7 && rank > 1 && board[rank-2][file+1] % 8 == 3){
    updateSlidingMove(rank-2,file+1,board);
  }
  //Right-Up
  if(file < 6 && rank != 0 && board[rank-1][file+2] % 8 == 3){
    updateSlidingMove(rank-1,file+2,board);
  }
  //Right-Down
  if(file < 6 && rank != 7 && board[rank+1][file+2] % 8 == 3){
    updateSlidingMove(rank+1,file+2,board);
  }
  //Down-Right
  if(file != 7 && rank < 6 && board[rank+2][file+1] % 8 == 3){
    updateSlidingMove(rank+2,file+1,board);
  }
  //Down-Left
  if(file != 0 && rank < 6 && board[rank+2][file-1] % 8 == 3){
    updateSlidingMove(rank+2,file-1,board);
  }
  //Left-Down
  if(file > 1 && rank != 7 && board[rank+1][file-2] % 8 == 3){
    updateSlidingMove(rank+1,file-2,board);
  }
  //Left-Up
  if(file > 1 && rank != 0 && board[rank-1][file-2] % 8 == 3){
    updateSlidingMove(rank-1,file-2,board);
  }
  //Up-Left
  if(file !=0 && rank > 1 && board[rank-2][file-1] % 8 == 3){
    updateSlidingMove(rank-2,file-1,board);
  }


}
/*
void updateSlidingPiecePositions(array<int,2> initPos, array<int,2> endPos){
  if(turnColour == white){
    auto it = find(whiteSlidingPiecePos.begin(), whiteSlidingPiecePos.end(), initPos);
    if(it == whiteSlidingPiecePos.end()){
      cout << "log Error updateWhiteSlidingPiecePositions" << endl;
      cout << initPos[0] << " " << initPos[1] << endl;
      cout << endPos[0] << " " << endPos[1] << endl;
      for(int i = 0;i<whiteSlidingPiecePos.size();i++){
        cout << whiteSlidingPiecePos[i][0] << " " << whiteSlidingPiecePos[i][1] << endl;
      }
      sleep(100);
    }
    int index = it - whiteSlidingPiecePos.begin();
    whiteSlidingPiecePos[index] = endPos;
  }
  else{
    auto it = find(blackSlidingPiecePos.begin(), blackSlidingPiecePos.end(), initPos);
    if(it == blackSlidingPiecePos.end()){
      cout << "log Error updateblackSlidingPiecePositions" << endl;
      sleep(100);
    }
    int index = it - blackSlidingPiecePos.begin();
    blackSlidingPiecePos[index] = endPos;
  }
}
*/
void updateBlackPiecePositions(array<int,2> initPos, array<int,2> endPos){
  if(endPos[1] > 7){
    cout << "Error with file > 7" << endl;
    sleep(100);
  }
  auto it = find(blackPiecePos.begin(), blackPiecePos.end(), initPos);
  if(it == blackPiecePos.end()){
    cout << "log Error updateBlackPiecePositions" << endl;
    cout << initPos[0] << " " << initPos[1] << endl;
    cout << endPos[0] << " " << endPos[1] << endl;
    for(int i = 0; i<blackPiecePos.size();i++){
      cout << blackPiecePos[i][0] << " " << blackPiecePos[i][1] << endl;
    }
    sleep(100);
  }
  int index = it - blackPiecePos.begin();
  blackPiecePos[index] = endPos;
  /*
  if(isSlidingPiece(initPos)){
    updateSlidingPiecePositions(initPos,endPos);
  }
  */
  //Check Capture
  it = find(whitePiecePos.begin(), whitePiecePos.end(), endPos);
  if (it != whitePiecePos.end()) {
    int index = it - whitePiecePos.begin();
    swap(whitePiecePos[index], whitePiecePos.back());
    whitePiecePos.pop_back();

    swap(whiteMoves[index], whiteMoves.back());
    whiteMoves.pop_back();
    /*
    it = find(whiteSlidingPiecePos.begin(), whiteSlidingPiecePos.end(), endPos);
    index = it - whiteSlidingPiecePos.begin();
    swap(whiteSlidingPiecePos[index], whiteSlidingPiecePos.back());
    whiteSlidingPiecePos.pop_back();
    */
  }
}

void updateWhitePiecePositions(array<int,2> initPos, array<int,2> endPos){
  auto it = find(whitePiecePos.begin(), whitePiecePos.end(), initPos);
  if(it == whitePiecePos.end()){
    cout << "log Error updateWhitePiecePositions" << endl;
    cout << "init" << initPos[0] << " " << initPos[1] << endl;
    cout << "end" << endPos[0] << " " << endPos[1] <<  endl;
    for(int i = 0; i < whitePiecePos.size();i++){
      cout << whitePiecePos[i][0] << " " << whitePiecePos[i][1] << endl;
    }

    sleep(100);
  }
  int index = it - whitePiecePos.begin();
  whitePiecePos[index] = endPos;
  /*
  if(isSlidingPiece(initPos)){
    updateSlidingPiecePositions(initPos,endPos);
  }
  */
  //Check Capture
  it = find(blackPiecePos.begin(), blackPiecePos.end(), endPos);
  if (it != blackPiecePos.end()) {
    int index = it - blackPiecePos.begin();
    swap(blackPiecePos[index], blackPiecePos.back());
    blackPiecePos.pop_back();
    swap(blackMoves[index], blackMoves.back());
    blackMoves.pop_back();

    /*
    it = find(blackSlidingPiecePos.begin(), blackSlidingPiecePos.end(), endPos);
    index = it - blackSlidingPiecePos.begin();
    swap(blackSlidingPiecePos[index], blackSlidingPiecePos.back());
    blackSlidingPiecePos.pop_back();
    */
  }
}

void updateWhitePieceMoves(array<int,2> initPos, array<int,2> endPos,int** board){
  auto it = find(whitePiecePos.begin(), whitePiecePos.end(), endPos);
  if(it == whitePiecePos.end()){
    cout << "log Error updateWhitePieceMoves" << endl;
    cout << "init" << initPos[0] << " " << initPos[1];
    cout << "end" << endPos[0] << " " << endPos[1];
    sleep(100);
  }
  int index = it - whitePiecePos.begin();
  whiteMoves[index] = pieceMoves(endPos[0],endPos[1],board);
  updateSlidingMoves(initPos,endPos,board);
  updateKnightMoves(initPos,endPos,board);

}

void updateblackPieceMoves(array<int,2> initPos, array<int,2> endPos,int** board){
  auto it = find(blackPiecePos.begin(), blackPiecePos.end(), endPos);
  if(it == blackPiecePos.end()){
    cout << "log Error updateblackPieceMoves" << endl;
    cout << "init: " << initPos[0] << " " << initPos[1] << endl;
    cout << "end: " << endPos[0] << " " << endPos[1] << endl;

    sleep(100);
  }
  int index = it - blackPiecePos.begin();
  blackMoves[index] = pieceMoves(endPos[0],endPos[1],board);

  updateSlidingMoves(initPos,endPos,board);

  updateKnightMoves(initPos,endPos,board);

}
/*
std::vector<array<int,2>> removeCheckMoves(std::vector<array<int,2>> potentialMoves, int rank, int file, int** board){
  std::vector<array<int,2>> initialpiecePostions[4];
  std::vector<vector<array<int,2>>> initialpieceMoves[2];
  array<int,2> initPos = {rank,file};
  array<int,2> endPos;
  for(int i = 0; i < potentialMoves.size(); i++){
    endPos = {potentialMoves[i][0],potentialMoves[i][1]};
    initialpieceMoves[0] = whiteMoves;
    initialpieceMoves[1] = blackMoves;
    initialpiecePostions[0] = whitePiecePos;
    initialpiecePostions[1] = blackPiecePos;
    initialpiecePostions[2] = whiteSlidingPiecePos;
    initialpiecePostions[3] = blackSlidingPiecePos;
    if(turnColour == white){
      updateWhitePiecePositions(initPos,endPos);
    }
    else{
      updateBlackPiecePositions(initPos,endPos);
    }
    int** copiedBoard = copyBoard(board);
    //EnPaisant
    if(copiedBoard[rank][file] % 8 == 1 && file != potentialMoves[i][1] && copiedBoard[potentialMoves[i][0]][potentialMoves[i][1]] == 0){
      if(turnColour == white){
        copiedBoard[potentialMoves[i][0] + 1][potentialMoves[i][1]] = 0;
        endPos = {potentialMoves[i][0]+1,potentialMoves[i][1]};
        auto it = find(blackPiecePos.begin(), blackPiecePos.end(), endPos);
        if (it != blackPiecePos.end()) {
          int index = it - blackPiecePos.begin();
          swap(blackPiecePos[index], blackPiecePos.back());
          blackPiecePos.pop_back();
        }
      }
      else{
        copiedBoard[potentialMoves[i][0] - 1][potentialMoves[i][1]] = 0;
        endPos = {potentialMoves[i][0] - 1,potentialMoves[i][1]};
        auto it = find(whitePiecePos.begin(), whitePiecePos.end(), endPos);
        if (it != whitePiecePos.end()) {
          int index = it - whitePiecePos.begin();
          swap(whitePiecePos[index], whitePiecePos.back());
          whitePiecePos.pop_back();
        }
      }
    }
    copiedBoard[potentialMoves[i][0]][potentialMoves[i][1]] = copiedBoard[rank][file];
    copiedBoard[rank][file] = 0;

    if(turnColour == white){
      updateWhitePieceMoves(initPos,endPos,board);
    }
    else{
      updateblackPieceMoves(initPos,endPos,board);
    }

    //cout << rank << " " << file << endl;
    //cout << potentialMoves[i][0] << " " << potentialMoves[i][1] << endl;
    if(moveResultsInCheck(copiedBoard)){
      swap(potentialMoves[i], potentialMoves.back());
      potentialMoves.pop_back();
      i--;
    }
    whiteMoves = initialpieceMoves[0];
    blackMoves = initialpieceMoves[1];
    whitePiecePos = initialpiecePostions[0];
    blackPiecePos = initialpiecePostions[1];
    whiteSlidingPiecePos = initialpiecePostions[2];
    blackSlidingPiecePos = initialpiecePostions[3];
    deleteBoard(copiedBoard);
  }
  return potentialMoves;
}
*/
void checkRookMove(int rank, int file, int** board){
  if(board[rank][file] % 8 != 5){
    return;
  }
  if(rank == 0 && file == 0 && !blackQueenRookMoved){
    blackQueenRookMoved = true;
  }
  else if(rank == 0 && file == 7 && !blackKingRookMoved){
    blackKingRookMoved = true;
  }
  else if(rank == 7 && file == 0 && !whiteQueenRookMoved){
    whiteQueenRookMoved = true;
  }
  else if(rank == 7 && file == 7 && !whiteKingRookMoved){
    whiteKingRookMoved = true;
  }
}

bool onlyKings(int** board){
  if(whitePiecePos.size() == 1 && blackPiecePos.size() == 1){
    return true;
  }
  return false;
}

int endGame(int** board){
  if(turnColour == white){
    return -1;
  }
  return 1;
}

bool applyTempMove(array<int,2> initPos, array<int,2> endPos, int** board){
  array<int,2> castling = {0,0};
  if((board[initPos[0]][initPos[1]] % 8 == 2) && abs(endPos[1] - initPos[1]) > 1){
    int** tempBoard = copyBoard(board);
    if(newMoveResultsInCheck(tempBoard)){
      deleteBoard(tempBoard);
      return false;
    }
    deleteBoard(tempBoard);
  }
  //cout << "calling applyTempMove" << endl;
  if(turnColour == white){
    updateWhitePiecePositions(initPos,endPos);
  }
  else{
    updateBlackPiecePositions(initPos,endPos);
  }
  checkRookMove(initPos[0], initPos[1],board);
  //Castling
  if((board[initPos[0]][initPos[1]] % 8 == 2) && abs(endPos[1] - initPos[1]) > 1){
    if((endPos[1] - initPos[1]) > 0){
      //kingSide
      if(turnColour == white){
        board[7][5] = board[7][7];
        board[7][7] = 0;
        whiteKingMoved = true;
        updateWhitePiecePositions({7,7},{7,5});
        //updateSlidingPiecePositions({7,7},{7,5});
        castling = {1,0};
      }
      else{
        board[0][5] = board[0][7];
        board[0][7] = 0;
        blackKingMoved = true;
        updateBlackPiecePositions({0,7},{0,5});
        //updateSlidingPiecePositions({0,7},{0,5});
        castling = {1,0};
      }
    }
    //castling queenSide
    else{
      if(turnColour == white){
        board[7][3] = board[7][0];
        board[7][0] = 0;
        whiteKingMoved = true;
        //cout << whiteQueenRookMoved << endl;
        updateWhitePiecePositions({7,0},{7,3});
        //updateSlidingPiecePositions({7,0},{7,3});
        castling = {1,1};
      }
      else{
        board[0][3] = board[0][0];
        board[0][0] = 0;
        blackKingMoved = true;
        updateBlackPiecePositions({0,0},{0,3});
        //updateSlidingPiecePositions({0,0},{0,3});
        castling = {1,1};
      }
    }
  }
  //KingMoved
  if(board[initPos[0]][initPos[1]] % 8 == 2){
    if(turnColour == white){
      whiteKingMoved = true;
    }
    else{
      blackKingMoved = true;
    }
  }
  //EnPaisant
  if((board[initPos[0]][initPos[1]] % 8 == 1) && abs(endPos[0] - initPos[0]) > 1){
    if(turnColour == white){
      blackEnPaisant = endPos[1];
      whiteEnPaisant = 9;
    }
    else{
      whiteEnPaisant = endPos[1];
      blackEnPaisant = 9;
    }
  }
  else{
    whiteEnPaisant = 9;
    blackEnPaisant = 9;
  }
//En Paisant capture
  if(board[initPos[0]][initPos[1]] % 8 == 1 && initPos[1] != endPos[1] && board[endPos[0]][endPos[1]] == 0){
    if(turnColour == white){
      board[endPos[0] + 1][endPos[1]] = 0;
      array<int,2> capturedPawn;
      capturedPawn[0] = endPos[0] + 1;
      capturedPawn[1] = endPos[1];
      auto it = find(blackPiecePos.begin(), blackPiecePos.end(), capturedPawn);
      if (it != blackPiecePos.end()) {
        int index = it - blackPiecePos.begin();
        swap(blackPiecePos[index], blackPiecePos.back());
        blackPiecePos.pop_back();

        swap(blackMoves[index], blackMoves.back());
        blackMoves.pop_back();
      }
      else{
        cout << "LogError removing black piece via EnPaisant" << endl;
        sleep(100);
      }
      board[endPos[0]][endPos[1]] = board[initPos[0]][initPos[1]];
      board[initPos[0]][initPos[1]] = 0;
      updateSlidingMoves(capturedPawn,capturedPawn,board);
      updateKnightMoves(capturedPawn,capturedPawn,board);
    }
    else{
      board[endPos[0] - 1][endPos[1]] = 0;
      array<int,2> capturedPawn;
      capturedPawn[0] = endPos[0] - 1;
      capturedPawn[1] = endPos[1];
      auto it = find(whitePiecePos.begin(), whitePiecePos.end(), capturedPawn);
      if (it != whitePiecePos.end()) {
        int index = it - whitePiecePos.begin();
        swap(whitePiecePos[index], whitePiecePos.back());
        whitePiecePos.pop_back();

        swap(whiteMoves[index], whiteMoves.back());
        whiteMoves.pop_back();
      }
      else{
        cout << "LogError removing white piece via EnPaisant" << endl;
        cout << endPos[0] << " " << endPos[1] << endl;
        cout << "whitePos: " << endl;
        for(int i=0;i<whitePiecePos.size();i++){
          cout << whitePiecePos[i][0] << " " << whitePiecePos[i][1] << endl;
        }
        boardPrinter(board);
        sleep(100);
      }
      board[endPos[0]][endPos[1]] = board[initPos[0]][initPos[1]];
      board[initPos[0]][initPos[1]] = 0;
      updateSlidingMoves(capturedPawn,capturedPawn,board);
      updateKnightMoves(capturedPawn,capturedPawn,board);
    }
  }
  else{
    board[endPos[0]][endPos[1]] = board[initPos[0]][initPos[1]];
    board[initPos[0]][initPos[1]] = 0;
  }
  //Queen -> pawn
  if(board[endPos[0]][endPos[1]] % 8 == 1 && (endPos[0] == 0 || endPos[0] == 7)){
    board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] + 5;
  }
  if(turnColour == white){
    updateWhitePieceMoves(initPos,endPos,board);
  }
  else{
    updateblackPieceMoves(initPos,endPos,board);
  }
/*
  if(!blackKingMoved && (initPos[0] == 0 || endPos[0] == 0)){
    array<int,2> kingPos = {0,4};
    auto it = find(blackPiecePos.begin(), blackPiecePos.end(), kingPos);
    int index = it - blackPiecePos.begin();
    blackMoves[index] = kingTurn(0,4,board);
  }
  if(!whiteKingMoved && (initPos[0] == 0 || endPos[0] == 0)){
    array<int,2> kingPos = {0,4};
    auto it = find(whitePiecePos.begin(), whitePiecePos.end(), kingPos);
    int index = it - whitePiecePos.begin();
    whiteMoves[index] = kingTurn(0,4,board);
  }
*/
  if(newMoveResultsInCheck(board)){
    return false;
  }
  //check square before castled square for check
  if(castling[0]){
    int** tempBoard = copyBoard(board);
    if(turnColour == white){
      //QS
      if(castling[1] == 1){
        tempBoard[7][3] = tempBoard[7][2];
        tempBoard[7][2] = 0;
        if(newMoveResultsInCheck(tempBoard)){
          deleteBoard(tempBoard);
          return false;
        }
        updateSlidingMoves({7,0},{7,3},board);
        updateKnightMoves({7,0},{7,3},board);
      }
      else{
        tempBoard[7][5] = tempBoard[7][6];
        tempBoard[7][6] = 0;
        if(newMoveResultsInCheck(tempBoard)){
          deleteBoard(tempBoard);
          return false;
        }
        updateSlidingMoves({7,7},{7,5},board);
        updateKnightMoves({7,7},{7,5},board);
      }
    }
    else{
      //QS
      if(castling[1] == 1){
        tempBoard[0][3] = tempBoard[0][2];
        tempBoard[0][2] = 0;
        if(newMoveResultsInCheck(tempBoard)){
          deleteBoard(tempBoard);
          return false;
        }
        updateSlidingMoves({0,0},{0,3},board);
        updateKnightMoves({0,0},{0,3},board);
      }
      else{
        tempBoard[0][5] = tempBoard[0][6];
        tempBoard[0][6] = 0;
        if(newMoveResultsInCheck(tempBoard)){
          deleteBoard(tempBoard);
          return false;
        }
        updateSlidingMoves({0,7},{0,5},board);
        updateKnightMoves({0,7},{0,5},board);
      }
    }
    deleteBoard(tempBoard);
  }

  if(turnColour == white){
    turnColour = black;
  }
  else{
    turnColour = white;
  }
  return true;
}

bool applyTempMoveKnight(array<int,2> initPos, array<int,2> endPos, int** board){

  whiteEnPaisant = 9;
  blackEnPaisant = 9;
  if(turnColour == white){
    updateWhitePiecePositions(initPos,endPos);
  }
  else{
    updateBlackPiecePositions(initPos,endPos);
  }
  board[endPos[0]][endPos[1]] = board[initPos[0]][initPos[1]];
  board[initPos[0]][initPos[1]] = 0;
  //knight pawn
  board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] + 2;

  if(turnColour == white){
    updateWhitePieceMoves(initPos,endPos,board);
  }
  else{
    updateblackPieceMoves(initPos,endPos,board);
  }

  if(newMoveResultsInCheck(board)){
    return false;
  }
  if(turnColour == white){
    turnColour = black;
  }
  else{
    turnColour = white;
  }
  return true;




}

bool applyTempMoveBishop(array<int,2> initPos, array<int,2> endPos, int** board){

  whiteEnPaisant = 9;
  blackEnPaisant = 9;
  if(turnColour == white){
    updateWhitePiecePositions(initPos,endPos);
  }
  else{
    updateBlackPiecePositions(initPos,endPos);
  }
  board[endPos[0]][endPos[1]] = board[initPos[0]][initPos[1]];
  board[initPos[0]][initPos[1]] = 0;
  //knight pawn
  board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] + 3;

  if(turnColour == white){
    updateWhitePieceMoves(initPos,endPos,board);
  }
  else{
    updateblackPieceMoves(initPos,endPos,board);
  }

  if(newMoveResultsInCheck(board)){
    return false;
  }
  if(turnColour == white){
    turnColour = black;
  }
  else{
    turnColour = white;
  }
  return true;

}

bool applyTempMoveRook(array<int,2> initPos, array<int,2> endPos, int** board){
  whiteEnPaisant = 9;
  blackEnPaisant = 9;
  if(turnColour == white){
    updateWhitePiecePositions(initPos,endPos);
  }
  else{
    updateBlackPiecePositions(initPos,endPos);
  }
  board[endPos[0]][endPos[1]] = board[initPos[0]][initPos[1]];
  board[initPos[0]][initPos[1]] = 0;
  //knight pawn
  board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] + 4;

  if(turnColour == white){
    updateWhitePieceMoves(initPos,endPos,board);
  }
  else{
    updateblackPieceMoves(initPos,endPos,board);
  }

  if(newMoveResultsInCheck(board)){
    return false;
  }
  if(turnColour == white){
    turnColour = black;
  }
  else{
    turnColour = white;
  }
  return true;

}

bool kingIsCastlingQueenSide(std::vector<array<int,2>> possMoves){
  for(int i = 0; i < possMoves.size(); i++){
    if(possMoves[i][1] == 2){
      return true;
    }
  }
  return false;
}
bool kingIsCastlingKingSide(std::vector<array<int,2>> possMoves){
  for(int i = 0; i < possMoves.size(); i++){
    if(possMoves[i][1] == 6){
      return true;
    }
  }
  return false;
}
bool kingIsCastling(std::vector<array<int,2>> possMoves){
  if((turnColour == white && whiteKingMoved == true) || (turnColour == black && blackKingMoved == true)){
    return false;
  }
  for(int i = 0; i<possMoves.size();i++){
    if(abs(possMoves[i][1] - 4) == 2){
      return true;
    }
  }
  return false;
}
std::vector<array<int,2>> removeKingSide(std::vector<array<int,2>> possMoves){
  for(int i = 0; i<possMoves.size();i++){
    if(possMoves[i][1] == 6){
      swap(possMoves[i], possMoves.back());
      possMoves.pop_back();
      return possMoves;
    }
  }
  cout << "shouldn't be here" << endl;
  sleep(10);
  return possMoves;
}
std::vector<array<int,2>> removeQueenSide(std::vector<array<int,2>> possMoves){
  for(int i = 0; i<possMoves.size();i++){
    if(possMoves[i][1] == 2){
      swap(possMoves[i], possMoves.back());
      possMoves.pop_back();
      return possMoves;
    }
  }
  cout << "shouldn't be here" << endl;
  sleep(10);
  return possMoves;
}

int generateMoves(int depth, int** board){
  if(depth == 0){
    //deleteBoard(copiedBoard);
    return 1;
  }
  int** copiedBoard = copyBoard(board);
  //cout << endl;
  int numPostions = 0;
  int prevNumPositions = 0;
  //int** copiedBoard = copyBoard(board);
  std::vector<array<int,2>> piecePositons = piecePos(board);
  for(int i = 0; i<piecePositons.size(); i++){
    /*
    if(board[7][2] == 18 && board[7][3] == 21){
      cout << "piece position is: " << piecePositons[i][0] << " " << piecePositons[i][1] << endl;
    }
    */
    //cout << "piece position is: " << piecePositons[i][0] << " " << piecePositons[i][1] << endl;
    //std::vector<array<int,2>> initpossMoves = pieceMoves(piecePositons[i][0],piecePositons[i][1],board);
    std::vector<array<int,2>> possMoves = NewpieceMoves(piecePositons[i],board);

    //cout << "init numMoves: " << initpossMoves.size() << endl;
    //std::vector<array<int,2>> possMoves = removeCheckMoves(initpossMoves,piecePositons[i][0],piecePositons[i][1], board);
    //cout << "numMoves: " << possMoves.size() << endl;
    /*
    if(board[piecePositons[i][0]][piecePositons[i][1]] % 8 == 2 && kingIsCastling(possMoves)){
      std::vector<array<int,2>> castleTester;
      if(kingIsCastlingKingSide(possMoves)){
        if(turnColour == white){
          castleTester.push_back({7,5});
        }
        else{
          castleTester.push_back({0,5});
        }
        castleTester = removeCheckMoves(castleTester,piecePositons[i][0],piecePositons[i][1], board);
        if(castleTester.empty()){
          possMoves = removeKingSide(possMoves);
        }
        else{
          castleTester.pop_back();
        }
      }
      if(kingIsCastlingQueenSide(possMoves)){
        if(turnColour == white){
          castleTester.push_back({7,3});
        }
        else{
          castleTester.push_back({0,3});
        }
        castleTester = removeCheckMoves(castleTester,piecePositons[i][0],piecePositons[i][1], board);
        if(castleTester.empty()){
          possMoves = removeQueenSide(possMoves);
        }
        else{
          castleTester.pop_back();
        }
      }

    }
    */
    for(int j = 0; j < possMoves.size(); j++){
      //if((board[piecePositons[i][0]][piecePositons[i][1]] % 8 == 1) && (possMoves[j][0] == 0 || possMoves[j][0] == 7)){
      //  numPostions = numPostions + 3;
      //}
      //cout << "move is: " << possMoves[j][0] << " " << possMoves[j][1] << endl;
      prevNumPositions = numPostions;
      int currTurnColour = turnColour;
      bool castleProperties[6];
      int EnPaisantProperties[2];
      std::vector<array<int,2>> initialpiecePostions[2];
      //std::vector<array<int,2>> initialSlidingpiecePostions[2];
      std::vector<vector<array<int,2>>> initialpieceMoves[2];
      initialpiecePostions[0] = whitePiecePos;
      initialpiecePostions[1] = blackPiecePos;
      //initialSlidingpiecePostions[0] = whiteSlidingPiecePos;
      //initialSlidingpiecePostions[1] = blackSlidingPiecePos;
      initialpieceMoves[0] = whiteMoves;
      initialpieceMoves[1] = blackMoves;
      castleProperties[0] = whiteKingMoved;
      castleProperties[1] = blackKingMoved;
      castleProperties[2] = whiteKingRookMoved;
      castleProperties[3] = whiteQueenRookMoved;
      castleProperties[4] = blackKingRookMoved;
      castleProperties[5] = blackQueenRookMoved;
      EnPaisantProperties[0] = whiteEnPaisant;
      EnPaisantProperties[1] = blackEnPaisant;
      if((board[piecePositons[i][0]][piecePositons[i][1]] % 8 == 1) && (possMoves[j][0] == 0 || possMoves[j][0] == 7)){
        bool promotable = false;
        if(applyTempMoveKnight(piecePositons[i],possMoves[j],board)){
          promotable = true;
          //int** tempBoard = copyBoard(board);
          numPostions = generateMoves(depth-1, board) + numPostions;
          //deleteBoard(tempBoard);
        }
        whiteMoves = initialpieceMoves[0];
        blackMoves = initialpieceMoves[1];
        whitePiecePos = initialpiecePostions[0];
        blackPiecePos = initialpiecePostions[1];
        //whiteSlidingPiecePos = initialpiecePostions[2];
        //blackSlidingPiecePos = initialpiecePostions[3];
        duplicateBoard(copiedBoard,board);

        turnColour = currTurnColour;

        if(promotable && applyTempMoveBishop(piecePositons[i],possMoves[j],board)){
          //int** tempBoard = copyBoard(board);
          numPostions = generateMoves(depth-1, board) + numPostions;
          //deleteBoard(tempBoard);
        }
        whiteMoves = initialpieceMoves[0];
        blackMoves = initialpieceMoves[1];
        whitePiecePos = initialpiecePostions[0];
        blackPiecePos = initialpiecePostions[1];
        //whiteSlidingPiecePos = initialpiecePostions[2];
        //blackSlidingPiecePos = initialpiecePostions[3];
        duplicateBoard(copiedBoard,board);

        turnColour = currTurnColour;

        if(promotable && applyTempMoveRook(piecePositons[i],possMoves[j],board)){
          //int** tempBoard = copyBoard(board);
          numPostions = generateMoves(depth-1, board) + numPostions;
          //deleteBoard(tempBoard);
        }
        whiteMoves = initialpieceMoves[0];
        blackMoves = initialpieceMoves[1];
        whitePiecePos = initialpiecePostions[0];
        blackPiecePos = initialpiecePostions[1];
        //whiteSlidingPiecePos = initialpiecePostions[2];
        //blackSlidingPiecePos = initialpiecePostions[3];
        duplicateBoard(copiedBoard,board);

        turnColour = currTurnColour;
      }

      if(applyTempMove(piecePositons[i],possMoves[j],board)){
        //cout << "copyBoard6" << endl;
        //int** tempBoard = copyBoard(board);
        //cout << "copyBoard6" << endl;
        numPostions = generateMoves(depth-1, board) + numPostions;
        //deleteBoard(tempBoard);
      }
      else{
        //cout << "failed" << endl;
      }
      //cout << "whitePiecePos: " << endl;
      //for(int i = 0; i < whitePiecePos.size();i++){
      //  cout << whitePiecePos[i][0] << " " << whitePiecePos[i][1] << endl;
      //}
      whiteMoves = initialpieceMoves[0];
      blackMoves = initialpieceMoves[1];
      whitePiecePos = initialpiecePostions[0];
      blackPiecePos = initialpiecePostions[1];
      //whiteSlidingPiecePos = initialSlidingpiecePostions[0];
      //blackSlidingPiecePos = initialSlidingpiecePostions[1];
      whiteKingMoved = castleProperties[0];
      blackKingMoved = castleProperties[1];
      whiteKingRookMoved = castleProperties[2];
      whiteQueenRookMoved = castleProperties[3];
      blackKingRookMoved = castleProperties[4];
      blackQueenRookMoved = castleProperties[5];
      whiteEnPaisant = EnPaisantProperties[0];
      blackEnPaisant = EnPaisantProperties[1];
      turnColour = currTurnColour;
      //board = copyBoard(copiedBoard);
      duplicateBoard(copiedBoard,board);
      //cout << "move is: " << possMoves[j][0] << " " << possMoves[j][1] << endl;
      if(depth == 6){
        cout << "current node: " << piecePositons[i][0] << " " << piecePositons[i][1] << endl;
        cout << "move is: " << possMoves[j][0] << " " << possMoves[j][1] << endl;
        cout << "numPostions: " << numPostions - prevNumPositions << endl;
      }
      /*
      if(board[7][2] == 18 && board[7][3] == 21){
        cout << "move is: " << possMoves[j][0] << " " << possMoves[j][1] << endl;
        cout << "numPostions: " << numPostions - prevNumPositions << endl;
        sleep(1);
      }
      */
      //cout << "passed gm" << endl;
    }
    if(depth == 6){
      cout << endl;
      //sleep(10);
    }
  }
  deleteBoard(copiedBoard);
  return numPostions;
}
/*
int applyMove(int** board){
  if(onlyKings(board)){
    return 0;
  }
  std::vector<array<int,2>> piecePositons = piecePos(board);

  int specificPiece = rand() % piecePositons.size();
  //int specificPiece = 10;
  cout << "piece is :" << " ";
  cout << board[piecePositons[specificPiece][0]][piecePositons[specificPiece][1]] << endl;
  std::vector<array<int,2>> possMoves = pieceMoves(piecePositons[specificPiece][0],piecePositons[specificPiece][1],board);
  cout << "init possMoves " << endl;
  for(int i = 0; i<possMoves.size(); i++){
    cout << possMoves[i][0] << " ";
    cout << possMoves[i][1] << endl;
  }
  possMoves = removeCheckMoves(possMoves,piecePositons[specificPiece][0],piecePositons[specificPiece][1], board);
  cout << "afterRemove possMoves " << endl;
  for(int i = 0; i<possMoves.size(); i++){
    cout << possMoves[i][0] << " ";
    cout << possMoves[i][1] << endl;
  }
  while(possMoves.size() == 0){
    swap(piecePositons[specificPiece], piecePositons.back());
    piecePositons.pop_back();
    if(piecePositons.size() == 0){
      int result = endGame(board);
      return result;
    }
    specificPiece = rand() % piecePositons.size();
    possMoves = pieceMoves(piecePositons[specificPiece][0],piecePositons[specificPiece][1],board);
    cout << "piece is :" << " ";
    cout << board[piecePositons[specificPiece][0]][piecePositons[specificPiece][1]] << endl;
    cout << "init possMoves " << endl;
    for(int i = 0; i<possMoves.size(); i++){
      cout << possMoves[i][0] << " ";
      cout << possMoves[i][1] << endl;
    }
    possMoves = removeCheckMoves(possMoves,piecePositons[specificPiece][0],piecePositons[specificPiece][1], board);
    cout << "afterRemove possMoves " << endl;
    for(int i = 0; i<possMoves.size(); i++){
      cout << possMoves[i][0] << " ";
      cout << possMoves[i][1] << endl;
    }
  }
  int specificMove = rand() % possMoves.size();
  array<int,2> actualMove = possMoves[specificMove];
  checkRookMove(piecePositons[specificPiece][0], piecePositons[specificPiece][1],board);
  //if king is moving more than 1 square -> castling
  if((board[piecePositons[specificPiece][0]][piecePositons[specificPiece][1]] % 8 == 2) && abs(actualMove[1] - piecePositons[specificPiece][1]) > 1){
    cout << "castling" << endl;
    sleep(3);//sleeps for 3 second
    //castling kingSide

    if((actualMove[1] - piecePositons[specificPiece][1]) > 0){
      cout << "kingSide" << endl;
      if(turnColour == white){
        board[7][5] = board[7][7];
        board[7][7] = 0;
        whiteKingMoved = true;
      }
      else{
        board[0][5] = board[0][7];
        board[0][7] = 0;
        blackKingMoved = true;
      }
    }
    //castling queenSide
    else{
      cout << "queenSide" << endl;
      if(turnColour == white){
        board[7][3] = board[7][0];
        board[7][0] = 0;
        whiteKingMoved = true;
      }
      else{
        board[0][3] = board[0][0];
        board[0][0] = 0;
        blackKingMoved = true;
      }
    }
  }
  if(board[piecePositons[specificPiece][0]][piecePositons[specificPiece][1]] % 8 == 2){
    if(turnColour == white){
      whiteKingMoved = true;
    }
    else{
      blackKingMoved = true;
    }
  }
  cout << "move is :" << " ";
  cout << actualMove[0] << " ";
  cout << actualMove[1] << endl;
  board[actualMove[0]][actualMove[1]] = board[piecePositons[specificPiece][0]][piecePositons[specificPiece][1]];
  board[piecePositons[specificPiece][0]][piecePositons[specificPiece][1]] = 0;
  //Queen pawn
  if(board[actualMove[0]][actualMove[1]] % 8 == 1 && (actualMove[0] == 0 || actualMove[0] == 7
  )){
    board[actualMove[0]][actualMove[1]] = board[actualMove[0]][actualMove[1]] + 5;
  }
  if(turnColour == white){
    turnColour = black;
  }
  else{
    turnColour = white;
  }
  return 9;
}
*/
int main(){

  int** board = initBoard();

  const string startFen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
  loadBoardFromFen(startFen, board);
  setupAttackingSquares(board);
  //setupSlidingPiecePos(board);
  boardPrinter(board);
  /*
  for(int i = 0; i< blackMoves.size();i++){
    cout << blackPiecePos[i][0] << " " << blackPiecePos[i][1] << endl;
    for(int j = 0;j<blackMoves[i].size();j++){
      cout << blackMoves[i][j][0] << " " << blackMoves[i][j][1] << endl;
    }
    cout << endl;
  }
  */
  clock_t start = clock();
  int result = generateMoves(5,board);
  cout << result << endl;
  cout << boards << endl;
  clock_t end = clock();
  printf ("time: %0.8f sec\n", ((float) end - start)/CLOCKS_PER_SEC);
  //int result = 9;
  //srand(time(0));
  /*
  //for(int i = 0; i<81;i++){
  while(result == 9){
    cout << turnColour << endl;
    result = applyMove(board);
    boardPrinter(board);
  }
  cout << result << endl;
  */
  return 0;
}
