/*
  Author: Derek Gordon
  Date: October 1, 2013
  Purpose:
  Given a Properly Formatted Dataset of Movie Ratings, This program
  will read in the movie ratings from files into two datasets, a test
  and training set.  The training set is used to teach the system by
  calculating movie ratings against the training and correct itself
  in the linear algebra cases or in the standard elementary cases it
  calculates a mean from the training set to test against the testing dataset.
*/
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
MovieRating StringToRating(/* in */char * line, ssize_t read); //string to be processed into rating

//checks to see if the given character is whitespace
bool IsWhiteSpace(/* in */char c); //character to be tested

//calculates the Global Mean of all the ratings in the base set and tests
// them against the test set and calculates the root mean squared error
float GlobalMeanValue(/* in */ const MovieRating * base,
                     /* in */ const MovieRating * test);

//calculates the mean rating for a given user from the base set,
// does this for every user in the test set to populate a comparison vector,
// test the comparison vector against the test set to calculate the root mean squared error
float UserMeanValue(/* in */const MovieRating * base,
                   /* in */const MovieRating * test);

//calculates the mean rating for a given movie from the base set,
// does this for every movie in the test set to populate a comparison vector,
// test the comparison vector against the test set to calculate the root mean squared error
float MovieMeanValue(/* in */const MovieRating * base,
                   /* in */const MovieRating * test);

float RMSECalc(/* in */ const MovieRating * expected,
               /* in */ const MovieRating * test);

int main(){
  //base and test dataset containers
  int bs = BASE_SIZE;
  int ts = TEST_SIZE;
  MovieRating base[bs], test[ts];
  char baseName[] = "assets/u1.base";
  char testName[] = "assets/u1.test";
  float RMSE[5][3];

  printf("\n******************Loading Dataset 1******************\n");
  if(LoadRatings(baseName, testName, base, test) == 1)
    printf("Loading Dataset 1 has failed, exiting.");
  RMSE[0][0] = GlobalMeanValue(base, test);
  RMSE[0][1] = UserMeanValue(base, test);
  RMSE[0][2] = MovieMeanValue(base, test);

  strncpy(baseName, "assets/u2.base", sizeof baseName);
  strncpy(testName, "assets/u2.test", sizeof testName);

  printf("\n******************Loading Dataset 2******************\n");
  if(LoadRatings(baseName, testName, base, test) == 1)
    printf("Loading Dataset 2 has failed, exiting.");
  RMSE[1][0] = GlobalMeanValue(base, test);
  RMSE[1][1] = UserMeanValue(base, test);
  RMSE[1][2] = MovieMeanValue(base, test);

  strncpy(baseName, "assets/u3.base", sizeof baseName);
  strncpy(testName, "assets/u3.test", sizeof testName);

  printf("\n******************Loading Dataset 3******************\n");
  if(LoadRatings(baseName, testName, base, test) == 1)
    printf("Loading Dataset 3 has failed, exiting.");
  RMSE[2][0] = GlobalMeanValue(base, test);
  RMSE[2][1] = UserMeanValue(base, test);
  RMSE[2][2] = MovieMeanValue(base, test);

  strncpy(baseName, "assets/u4.base", sizeof baseName);
  strncpy(testName, "assets/u4.test", sizeof testName);

  printf("\n******************Loading Dataset 4******************\n");
  if(LoadRatings(baseName, testName, base, test) == 1)
    printf("Loading Dataset 4 has failed, exiting.");
  RMSE[3][0] = GlobalMeanValue(base, test);
  RMSE[3][1] = UserMeanValue(base, test);
  RMSE[3][2] = MovieMeanValue(base, test);

  strncpy(baseName, "assets/u5.base", sizeof baseName);
  strncpy(testName, "assets/u5.test", sizeof testName);

  printf("\n******************Loading Dataset 5******************\n");
  if(LoadRatings(baseName, testName, base, test) == 1)
    printf("Loading Dataset 5 has failed, exiting.");
  RMSE[4][0] = GlobalMeanValue(base, test);
  RMSE[4][1] = UserMeanValue(base, test);
  RMSE[4][2] = MovieMeanValue(base, test);

  //AVERAGES
  printf("\n******************Averages***************************\n");
  printf("Global Mean RMSE: %f\n", (RMSE[0][0] + RMSE[1][0] + RMSE[2][0] + RMSE[3][0] + RMSE[4][0])/5);
  printf("User Mean RMSE: %f\n", (RMSE[0][1] + RMSE[1][1] + RMSE[2][1] + RMSE[3][1] + RMSE[4][1])/5);
  printf("Movie Mean RMSE: %f\n", (RMSE[0][2] + RMSE[1][2] + RMSE[2][2] + RMSE[3][2] + RMSE[4][2])/5);

  return 0;
}

