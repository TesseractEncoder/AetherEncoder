#include "AetherEncoder.h"
#include "gstreamer/gst.h"
#include "json/jsonParser.h"
#include <thread>

using namespace std;


//void creteNewPipeline()
/*
void controlButton(gstEncodeElement GE,createRtmpPipe RP,AetherAction AA)
{
    cout << "i am thread" << endl;
    int i;
    bool S1=true, S2=true, S3=true, S4=true;
    GstStateChangeReturn ret;
    createRtmpPipe *newRtmpPipe1 = new createRtmpPipe;
    createRtmpPipe *newRtmpPipe2 = new createRtmpPipe;
    createRtmpPipe *newRtmpPipe3 = new createRtmpPipe;
    while(true)
    {
            cout << "waiting for key input" << endl;
            cin >> i;

            switch (i)
            {
            case 1:
                cout << "get key 1" << endl;
                if(S1){
                    ret=gst_element_set_state(RP.P1, GST_STATE_PAUSED);
                    g_usleep(100);
                    gst_element_set_state(RP.P1, GST_STATE_NULL);
                    RP.freeRtmpPipe();
                    g_print("youtube 1 pipe OFF\n");
                }
                else{
                    RP.rtmpPipe(string("0dvg-jqed-4vc4-g6az-1qgd"));
                    g_print("youtube 1 pipe ON\n");
                }
                S1=!S1;
                break;
            case 2:
                cout << "get key 2" << endl;
                if(S2){
                    newRtmpPipe1->rtmpPipe(string("y7j5-27m0-9ztd-6hb1-2kzt"));
                        g_print("youtube 2 pipe ON\n");
                }
                else{
                    newRtmpPipe1->freeRtmpPipe();
                  //  delete newRtmpPipe1;
                    g_print("youtube 2 pipe OFF\n");
                }
                S2=!S2;
                break;
            case 3:
                cout << "get key 3" << endl;
                if(S3){
                    newRtmpPipe2->rtmpPipe(string("xj8u-t5jt-kx5c-krej-46mf"));
                        g_print("youtube 3 pipe ON\n");
                }
                else{
                    newRtmpPipe2->freeRtmpPipe();
                   // delete newRtmpPipe2;
                    g_print("youtube 3 pipe OFF\n");
                }
                S3=!S3;
                break;
                 
            case 4:
                cout << "get key 4" << endl;
                if(S4){
                    newRtmpPipe3->rtmpPipe(string("uz6e-p266-zpgb-uzhy-6c76"));
                        g_print("youtube 4 pipe ON\n");
                }
                else{
                    newRtmpPipe3->freeRtmpPipe();
                    //delete newRtmpPipe3;
                    g_print("youtube 4 pipe OFF\n");
                }
                S4=!S4;
                break;
            default:
                //gst_element_set_state (GE.P2, GST_STATE_NULL);
                break;
            }

        if (ret == GST_STATE_CHANGE_FAILURE) {
            g_printerr ("Unable to set the pipeline to the playing state.\n");
            gst_object_unref (RP.P1);
            //return -1;
        }
    }
}
*/

int handleYoutubePipeline(struct jsonParameter *YJP, createRtmpPipe *newRtmpPipe, int ch)
{
   // int ch;
    
    if(YJP[0].ytb[ch].active == "start"){
        //createRtmpPipe *newRtmpPipe1 = new createRtmpPipe;
        newRtmpPipe[ch].rtmpPipe(YJP[0].ytb[ch].key, YJP[0].ytb[ch].streamto);
    }
    else if(YJP[0].ytb[ch].active == "stop") {
        newRtmpPipe[YJP[0].ytb[ch].youtubeNO].freeRtmpPipe();
                //delete &newRtmpPipe[YJP.ytb.youtubeNO];
    }

    return 0;
}
int main(int argc, char *argv[])
{
    gstEncodeElement GE;
    createRtmpPipe RP;
    AetherAction AA;
    struct jsonParameter *JP = new jsonParameter[8];
    createRtmpPipe *newRtmpPipe = new createRtmpPipe[8];
    int ytbch;
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
    }

    //RP.freeRtmpPipe();
    GE.freeEncodePipe();   
}