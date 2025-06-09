/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/

#define SHMKEYCODE	(key_t)0x1234
#define ROBOMAX		10

#define ROBONAMELENGTH
typedef struct _Sspace_ {
	char 	name[80]; // Name of a Robot
	int	num; 	// Number of joints of the robot
	int	base;	// No. of base joint
} SSPACE ;

extern SSPACE *GenSspace( int num , char name[ROBONAMELENGTH] );
extern SSPACE *GetRobo( char name[ROBONAMELENGTH] );


