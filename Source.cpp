/* X00097724 Colm Fitzpatrick
--Usage			Modified: Line 54 - 64 , 92
--Date			26/10/2015
--Availability	http://www.dreamincode.net/forums/topic/59943-accessing-directories-in-cc-part-i/
*/

#include "dirent.h"
#include "md5.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <windows.h>
using namespace std;

int fileHandler(int argc, char *argv[]);
int md5Function(vector<string>& myFileNames);


void larrysFunction(string);
void findWordMatches(string file);
void findKeyWords(string file);

int main(int argc, char *argv[])
{
	fileHandler(argc, argv);	

	system("pause");
	return (0); // life is good 
}

//////////////////////////////////////////////////////////////////////////////
//	1.		//		Open a directory with 5 files inside for comparsion checks
//////////////////////////////////////////////////////////////////////////////
/* X00097724 Colm Fitzpatrick
--Usage			Modified:		How to convert a std::string to const char* or char*?
--Date			14:04 24/10/15	Used to get file path from console as Dirent take const char[]
--Availability	http://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char 
*/
int fileHandler(int argc, char *argv[])
{
	//string filePath1 = ("C:\\Users\\Colm\\Desktop\\test\\");
	//std::string filePath;//
	//cout << "Please enter a file path" << endl;
	//cin >> filePath;
	//const char * charFilePATH = filePath.c_str();// used to change file path string into const char for dirent to work	

	vector<string> myFileNames;//-- collect all valid file names
	DIR *pdir = NULL;//-- entry point of the program
	//pdir = opendir(charFilePATH);//-- add path to source Directory with 4 files inside  --  **	needs double backslashes  **
	pdir = opendir("C:\\Users\\Colm\\Desktop\\test\\");//-- add path to source Directory with 4 files inside  --  **	needs double backslashes  **

	struct dirent *pent = NULL;//-- Pent is a pointer to a struct in dirent.h that reads in file names from a directory until there are none left to read

	if (pdir == NULL)// if pdir wasn't initilised correctly( if directory not found pdir == null)
	{
		cout << "\nERROR! pdir could not be initilised correctly" << endl;
		system("pause");
		exit(1);
	}
//--	 while there is still something in the directory to list--> Get the file names && store them
	while (pent = readdir(pdir)) 
	{
		if (pent == NULL) // if pent has not been initialised correctly	
		{
			cout << "ERROR! pent could not be initialised correctly" << endl;// print an error message to let user know, and exit the program
			system("pause");
			exit(0);
		}// Else, it was initialised correctly. let's get the file names stored:

////////////////////////////////////////////
//	2.		//		(pent->d_name[0] != '.')
////////////////////////////////////////////
/* X00097724 Colm Fitzpatrick
--Usage			Based On:		if file names are != .(current directory) || != ..(previous directory) read in the file	
--Date			14:04 24/10/15	
--Availability	http://stackoverflow.com/questions/845556/how-to-ignore-hidden-files-with-opendir-and-readdir-in-c-library
*/
		else if (pent->d_name[0] != '.')
		{//-- Add the file names to the myfiles vector as they come through pent struct(dirent.h)							
			myFileNames.push_back(pent->d_name);
		}
		
	}
	md5Function( myFileNames);
	closedir(pdir);
	cin.get();
	return (0); // life is good 
}


