
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Struct that stores all words of certain length
char **wordStorage;
// Function to display the instructions of the game. 
// - int number: this number detrmines what text to dsiplay onto the console for instructions because users will reach certain points of the game
void displayInstructions(int number){
    if(number == 0){
        printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
	    printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
	    printf("Enjoy!\n\n");
    }

    if(number == 1){
        printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
        printf("You may also type in 'q' to quit guessing.\n");
    }
}

//Prompts User for how many letters they want in the words, and then the function finds the matches and displays onto console
int displayMatches(){
    int num;
    int lineNumbers = 0;
    printf("How many letters do you want to have in the words? ");
    scanf("%d", &num);

    char filename[] = "words.txt";
	FILE *filePtr  = fopen(filename, "r"); // "r" means we open the file for reading
	
	// Check that the file was able to be opened
	if(filePtr == NULL) {
		printf("Error: could not open %s for reading\n", filename);
		exit(-1);
	}

  wordStorage =  malloc(1 * sizeof(char *));
  
	// Read each word from file, and print them one by one
	char inputString[ 81];
	while(fscanf(filePtr, "%s", inputString) != EOF) {
		if(strlen(inputString) == num){
			lineNumbers++;
      wordStorage = realloc(wordStorage, lineNumbers * sizeof(char*));
		}
	}
	//initiliaze memory for array holding all the dictionary words
	//wordStorage =  malloc(lineNumbers * sizeof(char *));
	int indices = 0;
	
	fclose(filePtr);
	filePtr  = fopen(filename, "r");

  	while(fscanf(filePtr, "%s", inputString) != EOF) {
		if(strlen(inputString) == num){
			//memory for each indivdual word
			wordStorage[indices] = malloc((strlen(inputString) * sizeof(char)));
			strcpy(wordStorage[indices], inputString);
			indices++;
		}
  	}
	// Close the file
	printf("Number of %d-letter words found: %d.", num, lineNumbers);
	fclose(filePtr);
    return lineNumbers;
}

//checks if the starting and ending words are valid words
bool checkIfValidWord(char *str1, char *str2, int stringLength, int matches){
	if(strlen(str1) != stringLength){
		printf("Your word, '%s', is not a 4-letter word. Try again.\n", str1);
		return false;
	} else if(strlen(str2) != stringLength){
		printf("Your word, '%s', is not a 4-letter word. Try again.\n", str2);
		return false;
	}

	bool isStr1True = false;
	bool isStr2True = false;
	for(int i = 0; i < matches; i++){
		if(strcmp(str1, wordStorage[i]) == 0){
			isStr1True = true;
		}
		if(strcmp(str2, wordStorage[i]) == 0){
			isStr2True = true;
		}
	}

	if(!isStr1True){
		printf("Your word, '%s', is not a valid dictionary word. Try again.\n", str1);
		return false;
	} else if(!isStr2True){
		printf("Your word, '%s', is not a valid dictionary word. Try again.\n", str2);
		return false;
	}

	return true;
}



