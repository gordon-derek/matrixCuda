#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NR 3
#define NC 4

void Transpose(int m[NR][NC]);

int main(){
	int nc = NC;
	int nr = NR;
	int m[nr][nc];
	int i;
	int j;
	int v = 0;
	for(i = 0; i < nr; i++){
		for(j = 0; j < nc; j++){
			m[i][j] = v;
			v++;
		}
	}

	printf("*****Original*****\n");
	for(i = 0; i < nr; i++){
		for(j = 0; j < nc; j++)
			printf("%i ", m[i][j]);
		printf("\n");
	}

	int orig[nr][nc];
	for(i = 0; i < nr; i++){
		for(j = 0; j < nc; j++){
			orig[i][j] = m[i][j];
		}
	}
	Transpose(m);

	printf("\n*****Transpose*****\n");
	for(i = 0; i < nc; i++){
		for(j = 0; j < nr; j++)
			printf("%i ", m[i][j]);
		printf("\n");
	}

	printf("\n*****Original*****\n");
	for(i = 0; i < nr; i++){
		for(j = 0; j < nc; j++)
			printf("%i ", orig[i][j]);
		printf("\n");
	}

	int a[3][2] = {{2,4},{3,5},{1,7}};
	int b[2][3] = {{1,3,1},{9,5,2}};

	printf("\n*****Multpilication*****\n");
	
	for(i = 0; i < 3; i++){
		printf("%i %i", a[i][0], a[i][1]);
		if(i != 2)
			printf(" \t *\t%i %i %i", b[i][0],b[i][1], b[i][2]);
		printf("\n");
	}
	int sum = 0;

	int result[3][3];
	int row;
	for(row=0; row < 3; row++){
		for(i = 0; i < 3; i++){
			for(j = 0; j < 2; j++){
				sum = sum + a[i][j] * b[j][i];
			}
			sum = result[row][i];
			sum = 0;
		}
	}
	printf("\n*****Result*****\n");
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++)
			printf("%i ", result[i][j]);
		printf("\n");
	}
}

void Transpose(int m[NR][NC]){
	int nr = NR;
	int nc = NC;
	int newM[nc][nr];
	int i;
	int j;
	for(i = 0; i < nr; i++){
		for(j = 0; j < nc; j++){
			newM[j][i] = m[i][j];
		}
	}


	m[nc][nr];

	for(i = 0; i < nc; i++){
		for(j = 0; j < nr; j++){
			m[i][j] = newM[i][j];
		}
	}

	
}

