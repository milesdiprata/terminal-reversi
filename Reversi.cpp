/* Includes */
#include <iostream>


/* Namespaces */
using namespace std;


/* Global variables */
enum BOARD_PIECE
{
	UNOCCUPIED = 'U',
	WHITE = 'W',
	BLACK = 'B',
};
enum DIRECTION
{
	NORTHWEST,
	NORTH,
	NORTHEAST,
	WEST,
	EAST,
	SOUTHWEST,
	SOUTH,
	SOUTHEAST,
	null,
};

char (*board)[26] = new char[26][26];
char *axisTags = new char[26];


/* Function declarations */


/* Function initializations */

/*
	Converts character rows and columns into integer index for 2d array.
*/
int convertToIntCoord(const char coordinate)
{
	return (int)coordinate - 'a';
}

/*
	Displays the board's current state on screen.
*/
void printBoard(const int size)
{
	// Print top axis first
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
			cout << "  " << axisTags[i];
		else
			cout << axisTags[i];
	}
	cout << endl;

	for (int i = 0; i < size; i++)
	{
		cout << axisTags[i] << " ";
		for (int j = 0; j < size; j++)
			cout << board[i][j];
		cout << endl;
	}
}

/*
	Get the board size from the user.
*/
int getBoardSize()
{
	int n;
	cout << "Enter the board dimension: ";
	cin >> n;

	if (n < 0 || n > 26 || n % 2 != 0)
		return -1;

	return n;
}

/*
	Given the board size, intializes into the 'Othello' version start state.
*/
bool initializeBoard(const int size)
{
    // Error check
    if (size == -1)
   		return false;

    // Init axis tags
    for (int i = 0; i < 26; i++)
        axisTags[i] = i + 'a';

    // Init board to start position
    for (int i = 0; i < size; i++)
    	for (int j = 0; j < size; j++)
			board[i][j] = UNOCCUPIED;

    board[(size/2) - 1][(size/2) - 1] = WHITE;
    board[(size/2) - 1][size/2] = BLACK;
    board[(size/2)][(size/2) - 1] = BLACK;
    board[(size/2)][(size/2)] = WHITE;

	printBoard(size);
    return true;
}

/*
	Rejects invalid characters entered by the user. Note: !!! to quit.
*/
bool isValidConfigInput(const char input[])
{
	if (input[2] == '\0')
		if (input[1] >= 'a' && input[1] <= 'z')
			if (input[0] >= 'a' && input[0] <= 'z')
				return true;

	return false;

}


BOARD_PIECE getComputerColor()
{
	char input;
	BOARD_PIECE computerColor;

	cout << "Computer plays (B/W): ";
	cin >> input;

	if (input != 'B' && input != 'W')
		return UNOCCUPIED;
	computerColor = (BOARD_PIECE)input;

	return computerColor;
}

/*
	Checks whether a given row and column lies within the valid playing area of the board.
*/
bool positionInBounds(const int size, const char row, const char col)
{
	if ( (convertToIntCoord(row) >= 0 && convertToIntCoord(row) <= (size - 1))
		&& (convertToIntCoord(col) >= 0 && convertToIntCoord(col) <= (size - 1)) )
		return true;
	else
		return false;
}

/*
	Given the enum type direction, increments/decrements the row and column traversal modifier (i and j)
	appripriatly. Since we are using a nxn matrix, the coordinate scheme is as follows:
	-----------> +j
	|
	|
	|
	V +i
*/
void incrementDirectionModifiers(const DIRECTION direction, int &i, int &j)
{
	// Northwest
	if (direction == NORTHWEST)
	{
		i--;
		j--;
	}
	// North
	else if (direction == NORTH)
	{
		i--;
	}
	// Northeast
	else if (direction == NORTHEAST)
	{
		i--;
		j++;
	}
	// West
	else if (direction == WEST)
	{
		j--;
	}
	// East
	else if (direction == EAST)
	{
		j++;
	}
	//Southwest
	else if (direction == SOUTHWEST)
	{
		i++;
		j--;
	}
	// South
	else if (direction == SOUTH)
	{
		i++;
	}
	// Southeast
	else if (direction == SOUTHEAST)
	{
		i++;
		j++;
	}
}

