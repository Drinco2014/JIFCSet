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

extern SSPACE *GetNewPath( int key, int Ssize );
extern SSPACE *GetPathAddr( int key );
extern SSPACE *GenPathFromFileS( int key, char *fname );
extern SSPACE *ReGenPathFromFileS( int key, char *fname );

int main( int argc, char *argv[] ){

	SSPACE	*sp;
	JOINT	*jn;
	char	*fname;
	int 	key;
	
	if( argc < 3 ){
		printf("LoadPath <key> <file name>\n");
		exit(0);
	}
	
	fname = argv[2] ;
	key   = atoi( argv[1] );
	
	if( (SSPACE*)NULL >= (sp = ReGenPathFromFileS( key, fname ) ) ){
		printf("ERROR\n");
		return 0;
	}
	
	printf("Path data definition Success!!\n Robot Name = %s Key = %d \n",sp->name, key );
	
	shmdt(sp);
	return 0;
}


