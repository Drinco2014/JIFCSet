/*-----------------------------------------------------------------------

	
	
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
extern double Distance( double a[3], double b[3]);


int main(int argc, char *argv[]){
	int i;
	JOINT *Object;
	int no,num;
	FILE *fp;
	SSPACE *sp;
	double P[3];
	
	
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
		fscanf(fp,"%d%lf%lf%lf",&no,&P[0],&P[1],&P[2]);
		printf("%d\t%e\t",no,Distance(P,Object[no].ApData));
		printf("%lf\t%lf\t%lf\t",P[0],P[1],P[2]);
		printf("%lf\t%lf\t%lf\n",Object[no].ApData[0],Object[no].ApData[1],Object[no].ApData[2]);
		Object[no].NumberOfDtData = 1;
		
	}
	
	shmdt( sp );

	return 0;
}

