//

#include "ClientEntry.hpp"

#include <algorithm>
#include <stdexcept>

#include "utility/strutil.hpp"

using namespace std::string_literals ;

//======================================================================
// ==================================================================================================================
ClientEntry::ClientEntry():clientport(0),serverport(50000),useAudio(false),useLight(false){
    
}

// ==================================================================================================================
ClientEntry::ClientEntry(std::istream &input) : ClientEntry() {
    auto buffer = std::vector<char>(1024,0) ;
    while(input.good() && !input.eof()) {
        input.getline(buffer.data(), buffer.size()-1) ;
        if (input.gcount()>0){
            buffer[input.gcount()] = 0 ;
            std::string line = buffer.data() ;
            line = util::trim(util::strip(line,"#")) ;
            if (!line.empty()) {
                auto [key,value] = util::split(line,"=") ;
                auto ukey = util::upper(key) ;
                if (ukey == "NAME") {
                    this->name = value ;
                }
                else if (ukey == "CLIENTPORT") {
                    clientport = std::stoi(value) ;
                }
                else if (ukey == "SERVER") {
                    auto [ip,port] = util::split(value,",") ;
                    serverport = std::stoi(port) ;
                    server_ip = ip ;
                }
                else if (ukey == "MUSICPATH") {
                    musicpath = value ;
                }
                else if (ukey == "MUSICEXTENSION") {
                    musicextension = value ;
                }
                else if (ukey == "LIGHTPATH") {
                    lightpath = value ;
                }
                else if (ukey == "LIGHTEXTENSION") {
                    lightextension = value ;
                }
                else if (ukey == "RUNSPAN") {
                    runRange = util::MonthRange(value) ;
                }
                else if (ukey == "CONNECTHOURS") {
                    connectRange = util::HourRange(value) ;
                }
                else if (ukey == "AUDIO") {
                    useAudio = (std::stoi(value) != 0)  ;
                }
                else if (ukey == "LIGHTS") {
                    useLight = (std::stoi(value) != 0)  ;
                }
                else if (ukey == "AUDIODEVICE") {
                    audoDevice = std::stoi(value) ;
                }
                else if (ukey == "PRU") {
                    pru.push_back(value) ;
                }
           }
        }
    }
}
// ==================================================================================================================
auto ClientEntry::write(std::ostream &output) const -> void  {
    
    output << "# ************************************************************************************" << std::endl;
    output << "#" << std::endl;
    output << "# Configuration file for Show Client" << std::endl;
    output << "#" << std::endl;
    output << "# ************************************************************************************" << std::endl;
    output << "" << std::endl;
    
    output << "# Name of the client" << std::endl;
    output << "name = " << this->name << std::endl;
    output <<  std::endl;
    
    output << "#  The port the client should bind to locally" << std::endl;
    output << "clientport = " << this->clientport << std::endl;
    output <<  std::endl;
    
    output << "#  The server ip and port" << std::endl;
    output << "server = " << this->server_ip << " , "<< this->serverport << std::endl;
    output <<  std::endl;
    
    output << "#  Directory settings" << std::endl;
    output << "musicpath = " << this->musicpath << std::endl;
    output << "lightpath = " << this->lightpath << std::endl;
    output <<  std::endl;
    
    output << "#  File extensions" << std::endl;
    output << "musicextension = " << this->musicextension << std::endl;
    output << "lightextension = " << this->lightextension << std::endl;
    output <<  std::endl;

    output << "# The month/day span the Beagle should be running" << std::endl;
    output << "# Note this is inclusive of the the dates (start and end)" << std::endl;
    output << "" << std::endl;
    output << "# NOTE: the format for a Month/day is: month#:day#" << std::endl;
    output << "# Where month is 1 - 12 and day is 1 - 31" << std::endl;
    output << "runspan = " << this->runRange.describe() << std::endl;
    output <<  std::endl;

    output << "# The hours the client should look for the server" << std::endl;
    output << "# the span is inclusives of the start time, and not inclusive of the end time" << std::endl;
    output << "" << std::endl;
    output << "connecthours = " << this->connectRange.describe() << std::endl;
    output <<  std::endl;

    output << "# use audio  (0/1)" << std::endl;
    output << "audio = " << (this->useAudio ? "1" : "0") << std::endl;
    output <<  std::endl;

    output << "#what is the audio device to use? 0 is default" << std::endl;
    output << "audiodevice = " << this->audoDevice << std::endl;
    output <<  std::endl;

    output << "# use lights (0/1)" << std::endl;
    output << "lights = " << (this->useLight ? "1" : "0") << std::endl;
    output <<  std::endl;


    output << "#" << std::endl;
    output << "# Pru settings" << std::endl;
    output << "# pru = #(0,1), mode (0=SSD,1=DMX,2=WS2812), offset (offset to place in the OUTPUT stream), length (how much of the data to grab)" << std::endl;
    for (const auto &value: pru) {
        output << "pru = " << value << std::endl;
    }

}
