#include "core.h"

using namespace std;

int main(int argc, char** argv) {
    //Argument parsing
    if(argc < 2) { //Error out if no argument for a file to open
        cerr << "ERR: No file name argument." << endl;
        return 1;
    }

    ifstream circuitFile(argv[1]); //Take the first command line argument as a string with the filename
    if(circuitFile.rdstate() != 0) { //Check if the file doesn't have a good status when opened (ios::rdstate() returns 0 when the file opens fine)
        cerr << "Unable to open file: " << argv[1] << endl;
        return 1;
    }
    circuitFile.close(); //Close the file, and the file object is destroyed upon closing

    cout << "Opening file " << argv[1] << "..." << endl;
    
    //Call to syntax passing the path to parse
    vector<element> parsedElements = syntaxRead(argv[1]); //Send file path (that is known to exist) to parse into elements

    cout << "Here are the elements rolling in..." << endl;
    for(size_t index = 0; index < parsedElements.size(); index++) { //Loop through elements from syntax
        cout << parsedElements[index].getName() << " [" << parsedElements[index].getType() << "] ";
        vector<connection> connsToIterate = parsedElements[index].getConnections(); //All the connections of the current element
        cout << "#" << connsToIterate.size() << " ";
        for(size_t connIndex = 0; connIndex < connsToIterate.size(); connIndex++) {  //Loop through connections
            cout << connsToIterate[connIndex].getSource()->getName() << " -> " << connsToIterate[connIndex].getDestination()->getName() << " ";
        }
        cout << endl;
    }

    //Have some way to change input. Maybe make a truth table and allow for editing of values? Adhere to Specification 2.1.8 with evaluating only on input changes.

    return 0;
}