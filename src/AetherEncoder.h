#ifndef AETHER_ENCODER_INCLUDE
#define AETHER_ENCODER_INCLUDE

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <json/value.h>
#include <json/json.h>

class AetherAction{
    public:
        Json::Reader reader;  //for reading the data
        Json::Value root; //for modifying and storing new values
        Json::StyledStreamWriter writer; //for writing in json files
        ofstream newJson;
    public:
        int parseJson(string path);
        int updateJson(string path);
        int showJsonContain(string path);
        string showJsonValueStr(string key, string value);
        int showJsonValueInt(string key, int value);
};

#endif
