#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/timeb.h>
#include "HS.h"

typedef struct finger {
	int no;
	double p[3];
} FINGER ;
typedef struct ftoo {
	int fno,ono;
} FTOO;

#define FINGERNUM	10
FINGER fifi[30];/*=
	{
		{16,	10.0,	5.0,	20},
		{17,	10.0,	5.0,	20},
		{18,	10.0,	5.0,	20},
		{19,	10.0,	5.0,	20},
		{20,	10.0,	5.0,	20}
	};*/

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

int main(int argc, char *argv[]){
	int i,l, num,obnum,count,limit,j,fno, fkp;
	JOINT *Joints, *Object;
	int hantei;
	double	Target[3];
	double	tt[16],didi[20],didiold[20];
	static char FRname[30]="FRAP_Result.txt",buf[128];
	static char ObjFile[30]="Obj_Result.txt";
	static char HO[30]="HO_Result.txt";
	struct timeb tt1, tt2;
	double td1,td2;
	FILE *fp,*fp2,*fp3,*fp4,*fp5,*fp6;
	
	
	if(argc<4){
		if( argc<3 ){
			printf("arguments: <Structure file name> <Oblect file name> <Object Targets file>\n");
			return 0;
		}
		//Jointデータの読み込み
		Joints = GenModelFromFile(argv[1],&num);
		//Objectデータの読み込み
		Object = GenModelFromFile(argv[2],&obnum);
		fp4=fopen((char*)FRname,"w");
		//fp5=fopen((char*)ObjFile,"w");
		//グラフ表示用位置データの出力
		OutPutPointsData(fp4, Joints, 0, NULL);
		OutPutPointsData(fp4, Object, 0, NULL);
		fclose(fp4);
		return 0;
	}
	
	//Jointデータの読み込み
	Joints = GenModelFromFile(argv[1],&num);

	//Objectデータの読み込み
	Object = GenModelFromFile(argv[2],&obnum);

	
	
	fp=fopen(argv[3],"r");
	fp2=fopen("time_SP_proc_EP.txt","w");
	fp3=fopen("thdata.dat","w");
	fp4=fopen((char*)FRname,"w");
	fp5=fopen((char*)ObjFile,"w");
	fp6=fopen((char*)HO,"w");
	
	fscanf(fp,"%d",&limit);
	fscanf(fp,"%d",&fno);
	
	
	ftime(&tt1);
	td1 = tt1.time*1000 + tt1.millitm ;
	//グラフ表示用位置データの出力
	OutPutPointsData(fp4, Joints, 0, NULL);
	OutPutPointsData(fp5, Object, 0, NULL);

	OutPutPointsData(fp6, Joints, 0, NULL);
	OutPutPointsData(fp6, Object, 0, NULL);

	for(count=0;count<limit;++count){
	
	/*
	Objectの姿勢を求める
	*/
	for(;;){
		if( EOF >= (int)fgets(buf, sizeof(buf),fp)) return 0;
		if(buf[0] == '#') break;
	}
	for(i=0;i<fno;++i){
		fscanf(fp,"%d%lf%lf%lf",&fifi[i].no,&fifi[i].p[0],&fifi[i].p[1],&fifi[i].p[2]);
	}
	
	for(j=0;j<20;j++){
		didi[j]=didiold[j]=0.0;
	}
	
	for(j=0;;++j){	
		//終了条件は距離の変化がε以下の時-----
		for(j=0;j<20;j++){
			didiold[j]=didi[j];
		}
		for(i=0;i<fno;++i){
			didi[i] = Distance( fifi[i].p,Object[fifi[i].no].ApData );
		}
		
		for(i=0,hantei=0;i<fno;++i){
			if(fabs(didi[i]-didiold[i])>10E-6/*0.0001*/){
				hantei=-1;break;
			}			
		}
		
		//printf("%e\n",fabs(didi-didiold));
		if(hantei==0)
		{
			printf("end %d\n",count);
			break;
		}
		//--------------------------------------

		// 逆運動学計算は使用するジョイントに目標を各々設定
		for(i=0;i<fno;++i){
			for(l=Object[fifi[i].no].lower[0];
				0<=Object[l].lower[0];
					l=Object[l].lower[0]){//逆運動学計算
				Object[l].kp[0] = 0.3;
				SP_proc_IK_p( Object[fifi[i].no].ApData, fifi[i].p, -1.0,Object,l );
			}
		}
		//順運動学計算は、ベースから一気に
		SP_proc_DK( 1, tt ,Object,0);
		

	}
	
	/*
	Objectの姿勢を掴むロボットの姿勢を求める
	*/
	

	for(j=0;j<20;j++){
		didi[j]=didiold[j]=0.0;
	}
	
	for(j=0;;++j){	
		//終了条件は距離の変化がε以下の時-----
		for(j=0;j<20;j++){
			didiold[j]=didi[j];
		}
		for(i=0;i<fno;++i){
			didi[i] = Distance( Object[fo[i].ono].ApData,Joints[fo[i].fno].ApData );
		}
		
		for(i=0,hantei=0;i<fno;++i){
			if(fabs(didi[i]-didiold[i])>10E-6/*0.0001*/){
				hantei=-1;break;
			}			
		}
		
		//printf("%e\n",fabs(didi-didiold));
		if(hantei==0)
		{
			printf("end %d\n",count);
			break;
		}
		//--------------------------------------

		// 逆運動学計算は使用するジョイントに目標を各々設定
		for(i=0;i<FNO;++i){
			for(fkp=0,l=Joints[fo[i].fno].lower[0];
				0<=Joints[l].lower[0];
					l=Joints[l].lower[0],fkp++){//逆運動学計算
				if(fkp<3)Joints[l].kp[0] = 0.02;
				else Joints[l].kp[0] = 0.02;
				SP_proc_IK_p( Joints[fo[i].fno].ApData, Object[fo[i].ono].ApData, -1.0,Joints,l );
			}
		}
		//順運動学計算は、ベースから一気に
		SP_proc_DK( 1, tt ,Joints,0);
		

	}
	
	
	
	ftime(&tt1);
	td2 = tt1.time*1000 + tt1.millitm ;
	fprintf(fp2,"%lf\t%d\n",td2-td1,j);
	
	//グラフ表示用位置データの出力
	OutPutPointsData(fp4, Joints, 0, NULL);
	OutPutPointsData(fp5, Object, 0, NULL);
	
	OutPutPointsData(fp6, Joints, 0, NULL);
	OutPutPointsData(fp6, Object, 0, NULL);

	for(i=0;i<num;i++){
		fprintf(fp3,"%lf\t",Joints[i].dtAmount*180.0/3.1415);
	}
	fprintf(fp3,"\n");
	}

	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	return 0;
}

