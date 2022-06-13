#ifndef PIECELIST_H // This needs to be unique in each header
#define PIECELIST_H

#include <iostream>
#include <array>
using namespace std;
class PieceList{
  private:
    int numPieces;
    array<int,2>* occupiedSquares;
    int** pieceMap;
  public:
    PieceList();
    array<int,2>* getOccupiedSquares();
    int getSize();
    void setOccupiedSquares(int maxSize);
    void deleteOccupiedSquares();
    void setPieceMap(int** pieceMap);
    void deletePieceMap();
    int** getPieceMap();
    //bool duplicates();

    void addPieceAtSquare(int rank, int file);

    void removePiece(array<int,2> initPos, array<int,2> endPos);

    void movePiece(array<int,2> initPos, array<int,2> endPos);
};

#endif
