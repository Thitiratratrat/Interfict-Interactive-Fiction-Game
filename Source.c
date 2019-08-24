#define _CRT_SECURE_NO_WARNINGS
#define TRUE 1
#define FALSE 0
#define ALPHABET 26
#define TEXTLENGTH 1000
#define NAMELENGTH 100
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

struct cryptogram{
	char actualMessage[TEXTLENGTH];
	char cryptedMessage[TEXTLENGTH];
};

//random whole new set of alphabets
void randomLetters(char randomAlphabet[][2]) {
	char random;
	int count = 0;
	while (count != ALPHABET) {
		random = rand() % (122 + 1 - 97) + 97;
		for (int letter = 0; letter < ALPHABET; letter++) {
			if (random == randomAlphabet[letter][1]) {
				break;  
			}
			else if (random != randomAlphabet[letter][1] && randomAlphabet[letter][1] == '\0') {
				randomAlphabet[letter][1] = random;
				count += 1;
				break;
			}
		}
	}
}

void createCryptogram(struct cryptogram *currentCrypt, char randomAlphabet[][2]) {
	int letter = 0;
	strcpy(currentCrypt->cryptedMessage, currentCrypt->actualMessage);
	while (currentCrypt->actualMessage[letter] != '\0') {
		currentCrypt->actualMessage[letter] = tolower(currentCrypt->actualMessage[letter]);
		for (int swapLetter = 0; swapLetter < ALPHABET; swapLetter++) {
			if (currentCrypt->actualMessage[letter] == ' ' || currentCrypt->actualMessage[letter] == '.' ||
				currentCrypt->actualMessage[letter] == '!' || currentCrypt->actualMessage[letter] == '?' ||
				currentCrypt->actualMessage[letter] == ',' || currentCrypt->actualMessage[letter] == "'" ||
				currentCrypt->actualMessage[letter] == '\n') {
				currentCrypt->cryptedMessage[letter] = currentCrypt->actualMessage[letter];
				break;
			}
			else if (currentCrypt->actualMessage[letter] == randomAlphabet[swapLetter][0]) {
				currentCrypt->cryptedMessage[letter] = randomAlphabet[swapLetter][1];
				break;
			}
		}
		letter += 1;
	}
}

void switchLetters(char letter, char switchLetter, char textMessage[]) {
	int count = 0;
	while (textMessage[count] != '\0') {
		if (textMessage[count] == letter) {
			textMessage[count] = switchLetter;
		}
		else if (textMessage[count] == switchLetter) {
			textMessage[count] = letter;
		}
		count += 1;
	}
}
//load user's story database
int loadProgress(char storyChosen[], char userName[]) {
	unsigned long loadPosition;
	int length;
	char userProgress[NAMELENGTH] = {'\0'};
	char storyName[NAMELENGTH];
	char storyChk[103] = "->";
	FILE *progress;
	strcpy(userProgress, userName);
	strcat(userProgress, "_Progress.txt");
	strcat(storyChk, storyChosen);
	progress = fopen(userProgress, "r");
	length = strlen(storyChk);
	while (fgets(storyName, NAMELENGTH, progress) != NULL) {
		if (strncmp(storyName, storyChk, length) == 0) {
			fscanf(progress, "%lu", &loadPosition);
			fclose(progress);
			return (loadPosition);
		}
	}
	fclose(progress);
	return (0);
}


//save in user's database
void saveProgress(char storyChosen [], unsigned long position, char userName []) {
	int length;
	char userProgress[NAMELENGTH] = {'\0'};
	char storyName[NAMELENGTH];
	char storyChk[103] = "->";
	char userData[TEXTLENGTH] = {'\0'};
	FILE *progress;
	strcpy(userProgress, userName);
	strcat(userProgress, "_Progress.txt");
	progress = fopen(userProgress, "r");
	strcat(storyChk, storyChosen);
	length = strlen(storyChk);
	while (fgets(storyName, NAMELENGTH, progress) != NULL) {
		if (strncmp(storyName, storyChk, length) == 0) {
			fgets(storyName, NAMELENGTH, progress);
			continue;
		}
		strcat(userData, storyName);
	}
	fclose(progress);
	progress = fopen(userProgress, "w");
	fputs(userData, progress);
	fprintf(progress, "%s\n", storyChk); 
	fprintf(progress, "%lu\n", position);
	fclose(progress);
}

