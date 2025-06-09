/*-----------------------------------------------------------------------

	F_Kine.c
	A command for doing forward kinematics calculations by consol inputs 
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
	double	dth;
	double	tt[16];
	FILE *fp;
	SSPACE *sp;
	int initflg = 0 ;
	
	
	if( argc<2 ){
		printf("arguments: <Robot Key Code> \n");
		return 0;
	}else if( argc == 3 ){
		if( !strncmp( argv[2], "-i", 2 ) ) initflg = 1;
	}
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	Object = (JOINT*)(sp+1);
	
	fp=stdin;//fopen(argv[2],"r");
	
	fscanf(fp,"%d",&num);
		
	for(i=0;i<num;++i){
		fscanf(fp,"%d%lf",&no,&dth);
		Object[no].dtData = dth*PI/180.0;
		Object[no].NumberOfDtData = 1;
		if( initflg == 0 ){
			if( Object[no].dtData > 0.0 ){
				if( (Object[no].dtAmount+Object[no].dtData) > Object[no].AxisLimit[1] ){
					Object[no].dtData = Object[no].AxisLimit[1]-Object[no].dtAmount;
				}
			}else{
				if( (Object[no].dtAmount+Object[no].dtData) < Object[no].AxisLimit[0] ){
					Object[no].dtData = Object[no].AxisLimit[0]-Object[no].dtAmount;
				}
			}
		}
	
	}
	//順運動学計算は、ベースから一気に
	SP_proc_DK( 1, tt ,Object,sp->base);
	
	if( initflg == 1 ){
		for(i=0;i<sp->num;++i){
			Object[i].dtAmount = 0.0 ;
		}
	}
	
	shmdt( sp );

	return 0;
}

