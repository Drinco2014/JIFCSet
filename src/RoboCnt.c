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
#include <time.h>
#include "HS.h"
#include "Sspace.h"

extern SSPACE *GetNewSpace( int key, int Ssize );
extern SSPACE *GetSpaceAddr( int key );
extern SSPACE *GenModelFromFileS( int key, char *fname );

typedef struct finger {
	int no;
	double p[3];
	double oe[3];
	double dis ;
} FINGER ;
typedef struct ftoo {
	int fno,ono;
} FTOO;

#define FINGERNUM	10
/*FINGER fifi[30];=
	{
		{16,	10.0,	5.0,	20},
		{17,	10.0,	5.0,	20},
		{18,	10.0,	5.0,	20},
		{19,	10.0,	5.0,	20},
		{20,	10.0,	5.0,	20}
	};

FTOO fo[FINGERNUM]={
	{ 23,4},
	{ 1,5 },
	{ 2,6 },
	{ 3,7 },
	{ 4,8 },
	{ 42,10},
	{ 43,11 },
	{ 44,12 },
	{ 45,13 },
	{ 46,14 }
};
int FNO=FINGERNUM;
*/
int main(int argc, char *argv[]){
	int i,l, num,j,fno, dflg=0, Stop = -1, ss,k ;
	JOINT *Object;
	int hantei;
	double	tt[16],*didi,*didiold, totaldis,totaldisold;
	FILE *fp;
	SSPACE *sp;
	FINGER *fifi;	
	struct timeb tt1, tt2;
	double td1,td2,didiret,didimax,didimin;
	clock_t tv,tv2;
	
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
	for(i=0;i<fno;++i){
		fscanf(fp,"%d%lf%lf%lf",&fifi[i].no,&fifi[i].p[0],&fifi[i].p[1],&fifi[i].p[2]);
		Object[fifi[i].no].TaskPointFlg = 1 ;
		fifi[i].oe[0]= fifi[i].oe[1]= fifi[i].oe[2]= -999.99;
		
	}
	
	for(j=0;j<fno;j++){
		didi[j]=didiold[j]=0.0;
	}
	totaldisold=totaldis=99999.9;
	
	//ftime(&tt1);
	//td1 = tt1.time*1000 + tt1.millitm ;
	//gettimeofday(&tv , NULL );
	tv = clock();
#ifdef T1000
	for(j=0;j<ROOPNUM;++j){
#else	
	for(j=0;;++j){
#endif	
		//終了条件は距離の変化がε以下の時----- -> タスク点の変化量を基準に
		for(i=0;i<fno;i++){
			didiold[i]=didi[i];
		}
		//totaldisold=totaldis;
		for(i=0,totaldis=0.0;i<fno;++i){
			didi[i] = Distance( fifi[i].p,Object[fifi[i].no].ApData );
			totaldis += didi[i];
			fifi[i].dis = Distance( fifi[i].oe, Object[fifi[i].no].ApData );
#ifdef CNVPRO
			fprintf(fpcp,"%d\t%e\n",fifi[i].no,didi[i]);	
#endif
		}
		didiret = totaldis / (double)fno ;
#ifndef T1000		
		for(i=0,hantei=0;i<fno;++i){
			//if(fabs(didi[i]-didiold[i])>10E-10/*0.0001*/){
			//if(fabs(didi[i]-didiold[i])>10E-6/*0.0001*/){
			if(fabs(fifi[i].dis)>1E-6/*0.0001*/){
				hantei=-1;
				break;
				//totaldis += fabs(didi[i]-didiold[i]);
			}else{
				//totaldis += fabs(didi[i]-didiold[i]);

			}
		}
		
		/*if(totaldisold < totaldis){
			printf("(%d,%d,%d)%lf\n",j,hantei,i,totaldis);
			//break;
		}*/
		if(hantei==0)
		{
			//printf("end %d\n",j);
			break;
		}
		//--------------------------------------
#endif

		// 逆運動学計算は使用するジョイントに目標を各々設定
		for(i=0;i<fno;++i){
			for(ss=1,l=Object[fifi[i].no].lower[0];
				/*0<=Object[l].lower[0]*/l >= 0 ;
					l=Object[l].lower[0],ss++){//逆運動学計算
				Object[l].kp[0] =  1.0/(/*Object[l].LowerJno*/sp->num-1);
				SP_proc_IK_p( Object[fifi[i].no].ApData, fifi[i].p, -1.0,Object,l );
				if(dflg==1 && Object[l].TaskPointFlg == 1) break; //タスク点ならば終了
				if(dflg==2 && ss >= Stop) break;		//指定のジョイント数になれば終了
				if(0>Object[l].lower[0]) break;
				
				//printf("%d ",l);
			}
			
		}
		
		// 現在のタスク点の座標を保存する
		for(i=0;i<fno;++i){
			for(k=0;k<3;++k){
				fifi[i].oe[k] = Object[fifi[i].no].ApData[k];
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
	//ftime(&tt1);
	//td2 = tt1.time*1000 + tt1.millitm ;
	//gettimeofday(&tv2 , NULL );
	//td2 = (double)(tv2.tv_sec-tv.tv_sec)*10E6+(double)(tv2.tv_usec-tv.tv_usec);
	tv2 = clock();
	
	for(didimin=99999.99,didimax=0.0,i=0;i<fno;i++){
		if(didi[i] > didimax) didimax = didi[i];
		if(didi[i] < didimin) didimin = didi[i];
	}

	
	fprintf(stdout,"%e\t%lf sec\t%d times\t%e\t%e\t",didiret,(double)(tv2-tv)/CLOCKS_PER_SEC,j,didimin,didimax);
	for(i=0;i<fno;i++){
		fprintf(stdout,"%d\t%e\t",fifi[i].no,didi[i]);
	}
	fprintf(stdout,"\n");
	
	shmdt( sp );
	
#ifdef CNVPRO
	fclose(fpcp);
	fclose(fpcpth);
#endif

	return 0;
}

