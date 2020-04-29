
#include <vector>

string preferred = 1;         //preferred string (defaults to 1)
string unpreferred = 0;       //unpreferred string (defaults to 0)

class element {
    private:
        string type, name;
        vector<connection> connections;
        string value;   //for "switch" and "lamp" elements

    public:
        element(type, name);
        string setType(string type);
        string setName(string name);
        void addConnection(element *dest);
        string setValue(string newValue);
};

class connection {
    private:
        element *source;
        element *destination;
    public:
        void setSource(element *source);
        void setDestination(element *destination);

        element getSource();
        element getDestination();
};
