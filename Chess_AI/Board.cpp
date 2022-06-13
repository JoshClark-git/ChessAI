#include <iostream>
#include <array>
#include <unistd.h>
#include "PieceList.h"
#include "Globals.h"
#include "Board.h"
using namespace std;

  void Board::updateStacks(){
    for(int i = 0; i<6;i++){
      castlePropertiesStack.push(this->castleProperties[i]);
    }
    EnPaisantPropertiesStack.push(this->EnPaisantProperties[0]);
    EnPaisantPropertiesStack.push(this->EnPaisantProperties[1]);

  }
  int** Board::initPieceMap(){
    int** map = new int*[8];
    for(int i = 0; i < 8; i++){
      map[i] = new int[8];
      for(int j = 0; j < 8; j++){
        map[i][j] = 9;
      }
    }
    return map;
  }
  void Board::setupPieceLists(){
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

  void Board::deletePieceLists(){
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


  Board::Board(int** board,array<bool,6> castleProperties,array<int,2> EnPaisantProperties){
    this->boardReprentation = board;
    this->castleProperties = castleProperties;
    this->EnPaisantProperties = EnPaisantProperties;
    setupPieceLists();
    updateStacks();
  }

  void Board::newBoard(int** board){
    this->boardReprentation = board;
  }

  void Board::deleteBoard(){
    for(int i =0; i<8;i++){
      delete this->boardReprentation[i];
    }
    delete this->boardReprentation;
  }

  array<int,2> Board::getEnPaisantProperties(){
    return this->EnPaisantProperties;
  }
  array<bool,6> Board::getCastleProperties(){
    return this->castleProperties;
  }
  void Board::setEnPaisantProperties(array<int,2> properties){
    this->EnPaisantProperties = properties;
  }
  void Board::setCastleProperties(array<bool,6> properties){
    this->castleProperties = properties;
  }
  void Board::setBoard(int** board){
    this->boardReprentation = board;
  }
  int** Board::getBoard(){
    return this->boardReprentation;
  }
  void Board::restoreStacks(){
    for(int i = 0;i < 6;i++){
      castlePropertiesStack.pop();
    }
    EnPaisantPropertiesStack.pop();
    EnPaisantPropertiesStack.pop();
  }
  void Board::restoreQualities(){
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
  void Board::boardPrinter(int** board){
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
  void Board::checkRookMove(int rank, int file, int** board){
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
  void Board::updateProperties(array<int,2> initPos, array<int,2> endPos){
    int** board = this->getBoard();
    //RookMoved
    checkRookMove(initPos[0],initPos[1],board);
    //KingMoved
    if(board[initPos[0]][initPos[1]] % 8 == 2){
      if(board[initPos[0]][initPos[1]] == 18){
        this->castleProperties[0] = true;
        this->castleProperties[1] = true;
        this->castleProperties[2] = true;
      }
      else{
        this->castleProperties[3] = true;
        this->castleProperties[4] = true;
        this->castleProperties[5] = true;
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

  array<PieceList,6> Board::getWhitePieces(){
    return {this->pawnPositions[1],this->knightPositions[1],this->bishopPositions[1],
    this->rookPositions[1],this->queenPositions[1],this->kingPositions[1]};
  }
  array<PieceList,6> Board::getBlackPieces(){
    return {this->pawnPositions[0],this->knightPositions[0],this->bishopPositions[0],
    this->rookPositions[0],this->queenPositions[0],this->kingPositions[0]};
  }

  void Board::updatePiecePos(array<int,2> initPos,array<int,2> endPos, int pieceType, int pieceColour){
    //cout << "updating: " << initPos[0] << " " << initPos[1] << " to: " << endPos[0] << " " << endPos[1] << endl;
    switch(pieceType){
      case 1:
        this->pawnPositions[pieceColour].movePiece(initPos,endPos);
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
        sleep(9999);
    }
  }

  void Board::updatePieceRemoval(array<int,2> initPos,array<int,2> endPos, int pieceType, int pieceColour){
     switch(pieceType){
      case 1:
        this->pawnPositions[pieceColour].removePiece(initPos,endPos);
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

  void Board::restorePiecePos(int rank, int file, int pieceType, int pieceColour){
    switch(pieceType){
      case 1:
        this->pawnPositions[pieceColour].addPieceAtSquare(rank,file);
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
        sleep(9999);
    }
  }

  int Board::applyMoveForward(array<int,2> initPos,array<int,2> endPos){
    int ** board = this->boardReprentation;
    int piece = board[endPos[0]][endPos[1]];
    int pieceColour;
    //enPaisant capture

    if(board[initPos[0]][initPos[1]] % 8 == 1 && initPos[1] != endPos[1] && board[endPos[0]][endPos[1]] == 0){
      if(turnColour == white){
        board[endPos[0] + 1][endPos[1]] = 0;
        array<int,2> capturedPawn;
        capturedPawn[0] = endPos[0] + 1;
        capturedPawn[1] = endPos[1];
        updatePieceRemoval(initPos,capturedPawn,1, 0);
      }
      else{
        board[endPos[0] - 1][endPos[1]] = 0;
        array<int,2> capturedPawn;
        capturedPawn[0] = endPos[0] - 1;
        capturedPawn[1] = endPos[1];
        updatePieceRemoval(initPos,capturedPawn,1, 1);
      }
    }
    //Some capture or promotion
    if(piece != 0){
      int pieceColour;
      if(piece > 16){
        pieceColour = 1;
      }
      else{
        pieceColour = 0;
      }
      this->updatePieceRemoval(initPos,endPos,piece % 8,pieceColour);
    }

    int pieceType = board[initPos[0]][initPos[1]];

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
      restorePiecePos(endPos[0], endPos[1], 6, pieceColour);
      if(piece != 0){
        piece = piece * 10;
      }
      else{
        piece = 1;
      }
    }
    return piece;
  }
  void Board::applyMoveReverse(array<int,2> initPos,array<int,2> endPos,int pieceType,int piece){
    int ** board = this->boardReprentation;
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
  int Board::make_move(array<int,2> initPos,array<int,2> endPos){
    updateProperties(initPos,endPos);
    updateStacks();
    int** board = this->boardReprentation;
    //castling

    if((board[initPos[0]][initPos[1]] % 8 == 2) && abs(endPos[1] - initPos[1]) > 1){
      if((endPos[1] - initPos[1]) > 0){
        //kingSide
        if(turnColour == white){
          board[7][5] = board[7][7];
          board[7][7] = 0;
          updatePiecePos({7,7},{7,5}, 5, 1);
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

    if(turnColour == white){
      turnColour = black;
    }
    else{
      turnColour = white;
    }

    return piece;
  }
  void Board::unmake_move(array<int,2> initPos,array<int,2> endPos, int piece){
    if(turnColour == white){
      turnColour = black;
    }
    else{
      turnColour = white;
    }
    int** board = this->getBoard();
    //castling
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

      this->restorePiecePos(initPos[0],initPos[1],(piece/10) % 8, pieceColour);
      // remove queen at endPos
      this->updatePieceRemoval(initPos,endPos, 6, currPieceColour);
      // add pawn back
      this->restorePiecePos(endPos[0],endPos[1],1, currPieceColour);

    }
    //Promotion, no capture
    else if(piece == 1){

      board[endPos[0]][endPos[1]] = board[endPos[0]][endPos[1]] - 5;
      if(turnColour == white){
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
      if(turnColour == white){
        this->boardReprentation[initPos[0] + 1][initPos[1]] = 9;
        this->restorePiecePos(initPos[0] + 1,initPos[1],1, 0);

      }
      else{
        this->boardReprentation[initPos[0] - 1][initPos[1]] = 17;
        this->restorePiecePos(initPos[0] - 1,initPos[1],1, 1);

      }
    }
  }
