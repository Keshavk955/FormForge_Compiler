#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include "ir.h"

using namespace std;

class CodeGenerator
{
public:
    bool generate(FormIR& ir);
};

#endif