/*
 Shengcong Zhou, Benjamin Martin, Eduard Durech
 Date created: 17 Nov 2017
 Last modified: 30 Nov 2017
 This program will play a Cylindrical Mule Checkers, with a user-inputted even number board size from 8x8 to 18x18
 The game ends when (the player with which the argument is true loses in these cases) the player has no possible moves, the player has only mules,
 the opponent has no more mules, or the player's mule becomes a king
 */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

//Global constants
const int MAX_ARRAY_SIZE = 18;
const int MIN_ARRAY_SIZE = 8;
const int MAX_PIECES = 72;
const int NOCHECKER = 0;
const int WHITESOLDIER = 1;
const int WHITEMULE = 2;
const int WHITEKING = 3;
const int REDSOLDIER = 4;
const int REDMULE = 5;
const int REDKING = 6;
const int WHITEPLAYER = 1;
const int REDPLAYER = 2;
const int ODD_OR_EVEN = 2;
const int EVEN = 0;
const int ODD = 1;
const int NOJUMP = 0;

//Function prototypes
void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);
void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);
int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);
bool CheckList(int inArray1[], int inArray2[], int xIndex, int yindex);
int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[],  int yLocArray[]);
bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc);
bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc);
bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int fromSquareNum, int toSquareNum, bool &jumped );
bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

