# Lucas Ellenberger HW 6 Design Doc

## Note to the grader

I implemented hard mode so there is a prompt to chose if you want to play hardmode.

Any input that begins with an upper or lower case y or n will play in hardmode or regularly.

I suggest using a wordle solver from online because playing hardmode is difficult.

Your normal autograding scripts may fail because of the above implemnetation of hardmode.

## Hardmode

I store a static array of 26 integers, one for each letter

The array holds a 0 for all letters that have an unknown / unguessed status

In score guess I update the array:

	The array holds a 1 for all letters that are not in the word ('x')

	The array holds a 2 for all letters that are in the word but not yet the right spot ('y')

I have an array of size five that is the known green letters (initialize to 0)

In score guess I update the array:

	If the letter is green, I put it in the corresponding spot in the known green array

I have an array of size (26 * 5): Five positional spots for each character

In score guess I update the array:

	If a guessed char is yellow, I put it in the correspinding spot of the (26 * 5) array

	This is the numeric value of the letter (guess[i] - 'a') multiplied by five (because the array is positional and five times the length.
	I also add the position (i) to get the correct spot

In valid guess I check that they follow the previous feedback across the three arrays:

	I check if they know a letter is green but do not put it in the right spot of their guess.
	
	I check if they know a letter is yellow but do not include it in their guess

	I check if they know a letter is gray but include it in their guess

	I check if they are guessing a yellow letter in the same spot (in the (26 * 5) array)

Because if the array is 0, the player has not heard that the letter is not in the word

## Score Guess 
Loop through each character in the guess, and check if it is in the secret.

  If it is the same character:
    input 'g' into result and update the hardmode array

  else if it is elsewhere in the word:
    input 'y' into the result and update the two hardmode array

  else:
    input an 'x' into the result and update the hardmode array

If the entire result is g:
  return true;

else:
  return false;

## Valid Guess
Loop through each word in the vocab, for num words times:

If hardmode is on:
  
  check all of the things I outlined in the hardmode section above
  print a corresponding error message for why the guess is invalid

  if the guess is in the vocab:
    return true

If we have looped through everything but not returned true:
  return false

## Load Vocabulary
Create an array of 2d string array (char pointer to the pointer of the beginning of the string)

Read each line from the file and allocate more space for the char ** and the char * for the incoming string

Put in the first 5 characters from the line we read into the corresponding char array

Also set the 6th char in the array as the end of string flag '\0'

## Free Vocabulary
Loop through each string in the char array for num words:
  Free the word (6 char array)

After freeing each word:
  Free the vocabulary 2d array

Set the char ** to NULL

## Cleaning up leaks
The free vocab will free everything I allocate

I will close the input file at the end of load vocabulary
