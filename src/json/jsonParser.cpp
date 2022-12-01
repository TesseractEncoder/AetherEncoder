#include "../AetherEncoder.h"
#include "jsonParser.h"


using namespace std;
 
 int AetherAction::parseJson(string fpath) 
 {
    ofstream newJson;
    struct input in;
    struct output out;
    //opening file using fstream
    ifstream json(fpath);
 
    // check if there is any error is getting data from the json file
    if (!reader.parse(json, root)) {
        cout << reader.getFormattedErrorMessages();
        exit(1);
    }
       
    in.active_type = root["Input"][0]["active_type"].asString();
    in.framerate = root["Input"][0]["framerate"].asFloat();
    in.res.height = root["Input"][0]["resolution"]["height"].asInt();
    in.res.width = root["Input"][0]["resolution"]["weight"].asInt();
    in.type.ip.active = root["Input"][0]["type"]["IP"]["active"].asString();
    in.type.ip.input_type = root["Input"][0]["type"]["IP"]["input_type"].asString();
    in.type.ip.ipAddr = root["Input"][0]["type"]["IP"]["ipAddr"].asString();
    in.type.ip.port = root["Input"][0]["type"]["IP"]["port"].asInt();
    in.type.ip.sys_IP = root["Input"][0]["type"]["IP"]["input_type"].asString();

    out.active_type = root["Input"][0]["active_type"].asString();
    out.bitrate = root["Input"][0]["bitrate"].asInt();
    out.encode = root["Input"][0]["encode"].asString();
    out.framerate = root["Input"][0]["framerate"].asFloat();
    out.res.height = root["Input"][0]["resolution"]["height"].asInt();
    out.res.width = root["Input"][0]["resolution"]["weight"].asInt();
    out.type.ip.active = root["Input"][0]["type"]["IP"]["active"].asString();
    out.type.ip.input_type = root["Input"][0]["type"]["IP"]["input_type"].asString();
    out.type.ip.ipAddr = root["Input"][0]["type"]["IP"]["ipAddr"].asString();
    out.type.ip.port = root["Input"][0]["type"]["IP"]["port"].asInt();
    out.type.ip.sys_IP = root["Input"][0]["type"]["IP"]["input_type"].asString();

    cout << out.active_type << endl << out.framerate << endl << out.res.height << endl << out.type.ip.active << endl;
 
    //make the changes in a new file/original file
    //newJson.open(fpath);
    //writer.write(newJson, root);
    //newJson.close();
    return 0;
}
