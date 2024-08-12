// Encoder-Decoder-System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Logger.h";
#include "Field.h";
#include "Polynomial.h";
int main()
{
    wd_codec::Logger::init("logfile.txt");


    std::cout << "Hello World!\n";
    wd_codec::Logger::log(wd_codec::INFO, "Program started.");
    const wd_codec::galois::Field field(8, 9, wd_codec::galois::primitive_polynomial06);
    wd_codec::galois::Polynomial generator_polynomial(field);
    wd_codec::Logger::close();
}

