/*
 * main.cpp
 *
 *  Created on: 2015-07-09
 *      Author: luc
 */

#include <iostream>
#include <string>

#include "args.h"

struct flags {
        std::string login = "l";
}fl;

void printHelp() {

    std::cout << "Help...";

    std::exit(0);
}
int main(int argc, char* argv[]) {


    try {

        Args args("l,m,p#,d*,t##,h", argc, argv);

        bool help = args.getBoolean("h");
        if(help)
            printHelp();

        bool test = args.getBoolean(fl.login);
        bool tm = args.getBoolean("m");
        std::string str = args.getString("d");
        int num = args.getInteger("p");
        double time = args.getDouble("t");

        std::cout << "log?: " << test << std::endl;
        std::cout << "tm?: " << tm << std::endl;
        std::cout << "addr?: " << str << std::endl;
        std::cout << "port?: " << num << std::endl;
        std::cout << "time?: " << time << std::endl;

    } catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
