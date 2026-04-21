// James Chumpol
// CS135, Section 1001/1103

#include <stdio.h>
#define WORDFILE "mystery.txt"
#define TRUE 1
#define FALSE 0
#define ERR 1
#define ROW_SIZE 2
#define WORD_SIZE 5
#define COL_SIZE 10
#define CHANGE_CASE 32

int foundFile(FILE *file)
{
  if (file)
  {
    return TRUE;
  }
  printf("Unable to open file.\n");
  return FALSE;
}

// Test function to quickly print 2D arrays.
void printArr(char arr[ROW_SIZE][COL_SIZE], int row)
{
  for (int i = 0; i < WORD_SIZE; i++)
  {
    printf("%c", arr[row][i]);
  }
  printf("\n");
}

void getGoalWord(FILE *file, char word[WORD_SIZE])
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
}

void compareWordGuess(char wordGoal[WORD_SIZE], char wordGuess[ROW_SIZE][COL_SIZE])
{
  int columnPos;
  const int goal = 0;
  const int hint = 1;
  printf("\n");
  
  for (columnPos = 0; columnPos < WORD_SIZE; columnPos++)
  {
    char letterGoal = wordGoal[columnPos];
    char letterGuess = wordGuess[goal][columnPos];
    if (letterGoal == letterGuess)
    {
      wordGuess[goal][columnPos] -= CHANGE_CASE;
      // Place space char in hint row if found matching letter
      // This is to make sure the hint row has a printable characters
      wordGuess[hint][columnPos] = ' ';
    }
    else
    {
      for (int i = 0; i < WORD_SIZE; i++)
      {
        char letterGoal = wordGoal[i];
        if (letterGoal == letterGuess)
        {
          wordGuess[hint][columnPos] = '^';
          i = WORD_SIZE;
        }
        else
        {
          wordGuess[hint][columnPos] = ' ';
        }
      }
    }
  }

  printf("%s", wordGuess);
  if (wordGuess[hint])
  {
    printf("\n%s\n", wordGuess[hint]);
  }
}

void toLowerCase(char wordGuess[ROW_SIZE][COL_SIZE])
{
  int row = 0;
  for (int i = 0; i < WORD_SIZE; i++)
  {
    if (wordGuess[row][i] == '\0') return;
    if (wordGuess[row][i] >= 'A' && wordGuess[row][i] <= 'Z')
    {
      wordGuess[row][i] += CHANGE_CASE;
    }
  }
}

void startGame(char wordGoal[WORD_SIZE])
{
  int attempt = 1;
  printf("GUESS %d: ", attempt);

  char wordGuess[ROW_SIZE][COL_SIZE] = {0};
  scanf("%s", &wordGuess);

  toLowerCase(wordGuess);
  compareWordGuess(wordGoal,wordGuess);
}

int main()
{
  FILE *inputFile = fopen(WORDFILE, "r");
  if (!foundFile(inputFile))
  {
    return ERR;
  }

  char wordGoal[WORD_SIZE];
  getGoalWord(inputFile, wordGoal);
  fclose(inputFile);

  printf("Welcome to Twordle!\n");
  printf("Please enter your first guess that is 5 letters long.\n");
  startGame(wordGoal);
}
