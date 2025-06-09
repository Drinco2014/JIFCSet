/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "HS.h"
#include "Sspace.h"

SSPACE *GetNewSpace( int key, int Ssize );
SSPACE *GetSpaceAddr( int key );
SSPACE *GenModelFromFileS( int key, char *fname );
SSPACE *OutPutModelS( int key );
SSPACE *ReGenModelFromFileS( int key, char *fname );
SSPACE *SpidersReGenModelFromFileS( int key, char *fname );

SSPACE *GetNewPath( int key, int num );
SSPACE *GetPathAddr( int key );
SSPACE *GenPathFromFileS( int key, char *fname );
SSPACE *ReGenPathFromFileS( int key, char *fname );
SSPACE *OutPutPathlS( int key );

/*----------------------------------------------------------------
	SSPACE *GetNewSpace( int	key, int Ssize );
	
----------------------------------------------------------------*/
SSPACE *GetNewSpace( int key, int jointnum )
{
	SSPACE	*ret;
	int 	shmkey;
	int	Ssize ;
	
	Ssize = (jointnum+10)*sizeof(JOINT)+sizeof(SSPACE);
	
    if( -1 == (shmkey = shmget( key, Ssize, IPC_CREAT|IPC_EXCL|0666 ))) {
        		perror("shmget");
             	return (SSPACE*)NULL;
   }

    // 共有メモリアドレスを得る
    if((SSPACE*)-1==(ret=(SSPACE*)shmat(shmkey,(void*)NULL, 0 )) ){
       	 perror("shmget");
           return (SSPACE*)NULL;
    }
	memset(ret,0,Ssize);
	return ret;
	
}
/*----------------------------------------------------------------
	SSPACE *GetSpaceAddr( int	key )
	
----------------------------------------------------------------*/
SSPACE *GetSpaceAddr( int	key )
{
	void	*ret;
	int 	shmkey;
	
    if( -1 == (shmkey = shmget( key, 0, IPC_CREAT|0666 ))) {
            	perror("shmget");
            	return (SSPACE*)NULL;
    }

    // 共有メモリアドレスを得る
    if((SSPACE*)-1==(ret=(SSPACE*)shmat(shmkey,(void*)NULL, 0 )) ){
       	 perror("shmget");
           return (SSPACE*)NULL;
    }

	return ret;
	
}

/*----------------------------------------------------------------
	SSPACE *GenModelFromFileS( int key, char *fname )
	
----------------------------------------------------------------*/
//	ファイルからデータを読み込み計算モデルを生成
//
SSPACE *GenModelFromFileS( int key, char *fname ){
	JOINT *mp;
	SSPACE *ss;
	int i,j,nn,num;
	FILE *fp;
	static char buf[128];
	char *p;
	
	if((FILE*)NULL>=(fp=fopen(fname,"r"))){
		printf("%s is no exist!!!\n",fname);
		return NULL;
	}
	
	fscanf(fp,"%d",&num);
	
	if(NULL == (ss=GetNewSpace(key,num))){
		return NULL;
	}
	
	if( NULL == (p=strrchr(fname, '/')) ) p=fname;
	else p++;
	
	ss->num = num;
	strncpy( ss->name, p, 79 );
	mp = (JOINT*)(ss + 1) ;
	
	for(i=0;;i++){
	//printf("%d\n",i);
		memset(buf,0,sizeof(buf));
		if( NULL == fgets(buf, sizeof(buf),fp)) break;
		if(buf[0] == '#'){
			sscanf(&buf[1],"%d",&nn);
			//printf("%d\n",nn);
			
		fscanf(fp,"%lf%lf%lf",&mp[nn].ApData[0],&mp[nn].ApData[1],&mp[nn].ApData[2]);
		fscanf(fp,"%lf%lf%lf",&mp[nn].AaData[0],&mp[nn].AaData[1],&mp[nn].AaData[2]);
			fscanf(fp,"%lf%lf",&mp[nn].AxisLimit[0],&mp[nn].AxisLimit[1]);
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
			fscanf(fp,"%lf%lf",&mp[nn].kp[0],&mp[nn].kp[1]);
			fscanf(fp,"%x",&mp[nn].TypeOfJoint);
		}else if( buf[0] == 0x00 ) break;
			
			
			
	}
	fclose(fp);
	for(nn=0; mp[nn].lower[0] != -1 ; nn = mp[nn].lower[0]);

	ss->base = nn;

	return ss;
}

