#include "AetherEncoder.h"
#include "gstreamer/gst.h"
#include "json/jsonParser.h"
#include <thread>
#include <signal.h>

using namespace std;

int handleYoutubePipeline(struct jsonParameter *YJP, createRtmpPipe *newRtmpPipe, int ch)
{
   // int ch;
    
    if(YJP[0].ytb[ch].active == "start"){
        //createRtmpPipe *newRtmpPipe1 = new createRtmpPipe;
        newRtmpPipe[ch].rtmpPipe(YJP[0].ytb[ch].link, YJP[0].ytb[ch].streamto);
    }
    else if(YJP[0].ytb[ch].active == "stop") {
        newRtmpPipe[YJP[0].ytb[ch].youtubeNO].freeRtmpPipe();
                //delete &newRtmpPipe[YJP.ytb.youtubeNO];
    }

    return 0;
}
/*
int sigintHandler(gstEncodeElement GE) {
	g_print("You ctrl-c!\n");
	if (GE.recording)
		GE.stopRecording();
	else
		GE.startRecoding();
	return 0;
}
*/
int main(int argc, char *argv[])
{
    gstEncodeElement GE;
    createRtmpPipe RP;
    AetherAction AA;
    struct jsonParameter *JP = new jsonParameter[8];
    createRtmpPipe *newRtmpPipe = new createRtmpPipe[8];
    int ytbch;
    //signal(SIGINT, sigintHandler);
    cout<< "Initialization gstreamer\n" << endl;
    gst_init (&argc, &argv);
    
    //parse json file
    JP[0]=AA.parseJson("../sample.json",JP);

    cout<< "setup encoding pipe" << endl;
    GE.createEncodePipe(AA);

    while(true)
    {
        string filePath="../cmd/ytb.json";
        g_usleep(1000000);
        if(AA.fileExist(filePath)){
            cout << "call youtube json parser" << endl;
            JP[0] = AA.parseYtbJson(filePath, JP, &ytbch);
            handleYoutubePipeline(JP,newRtmpPipe,ytbch); 
            remove(filePath.c_str());
        }

      /*  if (GE.recording){
            g_print("stop recording\n");
		  //  GE.stopRecording();
        }
	    else{
            g_print("start recording\n");
		  //  GE.startRecording();
        }*/
    }

    //RP.freeRtmpPipe();
    GE.freeEncodePipe();   
}