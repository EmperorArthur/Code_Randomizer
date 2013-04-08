/*
Copyright (c) 2012, Arthur Moore
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

//Can't forget this
using namespace std;

//See if a string matches, and has nothing but whitespace before it.
bool FindAtBegining(string toSearch, string toSearchFor){
	bool onlyWhitespaces = true;
	//Find the string toSearchFor with only whitespace before it
	size_t stringPosition = toSearch.find(toSearchFor);
	if(string::npos != stringPosition){
		//Remove all whitespace
		string checkString = toSearch.substr(0,stringPosition);
		for (int i=0; i< checkString.length();i++){
			if (' ' != checkString[i] && '	' != checkString[i]){
				onlyWhitespaces = false;
				break;
			}
		}
	} else{
		onlyWhitespaces = false;
	}
	return onlyWhitespaces;
}

//Returns a string with all begining whitespace removed
string ChompWhitespace(string inString){
	size_t numberOfWhitespaceChars = 0;
	for (int i=0; i< inString.length();i++){
		if (' ' != inString[i] && '	' != inString[i]){
			numberOfWhitespaceChars = i;
			break;
		}
	}
	return inString.substr(numberOfWhitespaceChars,inString.length());
}

int main(int argc, char *argv[]){

	//Test code
	string helpString("-t");
	if (2 == argc && 0 == helpString.compare(argv[1])){
		cout <<"Begin Test 1"<<endl;
		#define RANDOMCODE
			cout <<"test1"<<endl;
			cout <<"test2"<<endl;
			cout <<"test3"<<endl;
		#define ENDRANDOMCODE
		cout <<"End test 1"<<endl;
		cout << endl;
		cout <<"Begin Test 2"<<endl;
		#define BETWEENRANDOMS cout << "This is between all random lines" << endl;
		#define RANDOMCODE
			cout <<"a"<<endl;
			cout <<"b"<<endl;
			cout <<"c"<<endl;
			cout <<"d"<<endl;
			cout <<"e"<<endl;
			cout <<"f"<<endl;
		#define ENDRANDOMCODE
		cout <<"End test 2"<<endl;
		cout << endl;
		cout << "Begin Test 3"<<endl;
		#define BETWEENRANDOMS
		#define RANDOMCODE
			cout <<"z"<<endl;
			cout <<"x"<<endl;
			cout <<"c"<<endl;
			cout <<"v"<<endl;
			cout <<"b"<<endl;
			cout <<"n"<<endl;
		#define ENDRANDOMCODE
		cout << "End Test 3" << endl;
	
	//Print help
	} else	if(3 != argc){
		cout << "Usage:  " << argv[0] << " infile outfile" << endl;
		cout << endl;
		cout << "This program randomizes the code between \"#define RANDOMCODE\" and \"#define ENDRANDOMCODE\" in a C/C++ header file" << endl;
		cout << "This is a very simple program." << endl;
		cout << "It merely randomizes based on line number.  There are few bells and whisles." << endl;
		cout << endl;
		cout << "To test this code run:  " << argv[0] << " -t"<<endl;
	}
	
	//Actuall code
	ifstream inFile (argv[1]);
	ofstream outFile (argv[2]);
	string line;											//The current working line
	string beginRandom ("#define RANDOMCODE");				//The tag we look for to start a random code section
	string endRandom ("#define ENDRANDOMCODE");				//The tag we look for to end a random code section
	string betweenRandomsTag ("#define BETWEENRANDOMS");	//The tag we look for that defines what goes between random lines
	string betweenRandoms;									//The string that goes between each random line
	vector <string> toRandomize;
	
	
	if(inFile.is_open() && outFile.is_open()){
		while(inFile.good() && outFile.good()){
			getline(inFile,line);
			//If we've found betweenRandomsTag
			if(FindAtBegining(line, betweenRandomsTag)){
				//Set betweenRandoms to what's after the tag (excluding any whitespace)
				size_t endofbetweenRandomsTag = line.find(betweenRandomsTag)+betweenRandomsTag.length();
				betweenRandoms = ChompWhitespace(line.substr(endofbetweenRandomsTag,line.length()));
			}
			//If we've found beginRandom
			if(FindAtBegining(line, beginRandom)){
				//Go ahead, and output beginRandom
				outFile << line << endl;
				//Start filling a vector with everything to randomize
				while(inFile.good() && outFile.good() && !FindAtBegining(line, endRandom)){
					getline(inFile,line);
					toRandomize.push_back(line);
				}
				//Make sure the files are still good
				if(inFile.good() && outFile.good()){
					//Get rid of the endRandom line in the vector
					toRandomize.pop_back();
					//Initalize the RNG
					srand ( unsigned ( time (NULL) ) );
					//Randomize the vector
					random_shuffle(toRandomize.begin(),toRandomize.end());
					//Output the random vector
					while(!toRandomize.empty()){
						//Put a betweenRandoms line in
						if(betweenRandoms.length() != 0){
							outFile << betweenRandoms << endl;
						}
						outFile << toRandomize.back() << endl;
						toRandomize.pop_back();
					}
					//Put the endRandom line back
					outFile << line << endl;
				//If the file's aren't good, complain
				} else {
					cerr << "Warning, File Error." << endl;
					cerr << "Make sure that \""<<endRandom<< "\" follows \""<<beginRandom <<"\""<<endl;
					return 1;
				}
			}else{
				outFile << line << endl;
			}
		}
	}
	return 0;
}

