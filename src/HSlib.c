/*-----------------------------------------------------------------------

	HSlib.c
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/





#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "HS.h"


void printV(double a[3]);

//spdk.cより引用
/*----------------------------------------------------------------
 *
 *   ロドリゲスの公式を使った同次変換行列を作る    
 *
 *  int Matrix_T( i
 *  	double a[3], 回転軸方向ベクトル
 *  	double p[3], 位置ベクトル
 *  	double th,   回転角度
 *  	double T[4][4] 作成される同次変換行列
 *  	)
 -----------------------------------------------------------------*/
int Matrix_T( double a[3], double p[3], double th, double T[4][4] ){

	T[0][0] = (- pow( a[2], 2.0) - pow(a[1], 2.0))*(1.0 - cos( th )) + 1.0 ; 
	T[0][1] = a[0] * a[1] * (1.0 - cos( th )) - a[2] * sin( th );
	T[0][2] = a[0] * a[2] * (1.0 - cos( th )) + a[1] * sin( th );
	T[0][3] = p[1] * (a[2] * sin( th ) - a[0] * a[1] * (1.0 - cos( th ))) 
		+ p[2] * (- a[1] * sin( th ) - a[0] * a[2] * (1.0 - cos( th ))) 
		- (- pow( a[2], 2.0 ) - pow( a[1], 2.0 )) * p[0] * (1.0 - cos( th )) ;

	T[1][0] =  a[2] * sin( th ) + a[0] * a[1] * ( 1.0 - cos( th ));
	T[1][1] =  (- pow( a[2], 2.0 ) - pow( a[0], 2.0 )) * ( 1.0 - cos( th )) + 1.0;
	T[1][2] =  a[1] * a[2] * (1.0 - cos( th )) - a[0] * sin( th );
	T[1][3] = p[0] * (- a[2] * sin( th ) - a[0] * a[1] * (1.0 - cos( th ))) 
		+ p[2] * ( a[0] * sin( th ) - a[1] * a[2] * (1.0 - cos( th ))) 
		- (- pow( a[2], 2.0 ) - pow( a[0], 2.0 )) * p[1] * (1.0 - cos( th )) ;

	T[2][0] = a[0] * a[2] * (1.0 - cos( th )) - a[1] * sin( th );
	T[2][1] = a[0] * sin( th ) + a[1] * a[2] * ( 1.0 - cos( th )); 
	T[2][2] = (- pow( a[1], 2.0 ) - pow( a[0], 2.0 )) * (1.0 - cos( th )) + 1.0;
	T[2][3] = p[0] * (a[1] * sin( th ) - a[0] * a[2] * (1.0 - cos( th ))) 
		+ p[1] * (- a[0] * sin( th ) - a[1] * a[2] * (1.0 - cos( th ))) 
		- (- pow( a[1], 2.0 ) - pow( a[0], 2.0 )) * p[2] * (1.0 - cos( th )) ;

	T[3][0] = 0.0 ; T[3][1] =  0.0 ; T[3][2] =  0.0 ; T[3][3] = 1.0 ;
	//printf("matrix1_executed¥n") ;

	return 1;

}

int Matrix_T2( double a[3], double p[3], double th, double T[4][4] ){

	T[0][0] = 1.0 ;	T[0][1] = 0.0 ;	T[0][2] = 0.0 ;	T[0][3] = a[0] * th ;

	T[1][0] = 0.0 ;	T[1][1] = 1.0 ;	T[1][2] = 0.0 ;	T[1][3] = a[1] * th ;

	T[2][0] = 0.0 ;	T[2][1] = 0.0 ;	T[2][2] = 1.0 ;	T[2][3] = a[2] * th ;

	T[3][0] = 0.0 ;	T[3][1] = 0.0 ; T[3][2] = 0.0 ; T[3][3] = 1.0 ;

	return 1;

}

int Matrix_T3( double a[3], double p[3], double th, double T[4][4] ){

	T[0][0] = 1.0 ;	T[0][1] = 0.0 ;	T[0][2] = 0.0 ;	T[0][3] = a[0] ;

	T[1][0] = 0.0 ;	T[1][1] = 1.0 ;	T[1][2] = 0.0 ;	T[1][3] = a[1] ;

	T[2][0] = 0.0 ;	T[2][1] = 0.0 ;	T[2][2] = 1.0 ;	T[2][3] = a[2] ;

	T[3][0] = 0.0 ;	T[3][1] = 0.0 ; T[3][2] = 0.0 ; T[3][3] = 1.0 ;

	return 1;

}

