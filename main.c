#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

/************  COMP201 ASSIGNMENT 1  ************/

//YAMAC OMUR


int main(int argc, char *argv[])
{

    if (strcmp(argv[1], "calculate-tf") == 0) // analyze frequencies
    {
       FILE * fp;
       int bufferLength= 255;
       char buffer[bufferLength]; //The variables I defined here are similar
       //to the file processing link provided by the assignment pdf. I used the "Link 2", and user "malat"s answer. 
     

	int cancerousCellNum= 0; //This variable will store the number of cancerous cells.
	int healthyCellNum= 0; //This variable will store the number of healthy cells.
	int wordCNum= 0; //This variable stores the frequency of a word in cancerous sequences.
	int wordHNum=0; //This variable stores the frequency of a word in healthy sequences.
	float tfCScore= 0; //This variable stores the TF score of a word for the cancerous class.
	float tfHScore= 0; //This variable stores the TF score of a word for the healthy class.


	//I opened the sequences.txt file for reading here. I again used user "malat"s answer on "Link 2" to understand how file reading works in C.

       fp= fopen("sequences.txt", "r");

       while (fgets(buffer,bufferLength,fp)) {
	       //"buffer" refers to each line in the sequences.txt file. If the line starts with a "0", that means it is a healthy sequence. The number of healthy cells is incremented if so.
		if (buffer[0]=='0') {
			healthyCellNum+=1;
			

//I tokenized each line by using the strtok function. Noting that the first two characters of the line are 0 or 1 and the '\t' character, which are not needed when searching for words, thus I tokenized "buffer+2" instead of "buffer".
			char *token= strtok(buffer+2, " ");
		
			while ( token != NULL) 	{

				//Note that the words that are at the ends of each line end with the new line character, which the strcmp function will consider. For this new line character to be removed, I substituted it with a null terminating character to end the string without the '\n' character.
				if (token[strlen(token)-1]== '\n') {
					token[strlen(token)-1] = '\0';
				}

		//"argv[2]" is the word we are looking for. strcmp function will return 0 if the token and the word are equal. If they are, I incremented the frequency of the word in healthy sequences by 1.
			if (strcmp(token, argv[2])==0) {
				wordHNum+=1;
				}

			token= strtok(NULL, " "); //This line continues making tokens until the while loop is over.
			}
				}


		//The rest of this if statement is the exact same as the previous one, this statement being for cancerous sequences, and the variables updated are storing the values for cancerous sequences.

		if (buffer[0]=='1') {
			cancerousCellNum+=1;

			char *token= strtok(buffer+2, " ");
			while ( token != NULL) {
				if (token [strlen(token)-1] == '\n') {
					token[strlen(token)-1] = '\0';
				}
			if (strcmp(token, argv[2])==0) {
			wordCNum+=1;
					}
			token= strtok(NULL, " ");
				}
			}

			
		}


		       


       fclose(fp); //Closed the file.

       		//If the word does not exist in any sequence, prints this message.
		if(wordHNum==0 && wordCNum == 0) {
			printf("This word doesn't occur in the text!\n");
			return 1;
		}

		 tfHScore= (float) wordHNum / healthyCellNum; //Calculated the tf score for healthy sequences.
	   	tfCScore= (float) wordCNum / cancerousCellNum;	//Calculated the tf score for cancerous sequences.

		//Printf statements to match the sample output.
		printf("Total Healthy: %d\n", healthyCellNum);
		printf("Word in Healthy: %d\n", wordHNum);
		printf("Calculated tf value for this word: %.6f\n", tfHScore);
		printf("Total Cancerous: %d\n", cancerousCellNum);
		printf("Word in Cancerous: %d\n", wordCNum);
		printf("Calculated tf value for this word: %.6f\n", tfCScore);


		//If the wordHNum is larger than wordCNum, that means that the word occurs more in healthy sequences, and cancerous sequences otherwise. I put two statements here to check this.

		if (wordHNum>wordCNum) {
		printf("This word is mostly used for healthy sequences.\n");
		}

		else if (wordCNum>wordHNum) {
		printf("This word is mostly used for cancerous sequences.\n");
		}

        
    }
    else if (strcmp(argv[1], "predict") == 0) // cancerous sequence detection
    {
        float tfidfHScore=0; //This variable stores the total tf-idf score of the 8 word sequence for healthy sequences.
	float tfidfCScore=0; //This variable stores the total tf-idf score of the 8 word sequence for cancerous sequences.

//The for loop iterates over each word, argv[2] through argv[9].
for (int i=2; i<=9;i++) {

	//The following 3 variables are the same as task 1, for the reading of the file.
 	FILE * fp;
       int bufferLength= 255;
       char buffer[bufferLength];

	int cancerousCellNum= 0; //Stores the number of cancerous sequences.
	int healthyCellNum= 0; //Stores the number of healthy sequences.
	int idfWordCNum= 0; //Stores the number of cancerous sequences that include the word.
	int idfWordHNum=0; //Stores the number of healthy sequences that include the word.
	int tfWordCNum=0; //Stores the frequency of a word in cancerous sequences.
	int tfWordHNum=0; //Stores the frequency of a word in healthy sequences.
	float wordTfCScore= 0; //Stores the tf score of a word for the cancerous class.
	float wordTfHScore= 0; //Stores the tf score of a word for the healthy class.
	float wordIdfScore=0; //Stores the idf score of a word.


	
       fp= fopen("sequences.txt", "r"); //Opens the file for reading.

       //Reading the file is the same as task 1, I used the information provided by the user "malat" in "Link 2" to write the file reading functions.

       while (fgets(buffer,bufferLength,fp)) {
	       //The "wordInSequence" variable stores whether a given word is included in a line or not. The difference between the tf score and the idf score is that we need the frequency of the word for the tf score, and we need the number of lines that contain the word for the idf score, meaning that we will not count the word twice for the idf score if it is included in the same line.

	       bool wordInSequence= false; 

	       //This part is the same as task 1.
		if (buffer[0]=='0') {
			healthyCellNum+=1;

//The tokenization process is the same as task 1.
			char *token2= strtok(buffer+2, " ");
			while ( token2 != NULL) {
				if (token2[strlen(token2)-1]== '\n') {
					token2[strlen(token2)-1] = '\0';
				}
	//Tries to find the argument in the created tokens.
			if (strcmp(token2,argv[i])==0) {
				tfWordHNum+=1;
				//If the word is included in the line, the "wordInSequence" variable is updated into "true". 
				wordInSequence=true;
				}

				token2 = strtok(NULL, " ");
			}
			
			//The end of the while loop above indicates that the line has been finished. Therefore, all the words in the line have been checked. If "wordInSequence" was somehow set to "true", then that means that the tokenized line contains the word. Therefore, the "idfWordHNum" variable is incremented by 1.
			if (wordInSequence) {
				idfWordHNum++;
			}

		}

				
		//Everything here is the same, except for the fact that the cells are cancerous, and I will be updating the variables that store information about the cancerous sequences.
		if (buffer[0]=='1') {
			cancerousCellNum+=1;

			char *token4= strtok(buffer+2, " ");
			while ( token4 != NULL) {
				if (token4[strlen(token4)-1]== '\n') {
					token4[strlen(token4)-1] = '\0';
				}
			
			if (strcmp(token4, argv[i])==0) {
					tfWordCNum+=1;
					wordInSequence= true;
				}

			token4= strtok(NULL, " ");
			}

		if (wordInSequence) {
				idfWordCNum++;
		}	

			
		}


		       } //The large while loop which reads the file ends here.

  

       		fclose(fp); //Closes the file.
			
			
		wordTfHScore= (float) tfWordHNum/ healthyCellNum; //Calculates the tf score of the word for healthy sequences.
		wordTfCScore= (float) tfWordCNum/ cancerousCellNum; //Calculates the tf score of the word for cancerous sequences.

		//If the word's tf score is completely 0 for both sequences, then a division by zero error may occur. The instructions told us to count the word's idf score as "0", so simply skipping the word will suffice. 
		if (tfWordCNum==0 & tfWordHNum==0) {
			continue;
		}
		
		else {
			wordIdfScore= logf((float)(cancerousCellNum + healthyCellNum) / (idfWordCNum + idfWordHNum) ); //Calculates the idf score of the word given the formula provided by the assignment instructions.

		}
			
		tfidfHScore += (wordIdfScore * wordTfHScore); //Increments the total tf-idf score of the 8 arguments for healthy sequences by the tf-idf score of the word for healthy sequences.
		tfidfCScore += (wordIdfScore * wordTfCScore); //Increments the total tf-idf score of the 8 arguments for cancerous sequences by the tf-idf score of the word for cancerous sequences.


	} //The outermost for loop for the 8 arguments ends here.



	//Some printf statements.
	printf("Total tf-idf score from healthy messages for the sequence: %.6f\n", tfidfHScore);
	printf("Total tf-idf score from cancerous sequence for the sequence: %.6f\n", tfidfCScore);

	//If the tf-idf score of the sequence is larger for healthy sequences, prints that the sequence is not cancerous.

	if(tfidfHScore> tfidfCScore) {
	printf("This sequence is not cancerous.\n");
	}
//If the cancerous sequences score is larger, prints that the sequence is cancerous.
	else if (tfidfCScore> tfidfHScore) {
	printf("This sequence is cancerous.\n");
	}

//If both scores are zero, prints the given statement.
	else if (tfidfCScore == 0 & tfidfHScore == 0) {
	printf("Tf-idf scores are found to be 0, cancerous sequence detection failed!\n");
	}



    }
    else
    	{
        printf("Wrong Function!\n");
    }

    return 0;
}
