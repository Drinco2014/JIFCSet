/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

int main()
{
	static char line[1024],dum[256],*rr,dd[256];
	double D,T;
	int R, maxR,minR,sumR;
	double maxD,maxT,sumD,sumT;
	double minD,minT;
	int i;
	
	sumD=sumT=0.0;
	sumR=0;
	for(i=0; NULL != fgets(line,1024,stdin) ; ++i){
		sscanf(line,"%le%le%s%d",&D,&T,dum,&R);
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
			
	printf("\tError\tTime(sec)\tCount\n");
	printf("Max\t%e\t%lf\t%d\n",maxD,maxT,maxR);
	printf("Min\t%e\t%lf\t%d\n",minD,minT,minR);
	printf("Ave\t%e\t%lf\t%d\n",sumD/i,sumT/i,sumR/i);
	printf("Sum\t%e\t%lf\t%d\n",sumD,sumT,sumR);
	printf("Time/Calc(sec)\t%lf\n",sumT/sumR);
	
	return 0;
}
		
