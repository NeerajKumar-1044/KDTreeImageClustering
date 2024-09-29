run command-

    for mac-
        make
        make clean
        ./RunProject
    
    for windows-
        g++ -std=c++20 -c KdTree.cpp project.cpp -o RunProject
        g++ KdTree.o project.o -o RunProject
        ./RunProject