void playStory(char userName [] ) {
	struct cryptogram *crypt;
	int correctSyntax = FALSE, length, storyExist = FALSE; 
	unsigned long position;
	unsigned long loadPosition;
	char storyName[NAMELENGTH];
	char storyChosen[NAMELENGTH];
	char storyText[TEXTLENGTH];
	char storyTextTemp[TEXTLENGTH];
	char signalInput[10] = "->";
	char choice[10];
	char pathCheck[10];
	char path[10];
	char letter, swapLetter, save;
	char randomAlphabet[26][2] = {{'a', '\0'}, {'b', '\0'}, {'c', '\0'}, {'d', '\0'}, {'e', '\0'}, {'f', '\0'},
								{'g', '\0'}, {'h', '\0'}, {'i', '\0'}, {'j', '\0'}, {'k', '\0'}, {'l', '\0'},
								{'m', '\0'}, {'n', '\0'}, {'o', '\0'}, {'p', '\0'}, {'q', '\0'}, {'r', '\0'},
								{'s', '\0'}, {'t', '\0'}, {'u', '\0'}, {'v', '\0'}, {'w', '\0'}, {'x', '\0'},
								{'y', '\0'}, {'z', '\0'}};
	FILE *storyList, *storyChoice;
	crypt = (struct cryptogram*)malloc(1 * sizeof(struct cryptogram));

	srand(time(NULL));

	storyList = fopen("storyList.txt", "r");
	Sleep(500);
	printf("Which journey would you like to take?\n\n");
	while (fgets(storyName, NAMELENGTH, storyList) != NULL) {
		printf("%s",storyName);
	}
	rewind(storyList);
	while (storyExist == FALSE) {
		storyExist = FALSE;
		printf("\n\nPlease enter the journey name: ");
		scanf("%s",storyChosen);
		length = strlen(storyChosen);
		while (fgets(storyName, NAMELENGTH, storyList) != NULL) {
			if (strncmp(storyName, storyChosen, length) == 0) {
				storyExist = TRUE;
				break;
			}
		}
		rewind(storyList);
	}
	fclose(storyList);

	storyChoice = fopen(strcat(storyChosen, ".txt"), "r");
	system("cls");
	
	
	Sleep(500);
	printf("\n\nYour journey will begin shortly...");
	Sleep(1500);
	system("cls");
	

	loadPosition = loadProgress(storyChosen, userName);

	if (loadPosition != 0) {
		fseek(storyChoice, loadPosition, SEEK_SET);
	}

	while (fgets(storyText, TEXTLENGTH, storyChoice) != NULL) {
		if (strncmp(storyText, "->END", 5) == 0) {
			printf("\n\n\n-------------------------------------------------------\n");
			printf("End of this story path.\n");
			printf("-------------------------------------------------------\n");
			saveProgress(storyChosen, 0, userName);
			Sleep(3000);
			break;
		}
		if (strncmp(storyText, "->choiceInput", 13) == 0) {
			do {
				printf("\n(Type ->Q to go back to main menu)\n");
				printf("(Type ->S to save progress)\n");
				printf("Decision: ");
				getchar();
				scanf("%s", &choice);
				choice[0] = toupper(choice[0]);
				printf("\n\n");
			} while (strcmp(choice,"A") != 0 && strcmp(choice, "B") != 0 && strncmp(choice, "->Q", 3) != 0 && strncmp(choice, "->S", 3) != 0);
			if (strncmp(choice, "->Q", 3) == 0) {
				printf("Do you wish to save your progress before quitting the game?\n");
				printf("Decison (Y/N): ");
				do {
					scanf("%c", &save);
					toupper(save);
				} while (save != 'Y' && save != 'N');
				if (save == 'Y') {
					saveProgress(storyChosen, position, userName);
				}
				break;
			}
			else if (strncmp(choice, "->S", 3) == 0) {
				saveProgress(storyChosen, position, userName);
				break;
			}
			strcat(signalInput, choice);
			while (fgets(storyText, TEXTLENGTH, storyChoice) != NULL) {
				if (strncmp(storyText, "->Path", 6) == 0) {
					fgets(pathCheck, 10, storyChoice);
					if (strcmp(pathCheck, path) == 0) {
						fgets(storyText, TEXTLENGTH, storyChoice);
						if (strncmp(storyText, signalInput, 3) == 0) {
							strcpy(signalInput, "->");
							correctSyntax = TRUE;
							break;
						}
					}
				}
			}
			if (correctSyntax == FALSE) {
				printf("\n\n\n------------------------------------------------------------------------------\n");
				printf("The story contains incorrect InterFict syntax.\nMissing path for decision chosen!\n");
				printf("------------------------------------------------------------------------------\n");
				Sleep(3500);
				break;
			}
			continue;
		}
		if (strncmp(storyText, "->choice", 8) == 0) {
			fseek(storyChoice, -10, SEEK_CUR);
			fflush(storyChoice);
			position = ftell(storyChoice);
			fseek(storyChoice, 10, SEEK_CUR);
			fgets(path, 10, storyChoice);
			continue;
		}
		if (strncmp(storyText, "->cryptogram", 12) == 0){
			randomLetters(randomAlphabet);
			strcpy(storyText, "");
			while (fgets(storyTextTemp, TEXTLENGTH, storyChoice) != NULL) {
				if (strncmp(storyTextTemp, "->cryptogramEnd", 15) == 0) {
					correctSyntax = TRUE;
					strcpy(crypt->actualMessage, storyText);
					createCryptogram(crypt, randomAlphabet);
					printf("%s\n", crypt->cryptedMessage);
					strcpy(storyText, crypt->cryptedMessage);
					while (strcmp(storyText, crypt->actualMessage) != 0) {
						getchar();
						do {
							printf("Swap Letters: ");
							scanf("%c %c", &letter, &swapLetter);
						} while (letter == ' ' || letter == '\n' || swapLetter == ' ' || swapLetter == '\n');
						switchLetters(letter, swapLetter, storyText);
						printf("%s\n", storyText);
						while (getchar() != '\n');
					}
					while (getchar() != '\n');
					break;
				}
				strcat(storyText, storyTextTemp);
				
			}
			if (correctSyntax == FALSE) {
				printf("\n\n\n------------------------------------------------------------------------------\n");
				printf("The story does not contain ->cryptogramEnd!\n");
				printf("------------------------------------------------------------------------------\n");
				Sleep(3500);
				break;
			}
			continue;
		}
		printf("%s", storyText);
		correctSyntax = FALSE;
	}
	fclose(storyChoice);
	system("cls");
	Sleep(500);
	free(crypt);
}

