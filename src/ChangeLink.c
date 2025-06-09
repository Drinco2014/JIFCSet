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
	int Adjnum;
	double	dth;
	double	tt[16];
	static	char buf[100];
	FILE *fp;
	SSPACE *sp;
	int *M;
	int k;
	int count;
	k=i=j=nn=count=0;
	Adjnum=0;
	if( argc<2 ){
		printf("arguments: <Robot Key Code> \n");
		return 0;
	}
	//Jointデータの読み込み
	sp = GetSpaceAddr( atoi(argv[1]) );
	mp = (JOINT*)(sp+1);
	
	fp=stdin;//fopen(argv[2],"r");
	mp = (JOINT*)(sp + 1) ;
	
	scanf("%d",&Adjnum);

	if(Adjnum!=sp->num){
		printf("隣接行列が正しくありません\n");
		return 0;
	}

	M=(int*)malloc(sp->num*sp->num*sizeof(int));//メモリ確保
	
//	printf("%d\n",sp->num);
	
	for(i=0;i<=sp->num*sp->num;i++){
		M[i]=0;

	}
	
//	printf("隣接行列を入力してください[行:lower  列:upper]\n");
	for(i=0;i<sp->num*sp->num;i++){
		
		scanf("%d",&M[i]);
		
	
	}
/*	
	for(i=0;i<sp->num*sp->num;i++){
		printf("%d",M[i]);
		count++;
		if(count==sp->num){
			printf("\n");
			count=0;
		}
	}
*/	
		//ジョイントの数だけループ
	for(i=0;i<sp->num;++i){
		
		//upperの書き換え
		for(j=0;j<sp->num;++j){
			if(M[i*sp->num+j]==1){
				mp[i].upper[k]=j;
				k++;
			}
		}
		mp[i].upper[k]=-1;
		k=0;
		
		for(j=0;j<sp->num;++j){
			if(M[j*sp->num+i]==1){
				mp[i].lower[k]=j;
				k++;
			}else if(M[j*sp->num+i]==0){
				count++;
				if(count==sp->num){
				sp->base=i;
				}
			}
		}
		mp[i].lower[k]=-1;
		k=0;
		count=0;
		mp[i].kp[0]=0.01;
		mp[i].kp[1]=0.01;
	}
	
//	printf("\n");
	shmdt( sp );
	free(M);

	return 0;
}

