CC = g++
OPT = -O3
SHARED = -fPIC -shared

#g++ -O3 -fPIC -shared -o libTest.so cleanbarebone.cpp

libTest.so: PieceList.cpp Board.cpp Hash.cpp Evaluate.cpp move.cpp Search.cpp
    $(CC) $(OPT) $(SHARED) -o libTest.so PieceList.cpp Board.cpp Hash.cpp Evaluate.cpp move.cpp Search.cpp

#		skipped is: 6601
#		accum is: 486674
#depth 5 at 1.165
