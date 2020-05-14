// Josh Falejczyk: CS201 Data Structures Library Phase 3 Binomial Heap Class
#include <iostream>
using namespace std;

template <typename keytype, typename valuetype>
class BHeap {

    struct Node { // Struct for each node of binomial heap

        valuetype val;
        keytype key;
        Node* sibling;
        Node* child;
        Node* parent;
        int degree;
        int order;
        Node(keytype key, valuetype val) { // Constructor for Node

            this->key = key;
            this->val = val;
            sibling = nullptr;
            child = nullptr;
            parent = nullptr;
            degree = 0;
            order = 0;

        }

    };

    public:
        BHeap(); // Constructor
        BHeap(keytype k[], valuetype V[], int s); // Overloaded constructor
        BHeap(const BHeap &src); // Copy constructor
        BHeap& operator=(const BHeap &src); // Copy assignment operator
        ~BHeap(); // Destructor
        keytype peakKey(); // Returns smallest key without modifying binomial heap
        valuetype peakValue(); // Returns smallest key's value without modifying binomial heap
        keytype extractMin(); // Removes node with smallest key and returns its key
        void insert(keytype k, valuetype v); // Inserts node of key k and value v into binomial heap
        void merge(BHeap<keytype, valuetype> &H2); // Merges H2 into binomial heap
        void printKey(); // Print binomial heap in modified preorder traversal

    private:    
        Node* head;
        keytype error; // Empty error variable to return
        void helperDeepCopy(Node* &n, Node* nSrc); // Helps copy constructor/assignment operator perform deep copies
        void helperDestructor(Node* n); // Helps destructor recursively free memory
        Node* findMinNode(); // Finds the smallest node by key
        Node* findMinNodePrev(); // Finds the smallest node by key's previous node
        Node* flipLL(Node* n); // Flips a linked list configuration
        Node* unionHeap(Node* H1, Node* H2); // Merges two Binomial Heaps and consolidates trees of duplicate degree
        Node* mergeHelper(Node* n1, Node* n2); // Helper function that merges two Binomial Heaps into one of ascending degree order
        void link(Node* n1, Node* n2); // Helper function that sets parent between two nodes
        void printKeyHelper(Node* n); // Helps printKey in recursive areas
        
};

template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::BHeap() { // Constructor

    head = nullptr;

}

template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::BHeap(keytype k[], valuetype V[], int s) { // Overloaded constructor

    head = nullptr;
    for (int i = 0; i < s; i ++) { // Iteratively insert nodes into Binomial Heap

        insert(k[i], V[i]);

    }

    return;

}

template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::BHeap(const BHeap &src) { // Copy constructor

    error = src.error;
    helperDeepCopy(head, src.head);

}

template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>& BHeap<keytype, valuetype>::operator=(const BHeap<keytype, valuetype> &src) { // Copy assignment operator

    if (this != &src) { // Prevents self-assignment

        error = src.error;
        helperDeepCopy(head, src.head);

    }

    return *this;

}

template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::~BHeap() { // Destructor

    helperDestructor(head);

}

template <typename keytype, typename valuetype>
keytype BHeap<keytype, valuetype>::peakKey() { // Returns smallest key without modifying binomial heap

    Node* minNode = findMinNode();
    return minNode->key;

}

template <typename keytype, typename valuetype>
valuetype BHeap<keytype, valuetype>::peakValue() { // Returns smallest key's value without modifying binomial heap

    Node* minNode = findMinNode();
    return minNode->val;

}

