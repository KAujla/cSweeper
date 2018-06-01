
/*cSweeperMain.c
 * Driver file containing the main function
 *
 *
 * TODO:
 * 2. rewrite the coordinate input to clear any extra values
 * 3. add a custom game functionality (most likely requires 2.)
*/

#include "mineFieldCommandIO.h"

int main(int argc, char const *argv[]) {
    //setting up initial values
    int easy[3] = {10, 10, 7};
    int medium[3] = {25, 25, 35};
    int hard[3] = {100, 100, 200};
    //int debug[3] = {7, 7, -1}; a known field, for debugging purposes

    int*   coordinateInput      = malloc(2* sizeof(int));// an array contain the cell that the user chooses
    struct mineField* gameField = NULL;
    int    continuePlaying      = 1;// controls whether the game loops after a game over
    char   continueInput           ;// will contain users choice of whether they wish to continue playing
    int    exitValidation       = 0; //a sentinel value to see if the user inputted a valid letter when being asked if they want to play again
    int    discard              = 0;//garbage variable to clear out the input

    while (continuePlaying == 1 ) {
        printf("\n\t1. easy\n\t2. medium\n\t3. hard\nplease select a difficulty:");

        int difficulty = getchar();
        //clear out stdin in case there is extra input
        while ((discard = getchar()) != '\n' && discard !=EOF);


        switch (difficulty) {
            case '1':
                gameField = mf_createField(easy);
                break;
            case '2':
                gameField = mf_createField(medium);
                break;
            case '3':
                gameField = mf_createField(hard);
                break;
                //DEBUG case
//            case '4':
//                gameField = mf_createField(debug);
//                break;
            default:
                printf("invalid choice. please choose 1, 2, or 3\n");
                continue;
        }


        //main game loop
        while (gameField->gameLost == 0) {

            printf("there are %d cells remaining\n", gameField->remainingCells);
            //1. print the mine field
            cLine_DisplayField(gameField);
            //2. get input from the user
            cLine_getCoords(*gameField, coordinateInput);

            //3. update the board
            mf_validatePoint(gameField, coordinateInput);
            //free the memory allocated for the coordinates


            //4. check if all the non-mine cells have been revealed
            if (gameField->remainingCells == 0){
                //if so break out of the loop so the game ends without gamelost being triggered
                break;
            } else if (gameField->remainingCells < 0){
                //in case of an error and somehow a mine gets revealed without triggering gamelost
                printf("an error has occurred, remaining cells is: %d\n",gameField->remainingCells );
                //end the game in a loss
                gameField->gameLost = 1;
                mf_gameLost(gameField);
            }
            //5. repeat
        }
        //display the final game board after the game ends
        cLine_DisplayField(gameField);
        //after the game ends, print different messages based on whether the game is over or not
        if(gameField->gameLost == 1){
            printf("o no you lost.\n");

        } else{
            printf("congratulations you won!\n");
        }

        while (exitValidation == 0){
            printf("play again? (y/n):");
            //clear out stdin
            continueInput =(char) getchar();
            while ((discard = getchar()) != '\n' && discard !=EOF);
            if (continueInput == 'n'){
                continuePlaying = 0;//end the game
                exitValidation = 1;
            } else if (continueInput == 'y'){
                //continue as normal and exit this loop
                exitValidation = 1;
            } else{
                printf("please input 'y' or 'n'\n");
            }
        }
        exitValidation = 0;//reset value after leaving the loop
        //free the current field
        mf_destroyField(gameField);
    }

    //once the game is finally over, free the input array
    free(coordinateInput);
    return 0;
}
