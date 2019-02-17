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
    MOCK_METHOD4( drawLine, void(double const x0, double const y0, double const x1, double const y1) );

    MOCK_METHOD0(start, void());
    MOCK_METHOD0(finish, void());
};


#endif //LINDENMAYER_ATURTLE2DMOCK_HPP
