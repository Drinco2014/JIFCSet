/*-----------------------------------------------------------------------

	GetPositions.c
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/shm.h>
#include <stdlib.h>

#include "HS.h"
#include "Sspace.h"


extern SSPACE *GetNewSpace( int key, int Ssize );
extern SSPACE *GetSpaceAddr( int key );
extern SSPACE *GenModelFromFileS( int key, char *fname );



int main(int argc, char *argv[]){
	int i;
	JOINT *Object;
	int no,num;
	FILE *fp;
	SSPACE *sp;
	
	
	if( argc<2 ){
		printf("arguments: <Robot Key Code> \n");
		return 0;
	}
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	Object = (JOINT*)(sp+1);
	
	fp=stdin;//fopen(argv[2],"r");
	
	fscanf(fp,"%d",&num);
		
	for(i=0;i<num;++i){
		fscanf(fp,"%d",&no);
		printf("%d\t%lf\t%lf\t%lf\n",no,Object[no].ApData[0],Object[no].ApData[1],Object[no].ApData[2]);
		Object[no].NumberOfDtData = 1;
		
	}
	
	shmdt( sp );

	return 0;
}

