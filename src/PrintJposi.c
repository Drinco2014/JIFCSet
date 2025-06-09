/*-----------------------------------------------------------------------

	PrintJposi.c
	A command for changing a structure of a robot by exchanged upper links and lower links data.
	  
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
	int i,j,Eflg=0;
	JOINT *Object;
	int no,num,up;
	double	dth;
	double	tt[16];
	FILE *fp;
	SSPACE *sp;
	
	
	if( argc<2 ){
		printf("arguments: <Robot Key Code> \n");
		return 0;
	}
	if( argc > 2 ){
		if( !strcmp(argv[2],"-e") || !strcmp(argv[2],"-E") ){
			Eflg = 1 ;
		}
	}

	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	Object = (JOINT*)(sp+1);
	
	while( EOF != scanf("%d",&i) ){
		if(Eflg == 0 ) printf("%lf\t%lf\t%lf\n",Object[i].ApData[0],Object[i].ApData[1],Object[i].ApData[2]);
		else printf("%e\t%e\t%e\n",Object[i].ApData[0],Object[i].ApData[1],Object[i].ApData[2]);
	}

	shmdt( sp );

	return 0;
}

