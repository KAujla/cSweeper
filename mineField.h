
/*mineField.c
 *
 *base struct for the game field  and related functions
 *
 * Created by Karan Aujla on 2017/09/03
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

struct mineField {

    int height;//how many rows there are
    int width;//how many columns
    int numOfMines;//how many mines
    int gameLost;//stores whether the game is over
    int debug;
    int** field;//the 2-d array storing the mine locations and numbers
    int** checked;//stores whether each cell is revealed or not
    int remainingCells; //keeps track of the number of non mine cells that have not been revealed
                        //once it reaches 0, the game is won


};
//creates a mineField using the info in the input array
//the input array should only contain 3 ints:
//the height of the mineField which is between 0 & 100,
//the width of the mineField which is between 0 & 100, and
//the number of mines in the field which is between 0 & width*height,
//all stored in that order
struct mineField* mf_createField(int*  setUpInfo);

//for debugging, prints the raw values in the mineField in a row
void mf_printRawField(struct mineField* inputField);

void mf_revealZeroes(struct mineField* playField, int* inputCoords);

int mf_validatePoint(struct mineField* playField, int* inputCoords);

void mf_gameLost(struct mineField* playField);

//deconstructor
void mf_destroyField(struct mineField *deadField);
