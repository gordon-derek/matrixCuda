#ifndef _PST_H_
#define _PST_H_

//////////////////////////////////////////////////////////////////
// This is based on the old pmath files. It contains some useful
// functions
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <vector>			// For vectors of objects
#include "Eigen/Dense"		// For matrix math
#include "Eigen/SVD"
#include <time.h>

using namespace std;
using namespace Eigen;

#define COMPILE_GCC			// This goes here to fix include order issues

#ifndef COMPILE_GCC
	#include <ppl.h>			// For parallel_for
	#include <random>			// For random numbers (see http://www.cplusplus.com/reference/random/normal_distribution/)
	using namespace concurrency;
#endif
	
/*
We use the Eigen package. To get it to use many threads, go to 
	Project Settings -> Config -> c/c++ -> languages -> openMP -> yes
This allows it to use more than one thread.

Also, see the Eigen documentation for getting the debugger to properly show
the values of Eigen objects. I couldn't get it to work, but usually I can say
m.m_storage.m_data[0]
*/

//////////////////////////////////////////////////////////////////
// Choose your precision: double is slower but more precise.
// If this is not defined, then it uses doubles, else it uses
// floats.
//////////////////////////////////////////////////////////////////
#define SINGLE_PRECISION

//////////////////////////////////////////////////////////////////
// Compilation flags and global variables that are computed 
// automatically
//////////////////////////////////////////////////////////////////
// Note: dReal is used by ODE (physics engine) and real is used by Eigen. I'm going to keep them
// separate, so you can do high-precision physics with lower-precision matrix math in the agent, or vice versa.
// So, if you're using this with ODE, the choice of ReleaseDouble vs ReleaseSingle doesn't change this.
#ifdef SINGLE_PRECISION
	#define real		float
	#define VectorXr	VectorXf
	#define MatrixXr	MatrixXf
	#define ArrayXr		ArrayXf
#else
	#define real		double
	#define VectorXr	VectorXd
	#define MatrixXr	MatrixXd
	#define ArrayXr		ArrayXd
#endif

void pinv(MatrixXr &a, MatrixXr &result, double epsilon = std::numeric_limits<real>::epsilon());

//////////////////////////////////////////////////////////////////
// Quadratic Programming
//////////////////////////////////////////////////////////////////


namespace Eigen {
// See matlab documentation
real quadprog(MatrixXr & H, VectorXr & f, const MatrixXr & A, const VectorXr & b, VectorXr & buff);

// See source for this one - it's more general than the version above, but the constraints are in a totally different (less intuitive) format.
real solve_quadprog(MatrixXr & G,  VectorXr & g0,  
                      const MatrixXr & CE, const VectorXr & ce0,  
                      const MatrixXr & CI, const VectorXr & ci0, 
                      VectorXr& x);
}

//////////////////////////////////////////////////////////////////
// Useful functions
//////////////////////////////////////////////////////////////////

// If compiled using g++, there isn't an itoa function, so I'm using one that someone else rolled.
char* MYitoa(int value, char* result, int base);

// Eat all characters in cin. This is useful if you want to force
// the user to press enter but there might already be a newline in
// cin.
void cinFlush();

// Calls cinFlush then getchar(). So, it forces the user to hit enter again.
// That is, if there's already a newline in the stream, it is ignored.
int forceGetchar();

// Calls forceGetchar then exits with the provided error code
void forceGetcharExit(int code = 0);

// Functions for checking if numbers have diverged. The built in functions for this aren't compatible between gcc and visual studio, so I use my own.
bool my_isnan(const real & x);
bool my_isinf(const real & x);
bool diverged(const real & x);	// Checks if nan or inf

// Return a random number in the range min-max, with a uniform distribution.
// This is faster than the std::uniform_real_distribution. It may be less accurate though.
real random(real min, real max);

// Returns an integer from 0 to num-1, each with the probability specified in probabilities
int wrand(const VectorXr & probabilities);
int wrand(const vector<real> & probabilities);

// Get a random number, such that it's log is uniformly distributed
real logRand(real min, real max);

// Returns a number drawn from the standard normal distribution (multiply by sigma and add mu to get any normal distribution)
real normRand();

// Power function where everything is an integer. The built in power function doesn't return an int. This one does. This is not particularly efficient - but it works.
int ipow(int a, int b);

// Logistic (sigmoid) function
real S(real x);					

// Returns the correlation between a and b
real corr(const VectorXr & a, const VectorXr & b);

// Returns the angle between a and b
real angle(const VectorXr & a, const VectorXr & b);

// Floating-point modulo
// The result (the remainder) has same sign as the divisor.
// Similar to matlab's mod(); Not similar to fmod() -   Mod(-3,4)= 1   fmod(-3,4)= -3
real Mod(const real & x, const real & y);	
real WrapPosNegPI(real fAng);		// wrap [rad] angle to [-PI..PI)
real WrapTwoPI(real fAng);			// wrap [rad] angle to [0..TWO_PI)
real WrapPosNeg180(real fAng);		// wrap [deg] angle to [-180..180)
real Wrap360(real fAng);			// wrap [deg] angle to [0..360)

// Similarly, you can bound a number
real bound(const real & x, const real & minValue, const real & maxValue);

char readChar(string & s);		// If s is not empty, it returns the next non-white character. If s is empty, it calls cin
int readInt(string & s);		// If s is not empty, it returns the next integer in s. If s is empty, it reads an integer from cin
real readReal(string & s);		// Ditto, but with a real rather than an integer

// Returns the covariance of two arrays
real cov(const ArrayXr & a, const ArrayXr & b);

// Returns the variance of the array of numbers
real var(const ArrayXr & a);
real var(const vector<real> & a);

// Remove columns from the provided matrix (assumes that cols are given in sorted order)
void rmCols(MatrixXr & M, const VectorXi & cols);

// Sign of a number
int sign(real x);

// Fast cosine (not as good as Eigen probably, but better than built in - for when you don't have a matrix)
real fastCos(real x);
real fastSin(real x);

// ... sometimes min and max aren't found for some reasons, so I just wrote my own.
real min(const real & x, const real & y);
real max(const real & x, const real & y);

#endif
