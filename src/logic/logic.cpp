#include <iostream>
#include "logic.h"

// Not function, takes a string "a" and inverts it and returns the result
string Not(string a)
{
    if (a == preferred) {
        return unpreferred;
    } else {
        return preferred;
    }

    return "undefined";
}

// Basic OR function
string Or(string a, string b)
{
    if ((a == preferred) && (b == preferred)) {
        return preferred;
    }
    if ((a == preferred) || (b == preferred)) {
        return preferred;
    }
    if ((a == unpreferred) && (b == unpreferred)){
        return unpreferred;
    }
    return "undefined";
}

// Basic AND function
string And(string a, string b) {
    if ((a == preferred) && (b == preferred)) {
        return preferred;
    }
    if ((a == unpreferred) || (b == unpreferred)) {
        return unpreferred;
    }
    if ((a == unpreferred) && (b == unpreferred)) {
        return unpreferred;
    }
    return "undefined";
}