void storyRules(){
	printf("Before you create your own universe, please write according to the following rules:\n\n");
	printf("For choice input, type ->choice to signal the start of a choice, followed by the tag of the choice.\n");
	printf("Use ->choiceInput as an end signal to let the user make a decision between A and B.\n");
	printf("For example, \n->choice\n->1\nA: decsiptionA\nB: descriptionB\n->choiceInput\n\n");
	printf("Label your path signal as ->Path followed by the tag of path that corresponds to its choice.\n");
	printf("For example, \n->Path\n->1\n->A: storypathA\n->Path\n->1\n->B: storypathB\n\n");
	printf("To use the cryptogram feature, type ->cryptogram to signal the start of the crytogram and use ->cryptogramEnd to signal the end.\n");
	printf("For example, \n->cryptogram\nEnter message here.\n->cryptogramEnd\n\n");
	printf("To signal an end, use ->END.\n");
	printf("To finish typing, enter '->Q'.\n");
}

void createStory() {
	int invalidInput = FALSE;
	char newStory[NAMELENGTH];
	char storyName[NAMELENGTH];
	char text[100000] = {'\0'};
	char tempText[TEXTLENGTH] = {'\0'};
	int nameTaken;
	FILE *storyList, *createStory;
	storyList = fopen("storyList.txt", "r");

	Sleep(500);
	getchar();
	printf("Please enter the name of the new universe you wish to create\n");
	do {
		invalidInput = FALSE;
		printf("Name: ");
		gets(newStory);
		nameTaken = FALSE;
		if (strlen(newStory) < 1) {
			invalidInput = TRUE;
		}
		else {
			while (fgets(storyName, NAMELENGTH, storyList) != NULL) {
				if (strcmp(newStory, storyName) == 0) {
					printf("This name has already been taken! Please enter another name.\n");
					nameTaken = TRUE;
					rewind(storyList);
					break;
				}
			}
			for (int letter = 0; letter < strlen(newStory); letter++) {
				if (newStory[letter] == '\0' && letter == 0) {
					printf("Your story name must contain at least one character.\n\n");
					invalidInput = TRUE;
					break;
				}
				if (newStory[letter] == ' ' || newStory[letter] == '\n') {
					printf("Your story name must not contain spaces.\n\n");
					invalidInput = TRUE;
					break;
				}
			}
		}
	} while (nameTaken == TRUE || invalidInput == TRUE);

	fclose(storyList);
	system("cls");

	Sleep(500);
	printf("\n\nYour universe is being created...");
	Sleep(1500);
	system("cls");

	storyList = fopen("storyList.txt", "a");
	fputs("\n", storyList);
	fputs(newStory, storyList);
	fclose(storyList);
	
	strcat(newStory, ".txt");
	createStory = fopen(newStory, "w");
	
	storyRules();
	printf("\nTo read the rules again, type '->R'.");
	Sleep(3000);
	system("cls");

	getchar();
	//get story text
	while (TRUE){
		gets(tempText);
		if (strncmp(&tempText, "->R", 3) == 0) {
			storyRules();
			continue;
		}
		if (strncmp(&tempText, "->Q", 3) == 0 ){
			break;
		}
		strcat(text, tempText);
		strcat(text, "\n");
	} 
	fputs(text, createStory);
	fclose(createStory);
	system("cls");
}

