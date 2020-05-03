#include <iostream>
#include <fstream>
#include "syntax.h"
using namespace std;

global vector<element> Token;


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
					
					getline(ActualFile, strParse) //reads line by line the file, sends to strParse to hold like a good little guppy
						if ( strParse.length() == 0 ) { //empty skip
							continue;
						};
						if (strParse[0]= "/"){ //if comment out then skip
							 continue;
						};
						//parse the line by spaces
							//THING NOTTHING THING NOTTHING
							
							Location[0]=0
							
							Location[1]=strParse.find(" ")
						
							Token[0]=strParse.substr(Location[0], Location[1])
							
							
							if ( !(strParse.find(" ",Location[1]))==NULL){
								Token[1]=strParse.find(Location[1])	
							};
							//wombo
							
							Token[0]=strParse.substr(Location[0], Location[1])
							
							
							//check for declaration of a var
	
								if (Token[1] == ArrayNameOfAllOurTypesAndShitThatIDontHaveANameFor[Iter]){
									element Declaration(Token[0], Token[1]);
								};
							
							}
				};	
			};
	ActualFile.close; 
 
return 0;

 //core.cpp syntax.cpp logic.cpp

}