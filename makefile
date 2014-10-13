all: main
#	g++ -O2 -std=c++11 -I /common/lib/boost-1.47/include/ lattice.cpp diamond.cpp pyrochlore.cpp test.cpp -o main

main: lattice.o test.o pyrochlore.o diamond.o
	g++ -O2 -std=c++11 -I /common/lib/boost-1.47/include/  lattice.o pyrochlore.o test.o diamond.o -o main

lattice.o: lattice.cpp lattice.h
	g++ -c -O2 -std=c++11 -I /common/lib/boost-1.47/include/ lattice.cpp

test.o: test.cpp pyrochlore.h diamond.h lattice.h
	g++ -c -O2 -std=c++11 -I /common/lib/boost-1.47/include/ test.cpp 

pyrochlore.o: pyrochlore.cpp pyrochlore.h diamond.h lattice.h point3d.h
	g++ -c -O2 -std=c++11 -I /common/lib/boost-1.47/include/ pyrochlore.cpp

diamond.o: diamond.cpp diamond.h lattice.h point3d.h
	g++ -c -O2 -std=c++11 -I /common/lib/boost-1.47/include/ diamond.cpp

#clean:
#	\rm *.o *~ 
