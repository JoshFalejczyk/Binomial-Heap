# Binomial-Heap

This was a project from Fall 2019 for my CS201 class.  It is a C++ implementation of a Binomial Heap, following all Binomial Heap principles and a variety of analysis functions can be found in BHeap.cpp.  The Binomial Heap data is represented by Nodes that hold both key and data values.  This project in particular was easily my most challenging in this class, however I am proud of the perfect results I achieved with it when tested.

Functions/Usage: 
1. Constructor (constructor for BHeap object, plus an optional overloaded constructor for preloaded data)
2. Insert (inserts a Node into the BHeap)
3. Extract Min (removes and returns the minimum key in the BHeap)
4. Merge (merges a separate BHeap object into current BHeap object)
5. Peak Key (returns the minimum key in the BHeap without modifying the BHeap structure)
6. Peak Value (returns the data value of the Node with the minimum key in the BHeap without modifying the BHeap structure) 
7. Print Key (outputs the keys associated with the BHeap in a modified preorder traversal from smallest to largest subtree)
8. Copy Constructor/Copy Assignment Operator (included for good practice of dynamic memory allocation)
9. Destructor (properly frees manually allocated memory within BHeap)

To Run: 
1. Compile using "make" or "g++ -Wall 201MainPhase3BHeap.cpp".
2. Run using "./phase3" for the former compilation command, or "./a.exe" (or "./a.out" if on Mac) for the latter.

Credit: 
1. The concept for the project and 201MainPhase3BHeap.cpp test runner file were written and supplied by my professor, Dr. Brandon Dixon.
2. The BHeap.cpp file and the Makefile were written by myself.
