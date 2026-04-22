// James Chumpol
// CS135, Section 1001/1103

#include <stdio.h>
#define WORDFILE "mystery.txt"
#define TRUE 1
#define FALSE 0
#define ERR 1
#define ROW_SIZE 6
#define COL_SIZE 5
#define CHANGE_CASE 32
#define MAX_ATTEMPT 6
#define SPACE ' '
#define CARET '^'
#define TERMINATOR '\0'

// * Function to quickly print 2D arrays
void printArr(char arr[ROW_SIZE][COL_SIZE], int row)
{
  for (int currentPos = 0; currentPos < COL_SIZE; currentPos++)
  {
    printf("%c", arr[row][currentPos]);
  }
  printf("\n");
  return;
}

// * Custom print function with an implicit line break
// * cuz I forget \n a lot
void printLine(const char* format, ...)
{
  printf(format);
  printf("\n");
  return;
}

FILE* foundFile(FILE *file)
{
  if (!file)
  {
    printLine("Unable to open file.");
  }
  return file;
}

void getGoalWord(FILE *file, char word[COL_SIZE])
{
  char inputChar;
  int columnPos;
  for (columnPos = 0; fscanf(file, "%c", &inputChar) == 1; columnPos++)
  {
    if (inputChar == '\0')
    {
      return;
    }
    word[columnPos] = inputChar;
  }
  return;
}

void processGuess
(
  char wordGoal[COL_SIZE],
  char wordGuess[ROW_SIZE][COL_SIZE],
  char hintArr[ROW_SIZE][COL_SIZE],
  int currentRow
)
{
  for (int columnPos = 0; columnPos < COL_SIZE; columnPos++)
  {
    char letterGoal = wordGoal[columnPos];
    char letterGuess = wordGuess[currentRow][columnPos];
    char letterHint = hintArr[currentRow][columnPos];
    if (letterGoal == letterGuess)
    {
      // * If found matching letter and position,
      // * subtract ascii value of 32 to get upper case equivalent
      wordGuess[currentRow][columnPos] -= CHANGE_CASE;

      // * Place null chars in hint array if found matching letter
      // * This is to make sure the hint row has something printable
      hintArr[currentRow][columnPos] = TERMINATOR;
    }
    else
    {
      // * If the guessed letter is found in the goal word,
      // * but is in the wrong position, place a caret underneath
      // * that letter. Otherwise, insert space
      for (int i = 0; i < COL_SIZE; i++)
      {
        char letterGoal = wordGoal[i];
        if (letterGoal == letterGuess)
        {
          hintArr[currentRow][columnPos] = '^';
          i = COL_SIZE;
        }
        else
        {
          hintArr[currentRow][columnPos] = SPACE;
        }
      }
    }
  }
  return;
}

int compareWordGuess
(
  char wordGoal[COL_SIZE],
  char wordGuess[ROW_SIZE][COL_SIZE],
  char hintArr[ROW_SIZE][COL_SIZE],
  int currentRow
)
{
  int matchCounter = 0;
  for (int currentPos = 0; currentPos < COL_SIZE; currentPos++)
  {
    // * Increment matchCounter for every capital letter found
    if (wordGuess[currentRow][currentPos] >= 'A' && wordGuess[currentRow][currentPos] <= 'Z')
    {
      matchCounter++;
    }
  }

  if (matchCounter == COL_SIZE)
  {
    printArr(wordGuess, currentRow);
    return TRUE;
  }
  else
  {
    // * If no complete match, print all entered guesses
    // * and paired hint rows
    for (int i = 0; i <= currentRow; i++)
    {
      printArr(wordGuess, i);
      printArr(hintArr, i);
    }
    printLine("Matched #: %d", matchCounter);
    return FALSE;
  }
}

void toLowerCase(char wordGuess[ROW_SIZE][COL_SIZE], int currentRow)
{
  // * If there is a capital letter found in the user's input,
  // * add the ascii decimal value of 32 to get its lowercase equivalent
  for (int currentPos = 0; currentPos < COL_SIZE; currentPos++)
  {
    if (wordGuess[currentRow][currentPos] >= 'A' && wordGuess[currentRow][currentPos] <= 'Z')
    {
      wordGuess[currentRow][currentPos] += CHANGE_CASE;
    }
  }
  return;
}

void getUserGuess
(
  char wordGuess[ROW_SIZE][COL_SIZE],
  int currentRow,
  int currentAttempt
)
{
  scanf("%s", &wordGuess[currentRow]);

  char lastLetter = wordGuess[currentRow][COL_SIZE - 1];
  char lookAhead = wordGuess[currentRow][COL_SIZE];
  // * Loops prompt if null character is found in last place
  // * or if there is a non-null character after last place
  while (lastLetter == TERMINATOR || lookAhead != TERMINATOR)
  {
    printArr(wordGuess, currentRow);

    // * Flush current row
    for (int currentPos = 0; currentPos < COL_SIZE; currentPos++)
    {
      wordGuess[currentRow][currentPos] = TERMINATOR;
    }

    printLine("Please enter a word that is 5 letters long.");
    printf("GUESS %d: ", currentAttempt);
    scanf("%s", &wordGuess[currentRow]);

    // * Update the value of the variables
    lastLetter = wordGuess[currentRow][COL_SIZE - 1];
    lookAhead = wordGuess[currentRow][COL_SIZE];
  }
  return;
}

void startTwordle(char wordGoal[COL_SIZE])
{
  printLine("Welcome to Twordle!");
  printLine("Please enter your first guess that is 5 letters long.");

  int currentAttempt = 1;
  int currentRow = currentAttempt - 1;
  char wordGuess[ROW_SIZE][COL_SIZE] = {0};
  char hintArr[ROW_SIZE][COL_SIZE] = {0};
  do
  {
    switch(currentAttempt)
    {
      default:
        printf("\nGUESS %d: ", currentAttempt);
        break;
      case MAX_ATTEMPT:
        printf("\nFINAL GUESS: ");
    }
    getUserGuess(wordGuess, currentRow, currentAttempt);
    printLine("==============");

    toLowerCase(wordGuess,currentRow);
    processGuess(wordGoal, wordGuess, hintArr, currentRow);

    if (compareWordGuess(wordGoal, wordGuess, hintArr, currentRow))
    {
      switch(currentAttempt)
      {
        case 1:
          printLine("You found the word on your first try!", currentAttempt);
          break;
        default:
          printLine("You found the word in %d attempts!", currentAttempt);
      }
      switch(currentAttempt)
      {
        case 1:
          printLine("GOATED.");
          return;
        case 2:
          printLine("Epic!");
          return;
        case 3:
          printLine("Sweet!");
          return;
        case 4:
        case 5:
          printLine("Nice!");
          return;
        case MAX_ATTEMPT:
        default:
          return;
      }
    }
    else
    {
      currentAttempt++;
      // * Check to prevent segmentation faults
      if (currentAttempt <= MAX_ATTEMPT)
      {
        currentRow++;
      }
      for (int currentPos = 0; currentPos < COL_SIZE; currentPos++)
      {
        wordGuess[currentRow][currentPos] = 0;
      }
    }
  } while (currentAttempt <= MAX_ATTEMPT);

  printLine("You lost! Better luck next time!");
  return;
}

int main()
{
  FILE *inputFile = fopen(WORDFILE, "r");
  if (!foundFile(inputFile)) return ERR;

  char wordGoal[COL_SIZE];
  getGoalWord(inputFile, wordGoal);
  fclose(inputFile);

  startTwordle(wordGoal);

  return 0;
}
