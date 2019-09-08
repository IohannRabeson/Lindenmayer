#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Unit tests " << APPLICATION_VERSION << "\n";
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
