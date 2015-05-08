cc=gcc
objects=main.o ssh.o
includes= -I. \
	  -I ../include
libs=-lssh2 -L ../lib  
cpaths=./
ssh:$(objects)
	$(cc) -o ssh  $(objects) $(libs)
$(objects):%.o:%.c
	$(cc) -c  $< -o  $@ $(includes)
clean:
	rm $(objects) ssh
bak:
	tar -cf ssh_$(shell date '+%Y%m%d%H%M%S').tar  $(objects) $(patsubst %.o,%.c,$(objects)) $(patsubst %.o,%.h,$(objects)) ssh  makefile
test:
	export LD_LIBRARY_PATH=../lib:$$LD_LIBRARY_PATH ;\
	./ssh  192.168.0.101  lmg lmglmg   2>err.log  ../res/cmd.sh