/*----------------------------------------------------------------
 *
 *   同次変換行列積    現在軸の行列(Tc).下位の行列(Tl)
 *	Tc.Tlを求める。
 *  int TCur_x_TLow( 
 *  	double Tc[4][4], 現在軸の同次変換行列
 *  	double Tl[4][4], 下位軸すべての軸の同次変換行列の積
 *  	double Tu[4][4]  積（上位軸に渡す同次変換行列）
 *  	)
 -----------------------------------------------------------------*/
int TCur_x_TLow( double Tc[4][4], double Tl[4][4], double Tu[4][4] ){

	int i,j,k;
	double s;

	for(i=0;i<4;++i){
		for(j=0;j<4;++j){
			for(s=0.0,k=0;k<4;++k){
				s += Tc[i][k] * Tl[k][j];
			}
			Tu[i][j] = s;
		}
	}
	return 1;
}

/*----------------------------------------------------------------
 *
 *   下位同次変換行列を用いて、位置ベクトルと軸方向ベクトルを求める
 *
 *  int TLow_x_PorA( double Tl[4][4], double PA[4][1], double nPA[4][1] ){
 *
 *  		double Tl[4][4], 下位軸すべての軸の同次変換行列の積
 *  		double PA[4][1], 位置ベクトルあるいは、方向ベクトル。
 *  					方向ベクトルは4行目が０位置ベクトルは１
 *  		double nPA[4][1]　求められた位置あるいは方向ベクトル
 *
 *  	)
 -----------------------------------------------------------------*/
int TLow_x_PorA( double Tl[4][4], double PA[4][1], double nPA[4][1] ){

	int i,j,k;
	double s;

	for(i=0;i<4;++i){
		for(j=0;j<1;++j){
			for(s=0.0,k=0;k<4;++k){
				s += Tl[i][k] * PA[k][j];
			}
			nPA[i][j] = s;
		}
	}
	return 1;
}
/*--------------------------------------------------------------------
 *	TLow_x_PorA 関数を使って位置ベクトルを変更する関数
 *
 * 	int NewPosi( 
 *
 *  		double Tl[4][4], 下位軸すべての軸の同次変換行列の積
 * 		double Posi[3]   位置ベクトル
 *
 * 		 )
 *
 ----------------------------------------------------------------------*/
int NewPosi( double Tl[4][4], double Posi[3] ){
	double PA[4][1],nPA[4][1];

	PA[0][0]=Posi[0];PA[1][0]=Posi[1];
	PA[2][0]=Posi[2];PA[3][0]=1.0;

	TLow_x_PorA(Tl,PA,nPA);

	Posi[0]=nPA[0][0];
	Posi[1]=nPA[1][0];
	Posi[2]=nPA[2][0];
	return 1;
}


/*--------------------------------------------------------------------
 *	TLow_x_PorA 関数を使って軸方向ベクトルを変更する関数
 *
 * 	int NewAxis( 
 *
 *  		double Tl[4][4], 下位軸すべての軸の同次変換行列の積
 * 		double Posi[3]   軸方向ベクトル
 *
 * 		 )
 *
 ----------------------------------------------------------------------*/
int NewAxis( double Tl[4][4], double Posi[3] ){
	double PA[4][1],nPA[4][1];

	PA[0][0]=Posi[0];PA[1][0]=Posi[1];
	PA[2][0]=Posi[2];PA[3][0]=0.0;

	TLow_x_PorA(Tl,PA,nPA);

	Posi[0]=nPA[0][0];
	Posi[1]=nPA[1][0];
	Posi[2]=nPA[2][0];
	return 1;
}

int printV4( double m[4][4] ){
	int i,j;

	for(i=0;i<4;++i){
		for(j=0;j<4;++j){
			printf("  %lf",m[i][j]);
		}
		printf("¥n");
	}
	return 1;
}
	

//drvlib.cより引用
// ベクトルの和　第3引数が解
double AddV(double a[3],double b[3],double result[3]){
	int i;
	for(i=0;i<3;i++){
		result[i]=a[i]+b[i];
	}
	return result[0];
}

// ベクトルの差　第3引数が解
double SubV(double a[3],double b[3],double result[3]){
	int i;
	for(i=0;i<3;i++){
		result[i]=a[i]-b[i];
	}
	return result[0];
}

// ベクトルの外積　第3引数が解
double Cross(double a[3],double b[3],double c[3]){
	c[0]=a[1]*b[2]-a[2]*b[1];
	c[1]=a[2]*b[0]-a[0]*b[2];
	c[2]=a[0]*b[1]-a[1]*b[0];
	return c[0];
}

