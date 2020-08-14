// main.cpp
#include <UGM/UGM.h>
#include <iostream>
using namespace Ubpa;
int main(){
    pointf3 p{0, 1, 2};
    pointf3 q{2, 1, 0};
    std::cout << p - q << std::endl;
}