int md5Function(vector<string>& myFileNames)
{
	ofstream outfile;
	ifstream infile;
	for (unsigned int i = 0; i < myFileNames.size(); ++i)
	{
		string line;
		infile.open("C:\\Users\\Colm\\Desktop\\test\\" + myFileNames[i],ios::app);//-- Inside college USB is E: drive  &&  need to have double backslashes for file paths	
		outfile.open("C:\\Users\\Colm\\Desktop\\Direnth\\Direnth\\" + myFileNames[i]);//-- opens file if not created yet it, if it is created it will append on to end of the file (,ios::app)

//////////////////////////////////////////////////////////////////////
//	3.		//		Write stream to a file for line by line comparsion
//////////////////////////////////////////////////////////////////////
		if (infile.is_open())
		{
			while (getline(infile, line))
			{
				outfile << line << '\n';
			}
		}
		else
		{
			cout << "Never opened the file line 106 .." << endl;
		}
		cout << myFileNames[i] << endl;

		infile.close();//-- Open and close the file each time it loops
		outfile.close();//-- No need to loop again
	}


////////////////////////////////////
//	4.		//		Md5 hash values
//////////////////////////////////////
/* X00097724 Colm Fitzpatrick
--Usage			Modified:		Put files into string array for md5 functions
--Date			14/10/2015 16:30
--Availability	http://stackoverflow.com/questions/2912520/read-file-contents-into-a-string-in-c
*/
		
		vector<string> myMd5Strings;//-- collect mds hash values
		vector<string> duplicateFilesFound;//-- collect 100% plargerised files

		for (unsigned int i = 0; i < myFileNames.size(); ++i)
		{
			ifstream f(myFileNames[i], fstream::in);
			string s;
			getline(f, s, '\0');
			myMd5Strings.push_back(md5(s));

			cout << "\n md5 of file No" << i + 1 << " " << md5(s) << endl;
			f.close();
		}


/////////////////////////////////////////////////////////
//	5.		//		Remove Duplicate elements from vector
//////////////////////////////////////////////////////////
/* X00097724 Colm Fitzpatrick	Find duplicate files from fileName list	
--Usage			Based On:		
--Date			24/10/2015 16:30
--Availability	http://stackoverflow.com/questions/33323406/find-delete-duplicate-vector-elements-in-a-for-loop?noredirect=1#comment54446405_33323406
*/
			
		for (unsigned int i = 0; i < myMd5Strings.size(); ++i) 
		{
			for (unsigned int j = 0; j < myMd5Strings.size(); ++j) 
			{
				if ((myMd5Strings[i] == myMd5Strings[j]) && (i != j)) 
				{
					duplicateFilesFound.push_back(myFileNames[i]);		
				}	
			}
		}

// --	it was collecting each fileNames twice so i worked around my loop problem 
		auto iter2 = std::unique(duplicateFilesFound.begin(), duplicateFilesFound.end());
		duplicateFilesFound.erase(iter2, duplicateFilesFound.end());

/////////////////////////////////////////////////////////////////
//	6.		//		Print out all file names still to be checked
/////////////////////////////////////////////////////////////////
		cout << "\n\nNumber of Files to be checked : "<< myFileNames.size()<< " \n" << endl;
		for (unsigned int k = 0; k < myFileNames.size(); ++k) {
			cout << myFileNames[k] << endl;
		}

/////////////////////////////////////////////////////////////////////////////
//	7.		//		Send duplicate files for further examination by examiner
//////////////////////////////////////////////////////////////////////////////
		CreateDirectory("C:\\Users\\Colm\\Desktop\\PlargirisedFiles\\", NULL);
			ofstream outfile2;
			fstream infile2;
			for (unsigned int i = 0; i < duplicateFilesFound.size(); ++i)
			{
				string line;
				infile2.open("C:\\Users\\Colm\\Desktop\\Direnth\\Direnth\\" + duplicateFilesFound[i],ios::in|ios::out);
				outfile2.open("C:\\Users\\Colm\\Desktop\\PlargirisedFiles\\" + duplicateFilesFound[i]);//-- opens file if not created yet it will append on to end of the file (,ios::app)																						//	1.--- $$$$$$$$$$	--->	Write stream to a file for line by line comparsion
				if (infile2.is_open())
				{
					while (infile2.good())
					{
						getline(infile2, line);
						outfile2 << line << '\n';
					}
				}
				else
				{
					cout << "Never opened the file .." << endl;
				}

				infile2.close();//-- Open and close the file each time it loops
				outfile2.close();//-- No need to loop again
			}
		
//////////////////////////////////////////////////////////////////////		
//	8.		//		Send all files to functions for comparsion checks
///////////////////////////////////////////////////////////////////////
		cout << "\nNumber of md5 matching files : " <<duplicateFilesFound.size() << "\n" << endl;
		for (unsigned int c = 0; c < duplicateFilesFound.size(); ++c) {
			cout << duplicateFilesFound[c] << endl;
		}
		for (unsigned int i = 0; i < myFileNames.size(); ++i) {
			larrysFunction(myFileNames[i]);
		}
		for (unsigned int i = 0; i < myFileNames.size(); ++i) {
			findWordMatches(myFileNames[i]);
		}
		for (unsigned int i = 0; i < myFileNames.size(); ++i) {
			findKeyWords(myFileNames[i]);
		}

//		verifylarrysFunction(duplicateFilesFound);
	return(0);
}


