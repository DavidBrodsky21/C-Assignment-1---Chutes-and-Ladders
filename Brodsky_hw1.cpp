/************************************************************************************
Title: Chutes & Ladders (HW1)
Author: David Brodsky
Created on: July 13, 2015
Description:  Simulates a game of chutes & ladders based on the criteria provided
			  by professor as part of HW #1
************************************************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace std;

//Initialize Global Constants

const int MAX_POS = 156; 
/*
Creates a constant for the max position a player could ever get to.  Used to initialize various arrays in program.
Max position = max # of cells on board, which is 144 (12^2) + 11 (if you roll max dice from postition 143) 
               + 1 (because I'm going to ignore position 0, i.e. index 0 for all of the arrays for tractability)
*/


//Initialize Functions
void setup(int& n, int& players, int& die_sides, int board[], int board_size);  //pulls info from text file, sets up the board 
void roll(int die_sides, int playerPos[], int whoseTurn, int board[]);  //represents a die roll
void checkForTrap(int board[], int playerPos[], int whoseTurn, bool& trap);  //check whether a cell has a snake or ladder
int max_element(int array[], int arraySize);  //returns the index of the largest member of an array
void rank(int playerPos[], int players);  //ranks + outputs player positions relative to each other



int main()
{
	srand(time(0)); //seed random # generator for dice rolls

// n holds the # of cells on each side of the board, i.e. board is n x n
	int n, players, die_sides;
//board[] will be an array containing all the snakes & ladders where the array index represents the board cell and the value stored represents 
//how many cells forward or backward a player should go if she lands on that board cell (due to snake or ladder).   
//I.e. board[1] = 0 represents the bottom left corner of the board, the first cell in the game, and indicates that there is no snake or ladder there.
// board[15] = -7 represents the 15th cell in the game and indicates that there was a snake on this cell so player will have to jump 7 cells backwards.
	int board[MAX_POS] = {0};  
//initialize all cells in the array to 0, i.e. no snakes or ladders.  Snakes and ladders will be placed on board[] using function "setup"

	setup(n, players, die_sides, board, MAX_POS);

	const int BOARD_SIZE = pow(n,2);
	const int PLAYERS = players;
	const int DIE_SIDES = die_sides;

	//Initialize array to contain player positions.
	//For playerPos[0] = x, x represents the cell player1 is 
	//currently occupying.  Initially everyone starts at 
	//position 1 on the board, i.e. board[1]

	int playerPos[PLAYERS];
	for (int i = 0; i < PLAYERS; i++)
	{
		playerPos[i] = 1; //start everyone at cell 1
	}

	//keep track of whose turn it is. Start with player 1;
	int roundNum = 0; //Keep track of rounds played. A round is when player 0 starts her turn.
	int gameOver = 0; //Binary var for whether someone has won.
	while(gameOver == 0)
	{
		roundNum++;
		
		int whoseTurn = 0; // keep track of which player's turn it is.  

		//Starts a round.  Cycle through players in order, starting with player1.
		for (int i = 0; i < PLAYERS; i++)
		{
			roll(DIE_SIDES, playerPos, whoseTurn, board); //roll the die, see details in function below.
			if (playerPos[whoseTurn] >= BOARD_SIZE) //if the player who rolled reaches or passes the last square of the board, initialize gameover sequence.
			{
				cout <<"\n\nPLAYER " << whoseTurn + 1 << " IS THE WINNER!!!\n\n\n";
				cout << "Welp, that took "<< roundNum << " rounds, glad it's over. Here are the final rankings: \n\n";

				rank(playerPos, PLAYERS); //rank function sorts and couts players, positions, and rankings

				cout << "\n\n";
				return 0;
			}
			whoseTurn++; //if game isn't over, move to next person's turn.
		}
	}

	cout << endl;
	return 0;
}

//DEFINE FUNCTIONS

