#!/bin/bash

ipcrm -M 100
LoadRobo 100 ./3arm.dat 

nn=2
ano=34

rm All.txt
rm Result.txt
rm Theta.txt

while [ $nn -le $ano ]
do
 	echo 1 > tmp.g
 	echo 3 >> tmp.g
 	head -$nn CL3.txt | tail -1 >> tmp.g
 	RoboCnt 100 -d < tmp.g >> Result.txt
 	PosePlotS 100 > data$nn
 	PosePlotS 100 >> All.txt 
 	OutPutTh 100 >> Theta.txt
 	let "nn=1+nn"	
done