// ベクトルの内積　
double InnerP(double a[3],double b[3]){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

//
// long double 版
//
// ベクトルの和　第3引数が解
long double AddVl(long double a[3],long double b[3],long double result[3]){
	int i;
	for(i=0;i<3;i++){
		result[i]=a[i]+b[i];
	}
	return result[0];
}

// ベクトルの差　第3引数が解
long double SubVl(long double a[3],long double b[3],long double result[3]){
	int i;
	for(i=0;i<3;i++){
		result[i]=a[i]-b[i];
	}
	return result[0];
}

// ベクトルの外積　第3引数が解
long double Crossl(long double a[3],long double b[3],long double c[3]){
	c[0]=a[1]*b[2]-a[2]*b[1];
	c[1]=a[2]*b[0]-a[0]*b[2];
	c[2]=a[0]*b[1]-a[1]*b[0];
	return c[0];
}

// ベクトルの内積　
long double InnerPl(long double a[3],long double b[3]){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
//------------------------------------------------
//------------------------------------------------

double Distance( double a[3], double b[3]){
	double result[3], dd;
	SubV(a,b,result);
	dd=sqrt(InnerP(result,result));
	return dd;
}

// ベクトルの単位ベクトル化
double VecNormal( double a[3], double ret[3] ){

	int i;
	double rv;
	
	rv = sqrt(InnerP(a,a));
	if(rv<0.000000000001){return 0.0;}
	for(i=0;i<3;++i){
		ret[i]=a[i]/rv;
	}

	return rv;
}

// ベクトルスカラー倍

void VecSt( double sv, double vec[3], double ret[3] ){

	int i;
	for(i=0;i<3;++i)ret[i]=sv*vec[i];

	return;
}
	

//駆動軸oからみた指令位置rと現在位置pとの偏角の駆動軸a方向成分の抽出
double Cc(double a[3],double o[3],double r[3],double p[3]){

	double na[3],ra[3],c[3],d[3], cs, aas,ratio,na2,ra2;
	int i;
	double ff, ret, Nr[3] ;
	double	nna[3],nra[3],vlna[3],lna,ntr[3],lntr;

	
	SubV(r,o,ra); // g Target
	SubV(p,o,na); // e Effecter
	
	Cross(na,ra,c);
	cs = sqrt(InnerP(c,c)) ;
	
	if(cs<10E-10){
		//printf("CC NULL\n");
		return 0.0;
	}
	
	for(i=0;i<3;i++){
	  d[i] = c[i] / cs ;
	}
	
	ratio = InnerP( d, a );

	na2=sqrt(InnerP(na,na));
	ra2=sqrt(InnerP(ra,ra));

	
/**/

	if( InnerP(na,ra) < 0.00 ){
	
		ff = cs / (na2*ra2);
		if( fabs(ff) > 1.0 ){
			 aas = 0.0;
		}
		else aas = asin( ff ) ;

	}else{
	
		ff = InnerP( na, ra ) / (na2*ra2);
		if( fabs(ff) > 1.0 ){
			 aas = 0.0;
		}
		else aas = acos( ff ) ;

	}
/**/	
	ret = aas * ratio;

	return ret;

}

//並進軸の計算
double Hc(double a[3],double o[3],double r[3],double p[3]){
	double na[3],ra[3],c[3],d[3], cs, aas;
	int i;

	SubV(r,p,na);
	
	return InnerP(na,a);
}

//指令姿勢rと現在姿勢pとの偏角の駆動軸a方向成分の抽出
double Ac( double a[3], double r[3], double p[3]){
	double na[3],ra[3],c[3],d[3],csth;
	int i;
	
	//if( 0.000001 > Distance(r,p) ) return 0.0;
	Cross(p,r,c);
	csth = asin( sqrt(InnerP(c,c)) ) ;
	for(i=0;i<3;i++){
	  d[i]= csth * a[i] ;
	}

	return InnerP(d,c);
}

double MR(double a[3],double q,double x[3],double result[3]){
	double b[3],c[3],cros[3], inn, ixc, sq,cq;
	//double result2[3];
	int i;

	if( (inn=InnerP(a,a)) <= 0.0 ) return 0;
	for(i=0;i<3;i++){
		c[i]=a[i]/*/inn*/;
	}

	Cross(c,x,cros);
	ixc = InnerP(x,c);
	sq = sin(q);
	cq = cos(q);
	for(i=0;i<3;i++){
		b[i]= ixc *c[i];
		result[i]=b[i]+cq*(x[i]-b[i])+sq*cros[i];
	}

	return a[0];	
}

void printV(double a[3]){
	printf("(%lf,%lf,%lf)\n",a[0],a[1],a[2]);
}

double Normalize( double a[3] ){
	int i;
	double inn;
	
	if( (inn=InnerP(a,a)) <= 0.0 ) return 0;
	for(i=0;i<3;i++){
		a[i] /= sqrt(inn);
	}
	return inn;
}
double Normalize2( double a[3], double b[3] ){
	int i;
	double inn;
	
	if( (inn=InnerP(a,a)) <= 0.0 ) return 0;
	for(i=0;i<3;i++){
		b[i] = a[i] / sqrt(inn);
	}
	return inn;
}
//引用ここまで




/*================================================================ 
 SP 位置の逆運動学

引数：
	efc		ＥＰの現在位置
	r		ＥＰ目標位置
	k		位置と姿勢，他の軸との協調係数

入力:   ジョイントの位置 (Ap)
	ジョイントの方向ベクトル (Aa)
	協調パラメータ (kp)	

出力:   ジョイントの変角 (Dtdata)
================================================================*/

int SP_proc_IK_p(double efc[3], double r[3], double ak, JOINT j[],int jn ){

	double temp[3],tempR[3];
	double dt1[1],th1[1], limth[2], k;
	double Aa1[3],Ap1[3];
	char *p1,*p2;
	FILE *fp;

#ifdef DEBUG
	printf("proc_IK_p in %d¥n",jn);
#endif


	if( ak <= 0 ) k = j[jn].kp[0];
	else k = ak;

// 状態，制限などのデータの取り出し
	Ap1[0]=j[jn].ApData[0]; 
	Ap1[1]=j[jn].ApData[1]; 
	Ap1[2]=j[jn].ApData[2];
	Aa1[0]=j[jn].AaData[0]; 
	Aa1[1]=j[jn].AaData[1]; 
	Aa1[2]=j[jn].AaData[2]; 
	th1[0]=j[jn].dtAmount;
//	SetRFCName("Get_Limit",M_Name);
//	Get_Limit(th1,limth);

#ifdef DEBUG
	printf("Ap1=");printV(Ap1);
	printf("Aa1=");printV(Aa1);
	printf("r=");printV(r);
	printf("efc=");printV(efc);
#endif

// 局所運動学計算

	if(j[jn].TypeOfJoint == LINER){
		dt1[0]=k*Hc(Aa1,Ap1,r,efc); // + ks*Ac(Aa1,rs,Es);	// 軸の制御
	}else if(j[jn].TypeOfJoint == STOP) {
		dt1[0]=0; // + ks*Ac(Aa1,rs,Es);	// 軸の制御
	}else{
		// 軸の制御
		dt1[0]=k*Cc(Aa1,Ap1,r,efc);
	}

	//printf("\t%d dt1=%e \n",jn,dt1[0]);
#ifdef DEBUG
	printf("¥tdt1=%lf¥n",dt1[0]);
	printf("¥tdt1=%lf(%d) %s¥n",dt1[0],fflg,thDataFileName);
	printf("%d %lf¥n",jn,dt1[0]);
#endif

// 角度制限のチェックと制限

	if( j[jn].dtAmount+dt1[0] > j[jn].AxisLimit[1] || j[jn].dtAmount+dt1[0] < j[jn].AxisLimit[0] ){
		if( j[jn].dtAmount+dt1[0] > j[jn].AxisLimit[1] ) dt1[0] = j[jn].AxisLimit[1]-j[jn].dtAmount;
		else if( j[jn].dtAmount+dt1[0] < j[jn].AxisLimit[0] ) dt1[0] = j[jn].AxisLimit[0]-j[jn].dtAmount;
		//printf("<<%d>>th1: %lf dt1: %lf Limit: {%lf,%lf}\n",
		//	jn,j[jn].dtAmount,dt1[0],j[jn].AxisLimit[0],j[jn].AxisLimit[1]);
	}

	j[jn].dtData+=dt1[0];
	j[jn].NumberOfDtData++;
	return 0;
}

/*================================================================ 
 SP Motion Link module program

引数：
	fl		FLAG
	T		同次変換行列
機能：
構造に従って，回転した軸の情報を引数として上位のＤＫを呼ぶ．
================================================================*/
/*int SP_proc_ML( int fl, double T[16]){
	int i,j,k;
	double Ts[16];

#ifdef DEBUG
	printf("SP_proc_ML in %s¥n",MOD_NAME);
#endif
	for(i=0;i<30 && M_Name[i][0]!='¥0';++i){

	for(k=0;k<4;k++)
	{
		for(j=0;j<4;j++)
		{
			Ts[k*4+j]=T[k*4+j];
			//printf("%lf ",T[k*4+j]);
		}
		//printf("¥n");
	}	
		SetRFCName("SP_proc_DK",M_Name[i]);
		SP_proc_DK( fl, Ts );
	}
	return 1;
}

*/
/*================================================================ 
 Spetial 局所順運動学計算（ＭＬより呼ばれる）
  int SP_proc_EP_main( int a, char *b )
実行の際に使われる。

引数：
	fl		フラグ。Tが無い場合は、０以外
	Tl		同次変換行列（下位のDKで合成されたもの）

	Tcc  Tl*Tc
機能：
下位の軸の変位に伴い変化する自身の回転軸の位置と方向を求め、
同次変換行列を合成する。
================================================================*/
int SP_proc_DK( int fl, double T16[16] ,JOINT jt[],int jn){

	double Tc[4][4],Tu[4][4],T[4][4],Tu16[16];
	int k,j,i;

	if( jt[jn].NumberOfDtData > 0.0 ){
		jt[jn].dtData /= 	jt[jn].NumberOfDtData;
	}else{
		jt[jn].dtData = 0.0 ;
	}
	
	jt[jn].dtAmount += jt[jn].dtData ;
	
#ifdef DEBUG
	printf("SP_proc_DK in %d (%lf,%lf,%lf) ¥n",jn,ApData[0],ApData[1],ApData[2]);
#endif
	memcpy(T,T16,sizeof(double)*16);
	if( fl == 0 ){
		NewPosi( T, jt[jn].ApData );
		NewAxis( T, jt[jn].AaData );
//#ifdef LINER
		if(jt[jn].TypeOfJoint == LINER){
			Matrix_T2( jt[jn].AaData, jt[jn].ApData, jt[jn].dtData, Tc );
			//printf("LINER_on¥n");

		}else{
//#else
			Matrix_T( jt[jn].AaData, jt[jn].ApData, jt[jn].dtData, Tc );
			//printf("LINER_off¥n");
		}
//#endif
		TCur_x_TLow( Tc, T, Tu );
	}else{
//#ifdef LINER
		if(jt[jn].TypeOfJoint == LINER){
			Matrix_T2( jt[jn].AaData, jt[jn].ApData, jt[jn].dtData, Tu );
			//printf("LINER_on¥n");
		}else{
//#else
			Matrix_T( jt[jn].AaData, jt[jn].ApData, jt[jn].dtData, Tu );
			//printf("LINER_off¥n");
		}
//#endif
	}
#ifdef DEBUG
	printf("¥t¥t(%lf,%lf,%lf)¥n",ApData[0],ApData[1],ApData[2]);
#endif
	jt[jn].dtData = 0.0;
	jt[jn].NumberOfDtData = 0.0;

	memcpy(Tu16,Tu,sizeof(double)*16);

	for(i=0;i<10;i++){
		if(jt[jn].upper[i]==-1) return 0;
		
		SP_proc_DK(0,Tu16,jt,jt[jn].upper[i]);
	}
	return 1;
}
//
//	ファイルからデータを読み込み計算モデルを生成
//
JOINT *GenModelFromFile( char *fname, int *num ){
	JOINT *mp;
	int i,j,nn;
	FILE *fp;
	static char buf[128];
	
	if((FILE*)NULL>=(fp=fopen(fname,"r"))){
		printf("%s is no exist!!!\n",fname);
		return NULL;
	}
	
	fscanf(fp,"%d",num);
	
	if(NULL == (mp=malloc(sizeof(JOINT)*(*num+1)))){
		return NULL;
	}
	
	for(i=0;;){
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
	return mp;
}

//
//	表示要データの出力（リンクの線分データの形式）
//
int OutPutPointsData(FILE *fp, JOINT *jd, int no, double v[3]){
	int i;
	
	if(v != NULL){
		fprintf(fp,"%lf\t%lf\t%lf\n",v[0],v[1],v[2]);
		fprintf(fp,"%lf\t%lf\t%lf\n\n\n",	jd[no].ApData[0],
						jd[no].ApData[1],
						jd[no].ApData[2]);
	}
	
		
	for(i=0;i<10;++i){
		if(jd[no].upper[i] == -1) break;
		OutPutPointsData(fp,jd,jd[no].upper[i],jd[no].ApData);
	}
	
	return 0;
}
	

// 下位ジョイントの数を調べていく関数
//
//

int CountLowerJoints( int no ,JOINT jt[],int jn){

	int k,j,i;

	jt[jn].LowerJno = no;
	no++;
	for(i=0;i<10;i++){
		if(jt[jn].upper[i]==-1) return 0;
		CountLowerJoints(no,jt,jt[jn].upper[i]);
	}
	return 1;
}



