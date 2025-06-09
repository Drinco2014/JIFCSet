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
	SSPACE *sp, *sp2;
	JOINT *mp;
	PATH *pp;
	int jnum,pnum,i,JointNoTmp;
	int UseJno;
	double rate=0.98;
	
	UseJno = 1;
	
	if( argc < 3 ){
		printf("OutPutTraceNext <Robo key> <Path key>\n");
		exit(0);
	}
	if( argc >= 5 ){
		if( !strcmp(argv[3],"-u") ){
			UseJno = atoi(argv[4]) ;
			if( argc >= 6 ) rate = atof(argv[5]);
		}
	}

	//Jointデータのセット
	sp = GetSpaceAddr( atoi(argv[1]) );
	mp = (JOINT*)(sp+1);
	jnum = sp->num;
	
	//Pathデータのセット
	sp2 = GetPathAddr( atoi(argv[2]) );
	pp = (PATH*)(sp2+1);
	pnum = sp2->num;

	//最初かどうかの判定、最初ならば先端の点と最初のPathを出力
	if( pp[0].no == -2 ){
		printf("%d\n",1);
		printf("%d\t%lf\t%lf\t%lf\n",jnum-1,pp[0].p[0],pp[0].p[1],pp[0].p[2]);
		mp[0].TaskPointFlg = 1 ;
		pp[0].no = jnum-1;
	}else{
	
		//タスク点と目標の関係リストPathを１つシフトする。
		for(i=pnum-1;i>0;--i){
			pp[i].no = pp[i-1].no ;
			if(pp[i].no > 0){
				mp[pp[i-1].no].TaskPointFlg = 0 ;
				mp[pp[i].no].TaskPointFlg = 1 ;
				JointNoTmp = i;
			}
		}
		
		//もし、rateが負の数の時、次のジョイントをタスク点に追加
		if( rate >= 0 ){
			//目標点の間隔よりリンクの長さが等しいか大きければ次のタスク点を追加***再考の必要あり
			if( Distance(pp[JointNoTmp].p,pp[0].p)*UseJno >=
			(rate*(double)UseJno*Distance(mp[pp[JointNoTmp].no].ApData,mp[pp[JointNoTmp].no-UseJno].ApData))
			){
				pp[0].no = pp[JointNoTmp].no-UseJno;
				mp[pp[0].no].TaskPointFlg = 1 ;
			}else{
				pp[0].no = -1;
			}
		}else{
			pp[0].no = pp[JointNoTmp].no-UseJno;
			mp[pp[0].no].TaskPointFlg = 1 ;
		}
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

				
	}
	
	shmdt( sp );
 	shmdt( sp2 );

	return 0;
}

