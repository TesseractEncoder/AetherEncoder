#ifndef JSONPARSER_INCLUDE
#define JSONPARSER_INCLUDE

//using namespace std;
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



#endif