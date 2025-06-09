/*-----------------------------------------------------------------------

	Chgstruct.c
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
	int i,j;
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
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	Object = (JOINT*)(sp+1);
	
	printf("$%d\n",sp->base);
	
	for(i=0;i<sp->num;++i){
		printf("\n#%d\n",i);
		for(j=0;j<10;++j){
			printf("%d ",Object[i].upper[j]);
			if( Object[i].upper[j] < 0 ){
				break;
			}
		}
		printf("\n");
		for(j=0;j<10;++j){
			printf("%d ",Object[i].lower[j]);
			if( Object[i].lower[j] < 0 ){
				break;
			}
		}
		printf("\n");		
	}

	shmdt( sp );

	return 0;
}

