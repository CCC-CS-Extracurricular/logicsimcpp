#include "syntax.h"

#define __DEBUG 1	//Uncomment to see debug output on the command line

using namespace std;

vector<element> syntaxRead(string filePath, vector<connection> connectionStorage) {
	#ifdef __DEBUG
	cout << "##########\n# PARSER #\n##########" << endl;
	#endif

	//Variables
	//char* match = (char*)malloc(sizeof(char) * 100);	//Match data from scanf
	char reqFlags = 0b00000110;	//Create a char to store bits to check that file meets requirements
		/*
		 * Bit 0: All inputs are connected to an output (Specification 2.1.7)
		 * Bit 1: Pass lexical analysis (set to false if failure occurs) (Specification 2.1.13)
		 * Bit 2: Pass grammatical analysis (set to false if failure occurs) (Specification 2.1.13)
		 * Bit 3: Grammatical analysis check if element exists or not
		 * Bit 4-7: Ignored
		 */
	ifstream circuitFile(filePath);	//File object for the file to parse which is initialized to the path passed to the function
	queue<string> lineTokens;	//Tokens found by lexical analysis
	//queue<string> tokens;	//Tokens that are not predefined and are created by the user
	string line;	//Line to process
	string match;
	vector<string> types;	//Tokens that the system has base support for
	vector<element> elements;	//Elements created in the process of parsing the file

	//Add values to defaultTokens vector
	types.push_back("and");
	types.push_back("or");
	types.push_back("not");
	types.push_back("switch");
	types.push_back("lamp");

	//Syntax analysis
	while(!circuitFile.eof()) {	//Run if the file pointer isn't at the end of the file
		getline(circuitFile, line);

		#ifdef __DEBUG
		cout << "\n\nCurrent line being parsed:\n\t" << line << endl;
		#endif

		//Initial passing over
		if(line.length() == 0) {	//If the line is empty, skip over line
			#ifdef __DEBUG
			cout << "\tLine length of zero.\n\tSkipping..." << endl;
			#endif
			continue;
		} else if(line.substr(0, 2) == "//") {	//If the line starts with a comment, skip over line
			#ifdef __DEBUG
			cout << "\tCommented line.\n\tSkipping..." << endl;
			#endif
			continue;
		}

		//Lexical analysis
		for(size_t index = 0; index < line.length(); index++) {	//Loop through line
			#ifdef __DEBUG
			cout << "\tMatching character (" << index << "/" << line.length() <<"): " << line[index];
			printf(":%x\n", line[index]);
			#endif
			if((line[index] > 0x40 && line[index] < 0x5b) ||	//Check if character is in between A and Z (ascii values)
			   (line[index] > 0x60 && line[index] < 0x7b) ||	//Check if character is in between a and z (ascii values)
			   (line[index] > 0x2f && line[index] < 0x3a) ||	//Check if character is in between 0 and 9 (ascii values) (out of specification, but used as an example)
			   (line[index] == 0x2d) || (line[index] == 0x5f)) {	//Check if character is a dash or underscore
				#ifdef __DEBUG
				cout << "\t\tCharacter match!" << endl;
				#endif
				match.push_back(line[index]);	//Add character to match string
				if(index == line.length() - 1) {	//Push string if last character
					#ifdef __DEBUG
					cout << "\tFound a token: " << match << endl;
					#endif
					lineTokens.push(match);
					match.erase(0, string::npos);
				}
			} else if(line[index] == 0x20) {	//Check if the match is a space or newline
				#ifdef __DEBUG
				cout << "\t\tWhitespace match!" << endl;
				#endif
				if(match.length() > 0) {	//If a match actually existed, add to the list of tokens to parse, and restart the match string
					#ifdef __DEBUG
					cout << "\tFound a token: " << match << endl;
					#endif
					lineTokens.push(match);
					match.erase(0, string::npos);
				}
			} else {	//If there wasn't a match
				cerr << "ERR: Failed to parse characters." << endl;
				reqFlags = reqFlags & 0b11111101;	//Set the lexical analysis flag to false (bit masking)
				break;
			}
		}

		/* According to the specification, all lines would have tokens in multiples of two (Specification 2.1.13).
		 * A declaration would have elementType, elementName (2 tokens).
		 * A definition would have elementName, signalName, elementName, signalName (4 tokens).
		 * Therefore, we can perform a size check on the queue to make sure that the queue is 2 or 4, and execute accordingly.
		 */
		//Grammatical analysis
		if(lineTokens.size() == 2) {	//Check for declaration
			/* BUG TO FIX:
			 *	(Too lazy to do it now...) What happens if the element exists already? Throw an error, obviously. It's more important that it works to begin with, so bugtesting can come later...
			 */
			#ifdef __DEBUG
			cout << "\tChecking declaration..." << endl;
			#endif
			string elementType = lineTokens.front();	//Grab and remove first element
			for(size_t index = 0; index < types.size(); index++) {
				if(elementType == types[index]) {	//If the elementName is one of the builtin types, process the data
					lineTokens.pop();	//Force name of element to front
					element newedElement(elementType, lineTokens.front());	//Create a new element with the type and name (now front of tokens list) passed to the constructor
					elements.push_back(newedElement);	//Add new element to list
					lineTokens.pop();	//Empty token list for next line
					#ifdef __DEBUG
					cout << "\tSize after emptying: " << lineTokens.size() << endl;
					#endif
					break;
				}
			}
		} else if(lineTokens.size() == 4) {	//Check for definition
			//Implement check for definitions to apply to 2.1.9 and 2.1.10. Switches cannot be destinations, and lamps cannot be source
			#ifdef __DEBUG
			cout << "\tChecking declaration..." << endl;
			#endif
			for(size_t _i = 0; _i < 2; _i++) { //Loop twice
				string sourceElementStr = lineTokens.front();	//Obtain the first token
				string destElementStr;
				element sourceElement("", "");
				element* destElement;
				for(size_t index = 0; index < elements.size(); index++) {	//Loop through elements
					if(sourceElementStr == elements[index].getName()) {
						reqFlags = reqFlags ^ 0b00001000;	//Set the proper bit to true
						sourceElement = elements[index];	//Set the elements to be the same
						break;
					}
				}
				if(reqFlags & 0b00001000) { //Check if bit was set
					lineTokens.pop();	//Shift elements forward
					string destElementStr = lineTokens.front();	//Get next element
					reqFlags = reqFlags ^ 0b11110111;	//Reset bit
				} else {
					break;
				}
				for(size_t index = 0; index < elements.size(); index++) {	//Loop through elements
					if(destElementStr == elements[index].getName()) {
						reqFlags = reqFlags ^ 0b00001000;	//Set the proper bit to true
						destElement = &elements[index];	//Get address of the element pointer
						break;
					}
				}
				if(reqFlags & 0b00001000) { //Check if bit was set
					sourceElement.addConnection(destElement);	//Apply the connection to the element
				} else {
					break;
				}
			}
		}

		//Flag check
		if(!(reqFlags & 0b00001110)) {	//Check if the flags are not set as expected
			cerr << "ERR: Unable to parse file." << endl;
			elements.clear();	//Remove all parsed elements
			break;	//Quit loop over lines
		}

	}
	
	return elements;
}

