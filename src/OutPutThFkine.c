/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <stdlib.h>

#include "HS.h"
#include "Sspace.h"


extern SSPACE *GetNewSpace( int key, int Ssize );
extern SSPACE *GetSpaceAddr( int key );
extern SSPACE *GenModelFromFileS( int key, char *fname );

int main(int argc, char *argv[]){
	int i,l, num,obnum,count,limit,j,fno, fkp;
	JOINT *Object;
	int hantei;
	double	Target[3];
	double	tt[16],didi[20],didiold[20];
	//struct timeb tt1, tt2;
	double td1,td2;
	SSPACE *sp;
	
	
	if( argc<2 ){
		printf("arguments: <Robot Key Code> \n");
		return 0;
	}
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	Object = (JOINT*)(sp+1);


	

	for(i=0;i<sp->num;i++){
		if(Object[i].TypeOfJoint== 0 ){
			fprintf(stdout,"%d\t%lf\t",i,Object[i].dtAmount*180.0/3.1415);
		}else{
			if(Object[i].TypeOfJoint== 1) fprintf(stdout,"%d\t%lf\t",i,Object[i].dtAmount);
		}
	}
	fprintf(stdout,"\n");

	shmdt(sp);


	return 0;
}

