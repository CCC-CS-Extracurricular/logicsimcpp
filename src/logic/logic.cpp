#include <iostream>
#include <string>
#include <vector>
#include "logic.h"

using namespace std;

string preferred = "1";         // preferred string (defaults to 1)
string unpreferred = "0";       // unpreferred string (defaults to 0)

// set element type
string element::setType(string type) {
    // set type only if the given type is valid
    if ((type == "not") || (type == "and") || (type == "or") ||
                (type == "switch") || (type == "lamp")) {
        this->type = type;
        return preferred;
    }
    return unpreferred;
}

// set element name
string element::setName(string name) {
    // set name only if the given name is valid
    if ((type != "not") || (type != "and") || (type != "or") ||
                (type != "switch") || (type != "lamp")) {
        this->name = name;
        return preferred;
    }
    return unpreferred;
}

string element::getType() { return this->type; }
string element::getName() { return this->name; }
vector<connection> element::getConnections() { return this->connections; }

// set a new value for an element
string element::setValue(string newValue) {

    // Check new value for validity before assigning it
    if((newValue == preferred) || (newValue == unpreferred)) {
        this->value = newValue;
        return preferred;
    }
    return unpreferred;
}

string element::getValue() { return this->value; }

element::element(string type, string name) {
    this->setType(type);
    this->setName(name);
}

// adds a connection to an element
void element::addConnection(element *src, element *dest) {
    connection newConnection;
    newConnection.setSource(src);
    newConnection.setDestination(dest);
    this->connections.push_back(newConnection);
}

// sets the source element of a connection
void connection::setSource(element *source) { this->source = source; }

// sets the destination element of a connection
void connection::setDestination(element *destination) { this->destination = destination; }

// returns the source element of a connection
element* connection::getSource() { return this->source; }

// returns the destination element of a connection
element* connection::getDestination() { return this->destination; }

string element::evaluate() {
    // NOT evaluate function
    if(this->type == "not") {
        string input;
        /* find our single input (aka destinations)
         * by comparing `this` to all of our destination connections*/
        for(unsigned int i; i < this->connections.size(); i++) {
            if(this->connections[i].getDestination() == this) {
                input = this->connections[i].getDestination()->evaluate();
            }
        }

        /* evaluate a NOT:
         * invert input
         * if input is preferred return unpreferred, and vice versa) */

        if (input == unpreferred) {
            return preferred;
        }
        return unpreferred;
    }

    // AND evaluate function
    if(this->type == "and") {
        vector<string> input;
        int x = 0;

        // find two destination/input connections
        for(unsigned int i; i < this->connections.size(); i++) {
            if(this->connections[i].getDestination() == this) {
                // FIXME: this needs error checking
                input[x] = connections[i].getDestination()->evaluate();
                x++;
            }
        }
        if ((input[0] == preferred) && (input[1] == preferred)) {
            return preferred;
        }
        return unpreferred;
    }

    // OR evaluate function
    if(this->type == "or") {
        vector<string> input;
        int x = 0;

        /* find our two inputs (aka destinations)
         * by comparing `this` to all of our destination connections*/
        for(unsigned int i; i < this->connections.size(); i++) {
            if(this->connections[i].getDestination() == this) {
                // FIXME: this needs error checking
                input[x] = connections[i].getDestination()->evaluate();
                x++;
            }
        }

        /* evaluate an OR:
         * if either input is preferred, return unpreferred
         * otherwise, return unpreferred */

        if ((input[0] == preferred) && (input[1] == preferred)) {
            return preferred;
        }

        if((input[0] == preferred) || (input[1] == preferred)) {
            return preferred;
        }

        if((input[0] == unpreferred) && (input[1] == unpreferred)) {
            return unpreferred;
        }
        return unpreferred;
    }

    /* SWITCH evaluate function
     * switches return their stored values when they are evaluated */
    if(this->type == "switch") {
        return value;
    }

    /* LAMP evaluate function
     * lamps "light up" if their input is "preferred" */
    if(this->type == "lamp") {
        string input;
        /* find our single input (aka destinations)
         * by comparing `this` to all of our destination connections*/
        for(unsigned int i; i < this->connections.size(); i++){
            if(this->connections[i].getDestination() == this) {
                input = this->connections[i].getDestination()->evaluate();
            }
        }

        /* Set our value to preferred or unpreferred depending by input */
        if(input == preferred) {
            this->value = preferred;
        }
        this->value = unpreferred;
    }
    return unpreferred;
}
