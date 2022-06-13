#include <iostream>
#include <array>
#include "PieceList.h"
using namespace std;

PieceList::PieceList(){
  this->pieceMap;
  this->numPieces = 0;
  this->occupiedSquares;

}
array<int,2>* PieceList::getOccupiedSquares(){
  return this->occupiedSquares;
}
int PieceList::getSize(){
  return this->numPieces;
}
void PieceList::setOccupiedSquares(int maxSize){
  this->occupiedSquares = new array<int,2>[maxSize];
}
void PieceList::deleteOccupiedSquares(){
  delete this->occupiedSquares;
  this->numPieces = 0;
}
void PieceList::setPieceMap(int** pieceMap){
  this->pieceMap = pieceMap;
}
void PieceList::deletePieceMap(){
  int** pieceMap = this->pieceMap;
  for(int i = 0; i < 8; i++){
    delete pieceMap[i];
  }
  delete pieceMap;
}
int** PieceList::getPieceMap(){
  return this->pieceMap;
}
void PieceList::addPieceAtSquare(int rank, int file){
  array<int,2> newSquare = {rank,file};
  this->occupiedSquares[this->numPieces] = newSquare;
  this->pieceMap[rank][file] = this->numPieces;
  this->numPieces++;
}
void PieceList::removePiece(array<int,2> initPos, array<int,2> endPos){
  int pieceIndex = this->pieceMap[endPos[0]][endPos[1]];
  this->occupiedSquares[pieceIndex] = this->occupiedSquares[this->numPieces - 1]; // move last element in array to the place of the removed element
  this->pieceMap[this->occupiedSquares[pieceIndex][0]][this->occupiedSquares[pieceIndex][1]] = pieceIndex; // update map to point to the moved element's new location in the array
  this->numPieces--;
}
void PieceList::movePiece(array<int,2> initPos, array<int,2> endPos){
  int pieceIndex = this->pieceMap[initPos[0]][initPos[1]]; // get the index of this element in the occupiedSquares array
  this->occupiedSquares[pieceIndex] = endPos;
  this->pieceMap[endPos[0]][endPos[1]] = pieceIndex;
}
