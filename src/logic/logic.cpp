#include <iostream>
#include "logic.h"

string preferred = "1";         //preferred string (defaults to 1)
string unpreferred = "0";       //unpreferred string (defaults to 0)

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

string element::setValue(string newValue)
{
    this->value = newValue;
    return this->value;
}

element::element(string type, string name)
{
    this->setType(type);
    this->setName(name);
}

// sets the source element of a connection
void connection::setSource(element *source)
{
    this->source = source;
}

// sets the destination element of a connection
void connection::setDestination(element *source)
{
    this->destination = destination;
}

// returns the source element of a connection
element* connection::getSource()
{
    return this->source;
}

// returns the destination element of a connection
element* connection::getDestination()
{
    return this->destination;
}

string element::evaluate()
{
    //evaluate a NOT
    if(this->type == "not"){
        string input;
        /*find our two inputs (aka destinations)\
            by comparing `this` to all of our destination connections*/
        for(unsigned int i; i < this->connections.size(); i++){
            if(this->connections[i].getDestination() == this){
                input = this->connections[i].getDestination()->evaluate();
            }
        }

        /* evaluate a NOT:
            invert input
            (if input is preferred return unpreferred, and vice versa) */

        if (input == preferred) {
            return unpreferred;
        } else if (input == unpreferred) {
            return preferred;
        }
        return "invalid";
    }

    //evaluate an AND
    if(this->type == "and") {
        vector<string> input;
        int x = 0;

        //find two destination/input connections
        for(unsigned int i; i < this->connections.size(); i++){
            if(this->connections[i].getDestination() == this) {
                //FIXME: this needs error checking
                input[x] = connections[i].getDestination()->evaluate();
                x++;
            }
        }
        if ((input[0] == preferred) && (input[1] == preferred)) {
            return preferred;
        } else {
            return unpreferred;
        }
        return unpreferred;
    }

    //evaluate an OR
    if(this->type == "or") {
        vector<string> input;
        int x = 0;

        /*find our two inputs (aka destinations)\
            by comparing `this` to all of our destination connections*/
        for(unsigned int i; i < this->connections.size(); i++){
            if(this->connections[i].getDestination() == this) {
                //FIXME: this needs error checking
                input[x] = connections[i].getDestination()->evaluate();
                x++;
            }
        }

        /* evaluate an OR:
        if either input is preferred, return unpreferred
        otherwise, return unpreferred */

        if ((input[0] == preferred) && (input[1] == preferred)) {
            return preferred;
        } else if((input[0] == preferred) || (input[1] == preferred)){
            return preferred;
        } else if((input[0] == unpreferred) && (input[1] == unpreferred)){
            return unpreferred;
        }
        return unpreferred;
    }

    return unpreferred;
}