template <typename keytype, typename valuetype>
keytype BHeap<keytype, valuetype>::extractMin() { // Removes node with smallest key and returns its key

    if (head == NULL) { // Binomial Heap is empty

        cout << "Error: Binomial Heap is empty" << endl;
        return error;

    }
    Node* minNodePrev = findMinNodePrev(); // Find node behind the minNode in root linked list
    Node* minNodeChild;
    Node* temp;
    keytype minKey;
    if (minNodePrev == NULL || head->sibling == NULL) { // MinNode is head

        minKey = head->key;
        if (head->child) {

            minNodeChild = head->child;
            minNodeChild = flipLL(minNodeChild); // Flip children to ascending order
            temp = minNodeChild;
            while (temp != NULL) { // Cut off potential pointers to parent node

                temp->parent = NULL;
                temp = temp->sibling;

            }
            if (head->sibling == NULL) { // Head is sole tree

                head = minNodeChild;
                return minKey;

            }
            head = head->sibling; // "Remove" minNode
            head = unionHeap(head, minNodeChild); // Merge root linked list with extracted node's children

        } else {

            head = head->sibling; // "Remove" minNode

        }

    } else {

        minKey = minNodePrev->sibling->key;
        if (minNodePrev->sibling->child) {

            minNodeChild = minNodePrev->sibling->child;
            minNodePrev->sibling = minNodePrev->sibling->sibling; // "Remove" minNode
            minNodeChild->parent = NULL;
            minNodeChild = flipLL(minNodeChild); // Flip children to ascending order
            temp = minNodeChild;
            while (temp != NULL) { // Cut off potential pointers to parent node

                temp->parent = NULL;
                temp = temp->sibling;

            }
            head = unionHeap(head, minNodeChild); // Merge root linked list with extracted node's children

        } else {

            minNodePrev->sibling = minNodePrev->sibling->sibling; // "Remove" minNode

        }

    }

    return minKey;

}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::insert(keytype k, valuetype v) { // Inserts node of key k and value v into binomial heap

    Node* n = new Node(k, v);
    head = unionHeap(head, n); // Merge new node into root linked list

    return;

}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::merge(BHeap<keytype, valuetype> &H2) { // Merges H2 into binomial heap

    head = unionHeap(head, H2.head);

    return;

}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::printKey() { // Print binomial heap in modified preorder traversal

    if (head == NULL) { // Binomial Heap is empty

        cout << "Error: Binomial Heap is empty" << endl;

        return;

    }
    printKeyHelper(head); // Recursively print nodes
    cout << endl;

    return;

}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::helperDeepCopy(Node* &n, Node* nSrc) { // Helps copy constructor/assignment operator perform deep copies

    if (nSrc == NULL) { // End-case for recursion

        n = NULL;
        return;

    } else { // Copy over data from nSrc to n

        n = new Node(nSrc->key, nSrc->val);
        n->sibling = nSrc->sibling;
        n->child = nSrc->child;
        n->parent = nSrc->parent;
        n->degree = nSrc->degree;
        n->order = nSrc->order;
        helperDeepCopy(n->child, nSrc->child); // Move onto potential children
        helperDeepCopy(n->sibling, nSrc->sibling); // Move onto potential siblings

    }

}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::helperDestructor(Node* n) { // Helps destructor recursively free memory

    if (n == NULL) { // End-case for recursion

        return;

    } else { // Freeing of memory

        helperDestructor(n->child); // Move onto potential children
        helperDestructor(n->sibling); // Move onto potential siblings
        n = nullptr;
        delete n;

    }

}

template <typename keytype, typename valuetype>
typename BHeap<keytype, valuetype>::Node* BHeap<keytype, valuetype>::findMinNode() { // Finds the smallest node by key

    Node* temp1 = head;
    Node* temp2 = head;
    keytype min = temp1->key; // Set base minimum key
    while (temp1 != NULL) {

        if (temp1->key < min) { // Comparison to find minNode

            temp2 = temp1;
            min = temp1->key;

        }
        temp1 = temp1->sibling;

    }
    return temp2;

}

template <typename keytype, typename valuetype>
typename BHeap<keytype, valuetype>::Node* BHeap<keytype, valuetype>::findMinNodePrev() { // Finds the smallest node by key's previous node

    Node* temp1 = head;
    Node* temp2 = NULL; // Set to null because minNode could be head
    keytype min = temp1->key; // Set base minimum key
    while (temp1 != NULL) {

        if (temp1->sibling && temp1->sibling->key < min) { // Comparison to find minNodePrev

            temp2 = temp1;
            min = temp1->sibling->key;

        }
        temp1 = temp1->sibling;

    }
    return temp2;

}

