
string preferred = 1;         //preferred string (defaults to 1)
string unpreferred = 0;       //unpreferred string (defaults to 0)

class element {
    private:
        string type, name;
        string input[];
        string output;

    public:
        element(type, name);
        string setType(string type);
        string setName(string name);
};