/*----------------------------------------------------------------
	SSPACE *ReGenModelFromFileS( int key, char *fname )
	
----------------------------------------------------------------*/
//	ファイルからデータを読み込み計算モデルを生成
//
SSPACE *ReGenModelFromFileS( int key, char *fname ){
	JOINT *mp;
	SSPACE *ss;
	int i,j,nn,num;
	FILE *fp;
	static char buf[128];
	char *p;
	
	if((FILE*)NULL>=(fp=fopen(fname,"r"))){
		printf("%s is no exist!!!\n",fname);
		return NULL;
	}
	
	fscanf(fp,"%d",&num);
	
	if(NULL == (ss=GetNewSpace(key,num))){
		return NULL;
	}
	
	if( NULL == (p=strrchr(fname, '/')) ) p=fname;
	else p++;
	
	ss->num = num;
	strncpy( ss->name, p, 79 );
	mp = (JOINT*)(ss + 1) ;
	
	for(i=0;;){
		memset(buf,0,sizeof(buf));
		if( NULL == fgets(buf, sizeof(buf),fp)) break;
		if(buf[0] == '#'){
			sscanf(&buf[1],"%d",&nn);
			//printf("%d\n",nn);
			if( nn >= num ){
				return NULL;
			}	fscanf(fp,"%lf%lf%lf",&mp[nn].ApData[0],&mp[nn].ApData[1],&mp[nn].ApData[2]);
			fscanf(fp,"%lf%lf%lf",&mp[nn].AaData[0],&mp[nn].AaData[1],&mp[nn].AaData[2]);
			fscanf(fp,"%lf%lf",&mp[nn].AxisLimit[0],&mp[nn].AxisLimit[1]);
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
			fscanf(fp,"%lf%lf",&mp[nn].kp[0],&mp[nn].kp[1]);
			fscanf(fp,"%x",&mp[nn].TypeOfJoint);
			fscanf(fp,"%lf",&mp[nn].dtAmount);
		}else if( buf[0] == 0x00 ) break;
			
			
			
	}
	for(nn=0; mp[nn].lower[0] != -1 ; nn = mp[nn].lower[0]);

	ss->base = nn;

	return ss;
}

/*----------------------------------------------------------------
	SSPACE *SpidersReGenModelFromFileS( int key, char *fname )
	
----------------------------------------------------------------*/
//	ファイルからデータを読み込み座標、ベクトル、変位のみ継承
//