//Pre: Vectors have been initialized to proper size and strings contain valid filenames
//Post: Vectors have been populated by the corresponding files
//Purpose: reads the data in from the file and populates the corrosponding vector for tests
int LoadRatings (/* in */ char* baseName,
                  /* in */ char* testName,
                  /* out */ MovieRating* base,
                  /* out */ MovieRating* test){
  //printf("here");
  //load the base data set
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

  //process base file, line by line

  int i = 0;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;


  while ((read = getline(&line, &len, baseF)) != -1) {
    //printf("Retrieved line of length %zu :\n", read);
    //printf("%s\n", line);
    MovieRating r = StringToRating(line, read);
    base[i] = r;
    //printf("rating - %f\n", base[i].rating);
    i++;
  }

 

  fclose(baseF);

  i=0;
  while ((read = getline(&line, &len, testF)) != -1) {
    //convert line to MovieRating Object
    MovieRating r = StringToRating(line, read);
    test[i] = r;
    i++;
  }

  return 0;
}//end LoadRatings

//Pre: string contains a properly formatted line from a dataset file
//Post: A populated MovieRating object is returned
//Purpose: to convert a string that is properly formatted to a MovieRating object
MovieRating StringToRating(/* in */char * line, ssize_t read){
  MovieRating r;  //to process string into
  int numCat = 0; //tells which catagory to input in
  char *item = malloc(10);
  int iPos = 0;
  int i;
  //printf("%zu", read);
  //process line
  for(i = 0; i <= read; i++){
    //whitespace signifies end of item
    if(!IsWhiteSpace(line[i]) && i != read){
      item[iPos] = line[i];
      iPos++;
    }
    else{
      //places item in the correct catagory
      //printf(item); printf("\n");
      if(sizeof item > 0){
        switch(numCat){
          case 0: r.uId = atoi(item); numCat++; break;
          case 1: r.mId = atoi(item); numCat++; break;
          case 2: r.rating = atoi(item); numCat++; break;
          case 3: r.timestamp = atoi(item); numCat++; break;
          default: break;
        }//end switch
        free(item);
        char *item = malloc(10);
        iPos=0;
        //printf(item);
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

//Pre: vectors are both populated
//Post: The Root Mean Squared Error is calculated for this specific test
//Purpose: to process the base vector to calculate a global mean movie rating
// this rating is tested against the test vector to calculate an RMSE.
float GlobalMeanValue(/* in */const MovieRating * base,
                     /* in */const MovieRating * test){
	int ts = TEST_SIZE;
	int bs = BASE_SIZE;
  MovieRating expected[ts];
  int i;
  //add all ratings in base vector together
  float gMean = 0.0f;
  for(i = 0; i < bs; i++)
    gMean = gMean + base[i].rating;

  //divide by the number of ratings
  gMean = (float)gMean/bs;

  //fill the comparison vector with the Global Mean rating
  for(i = 0; i < ts; i++){
    expected[i] = test[i];
    expected[i].rating = gMean;
  }//end for

  float result = RMSECalc(expected, test);
    //calculate root mean squared error
  printf("Global Mean RMSE: %f\n", result);
  return result;
}//end GlobalMeanValue

//Pre: vectors are both populated
//Post: The Root Mean Squared Error is calculated for this specific test
//Purpose: to process the base vector to calculate a user mean movie rating
// for every user that has a rating in the test data, this rating is tested 
// against the test vector to calculate an RMSE.
float UserMeanValue(/* in */const MovieRating * base,
                   /* in */const MovieRating * test){
  int ts = TEST_SIZE;
  int bs = BASE_SIZE;
  MovieRating expected[ts];

  int i;
  int j;
  //copy the test data into a comparison vector
  for(i = 0; i < ts; i++){
    expected[i] = test[i];
    //-1 signifies it hasn't had a usermean calculated from base set
    expected[i].rating = -1;
  }//end for

  //cycle through expected vector and for every rating of -1,
  // go through the base and calculate the average of all the movie
  // ratings given by the same user.
  for(i = 0; i < ts; i++){
    //for user mean calculation
    int rateNum = 0;
    int uSum = 0;

    if(expected[i].rating == -1){
    //cycle base calculate the user mean for the given user
      for(j = 0; j < bs; j++){
        if(base[j].uId == expected[i].uId){
          uSum += base[j].rating;
          rateNum++;
        }//end if
      }//end for
      //place the newly calculated user mean in all ratings
      // done by this user in the expected vector
      for(j = i; j < ts; j++){
        if(expected[j].uId == expected[i].uId)
          expected[j].rating = (float)uSum/(float)rateNum;
      }//end for
    }//end if
  }//end for

  float result = RMSECalc(expected, test);
  //calculate root mean squared error
  printf("User Mean RMSE: %f\n", result);
  return result; 
}

//Pre: vectors are both populated
//Post: The Root Mean Squared Error is calculated for this specific test
//Purpose: to process the base vector to calculate a mean movie rating
// from the base set for every movie in the test set, this rating is tested
// against the test vector to calculate an RMSE.
float MovieMeanValue(/* in */const MovieRating * base,
                   /* in */const MovieRating * test){
  int ts = TEST_SIZE;
  int bs = BASE_SIZE;
  MovieRating expected[ts];

  int i;
  int j;

  //copy test vector into a comparison vector
  for(i = 0; i < ts; i++){
    expected[i] = test[i];
    //-1 signifies a movie mean hasn't been calculated for it yet
    expected[i].rating = -1;
  }//end for

  //for total movie mean calculation, the total movie mean is used for
  // movies that have no ratings in the base set
  int totalMSum = 0;
  int totalRateNum = 0;

  for(i = 0; i < ts; i++){
    int rateNum = 0;
    int mSum = 0;
    if(expected[i].rating == -1){
      for(j = 0; j < bs; j++){
        if(base[j].mId == expected[i].mId){
          mSum += base[j].rating;
          rateNum++;
        }//end if
      }//end for
      if(rateNum != 0){       
        for(j = i; j < ts; j++){
          if(expected[j].mId == expected[i].mId){
            expected[j].rating = (float)mSum/(float)rateNum;
          }//end if
        }//end for
      }//end if
    }//end if
    totalMSum += mSum;
    totalRateNum += rateNum;
  }//end for

  //fill all movies that did not have a rating in the base set with the
  //movie mean for all movies that were calculated
  for(i = 0; i < ts; i++){
    if(expected[i].rating == -1)
      expected[i].rating = (float)totalMSum/(float)totalRateNum;
  }//end for

  float result = RMSECalc(expected, test);
  //calculate root mean squared error
  printf("Movie Mean RMSE: %f\n", result);
  return result;
}//end MovieMeanValue

//Pre: test vector is the test dataset, expected is a copy of test but the
// ratings in expected are calculated from base using a method instead of given.
//Post: the root mean squared error between expected and test is calculated
//Purpose: to calculate the root mean squared error to see how good a test is.
float RMSECalc(/* in */ const MovieRating * expected,
               /* in */ const MovieRating * test){
	  int ts = TEST_SIZE;
    float sumS = 0.0f;
    int i;
    for(i = 0; i < ts; i++){
        float s = (expected[i].rating - test[i].rating)*(expected[i].rating - test[i].rating);
        sumS = sumS + s;
    }//end for

    return (float)sqrt((float)1/(float)ts*sumS);
}//end RMSECalc