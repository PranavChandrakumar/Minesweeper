#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define SIZE 10
#define BOMBS 15

void Fill_Board(char board[SIZE][SIZE]){ //Function that fills the supplied array with -
    
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = '-';
        }
    }
}

void Bomb_Placement(char board[SIZE][SIZE]){ //Function that randomly fills the supplied array with bombs
    time_t t; //Initializing time variable for an srand() seed
    srand((unsigned)(char)time(&t)); //Using the time variable to generate a set of random numbers
    int bomb_count = 0;
    while(bomb_count < BOMBS){
        int num1 = rand() % (9 + 1); //Sets range of random integer between 0 and 9 (inclusive), learned this from stack overflow
        int x_coord = num1;

        int num2 = rand() % (9 + 1);
        int y_coord = num2;

        if (board[x_coord][y_coord] != 'X')
        {
            board[x_coord][y_coord] = 'X';
            bomb_count++;
        }
    }
}

void Print_Board(char board[SIZE][SIZE]){
    printf("\n  ");
    for (int c = 0; c < SIZE; c++)
    {
        printf("%d ", c); //Prints out the column headers
    }
    printf("\n");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", i); //Prints out the row headers
        for (int j = 0; j < SIZE; j++)
        {
            printf("%c ", board[i][j]); //Prints out the ith row and jth column value of the supplied array
        }
        printf("\n");
    }
    printf("\n");
}

void Count_Adjacent_Bombs(char board[SIZE][SIZE]){ //Function which iterates through each cell in the array, and counts bombs in surrounding cells cells
    for (int i = 0; i < SIZE; i++){ //Loops for each cell in the array
        for (int j = 0; j < SIZE; j++){
            char num_bombs = '0'; //Initialize counter for the number of bombs in surrounding cells
            for(int x = -1; x <= 1; x++){ //Loops through the index adjustments for surrounding cells
                for(int y = -1; y <= 1; y++){
                    int x_pos = i + x; //Cell coordinate for the specific cell being scanned
                    int y_pos = j + y;
                    if (x_pos >= 0 && x_pos <= (SIZE -1) && y_pos >= 0 && y_pos <= (SIZE - 1) && board[x_pos][y_pos] == 'X'){ //Conditions to ensure the index is in range, and the cell contains a bomb
                        num_bombs++; //Increasing the counter by one for each bomb detected
                    }
                }
            }
            if(board[i][j] != 'X'){ //Each cell should only be a number reflecting the number of surrounding bombs if and only if the cell itself is not a bomb
                board[i][j] = num_bombs;
            }
        }
    }
}

bool Input_Validity(char input[20]){ //Function to determine the validity of the input by the user
    int length = strlen(input);
    if(length == 1 && input[0] == 'q'){ // Quit condition
        return true;
    }
    else if (length == 3 && (isdigit(input[0]) && isdigit(input[2]) && input[1] == ' ')){ //Coordinate condition
        return true;
    }
    else{
        return false;
    }
}


void Play_Game(char main_board[SIZE][SIZE], char player_board[SIZE][SIZE]){ //Function which controls game logic
    printf("Welcome to Minesweeper!\n");
    Print_Board(player_board);
    int choice_count = 0; //Active counter variable which contains the number of correct cells chosen

    while (choice_count < SIZE * SIZE - BOMBS)
    {
        char input[20]; //Variable which is used to store the user input, buffer size of 20
        printf("Enter the coordinates of a cell (x y), seperated by a space, or press q to quit: ");

        scanf(" %[^\n]",input); //scanf command which uses a unique placeholder to count ALL characters until a newline is detected, learned this from chatGPT

        if(Input_Validity(input)){ //Execution of game logic is conditioned on input validity
            if(input[0] == 'q'){ //Game exit condition
                printf("Exiting the game.\n");
                break;
            }

            int x = input[0] - '0'; //Converts character integer number into integer number, needed for array indexing
            int y = input[2] - '0';

            if ((x >= 0 && x <= (SIZE - 1)) && (y >= 0 && y <= (SIZE - 1))){ //Condition to check if index values are within range
                if (player_board[x][y] != '-') //Condition to check if the cell has already been revealed
                {
                    printf("Cell has already been revealed.\n\n");
                }
                else
                {
                    player_board[x][y] = main_board[x][y];
                    if (player_board[x][y] == 'X') //Condition to check if the selected cell is a bomb
                    {
                        printf("Game Over! A Bomb has been detonated.\n");
                        for (int rows = 0; rows < SIZE; rows++) //Loop to find the positions of all bombs and display them
                        {
                            for (int col = 0; col < SIZE; col++)
                            {
                                if (main_board[rows][col] == 'X')
                                {
                                    player_board[rows][col] = 'X';
                                }
                            }
                        }
                        Print_Board(player_board);
                        break;
                    }
                    else //Executes if and only if all other game conditions are satisfied
                    {
                        choice_count++;
                        Print_Board(player_board);
                    }
                }
            }
            else{ //Executes if the index is out of range. Useless for 10x10 grid, as this while loop doesn't accept negative numbers or numbers greater than 10.
                printf("Input is invalid. The index is out of range.\n");
            }
        }
        else{ //Executes if the input validation function returns false
            printf("Input is invalid. Invalid data type, the index is out of range, or there is a spacing error in your input.\n\n");
        }
    }
    if (choice_count == SIZE * SIZE - BOMBS) //Condition which executes if all cells have been revealed that do not contain bombs
    {
        printf("Congratulations! You have successfully located all bombs\n");
        Print_Board(main_board);
    }
}

int main(){
    char main_array[SIZE][SIZE]; //Defining main game board, which is always hideen from the player
    char player_array[SIZE][SIZE]; //Defining player board, which is updated throughout the game loop

    Fill_Board(main_array); //Fill the main board with - in each cell
    Fill_Board(player_array); //Fill the player board with - in each cell
    Bomb_Placement(main_array); //Fill the main board with randomly placed bobs
    Count_Adjacent_Bombs(main_array); //Fill the main board with numbers which reflect the number of bombs in surrounding cells

    Play_Game(main_array, player_array); //Main game loop function

    return 0;
}
