//
#include "mineFieldCommandIO.h"



void cLine_DisplayField(struct mineField* currentField){
    // printf("printing the first line\n");
    // for (int i = 0; i < currentField->width; ++i)
    // {
    //     printf("%d ",currentField->field[0][i] );
    //     /* code */
    // }
    printf("\n");
    printf("\n");
    //print the number of columns above the board;
    printf("   " );
    for (int col = 0; col < currentField->width; col++) {

        printf("%d ",col );
        //only want a space for padding if there is 1 digit
        if (col < 10) {
            printf(" " );
        }
    }
    printf("\n   ");

    //print a line of dashes in between the numbers and board
    for (int col = 0; col < currentField->width; col++) {
        printf("-- " );
    }
    printf("\n" );

    //print the board
    for (int row = 0; row < currentField->height; row++) {
        //print the row number and a line in front of every row
        //add padding if there is only one digit
        if (row < 10) {
            printf("%d |", row );
        } else {

            printf("%d|",row );
        }
        //printf("row %d  pointer=%d \n",currentField->field[row], currentField->field );
        for (int col = 0; col < currentField->width; col++) {
            //if the cell has not been checked
            if (currentField->checked[row][col] == 0) {
                printf("-  " );
            //if the cell Has been checked or the game has been lost
            } else {
                //if it is a mine display a star instead
                //should only be triggered in the event of a loss
                if (currentField->field[row][col] == -1) {
                    printf("*  ");
                }else
                //if there is no mines around the cell, display some spaces
                if (currentField->field[row][col] == 0) {
                    printf("   " );
                } else {

                    printf("%d  ",currentField->field[row][col] );
                }
            }
            //go to the next line/row
        }
        printf("\n" );
    }

}


void cLine_getCoords(struct mineField currentField, int* receivedCoords){


    int isValidInput = 0;//a check to make sure the input is a valid at the end.
    int inputArray[2] = {0,0};//malloc(2*sizeof(int));
    inputArray[0] = -1;//not a valid coordinate, thus it can be used to check for valid input
    inputArray[1] = -1;

    int maxSize = 7;// there should be at most 6 characters:
    //at most 2 numbers, a space,at most 2 numbers, \n
    char coordInput[maxSize];

    //continue to get from stdin until we have a valid input
    while (!(isValidInput)) {


        printf("please enter the coordinates of your guess seperated by a space:" );
        if (fgets(coordInput, maxSize, stdin) != NULL) {

            int inputIndex = 0;
            int arrayIndex = 0;// for inputing the numbers into the correct index of the array
            //make sure the first character is a number
            if ( !(isdigit(coordInput[0])) ) {
                printf("\nyou entered %c other than a number first\n",coordInput[0] );
                printf("make sure the first character is a number\n\n" );
                continue;
            }
            while ((coordInput[inputIndex] != '\0')&&
                   (coordInput[inputIndex] != '\n')&&
                   inputIndex < maxSize) {

                if (isdigit(coordInput[inputIndex]) == 0 && coordInput[inputIndex] != 32) {

                    printf("%c is not a space or number\n",coordInput[inputIndex] );
                    printf("your input was not valid. Make sure you only include numbers and spaces\n\n" );

                    break;
                }
                //only check if it is not the first character
                if (inputIndex > 0 && coordInput[inputIndex] == 32 && coordInput[inputIndex-1] != 32 ) {
                    /* if the current character is a space and the last character wasn't,
                    then the first index is over and we should now insert into the next index of the array
                    the third check is if the user entered more than 1 space*/
                    arrayIndex++;
                }



                if (isdigit(coordInput[inputIndex])) {

                    if (inputIndex > 0 && isdigit(coordInput[inputIndex-1]) ) {
                        /* if the previous character was a number and there was a
                        previous character, then we need to
                        modify the current number in the arrayIndex and before we add
                        the new character  */
                        inputArray[arrayIndex] *= 10;//shift it over by to the tens slot, e.g 5 => 50
                        //then add the new number in the ones slot
                        inputArray[arrayIndex] += coordInput[inputIndex] - '0';
                    } else {
                        inputArray[arrayIndex] = coordInput[inputIndex] - '0';
                    }
                }

                inputIndex++;
            }
        }
        if (inputArray[0] >=0 && inputArray[1] >= 0 &&
            inputArray[0] <currentField.width && inputArray[1]< currentField.height) {

            isValidInput = 1;
        }
        else{
            printf("your input was not valid, please try again \n\n" );
        }

    }

    receivedCoords[0] = inputArray[0];
    receivedCoords[1] = inputArray[1];

}
