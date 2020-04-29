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

string element::setValue(string newValue)
{
    this->value = newValue;
}

element::element(string type, string name)
{
    this->setType(type);
    this->setName(name);
}

element setSource(element *source)
{
    this->source = source;
}

element setDestination(element *source)
{
    this->destination = destination;
}

element connection::getSource()
{
    return this->source;
}

element connection::getDestination()
{
    return this->destination;
}
