//

#ifndef ClientEntry_hpp
#define ClientEntry_hpp

#include <cstdint>
#include <iostream>
#include <string>
#include <istream>
#include <ostream>
#include <vector>

#include "utility/timeutil.hpp"
//======================================================================
struct ClientEntry {
    std::string name ;
    int clientport ;
    std::string server_ip ;
    int serverport ;
    std::string musicpath ;
    std::string lightpath ;
    std::string musicextension ;
    std::string lightextension ;
    util::MonthRange runRange;
    util::HourRange connectRange ;
    bool useAudio ;
    int audoDevice ;
    bool useLight ;
    std::vector<std::string> pru ;
    
    ClientEntry() ;
    ClientEntry(std::istream &input)  ;
    auto write(std::ostream &output) const -> void  ;
};

#endif /* ClientEntry_hpp */
