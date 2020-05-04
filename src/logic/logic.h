#include <vector>
#include <string>

using namespace std;

class connection;

class element {
    private:
        string type, name;
        vector<connection> connections;
        string value;   //for "switch" and "lamp" elements

    public:
        element(string type, string name);
        string setType(string type);
        string setName(string name);
        void addConnection(element *dest);
        string setValue(string newValue);
        string getType();
        string getName();
        vector<connection> getConnections();
        string getValue();
        string evaluate();
};

class connection {
    private:
        element *source;
        element *destination;
    public:
        void setSource(element *source);
        void setDestination(element *destination);

        element* getSource();
        element* getDestination();
};

//Flawed
/* class nott : public element {
    private:

    public:
        string evaluate();
};

class andd : public element {
    private:

    public:
        string evaluate();
};*/
