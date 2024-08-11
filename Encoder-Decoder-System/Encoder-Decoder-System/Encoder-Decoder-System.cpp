// Encoder-Decoder-System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Logger.h"
#include "Field.h";
int main()
{
    wd_codec::Logger::init("logfile.txt");


    std::cout << "Hello World!\n";
    wd_codec::Logger::log(wd_codec::INFO, "Program started.");
    wd_codec::galois::Field field(8, 9, wd_codec::galois::primitive_polynomial06);
    wd_codec::Logger::log(wd_codec::INFO, "Finish generate field.");


    wd_codec::Logger::close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
