/*
   Author: Derek Gordon
   Date: May 31st, 2013
   Purpose: Client that will create a MovieRatings object and use it
	to test matrix completion given certain tests.
*/

//Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "math.h"
#include "MovieRating.h"

using namespace std;

//constants
const int BASE_SIZE = 80000;
const int TEST_SIZE = 20000;
const int OBJ_SIZE = 16;

//upon input of a string contains a number, it converts that string to an integer
int strtoi (/* in */const string& str);    //string to be converted into an int

//upon input of an int, it converts that int to a string containing that number
string itos (/* in */int n);    //int to be converted to string

//given filenames for both base and testing datasets, this method creates the datasets
//in vectors of the MovieRating object to perform tests on
void LoadRatings (/* in */ string baseF,				//base(training) dataset filename
                  /* in */ string testF,				//test dataset filename
                  /* out */ vector<MovieRating> & base, //base(training) dataset vector
                  /* out */ vector<MovieRating> & test);//test dataset vector

//upon input of a specifically formatted string, the string is parsed and
//created into a populated MovieRating object.
//format:
//	uid mid rating timestamp(unix seconds)
MovieRating StringToRating(/* in */string line); //string to be processed into rating

//checks to see if the given character is whitespace
bool IsWhiteSpace(/* in */char c); //character to be tested

//calculates the Global Mean of all the ratings in the base set and tests
// them against the test set and calculates the root mean squared error
void GlobalMeanValue(/* in */ const vector<MovieRating> & base,
                     /* in */ const vector<MovieRating> & test);

//calculates the mean rating for a given user from the base set,
// does this for every user in the test set to populate a comparison vector,
// test the comparison vector against the test set to calculate the root mean squared error
void UserMeanValue(/* in */const vector<MovieRating> & base,
                   /* in */const vector<MovieRating> & test);

//calculates the mean rating for a given movie from the base set,
// does this for every movie in the test set to populate a comparison vector,
// test the comparison vector against the test set to calculate the root mean squared error
void MovieMeanValue(/* in */const vector<MovieRating> & base,
                   /* in */const vector<MovieRating> & test);

void MovieUserMean(/* in */const vector<MovieRating> & base,
                   /* in */const vector<MovieRating> & test);

//takes in two vectors, expected contains all the calculated ratings for the movies,
// test is the test dataset which contains all the real ratings.  RMSE then calculates
// the root mean squared error for the given experiment.
float RMSECalc(/* in */ const vector<MovieRating> & expected,
               /* in */ const vector<MovieRating> & test);

int main(){
	//base and test dataset containers
    vector<MovieRating> base(BASE_SIZE);
    vector<MovieRating> test(TEST_SIZE);

	//load and test the first datasets
    cout << "*************Loading Dataset 1*************\n" << endl;
    try{
        LoadRatings("assets/u1.base", "assets/u1.test", base, test);
    }catch(int e){
		system("Pause");
		return e;
	}

	//run tests
    GlobalMeanValue(base, test);
    UserMeanValue(base, test);
    MovieMeanValue(base,test);
	MovieUserMean(base,test);

	//load and test the second datasets
    cout << "\n*************Loading Dataset 2*************\n" << endl;
    try{
        LoadRatings("assets/u2.base", "assets/u2.test", base, test);
    }catch(int e){
		system("Pause");
		return e;
	}//end try/catch

	//run tests
    GlobalMeanValue(base, test);
    UserMeanValue(base, test);
	MovieMeanValue(base,test);

	//load and test the third datasets
    cout << "\n*************Loading Dataset 3*************\n" << endl;
    try{
        LoadRatings("assets/u3.base", "assets/u3.test", base, test);
    }catch(int e){
		system("Pause");
		return e;
	}//end try/catch

	//run tests
    GlobalMeanValue(base, test);
    UserMeanValue(base, test);
	MovieMeanValue(base,test);

	//load and test the fourth datasets
    cout << "\n*************Loading Dataset 4*************\n" << endl;
    try{
        LoadRatings("assets/u4.base", "assets/u4.test", base, test);
    }catch(int e){
		system("Pause");
		return e;
	}//end try/catch

	//run tests
    GlobalMeanValue(base, test);
    UserMeanValue(base, test);
	MovieMeanValue(base,test);

	//load and test the fifth datasets
    cout << "\n*************Loading Dataset 5*************\n" << endl;
    try{
        LoadRatings("assets/u5.base", "assets/u5.test", base, test);
    }catch(int e){
		system("Pause");
		return e;
	}//end try/catch

	//run tests
    GlobalMeanValue(base, test);
    UserMeanValue(base, test);
	MovieMeanValue(base,test);

	system("Pause");
	return 0;
}

