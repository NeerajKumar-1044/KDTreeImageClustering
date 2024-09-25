RunProject: KdTree.o project.o
	g++ KdTree.o project.o -o RunProject

KdTree.o: KdTree.cpp
	g++ -std=c++20 -c KdTree.cpp

project.o: project.cpp
	g++ -std=c++20 -c project.cpp

clean:
	rm *.o