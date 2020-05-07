#include "syntax.h"

//#define __DEBUG 1	//Uncomment to see debug output on the command line

using namespace std;

vector<element> syntaxRead(string filePath) {
	#ifdef __DEBUG
	cout << "##########\n# PARSER #\n##########" << endl;
	#endif

	//Variables
	char reqFlags = 0b00000110;	//Create a char to store bits to check that file meets requirements
		/*
		 * Bit 0: All inputs are connected to an output (Specification 2.1.7)
		 * Bit 1: Pass lexical analysis (set to false if failure occurs) (Specification 2.1.13)
		 * Bit 2: Pass grammatical analysis (set to false if failure occurs) (Specification 2.1.13)
		 * Bit 3: Grammatical analysis check if element exists or not
		 * Bit 4: Token exists as a builtin type
		 * Bit 5-7: Ignored
		 */
	ifstream circuitFile(filePath);	//File object for the file to parse which is initialized to the path passed to the function
	vector<element> elements;	//Elements created in the process of parsing the file
	string line;	//Line to process
	string match;
	vector<string> types;	//Tokens that the system has base support for

	//Add values to defaultTokens vector
	types.push_back("and");
	types.push_back("or");
	types.push_back("not");
	types.push_back("switch");
	types.push_back("lamp");

//gcc has four phases: preprocessing, compiling, assembly, and linking. You can specifify to 

	//Syntax analysis
	while(!circuitFile.eof()) {	//Run if the file pointer isn't at the end of the file
		getline(circuitFile, line);
		queue<string> lineTokens;	//Tokens found by lexical analysis

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
			cout << "\tMatching character (" << index << "/" << line.length() - 1 <<"): " << line[index];
			printf(":%x\n", line[index]);
			#endif
			/*
			if((line[index] > 0x40 && line[index] < 0x5b) ||	//Check if character is in between A and Z (ascii values)
			   (line[index] > 0x60 && line[index] < 0x7b) ||	//Check if character is in between a and z (ascii values)
			   (line[index] > 0x2f && line[index] < 0x3a) ||	//Check if character is in between 0 and 9 (ascii values) (out of specification, but used as an example)
			   (line[index] == 0x2d) || (line[index] == 0x5f)) {	//Check if character is a dash or underscore
			*/
			if((line[index] >= 'A' && line[index] <= 'Z') ||	//Check if character is in between A and Z (ascii values)
			   (line[index] >= 'a' && line[index] <= 'z') ||	//Check if character is in between a and z (ascii values)
			   (line[index] >= '0' && line[index] <= '9') ||	//Check if character is in between 0 and 9 (ascii values) (out of specification, but used as an example)
			   (line[index] == '-') || (line[index] == '_')) {	//Check if character is a dash or underscore
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
		if(!(reqFlags & 0b00000010)) {
			cerr << "ERR: Failed lexical analysis." << endl;
			break;
		}

		#ifdef __DEBUG
		cout << endl;
		#endif
		
		/* According to the specification, all lines would have tokens in multiples of two (Specification 2.1.13).
		 * A declaration would have elementType, elementName (2 tokens).
		 * A definition would have elementName, signalName, elementName, signalName (4 tokens).
		 * Therefore, we can perform a size check on the queue to make sure that the queue is 2 or 4, and execute accordingly.
		 */
		/* BUG TO FIX:
		 *	(Too lazy to do it now...) What happens if the element exists already? Throw an error, obviously. It's more important that it works to begin with, so bugtesting can come later...
		 */
		//Grammatical analysis
		if(lineTokens.size() % 2 == 0) {	//If there are 2*n elements and passed lexical analysis
			#ifdef __DEBUG
			cout << "Beginning grammatical analysis..." << endl;
			#endif
			string initToken = lineTokens.front();
			
			//Initial token sweep
			for(size_t index = 0; index < types.size(); index++) {
				#ifdef __DEBUG
				cout << "\tChecking " << initToken << " against " << types[index] << " for a builtin type..." << endl;
				#endif
				if(initToken == types[index]) {	//Check if the token is a builtin type
					reqFlags = reqFlags ^ 0b00010000;	//Set token exists flag
					break;
				}
			}

			if(reqFlags & 0b00010000) {	//If the token is a builtin type
				#ifdef __DEBUG
				cout << "Builtin type." << endl;
				#endif
				lineTokens.pop();	//Force name of element to front
				element newElement(initToken, lineTokens.front());	//Create a new element with the type and name (now front of tokens list) passed to the constructor
				elements.push_back(newElement);	//Add new element to list
				lineTokens.pop();	//Empty token list for next line
				if(!lineTokens.size()) {	//If there are no more element in the tokens on that line
					reqFlags = reqFlags ^ 0b00000100;	//Sets pass for grammatical analysis
				} else {
					cerr << "ERR: Declarations only take two tokens." << endl;
					break;
				}
			} else {	//If the initial token wasn't found to be a builtin type
				for(size_t index = 0; index < elements.size(); index++) {
					#ifdef __DEBUG
					cout << "\tChecking " << initToken << " against " << elements[index].getName() << " for an existing element..." << endl;
					#endif
					if(initToken == elements[index].getName()) {	//Check if the token exists as another element
						reqFlags = reqFlags ^ 0b00010000;	//Set token exists flag
						break;
					}
				}
				if(reqFlags & 0b00010000) {	//If the token is an element name
					reqFlags = reqFlags ^ 0b00010000;	//Reset token exists flag
					while(lineTokens.size()) {	//Loop until there are no tokens left to parse
						//Variables
						string source = lineTokens.front();	//Obtain the first token (repetative, but needed for initial loop)
						string dest;
						element* sourceElement;
						element* destElement;

						//Check if source is an element
						for(size_t index = 0; index < elements.size(); index++) {	//Loop through elements
							if(source == elements[index].getName()) {	//If the first element is an element
								reqFlags = reqFlags ^ 0b00001000;	//Set the element exists flag
								sourceElement = &elements[index];	//Set the source address to the current element's address
								break;
							}
						}
						if(reqFlags & 0b00001000) {	//Check if element existing flag is set
							lineTokens.pop();	//Shift tokens
							dest = lineTokens.front();	//Set destination to next token
							reqFlags = reqFlags ^ 0b00001000;	//Reset element exist flag
						} else {
							cerr << "ERR: Could not find source element." << endl;
							break;
						}

						//Check if destination is an element
						for(size_t index = 0; index < elements.size(); index++) {	//Loop through elements
							if(dest == elements[index].getName()) {	//Check if destination exists as an element
								reqFlags = reqFlags ^ 0b00001000;	//Set element exist flag
								destElement = &elements[index];	//Set the destination adress to the current element's address
								break;
							}
						}
						if(reqFlags & 0b00001000) {
							#ifdef __DEBUG
							cout << "Adding connection from " << sourceElement->getName() << " to " << destElement->getName() << endl;
							#endif
							sourceElement->addConnection(sourceElement, destElement);	//Apply the connection to the source
							destElement->addConnection(sourceElement, destElement);	//Apply the connection to the destination

							lineTokens.pop();	//Shift tokens
							reqFlags = reqFlags ^ 0b00001000;	//Reset element exist flag
						} else {
							cerr << "ERR: Could not find destination element." << endl;
							break;
						}
					}
				} else {
					cerr << "ERR: Invalid start of line token." << endl;
					break;
				}
			}
		} else {
			cerr << "ERR: Cannot parse tokens." << endl;
			break;
		}

		//Flag check
		if(!(reqFlags & 0b00001110)) {	//Check if the flags are not set as expected
			cerr << "ERR: Unable to parse file." << endl;
			elements.clear();	//Remove all parsed elements
			break;	//Quit loop over lines
		}

		reqFlags = reqFlags & 0b11101111;	//Make sure token exists flag is reset before looping again by masking out the bit
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