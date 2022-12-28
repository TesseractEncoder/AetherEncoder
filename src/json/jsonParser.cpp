#include "../AetherEncoder.h"
#include "jsonParser.h"
#include <unistd.h>


using namespace std;
 
  struct jsonParameter AetherAction::parseJson(string fpath,  struct jsonParameter *JP) 
 {
    //ofstream newJson;
   // struct input in;
   // struct output out;
   // struct jsonParameter jsonParam;
    cout << fpath << endl;
    //opening file using fstream
    ifstream json(fpath);
    if(!json.is_open())
    {
        cout<<"File doesn't exits" << endl;
        exit(0);
    }
 
    // check if there is any error is getting data from the json file
    if (!reader.parse(json, root)) {
        cout << reader.getFormattedErrorMessages();
        exit(1);
    }
    
    JP[0].in.active_type = root["Input"][0]["active_type"].asString();
    JP[0].in.framerate = root["Input"][0]["framerate"].asFloat();
    JP[0].in.res.height = root["Input"][0]["resolution"]["height"].asInt();
    JP[0].in.res.width = root["Input"][0]["resolution"]["width"].asInt();
    JP[0].in.type.ip.active = root["Input"][0]["type"]["IP"]["active"].asString();
    JP[0].in.type.ip.input_type = root["Input"][0]["type"]["IP"]["input_type"].asString();
    JP[0].in.type.ip.ipAddr = root["Input"][0]["type"]["IP"]["ipAddr"].asString();
    JP[0].in.type.ip.port = root["Input"][0]["type"]["IP"]["port"].asInt();
    JP[0].in.type.ip.sys_IP = root["Input"][0]["type"]["IP"]["input_type"].asString();

    JP[0].out.active_type = root["Input"][0]["active_type"].asString();
    JP[0].out.bitrate = root["Input"][0]["bitrate"].asInt();
    JP[0].out.encode = root["Input"][0]["encode"].asString();
    JP[0].out.framerate = root["Input"][0]["framerate"].asFloat();
    JP[0].out.res.height = root["Input"][0]["resolution"]["height"].asInt();
    JP[0].out.res.width = root["Input"][0]["resolution"]["weight"].asInt();
    JP[0].out.type.ip.active = root["Input"][0]["type"]["IP"]["active"].asString();
    JP[0].out.type.ip.input_type = root["Input"][0]["type"]["IP"]["input_type"].asString();
    JP[0].out.type.ip.ipAddr = root["Input"][0]["type"]["IP"]["ipAddr"].asString();
    JP[0].out.type.ip.port = root["Input"][0]["type"]["IP"]["port"].asInt();
    JP[0].out.type.ip.sys_IP = root["Input"][0]["type"]["IP"]["input_type"].asString();

    //cout << out.active_type << endl << out.framerate << endl << out.res.height << endl << out.type.ip.active << endl;
    //cout << JP[0].in.res.width << endl;
    //make the changes in a new file/original file
    //newJson.open(fpath);
    //writer.write(newJson, root);
    //newJson.close();
    return JP[0];
}
bool AetherAction::fileExist(string filePath)
{
   return ( access( filePath.c_str(), F_OK ) != -1 ); 
}

struct jsonParameter AetherAction::parseYtbJson(string fpath, jsonParameter *JP, int* ch)
{
    struct jsonParameter ytbParam;
    //opening file using fstream
    ifstream json(fpath);
    int nch;

    if(!json.is_open())
        cout<<"cmd file doesnit exits"<<endl;
    // check if there is any error is getting data from the json file
    if (!reader.parse(json, root)) {
        cout << reader.getFormattedErrorMessages();
        exit(1);
    }
    *ch = root["youtubeNO"].asInt();
    nch = *ch;
    JP[0].ytb[nch].active = root["active"].asString();
    JP[0].ytb[nch].youtubeNO = root["youtubeNO"].asInt();
    JP[0].ytb[nch].streamto = root["streamto"].asString();
    JP[0].ytb[nch].key = root["key"].asString();
    JP[0].ytb[nch].link = root["link"].asString();

    cout << JP[0].ytb[nch].active << endl;
    cout << JP[0].ytb[nch].youtubeNO << endl;
    cout << JP[0].ytb[nch].streamto << endl;
    cout << JP[0].ytb[nch].key << endl;
    cout << JP[0].ytb[nch].link << endl;

    return JP[0];
}