//Functions
int main()
{
    //Constants
    const int MAXTRIAL = 3;
    const int MIN_SQUARE_ON_BOARD = 0;
    
    //Tries
    int trial = 0;
    
    //Rows
    int numRowsInBoard = 0;
    
    //Checkers in this array can be moved
    int xIndicesMove[MAX_PIECES] = {0};
    int yIndicesMove[MAX_PIECES] = {0};
    
    //Checkers in this array can jump
    int xIndicesJump[MAX_PIECES] = {0};
    int yIndicesJump[MAX_PIECES] = {0};
    
    //Board
    int myCMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = {0};
    int player = 0;
    int checkersCanJump = 0;
    int checkersCanMove = 0;
    bool jumped = false;
    
    //Moving chckers
    int fromSquareNum = 0;
    int toSquareNum = 0;
    int maxSquareOnBoard = 0;
    
    //Location
    int i = 0;
    int j = 0;
    int y = 0;
    
    //other
    string endGame = "";
    string colour = "";
    string othercolour = "";
    bool error = true;
    int turn = 1;
    
    player = WHITEPLAYER; //White player takes the 1st turn
    
    do //Prompting user for chosen value of board size
    {
        if(trial == MAXTRIAL) //Limiting tries
        {
            cerr << "ERROR: Too many errors entering the size of the board";
            return 1;
        }
        
        cout << "Enter the number of squares along each edge of the board" << endl;
        
        if(!(cin >> numRowsInBoard)) //Board size must be an integer
        {
            cerr << "ERROR: Board size is not an integer" << endl << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << endl;
            cin.clear();
            cin.sync();
        }
        else if((numRowsInBoard%ODD_OR_EVEN) == ODD) //Board size must be even
        {
            cerr << "ERROR: Board size odd" << endl << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << endl;
        }
        else if(numRowsInBoard > MAX_ARRAY_SIZE) //Limiting board size
        {
            cerr << "ERROR: Board size too large" << endl << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << endl;
        }
        else if(numRowsInBoard < MIN_ARRAY_SIZE)
        {
            cerr << "ERROR: Board size too small" << endl << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << endl;
        }
        
        trial++;
        
    }while(numRowsInBoard < MIN_ARRAY_SIZE || numRowsInBoard > MAX_ARRAY_SIZE || (numRowsInBoard%2) == 1);
    
    InitializeBoard(myCMCheckersBoard, numRowsInBoard); //Initializing board
    
    do //Playing the game
    {
        if(turn%ODD_OR_EVEN == ODD) //Checking whose turn
        {
            player = WHITEPLAYER;
        }
        else if(turn%ODD_OR_EVEN == EVEN)
        {
            player = REDPLAYER;
        }
        
        DisplayBoard(myCMCheckersBoard, numRowsInBoard); //Showing the board
        cout << endl << endl << endl;
        
        //Check if players have possible jumps or moves
        checkersCanJump = CountJumps(myCMCheckersBoard, numRowsInBoard, player, xIndicesJump, yIndicesJump);
        checkersCanMove = CountMove1Squares(myCMCheckersBoard, numRowsInBoard, player, xIndicesMove, yIndicesMove);
        
        if(checkersCanJump > NOCHECKER || checkersCanMove > NOCHECKER)
        {
            if(player == WHITEPLAYER)
            {
                cout << "White takes a turn\n";
                
            }
            else
            {
                cout << "Red takes a turn\n";
            }
        }
        else
        {
            if(player == WHITEPLAYER)
            {
                colour = "White";
                othercolour = "Red";
            }
            else if(player == REDPLAYER)
            {
                colour = "Red";
                othercolour = "White";
            }
            
            cerr << colour << " is unable to move\nGAME OVER, " << othercolour << " has won\nEnter any character to close the game\n";
            cin >> endGame;
            return 0;
        }
        
        //Prompt for what checker the player wants to move
        maxSquareOnBoard = numRowsInBoard * numRowsInBoard - 1;
        error = true;
        do
        {
            cout << "Enter square number of the checker you want to move\n";
            
            if(!(cin >> fromSquareNum))
            {
                cerr << "ERROR: you did not enter an integer\nTry again\n";
                cin.clear();
                cin.sync();
                continue;
            }
            
            if(fromSquareNum < MIN_SQUARE_ON_BOARD || fromSquareNum > maxSquareOnBoard)
            {
                cerr << "ERROR: that square is not on the board.\nTry again\n";
                continue;
            }
            
            i = fromSquareNum / numRowsInBoard;
            j = fromSquareNum % numRowsInBoard;
            if(myCMCheckersBoard[i][j] != NOCHECKER) //Error checking for valid checker choice given rules of checkers
            {
                if(player == WHITEPLAYER)
                {
                    if(myCMCheckersBoard[i][j] == REDMULE || myCMCheckersBoard[i][j] == REDSOLDIER || myCMCheckersBoard[i][j] == REDKING)
                    {
                        cerr << "ERROR: that square contains an opponent's checker\nTry again\n";
                        continue;
                    }
                }
                else if(player == REDPLAYER)
                {
                    if(myCMCheckersBoard[i][j] == WHITEMULE || myCMCheckersBoard[i][j] == WHITESOLDIER || myCMCheckersBoard[i][j] == WHITEKING)
                    {
                        cerr << "ERROR: that square contains an opponent's checker\nTry again\n";
                        continue;
                    }
                }
                else
                {
                    cerr << "ERROR: something wrong to the checker you chose\nEnter any character to close the game\n";
                    cin >> endGame;
                    return 0;
                }
            }
            else
            {
                cerr << "ERROR: that square is empty\nTry again\n";
                continue;
            }
            
            if((CheckList(xIndicesJump, yIndicesJump, j, i)) == false && checkersCanJump > NOJUMP )
            {
                cerr << "ERROR: You can jump with another checker, you may not move your chosen checker\nYou can jump with checkers on the following squares: ";
                for (y=0; y<checkersCanJump; y++)
                {
                    cout << yIndicesJump[y] * numRowsInBoard + xIndicesJump[y] << " ";
                }
                cerr << "\nTry again\n";
                continue;
            }
            else if((CheckList(xIndicesMove, yIndicesMove, j, i) ) == false && checkersCanJump == NOJUMP )
            {
                cerr << "ERROR: There is no legal move for this checker\nTry again\n";
                continue;
            }
            
            error = false;
            
        }while(error);
        
        //Prompt for where the player wants to move the chosen checker
        error = true;
        do
        {
            cout << "Enter square number of the square you want to move your checker to\n";
            
            if(!(cin >> toSquareNum))
            {
                cerr << "ERROR: you did not enter an integer\nTry again\n";
                cin.clear();
                cin.sync();
                continue;
            }
            
            if(toSquareNum < MIN_SQUARE_ON_BOARD || toSquareNum > maxSquareOnBoard)
            {
                cerr << "ERROR: It is not possible to move to a square that is not on the board.\nTry again\n";
                continue;
            }
            
            i = toSquareNum / numRowsInBoard;
            j = toSquareNum % numRowsInBoard;
            
            if(myCMCheckersBoard[i][j] != NOCHECKER)
            {
                cerr << "ERROR: It is not possible to move to a square that is already occupied" << endl << "Try again" << endl;
                continue;
            }
            
            if(MakeMove(myCMCheckersBoard, numRowsInBoard, player, fromSquareNum, toSquareNum, jumped) == false)
            {
                cerr << "ERROR: moving to that square is not legal, Try again\n";
                continue;
            }
            
            error = false;
            
        }while(error);
        
        //Check if still jumps available
        while(jumped)
        {
            if((IsJump(myCMCheckersBoard, numRowsInBoard, player, j, i)) == false )
            {
                jumped = false;
                break;
            }
            
            fromSquareNum = toSquareNum;
            error = true;
            
            do
            {
                cout << "You can jump again,  Please enter the next square you wish to move your checker to\n";
                
                if(!(cin >> toSquareNum))
                {
                    cerr << "ERROR: you did not enter an integer\nTry again\n";
                    cin.clear();
                    cin.sync();
                    continue;
                }
                
                if(toSquareNum < MIN_SQUARE_ON_BOARD || toSquareNum > maxSquareOnBoard)
                {
                    cerr << "ERROR: It is not possible to move to a square that is not on the board.\nTry again\n";
                    continue;
                }
                
                i = toSquareNum / numRowsInBoard;
                j = toSquareNum % numRowsInBoard;
                
                if(myCMCheckersBoard[i][j] != NOCHECKER)
                {
                    cerr << "ERROR: It is not possible to move to a square that is alcready occupied" << endl << "Try again" << endl;
                    continue;
                }
                
                if(MakeMove(myCMCheckersBoard, numRowsInBoard, player, fromSquareNum, toSquareNum, jumped) == false)
                {
                    cerr << "ERROR: moving to that square is not legal, Try again\n";
                    continue;
                }
                
                error = false;
                
            }while(error);
            
        }
        turn++;
    }while(CheckWin(myCMCheckersBoard, numRowsInBoard) == false);
    
    cout << "Enter any character to terminate the game then press the enter key\n"; //GAME OVER
    cin >> endGame;
    
    return 0;
}