//1) SETUP
// n, players, and die_sides do not yet have values.  
// board[] is of size MAX_POS and has all elements initialized to 0.  
// board_size will have value of MAX_POS
void setup(int& n, int& players, int& die_sides, int board[], int board_size)
{
	/*
	Create the board & players.  Board will be an array where the
	array index represents the position (cell) on the board and its
	value represents how many cells forward or backwards a player
	will go if she lands on said cell. 
	*/

	//Initialize 3 arrays to store snakes/ladders, their positions, and their offsets
	//type[] contains whether an entry from the input file is a snake or a ladder
	//cell[] contains the position on the board where the snake/ladder is located
	//offset[] contains how far a player landing on a particular cell should move forward or backwards
	string type[board_size];
	int cell[board_size], offset[board_size]; 
	

	ifstream infile;
	infile.open("input.txt");

	int row = 0;

	while(infile.good())
	{
		//the first 3 rows of the input file have one element each which gives us the size of the board, # of players, and die sides.
		//we treat these elements as "row 0" of our file
		if (row == 0)
			infile >> n >> players >> die_sides;
		//after the first row start placing snake/ladder info into our 3 arrays
		else 
			infile >> type[row] >> cell[row] >> offset[row];

		row++;
	}

	infile.close();

	//Place snakes and ladders on board
	for(int i = 1; i < board_size; i++)
	{
		if (type[i] == "S")
			board[cell[i]] = -offset[i];
		else if (type[i] == "L")
			board[cell[i]] = offset[i];
	}
}

//2) CHECKFORTRAP - checks whether there is a snake or a ladder on a given cell
void checkForTrap(int board[], int playerPos[], int whoseTurn, bool& trap)
{
	//Evaluate if there's a snake or a ladder there, move accordingly, output appropriate message;
	//playerPos[whoseTurn] evaluates to the cell position of the player whose turn it is
	if (board[playerPos[whoseTurn]] < 0)
	// if there's a negative "offset" associated with a particular cell position, that means there's a snake so do the following:
	{
		cout << "However, there was a snake there so Player " << whoseTurn + 1 << " was moved down to cell " 
			 << playerPos[whoseTurn] + board[playerPos[whoseTurn]]<< ".  :("<< endl;
	}
	else if (board[playerPos[whoseTurn]] > 0)
	// if there's a positive "offset" associated with a particular cell position, that means there's a snake so do the following:
	{
		cout << "However, there was a ladder there so Player " << whoseTurn + 1 << " was moved up to cell " 
			 << playerPos[whoseTurn] + board[playerPos[whoseTurn]]<< "!!!  :)"<< endl;
	}
	else if (board[playerPos[whoseTurn]] == 0)
		trap = false;

	playerPos[whoseTurn] += board[playerPos[whoseTurn]]; //update the player's position
}

//3) ROLL - controls the die rolling behavior in the program
void roll(int die_sides, int playerPos[], int whoseTurn, int board[])
{
	int die = rand() % die_sides + 1; //add the 1 so we start at rolling a "1" rather than a "0"

	//You can have either 1 or 2 rolls, depending on whether you roll the highest die number
	int numRolls = 1;
	if (die == die_sides)
		numRolls = 2;

	for(int i = 1; i <= numRolls; i++)
	{
		if(i > 1) //if 2nd roll, roll the die again
			die = rand() % die_sides + 1;

		//Move whoever's turn it is up by whatever the die rolled;
		playerPos[whoseTurn] += die; 

		cout << "Player " << whoseTurn + 1 << " rolled a " << die << " and moved to cell " << playerPos[whoseTurn] <<endl;

		//check whether there's a snake or ladder associated with the cell a player lands on
		bool trap = true;
		while (trap == true)
		{
			checkForTrap(board, playerPos, whoseTurn, trap);
		}
	}
}

//4) MAX_ELEMENT - takes in an array of integers, returns the index of the largest element
int max_element(int array[], int arraySize)
{
	int max_element = 0;
	for(int i = 1; i < arraySize; i++)
	{
		if (array[max_element] < array[i])
		{
			max_element = i;
		}
	}
	return max_element;
}

//5) RANKS - outputs the rank of the players at the end of the game
void rank(int playerPos[], int players)
{
	for(int i= 0; i < players; i++)
	{
		int max = max_element(playerPos, players); //figure out who's in first place
		cout << "#" << i + 1 << "\tPlayer " << max + 1 << " with a score of: " << playerPos[max]<<endl; // output the player & position
		playerPos[max] = -1; //set that player's position to -1 so that the player is not included for future evaluations of "max_element"
	}
}



