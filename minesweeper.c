// Assignment 1 21T2 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by Gabriel Esquivel (z5358503)
// on July, 2021
//
// Version 1.0.0 (2021-06-19): Assignment released.

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE            8

// The possible command codes.
#define DETECT_ROW      1
#define DETECT_SQUARE   2
#define REVEAL_CROSS    3
#define GAME_MODE       4

// Add any extra #defines here.
#define TOTAL_VALUES    64
#define RESTRICT_MAX    3

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int mode_detector, int minefield[SIZE][SIZE]);

// Place your function prototyes here.

void detect_row_function(int restrict_hints[1], int mode_detector, int detect_row, int col_start, int col_end, int minefield[SIZE][SIZE]);
void detect_square_function(int restrict_hints[1], int mode_detector, int row, int col, int size, int minefield[SIZE][SIZE]);
void reveal_cross(int reveal_row_centre, int reveal_cross_centre, int game_win[1], int mode_detector, int minefield[SIZE][SIZE]);
int reveal_cross_detect(int row, int col, int size, int minefield[SIZE][SIZE]);
void gamemode_switch(int game_win[1], int minefield[SIZE][SIZE]);

int main(void) {

    int minefield[SIZE][SIZE];
    int mode_detector = 0;

    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");

    //Scan in the amount of mines the user will input.
    int n_mines;
    scanf ("%d", &n_mines);

    printf("Enter pairs:\n");

    //Declare variables.
    int loop = 0;
    int i = 0;
    int j = 0;
   
    //Rows
    int row[n_mines];
    
    // Columns
    int column[n_mines];
    
    //Scan in location of the mines declared prior.
    while (loop < n_mines) {
        
        scanf ("%d %d", &row[i], &column[j]);
        i++;
        j++;
        loop++;
    }
    
    
    //Place mines on grid
    
    i = 0;
    j = 0;
    loop = 0;
    
    while (loop < n_mines) {
    
        if (row[i] > -1 && row[i] < SIZE && column[j] > -1 && column[j] < SIZE) {
            minefield[row[i]][column[j]] = HIDDEN_MINE;
        }
        
        i++;
        j++;
        loop++;
    }

    printf("Game Started\n");
    print_debug_minefield(mode_detector, minefield);
    
    //Declare variables for while loop
    int command;
    int restrict_hints[1] = {0};    
    int game_win[1] = {0};
    int restrictor1 = 0;
    int restrictor2 = 0;
    
    //Program ceases if user enters EOF
    while ((scanf ("%d", &command)) == 1) {
    
        // Detecting mines in a row
        if (command == DETECT_ROW && restrict_hints[0] < RESTRICT_MAX) {
        
            int detect_row;
            int col_start;
            int cols_check;
    
            scanf ("%d %d %d", &detect_row, &col_start, &cols_check);
            
            detect_row_function(restrict_hints, mode_detector, detect_row, col_start, cols_check, minefield);    
                        
        } else if (command == DETECT_ROW && restrict_hints[0] == RESTRICT_MAX && restrictor1 == 0) {
            printf ("Help already used\n");
            print_debug_minefield(mode_detector, minefield);
            restrictor1++;
            
        }
        
        //Detect mines in a square
        if (command == DETECT_SQUARE && restrict_hints[0] < RESTRICT_MAX) {
        
            int row_centre;
            int col_centre;
            int square_size;
  
            scanf ("%d %d %d", &row_centre, &col_centre, &square_size);
        
            detect_square_function(restrict_hints, mode_detector, row_centre, col_centre, square_size, minefield);

        } else if (command == DETECT_SQUARE && restrict_hints[0] == RESTRICT_MAX && restrictor2 == 0) {
            printf ("Help already used\n");
            print_debug_minefield(mode_detector, minefield);
            restrictor2++;
        }
            
        
        //Reveal cross
        else if (command == REVEAL_CROSS) {
        
            int reveal_row_centre;
            int reveal_col_centre;
    
            scanf ("%d %d", &reveal_row_centre, &reveal_col_centre);
            
            reveal_cross(reveal_row_centre, reveal_col_centre, game_win, mode_detector, minefield);
        
        }
        
        //Game mode switch
        if (command == GAME_MODE) {
        
            if (mode_detector == 0) {
            
                mode_detector++;
                printf ("Gameplay mode activated\n");
                
                print_debug_minefield(mode_detector, minefield);

            } else if (mode_detector == 1) {
            
                mode_detector--;
                printf ("Debug mode activated\n");
                print_debug_minefield(mode_detector, minefield);
                
            }   
                   
            //Game status checker. If the game hasn't been won, it continues.
        
            int loop_status1 = 0;
            int loop_status2 = 0;
            int status_checker = 0;

            while (loop_status1 < SIZE) {
        
                while (loop_status2 < SIZE) {
            
                    //Checks each position for the absence of a mine.
                    if (minefield[loop_status1][loop_status2] != HIDDEN_MINE) {
                    
                        status_checker++;
                    }
                    loop_status2++;
                }
            
                loop_status1++;
     
            }
        
            // If there are no mines, then the game is won.
            if (status_checker == TOTAL_VALUES) {
        
                game_win[0] = 1;
            }
        
        
        }
    
        if (game_win[0] == 1) {
        
            gamemode_switch(game_win, minefield);
        
            return 0;
        }
    }
             
    return 0;
}

// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}

// Print out the actual values of the minefield.
void print_debug_minefield(int mode_detector, int minefield[SIZE][SIZE]) {
   
    if (mode_detector == 0) {
        int i = 0;
        while (i < SIZE) {
            int j = 0;
            while (j < SIZE) {
                printf("%d ", minefield[i][j]);
                j++;
            }
            printf("\n");
            i++;
        }
    } else if (mode_detector == 1) {    
    
        int game_win = 0;
        gamemode_switch(&game_win, minefield);  
    }
}

// Detect row function
void detect_row_function(int restrict_hints[1], int mode_detector, int detect_row, int col_start, int cols_check, int minefield[SIZE][SIZE]) {
 
    int n_detects = 0;
    int loop_detect = 0;
    
    //Check each position in row for a mine. If found, add to sum.
    if (col_start + cols_check <= SIZE && detect_row < SIZE && detect_row > -1 && col_start > -1 && cols_check > -1) {        
        while (col_start + loop_detect < col_start + cols_check) {
            
            if (minefield[detect_row][col_start + loop_detect] == HIDDEN_MINE) {
                n_detects++;
            }
            loop_detect++;
                
        }
    
        printf ("There are %d mine(s) in row %d, from column %d to %d\n", n_detects, detect_row, col_start, col_start + cols_check - 1);
        print_debug_minefield(mode_detector, minefield);
        restrict_hints[0]++;
        
    } else {
        printf ("Coordinates not on map\n");
        print_debug_minefield(mode_detector, minefield);
    }
}

// Detect square function
void detect_square_function(int restrict_hints[1], int mode_detector, int row, int col, int size, int minefield[SIZE][SIZE]) {

    int n_detects = 0;
    
    // Determines how far the edge of the square is from the centre
    int disp = (size - 1) / 2;
     
    // Determines the starting position to check the square for mines   
    int row_counter = row - disp;
    int col_counter = col - disp;
    
    if (row > -1 && row < SIZE && col > -1 && col < SIZE) {
        
        while (col_counter >= (col - disp) && col_counter <= (col + disp)) {
          
            row_counter = row - disp;
            
            while (row_counter >= (row - disp) && row_counter <= (row + disp)) {             
            
                if (row_counter > -1 && row_counter < SIZE && col_counter > -1 && col_counter < SIZE && minefield[row_counter][col_counter] == HIDDEN_MINE) {
                    n_detects++;
                }
                
                row_counter++;
            }
            col_counter++;
        }   
    
        printf ("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", n_detects, row, col, size); 
        print_debug_minefield(mode_detector, minefield);
        restrict_hints[0]++;
    
    } else {
    
        printf ("Coordinates not on map\n");
        print_debug_minefield(mode_detector, minefield);
        
    }
    
}

