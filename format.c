/*
 * format.c
 *
 *  Created on: Apr 19, 2016
 *  Author: Daniel O'Connell
 *  NUID: 12992765
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

int compare_string(char *first, char *second);
int sort(const void * a, const void * b ) ;
int strcasecmp(char *a, char *b);

struct wordWithCount{
    char *word;
    int occurrances;
};

int main(int argc, char *argv[]){
    /*create variables*/
    int lineLength,fileSize,i,wordCounter;
    char outFileName[32];
    char wordFileName[32];
    char currentLine[256];
    char *words, *contents, *temp;
    FILE *infile,*outfile,*wordfile;
    struct wordWithCount map[256];

    /*check arg count*/
    if(argc > 3){
        printf("Expected usage:\n./format <length_of_line> <input_filename>\n");
        return 1;
    }

    /*get arguments*/
    lineLength = atoi(argv[1]);
    infile = fopen(argv[2],"r");

    /*create .out filename*/
    strcpy(outFileName,argv[2]);
    strcat(outFileName, ".out");

    /*create .words filename*/
    strcpy(wordFileName,argv[2]);
    strcat(wordFileName, ".words");

    /*open both files*/
    outfile = fopen(outFileName,"w");
    wordfile = fopen(wordFileName,"w");

    /*init array*/
    currentLine[0] = '\0';
    temp = (char*) malloc(32*sizeof(char));

    /*prepare input file for reading*/
    fseek(infile,0,SEEK_END);
    fileSize = ftell(infile);
    rewind(infile);
    contents = malloc(fileSize * (sizeof(char)));
    fread(contents,sizeof(char),fileSize,infile);

    words = strtok(contents," \n\0");
    map[0].word = words;
    wordCounter = 0;

    /*
     * create formatted text file (.out)
     */
    while(words != NULL){
        map[wordCounter].word = words;
        map[wordCounter].occurrances = 1;
        for(i = 0; i < wordCounter; i++){
            if(compare_string(words,map[i].word) == 0){
                /*words match*/
                map[wordCounter].word = "~~~~";
                map[i].occurrances++;
            }
        }
        /*check if current word will fit*/
		if(strlen(words) + strlen(currentLine) + 1 < lineLength){
			/*check if line is empty*/
		    if(strlen(currentLine) > 0){
				strcat(currentLine," ");
			}
			strcpy(temp,words);
			strcat(currentLine, temp);
        }
        else{
            fprintf(outfile,"%s\n",currentLine);
            strcpy(currentLine,words);
        }
        words = strtok(NULL, " \n");
        wordCounter++;
    }
    /*print what remains in buffer*/
    fprintf(outfile,"%s\n", currentLine);
    wordCounter--;

    /*sort structs and create .words file*/
    qsort(map,wordCounter,sizeof(struct wordWithCount),sort);

    for(i = 0; i < wordCounter + 1; i++){
        if(strcmp(map[i].word,"~~~~") != 0){
            fprintf(wordfile, "%s - %d\n", map[i].word,map[i].occurrances);
        }
    }
    free(temp);
    free(contents);
    free(words);
    fclose(infile);
    fclose(outfile);

    return 0;
}

int compare_string(char *first, char *second){
   while(toupper(*first)==toupper(*second)){
      if (toupper(*first) == '\0' || toupper(*second) == '\0'){
          break;
      }
      first++;
      second++;
   }
   if(toupper(*first) == '\0' && toupper(*second) == '\0'){
       return 0;
   }
   else{
       return -1;
   }
}

int sort(const void * a, const void * b ) {
  return strcasecmp(*(char **)a, *(char **)b);
}
