cc=gcc
objects=sshtool.o
includes= -I. \
          -I../include
libs    = -L../lib 
cpaths  =./
all:
	for i in $(objects) ;\
        do    \
                 $(cc) -c -g $$(echo $$i|sed 's/.o$$/.c/') $(includes) -o  $$i ;\
        done ;
	$(cc) -o ssh  $(objects) $(libs)
clean:
	rm $(objects) ssh
bak:
	tim=$$(date "+%Y%m%d%H%M%S"); \
        tar -cf ../../ssh_fd_$${tim}.tar  ../../ssh_fd
test:
	export LD_LIBRARY_PATH=../lib:$$LD_LIBRARY_PATH ;\
        ./ssh host.list
