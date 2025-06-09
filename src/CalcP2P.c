#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[] ){
	char s[128];
	double p[4];
	while(EOF != scanf("%lf%lf%lf%lf",&p[0],&p[1],&p[2],&p[3] )){
		printf("%e\n",sqrt(pow((p[2]-p[0]),2.0)+pow((p[3]-p[1]),2.0)));
	}
}
			