void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
    const int numRowsNotSoldier = 4;
    int numRowsOfSoldiers = 0;
    int i = 0;
    int j = 0;
    
    numRowsOfSoldiers = numRowsInBoard - numRowsNotSoldier; //Determine rows of soldier
    
    for(j=1; j<numRowsInBoard; j+=2) //Place mules
    {
        CMCheckersBoard[0][j] = WHITEMULE;
        CMCheckersBoard[numRowsInBoard-1][j-1] = REDMULE;
    }
    
    for(i=1; i<(numRowsOfSoldiers/2)+1; i++) //Place soldiers
    {
        for(j=0; j<numRowsInBoard; j+=2)
        {
            CMCheckersBoard[i][1-(i % ODD_OR_EVEN)+j] = WHITESOLDIER;
            CMCheckersBoard[numRowsInBoard-1-i][(i % ODD_OR_EVEN)+j] = REDSOLDIER;
        }
    }
    return ;
}

void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
    const int WIDTH_SQUARE = 4;
    int i = 0;
    int j = 0;
    int indexSquare = 0;
    //print the corresponding label for the checkers
    for(i=0; i<numRowsInBoard; i++)
    {
        for(j=0; j<numRowsInBoard; j++)
        {
            if(CMCheckersBoard[i][j] == WHITESOLDIER)
            {
                cout << setw(WIDTH_SQUARE) << "WS";
            }
            else if(CMCheckersBoard[i][j] == WHITEMULE)
            {
                cout << setw(WIDTH_SQUARE) << "WM";
            }
            else if(CMCheckersBoard[i][j] == WHITEKING)
            {
                cout << setw(WIDTH_SQUARE) << "WK";
            }
            else if(CMCheckersBoard[i][j] == REDSOLDIER)
            {
                cout << setw(WIDTH_SQUARE) << "RS";
            }
            else if(CMCheckersBoard[i][j] == REDMULE)
            {
                cout << setw(WIDTH_SQUARE) << "RM";
            }
            else if(CMCheckersBoard[i][j] == REDKING)
            {
                cout << setw(WIDTH_SQUARE) << "RK";
            }
            else
            {
                cout << setw(WIDTH_SQUARE) << indexSquare;
            }
            
            indexSquare++;
        }
        
        cout << endl;
    }
    return ;
}