SSPACE *SpidersReGenModelFromFileS( int key, char *fname ){
	JOINT *mp;
	SSPACE *ss;
	int i,j,nn,num,dumint;
	double dumdouble;
	FILE *fp;
	static char buf[128];
	char *p;
	
	if((FILE*)NULL>=(fp=fopen(fname,"r"))){
		printf("%s is no exist!!!\n",fname);
		return NULL;
	}
	
	fscanf(fp,"%d",&num);
	
	if( NULL == (p=strrchr(fname, '/')) ) p=fname;
	else p++;

	ss = GetSpaceAddr( key );
	mp = (JOINT*)(ss+1);
	
	for(i=0;;){
		memset(buf,0,sizeof(buf));
		if( NULL == fgets(buf, sizeof(buf),fp)) break;
		if(buf[0] == '#'){
			sscanf(&buf[1],"%d",&nn);
			printf("%d\n",nn);
			fscanf(fp,"%lf%lf%lf",&mp[nn].ApData[0],&mp[nn].ApData[1],&mp[nn].ApData[2]);
			fscanf(fp,"%lf%lf%lf",&mp[nn].AaData[0],&mp[nn].AaData[1],&mp[nn].AaData[2]);
			
			fscanf(fp,"%lf%lf",&mp[nn].AxisLimit[0],&mp[nn].AxisLimit[1]);
			for(j=0;j<10;++j){
				fscanf(fp,"%d",&dumint);
				if(dumint==-1){
					break;
				}
			}
			for(j=0;j<10;++j){
				fscanf(fp,"%d",&dumint);
				if(dumint==-1){
					break;
				}
			}
			fscanf(fp,"%lf%lf",&dumdouble,&dumdouble);
			fscanf(fp,"%x",&dumint);
			
			fscanf(fp,"%lf",&mp[nn].dtAmount);
		}else if( buf[0] == 0x00 ) break;
			
			
			
	}
	for(nn=0; mp[nn].lower[0] != -1 ; nn = mp[nn].lower[0]);

	ss->base = nn;

	return ss;
}


/*----------------------------------------------------------------
	SSPACE *OutPutModelS( int key )
	
----------------------------------------------------------------*/
//	計算モデルを出力
//
SSPACE *OutPutModelS( int key ){
	JOINT *mp;
	SSPACE *sp;
	int i,j,nn,num;
	FILE *fp;
	static char buf[128];
	char *p;
	
	sp = GetSpaceAddr( key );
	mp = (JOINT*)(sp+1);
	
	fp=stdout;
	
	fprintf(fp,"%d\n\n",sp->num);

	for(nn=0;nn<sp->num;++nn){
		fprintf(fp,"#%d\n",nn);
		fprintf(fp,"%lf\t%lf\t%lf\n",mp[nn].ApData[0],mp[nn].ApData[1],mp[nn].ApData[2]);
		fprintf(fp,"%lf\t%lf\t%lf\n",mp[nn].AaData[0],mp[nn].AaData[1],mp[nn].AaData[2]);
		fprintf(fp,"%lf\t%lf\n",mp[nn].AxisLimit[0],mp[nn].AxisLimit[1]);
		for(j=0;j<10;++j){
			if(mp[nn].upper[j]==-1){
				fprintf(fp,"%d\n",mp[nn].upper[j]);
				break;
			}
			fprintf(fp,"%d\t",mp[nn].upper[j]);
		}
		for(j=0;j<10;++j){
			if(mp[nn].lower[j]==-1){
				fprintf(fp,"%d\n",mp[nn].lower[j]);
				break;
			}
			fprintf(fp,"%d\t",mp[nn].lower[j]);
		
		}
		fprintf(fp,"%lf\t%lf\n",mp[nn].kp[0],mp[nn].kp[1]);
		fprintf(fp,"0x%x\n",mp[nn].TypeOfJoint);
		fprintf(fp,"%lf\n\n",mp[nn].dtAmount);
						
	}
	shmdt( sp );
	return sp;
}


/*************************************************************************

	形状制御のための点列を共有メモリ内に生成する関数

**************************************************************************/


/*----------------------------------------------------------------
	SSPACE *GetNewPath( int	key, int Ssize );
	
----------------------------------------------------------------*/
SSPACE *GetNewPath( int key, int num )
{
	SSPACE	*ret;
	int 	shmkey;
	int	Ssize ;
	
	Ssize = num*sizeof(PATH)+sizeof(SSPACE);
	
    if( -1 == (shmkey = shmget( key, Ssize, IPC_CREAT|IPC_EXCL|0666 ))) {
        		perror("shmget");
             	return (SSPACE*)NULL;
   }

    // 共有メモリアドレスを得る
    if((SSPACE*)-1==(ret=(SSPACE*)shmat(shmkey,(void*)NULL, 0 )) ){
       	 perror("shmget");
           return (SSPACE*)NULL;
    }
	memset(ret,0,Ssize);
	return ret;
	
}
/*----------------------------------------------------------------
	SSPACE *GetPathAddr( int key )
	
----------------------------------------------------------------*/
SSPACE *GetPathAddr( int key )
{
	void	*ret;
	int 	shmkey;
	
    if( -1 == (shmkey = shmget( key, 0, IPC_CREAT|0666 ))) {
            	perror("shmget");
            	return (SSPACE*)NULL;
    }

    // 共有メモリアドレスを得る
    if((SSPACE*)-1==(ret=(SSPACE*)shmat(shmkey,(void*)NULL, 0 )) ){
       	 perror("shmget");
           return (SSPACE*)NULL;
    }

	return ret;
	
}

