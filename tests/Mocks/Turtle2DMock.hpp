//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_ATURTLE2DMOCK_HPP
#define LINDENMAYER_ATURTLE2DMOCK_HPP
#include <gmock/gmock.h>
#include <ATurtle2D.hpp>

class Turtle2DMock : public lcode::ATurtle2D
{
public:
    MOCK_METHOD1( drawLine, void(QLineF const&) );
};


#endif //LINDENMAYER_ATURTLE2DMOCK_HPP
