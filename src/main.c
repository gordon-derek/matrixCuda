
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MovieRating.h"

//constants
#define BASE_SIZE 80000;
#define TEST_SIZE 20000;
#define OBJ_SIZE 16;

typedef struct MovieRating MovieRating;
typedef enum { false, true } bool;


//given filenames for both base and testing datasets, this method creates the datasets
//in vectors of the MovieRating object to perform tests on
int LoadRatings (/* in */ char* baseF,        //base(training) dataset filename
                  /* in */ char* testF,        //test dataset filename
                  /* out */ MovieRating* base, //base(training) dataset vector
                  /* out */ MovieRating* test);//test dataset vector

//upon input of a specifically formatted string, the string is parsed and
//created into a populated MovieRating object.
//format:
//  uid mid rating timestamp(unix seconds)
MovieRating StringToRating(/* in */char line[]); //string to be processed into rating

//checks to see if the given character is whitespace
bool IsWhiteSpace(/* in */char c); //character to be tested

int main(){
  //base and test dataset containers
  int bs = BASE_SIZE;
  int ts = TEST_SIZE;
  MovieRating base[bs], test[ts];
  char baseName[] = "assets/u1.base";
  char testName[] = "assets/u1.test";

  printf("******************Loading Dataset 1******************\n");
  if(LoadRatings(baseName, testName, base, test) == 1)
    printf("Loading Dataset 1 has failed, exiting.");
  

  return 0;
}

//Pre: Vectors have been initialized to proper size and strings contain valid filenames
//Post: Vectors have been populated by the corresponding files
//Purpose: reads the data in from the file and populates the corrosponding vector for tests
int LoadRatings (/* in */ char* baseName,
                  /* in */ char* testName,
                  /* out */ MovieRating* base,
                  /* out */ MovieRating* test){
  printf("here");
  //load the base data set
  char line [35];
  FILE * baseF = fopen(baseName, "rt");
  FILE * testF = fopen(testName, "rt");


  if(baseF == NULL){
    printf("Unable to open File. Program Terminating...");
    return 1;
  }//end if

  //load testing dataset
  if(testF == NULL){
    printf("Unable to open File. Program Terminating...");
    return 1;
  }//end if
  printf("here");
  //process base file, line by line
  int i = 0;
  while(fgets(line, sizeof line, baseF) != NULL){
    //convert line to MovieRating object
    //place in vector
    base[i] = StringToRating(line);
    i++;
  }//end while

  fclose(baseF);
  printf("here");

  //process test file, line by line
  i = 0;
  while(fgets(line, sizeof line, testF) != NULL){
    //convert line to MovieRating object
    //place in vector
    test[i] = StringToRating(line);
    i++;
  }//end while
  return 0;
}//end LoadRatings

//Pre: string contains a properly formatted line from a dataset file
//Post: A populated MovieRating object is returned
//Purpose: to convert a string that is properly formatted to a MovieRating object
MovieRating StringToRating(/* in */char line[]){
  MovieRating r;  //to process string into
  int numCat = 0; //tells which catagory to input in
  char *item;
  int i;
  //process line
  for(i = 0; i <= sizeof line; i++){
    //whitespace signifies end of item
    if(!IsWhiteSpace(line[i]) && i != sizeof line)
      item += line[i];
    else{
      //places item in the correct catagory
      if(sizeof item > 0){
        switch(numCat){
          case 0: r.uId = atoi(item); numCat++; break;
          case 1: r.mId = atoi(item); numCat++; break;
          case 2: r.rating = atoi(item); numCat++; break;
          case 3: r.timestamp = atoi(item); numCat++; break;
          default: break;
        }//end switch
        item = "";
      }//end if
    }//end else 
  }//end for
  return r;
}//end StringToRating

//checks given character to see if it is whitespace
bool IsWhiteSpace(/* in */ char c){
    switch(c){
    case ' ':
    case '\t':
    case '\r':
    case '\n': return true;
    default: return false;
    }//end switch
}//end IsWhiteSpace