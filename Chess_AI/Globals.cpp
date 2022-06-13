#include <iostream>
#include <stdlib.h>
#include "Globals.h"
using namespace std;

for(int i = 0; i < 8; i++){
  for(int j = 0; j < 8;j++){
    int maxFileDst = Max(3 - j, j - 4);
    int maxRankDst = Max(3 - i, i - 4);
    centerManhattanDistance[i*8+j] = maxFileDst + maxRankDst;
    cout << i << " " << j << " " << maxFileDst + maxRankDst << endl;
  }
}