// Reveal Cross Function
void reveal_cross(int reveal_row_centre, int reveal_col_centre, int game_win[1], int mode_detector, int minefield[SIZE][SIZE]) {
    #define REVEAL_SIZE 3
    
    if (reveal_row_centre > -1 && reveal_row_centre < SIZE && reveal_col_centre > -1 && reveal_row_centre < SIZE) { 

        // If the reveal cross centre is on a mine, the game has been lost.
        if (minefield[reveal_row_centre][reveal_col_centre] == HIDDEN_MINE) {
      
            game_win[0] = 1;
        
        }
    
        if (minefield[reveal_row_centre][reveal_col_centre] != HIDDEN_MINE) {   
            minefield[reveal_row_centre][reveal_col_centre] = VISIBLE_SAFE;      
        }
          
        //The following if chain checks the adjacent squares for mines.  
        if (reveal_cross_detect(reveal_row_centre + 1, reveal_col_centre, REVEAL_SIZE, minefield) == 0 && minefield[reveal_row_centre + 1][reveal_col_centre] != HIDDEN_MINE) {    
            minefield[reveal_row_centre + 1][reveal_col_centre] = VISIBLE_SAFE; 
        }
    
        if (reveal_cross_detect(reveal_row_centre - 1, reveal_col_centre, REVEAL_SIZE, minefield) == 0 && minefield[reveal_row_centre - 1][reveal_col_centre] != HIDDEN_MINE) {    
            minefield[reveal_row_centre - 1][reveal_col_centre] = VISIBLE_SAFE; 
        } 
        
        if (reveal_cross_detect(reveal_row_centre, reveal_col_centre + 1, REVEAL_SIZE, minefield) == 0 && minefield[reveal_row_centre][reveal_col_centre + 1] != HIDDEN_MINE) {
            minefield[reveal_row_centre][reveal_col_centre + 1] = VISIBLE_SAFE;     
        }
        
        if (reveal_cross_detect(reveal_row_centre, reveal_col_centre - 1, REVEAL_SIZE, minefield) == 0 && minefield[reveal_row_centre][reveal_col_centre - 1] != HIDDEN_MINE) {
            minefield[reveal_row_centre][reveal_col_centre - 1] = VISIBLE_SAFE;     
        }
    
        if (game_win[0] == 0) {
            print_debug_minefield(mode_detector, minefield);
        }
    
    } else {
    
        printf ("Coordinates not on map\n");
        print_debug_minefield(mode_detector, minefield);
    
    } 
    
}

// Reveal Cross Detect. The following code checks the 3x3 region around the position for mines. It then returns the amount of mines detected.
int reveal_cross_detect(int row, int col, int size, int minefield[SIZE][SIZE]) {
    int disp = (size - 1) / 2;
        
    int n_detects = 0;
    
    // Determines the starting position for the 3 x 3 square
    int row_counter = row - disp;
    int col_counter = col - disp;

    
    if (col_counter == -1 || col_counter == SIZE) {
        return 1;
    }
    
    if (row_counter == -1 || row_counter == SIZE) {
        return 1;
    }
          
    while (col_counter >= (col - disp) && col_counter <= (col + disp) && col_counter > -1 && col_counter < SIZE) {
          
        row_counter = row - disp;
            
        while (row_counter >= (row - disp) && row_counter <= (row + disp) && row_counter > -1 && row_counter < SIZE) {             
            
            
            if (minefield[row_counter][col_counter] == HIDDEN_MINE) {
                n_detects++;
            }
                
            row_counter++;
        }
        col_counter++;
    }    
    return n_detects;
}
        
