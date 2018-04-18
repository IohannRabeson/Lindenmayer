//
// Created by Io on 17/04/2018.
//

#include "ProgramError.hpp"
#include <vector>
#include <iostream>

std::vector<lcode::Program::Error> const& printErrors(std::vector<lcode::Program::Error> const& errors)
{
    for (auto const& error : errors)
    {
        std::cout << " - Error: " << error.message << "\n";
    }
    return errors;
}