void login(char name[NAMELENGTH]) {
	int sameUserPass, invalidInput, quit;
	char choice[NAMELENGTH];
	char user[NAMELENGTH];
	char userChk[NAMELENGTH];
	char password[NAMELENGTH];
	char passwordChk[NAMELENGTH];
	char userProgress[NAMELENGTH];
	FILE *username, *progress;
	username = fopen("username.txt", "r");
	while (TRUE) {
		printf("Before we move on, please login or create your personal account.\n");
		printf("1. Login\n");
		printf("2. Create Account\n\n");
		do {
			printf("I would like to (enter 1 or 2): ");
			scanf("%s", &choice);
		} while (strcmp(choice, "1") != 0 && strcmp(choice, "2"));
		system("cls");
		if (strcmp(choice, "1") == 0) {
			rewind(username);
			while (TRUE) {
				printf("(Enter ->Q in Username to go back)\n");
				printf("Username: ");
				scanf("%s", user);
				if (strcmp(user, "->Q") == 0) {
					system("cls");
					break;
				}
				printf("Password: ");
				scanf("%s", password);
				while (fscanf(username, "%s %s", userChk, passwordChk) != EOF) {
					if (strcmp(userChk, user) == 0 && strcmp(passwordChk, password) == 0) {
						strcpy(name, user);
						system("cls");
						fclose(username);
						return;
					}
				}
				printf("\nUnfortunately your account could not be found.\n\n");
				rewind(username);
			}
		}
		else if (strcmp(choice, "2") == 0) {
			do {
				while (getchar() != '\n');
				quit = FALSE;
				sameUserPass = FALSE;
				invalidInput = FALSE;
				printf("Enter ->Q in username to go back.\n");
				printf("Please enter your username.\n");
				printf("Username: ");
				gets(user);
				if (strcmp(user, "->Q") == 0) {
					quit = TRUE;
					system("cls");
					break;
				}
				while (fscanf(username, "%s %s", userChk, passwordChk) != EOF) {
					if (strcmp(userChk, user) == 0) {
						printf("This username has already been taken!\n\n");
						sameUserPass = TRUE;
						rewind(username);
						break;
					}
				}
				rewind(username);
				if (strlen(user) < 1) {
					invalidInput = TRUE;
				}
				for (int letter = 0; letter < strlen(user); letter++) {
					if (user[letter] == '\0' && letter == 0) {
						printf("Your username must contain at least one character.\n\n");
						invalidInput = TRUE;
						break;
					}
					if (user[letter] == ' ' || user[letter] == '\n') {
						printf("Your username must not contain spaces.\n\n");
						invalidInput = TRUE;
						break;
					}
				}
			} while (sameUserPass == TRUE || invalidInput == TRUE);
			if (quit == FALSE) {
				do {
					while (getchar() != '\n');
					quit = FALSE;
					sameUserPass = FALSE;
					invalidInput = FALSE;
					printf("Your password must contain at least 4 characters.\n");
					printf("Please enter your password.\n");
					printf("Password: ");
					gets(password);
					if (strncmp(password, "->Q", 3) == 0) {
						quit = TRUE;
						system("cls");
						break;
					}
					while (fscanf(username, "%s %s", userChk, passwordChk) != EOF) {
						if (strcmp(passwordChk, password) == 0) {
							printf("This password has already been taken!\n\n");
							sameUserPass = TRUE;
							rewind(username);
							break;
						}
					}
					rewind(username);
					if (strlen(password) < 4) {
						printf("Your password must contain at least four characters!\n\n");
						invalidInput = TRUE;
					}
					else {
						for (int letter = 0; letter < strlen(password); letter++) {
							if (password[letter] == 0 && letter == 0) {
								printf("Your password must contain at least four characters.\n\n");
								invalidInput = TRUE;
								break;
							}
							if (password[letter] == ' ' || password[letter] == '\n') {
								printf("Your password must not contain spaces.\n\n");
								invalidInput = TRUE;
								break;
							}
						}
					}
				} while (sameUserPass == TRUE || invalidInput == TRUE);
				if (quit == FALSE) {
					fclose(username);
					username = fopen("username.txt", "a");
					fprintf(username, "%s %s\n", user, password);
					fclose(username);
					strcpy(userProgress, user);
					strcat(userProgress, "_Progress.txt");
					progress = fopen(userProgress, "w");
					fclose(progress);
					username = fopen("username.txt", "r");
					system("cls");
				}
			}
		}
		
	}
	fclose(username);
}

