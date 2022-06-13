import pygame as p
import random
import time
import os
import ctypes
import pygame.gfxdraw

MAX_FPS = 15
blackFlip = [7,6,5,4,3,2,1,0]
WIDTH = HEIGHT = 512
AICOLOR = None
HUMANCOLOR = None
AIDIFFICULTY = None
IMAGES = {}
TURNCOLOR = 16
currTurn = 16

prevHighlight = []

whiteKingMoved = True
blackKingMoved = True

whiteKingRookMoved = False;
whiteQueenRookMoved = False;
blackKingRookMoved = False;
blackQueenRookMoved = False;

blackEnPaisant = 9;
whiteEnPaisant = 9;

charptr = ctypes.POINTER(ctypes.c_char)

dir_path = os.path.dirname(os.path.realpath(__file__))
handle = ctypes.CDLL(dir_path + "/libTest.so")     

handle.My_Function.argtypes = [charptr]
handle.My_Function.restype = ctypes.c_int

handle.My_Function.argtypes = [charptr]
handle.My_Function.restype = ctypes.c_int
  
def New_Function(string):
    return handle.New_Function(string)
def My_Function(string):
    return handle.My_Function(string)

class GameState():
    def __init__(self):
        self.board = [
            ["bR","bN","bB","bQ","bK","bB","bN","bR"],
            ["bP","bP","bP","bP","bP","bP","bP","bP"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["wP","wP","wP","wP","wP","wP","wP","wP"],
            ["wR","wN","wB","wQ","wK","wB","wN","wR"]]

        
        self.board = [
            ["bR","--","--","--","--","bR","bK","--"],
            ["bP","bP","--","--","bB","bP","bP","--"],
            ["--","--","--","--","bP","bN","--","bP"],
            ["--","--","--","bP","--","--","--","--"],
            ["--","wP","--","--","--","wB","--","--"],
            ["--","--","wP","--","--","wQ","--","--"],
            ["wP","--","--","wN","--","wP","wP","wP"],
            ["--","--","wR","--","--","wR","wK","--"],]
        '''
        self.board = [
            ["--","--","--","--","--","bR","bK","--"],
            ["--","--","--","--","--","bP","bP","bP"],
            ["--","--","--","--","bP","--","--","--"],
            ["--","--","--","--","wP","--","--","--"],
            ["--","--","--","--","wR","--","wQ","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","wP","wP","wP"],
            ["--","--","--","--","--","--","wK","--"]]

        
        self.board = [
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","bK","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","bP","--","bP","--","wP","--"],
            ["--","bP","--","bP","--","bR","--","wP"],
            ["--","--","--","--","wK","--","--","--"],
            ["--","--","--","--","--","wP","--","--"],
            ["--","--","--","--","--","--","--","wR"]]
    
        
        self.board = [
            ["--","--","--","--","--","--","--","--"],
            ["--","wR","--","--","--","bP","bP","bK"],
            ["--","--","--","--","bP","--","--","bP"],
            ["bR","bP","--","--","wP","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","bB","--","wB","wP","--","--"],
            ["--","--","--","--","--","wP","wK","wP"],
            ["--","--","--","--","--","--","--","--"]]
        
        self.board = [
            ["--","--","--","--","--","--","--","--"],
            ["--","bP","--","--","--","--","--","--"],
            ["bP","--","--","--","bK","--","--","--"],
            ["--","--","--","bP","--","bR","--","--"],
            ["--","--","bB","--","bP","wB","--","--"],
            ["--","--","--","--","wR","--","--","--"],
            ["wP","--","wP","--","--","--","--","--"],
            ["--","wK","--","--","--","--","--","--"]]
        
        self.board = [
            ["--","--","--","--","--","--","--","--"],
            ["--","wN","--","bP","--","--","--","--"],
            ["bP","--","--","--","bP","--","--","bP"],
            ["--","--","--","--","--","--","bK","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","wP","--","--","--","--"],
            ["wP","bB","bR","--","--","--","--","wP"],
            ["--","--","--","--","--","wR","--","wK"]]
        
        
        self.board = [
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","bK","--","--","bP"],
            ["--","bP","--","bR","--","--","bP","--"],
            ["--","--","--","--","--","--","--","--"],
            ["wP","--","wK","--","--","wP","--","wP"],
            ["--","--","--","--","--","--","--","--"],
            ["bP","wP","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","--"]]
        
        self.board = [
            ["bR","--","--","--","--","bR","bK","--"],
            ["bP","--","--","--","--","bP","--","bP"],
            ["--","bB","bP","--","--","bP","--","--"],
            ["--","--","--","bQ","wP","bB","--","--"],
            ["--","--","--","wP","--","--","bN","--"],
            ["--","--","--","wQ","--","--","--","--"],
            ["wP","wP","wP","--","wN","--","wP","wP"],
            ["wR","--","wB","--","wK","--","--","wR"]]
        
        self.board = [
            ["--","--","--","--","--","bR","bK","--"],
            ["bP","--","bP","bQ","--","bP","bP","bP"],
            ["--","bP","bN","--","bP","--","--","wP"],
            ["--","--","wP","bP","wP","--","wB","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","wQ","--","wN","--","--"],
            ["wP","bR","--","--","--","wP","wP","--"],
            ["wR","--","--","--","--","--","wK","--"]]
        
        self.board = [
            ["--","--","--","bR","--","bQ","bK","--"],
            ["bP","--","bP","--","bB","bP","bP","--"],
            ["--","--","bN","--","bP","--","--","--"],
            ["--","wP","--","bP","wP","bN","wP","--"],
            ["--","--","--","wP","--","wB","wQ","--"],
            ["--","--","wP","--","--","--","--","--"],
            ["wP","wP","--","wN","--","wN","--","--"],
            ["--","--","wK","--","--","--","--","wR"]]
        
        self.board = [
            ["bR","--","--","--","bQ","--","bK","--"],
            ["--","bP","bP","--","bB","bP","bP","--"],
            ["--","--","bN","--","bP","--","--","--"],
            ["bP","--","--","bP","wP","bN","wP","--"],
            ["--","--","--","wP","--","wB","wQ","--"],
            ["--","--","wP","--","--","--","--","--"],
            ["wP","wP","--","wN","--","wN","--","--"],
            ["--","--","wK","wR","--","--","--","--"]]
        self.board = [
            ["bR","--","--","bR","--","--","bK","--"],
            ["bP","--","--","--","--","bP","--","--"],
            ["--","bB","bP","--","--","bP","--","--"],
            ["--","--","--","bQ","wP","--","--","--"],
            ["--","--","--","--","--","--","--","wQ"],
            ["--","--","--","--","--","--","--","--"],
            ["wP","wP","wP","--","wN","--","wP","wP"],
            ["wR","--","wB","--","wK","--","--","wR"]]
        
        self.board = [
            ["bR","bN","bB","bQ","bK","bB","bN","bR"],
            ["bP","bP","bP","bP","bP","bP","bP","bP"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","bR","--","--"],
            ["bR","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","wK"]]
        
        self.board = [
            ["bR","bN","bB","bQ","bK","bB","bN","bR"],
            ["bP","bP","bP","bP","bP","bP","bP","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","--","--","--","bP"],
            ["--","--","--","--","--","--","wQ","--"],
            ["--","--","--","--","--","--","--","wK"]]
         
        self.board = [
            ["bR","bN","--","bQ","bK","--","bN","bR"],
            ["bP","bP","bP","--","--","bP","bP","bP"],
            ["--","--","--","bB","bP","--","bB","--"],
            ["--","--","--","--","wN","--","--","--"],
            ["--","--","--","--","--","--","--","--"],
            ["--","--","--","--","wP","--","wN","--"],
            ["wP","wP","wP","wP","--","--","wP","wP"],
            ["wR","--","wB","wQ","wK","wB","--","wR"]]
       
        '''
class button():
    def __init__(self,colour,x,y,width,height,text,screen):
        self.colour = colour
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.text = text
        self.draw(screen)
    def draw(self,screen):
        p.draw.rect(screen,self.colour, (self.x,self.y,self.width,self.height),0)
        font = p.font.SysFont('comicsans',27)
        text = font.render(self.text,1,"black")
        screen.blit(text, (self.x + (self.width/2 - text.get_width()/2), self.y + (self.height/2 - text.get_height()/2)))
    def isOver(self, pos):
        if pos[0] > self.x and pos[0] < self.x + self.width:
            if pos[1] > self.y and pos[1] < self.y + self.height:
                return True
            
        return False

def main():
    games = 0
    whiteWins = 0
    blackWins = 0
    global HUMANCOLOR
    global AICOLOR
    global TURNCOLOR
    global prevHighlight
    global whiteEnPaisant
    global blackEnPaisant
    random.seed(a=None)
    score = 0
    p.init()
    loadImages()
    clock = p.time.Clock()
    screen = p.display.set_mode((WIDTH,HEIGHT))
    gs = GameState()
    screen.fill(p.Color("white"))
    AISetup(screen)
    sharedFEN = boardToFEN(gs.board,[])
    if(HUMANCOLOR == 8):
        gs.board = reverseBoard(gs.board)
    running = True
    moves = 0
    squareSelected = ()
    prevClicks = []
    drawBoardState(screen,gs.board,squareSelected,[-1,-1],[])
    clock.tick(MAX_FPS)
    p.display.flip()
    prevMove = [-1,-1]
    possibleMoves = []
    while running:
        for e in p.event.get():
            if e.type == p.QUIT:
                running = False
            
            elif HUMANCOLOR == TURNCOLOR and e.type == p.MOUSEBUTTONDOWN:
                location = p.mouse.get_pos()
                pieceCol = location[1]//64
                pieceRow = location[0]//64
                if squareSelected == (pieceCol,pieceRow) or (len(prevClicks) == 1 and (pieceCol,pieceRow) not in possibleMoves): #undo click
                    squareSelected = ()
                    prevClicks = []
                elif len(prevClicks) == 0 and ((HUMANCOLOR == 8 and gs.board[pieceCol][pieceRow][0] != 'b') or (HUMANCOLOR == 16 and gs.board[pieceCol][pieceRow][0] != 'w')):
                    squareSelected = ()
                    prevClicks = []
                else:
                    squareSelected = (pieceCol,pieceRow)
                    prevClicks.append(squareSelected)
                    print(pieceCol,pieceRow)
                    highlightSquare(squareSelected,screen)
                if len(prevClicks) == 1:
                    possibleMoves = getPossibleMoves(gs.board,squareSelected[0],squareSelected[1])
                elif len(prevClicks) == 2:
                    movePiece(prevClicks,screen,gs.board)
                    prevMove = prevClicks
                    if HUMANCOLOR == 8:
                        reverseBoard(gs.board)
                        prevClicks = reverseMove(prevClicks)
                        print(str(prevClicks[0][0]) + " " + str(prevClicks[0][1]))
                    castledMove(prevClicks,screen,gs.board)
                    updateProperties(prevClicks)

                    squareSelected = ()
                    if(TURNCOLOR == 16):
                        TURNCOLOR = 8
                    else:
                        TURNCOLOR = 16
                    sharedFEN = boardToFEN(gs.board,prevClicks)
                    prevClicks = []
                    if HUMANCOLOR == 8:
                        reverseBoard(gs.board)
                    print (sharedFEN)
            
            elif TURNCOLOR == AICOLOR:
                FEN_as_bytes = str.encode(sharedFEN)
                FENBuffer = ctypes.create_string_buffer(FEN_as_bytes)
                AIMove = str(New_Function(FENBuffer))
                print("back AIMove: " + AIMove)
                if(AIMove == "-1"):
                    break
                squareSelected = (int(AIMove[0]),int(AIMove[1]))
                if AIMove[0] == '9':
                    squareSelected = (0,int(AIMove[1]))
                prevClicks.append(squareSelected)
                squareSelected = (int(AIMove[2]),int(AIMove[3]))
                if(AIMove[4] == '0' and len(AIMove) > 5):
                    score = int(AIMove[5:])
                    score = -score
                else:
                    score = int(AIMove[4:])
                print("back score: " + str(score))
                prevClicks.append(squareSelected)
                if HUMANCOLOR == 8:
                    #reverseBoard(gs.board)
                    prevClicks = reverseMove(prevClicks)
                if gs.board[prevClicks[0][0]][prevClicks[0][1]][1] == 'P' and abs(prevClicks[0][0] - prevClicks[1][0]) > 1:
                    if AICOLOR == 16:
                        blackEnPaisant = prevClicks[0][1]
                        whiteEnPaisant = 9
                    else:
                        blackEnPaisant = 9
                        whiteEnPaisant = prevClicks[0][1]
                else:
                    whitEnPaisant = 9
                    blackEnPaisant = 9
                movePiece(prevClicks,screen,gs.board)
                prevMove = prevClicks
                if HUMANCOLOR == 8:
                    reverseBoard(gs.board)
                    prevClicks = reverseMove(prevClicks)

                castledMove(prevClicks,screen,gs.board)
                updateProperties(prevClicks)

                print(prevClicks)
                if(TURNCOLOR == 16):
                    TURNCOLOR = 8
                else:
                    TURNCOLOR = 16

                sharedFEN = boardToFEN(gs.board,prevClicks)
                if HUMANCOLOR == 8:
                    reverseBoard(gs.board)
                prevClicks = []
                squareSelected = ()
            if len(prevClicks) != 1:
                possibleMoves = []
            drawBoardState(screen,gs.board,squareSelected,prevMove,possibleMoves)
            clock.tick(MAX_FPS)
            p.display.flip()
            

def inCheck(board):
        if HUMANCOLOR == 16:
            for i in range(8):
                for j in range(8):
                    if board[i][j] == 'wK':
                        kingPos = (i,j)
                        break
        else:
            for i in range(8):
                for j in range(8):
                    if board[i][j] == 'bK':
                        kingPos = (i,j)
                        break
        if rookCheck(board,kingPos[0],kingPos[1]) or bishopCheck(board,kingPos[0],kingPos[1]) or knightCheck(board,kingPos[0],kingPos[1]) or pawnCheck(board,kingPos[0],kingPos[1]) or kingCheck(board,kingPos[0],kingPos[1]):
            return True

        return False

    
            

def moveResultsInCheck(board,colPos,rowPos,colMove,rowMove):
    if board[colPos][rowPos][1] == 'K' and abs(rowPos - rowMove) > 1:
        if inCheck(board):
            return True
        temp = board[colPos][rowPos]
        if HUMANCOLOR == 16 and rowMove == 6:
            board[7][4] = '--'
            board[7][5] = temp
            if inCheck(board):
                board[7][5] = '--'
                board[7][4] = temp
                return True
            board[7][5] = '--'
            board[7][4] = temp
        elif HUMANCOLOR == 16 and rowMove == 2:
            board[7][4] = '--'
            board[7][3] = temp
            if inCheck(board):
                board[7][3] = '--'
                board[7][4] = temp
                return True
            board[7][3] = '--'
            board[7][4] = temp
        elif HUMANCOLOR == 8 and rowMove == 1:
            board[7][3] = '--'
            board[7][2] = temp
            if inCheck(board):
                board[7][2] = '--'
                board[7][3] = temp
                return True
            board[7][2] = '--'
            board[7][3] = temp
        elif HUMANCOLOR == 8 and rowMove == 5:
            board[7][3] = '--'
            board[7][4] = temp
            if inCheck(board):
                board[7][4] = '--'
                board[7][3] = temp
                return True
            board[7][4] = '--'
            board[7][3] = temp
        
    
    move = ((colPos,rowPos),(colMove,rowMove))
    temp = board[move[0][0]][move[0][1]]
    oppTemp = board[move[1][0]][move[1][1]]
    
    board[move[0][0]][move[0][1]] = '--'
    board[move[1][0]][move[1][1]] = temp

    if inCheck(board):
        board[move[0][0]][move[0][1]] = temp
        board[move[1][0]][move[1][1]] = oppTemp
        return True
    board[move[0][0]][move[0][1]] = temp
    board[move[1][0]][move[1][1]] = oppTemp
    return False

def getPossibleMoves(board, col, row):
    piece = board[col][row]
    result = []
    if piece[1] == 'P':
        moves = pawnMoves(board,col,row)
        for move in moves:
            if not moveResultsInCheck(board,col,row,move[0],move[1]):
                result.append(move)
        return result
    if piece[1] == 'N':
        moves = knightMoves(board,col,row)
        for move in moves:
            if not moveResultsInCheck(board,col,row,move[0],move[1]):
                result.append(move)
        return result
    if piece[1] == 'B':
        moves = bishopMoves(board,col,row)
        for move in moves:
            if not moveResultsInCheck(board,col,row,move[0],move[1]):
                result.append(move)
        return result
    if piece[1] == 'R':
        moves = rookMoves(board,col,row)
        for move in moves:
            if not moveResultsInCheck(board,col,row,move[0],move[1]):
                result.append(move)
        return result
    if piece[1] == 'Q':
        moves = bishopMoves(board,col,row) + rookMoves(board,col,row)
        for move in moves:
            if not moveResultsInCheck(board,col,row,move[0],move[1]):
                result.append(move)
        return result

    moves = kingMoves(board,col,row)
    for move in moves:
        if not moveResultsInCheck(board,col,row,move[0],move[1]):
            result.append(move)
    return result

def pawnMoves(board,col,row):
    global whiteEnPaisant
    global blackEnPaisant
    if HUMANCOLOR == 8:
        opposite = 'w'
    else:
        opposite = 'b'
    result = []
    if board[col-1][row] == '--':
        result.append((col-1,row))
        if col == 6 and board[col-2][row] == '--':
            result.append((col-2,row))
    if row > 0 and board[col-1][row-1][0] == opposite:
        result.append((col-1,row-1))
    if row < 7 and board[col-1][row+1][0] == opposite:
        result.append((col-1,row+1))
    print(col,whiteEnPaisant,blackEnPaisant)
    if HUMANCOLOR == 16 and col == 3 and abs(whiteEnPaisant-row) == 1:
        if row > whiteEnPaisant:
            result.append((col-1,row-1))
        else:
            result.append((col-1,row+1))
    elif HUMANCOLOR == 8 and col == 3 and abs(blackEnPaisant-row) == 1:
        if row > blackEnPaisant:
            result.append((col-1,row-1))
        else:
            result.append((col-1,row+1))
    
    return result

def pawnCheck(board,col,row):
    if HUMANCOLOR == 8:
        opp = 'w'
    else:
        opp = 'b'
    if row > 0 and board[col-1][row-1] == opp + 'P':
        return True
    if row < 7 and board[col-1][row+1] == opp + 'P':
        return True
    return False


def knightMoves(board,col,row):
    result = []
    if HUMANCOLOR == 8:
        same = 'b'
    else:
        same = 'w'
    if col > 1 and row < 7 and board[col-2][row+1][0] != same:
        result.append((col-2,row+1))
    if col > 0 and row < 6 and board[col-1][row+2][0] != same:
        result.append((col-1,row+2))
    if col < 6 and row < 7 and board[col+2][row+1][0] != same:
        result.append((col+2,row+1))
    if col < 7 and row < 6 and board[col+1][row+2][0] != same:
        result.append((col+1,row+2))
    
    if col > 1 and row > 0 and board[col-2][row-1][0] != same:
        result.append((col-2,row-1))
    if col > 0 and row > 1 and board[col-1][row-2][0] != same:
        result.append((col-1,row-2))
    if col < 6 and row >0 and board[col+2][row-1][0] != same:
        result.append((col+2,row-1))
    if col < 7 and row > 1 and board[col+1][row-2][0] != same:
        result.append((col+1,row-2))
    return result

def knightCheck(board,col,row):
    if board[col][row] == 'wK':
        opp = 'b'
    else:
        opp ='w'
    if col > 1 and row < 7 and board[col-2][row+1] == opp + 'N':
        return True
    if col > 0 and row < 6 and board[col-1][row+2]== opp + 'N':
        return True
    if col < 6 and row < 7 and board[col+2][row+1] == opp + 'N':
        return True
    if col < 7 and row < 6 and board[col+1][row+2] == opp + 'N':
        return True
    
    if col > 1 and row > 0 and board[col-2][row-1] == opp + 'N':
        return True
    if col > 0 and row > 1 and board[col-1][row-2] == opp + 'N':
        return True
    if col < 6 and row >0 and board[col+2][row-1] == opp + 'N':
        return True
    if col < 7 and row > 1 and board[col+1][row-2] == opp + 'N':
        return True
    return False
    

def bishopMoves(board,col,row):
    if HUMANCOLOR == 8:
        opp = 'w'
    else:
        opp = 'b'
    currentCol = col
    currentRow = row
    result = []
    while col > 0 and row < 7 and board[col-1][row+1][0] == '-':
        result.append((col-1,row+1))
        col -= 1
        row += 1
    if col > 0 and row < 7 and board[col-1][row+1][0] == opp:
        result.append((col-1,row+1))
    col = currentCol
    row = currentRow
    while col < 7 and row < 7 and board[col+1][row+1][0] == '-':
        result.append((col+1,row+1))
        col += 1
        row += 1
    if col < 7 and row < 7 and board[col+1][row+1][0] == opp:
        result.append((col+1,row+1))
    col = currentCol
    row = currentRow
    while col < 7 and row > 0 and board[col+1][row-1][0] == '-':
        result.append((col+1,row-1))
        col += 1
        row -= 1
    if col < 7 and row > 0 and board[col+1][row-1][0] == opp:
        result.append((col+1,row-1))
    col = currentCol
    row = currentRow
    while col > 0 and row > 0 and board[col-1][row-1][0] == '-':
        result.append((col-1,row-1))
        col -= 1
        row -= 1
    if col > 0 and row > 0 and board[col-1][row-1][0] == opp:
        result.append((col-1,row-1))
    return result

def bishopCheck(board,col,row):
    if HUMANCOLOR == 8:
        opp = 'w'
    else:
        opp = 'b'
    currentCol = col
    currentRow = row
    while col > 0 and row < 7 and board[col-1][row+1][0] == '-':
        col -= 1
        row += 1
    if col > 0 and row < 7 and board[col-1][row+1] == opp + 'B':
        return True
    if col > 0 and row < 7 and board[col-1][row+1] == opp + 'Q':
        return True
    col = currentCol
    row = currentRow
    while col < 7 and row < 7 and board[col+1][row+1][0] == '-':
        col += 1
        row += 1
    if col < 7 and row < 7 and board[col+1][row+1] == opp + 'B':
        return True
    if col < 7 and row < 7 and board[col+1][row+1] == opp + 'Q':
        return True
    col = currentCol
    row = currentRow
    while col < 7 and row > 0 and board[col+1][row-1][0] == '-':
        col += 1
        row -= 1
    if col < 7 and row > 0 and board[col+1][row-1] == opp + 'B':
        return True
    if col < 7 and row > 0 and board[col+1][row-1] == opp + 'Q':
        return True
    col = currentCol
    row = currentRow
    while col > 0 and row > 0 and board[col-1][row-1][0] == '-':
        col -= 1
        row -= 1
    if col > 0 and row > 0 and board[col-1][row-1] == opp + 'B':
        return True
    if col > 0 and row > 0 and board[col-1][row-1] == opp + 'Q':
        return True
    return False
    
def rookMoves(board,col,row):
    if HUMANCOLOR == 8:
        opp = 'w'
    else:
        opp = 'b'
    currentCol = col
    currentRow = row
    result = []
    while col > 0 and board[col-1][row][0] == '-':
        result.append((col-1,row))
        col -= 1
    if col > 0 and board[col-1][row][0] == opp:
        result.append((col-1,row))
    col = currentCol
    row = currentRow
    while col < 7 and board[col+1][row][0] == '-':
        result.append((col+1,row))
        col += 1
    if col < 7 and board[col+1][row][0] == opp:
        result.append((col+1,row))
    col = currentCol
    row = currentRow
    while row > 0 and board[col][row-1][0] == '-':
        result.append((col,row-1))
        row -= 1
    if row > 0 and board[col][row-1][0] == opp:
        result.append((col,row-1))
    col = currentCol
    row = currentRow
    while row < 7 and board[col][row+1][0] == '-':
        result.append((col,row+1))
        row += 1
    if row < 7 and board[col][row+1][0] == opp:
        result.append((col,row+1))
    return result

def rookCheck(board,col,row):
    if HUMANCOLOR == 8:
        opp = 'w'
    else:
        opp = 'b'
    currentCol = col
    currentRow = row
    while col > 0 and board[col-1][row][0] == '-':
        col -= 1
    if col > 0 and board[col-1][row] == opp + 'R':
        return True
    if col > 0 and board[col-1][row] == opp + 'Q':
        return True
    col = currentCol
    row = currentRow
    while col < 7 and board[col+1][row][0] == '-':
        col += 1
    if col < 7 and board[col+1][row] == opp + 'R':
        return True
    if col < 7 and board[col+1][row] == opp + 'Q':
        return True
    col = currentCol
    row = currentRow
    while row > 0 and board[col][row-1][0] == '-':
        row -= 1
    if row > 0 and board[col][row-1] == opp + 'R':
        return True
    if row > 0 and board[col][row-1] == opp + 'Q':
        return True
    col = currentCol
    row = currentRow
    while row < 7 and board[col][row+1][0] == '-':
        row += 1
    if row < 7 and board[col][row+1] == opp + 'R':
        return True
    if row < 7 and board[col][row+1] == opp + 'Q':
        return True
    return False

def kingMoves(board,col,row):
    result = []
    if HUMANCOLOR == 8:
        kingMoved = blackKingMoved
        kingRookMoved = blackKingRookMoved
        queenRookMoved = blackQueenRookMoved
        if not kingMoved and  not kingRookMoved and board[7][2][0] == '-' and board[7][1][0] == '-':
            result.append((7,1))
        if not kingMoved and not queenRookMoved and board[7][4][0] == '-' and board[7][5][0] == '-' and board[7][6][0] == '-':
            result.append((7,5))
        
        same = 'b'
    else:
        kingMoved = whiteKingMoved
        kingRookMoved = whiteKingRookMoved
        queenRookMoved = whiteQueenRookMoved
        if not kingMoved and  not kingRookMoved and board[7][5][0] == '-' and board[7][6][0] == '-':
            result.append((7,6))
        if not kingMoved and not queenRookMoved and board[7][3][0] == '-' and board[7][2][0] == '-' and board[7][1][0] == '-':
            result.append((7,2))
        same = 'w'
    currentCol = col
    currentRow = row
    if col > 0 and board[col-1][row][0] != same:
        result.append((col-1,row))
    col = currentCol
    row = currentRow
    if col < 7 and board[col+1][row][0] != same:
        result.append((col+1,row))
    col = currentCol
    row = currentRow
    if row > 0 and board[col][row-1][0] != same:
        result.append((col,row-1))
    col = currentCol
    row = currentRow
    if row < 7 and board[col][row+1][0] != same:
        result.append((col,row+1))
    if col > 0 and row < 7 and board[col-1][row+1][0] != same:
        result.append((col-1,row+1))
    col = currentCol
    row = currentRow
    if col < 7 and row < 7 and board[col+1][row+1][0] != same:
        result.append((col+1,row+1))
    col = currentCol
    row = currentRow
    if col < 7 and row > 0 and board[col+1][row-1][0] != same:
        result.append((col+1,row-1))
    col = currentCol
    row = currentRow
    if col > 0 and row > 0 and board[col-1][row-1][0] != same:
        result.append((col-1,row-1))
    return result

def kingCheck(board,col,row):
    if HUMANCOLOR == 8:
        opp = 'w'
    else:
        opp = 'b'

    if col > 0 and board[col-1][row] == opp + 'K':
        return True

    if col < 7 and board[col+1][row] == opp + 'K':
        return True

    if row > 0 and board[col][row-1] == opp + 'K':
        return True

    if row < 7 and board[col][row+1] == opp + 'K':
        return True
    if col > 0 and row < 7 and board[col-1][row+1] == opp + 'K':
        return True
 
    if col < 7 and row < 7 and board[col+1][row+1] == opp + 'K':
        return True

    if col < 7 and row > 0 and board[col+1][row-1] == opp + 'K':
        return True

    if col > 0 and row > 0 and board[col-1][row-1] == opp + 'K':
        return True
    return False


def highlightSquare(square,screen):
    if square == -1:
        return
    surface = p.Surface((WIDTH,HEIGHT),p.SRCALPHA)
    p.draw.rect(surface, (255,255,0,210),p.Rect(square[1]*64,square[0]*64,64,64))
    screen.blit(surface,(0,0))
    

def updateProperties(move):
    global whiteKingMoved
    global blackKingMoved

    global whiteKingRookMoved
    global whiteQueenRookMoved
    global blackKingRookMoved
    global blackQueenRookMoved

    if(not whiteKingMoved):
        if(move[0][0] == 7 and move[0][1] == 4):
            whiteKingMoved = True
        if(move[0][0] == 7 and move[0][1] == 0):
            whiteQueenRookMoved = True
        if(move[0][0] == 7 and move[0][1] == 7):
            whiteKingRookMoved = True
    
    if(not blackKingMoved):
        if(move[0][0] == 0 and move[0][1] == 4):
            blackKingMoved = True
        if(move[0][0] == 0 and move[0][1] == 0):
            blackQueenRookMoved = True
        if(move[0][0] == 0 and move[0][1] == 7):
            blackKingRookMoved = True
    

def loadImages():
    pieces = ['wP','wR','wN','wB','wK','wQ','bP','bR','bN','bB','bK','bQ']
    for piece in pieces:
        IMAGES[piece] = p.transform.scale(p.image.load("images/" + piece + ".png"), (64,64))
def movePiece(move,screen,board):
    global HUMANCOLOR
    global AICOLOR
    global TURNCOLOR
    temp = board[move[0][0]][move[0][1]]
    #EP check
    if temp[1] == 'P' and HUMANCOLOR == TURNCOLOR:
        if move[0][1] != move[1][1] and board[move[1][0]][move[1][1]] == '--':
            board[move[1][0]+1][move[1][1]] = '--'
    elif temp[1] == 'P' and AICOLOR == TURNCOLOR:
        if move[0][1] != move[1][1] and board[move[1][0]][move[1][1]] == '--':
            board[move[1][0]-1][move[1][1]] = '--'
    board[move[0][0]][move[0][1]] = '--'
    board[move[1][0]][move[1][1]] = temp
    #EP check

    print(move[0][0],move[0][1])
    if temp[1] == 'P' and (move[1][0] == 0 or move[1][0] == 7):
        board[move[1][0]][move[1][1]] = temp[0] + 'Q'
    drawPieces(screen,board)

def castledMove(move,screen,board):
    #AICOLOR = 16
    #move 
    temp = board[move[1][0]][move[1][1]]
    if(temp[1] == 'K' and (abs(move[0][1] - move[1][1]) == 2)):
        #king side
        if(move[0][1] < move[1][1]):
            #white
            if(move[0][0] == 7):
                board[7][7] = '--'
                board[7][5] = 'wR'
            else:
                board[0][7] = '--'
                board[0][5] = 'bR'
        else:
            if(move[0][0] == 7):
                board[7][0] = '--'
                board[7][3] = 'wR'
            else:
                board[0][0] = '--'
                board[0][3] = 'bR'
def reverseMove(move):
    #1 4 -> 6 4
    global blackFlip
    move.append((blackFlip[move[0][0]],blackFlip[move[0][1]]))
    move.append((blackFlip[move[1][0]],blackFlip[move[1][1]]))
    move = move[2:]

    return move
    

def reverseBoard(board):
    global blackFlip
    for i in range(4):
        for j in range(8):
            temp = board[i][j]
            board[i][j] = board[blackFlip[i]][blackFlip[j]]
            board[blackFlip[i]][blackFlip[j]] = temp
    return board

def boardPrinter(board):
    tempStr = ""
    for i in range(8):
        for j in range(8):
            tempStr += " " + board[i][j] + " "
        print(tempStr)
        tempStr = ""
def drawPossibleMoves(moves,screen,board):
    surface = p.Surface((WIDTH,HEIGHT),p.SRCALPHA)
    for x,y in moves:
        if board[x][y] == '--':
            p.draw.circle(surface, (169,169,169,100),(32+64*y,32+64*x),10)
            screen.blit(surface,(0,0))
        else:
            p.draw.circle(surface, (169,169,169,150),(32+64*y,32+64*x),32)
            p.draw.circle(surface, (169,169,169,0),(32+64*y,32+64*x),27)
            screen.blit(surface,(0,0))
            
def drawBoardState(screen,board,squareSelected,prevMove,possibleMoves):
    drawBoard(screen,squareSelected)
    highlightSquare(prevMove[0],screen)
    highlightSquare(prevMove[1],screen)
    drawPieces(screen,board)
    drawPossibleMoves(possibleMoves,screen,board)
        
def drawBoard(screen, squareSelected):
    colours = [p.Color("white"), p.Color("dark green")]
    for i in range(8):
        for j in range(8):
            colour = colours[(i+j)%2]
            p.draw.rect(screen, colour, p.Rect(i*64,j*64,64,64))
    if squareSelected != ():
        surface = p.Surface((WIDTH,HEIGHT),p.SRCALPHA)
        p.draw.rect(surface, (255,255,0,210), p.Rect(squareSelected[1]*64,squareSelected[0]*64,64,64))
        screen.blit(surface,(0,0))

def drawPieces(screen,board):
    for i in range(8):
        for j in range(8):
            piece = board[i][j]
            if (piece != '--'):
                screen.blit(IMAGES[piece],p.Rect(j*64,i*64,64,64))
def cRange(num):
    global HUMANCOLOR
    if(HUMANCOLOR == 16):
        return range(num)
    else:
        return reversed(range(num))
                    
def boardToFEN(board,move):
    global whiteKingMoved
    global blackKingMoved

    global whiteKingRookMoved
    global whiteQueenRookMoved
    global blackKingRookMoved
    global blackQueenRookMoved

    global TURNCOLOR

    global AIDIFFICULTY
    FEN = ""
    count = 0
    newChar = ''
    for i in range(8):
        for j in range(8):
            if board[i][j] == 'bR':
                newChar = 'r'
            elif board[i][j] == 'bN':
                newChar =  'n'
            elif board[i][j] == 'bB':
                newChar =  'b'
            elif board[i][j] == 'bQ':
                newChar =  'q'
            elif board[i][j] == 'bK':
                newChar =  'k'
            elif board[i][j] == 'bP':
                newChar =  'p'
            elif board[i][j] == 'wR':
                newChar =  'R'
            elif board[i][j] == 'wN':
                newChar =  'N'
            elif board[i][j] == 'wB':
                newChar =  'B'
            elif board[i][j] == 'wQ':
                newChar =  'Q'
            elif board[i][j] == 'wK':
                newChar =  'K'
            elif board[i][j] == 'wP':
                newChar =  'P'
            if newChar == '':
                count += 1
            elif count == 0:
                FEN += newChar
            else:
                FEN += str(count) + newChar
                count = 0
            newChar = ''
        if count != 0:
            FEN += str(count)
            count = 0
        FEN += '/'
    FEN = FEN[:-1]
    #rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    '''
    if(HUMANCOLOR == 16):
        FEN += " b "
    else:
        FEN += " w "
    '''
    if TURNCOLOR == 16:
        FEN += " w "
    else:
        FEN += " b "
    FEN += "-"
    if(not whiteKingMoved):
        if(not whiteKingRookMoved):
            FEN += "K"
        if(not whiteQueenRookMoved):
            FEN += "Q"
    if(not blackKingMoved):
        if(not blackKingRookMoved):
            FEN += "k"
        if(not blackQueenRookMoved):
            FEN += "q"
    if(len(move) == 2):
        if((board[move[1][0]][move[1][1]][1] == 'P') and (abs(move[0][0] - move[1][0]) == 2)):
            FEN += " " + str(move[0][1])
        else:
            FEN += " " + "-"
    else:
        FEN += " " + "-"
    if AIDIFFICULTY == 1:
        FEN += " 1"
    elif AIDIFFICULTY == 3:
        FEN += " 3"
    else:
        FEN += " 10"
    if(len(move) == 2):
        FEN += " " + str(move[0][0]) + str(move[0][1]) +str(move[1][0]) + str(move[1][1])
    return FEN

            
def AISetup(screen):
    global AICOLOR
    global AIDIFFICULTY
    global HUMANCOLOR
    diffButton = button("white",100,0,300,60,'Select a Difficulty',screen)
    easyButton = button("green",75,80,100,60,'Easy',screen)
    mediumButton = button("yellow",200,80,100,60,'Medium',screen)
    hardButton = button("red",325,80,100,60,'Hard',screen)
    colorButton = button("white",100,200,300,60,'Select a Color',screen)
    whiteButton = button("light grey",75,280,100,60,'White',screen)
    blackButton = button("dark grey",200,280,100,60,'Black',screen)
    randomButton = button("orange",325,280,100,60,'Random',screen)
    while(AIDIFFICULTY == None or AICOLOR == None):
        p.display.update()
        for event in p.event.get():
            pos = p.mouse.get_pos()
            if(event.type == p.MOUSEBUTTONDOWN):
               if (easyButton.isOver(pos)):
                   AIDIFFICULTY = 1
               elif (mediumButton.isOver(pos)):
                   AIDIFFICULTY = 3
               elif (hardButton.isOver(pos)):
                   AIDIFFICULTY = 10
               elif (whiteButton.isOver(pos)):
                   AICOLOR = 8
                   HUMANCOLOR = 16
               elif (blackButton.isOver(pos)):
                   AICOLOR = 16
                   HUMANCOLOR = 8
               elif (randomButton.isOver(pos)):
                   AICOLOR = random.randint(0,1)
                   AICOLOR = AICOLOR * 8 + 8
                   if AICOLOR == 16:
                       HUMANCOLOR = 8
                   else:
                        HUMANCOLOR = 16
                
                   
if __name__ == "__main__":
    main()