/*----------------------------------------------------------------
	SSPACE *GenPathFromFileS( int key, char *fname )
	
----------------------------------------------------------------*/
//	ファイルからデータを読み込み計算モデルを生成
//
SSPACE *GenPathFromFileS( int key, char *fname ){
	PATH *mp;
	SSPACE *ss;
	int i,j,nn,num;
	FILE *fp;
	char *p;
	
	if((FILE*)NULL>=(fp=fopen(fname,"r"))){
		printf("%s is no exist!!!\n",fname);
		return NULL;
	}
	
	fscanf(fp,"%d",&num);
	
	if(NULL == (ss=GetNewPath(key,num))){
		return NULL;
	}
	
	if( NULL == (p=strrchr(fname, '/')) ) p=fname;
	else p++;
	
	ss->num = num;
	strncpy( ss->name, p, 79 );
	mp = (PATH*)(ss + 1) ;
	
	for(i=0;i<num;++i){
		if( i == 0 )mp[i].no = -2;
		else mp[i].no = -1;
		mp[i].dist = 99999999.99;
		fscanf(fp,"%lf%lf%lf",&mp[i].p[0],&mp[i].p[1],&mp[i].p[2]);			
	}
	fclose(fp);
	return ss;
}
/*----------------------------------------------------------------
	SSPACE *ReGenPathFromFileS( int key, char *fname )
	
----------------------------------------------------------------*/
//	ファイルからデータを読み込み計算モデルを生成
//
SSPACE *ReGenPathFromFileS( int key, char *fname ){
	PATH *mp;
	SSPACE *ss;
	int i,j,nn,num;
	FILE *fp;
	char *p;
	
	if((FILE*)NULL>=(fp=fopen(fname,"r"))){
		printf("%s is no exist!!!\n",fname);
		return NULL;
	}
	
	fscanf(fp,"%d",&num);
	
	if(NULL == (ss=GetNewPath(key,num))){
		return NULL;
	}
	
	if( NULL == (p=strrchr(fname, '/')) ) p=fname;
	else p++;
	
	ss->num = num;
	strncpy( ss->name, p, 79 );
	mp = (PATH*)(ss + 1) ;
	
	for(i=0;i<num;++i){
		mp[i].dist = 99999999.99;
		fscanf(fp,"%d%lf%lf%lf",&mp[i].no,&mp[i].p[0],&mp[i].p[1],&mp[i].p[2]);			
	}
	fclose(fp);
	return ss;
}

/*----------------------------------------------------------------
	SSPACE *OutPutPathS( int key )
	
----------------------------------------------------------------*/
//	PATHを出力
//
SSPACE *OutPutPathlS( int key ){
	PATH *mp;
	SSPACE *sp;
	int i,j,nn,num;
	FILE *fp;
	static char buf[128];
	char *p;
	
	sp = GetPathAddr( key );
	mp = (PATH*)(sp+1);
	
	fp=stdout;
	
	fprintf(fp,"%d\n",sp->num);

	for(nn=0;nn<sp->num;++nn){
		fprintf(fp,"%d\t%lf\t%lf\t%lf%e\n",mp[nn].no,mp[nn].p[0],mp[nn].p[1],mp[nn].p[2],mp[nn].dist);
					
	}

	return sp;
}