/*
int FatRead (string Filepath) {
//Tim
//Filepath from core.h
	string Token;
	int Location[2];
	string strParse; //declare vars
	ifstream ActualFile; //open filepath form core.h
	ActualFile.open(Filepath);
	
	if (ActualFile.is_open()) { //make sure it actually opens the file
		while (!ActualFile.eof()) {
			getline(ActualFile, strParse); //reads line by line the file, sends to strParse to hold like a good little guppy
			if ( strParse.length() == 0 ) { //empty skip
				continue;
			}
			if (strParse[0]= "/"){ //if comment out then skip
					continue;
			}
			//parse the line by spaces
			//THING NOTTHING THING NOTTHING
			
			Location[0]=0
			
			Location[1]=strParse.find(" ")
		
			Token[0]=strParse.substr(Location[0], Location[1])
			
			if ( !(strParse.find(" ",Location[1]))==NULL){
				Token[1]=strParse.find(Location[1])	
			}
			
			Token[0]=strParse.substr(Location[0], Location[1])
			
			//check for declaration of a var
			if (Token[1] == ArrayNameOfAllOurTypesAndShitThatIDontHaveANameFor[Iter]){
				element Declaration(Token[0], Token[1]);
			}
		}
	}

	ActualFile.close(); 

	return 0;

	//core.cpp syntax.cpp logic.cpp
}
*/