#!/bin/bash

ipcrm -M 100
LoadRobo 100 ./12arm.dat 
ipcrm -M 200
LoadRobo 200 ./marumaru.dat 




nn=1
ano=6

rm All.txt
rm Result.txt
rm Theta.dat
rm data*
rm tdata.txt


PMove 200 5 -20 25
rm MARU.dat

F_Kine 100  < InitPose.dat
PosePlotS 100 >> All.txt 
while [ $nn -le $ano ]
do
	PosePlotS 200 >> MARU.dat
	GetPositions 200 < marup.txt > tmp.dat
	echo "2" > tmp.g
	sed s/12\\t/8\\t/ tmp.dat >tmp.d
	sed s/4\\t/12\\t/ tmp.d >> tmp.g
	cat tmp.g >> tdata.txt
	#echo "4 0.0	20.0 19.0" >>tmp.g

 	#head -$nn Targets.txt | tail -1 > tmp.g
 	RoboCnt 100  < tmp.g >> Result.txt
 	PosePlotS 100 > data$nn
 	PosePlotS 100 >> All.txt 
 	OutPutTh 100 >> Theta.dat
 	let "nn=1+nn"	
	PMove 200 -2 -2 2


done
ResCalc < Result.txt >ResultOrg.txt
awk '{print $9}' Result.txt | nl -b a > Jtip.dat
awk '{print $11}' Result.txt | nl -b a > J2nd.dat

awk '{print $2 "\t" $3 "\t" $4}' tdata.txt > TP.dat




