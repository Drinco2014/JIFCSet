/*-----------------------------------------------------------------------

	
	
	Hayashi & Satake Kinematics Calculation Program
	
				Copylight 2016- DRINCO Project 

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "HS.h"
void printV(double a[3]);
double InnerP(double a[3],double b[3]);
double Cross(double a[3],double b[3],double c[3]);
int main(){

	double na[3]={1,0,0};
	double ra[3]={1,-10,0},A[3]={0,0,1};
	double aas,cs,c[3],d[3], BB;
	int i;

	//VecNormal(e,ne);
	//VecNormal(g,ng);

	Cross(na,ra,c);
	printf("Cross:");
	printV(c);
	cs = sqrt(InnerP(c,c)) ;
	for(i=0;i<3;i++){
		d[i] = c[i] / cs ;
	}
	BB = InnerP(d,A);



aas = acos( InnerP( na, ra )/ (sqrt(InnerP(na,na)) * sqrt(InnerP(ra,ra))) ) ;
	printf("aas = %e %lf BB=%e cs= %e\n",aas, aas * 180.0 / PI,BB,cs);
	

aas = asin( cs / (sqrt(InnerP(na,na)) * sqrt(InnerP(ra,ra))) ) ;

	printf("aas = %e %lf BB=%e cs= %e\n",aas, aas * 180.0 / PI,BB,cs);


}


