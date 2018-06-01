#include "mineField.h"
// used to display the mineField in a command line to receive input

//displays the mineField in the command line
//using ascii art
//input is a well made, mineField
//#include "mineField.h"
void cLine_DisplayField(struct mineField* currentField);


// receives an input from the commandline and validates it
// output is a int array of size 2,
// the values between 0 and height-1, and 0 and width-1
// input is a string which is between 3 and 5 characters,
// where n and m are the max number of numbers for height and width
void cLine_getCoords(struct mineField currentField, int* receivedCoords);
