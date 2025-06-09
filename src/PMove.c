/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "HS.h"
#include "Sspace.h"


extern SSPACE *GetNewSpace( int key, int Ssize );
extern SSPACE *GetSpaceAddr( int key );
extern SSPACE *GenModelFromFileS( int key, char *fname );

int main(int argc, char *argv[]){
	int i ;
	JOINT *Object;
	SSPACE *sp;
	double dx,dy,dz;
	
	
	if( argc<5 ){
		printf("arguments: <Robot Key Code> < X > < Y > < z > \n");
		return 0;
	}
	
	dx = atof(argv[2]);
	dy = atof(argv[3]);
	dz = atof(argv[4]);
	
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	Object = (JOINT*)(sp+1);


	

	for(i=0;i<sp->num;i++){
		Object[i].ApData[0] += dx;
		Object[i].ApData[1] += dy;
		Object[i].ApData[2] += dz;
	}
	fprintf(stdout,"\n");

	shmdt(sp);


	return 0;
}

