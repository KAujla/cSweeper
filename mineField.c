
/*mineField.c
 *
 *base struct for the game field  and related functions
 *
 * Created by Karan Aujla on 2017/09/03
*/

#include "mineField.h"

//creates a mineField using the info in the input array
//the input array should only contain 3 ints:
//the height of the mineField,
//the width of the mineField, and
//the number of mines in the field,
//all stored in that order
struct mineField* mf_createField(int  setUpInfo[3]){
    struct mineField* playField = malloc(sizeof(struct mineField));

    //set the received info
    playField->height = setUpInfo[0];
    playField->width = setUpInfo[1];

    //if the inputted number of mines is -1 then debug mode is activated
    if (setUpInfo[2] == -1){
        playField->debug = 1;
        playField->numOfMines = 4;
    } else{
        playField->numOfMines = setUpInfo[2];
        playField->debug = 0;
    }

    playField->gameLost = 0;

    //we only want to reveal the cells that are not mines
    playField->remainingCells = playField->height*playField->width - playField->numOfMines;

    playField->field = (int **) malloc(sizeof(int*) * playField->height);


    //confirm that the outer array was set correctly
    assert(playField->field != NULL);

    //allocate each of the inner arrays
    for (int i = 0; i < playField->height; i++) {

        playField->field[i] =(int *) malloc(sizeof(int*) * playField->width);
        assert(playField->field[i] != NULL);

    }
    
    //populate the field with mines

    //first seed the rng using the current time
    //this ensures better randomization
    time_t currentTime;
    unsigned int seed = (unsigned) time(&currentTime);
    srand(seed);



    int randomCol = 0;
    int randomRow = 0;
    //only generate randomly if debug mode is not active
    if (playField->debug == 1){
        playField->field[1][1] = -1;
        playField->field[1][4] = -1;
        playField->field[3][1] = -1;
        playField->field[3][3] = -1;


    } else{
        //generate a random row and column of a mine and place it in there
        //do this numOfMines times
        for (size_t i = 0; i < playField->numOfMines; i++) {
            randomRow = rand() % playField->height;
            randomCol = rand() % playField->width;
            //using -1 to represent a mine, place on in the generated col and row
            playField->field[randomRow][randomCol] = -1;

        }

    }
    //go through the playfield and sum the number of mines surrounding a cell
    int surroundingMines = 0;

    for (int i = 0; i < playField->height; i++) {
        for (int j = 0; j < playField->width; j++) {
            //reset the surroundingMines counter for every cell
            surroundingMines = 0;
            //only check if the current cell does not contain a mine
            if (playField->field[i][j] != -1) {

                //look at the surrounding cells and tally up the mines
                for (int x = -1; x < 2; x++) {
                    for (int y = -1; y < 2; y++) {

                        //if the cell is out of bounds in any direction

                        if ( i+x >= 0 && j+y >= 0
                            && j+y <= playField->width-1
                            && i+x <= playField->height-1 ) {


                            if(playField->field[i+x][j+y] == -1){

                                surroundingMines++;
                            }
                        }

                    }
                }
                //store the surroundingMines in the current cell
                playField->field[i][j] = surroundingMines;
            }
        }
    }

    //create an the 2d array to check if the cells are revielled
    playField->checked = malloc(sizeof(int*) * playField->height);

    for (int i = 0; i < playField->height; i++) {
        playField->checked[i] = malloc(sizeof(int) * playField->width);
        for (int j = 0; j < playField->width; j++) {
            playField->checked[i][j] = 0;
        }
    }

    return playField;
}


