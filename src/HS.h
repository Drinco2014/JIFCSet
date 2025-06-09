/*-----------------------------------------------------------------------

	HS.h
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/

typedef struct _joint_ {
// Current Status
	double 	ApData[3];	//=_AP_DATA_; 位置
	double 	AaData[3];	//=_AA_DATA_;　方向
	double 	dtData;		//求めた変位
	double 	NumberOfDtData;	//このジョイントを使うEFの数
	double 	dtAmount;	//積算変位
	double 	AxisLimit[2];	//=_AXIS_LIMIT_; 軸の変位リミット
	int 	upper[10];	//上位リンクの番号
	int 	lower[10];	//下位リンクの番号
	double 	kp[2];		//=_K_PARA_; 協調パラメータ
	int	TypeOfJoint; 	// 0x00:回転, 0x01:並進
	int	LowerJno;	//下位のジョイント数（協調パラメータのために）
	int	TaskPointFlg;	//タスク点である場合は”１”そうでない場合はその他。

} JOINT ;
#define LINER	0x01
#define STOP	0x02

#define	PI 3.1415927

extern double Distance( double a[3], double b[3]);
extern int SP_proc_IK_p(double efc[3], double r[3], double ak, JOINT j[],int jn );
extern int SP_proc_DK( int fl, double T16[16] ,JOINT jt[],int jn);
extern JOINT *GenModelFromFile( char *fname, int *num );
extern int OutPutPointsData(FILE *fp, JOINT *jd, int no, double v[3]);
extern int CountLowerJoints( int no ,JOINT jt[],int jn);


typedef struct Pathdata {
	int no;
	double p[3];
	double dist;
} PATH ;