//converts a given string to an int
int strtoi (/* in */const string& str) {
    stringstream ss(str);
    int n;
    ss >> n;
    return n;
}//end strtoi

//converts a given int to a string
string itos(/* in */int n){
    stringstream ss;
    ss << n;
    return ss.str();
}//end itos

//Pre: Vectors have been initialized to proper size and strings contain valid filenames
//Post: Vectors have been populated by the corresponding files
//Purpose: reads the data in from the file and populates the corrosponding vector for tests
void LoadRatings (/* in */ string baseName,
                  /* in */ string testName,
                  /* out */ vector<MovieRating> & base,
                  /* out */ vector<MovieRating> & test){

    //load the base data set
    string line ="";
	ifstream baseF(baseName.c_str());
	ifstream testF(testName.c_str());
	if(baseF.fail()){
		cout << "Unable to open File. Program Terminating..."<<endl;
		throw(1);
	}//end if

	//process base file, line by line
	int i = 0;
	getline(baseF, line);
	while(baseF && i < BASE_SIZE){
		//convert line to MovieRating object
		//place in vector
		base[i] = StringToRating(line);
        i++;
        getline(baseF,line);
	}//end while

	baseF.close();

	//load testing dataset
	if(testF.fail()){
		cout << "Unable to open File. Program Terminating..."<<endl;
		throw(1);
	}//end if

	//process test file, line by line
	i = 0;
	getline(testF, line);
	while(testF && i < TEST_SIZE){
		//convert line to MovieRating object
		//place in vector
        test[i] = StringToRating(line);
        i++;
        getline(testF,line);
	}//end while

}//end LoadRatings

