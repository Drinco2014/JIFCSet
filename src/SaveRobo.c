/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>

#include "HS.h"
#include "Sspace.h"

extern SSPACE *GetNewSpace( int key, int Ssize );
extern SSPACE *GetSpaceAddr( int key );
extern SSPACE *GenModelFromFileS( int key, char *fname );
extern SSPACE *OutPutModelS( int key );
extern SSPACE *ReGenModelFromFileS( int key, char *fname );

int main( int argc, char *argv[] ){

	int key;
	if( argc < 2 ){
		printf("SaveRobo <key>\n");
		exit(0);
	}

	key   = atoi( argv[1] );
	OutPutModelS( key );
	return 0;
}


