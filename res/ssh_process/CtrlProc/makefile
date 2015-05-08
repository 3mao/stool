cc=gcc
objects=CtrlProc.o
includes= -I. \
          -I/home/lmg/ssh
libs= 
cpaths=./
exname=hntool
$(exname):$(objects)
	$(cc) -o $(exname)  $(objects) $(libs)
$(objects):%.o:%.c
	$(cc) -c  $< -o  $@ $(includes)
clean:
	rm $(objects) $(exname)
bak:
test:
	export LD_LIBRARY_PATH=../lib:$$LD_LIBRARY_PATH	; \
	./hntool host.txt 2> stderr.log