void larrysFunction(string file)
{

	string word,line;
	ifstream instream;

	vector<string> docWords;
	int commentCount = 0;

	instream.open(file);

	if (instream.fail()) {
		cout << "Failed to open input file"
			<< endl;
	}
	while (!instream.eof()) {
		//while word input
		while (instream >> word) {
			//loop through each word
			for (std::string::size_type ii = 0; ii < 2; ii++) {
				if ((word[ii] == '/') && (word[ii + 1] == '/')) {
					commentCount++;
					getline(instream, line);
					line = word + line;
					docWords.push_back(line);
				}
				//check if word starts wit '/*'
				else if ((word[ii] == '/') && (word[ii + 1] == '*')) {
					getline(instream, line, '/');
					commentCount++;
					line = word + line + "/\n";
					docWords.push_back(line);
				}
			}
		}
	}

	/*for (std::vector<string>::const_iterator i = docWords.begin(); i != docWords.end(); i++) {
	cout << *i << endl;
	}*/

	cout << "Comment count: " << commentCount << "\n" << endl;

	instream.close();
}


void findWordMatches(string file)
{

	string word, line;
	ifstream instream;

	int ifCount = 0, elseifCount = 0, elseCount = 0, switchCount = 0;
	int whileCount = 0, doCount = 0, forCount = 0;
	int iterationSt = 0, selectionSt = 0;


	vector<string> docWords;
	int commentCount = 0;

	instream.open(file);

	if (instream.fail()) {
		cout << "Failed to open input file"
			<< endl;
	}
	//Add all words from file to vector
	while (!instream.eof()) {
		while (instream >> word) {
			docWords.push_back(word);
		}
	}

	size_t last = docWords.size() - 1;
	for (size_t i = 0; i < docWords.size(); ++i) {
		if (docWords[i] == "while") {
			iterationSt++;
			whileCount++;
		}
		if (docWords[i] == "do") {
			iterationSt++;
			doCount++;
		}
		if (docWords[i] == "for") {
			iterationSt++;
			forCount++;
		}
		if (docWords[i] == "switch") {
			selectionSt++;
			switchCount++;
		}
		if (docWords[i] == "if") {
			if (docWords[i - 1] != "else") {
				selectionSt++;
				ifCount++;
			}
		}
		if (docWords[i] == "else") {
			if (docWords[i + 1] == "if") {
				selectionSt++;
				elseifCount++;
			}
			else {
				selectionSt++;
				elseCount++;
			}
		}
	}

	cout << "While: " << setw(8) << whileCount
		<< "\nDo while: " << setw(5) << doCount
		<< "\nfor: " << setw(10) << forCount
		<< "\nIteration Statements: " << iterationSt
		<< "\n\nswitch: " << setw(7) << switchCount
		<< "\nIf: " << setw(11) << ifCount
		<< "\nelse if: " << setw(6) << elseifCount
		<< "\nelse: " << setw(9) << elseCount
		<< "\nSelection Statements: " << selectionSt << "\n"
		<< endl;

	instream.close();
}
//============================================================================


void findKeyWords(string file)
{


	string word, line, nextWord;

	int intCount = 0, doubleCount = 0, stringCount = 0;
	vector<string> keyWords;

	int functionCount = 0;


	ifstream instream;

	instream.open(file);

	if (instream.fail()) {
		cout << "Failed to open input file"
			<< endl;
	}
	
	while (!instream.eof()) {
		//while word input
		while (instream >> word) {
			//loop through each word
			for (std::string::size_type ii = 0; ii < 2; ii++) {
				//Goes past method brackets so variable being passed
				//through functions are not counted twice

				if ((word[ii] == '(')) {
					getline(instream, line, ')');
					//	cout << "NOT INCLUDED: " << line << endl;
				}
				else {
					if (word == "int") {
						instream >> nextWord;
						if (nextWord.find('(') != std::string::npos) {
							if (nextWord.find(')') != std::string::npos) {
								//	cout << "Function: " << word << " " << nextWord << endl;
								functionCount++;
							}
							else {
								getline(instream, line, ')');
								line = word + " " + nextWord + line + ")";
								//	cout << "Function: " << line << endl;
								functionCount++;
							}
						}
						else {
							keyWords.push_back(word + " " + nextWord);
							intCount++;
						}
					}
					if (word == "double") {
						instream >> nextWord;
						keyWords.push_back(word + " " + nextWord);
						doubleCount++;
					}
					if (word == "string") {
						instream >> nextWord;
						keyWords.push_back(word + " " + nextWord);
						stringCount++;
					}
				}
			}
		}
	}

	cout << "\nInt:    " << intCount
		<< "\nDouble: " << doubleCount
		<< "\nString: " << stringCount
		<< "\nFunctions: " << functionCount
		<< endl;

	instream.close();
}
