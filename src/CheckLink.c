/*-----------------------------------------------------------------------

	PosePlot.c
	
	Hayashi & Satake Kinematics Calculation system Shell
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/





#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>


#include "HS.h"
#include "Sspace.h"

extern SSPACE *GetNewSpace( int key, int Ssize );
extern SSPACE *GetSpaceAddr( int key );
extern SSPACE *GenModelFromFileS( int key, char *fname );

int main( int argc, char *argv[] ){

	JOINT 	*Joints ;
	SSPACE 	*sp;
	int 	num;
	int	i,j;

	if(argc<2){
		printf("arguments: <KEY>\n");
			return 0;
	}
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	Joints = (JOINT*)(sp+1);
	//グラフ表示用位置データの出力
	
	
	for(i=0;i<sp->num;++i){
		printf("ジョイント番号:%d\n",i);
		printf("Upper:\t");
		for(j=0;Joints[i].upper[j]>=0;++j){
			printf("%d\t",Joints[i].upper[j]);
		}
		printf("\n");
		printf("Lower:\t");
		for(j=0;Joints[i].lower[j]>=0;++j){
			printf("%d\t",Joints[i].lower[j]);
		}
		printf("\n");

	}



	shmdt(sp);
	return 0;
}

