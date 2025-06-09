#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[] ){
	char s[128];
	double p[6];
	while(EOF != scanf("%lf%lf%lf%lf%lf%lf",&p[0],&p[1],&p[2],&p[3],&p[4],&p[5] )){
		printf("%e\n",sqrt(
				pow((p[3]-p[0]),2.0)
				+pow((p[4]-p[1]),2.0)
				+pow((p[5]-p[2]),2.0)
				));
	}
}
			