//Finds the start stop words to be played and then copies those words into the paramters 
//probably buggy fix later
void findStartAndEnd(char *str1, char *str2, int stringLength, int matches){
	char tempArr[2];
	int random;
	char tempStr1[stringLength];
	char tempStr2[stringLength];

	printf("\n\nEnter starting and ending words, or 'r' for either for a random word: ");
	scanf("%s",&tempStr1);
	scanf("%s",&tempStr2);
	
	//If first string is random then randomize
	if(strcmp(tempStr1, "r") == 0){
		 random = rand() % matches;
		strcpy(tempStr1, wordStorage[random]);
		strcpy(str1, tempStr1);
		strcpy(str2, tempStr2);
	}

	//If second string is random then randomize
	if(strcmp(tempStr2, "r") == 0){
		 random = rand() % matches;
		strcpy(tempStr2, wordStorage[random]);
		strcpy(str1, tempStr1);
		strcpy(str2, tempStr2);
	}

	//if no strings are random then enter input
	if((strcmp(tempStr1, "r") != 0) && (strcmp(tempStr2, "r") != 0)){
		while(!checkIfValidWord(tempStr1, tempStr2, stringLength, matches)){
			printf("Enter starting and ending words, or 'r' for either for a random word: ");
			scanf("%s",&tempStr1);
			scanf("%s",&tempStr2);

			if(strcmp(tempStr1, "r") == 0){
		 	random = rand() % matches;
			strcpy(tempStr1, wordStorage[random]);
			strcpy(str1, tempStr1);
			strcpy(str2, tempStr2);
			}
			if(strcmp(tempStr2, "r") == 0){
			random = rand() % matches;
			strcpy(tempStr2, wordStorage[random]);
			strcpy(str1, tempStr1);
			strcpy(str2, tempStr2);
			}
		}
		}
		if(checkIfValidWord(tempStr1, tempStr2, stringLength, matches)){
			printf("Your starting word is: %s.\n", tempStr1);
			printf("Your ending word is: %s.\n\n", tempStr2);
		}
		strcpy(str1, tempStr1);
		strcpy(str2, tempStr2);
}

// checks if changed words are valid
bool checkForUserErrors(char *prevWord, char *nextWord, int stringLength, int matches){
	if(strlen(nextWord) != stringLength){
		printf("Your word, '%s', is not a %d-letter word. Try again.\n", nextWord, stringLength);
		return false;
	}

	bool val = false;
	for(int i = 0; i < matches; i++){
		if(strcmp(nextWord, wordStorage[i]) == 0){
			val = true;
		}
	}
	if(!val){
		printf("Your word, '%s', is not a valid dictionary word. Try again.\n", nextWord);
		return false;
	}

	int diffCount = 0;
	for(int i = 0; i < stringLength; i++){
		if(prevWord[i] != nextWord[i]){
			diffCount++;
		}
	}
	if(diffCount > 1){
		printf("Your word, '%s', is not exactly 1 character different. Try again.\n", nextWord);
		return false;
	}

	return true;
}

//plays game to change previous word into goal word
void playGame(char *str1, char *str2, int stringLength, int matches){
	int moveCount = 1;
	int userQuit = 0;
	char nextWord[stringLength];
	char previousWord[stringLength];
	strcpy(previousWord, str1);
		while(true){
			bool val;
			printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", moveCount, previousWord ,str2);
			scanf("%s", &nextWord);
			if(strcmp(nextWord, "q") == 0){
			break;
		}
		val = checkForUserErrors(previousWord, nextWord, stringLength, matches);
		if(val == false){
			continue;
		} else {
			strcpy(previousWord, nextWord);
			moveCount++;
		}
		if((strcmp(nextWord, str2) == 0) && (val == true)){
		printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", str1, str2, moveCount - 1);
		break;
		}
	}
}

//Runs all the functions in a certain order so it follows the logical prcedence of what must be done in the game
void runFunctions(){
  displayInstructions(0);
	int userQuit = 0;
	while(userQuit != 3){
 		int matches = displayMatches();
		int stringLen = strlen(wordStorage[0]);
		while(true){
			char str1[stringLen];
			char str2[stringLen];
			findStartAndEnd(str1, str2, stringLen, matches);
			displayInstructions(1);
			playGame(str1, str2, stringLen, matches);
			printf("\nEnter: \t1 to play again,\n");
    		printf("\t2 to change the number of letters in the words and then play again, or\n");
   	 		printf("\t3 to exit the program.\n");
    		printf("Your choice --> ");
			scanf("%d", &userQuit);
			if(userQuit == 1){
				continue;
			} else if(userQuit == 2){
				for(int i = 0; i < matches; i++){
					free(wordStorage[i]);
				}
				free(wordStorage);
				break;
			} else if(userQuit == 3){
				
				for(int i = 0; i < matches; i++){
					free(wordStorage[i]);
				}
				free(wordStorage);
				break;
			}
		}
	}
}


int main(void) {
	srand(1);
  runFunctions();
	return 0;
}