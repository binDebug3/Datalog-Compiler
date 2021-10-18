#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

using namespace std;

int main(int argc, char** argv) {
    VS_MEM_CHECK
    // check command line arguments
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " input_file" << endl;
        return 1;
    }
    // open file
    ifstream in(argv[1]);
    stringstream ss;
    if (!in) {
        std::cout << "File could not be found or opened." << endl;
        return 1;
    }
    //read the file
    string line = "";
    string bigString = "";
    while (getline(in, line)) {
        istringstream iss(line);
        line = line + "\n";
        bigString += line;
    }
    //tokenize
    Lexer* lexer = new Lexer();
    lexer->Run(bigString);
    //check syntax and parse
    Parser* parser = new Parser();
    try {
        parser->parseDatalogProgram(lexer->sendTokens());
        //std::cout << parser->toString();
    }
    catch (std::string E) {
        cout << "Failure!" << std::endl << E;
    }
    //std::cout << std::endl << "Starting Project 3:" << std::endl;
    Interpreter* interpreter = new Interpreter(parser->getDatalog());

    std::cout << interpreter->toString();
    //end
    in.close();
    delete lexer;
    delete parser;
    delete interpreter;
    return 0;
}