/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <sys/shm.h>
#include "HS.h"
#include "Sspace.h"

extern SSPACE *GetNewSpace( int key, int Ssize );
extern SSPACE *GetSpaceAddr( int key );
extern SSPACE *GenModelFromFileS( int key, char *fname );
extern SSPACE *GetNewPath( int key, int num );
extern SSPACE *GetPathAddr( int key );

int main(int argc, char *argv[]){
	SSPACE *sp2;
	PATH *pp;
	int pnum,i,JointNoTmp;

	if( argc < 2 ){
		printf("OutPutTaskPoints <Robo <Path key>\n");
		exit(0);
	}

	//Pathデータのセット
	sp2 = GetPathAddr( atoi(argv[1]) );
	pp = (PATH*)(sp2+1);
	pnum = sp2->num;

	for(JointNoTmp=i=0;i<pnum;++i){
		if(pp[i].no > 0){
			JointNoTmp++;
		}
	}
	printf("%d\n",JointNoTmp);
	for(i=0;i<pnum;++i){
		if(pp[i].no > 0){
			 printf("%d\t%lf\t%lf\t%lf\n",pp[i].no,pp[i].p[0],pp[i].p[1],pp[i].p[2]);
			//mp[i].TaskPointFlg = 1 ;
		}
	}

 	shmdt( sp2 );

	return 0;
}

