/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

int main()
{
	static char line[256],dum[20],dd[20],dd2[20];
	char *rr;
	double D,T;
	int R, maxR,minR,sumR;
	double maxD,maxT,sumD,sumT;
	double minD,minT;
	double didimin,didimax;
	int i;
	
	sumD=sumT=0.0;
	sumR=0;
	for(i=0; NULL != fgets(line,256,stdin) ; ++i){
		sscanf(line,"%s%lf%lf%s%d%s%lf%lf",dd2,&D,&T,dum,&R,dd,&didimin,&didimax);
		sumD += D;
		sumR+=R;
		sumT+=T;
		if(i==0){maxD=minD=D;maxR=minR=R;minT=maxT=T;}
		else{
			if ( D < minD ) minD = D ;
			else if ( D > maxD ) maxD = D;
			if ( T < minT ) minT = T ;
			else if ( T > maxT ) maxT = T;
			if ( R < minR ) minR = R ;
			else if ( R > maxR ) maxR = R;
		}
	}
			
	printf("\tError\tTime(microsec)\tCount\n");
	printf("Max\t%e\t%lf\t%d\n",maxD,maxT,maxR);
	printf("Min\t%e\t%lf\t%d\n",minD,minT,minR);
	printf("Ave\t%e\t%lf\t%d\n",sumD/i,sumT/i,sumR/i);
	printf("Sum\t%e\t%lf\t%d\n",sumD,sumT,sumR);
	printf("Time/Calc(microsec)\t%lf\n",sumT/sumR);
	
	return 0;
}
		
