/*
    Author: Derek Gordon
    Date: May 31st, 2013
    Purpose: The MovieRatings object will be ideally constructed from
	a file that will have a list of ratings in a table like layout
	following userid/movieid/rating/timestamp.  This object will
	have different methods to perform matrix completion to test against
	another matrix that has all of the correct movie ratings this object
	will be trying to predict.
*/
#ifndef rating_h
#define rating_h


struct MovieRating{
    int uId;
    int mId;
    float rating;
    int timestamp;
};
#endif
