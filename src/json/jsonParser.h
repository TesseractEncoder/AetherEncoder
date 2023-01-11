#ifndef JSONPARSER_INCLUDE
#define JSONPARSER_INCLUDE

using namespace std;

struct resolution{
    int height;
    int width;
};

struct IP{
    string ipAddr;
    int port;
    string active;
    string input_type;
    string sys_IP;
};

struct SDI{
     string input_type;
     string active;
     int sdi_num;
     string dev_name;
};

struct TYPE{
    struct IP ip;
    struct SDI sdi;
};

struct input{
    string active_type;
    struct resolution res;
    float framerate; 
    struct TYPE type; 
};

struct output{
    string active_type;
    int bitrate;
    string encode;
    float framerate;
    struct resolution res;
    struct TYPE type; 
};

struct youtube{
    string streamto;		
	string active;
	string key ;
	string link;
    int youtubeNO;
};

struct jsonParameter{
    struct input in;
    struct output out;
    struct youtube ytb[10]; 
};

class AetherAction{
    public:
        Json::Reader reader;  //for reading the data
        Json::Value root; //for modifying and storing new values
        Json::StyledStreamWriter writer; //for writing in json files
        struct jsonParameter JP[10];
       // bool status;
        
        
    public:
        jsonParameter parseJson(string fpath, jsonParameter *JP);
        int updateJson(string path);
        int showJsonContain(string path);
        string showJsonValueStr(string key, string value);
        int showJsonValueInt(string key, int value);
        bool fileExist(string filePath);
        jsonParameter parseYtbJson(string fpath, jsonParameter *JP, int* ch);
};

#endif