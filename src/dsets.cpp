/* Your code here! */
#include <dsets.h>

/* Creates n unconnected root nodes at the end of the vector.
num	The number of nodes to create
*/
void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; i++) {
        // push at the end 'num' times
        _elems.push_back(-1);
    }
}


/* This function should compress paths and works as described in lecture.
returns the index of the root of the up-tree in which the parameter element resides.
*/
int DisjointSets::find(int elem) {
    // pseudocode
    if (_elems[elem] < 0) {
        return elem;
    } else {
        return find(_elems[elem]);
    }
}


/*This function should be implemented as union-by-size.

That is, when you setunion two disjoint sets, the smaller (in terms of number of nodes) should point at the larger. 
This function works as described in lecture, except that you should not assume that the arguments to setunion are roots of existing uptrees.

Your setunion function SHOULD find the roots of its arguments before combining the trees. 
If the two sets are the same size, make the tree containing the second argument point to the tree containing the first. 
*/
void DisjointSets::setunion(int a, int b) {

    // check if the roots aren't the same
    if (find(a) != find(b)) {
        int newSize = _elems[find(a)] + _elems[find(b)];

        // if _elems[a] is less than (more negative), it is the larger set;
        // union the smaller set, b, with a
        if (_elems[find(a)] < _elems[find(b)]) {
            // make sure roots are compared
            _elems[find(b)] = find(a);
            _elems[find(a)] = newSize;
        } 
        
        // otherwise do the opposite
        else {
            _elems[find(a)] = find(b);
            _elems[find(b)] = newSize;  
        }
    }
}


/*This function should return the number of nodes in the up-tree containing the element.
returns : number of nodes in the up-tree containing the element
*/
int DisjointSets::size(int elem) {

    // negative
    return -1 * _elems[find(elem)];
}
