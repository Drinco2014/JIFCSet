/*-----------------------------------------------------------------------

	PosePlot.c
	
	Hayashi & Satake Kinematics Calculation system Shell
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/





#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "HS.h"

int main( int argc, char *argv[] ){

	JOINT 	*Joints ;
	int 	num;


	if(argc<2){
		printf("arguments: <Structure file name>\n");
			return 0;
	}
	//Jointデータの読み込み
	Joints = GenModelFromFile(argv[1],&num);
	//グラフ表示用位置データの出力
	OutPutPointsData(stdout, Joints, 0, NULL);
	return 0;
}

