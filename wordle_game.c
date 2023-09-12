#include "wordle_game.h"
#include <stdlib.h>
#include <string.h>

static int known_x_y[26];
static int known_y[130];
static int known_g[5];
static int hard_mode = 0;

void hardmode(int i) {
  hard_mode = i;
}

// Most of the code that you're going to have to implement goes in here.
// Feel free to add more helper functions if necessary.

// Returns true if the guess is an exact match with the secret word, but
// more importantly, fills in the result with the following:
// - 'x' goes in a slot if the corresponding letter in the guess does not appear
//   anywhere in the secret word
// - 'y' goes in a slot if the corresponding letter in the guess appears in the
//   secret word, but not in the corresponding slot
// - 'g' goes in a slot if the corresponding letter in the guess appears in the
//   corresponding slot in the secret word.
//   You can assume that result points at enough memory for a string of length
//   5. (ie, at least 6 bytes long)
bool score_guess(char *secret, char *guess, char *result) {
  // Implemented by me
  memset(result, '\0', 5);
  for (int i = 0; i < 5; i++) {
    // if the guess is in the correct spot
    if (guess[i] == secret[i]) {
      // store the letter they got correct in the 5 char array
      known_g[i] = (guess[i] - 'a' + 1);
      result[i] = 'g';
    } else {
      for (int j = 0; j < 5; j++) {
        // if the guessed letter is in the secret word
        if (guess[i] == secret[j]) {
          // stores the letter and position that the y was returned for
          known_y[((guess[i] - 'a') * 5) + i]++;
          // remembers that a letter should be in the guessed word
          known_x_y[guess[i] - 'a'] = 2;
          // printf("I set the letter %c to two, it is set as: %d\n", guess[i], known_x_y[guess[i] - 'a']);
          result[i] = 'y';
        }
      }
      // if the letter is not in the word at all
      if (result[i] != 'g' && result[i] != 'y') {
        // remember that they cannot guess this letter again
        known_x_y[guess[i] - 'a'] = 1;
        // printf("I set the letter %c to one, it is set as: %d\n", guess[i], known_x_y[guess[i] - 'a']);
        result[i] = 'x';
      }
    }
  }
  result[5] = '\0';

  for (int i = 0; i < 5; i++) {
    if (result[i] != 'g') {
      return false;
    }
  }
  return true;
}

// Returns true if the specified guess is one of the strings in the vocabulary,
// and false otherwise. num_words is the length of the vocabulary.
// A simple linear scan over the strings in vocabulary is fine for our purposes,
// but consider: could you do this search more quickly?
bool valid_guess(char *guess, char **vocabulary, size_t num_words) {
  // Implemented by me
  int result;
  
  if (hard_mode) {
    for (int i = 0; i < 5; i++) {
      // check if the guessed letter is known to be gray (not in the word)
      if (known_x_y[guess[i] - 'a'] == 1) {
        printf("You can't guess %c, it is not present.\n", guess[i]);
        return false;
      }
      // check if the a letter is known to be green but not guessed again
      if ((known_g[i] != 0) && (known_g[i] != (guess[i] - 'a' + 1))) {
        printf("You cannot guess the letter %c there. You know that %c should be in position %d.\n",
	      (known_g[i] + 'a' - 1), (known_g[i] + 'a' - 1), (i + 1));
        return false;
      }
      // check if they guess a yellow letter in the same spot
      if (known_y[((guess[i] - 'a') * 5) + i] != 0) {
        printf("You cannot guess the letter %c in position %d, but it is in the word.\n", guess[i], (i +1));
        return false;
      }
      for (int j = 0; j < 26; j++) {
        // printf("checking letter %c.\n What number is with it? %d Is the letter in the word? %s\n", 
	//	(j + 'a'), known_x_y[j], strrchr(guess, (j + 'a')));
        if ((known_x_y[j] == 2) && (!strrchr(guess, (j + 'a')))) {
          printf("Guess must include %c, it is in the word.\n", (j + 'a'));
          return false;
        }
      }
    }
  }

  for (size_t i = 0; i < num_words; i++) {
    result = strcmp(guess, vocabulary[i]);
    if (result == 0) {
      return true;
    }
  }
  return false;
}

// Returns an array of strings (so, char **), where each string contains a word
// from the specified file. The file is assumed to contain 5-letter words, one
// per line.
// Also, this function sets the value pointed at by *num_words to be the number
// of words read.
// This will need to allocate enough memory to hold all of the char* pointers --
// so you will keep track of the size of your char** array and then use realloc
// to make the array larger over time, so that you have enough space for the
// dynamically-growing array of char *.
// Use fopen to open the input file for reading,
// strdup (or strndup) to make copies of each word read from that file, and
// fclose to close the file when you are done reading from it.
// Each element of the array should be a single five-letter word,
// null-terminated.
char **load_vocabulary(char *filename, size_t *num_words) {
  char **out = NULL;
  *num_words = 0;

  out = (char **)malloc(sizeof(char *));
  char buffer[7];
  
  FILE * infile;
  infile = fopen(filename, "r");

  if (out && infile) {
    while (fgets(buffer, 7, infile)) {
      out[*num_words] = strndup(buffer, 5);
      // printf("%s and %s\n", buffer, out[*num_words]);
      (*num_words)++;
      out = (char **)realloc(out, (*num_words + 1) * sizeof(char *));
    }
  }

  fclose(infile);
  return out;
}

// Free each of the strings in the vocabulary, as well as the pointer vocabulary
// itself (which points to an array of char *).
void free_vocabulary(char **vocabulary, size_t num_words) {
  for (size_t i = 0; i < num_words; i++) {
    if (vocabulary[i]) {
      free(vocabulary[i]);
    }
  }
  if (vocabulary) {
    free(vocabulary);
  }
  vocabulary = NULL;
  return;
}
