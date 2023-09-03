//
// Created by Thijs on 02/09/2023.
//

#include <iostream>
#include <string>
#include <fstream>
#include "TokenHandler.h"

int main() {

    std::ifstream ifs("../test.cf");
    std::string fileContent((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));

    TokenHandler tokenHandler;

    tokenHandler.tokenize(fileContent);

    return 0;
}