//Pre: string contains a properly formatted line from a dataset file
//Post: A populated MovieRating object is returned
//Purpose: to convert a string that is properly formatted to a MovieRating object
MovieRating StringToRating(/* in */string line){
    MovieRating r;	//to process string into
    int numCat = 0; //tells which catagory to input in
    string item;
	//process line
    for(int i = 0; i <= line.size(); i++){
		//whitespace signifies end of item
        if(!IsWhiteSpace(line[i]) && i != line.size())
            item += line[i];
        else{
			//places item in the correct catagory
            if(item.size() > 0){
                switch(numCat){
                    case 0: r.uId = strtoi(item); numCat++; break;
                    case 1: r.mId = strtoi(item); numCat++; break;
                    case 2: r.rating = strtoi(item); numCat++; break;
                    case 3: r.timestamp = strtoi(item); numCat++; break;
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


//Pre: vectors are both populated
//Post: The Root Mean Squared Error is calculated for this specific test
//Purpose: to process the base vector to calculate a global mean movie rating
// this rating is tested against the test vector to calculate an RMSE.
void GlobalMeanValue(/* in */const vector<MovieRating> & base,
                     /* in */const vector<MovieRating> & test){
    vector<MovieRating> expected(TEST_SIZE);

	//add all ratings in base vector together
    float gMean = 0.0f;
    for(int i = 0; i < BASE_SIZE; i++)
        gMean += base[i].rating;

	//divide by the number of ratings
    gMean = (float)gMean/BASE_SIZE;

	//fill the comparison vector with the Global Mean rating
    for(int i = 0; i < TEST_SIZE; i++){
        expected[i] = test[i];
        expected[i].rating = gMean;
    }//end for


    //calculate root mean squared error
    cout << "Global Mean RMSE: " << RMSECalc(expected, test) << endl;
}//end GlobalMeanValue

//Pre: vectors are both populated
//Post: The Root Mean Squared Error is calculated for this specific test
//Purpose: to process the base vector to calculate a user mean movie rating
// for every user that has a rating in the test data, this rating is tested 
// against the test vector to calculate an RMSE.
void UserMeanValue(/* in */const vector<MovieRating> & base,
                   /* in */const vector<MovieRating> & test){
    vector<MovieRating> expected(TEST_SIZE);

	//copy the test data into a comparison vector
    for(int i = 0; i < TEST_SIZE; i++){
        expected[i] = test[i];
		//-1 signifies it hasn't had a usermean calculated from base set
        expected[i].rating = -1;
    }//end for

	//cycle through expected vector and for every rating of -1,
	// go through the base and calculate the average of all the movie
	// ratings given by the same user.
    for(int i = 0; i < TEST_SIZE; i++){
		//for user mean calculation
        int rateNum = 0;
        int uSum = 0;

        if(expected[i].rating == -1){
			//cycle base calculate the user mean for the given user
            for(int j = 0; j < BASE_SIZE; j++){
                if(base[j].uId == expected[i].uId){
                    uSum += base[j].rating;
                    rateNum++;
                }//end if
            }//end for
			//place the newly calculated user mean in all ratings
			// done by this user in the expected vector
            for(int j = i; j < TEST_SIZE; j++){
                if(expected[j].uId == expected[i].uId)
                    expected[j].rating = (float)uSum/(float)rateNum;
            }//end for
        }//end if
    }//end for

	//calculate root mean squared error
    cout << "User Mean RMSE: " << RMSECalc(expected, test) << endl;
}//end UserMeanValue

//Pre: vectors are both populated
//Post: The Root Mean Squared Error is calculated for this specific test
//Purpose: to process the base vector to calculate a mean movie rating
// from the base set for every movie in the test set, this rating is tested
// against the test vector to calculate an RMSE.
void MovieMeanValue(/* in */const vector<MovieRating> & base,
                   /* in */const vector<MovieRating> & test){
    vector<MovieRating> expected(TEST_SIZE);

	//copy test vector into a comparison vector
    for(int i = 0; i < TEST_SIZE; i++){
        expected[i] = test[i];
		//-1 signifies a movie mean hasn't been calculated for it yet
        expected[i].rating = -1;
    }//end for

	//for total movie mean calculation, the total movie mean is used for
	// movies that have no ratings in the base set
	int totalMSum = 0;
	int totalRateNum = 0;

    for(int i = 0; i < TEST_SIZE; i++){
        int rateNum = 0;
        int mSum = 0;
        if(expected[i].rating == -1){
            for(int j = 0; j < BASE_SIZE; j++){
                if(base[j].mId == expected[i].mId){
                    mSum += base[j].rating;
                    rateNum++;
                }//end if
            }//end for
            if(rateNum != 0){       
				for(int j = i; j < TEST_SIZE; j++){
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
	for(int i = 0; i < TEST_SIZE; i++){
		if(expected[i].rating == -1)
			expected[i].rating = (float)totalMSum/(float)totalRateNum;
	}//end for

	//calculate root mean squared error
    cout << "Movie Mean RMSE: " << RMSECalc(expected, test) << endl;
}//end MovieMeanValue

//Pre:
//Post:
//Purpose:
void MovieUserMean(/* in */const vector<MovieRating> & base,
                   /* in */const vector<MovieRating> & test){
	vector<MovieRating> expected;

	for(int i = 0; i < TEST_SIZE; i++){
		expected[i] = test[i];
		//-1 signifies a movie that has not had a calculated rating
		expected[i].rating = -1;
	}

	for(int i = 0; i < TEST_SIZE; i++){
        int mRateNum = 0;
        int mSum = 0;
		int uRateNum = 0;
		int uSum = 0;
        if(expected[i].rating == -1){
            for(int j = 0; j < BASE_SIZE; j++){
                if(base[j].mId == expected[i].mId){
                    mSum += base[j].rating;
                    mRateNum++;
                }//end if
				else if(base[j].uId == expected[i].uId){
					uSum += base[j].rating;
					uRateNum++;
				}
            }//end for
			float mMean = (float)mSum/(float)mRateNum;
			float uMean = (float)uSum/(float)uRateNum;
			if(abs(test[i].rating - mMean) - abs(test[i].rating - uMean) >= 0)
				expected[i].rating = uMean;
			else
				expected[i].rating = mMean;
			
			
        }//end if
    }//end for

	//calculate root mean squared error
    cout << "Movie/User Mean RMSE: " << RMSECalc(expected, test) << endl;
}

//Pre: test vector is the test dataset, expected is a copy of test but the
// ratings in expected are calculated from base using a method instead of given.
//Post: the root mean squared error between expected and test is calculated
//Purpose: to calculate the root mean squared error to see how good a test is.
float RMSECalc(/* in */ const vector<MovieRating> & expected,
               /* in */ const vector<MovieRating> & test){
    float sumS = 0.0f;

    for(int i = 0; i < TEST_SIZE; i++){
        float s = (expected[i].rating - test[i].rating)*(expected[i].rating - test[i].rating);
        sumS += s;
    }//end for

    return sqrt((float)1/(float)TEST_SIZE*sumS);
}//end RMSECalc
