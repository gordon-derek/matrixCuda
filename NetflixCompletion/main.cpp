/* 
 Author: Derek Gordon
 Date: 7/22/2013
 Project: Netflix problem/Matrix Completion
 Purpose: This project is used to test matrix completion on a dataset of movie ratings
 the dataset is split up into 2 sets.  The training set which is 80% of the movie ratings
 and the testing set which is the remaining 20%.  The algorithm will learn from the training
 set and use the testing set to test its learning when trying to predict the its own testing set.
*/

#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include "Eigen/Core"
#include "pst.h"
#include "MovieRating.h"

using namespace Eigen;
using namespace std;

const int NUM_USERS = 943;
const int NUM_MOVIES = 1682;
const int NUM_FEATURES = 10;
const int NUM_RATINGS = 100000;

MovieRating StringToRating(/* in */string line);
string itos(/* in */int n);
int strtoi (/* in */const string& str);
bool IsWhiteSpace(/* in */ char c);void LearnRandom(/* in */ MatrixXf & p, //the user matrix
				 /* out*/ MatrixXf & q, //the movie matrix
				 /* in */ const MatrixXf & R);
void RemoveRow(/* in */ MatrixXf & orig,
				   /* in */ const int index,
				   /* in */ const int mSize);

int main(){
	MatrixXf p(NUM_USERS, NUM_FEATURES);
	MatrixXf q(NUM_MOVIES, NUM_FEATURES);
	MatrixXf R(NUM_USERS, NUM_MOVIES);
	R.setConstant(-1.f);
	MatrixXf RT(NUM_USERS, NUM_MOVIES);
	RT.setConstant(-1.f);
	
	//load training dataset
	ifstream base("assets/u.data");
	if(base.fail()){
		cout << "Unable to open File. Program Terminating..."<<endl;
		return 1;
	}//end if
	string line = "";
	getline(base, line);
	MovieRating m = StringToRating(line);
	for(int r = 0; r < (NUM_RATINGS * .8); r++){
		for(int i = 0; i < NUM_USERS; i++){
			while(m.uId == i){
				R(i,(m.mId-1)) = m.rating;
				getline(base,line);
				m = StringToRating(line);
			}//end while
		}//end for(i)
	}//end for(r)

	//load testing dataset
	for(int r = 0; r < (NUM_RATINGS * .2); r++){
		for(int i = 0; i < NUM_USERS; i++){
			while(m.uId == i){
				RT(i,(m.mId-1)) = m.rating;
				getline(base,line);
				m = StringToRating(line);
			}//end while
		}//end for(i)
	}//end for(r)

	float bestRMSE;
	for(int n = 0; n < 100; n++){
		for(int i = 0; i < q.rows(); i++)
			for(int j = 0; j < q.cols(); j++)
				q(i,j) = rand() % 3;
		for(int i = 0; i < p.rows(); i++)
			for(int j = 0; j < p.cols(); j++)
				p(i,j) = rand() % 3;

		LearnRandom(p, q, R);
	}
	

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

//Pre: string contains a properly formatted line from a dataset file
//Post: A populated MovieRating object is returned
//Purpose: to convert a string that is properly formatted to a MovieRating object
MovieRating StringToRating(/* in */string line){
    MovieRating rating;	//to process string into
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
                    case 0: rating.uId = strtoi(item); numCat++; break;
                    case 1: rating.mId = strtoi(item); numCat++; break;
                    case 2: rating.rating = strtoi(item); numCat++; break;
                    case 3: rating.timestamp = strtoi(item); numCat++; break;
                    default: break;
                }//end switch
                item = "";
            }//end if
        }//end else
    }//end for
    return rating;
}//end StringToRating

void LearnRandom(/* in */ MatrixXf & p, //the user matrix
				 /* out*/ MatrixXf & q, //the movie matrix
				 /* in */ const MatrixXf & R){ //the training matrix

	MatrixXf RTest(NUM_USERS, NUM_MOVIES);	
	MatrixXf qTemp = q;
	MatrixXf invResult(NUM_FEATURES, NUM_FEATURES);

	int mSize = NUM_MOVIES;

	for(int i = 0; i < NUM_USERS; i++){

		for(int j = NUM_MOVIES-1; j >= 0; j--)
			if(R(i,j) == -1 && mSize > 1){
				RemoveRow(qTemp, j, mSize);
				mSize--;
			}
		if(mSize >= 1){
			RTest.setConstant(-1.f);
			VectorXf pRow = p.row(i);
			VectorXf r = (q*pRow)/5;
			int index = 0;
			
			for( int k = 0; k < NUM_MOVIES; k++){
				if(R(i,k) != -1){
					RTest(i,k) = r(index);
					index++;
				}
			}
			cout << RTest.row(i) << endl << endl;
		}
		mSize = NUM_MOVIES;
		qTemp = q;
	}

}

void RemoveRow(/* in */ MatrixXf & orig,
				   /* in */ const int index,
				   /* in */ const int mSize){

		orig.row(index) = orig.row(mSize - 1);
		orig.conservativeResize(mSize - 1, NUM_FEATURES);


}