/**
 * fifteen.c
 * Brandon Krepel
 * bmkrepel@gmail.com
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// coordinates to track blank tile
int x, y;

// winning board, whereby winboard[i][j] represents row i and column j
int winboard[MAX][MAX];

// prototypes
void clear(void);
void greet(void);
void init(void);
void swap(int* a, int* b);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(250000);
        }

        // sleep for animation's sake
        usleep(250000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(20000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    
    int counter = 1;
    
    // create 2D array of (d^2-1) tiles in order from highest to lowest
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = (d * d - 1) - j - (i * d);
            // printf("board[%i][%i] = %i\n", i, j, board[i][j]);
            winboard[i][j] = counter;
            counter++;
        }
    }
    // initialize winboard's lower left corner to zero
    winboard[d - 1][d- 1] = 0;
    
    // if odd number of tiles, swap #1 and #2    
    if(d % 2 != 1)
    {
        swap(&board[d - 1][d - 2], &board[d - 1][d - 3]);
    }
}

/**
 * implements swap function
 */
void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Draw board with 'd' columns and rows
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // print _ for blank tile and remember coordinates
            if (board[i][j] == 0)
            {
                printf(" _  ");
            
                x = i;
                y = j;
            }
            else
            printf("%2i  ", board[i][j]);
        }

        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Search for tile's position
    for (int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == tile)
            {
                // if tile is adjacent to blank, return true    
                if( (i == x - 1 && j == y) ||
                    (i == x + 1 && j == y) ||
                    (i == x && j == y - 1) ||
                    (i ==x && j == y + 1) )
                {
                    // swap tile with blank
                    swap(&board[i][j], &board[x][y]);
                    
                    // remember new location of blank tile
                    x = i;
                    y = j;
                    return 1;
                }
                // return false for invalid move
                else
                return 0;
            }
        }
    } 
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    
    int wincheck = 0;
    // iterate through array
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // for each tile in the proper location, increase win counter
            if(board[i][j] == winboard[i][j])
            {
                wincheck++;
            }    
            
            // if win counter is equal to number of tiles, return 1
            if(wincheck == (d * d))
            {
                return 1;
            }            
        }
    }
    return false;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
