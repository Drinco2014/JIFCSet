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
	int i,j,nn;
	JOINT *mp;
	int no,num,up;
	double	dth;
	double	tt[16];
	static	char buf[100];
	FILE *fp;
	SSPACE *sp;
	
	
	if( argc<2 ){
		printf("arguments: <Robot Key Code> \n");
		return 0;
	}
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	mp = (JOINT*)(sp+1);
	
	fp=stdin;//fopen(argv[2],"r");
	mp = (JOINT*)(sp + 1) ;
	
	for(i=0;;){
		memset(buf,0,sizeof(buf));
		if( NULL == fgets(buf, sizeof(buf),fp)) break;
		if(buf[0] == '$'){
			sscanf(&buf[1],"%d",&sp->base);
		}else if(buf[0] == '#'){
			sscanf(&buf[1],"%d",&nn);
			for(j=0;j<10;++j){
				fscanf(fp,"%d",&mp[nn].upper[j]);
				if(mp[nn].upper[j]==-1){
					break;
				}
			}
			for(j=0;j<10;++j){
				fscanf(fp,"%d",&mp[nn].lower[j]);
				if(mp[nn].lower[j]==-1){
					break;
				}
			}
		}else if( buf[0] == '$' ){
			sscanf(&buf[1],"%d",&sp->base);
		}else if( buf[0] == 0x00 ) break;
			
			
			
	}
	
	shmdt( sp );


	return 0;
}

