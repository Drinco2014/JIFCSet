SRCDIR=../src/
OBJDIR=./
CC = gcc -g 
TARGET = PosePlotF LoadRobo PosePlotS RoboCnt RoboCntCP OutPutTh SaveRobo ReLoadRobo \
PMove F_Kine GetPositions OutputSt Chgstruct OutLabel PrintJposi costest ResCalc ResCalc2 CalcP2P-3d\
LoadPath  OutPutPath OutPutTraceNext ReLoadPath OutPutTaskPoints RoboCntEx1000 \
RoboCntEx100 RoboCntEx10 RoboCntEx5000 MultiDist Inherit RoboCntEx10000 OutPutThFkine CheckLink ChangeLink CheckAll MakeAdj CheckAdj
LIB= -lm -g

all:    $(TARGET)

CalcP2P-3d:$(SRCDIR)CalcP2P-3d.c
	$(CC) $^ -o $@ $(LIB)

ResCalc2:$(SRCDIR)ResCalc2.c
	$(CC) $^ -o $@ $(LIB)

ResCalc:$(SRCDIR)ResCalc.c
	$(CC) $^ -o $@ $(LIB)

costest: $(OBJDIR)costest.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

costest.o:$(SRCDIR)costest.c
	$(CC) -c $^ -o $@


MultiDist: $(OBJDIR)MultiDist.o $(OBJDIR)HSlib.o $(OBJDIR)spaceLib.o 
	$(CC) $^ -o $@ $(LIB)

PrintJposi: $(OBJDIR)PrintJposi.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

OutLabel: $(OBJDIR)OutLabel.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

Chgstruct: $(OBJDIR)Chgstruct.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

OutputSt: $(OBJDIR)OutputSt.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

GetPositions: $(OBJDIR)GetPositions.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

F_Kine: $(OBJDIR)F_Kine.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

PMove: $(OBJDIR)PMove.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

ReLoadRobo: $(OBJDIR)ReLoadRobo.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

Inherit: $(OBJDIR)Inherit.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

SaveRobo: $(OBJDIR)SaveRobo.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

OutPutTh: $(OBJDIR)OutPutTh.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

OutPutThFkine: $(OBJDIR)OutPutThFkine.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

RoboCnt: $(OBJDIR)RoboCnt.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

RoboCntCP: $(OBJDIR)RoboCntCP.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

RoboCntEx1000: $(OBJDIR)RoboCntEx1000.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

RoboCntEx100: $(OBJDIR)RoboCntEx100.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

RoboCntEx10: $(OBJDIR)RoboCntEx10.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

RoboCntEx5000: $(OBJDIR)RoboCntEx5000.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

RoboCntEx10000: $(OBJDIR)RoboCntEx10000.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

LoadPath: $(OBJDIR)LoadPath.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

ReLoadPath: $(OBJDIR)ReLoadPath.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

OutPutPath: $(OBJDIR)OutPutPath.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

OutPutTraceNext: $(OBJDIR)OutPutTraceNext.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

OutPutTaskPoints: $(OBJDIR)OutPutTaskPoints.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

PosePlotS: $(OBJDIR)PosePlotS.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

PosePlotF: $(OBJDIR)PosePlot.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

LoadRobo: $(OBJDIR)LoadRobo.o $(OBJDIR)HSlib.o  $(OBJDIR)spaceLib.o
	$(CC) $^ -o $@ $(LIB)



CheckLink: $(OBJDIR)CheckLink.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)
	
CheckAll: $(OBJDIR)CheckAll.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)

ChangeLink: $(OBJDIR)ChangeLink.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)
	
MakeAdj: $(OBJDIR)MakeAdj.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)
	
CheckAdj: $(OBJDIR)CheckAdj.o $(OBJDIR)spaceLib.o $(OBJDIR)HSlib.o
	$(CC) $^ -o $@ $(LIB)


$(OBJDIR)PosePlot.o: $(SRCDIR)PosePlot.c
	$(CC) -c $^ -o $@

$(OBJDIR)PosePlotS.o: $(SRCDIR)PosePlotS.c
	$(CC) -c $^ -o $@



$(OBJDIR)HSlib.o: $(SRCDIR)HSlib.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)MultiDist.o: $(SRCDIR)MultiDist.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)spaceLib.o: $(SRCDIR)spaceLib.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)LoadRobo.o: $(SRCDIR)LoadRobo.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)RoboCnt.o: $(SRCDIR)RoboCnt.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)RoboCntCP.o: $(SRCDIR)RoboCnt.c
	$(CC) -c $^ -o $@ -DCNVPRO
	
$(OBJDIR)RoboCntEx1000.o: $(SRCDIR)RoboCnt.c
	$(CC) -c $^ -o $@  -DT1000 -DROOPNUM=1000
	
$(OBJDIR)RoboCntEx100.o: $(SRCDIR)RoboCnt.c
	$(CC) -c $^ -o $@ -DCNVPRO -DT1000 -DROOPNUM=100
	
$(OBJDIR)RoboCntEx10.o: $(SRCDIR)RoboCnt.c
	$(CC) -c $^ -o $@ -DCNVPRO -DT1000 -DROOPNUM=10
	
$(OBJDIR)RoboCntEx5000.o: $(SRCDIR)RoboCnt.c
	$(CC) -c $^ -o $@ -DCNVPRO -DT1000 -DROOPNUM=5000
	
$(OBJDIR)RoboCntEx10000.o: $(SRCDIR)RoboCnt.c
	$(CC) -c $^ -o $@ -DCNVPRO -DT1000 -DROOPNUM=10000
	
$(OBJDIR)LoadPath.o: $(SRCDIR)LoadPath.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)ReLoadPath.o: $(SRCDIR)ReLoadPath.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)OutPutPath.o: $(SRCDIR)OutPutPath.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)OutPutTraceNext.o: $(SRCDIR)OutPutTraceNext.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)OutPutTaskPoints.o: $(SRCDIR)OutPutTaskPoints.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)OutPutTh.o: $(SRCDIR)OutPutTh.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)OutPutThFkine.o: $(SRCDIR)OutPutThFkine.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)SaveRobo.o: $(SRCDIR)SaveRobo.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)ReLoadRobo.o: $(SRCDIR)ReLoadRobo.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)Inherit.o: $(SRCDIR)Inherit.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)PMove.o: $(SRCDIR)PMove.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)F_Kine.o: $(SRCDIR)F_Kine.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)GetPositions.o: $(SRCDIR)GetPositions.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)OutputSt.o: $(SRCDIR)OutputSt.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)Chgstruct.o: $(SRCDIR)Chgstruct.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)OutLabel.o: $(SRCDIR)OutLabel.c
	$(CC) -c $^ -o $@

$(OBJDIR)PrintJposi.o: $(SRCDIR)PrintJposi.c
	$(CC) -c $^ -o $@

$(OBJDIR)CheckLink.o: $(SRCDIR)CheckLink.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)ChangeLink.o: $(SRCDIR)ChangeLink.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)CheckAll.o: $(SRCDIR)CheckAll.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)MakeAdj.o: $(SRCDIR)MakeAdj.c
	$(CC) -c $^ -o $@
	
$(OBJDIR)CheckAdj.o: $(SRCDIR)CheckAdj.c
	$(CC) -c $^ -o $@


cleanall:
	rm $(TARGET)
	rm  $(OBJDIR)*.o

clean:
	rm  $(OBJDIR)*.o
	
install:
	cp $(TARGET) /usr/local/bin/.

