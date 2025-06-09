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
	int i,l, num,j,fno, dflg=0, Stop = -1, ss ;
	int TotalCount,JointNoTmp;
	JOINT *Object;
	int hantei;
	double	tt[16],*didi,*didiold, totaldis,totaldisold;
	FILE *fp;
	SSPACE *sp;
	FINGER *fifi;	
	struct timeb tt1, tt2;
	double td1,td2;
#ifdef CNVPRO
	FILE *fpcp,*fpcpth;
#endif
	
	if( argc<2 ){
		printf("arguments: <Robot Key Code> \n");
		return 0;
	}
	if( argc > 2 ){
		if( !strcmp(argv[2],"-d") ){
			dflg = 1 ;
		}else if( argc > 3 ){
			if( !strcmp(argv[2],"-s") ){
				dflg = 2 ;
				Stop = atoi( argv[3] );
			}
		}
	}
		
#ifdef CNVPRO
	fpcp = fopen("CnvPro.dat","w");
	fpcpth = fopen("CnvProth.dat","w");
#endif
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	Object = (JOINT*)(sp+1);

	CountLowerJoints(0,Object,0);
	
	fp=stdin;//fopen(argv[2],"r");
	
	fscanf(fp,"%d",&fno);
	fifi = (FINGER*)malloc( sizeof(FINGER)*(fno+2) );
	didi = (double*)malloc( sizeof(double)*(fno+2) );
	didiold = (double*)malloc( sizeof(double)*(fno+2) );
		
	/*
	タスク点の目標を読む
	*/
	for(i=0;i<sp->num;++i){
		Object[i].TaskPointFlg = 0 ;
	}
	//データは，トレースする点の点列座標値．タスク点は初期値−１
	for(i=0;i<fno;++i){
		fifi[i].no = -1;
		fscanf(fp,"%lf%lf%lf",&fifi[i].p[0],&fifi[i].p[1],&fifi[i].p[2]);
	}

  for(TotalCount=0;;){
	if(TotalCount==0){
		//最初のタスク点は先端
		fifi[0].no = sp->num - 1;
		Object[fifi[0].no].TaskPointFlg = 1 ;
	}else{
		//タスク点と目標の関係リストfifiを１つシフトする。
		for(i=fno-1;i>0;--i){
			fifi[i].no = fifi[i-1].no ;
			if(fifi[i].no != -1){
				Object[fifi[i-1].no].TaskPointFlg = 0 ;
				Object[fifi[i].no].TaskPointFlg = 1 ;
				JointNoTmp = i;
			}
		}
		//目標点の間隔よりリンクの長さが等しいか大きければ次の託す点をfifiに追加
		if( Distance(fifi[JointNoTmp].p,fifi[0].p) >=
			Distance(Object[fifi[JointNoTmp].no].ApData,Object[fifi[JointNoTmp].no-1].ApData)
		){
			fifi[0].no = fifi[JointNoTmp].no-1;
			Object[fifi[0].no].TaskPointFlg = 1 ;
		}else{
			fifi[0].no = -1;
		}
				
	}
		
	for(j=0;j<fno;j++){
		didi[j]=didiold[j]=0.0;
	}
	
	//　一回の運動学計算のループ
	totaldisold=totaldis=99999.9;
	ftime(&tt1);
	td1 = tt1.time*1000 + tt1.millitm ;

	for(j=0;;++j){	
		//終了条件は距離の変化がε以下の時-----
		for(i=0;i<fno;i++){
			didiold[i]=didi[i];
		}
		totaldisold=totaldis;
		for(i=0;i<fno;++i){
			if(fifi[i].no > -1 ){
				didi[i] = Distance( fifi[i].p,Object[fifi[i].no].ApData );
#ifdef CNVPRO
				fprintf(fpcp,"%d\t%e\n",fifi[i].no,didi[i]);	
#endif
			}
		}
		
		for(i=0,hantei=0,totaldis=0.0;i<fno;++i){
			
			if(fifi[i].no > -1 ){
				//if(fabs(didi[i]-didiold[i])>10E-10/*0.0001*/){
				if(fabs(didi[i]-didiold[i])>10E-6/*0.0001*/){
					hantei=-1;break;
					//totaldis += fabs(didi[i]-didiold[i]);
				}else{
					//totaldis += fabs(didi[i]-didiold[i]);
				}
			}
		}
		
		/*if(totaldisold < totaldis){
			printf("(%d,%d,%d)%lf\n",j,hantei,i,totaldis);
			//break;
		}*/
		if(hantei==0)
		{
			printf("end %d\n",j);
			break;
		}
		//--------------------------------------

		// 逆運動学計算は使用するジョイントに目標を各々設定
		for(i=0;i<fno;++i){
			
			if(fifi[i].no > -1 ){
			    for(ss=1,l=Object[fifi[i].no].lower[0];
				/*0<=Object[l].lower[0]*/l >= 0 ;
					l=Object[l].lower[0],ss++){//逆運動学計算
				Object[l].kp[0] = 1.0/(/*Object[l].LowerJno*/sp->num);
				SP_proc_IK_p( Object[fifi[i].no].ApData, fifi[i].p, -1.0,Object,l );
				if(dflg==1 && Object[l].TaskPointFlg == 1) break; //タスク点ならば終了
				if(dflg==2 && ss >= Stop) break;		//指定のジョイント数になれば終了
				if(0>Object[l].lower[0]) break;
				
				//printf("%d ",l);
			    }
			}
			
		}
		//順運動学計算は、ベースから一気に
		SP_proc_DK( 1, tt ,Object,sp->base);
#ifdef CNVPRO
	for(i=0;i<sp->num;i++){
		fprintf(fpcpth,"%e\t",Object[i].dtAmount*180.0/3.1415);
	}
	fprintf(fpcpth,"\n");
#endif		

	}
	ftime(&tt1);
	td2 = tt1.time*1000 + tt1.millitm ;
	fprintf(stdout,"%lfms\t%dtimes\n",td2-td1,j);
	
	shmdt( sp );
    }
#ifdef CNVPRO
	fclose(fpcp);
	fclose(fpcpth);
#endif

	return 0;
}

