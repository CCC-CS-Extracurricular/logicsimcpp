#include "core.h"
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
    //Error out if no argument for a file to open
    if(argc < 2) {
        cerr << "ERR: No file name argument." << endl;
        return 1;
    }

    string filePath(argv[1]); //Uses a char* for the constructor

    //cout << "Argument count: " << (argc - 1) << endl << "Arguments: " << endl;
    //Iterate through argv
    /*for(size_t argNum = 1; argNum < (sizeof(argv) / sizeof(char*)); argNum++) {
        cout << "    " << argv[argNum] << endl;
    }*/

    //Check if file is open: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    ifstream circuitFile(filePath);
    if(circuitFile.bad()) { //Check if the file isn't openable for whatever reason
        cerr << "Unable to open file: " << filePath << endl;
        return 1;
    }
    circuitFile.close(); //Close the file. Remove and pass the file object, pass the path, or pass the file contents to syntax

    cout << "Opening file " << filePath << "..." << endl;
    
    //Call to syntax to analyze the file
    
    return 0;
}