/*
	Given a potential position for a given color's move, returns true if the board position satisfies the following
	criteria:
		1. 	There must be a continuous straight line of tiles of the opponent's color in at least one of the eight
		 	directions from the candidate empty position (North, South, East, West, and diagonals).
		2. 	In the position immediately following the continuous straight line mentioned in #1 above,
			a tile of the player’s color must already be placed.
	and returns false otherwise.
*/
bool checkIfLegalPositionInDirection(const int size, const char row, const char col, const char color, const DIRECTION direction)
{
	if (direction == null || color == 'U' || board[convertToIntCoord(row)][convertToIntCoord(col)] != UNOCCUPIED)
		return false;

	BOARD_PIECE playerColor = (BOARD_PIECE)color;
	BOARD_PIECE opponentColor;

	int i = 0;
	int j = 0;

	bool foundOneOpponent = false;

	if (playerColor == WHITE)
		opponentColor = BLACK;
	else
		opponentColor = WHITE;

	while (positionInBounds(size, row + i, col + j))
	{
		// Check for one or more contigious opponent pieces followed by a single player piece
		if (i == 0 && j == 0)
		{
			incrementDirectionModifiers(direction, i, j); // Initial increment/decrement
		}
		else if (board[convertToIntCoord(row) + i][convertToIntCoord(col) + j] == opponentColor)
		{
			incrementDirectionModifiers(direction, i, j);
			foundOneOpponent = true;
		}
		else if (board[convertToIntCoord(row) + i][convertToIntCoord(col) + j] == playerColor && foundOneOpponent)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
	
}

/*
	Returns the direction in which a given potential move is legal in.
*/
DIRECTION checkIfLegalPositionInAllDirections(const int size, const char row, const char col, const char color)
{
	if (checkIfLegalPositionInDirection(size, row, col, color, NORTHWEST)) // Northwest
		return NORTHWEST;
	if (checkIfLegalPositionInDirection(size, row, col, color, NORTH))  // North
		return NORTH;
	if (checkIfLegalPositionInDirection(size, row, col, color, NORTHEAST)) // Northeast
		return NORTHEAST;
	if (checkIfLegalPositionInDirection(size, row, col, color, WEST)) // West
		return WEST;
	if (checkIfLegalPositionInDirection(size, row, col, color, EAST)) // East
		return EAST;
	if (checkIfLegalPositionInDirection(size, row, col, color, SOUTHWEST)) // Southwest
		return SOUTHWEST;
	if (checkIfLegalPositionInDirection(size, row, col, color, SOUTH)) // South
		return SOUTH;
	if (checkIfLegalPositionInDirection(size, row, col, color, SOUTHEAST)) // Southeast
		return SOUTHEAST;

	return null;
}


/*
	Returns an array of directions in which a move is legal in. Helper function for placeTile().
*/
DIRECTION* getLegalDirectionsForTile(const int size, const char row, const char col, const char color)
{
	DIRECTION *arrayOfDirections = new DIRECTION[7];

	int index = 0;

	for (int i = 0; i < 7; i++)
		arrayOfDirections[i] = null;

	if (checkIfLegalPositionInDirection(size, row, col, color, NORTHWEST)) // Northwest
		arrayOfDirections[index++] = NORTHWEST;
	if (checkIfLegalPositionInDirection(size, row, col, color, NORTH)) // North
		arrayOfDirections[index++] = NORTH;
	if (checkIfLegalPositionInDirection(size, row, col, color, NORTHEAST)) // Northeast
		arrayOfDirections[index++] = NORTHEAST;
	if (checkIfLegalPositionInDirection(size, row, col, color, WEST)) // West
		arrayOfDirections[index++] = WEST;
	if (checkIfLegalPositionInDirection(size, row, col, color, EAST)) // East
		arrayOfDirections[index++] = EAST;
	if (checkIfLegalPositionInDirection(size, row, col, color, SOUTHWEST)) // Southwest
		arrayOfDirections[index++] = SOUTHWEST;
	if (checkIfLegalPositionInDirection(size, row, col, color, SOUTH)) // South
		arrayOfDirections[index++] = SOUTH;
	if (checkIfLegalPositionInDirection(size, row, col, color, SOUTHEAST)) // Southeast
		arrayOfDirections[index++] = SOUTHEAST;

	return arrayOfDirections;
}


void placeTile(const int size, const char row, const char col, const char color)
{
	int index = 0;
	int i = 0;
	int j = 0;

	BOARD_PIECE playerColor = (BOARD_PIECE)color;
	DIRECTION *directionsToFill = getLegalDirectionsForTile(size, row, col, color);

	// Iterate through each legal direction
	while (directionsToFill[index] != null)
	{
		while (board[convertToIntCoord(row) + i][convertToIntCoord(col) + j] != playerColor)
		{
			board[convertToIntCoord(row) + i][convertToIntCoord(col) + j] = playerColor;
			incrementDirectionModifiers(directionsToFill[index], i, j);
		}
		i = 0;
		j = 0;
		incrementDirectionModifiers(directionsToFill[++index], i, j);
	}
}

int getMoveScore(const int size, const char row, const char col, const char color)
{
	int index = 0;
	int i = 0;
	int j = 0;

	int currentMoveScore = 0;

	BOARD_PIECE playerColor = (BOARD_PIECE)color;
	DIRECTION *directionsToFill = getLegalDirectionsForTile(size, row, col, color);

	// Iterate through each legal direction
	while (directionsToFill[index] != null)
	{
		while (board[convertToIntCoord(row) + i][convertToIntCoord(col) + j] != playerColor)
		{
			currentMoveScore++;
			incrementDirectionModifiers(directionsToFill[index], i, j);
		}
		i = 0;
		j = 0;
		incrementDirectionModifiers(directionsToFill[++index], i, j);
	}

	return currentMoveScore;
}

/*
	Given the board's current state, display which potential color's moves are legal as per the same
	criteria of Reversei.
*/
void displayPossibleMoves(const int size, const BOARD_PIECE color)
{
	cout << "Possible moves for " << (char)color << ": " << endl;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (checkIfLegalPositionInAllDirections(size, i + 'a', j + 'a', color) != null)
				cout << (char)(i + 'a') << (char)(j + 'a') << ", Score: " << getMoveScore(size, i + 'a', j + 'a', color) << endl;
}

/*
	Get user input for entering a move.
*/
void getUserMove(const int size, const BOARD_PIECE playerColor)
{
	char input[100];

	// displayPossibleMoves(size, playerColor);

	cout << "Enter move for color " << (char)playerColor << " (RowCol): " << endl;
	cin >> input;

	char row = input[0];
	char col = input[1];

	if (isValidConfigInput(input) && checkIfLegalPositionInAllDirections(size, row, col, playerColor) != null)
	{
		cout << "Valid move." << endl;
		placeTile(size, row, col, playerColor);
		printBoard(size);
	}
	else
	{
		cout << "Invalid move!" << endl;
		getUserMove(size, playerColor);
	}
}

void computerPlayMove(const int size, const BOARD_PIECE computerColor)
{
	int bestMoveScore = -1;
	char bestRow = '\0';
	char bestCol = '\0';

	int currentMoveScore;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (checkIfLegalPositionInAllDirections(size, i + 'a', j + 'a', computerColor) != null)
			{
				currentMoveScore = getMoveScore(size, i + 'a', j + 'a', computerColor);
				if (currentMoveScore >= bestMoveScore && currentMoveScore > 0)
				{
					bestMoveScore = currentMoveScore;
					bestRow = i + 'a';
					bestCol = j + 'a';	
				}
			}

		}
	}

	cout << "Computer places " << (char)computerColor << " at " << (char)bestRow << (char)bestCol << "." << endl;
	placeTile(size, bestRow, bestCol, computerColor);
	printBoard(size);
}

