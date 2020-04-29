#include <iostream>
#include "logic.h"

//set element type
string element::setType(string type)
{
    //set type only if the given type is valid
    if ((type == "not") || (type == "and") || (type == "or") ||
                (type == "switch") || (type == "lamp")) {
        this->type = type;
        return preferred;
    }
    return unpreferred;
}

//set element name
string element::setName(string name)
{
    //set name only if the given name is valid
    if ((type != "not") || (type != "and") || (type != "or") ||
                (type != "switch") || (type != "lamp")) {
        this->name = name;
        return preferred;
    }
    return unpreferred;
}

element::element(string type, string name)
{
    this->setType(type);
    this->setName(name);
}


/* Broken
string element::sim()
{
    string a = this->a;
    string b = this->b;

    //Not
    if (this->type == "not") {

        if (a == preferred) {
            return unpreferred;
        } else {
            return preferred;
        }
    }

    //Or
    if (this->type == "or") {
        if ((a == preferred) && (b == preferred)) {
            return preferred;
        }
        if ((a == preferred) || (b == preferred)) {
            return preferred;
        }
        if ((a == unpreferred) && (b == unpreferred)){
            return unpreferred;
        }
    }

    //And
    if(this->type == "and"){
        if ((a == preferred) && (b == preferred)) {
            return preferred;
        }
        if ((a == unpreferred) || (b == unpreferred)) {
            return unpreferred;
        }
        if ((a == unpreferred) && (b == unpreferred)) {
            return unpreferred;
        }
    }

    return "undefined";
}*/
