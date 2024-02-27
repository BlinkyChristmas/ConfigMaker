// ClientConfigMaker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>



#include "utility/strutil.hpp"
#include "ClientEntry.hpp"

using namespace std::string_literals;


// =============================================================================
int main(int argc, const char* argv[]) {

    auto currentdirectory = std::filesystem::current_path() ;
    auto exitcode = EXIT_SUCCESS;
    try {
        // Check to ensure we got at least one argument, our build file
        if (argc < 3) {
            throw std::runtime_error("Insufficient arugments: clientcfgtemplate handle numbertomake");
        }
        auto input = std::ifstream(argv[1]) ;
        if (!input.is_open()) {
            throw std::runtime_error("Unable to open: "s + std::string(argv[1]));
        }
        auto cliententry = ClientEntry(input);
        input.close() ;
        auto basename = std::string(argv[2]) ;
        auto amount = std::stoi(argv[3]) ;
        
        for (auto i = 0 ; i < amount ; i++) {
            auto handle = util::format("%s-%02i",basename.c_str(),i) ;
#if defined(_WIN32)
            auto shellname = handle + ".bat"s ;
#else
            auto shellname = handle + ".sh"s ;
#endif
            auto cfgname = handle + ".cfg"s ;
            auto output = std::ofstream(cfgname) ;
            if (!output.is_open()) {
                throw std::runtime_error("Unable to create: "s + cfgname ) ;
            }
            cliententry.name = handle ;
            cliententry.write(output) ;
            output.close() ;
            
            // Now we need to write the shell script
            output = std::ofstream(shellname) ;
            if (!output.is_open()) {
                throw std::runtime_error("Unable to create: "s + shellname) ;
            }
#if defined(_WIN32)
            output << "@Echo off" << std::endl;
            output << "TITLE %~nx0" << std::endl;
            output << ".\\ShowClient.exe " << cfgname << std::endl;
#else
            output << "#!/bin/tcsh" << std::endl;
            output << "cd " << currentdirectory.string() << std::endl;
            output << "./ShowClient " << cfgname  << std::endl;
#endif
            output.close() ;
        }
        
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exitcode = EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "Uknown error!" << std::endl;
        exitcode = EXIT_FAILURE;
    }
    return exitcode;
 }
