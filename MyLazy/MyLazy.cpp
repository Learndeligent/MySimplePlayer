#include "MyLazy.h"

#include <iostream>

void Judge_and_Print(int ret, std::string str)
{
    if(ret) {
        std::cout << str << std::endl;
    }
}