void mf_revealZeroes(struct mineField* playField, int* inputCoords){

    //variable to store the values
    int cellValue = 0;

    //store the initial x and y to make recursion easier
    int x = inputCoords[1];
    int y = inputCoords[0];
    //see if the cell is actually inside the field
    if ((0 <= y && y < playField->width) &&
        (0 <= x && x < playField->height)) {
        //only store the value if it is within bounds
        cellValue = playField->field[y][x];
        //if the cell hasn't been revealed already


        if (playField->checked[y][x] == 0) {
            //reveal the cell
            playField->checked[y][x] = 1;
            //and count it
            playField->remainingCells--;
        } else{
            //just return
            return;
        }
        //only continue if the cell contains a 0
        if (cellValue == 0) {
            //recurse on all adjacent cells

            inputCoords[0] = y + 1;
            inputCoords[1] = x;
            mf_revealZeroes(playField, inputCoords);

            inputCoords[1] = x + 1;
            mf_revealZeroes(playField, inputCoords);

            inputCoords[0] = y - 1;
            inputCoords[1] = x;
            mf_revealZeroes(playField, inputCoords);

            inputCoords[1] = x - 1;
            mf_revealZeroes(playField, inputCoords);


            inputCoords[0] = y;
            inputCoords[1] = x + 1;
            mf_revealZeroes(playField, inputCoords);

            inputCoords[0] = y - 1;
            mf_revealZeroes(playField, inputCoords);


            inputCoords[0] = y;
            inputCoords[1] = x - 1;
            mf_revealZeroes(playField, inputCoords);

            inputCoords[0] = y + 1;
            mf_revealZeroes(playField, inputCoords);


        }
        //otherwise it is the base case so return
        return;
    }




}

//checks if the given coordinates are valid and sets the corresponding cell in checked to 1
//assumes that inputCoords has only 2 indices
//if not then it returns 0
//coordinate order: [x, y]
int mf_validatePoint(struct mineField* playField, int* inputCoords){
    int validCoord = 0;


    if ((0 <= inputCoords[0] && inputCoords[0] < playField->width) &&
        (0 <= inputCoords[1] && inputCoords[1] < playField->height)) {

        //check if the cell has already been revealed
        if ( playField->checked[inputCoords[1]][inputCoords[0]] == 1) {
            validCoord = -1;
            return validCoord;
        }

        //if the coordinate contains a zero,
        //run revealZeroes to find all adjacent zeroes
        if ( playField->field[inputCoords[1]][inputCoords[0]] == 0) {
            mf_revealZeroes(playField, inputCoords);
        }
        //if the coordinates are valid, set the corresponding cell in checked to 1 (revealed)
        playField->checked[inputCoords[1]][inputCoords[0]] = 1;

        //once it is reveal, decrement the remaining cells
        playField->remainingCells--;

        //check if the cell has a mine, if so the game is lost
        if (playField->field[inputCoords[1]][inputCoords[0]] == -1) {
            playField->gameLost = 1;
            //call the game lost functions
            mf_gameLost(playField);
        }
        validCoord = 1;
    } else {
        validCoord = 0;
    }

    return validCoord;
}

//called when the game is lost to update the necessary information
void mf_gameLost(struct mineField* playField){
    if (playField->gameLost == 1) {
        //if it is lost, reveal all the cells
        for (int i = 0; i < playField->height; i++) {
            for (int j = 0; j < playField->width ; ++j) {
                playField->checked[i][j] = 1;
            }
        }

    }
    //return;
}


//debugging function
void mf_printRawField(struct mineField* inputField) {
    if (inputField->debug == 1){
        printf("debug is on\n");
    } else{
        printf("debug is off\n");
    }
    printf("height is     %d\n", inputField->height );
    printf("width is      %d\n", inputField->width );
    printf("numOfMines is %d\n", inputField->numOfMines );
    if (inputField->gameLost == 1){
        printf("gameLost is true\n" );
    }else{
        printf("gameLost is false\n");
    }
    for (int i = 0; i < inputField->height; i++) {
        for (int j = 0; j < inputField->width; j++) {
            printf("(%d,%d) = %d, checked =  %d \n",i, j,
             inputField->field[i][j], inputField->checked[i][j] );
            /* code */
        }
    }
    printf("\n" );
}
void mf_destroyField(struct mineField *deadField){
     for (int i = 0; i < deadField->height; i++) {
         free(deadField->field[i]);
         free(deadField->checked[i]);
     }
    free(deadField->field);
    free(deadField->checked);
    deadField->width = 0;
    deadField->height = 0;
    deadField->numOfMines = 0;
    free(deadField);
}
