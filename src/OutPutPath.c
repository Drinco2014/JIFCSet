/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdlib.h>

#include "HS.h"
#include "Sspace.h"


extern SSPACE *GetNewPath( int key, int Ssize );
extern SSPACE *GetPathAddr( int key );
extern SSPACE *GenPathFromFileS( int key, char *fname );
SSPACE *OutPutPathlS( int key );

int main(int argc, char *argv[]){
	int i;
	PATH *path;
	SSPACE *sp;
	
	
	if( argc<2 ){
		printf("arguments: <Path Key Code> \n");
		return 0;
	}

	sp = OutPutPathlS( atoi(argv[1]) );
	
	shmdt(sp);


	return 0;
}