bool hasAvailibleMove(const int size, const BOARD_PIECE color)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (checkIfLegalPositionInAllDirections(size, i + 'a', j + 'a', color) != null)
				return true;

	return false;
}

int getColorScore(const int size, const BOARD_PIECE color)
{
	int score = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (board[i][j] == color)
				score++;

	return score;
}

void playGame(const int size, const BOARD_PIECE computerColor)
{
	BOARD_PIECE playerColor;
	bool isComputerMove;

	if (computerColor == BLACK)
	{
		playerColor = WHITE;
		isComputerMove = true;
	}
	else
	{
		playerColor = BLACK;
		isComputerMove = false;
	}

	while (hasAvailibleMove(size, computerColor) || hasAvailibleMove(size, playerColor))
	{
		if (isComputerMove)
		{
			isComputerMove = false;
			if (hasAvailibleMove(size, computerColor))
				computerPlayMove(size, computerColor);
		}
		else if (!isComputerMove)
		{
			isComputerMove = true;
			if (hasAvailibleMove(size, playerColor))
				getUserMove(size, playerColor);
		}
	}

	if (getColorScore(size, computerColor) > getColorScore(size, playerColor))
		cout << (char)computerColor << " player wins!" << endl;
	else if (getColorScore(size, computerColor) == getColorScore(size, playerColor))
		cout << "Draw!" << endl;
	else
		cout << (char)playerColor << " player wins!" << endl;

}

void closeGame()
{
	delete[] board;
	delete[] axisTags;

	board = nullptr;
	axisTags = nullptr;
}

int main(const int argc, const char* const argv[])
{
	int size = getBoardSize();
    if (!initializeBoard(size))
    {
    	cerr << "Error: Could not initialize board!" << endl;
    	return -1;
    }
	BOARD_PIECE computerColor = getComputerColor();
    if (computerColor == UNOCCUPIED)
    {
    	cerr << "Error: Could not get the comptuer color!" << endl;
    	return -2;
    }
    playGame(size, computerColor);

	closeGame();
    return 0;
}
