#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "AVL.h"

const int NUM_FILES = 5; // the total number of files to be read from

const std::string fileArray[NUM_FILES] = { "file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt" }; // the string array containing the file names

// This will take a string temp and an AVL object and will execute an instruction from the string
// no return, but writes the results of the instruction into the ofs
void parse_instruction(std::string temp, std::ofstream &ofs, AVL* aptr);

// This function is a platform independent way of reading files of various line ending types.
// It's definiton is at the bottom of the file, don't worry if you don't understand it.
namespace ta {
	std::istream& getline(std::istream& is, std::string& line);
}


int main() {

	std::ifstream ifs; // create the stream to read in from the files
	std::ofstream ofs; // create the output stream to write to an output file
	std::string temp; // used to store the current instruction
	AVL* avlptr = NULL;//the AVL

	for (int i = 0; i < NUM_FILES; i++) {
		ifs.open(fileArray[i]); // open the file to read from
		ofs.open("out_" + fileArray[i]); // open the file to write to
		avlptr = new AVL();

		if (!ifs.is_open()) { // if the file did not open, there was no such file
			std::cout << "File " << i + 1 << " could not open, please check your lab setup" << std::endl;
		}
		else {
			std::cout << "Reading file" << i + 1 << ".txt..." << std::endl;
		}

		std::cout << "Beginning out_file" << i + 1 << ".txt write" << std::endl;
		while (ta::getline(ifs, temp)) { // while there are more instructions to get,
			parse_instruction(temp, ofs, avlptr); // parse the instructions using the AVL
		}
		std::cout << "File write complete" << std::endl << std::endl;
		if (avlptr != NULL) {
			delete avlptr;
			avlptr = NULL;
		}
		ifs.close();
		ofs.close();
	}
	std::cout << "end" << std::endl; // indicate that the program has successfuly executed all instructions
	return 0;
}


//a function that takes an AVL and returns a level-order string representation of the AVL
//returns a string representation of the nodes in level order
string BSTtoString(AVL* bst);

void parse_instruction(std::string temp, std::ofstream &ofs, AVL* aptr) {
	std::string command, expression;
	std::stringstream ss(temp);

	if (!(ss >> command)) { return; } // get command, but if string was empty, return
	if (command == "Add") { // add the given value to the AVL if possible
		int valToAdd;
		ss >> valToAdd;
		if (aptr->add(valToAdd)) {
			ofs << temp << " True" << std::endl;
		}
		else {
			ofs << temp << " False" << std::endl;
		}
	}
	else if (command == "Remove") { // remove the given value from the AVL
		int valToRemove;
		ss >> valToRemove;
		if (aptr->remove(valToRemove)) {
			ofs << temp << " True" << std::endl;
		}
		else {
			ofs << temp << " False" << std::endl;
		}
	}
	else if (command == "Clear") { // clear the AVL
		aptr->clear();
		ofs << temp << std::endl;
	}
	else if (command == "PrintBST") { //you don't need to implement any function for this command
		ofs << temp << "\n" << BSTtoString(aptr) << std::endl;
	}
	else { // invalid command, wrong input file format
		std::cout << "Command: \"" << command << "\"" << std::endl;
		std::cout << "Invalid command.  Do you have the correct input file?" << std::endl;
	}
}


//a function that takes a BST and returns a level-order string representation of the BST
//returns a string representation of the nodes in level order
string BSTtoString(AVL* bst) {
	queue<NodeInterface*> readQ; // used to read in the levels of the tree, contains Node*
	stringstream nodeReader_ss; // used to store the values of the nodes and the level-order sequence
	int depth = 0; // the depth of a node on the tree

	if (bst->getRootNode() == NULL) {
		return "BST is empty\n";
	}

	readQ.push(bst->getRootNode()); // push the root node of the tree into the queue

	while (!readQ.empty()) { // as long as the queue has a remaining node:
		int i = readQ.size(); // store the number of nodes on this level of the tree
		nodeReader_ss << depth << ":  ";
		for (; i > 0; i--) { // for each node on this level,
			NodeInterface* nextNode = readQ.front(); // store the next node in the queue
			nodeReader_ss << nextNode->getData() << " "; // store the data from the node into the ss
			if (nextNode->getLeftChild() != NULL) { // if there is a left child, push the left child into the queue
				readQ.push(nextNode->getLeftChild());
			}
			if (nextNode->getRightChild() != NULL) { // if there is a right child, push the left child into the queue
				readQ.push(nextNode->getRightChild());
			}
			readQ.pop(); // pop the node off of the queue, leaving its children in the queue
		}
		nodeReader_ss << "\n"; // push an endl into the ss to distinguish levels
		depth++;
	}

	return nodeReader_ss.str();
}





// Version of getline which does not preserve end of line characters
namespace ta {
	std::istream& getline(std::istream& in, std::string& line) {
		line.clear();

		std::istream::sentry guard(in, true); // Use a sentry to maintain the state of the stream
		std::streambuf *buffer = in.rdbuf();  // Use the stream's internal buffer directly to read characters
		int c = 0;

		while (true) { // Continue to loop until a line break if found (or end of file)
			c = buffer->sbumpc(); // Read one character
			switch (c) {
			case '\n': // Unix style, return stream for further parsing
				return in;
			case '\r': // Dos style, check for the following '\n' and advance buffer if needed
				if (buffer->sgetc() == '\n') { buffer->sbumpc(); }
				return in;
			case EOF:  // End of File, make sure that the stream gets flagged as empty
				in.setstate(std::ios::eofbit);
				return in;
			default:   // Non-linebreak character, go ahead and append the character to the line
				line.append(1, (char)c);
			}
		}
	}
}
