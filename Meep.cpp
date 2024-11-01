//
// Created by Dalton Pierce on 10/30/24.
//

#include <iostream>

#include "Meep.h"


class Meep {
public:
    const unsigned char *name;

public:
    unsigned int age;

    Meep():
        name(nullptr), age(0) {
        std::cout << "Meep Born\n";
    }

    ~Meep() {
        std::cout << "Meep Destroyed\n";
    }
};