void gamemode_switch(int game_win[1], int minefield[SIZE][SIZE]) {

    int size = 3;
    int gamemode_row = 0;
    
    // If the game is continuing, print the following.
    if (game_win[0] == 0) {
        printf ("..\n");
        printf ("\\/ \n");
    }
    
    // If the game has ended, print the following.
    if (game_win[0] == 1) {
        printf ("Game over\n");
        printf ("xx\n");
        printf ("/\\\n");
    }
    
    // The folllowing code formats the matrix in game mode.
    printf ("   00 01 02 03 04 05 06 07\n");
    printf ("   -------------------------\n");
    printf ("00 |");
    
    int loop_zero = 0;
    while (loop_zero < SIZE) {
        
        if (minefield[gamemode_row][loop_zero] == HIDDEN_SAFE) { 
            printf ("##");
            if (loop_zero < SIZE - 1) {
                printf (" ");
            }
        } else if (minefield[gamemode_row][loop_zero] == HIDDEN_MINE && game_win[0] == 0) {
            printf ("##");
            if (loop_zero < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_zero, size, minefield) == 0 && minefield[gamemode_row][loop_zero] == VISIBLE_SAFE ) {
            printf ("  ");
            if (loop_zero < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_zero, size, minefield) > 0 && minefield[gamemode_row][loop_zero] == VISIBLE_SAFE) {
                
            printf ("0%d", reveal_cross_detect(gamemode_row, loop_zero, size, minefield));
            if (loop_zero < SIZE - 1) {
                printf (" ");
            }
        } else {
            printf ("()");
            if (loop_zero < SIZE - 1) {
                printf (" ");
            }
        }
            
        loop_zero++;
    }
    printf ("|\n");
    
    printf ("01 |");
    
    // Detects the nature of each square and determines it's translation.
    gamemode_row++;
    int loop_one = 0;
    while (loop_one < SIZE) {
        if (minefield[gamemode_row][loop_one] == HIDDEN_SAFE) {
            printf ("##");
            if (loop_one < SIZE - 1) {
                printf (" ");
            }
        } else if (minefield[gamemode_row][loop_one] == HIDDEN_MINE && game_win[0] == 0) {
            printf ("##");
            if (loop_one < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_one, size, minefield) == 0 && minefield[gamemode_row][loop_one] == VISIBLE_SAFE) {
            printf ("  ");
            if (loop_one < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_one, size, minefield) > 0 && minefield[gamemode_row][loop_one] == VISIBLE_SAFE) {
                
            printf ("0%d", reveal_cross_detect(gamemode_row, loop_one, size, minefield));
            if (loop_one < SIZE - 1) {
                printf (" ");
            }  
        } else {
            printf ("()");
            if (loop_one < SIZE - 1) {
                printf (" ");
            }
        }
        
        loop_one++;
    }
    printf ("|\n");
        
    printf ("02 |");
    
    gamemode_row++;
    int loop_two = 0;
    while (loop_two < SIZE) {
        if (minefield[gamemode_row][loop_two] == HIDDEN_SAFE) {
            printf ("##");
            if (loop_two < SIZE - 1) {
                printf (" ");
            }
        } else if (minefield[gamemode_row][loop_two] == HIDDEN_MINE && game_win[0] == 0) {
            printf ("##");
            if (loop_two < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_two, size, minefield) == 0 && minefield[gamemode_row][loop_two] == VISIBLE_SAFE) {
            printf ("  ");
            if (loop_two < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_two, size, minefield) > 0 && minefield[gamemode_row][loop_two] == VISIBLE_SAFE) {
                
            printf ("0%d", reveal_cross_detect(gamemode_row, loop_two, size, minefield));
            if (loop_two < SIZE - 1) {
                printf (" ");
            }  
        }  else {
            printf ("()");
            if (loop_two < SIZE - 1) {
                printf (" ");
            }
        }
        loop_two++;
    }
    printf ("|\n");
        
    printf ("03 |");
    
    gamemode_row++;
    int loop_three = 0;
    while (loop_three < SIZE) {
        if (minefield[gamemode_row][loop_three] == HIDDEN_SAFE) {
            printf ("##");
            if (loop_three < SIZE - 1) {
                printf (" ");
            }
        } else if (minefield[gamemode_row][loop_three] == HIDDEN_MINE && game_win[0] == 0) {
            printf ("##");
            if (loop_three < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_three, size, minefield) == 0 && minefield[gamemode_row][loop_three] == VISIBLE_SAFE) {
            printf ("  ");
            if (loop_three < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_three, size, minefield) > 0 && minefield[gamemode_row][loop_three] == VISIBLE_SAFE) {
                
            printf ("0%d", reveal_cross_detect(gamemode_row, loop_three, size, minefield));
            if (loop_three < SIZE - 1) {
                printf (" ");
            }  
        }  else {
            printf ("()");
            if (loop_three < SIZE - 1) {
                printf (" ");
            }
        }
        loop_three++;
    }
    printf ("|\n");
        
    printf ("04 |");
    
    gamemode_row++;
    int loop_four = 0;
    while (loop_four < SIZE) {
        if (minefield[gamemode_row][loop_four] == HIDDEN_SAFE) {
            printf ("##");
            if (loop_four < SIZE - 1) {
                printf (" ");
            }
        } else if (minefield[gamemode_row][loop_four] == HIDDEN_MINE && game_win[0] == 0) {
            printf ("##");
            if (loop_four < SIZE - 1) {
                printf (" ");
            }           
        } else if (minefield[gamemode_row][loop_four] == VISIBLE_SAFE) {
            printf ("  ");
            if (loop_four < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_four, size, minefield) > 0 && minefield[gamemode_row][loop_four] != HIDDEN_MINE) {
                
            printf ("0%d", reveal_cross_detect(gamemode_row, loop_four, size, minefield));
            if (loop_four < SIZE - 1) {
                printf (" ");
            }
        } else {
            printf ("()");
            if (loop_four < SIZE - 1) {
                printf (" ");
            }
        }
        
        loop_four++;
    }
    printf ("|\n");
        
    printf ("05 |");
    
    gamemode_row++;
    int loop_five = 0;
    while (loop_five < SIZE) {
        if (minefield[gamemode_row][loop_five] == HIDDEN_SAFE) {
            printf ("##");
            if (loop_five < SIZE - 1) {
                printf (" ");
            }
        } else if (minefield[gamemode_row][loop_five] == HIDDEN_MINE && game_win[0] == 0) {
            printf ("##");
            if (loop_five < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_five, size, minefield) == 0 && minefield[gamemode_row][loop_five] == VISIBLE_SAFE) {
            printf ("  ");
            if (loop_five < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_five, size, minefield) > 0  && minefield[gamemode_row][loop_five] == VISIBLE_SAFE) {
                
            printf ("0%d", reveal_cross_detect(gamemode_row, loop_five, size, minefield));
            if (loop_five < SIZE - 1) {
                printf (" ");
            }
        } else {
            printf ("()");
            if (loop_five < SIZE - 1) {
                printf (" ");
            }
        }
        
        loop_five++;
    }
    printf ("|\n");
        
    printf ("06 |");
    
    gamemode_row++;
    int loop_six = 0;
    while (loop_six < SIZE) {
        if (minefield[gamemode_row][loop_six] == HIDDEN_SAFE) {
            printf ("##");
            if (loop_six < SIZE - 1) {
                printf (" ");
            }
        } else if (minefield[gamemode_row][loop_six] == HIDDEN_MINE && game_win[0] == 0) {
            printf ("##");
            if (loop_six < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_six, size, minefield) == 0 && minefield[gamemode_row][loop_six] == VISIBLE_SAFE) {
            printf ("  ");
            if (loop_six < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_six, size, minefield) > 0 && minefield[gamemode_row][loop_six] == VISIBLE_SAFE) {
                
            printf ("0%d", reveal_cross_detect(gamemode_row, loop_six, size, minefield));
            if (loop_six < SIZE - 1) {
                printf (" ");
            }    
        } else {
            printf ("()");
            if (loop_six < SIZE - 1) {
                printf (" ");
            }
        }
        
        loop_six++;
    }
    printf ("|\n");
        
    printf ("07 |");
    
    gamemode_row++;
    int loop_seven = 0;
    while (loop_seven < SIZE) {
        if (minefield[gamemode_row][loop_seven] == HIDDEN_SAFE) {
            printf ("##");
            if (loop_seven < SIZE - 1) {
                printf (" ");
            }
        } else if (minefield[gamemode_row][loop_seven] == HIDDEN_MINE && game_win[0] == 0) {
            printf ("##");
            if (loop_seven < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_seven, size, minefield) == 0 && minefield[gamemode_row][loop_seven] == VISIBLE_SAFE) {
            printf ("  ");
            if (loop_seven < SIZE - 1) {
                printf (" ");
            }
        } else if (reveal_cross_detect(gamemode_row, loop_seven, size, minefield) > 0 && minefield[gamemode_row][loop_seven] == VISIBLE_SAFE) {
                
            printf ("0%d", reveal_cross_detect(gamemode_row, loop_seven, size, minefield));
            if (loop_seven < SIZE - 1) {
                printf (" ");
            }
        } else {
            printf ("()");
            if (loop_seven < SIZE - 1) {
                printf (" ");
            }
        }
        
        loop_seven++;
    }
    printf ("|\n");
        
    printf ("   -------------------------\n");
}