void main() {
	char choice[NAMELENGTH];
	char name[NAMELENGTH];
	
	Sleep(1000);
	printf("\n\n\n\nInterFict: A world where a new universe is created with each choice you make.\n");
	Sleep(2000);
	system("cls");
	printf("Greetings fellow traveler! ");
	Sleep(1000);
	printf("Welcome to the world of InterFict!\n\n");
	Sleep(1000);
	
	login(name);
	
	Sleep(1000);
	printf("\n\n\n\nWelcome to InterFict, %s", name);
	Sleep(2000);
	system("cls");
	
	while (TRUE) {
		printf("Which journey would you like to take today?\n");
		printf("1. Play a story.\n");
		printf("2. Create story.\n");
		printf("3. Quit InterFict.\n");
		do {
			printf("\nI'm here to (enter 1,2,or 3): ");
			scanf("%s", &choice);
		} while (strcmp(choice, "1") != 0 && strcmp(choice, "2") && strcmp(choice, "3"));
		system("cls");
		if (strcmp(choice, "1") == 0) {
			playStory(name);
		}
		else if (strcmp(choice, "2") == 0) {
			createStory();
		}
		else{
			Sleep(500);
			printf("\n\nHope our paths will meet again someday!");
			Sleep(2000);
			exit(1);
		}
	}   
	system("pause");
}