int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[])
{
    int i = 0;
    int yIndex = 0;
    int xIndex = 0;
    int jumps = 0;
    
    for(i=0; i<MAX_PIECES; i++) //initialize the contents of array to -1
    {
        yLocArray[i] = -1;
        xLocArray[i] = -1;
    }
    
    for(yIndex=0; yIndex<numRowsInBoard; yIndex++)
    {
        for(xIndex=0; xIndex<numRowsInBoard; xIndex++)
        {
            if(IsJump(CMCheckersBoard, numRowsInBoard, player, xIndex, yIndex) == true)
            {
                xLocArray[jumps] = xIndex; //store the x index of checker that can move
                yLocArray[jumps] = yIndex; //store the y index of checker that can move
                jumps++;
            }
        }
    }
    
    return jumps;
}

bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex)
{
    bool InList = true;
    bool NotInList = false;
    int index = 0;
    
    for(index=0; index<MAX_PIECES; index++) //Sending statement whether chosen checker can make chosen move
    {
        if(inArray1[index] == -1)
        {
            break;
        }
        else if(inArray1[index] == xIndex && inArray2[index] == yIndex)
        {
            return InList;
        }
    }
    
    return NotInList;
}

int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[],  int yLocArray[])
{
    int index = 0;
    int xindex = 0;
    int yindex = 0;
    int checkersCanMove = 0;
    bool cannotMove = false;
    
    for(index=0; index<MAX_PIECES; index++) //initialize the contents of array to -1
    {
        xLocArray[index] = -1;
        yLocArray[index] = -1;
    }
    
    for(yindex=0; yindex<numRowsInBoard; yindex++)
    {
        for(xindex = 0; xindex<numRowsInBoard; xindex++)
        {
            if(IsMove1Square(CMCheckersBoard, numRowsInBoard, player, xindex, yindex) != cannotMove)
            {
                xLocArray[ checkersCanMove] = xindex; //store the x index of checker that can move
                yLocArray[ checkersCanMove] = yindex; //store the y index of checker that can move
                checkersCanMove++;
            }
        }
    }
    
    return checkersCanMove;
}

bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
    bool move = false;
    if (player == WHITEPLAYER)
    {
        //see if there is space to move going forward
        if (yLoc < numRowsInBoard - 1)
        {
            if (xLoc == 0) //special case on side of board
            {
                if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    //check if there is an empty square diagonally in the row forward
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == NOCHECKER )|| (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
            else if (xLoc == numRowsInBoard -1) //special case on side of board
            {
                if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    //check if there is an empty square diagonally in the row forward
                    if( (CMCheckersBoard[yLoc + 1][0] == NOCHECKER )|| (CMCheckersBoard[yLoc + 1][xLoc - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
            else
            {
                if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    //check if there is an empty square diagonally in the row forward
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == NOCHECKER )|| (CMCheckersBoard[yLoc + 1][xLoc - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
        }
        //see if there is space to move going backward
        if (yLoc > 0)
        {
            if (xLoc == 0) //special case on side of board
            {
                //check behind the king for a move
                if (CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == NOCHECKER )|| (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
            
            else if (xLoc == numRowsInBoard -1) //special case on side of board
            {
                //check behind the king for moves
                if (CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    if( (CMCheckersBoard[yLoc - 1][0] == NOCHECKER )|| (CMCheckersBoard[yLoc - 1][xLoc - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
            else
            {
                //check behind the king for moves
                if (CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == NOCHECKER )|| (CMCheckersBoard[yLoc - 1][xLoc - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
        }
    }
    
    //now check for player 2 ***********************************************
    
    else
    {
        //see if there is space to move going forward
        if (yLoc > 0)
        {
            if (xLoc == 0) //special case on side of board
            {
                if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    //check if there is an empty square diagonally in the row forward
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == NOCHECKER )|| (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
            else if (xLoc == numRowsInBoard -1) //special case on side of board
            {
                if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    //check if there is an empty square diagonally in the row forward
                    if( (CMCheckersBoard[yLoc - 1][0] == NOCHECKER )|| (CMCheckersBoard[yLoc - 1][xLoc - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
            else
            {
                if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    //check if there is an empty square diagonally in the row forward
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == NOCHECKER )|| (CMCheckersBoard[yLoc - 1][xLoc - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
        }
        //see if there is space to move going backward
        if (yLoc < numRowsInBoard - 1)
        {
            if (xLoc == 0) //special case on side of board
            {
                //check behind the king for a move
                if (CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == NOCHECKER )|| (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
            
            else if (xLoc == numRowsInBoard -1) //special case on side of board
            {
                //check behind the king for moves
                if (CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    if( (CMCheckersBoard[yLoc + 1][0] == NOCHECKER )|| (CMCheckersBoard[yLoc + 1][xLoc - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
            else
            {
                //check behind the king for moves
                if (CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == NOCHECKER )|| (CMCheckersBoard[yLoc + 1][xLoc - 1] == NOCHECKER ) )
                    {
                        bool move = true;
                        return move;
                    }
                }
            }
        }
    }
    
    return move;
}

bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
    bool jump = false;
    //check player
    if (player == WHITEPLAYER)
    {
        //see if there is space to jump going forward
        if (yLoc < numRowsInBoard - 2)
        {
            if (xLoc == 0) //special case on side of board
            {
                if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            else if (xLoc == 1) //special case on side of board
            {
                if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            else if (xLoc == numRowsInBoard - 2) //special case on side of board
            {
                if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][0] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            else if (xLoc == numRowsInBoard -1) //special case on side of board
            {
                if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc + 1][0] == REDSOLDIER || CMCheckersBoard[yLoc + 1][0] == REDMULE || CMCheckersBoard[yLoc + 1][0] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][1] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            else
            {
                
                //check checker type in this board position
                if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            
        }
        //see if there is space to jump going backward
        if (yLoc > 1)
        {
            if (xLoc == 0) //special case on side of board
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
            else if (xLoc == 1) //special case on side of board
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
            else if (xLoc == numRowsInBoard - 2) //special case on side of board
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][0] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
            else if (xLoc == numRowsInBoard -1) //special case on side of board
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    if( (CMCheckersBoard[yLoc - 1][0] == REDSOLDIER || CMCheckersBoard[yLoc - 1][0] == REDMULE || CMCheckersBoard[yLoc - 1][0] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][1] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
            else
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == WHITEKING )
                {
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
        }
    }
    
    //now check for player 2 ***********************************************
    
    else
    {
        //see if there is space to jump going forward
        if (yLoc > 1)
        {
            if (xLoc == 0) //special case on side of board
            {
                //check checker type in this board position
                if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            else if (xLoc == 1) //special case on side of board
            {
                //check checker type in this board position
                if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            else if (xLoc == numRowsInBoard - 2) //special case on side of board
            {
                //check checker type in this board position
                if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][0] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            else if (xLoc == numRowsInBoard -1) //special case on side of board
            {
                //check checker type in this board position
                if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc - 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][0] == WHITEMULE || CMCheckersBoard[yLoc - 1][0] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][1] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            else
            {
                
                //check checker type in this board position
                if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    //check if there is an oppent's checker diagonally in the row forward
                    if( (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc - 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc - 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    
                }
            }
            
        }
        //see if there is space to jump going backward
        if (yLoc < numRowsInBoard - 2)
        {
            if (xLoc == 0) //special case on side of board
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
            else if (xLoc == 1) //special case on side of board
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
            else if (xLoc == numRowsInBoard - 2) //special case on side of board
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][0] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
            else if (xLoc == numRowsInBoard -1) //special case on side of board
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == REDKING )
                {
                    if( (CMCheckersBoard[yLoc + 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][0] == WHITEMULE || CMCheckersBoard[yLoc + 1][0] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][1] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
            else
            {
                //check behind the king for jumps
                if (CMCheckersBoard[yLoc][xLoc] == REDKING)
                {
                    if( (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
                       //check if the next space in the same diagonal is empty
                       && CMCheckersBoard[yLoc + 2][xLoc + 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                    else if( (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
                            //check if the next space in the same diagonal is empty
                            && CMCheckersBoard[yLoc + 2][xLoc - 2] == NOCHECKER )
                    {
                        bool jump = true;
                        return jump;
                    }
                }
            }
        }
    }
    
    return jump;
}

bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int fromSquareNum, int toSquareNum, bool &jumped )
{
    int xDistance = 0;
    int yDistance = 0;
    int xLocFrom = fromSquareNum%numRowsInBoard;
    int yLocFrom = fromSquareNum/numRowsInBoard;
    int xLocTo = toSquareNum%numRowsInBoard;
    int yLocTo = toSquareNum/numRowsInBoard;
    string endGame = "";
    
    //determine the xDistance
    xDistance = abs(fromSquareNum%numRowsInBoard - toSquareNum%numRowsInBoard);
    //special cases going around the side of the board
    if ( xDistance == numRowsInBoard - 1)
    {
        xDistance = 1;
    }
    else if ( xDistance == numRowsInBoard - 2)
    {
        xDistance = 2;
    }
    
    //determine the yDistance
    yDistance = abs(fromSquareNum/numRowsInBoard - toSquareNum/numRowsInBoard);
    
    if ( !((xDistance == 2 && yDistance == 2) || (xDistance == 1 && yDistance == 1)) )
    {
        cerr << "Error; illegal move" << endl;
        return false;
    }
    
    //check if the space between the two squares is empty
    //check special cases first
    if ( (xDistance == 2 && yDistance == 2)        )
    {
        
        if ( player == WHITEPLAYER)
        {
            if((fromSquareNum%numRowsInBoard == 0 && toSquareNum%numRowsInBoard == numRowsInBoard -2)
               || (fromSquareNum%numRowsInBoard == numRowsInBoard -2 && toSquareNum%numRowsInBoard == 0))
            {
                if( !(CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][numRowsInBoard-1] == REDSOLDIER
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][numRowsInBoard-1] == REDMULE
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][numRowsInBoard-1] == REDKING)
                   )
                    
                {
                    cerr << "Error; illegal move" << endl;
                    return false;
                }
            }
            else if ((fromSquareNum%numRowsInBoard == 1 && toSquareNum%numRowsInBoard == numRowsInBoard -1)
                     || (fromSquareNum%numRowsInBoard == numRowsInBoard -1 && toSquareNum%numRowsInBoard == 1))
            {
                if( !(CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][0] == REDSOLDIER
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][0] == REDMULE
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][0] == REDKING)
                   )
                    
                {
                    cerr << "Error; illegal move" << endl;
                    return false;
                }
            }
            
            else
            {
                if( !(CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][(fromSquareNum%numRowsInBoard+toSquareNum%numRowsInBoard)/2] == REDSOLDIER
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][(fromSquareNum%numRowsInBoard+toSquareNum%numRowsInBoard)/2] == REDMULE
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][(fromSquareNum%numRowsInBoard+toSquareNum%numRowsInBoard)/2] == REDKING)
                   )
                    
                {
                    cerr << "Error; illegal move" << endl;
                    return false;
                }
                
            }
        }
        else if ( player == REDPLAYER)
        {
            if((fromSquareNum%numRowsInBoard == 0 && toSquareNum%numRowsInBoard == numRowsInBoard -2)
               || (fromSquareNum%numRowsInBoard == numRowsInBoard -2 && toSquareNum%numRowsInBoard == 0))
            {
                if( !(CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][numRowsInBoard-1] == WHITESOLDIER
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][numRowsInBoard-1] == WHITEMULE
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][numRowsInBoard-1] == WHITEKING)
                   )
                    
                {
                    cerr << "Error; illegal move" << endl;
                    return false;
                }
            }
            else if ((fromSquareNum%numRowsInBoard == 1 && toSquareNum%numRowsInBoard == numRowsInBoard -1)
                     || (fromSquareNum%numRowsInBoard == numRowsInBoard -1 && toSquareNum%numRowsInBoard == 1))
            {
                if( !(CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][0] == WHITESOLDIER
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][0] == WHITEMULE
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][0] == WHITEKING)
                   )
                    
                {
                    cerr << "Error; illegal move" << endl;
                    return false;
                }
            }
            
            else
            {
                if( !(CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][(fromSquareNum%numRowsInBoard+toSquareNum%numRowsInBoard)/2] == WHITESOLDIER
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][(fromSquareNum%numRowsInBoard+toSquareNum%numRowsInBoard)/2] == WHITEMULE
                      || CMCheckersBoard[(fromSquareNum/numRowsInBoard+toSquareNum/numRowsInBoard)/2][(fromSquareNum%numRowsInBoard+toSquareNum%numRowsInBoard)/2] == WHITEKING)
                   )
                    
                {
                    cerr << "Error; illegal move" << endl;
                    return false;
                }
                
            }
        }
    }
    
    //if soldiers or mules are moving in the wrong direction
    if (player == WHITEPLAYER)
    {
        if (CMCheckersBoard[fromSquareNum/numRowsInBoard][fromSquareNum%numRowsInBoard] == WHITESOLDIER
            || CMCheckersBoard[fromSquareNum/numRowsInBoard][fromSquareNum%numRowsInBoard] == WHITEMULE)
        {
            if (fromSquareNum/numRowsInBoard - toSquareNum/numRowsInBoard > 0)
            {
                cerr << "Error; illegal move" << endl;
                return false;
            }
        }
    }
    else if (player == REDPLAYER)
    {
        if (CMCheckersBoard[fromSquareNum/numRowsInBoard][fromSquareNum%numRowsInBoard] == REDSOLDIER
            || CMCheckersBoard[fromSquareNum/numRowsInBoard][fromSquareNum%numRowsInBoard] == REDMULE)
        {
            if (fromSquareNum/numRowsInBoard - toSquareNum/numRowsInBoard < 0)
            {
                cerr << "Error; illegal move" << endl;
                return false;
            }
        }
    }
    if(xDistance == 1 && yDistance == 1 && IsJump(CMCheckersBoard, numRowsInBoard, player, xLocFrom, yLocFrom))
    {
        cerr << "ERROR: You can jump with this checker, you must jump not move 1 space" << endl << "Try again" << endl;
        return false;
    }
    //make the move
    if(IsJump(CMCheckersBoard, numRowsInBoard, player, xLocFrom, yLocFrom) == false)
    {
        if (player == WHITEPLAYER)
        {
            if (xDistance == 1 && yDistance == 1)
            {
                if (CMCheckersBoard[yLocFrom][xLocFrom] == WHITESOLDIER
                    || CMCheckersBoard[yLocFrom][xLocFrom] == WHITEKING
                    || CMCheckersBoard[yLocFrom][xLocFrom] == WHITEMULE)
                {
                    CMCheckersBoard[yLocTo][xLocTo] = CMCheckersBoard[yLocFrom][xLocFrom];
                    CMCheckersBoard[yLocFrom][xLocFrom] = NOCHECKER;
                }
            }
            
        }
        else if (player == REDPLAYER)
        {
            if (xDistance == 1 && yDistance == 1)
            {
                if (CMCheckersBoard[yLocFrom][xLocFrom] == REDSOLDIER
                    || CMCheckersBoard[yLocFrom][xLocFrom] == REDKING
                    || CMCheckersBoard[yLocFrom][xLocFrom] == REDMULE)
                {
                    CMCheckersBoard[yLocTo][xLocTo] = CMCheckersBoard[yLocFrom][xLocFrom];
                    CMCheckersBoard[yLocFrom][xLocFrom] = NOCHECKER;
                }
            }
        }
    }
    
    //make the jump
    if (player == WHITEPLAYER)
    {
        if (xDistance == 2 && yDistance == 2)
        {
            if (CMCheckersBoard[yLocFrom][xLocFrom] == WHITESOLDIER
                || CMCheckersBoard[yLocFrom][xLocFrom] == WHITEKING
                || CMCheckersBoard[yLocFrom][xLocFrom] == WHITEMULE)
            {
                CMCheckersBoard[yLocTo][xLocTo] = CMCheckersBoard[yLocFrom][xLocFrom];
                CMCheckersBoard[yLocFrom][xLocFrom] = NOCHECKER;
                jumped = true;
                
                //now get rid of the piece that was jumped over
                //start with special cases
                if (xLocFrom == 0 && xLocTo == numRowsInBoard -2)
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][numRowsInBoard - 1] = NOCHECKER;
                }
                else if (xLocFrom == 1 && xLocTo == numRowsInBoard -1)
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][0] = NOCHECKER;
                }
                else if (xLocFrom == numRowsInBoard - 1 && xLocTo == 1)
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][0] = NOCHECKER;
                }
                else if (xLocFrom == numRowsInBoard - 2 && xLocTo == 0)
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][numRowsInBoard - 1] = NOCHECKER;
                }
                else
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][(xLocFrom + xLocTo)/2] = NOCHECKER;
                }
                
            }
        }
        
    }
    else if (player == REDPLAYER)
    {
        if (xDistance == 2 && yDistance == 2)
        {
            if (CMCheckersBoard[yLocFrom][xLocFrom] == REDSOLDIER
                || CMCheckersBoard[yLocFrom][xLocFrom] == REDKING
                || CMCheckersBoard[yLocFrom][xLocFrom] == REDMULE)
            {
                CMCheckersBoard[yLocTo][xLocTo] = CMCheckersBoard[yLocFrom][xLocFrom];
                CMCheckersBoard[yLocFrom][xLocFrom] = NOCHECKER;
                jumped = true;
                
                //now get rid of the piece that was jumped over
                //start with special cases
                if (xLocFrom == 0 && xLocTo == numRowsInBoard -2)
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][numRowsInBoard - 1] = NOCHECKER;
                }
                else if (xLocFrom == 1 && xLocTo == numRowsInBoard -1)
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][0] = NOCHECKER;
                }
                else if (xLocFrom == numRowsInBoard - 1 && xLocTo == 1)
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][0] = NOCHECKER;
                }
                else if (xLocFrom == numRowsInBoard - 2 && xLocTo == 0)
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][numRowsInBoard - 1] = NOCHECKER;
                }
                else
                {
                    CMCheckersBoard[(yLocFrom + yLocTo)/2][(xLocFrom + xLocTo)/2] = NOCHECKER;
                }
            }
        }
    }
    
    // check to see if jump or move made checker a king
    // first check mule
    if (player == WHITEPLAYER)
    {
        if (yLocTo == numRowsInBoard-1)
        {
            if (CMCheckersBoard[yLocTo][xLocTo] == WHITEMULE)
            {
                cout << "White has created a Mule King, Red wins the game" << endl;
                cout << "Enter any character to terminate the game then press the enter key" << endl;
                cin >> endGame;
                exit(111);
            }
            else if (CMCheckersBoard[yLocTo][xLocTo] == WHITESOLDIER)
            {
                CMCheckersBoard[yLocTo][xLocTo] = WHITEKING;
            }
        }
    }
    else if (player == REDPLAYER)
    {
        if (yLocTo == 0)
        {
            if (CMCheckersBoard[yLocTo][xLocTo] == REDMULE)
            {
                cout << "Red has created a Mule King, White wins the game" << endl;
                cout << "Enter any character to terminate the game then press the enter key" << endl;
                cin >> endGame;
                exit(111);
            }
            else if (CMCheckersBoard[yLocTo][xLocTo] == REDSOLDIER)
            {
                CMCheckersBoard[yLocTo][xLocTo] = REDKING;
            }
        }
    }
    
    return true;
}

bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
    
    int i = 0;
    int j = 0;
    int countWhiteSoldier = 0;
    int countWhiteMule = 0;
    int countWhiteKing = 0;
    int countRedSoldier = 0;
    int countRedMule = 0;
    int countRedKing = 0;
    
    //count the number of each type of piece on board
    for (i = 0; i < numRowsInBoard; i++)
    {
        for (j = 0; j < numRowsInBoard; j++)
        {
            if (CMCheckersBoard[i][j] == WHITESOLDIER)
            {
                countWhiteSoldier++;
            }
            else if
                (CMCheckersBoard[i][j] == WHITEMULE)
            {
                countWhiteMule++;
            }
            else if
                (CMCheckersBoard[i][j] == WHITEKING)
            {
                countWhiteKing++;
            }
            else if
                (CMCheckersBoard[i][j] == REDSOLDIER)
            {
                countRedSoldier++;
            }
            else if
                (CMCheckersBoard[i][j] == REDMULE)
            {
                countRedMule++;
            }
            else if
                (CMCheckersBoard[i][j] == REDKING)
            {
                countRedKing++;
            }
        }
    }
    
    //if no there are no Red Mules red wins
    if (countRedMule == 0)
    {
        cout << "The Red Player has won the game by losing all of the Red Mules" << endl;
    }
    
    //if no white mules white wins
    if (countWhiteMule == NOCHECKER)
    {
        cout << "The White Player has won the game by losing all of the White Mules" << endl;
    }
    
    //if there are red mules left but no red other pieces white wins
    if (countRedMule > NOCHECKER && countRedSoldier == NOCHECKER && countRedKing == NOCHECKER)
    {
        cout << "The White Player has won by capturing all of the red players soldiers and kings" << endl;
    }
    
    //if there are white mules left but no white other pieces red wins
    if (countWhiteMule > NOCHECKER && countWhiteSoldier == NOCHECKER && countWhiteKing == NOCHECKER)
    {
        cout << "The Red Player has won by capturing all of the white players soldiers and kings" << endl;
    }
    
    
    if ( (countWhiteMule > NOCHECKER && countWhiteSoldier == NOCHECKER && countWhiteKing == NOCHECKER) || (countRedMule > NOCHECKER && countRedSoldier == NOCHECKER && countRedKing == NOCHECKER) || (countWhiteMule == NOCHECKER) || (countRedMule == NOCHECKER) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