template <typename keytype, typename valuetype>
typename BHeap<keytype, valuetype>::Node* BHeap<keytype, valuetype>::flipLL(Node* n) { // Flips a linked list configuration

    Node* temp = n; // Current node
    Node* tempPrev = NULL;
    Node* tempNext = NULL;
    while (temp != NULL) { // Flip list iteratively

        tempNext = temp->sibling;  
        temp->sibling = tempPrev;  
        tempPrev = temp; 
        temp = tempNext;

    }
    n = tempPrev; // Set n to formerly last node
    
    return n;

}

template <typename keytype, typename valuetype>
typename BHeap<keytype, valuetype>::Node* BHeap<keytype, valuetype>::unionHeap(Node* h1, Node* h2) { // Merges two Binomial Heaps and consolidates trees of duplicate degree

    Node* temp = mergeHelper(h1, h2); // Merge both Binomial Heaps into one of ascending degree order
    if (temp == NULL) { // Checks for empty Binomial Heap

        return temp;

    }
    Node* temp1 = temp; // Copy of Binomial Heap
    Node* temp1Prev = nullptr;
    Node* temp1Next = temp1->sibling;
    while (temp1Next != NULL) { // Consolidation of potential trees of same degree

        if (temp1->degree != temp1Next->degree || (temp1Next->sibling != NULL && temp1Next->sibling->degree == temp1->degree)) { // Move forward

            temp1Prev = temp1;
            temp1 = temp1Next;

        } else {

            if (temp1->key <= temp1Next->key) { // Consolidate temp1 as parent

                temp1->sibling = temp1Next->sibling;
                link(temp1Next, temp1);

            } else { // Consolidate temp1Next as parent

                if (temp1Prev == NULL) {

                    temp = temp1Next;

                } else {

                    temp1Prev->sibling = temp1Next;

                }
                link(temp1, temp1Next);
                temp1 = temp1Next;

            }

        }
        temp1Next = temp1->sibling;

    }

    return temp; // Return temp once it's been modified where necessary

}

template <typename keytype, typename valuetype>
typename BHeap<keytype, valuetype>::Node* BHeap<keytype, valuetype>::mergeHelper(Node* n1, Node* n2) { // Helper function that merges two Binomial Heaps into one of ascending degree order

    Node* final; // Final linked list to return
    if (n1 == NULL) {

        return n2;

    } else if (n2 == NULL) {

        return n1;

    }
    if (n1->order <= n2->order) { // Add n1 to linked list and move forward

        final = n1;
        final->sibling = mergeHelper(n1->sibling, n2);

    } else { // Add n2 to linked list and move forward

        final = n2;
        final->sibling = mergeHelper(n1, n2->sibling);

    }

    return final;

}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::link(Node* n1, Node* n2) { // Helper function that creates necessary links for binomial trees

    n1->parent = n2; // Set n1 parent as n2, as it's already been decided in unionHeap
    n1->sibling = n2->child;
    n2->child = n1;
    if (n1->degree == n2->degree) { // Extra check to make sure order is only increased when trees of same degree are consolidated

        n2->order ++;

    }
    n2->degree ++; // Increase new parent's degree as it has a new child
    
    return;

}

template <typename keytype, typename valuetype> 
void BHeap<keytype, valuetype>::printKeyHelper(Node* n) { // Helps printKey in recursive areas

    while (n) {

        if (n->parent == NULL) { // Check for if n is a root node

            if (n != head) { // Check to allow correct formatting

                cout << endl << endl;

            }
            cout << "B" << n->order << endl; // Binomial Tree label

        }
        cout << n->key << " ";
        printKeyHelper(n->child); // Move onto potential children
        n = n->sibling; // Move onto potential siblings

    }

}