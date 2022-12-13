#include "AetherEncoder.h"
#include "gstreamer/gst.h"
#include "json/jsonParser.h"
#include <thread>

using namespace std;


//void creteNewPipeline()

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
                    newRtmpPipe1->rtmpPipe(string("2mzg-vv14-75b3-746m-9xxz"));
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
                    newRtmpPipe2->rtmpPipe(string("4rgs-wmqe-t00b-zm63-9adu"));
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
                    newRtmpPipe3->rtmpPipe(string("xwgw-61wf-wdbj-qrpr-cmgb"));
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


int main(int argc, char *argv[])
{
    AetherAction AA;
    gstEncodeElement GE;
    createRtmpPipe RP;
    struct jsonParameter JP;
    cout<< "Initialization gstreamer\n" << endl;
    gst_init (&argc, &argv);

    //parse json file
    JP=AA.parseJson("../sample.json");
    
    cout<< "setup encoding pipe" << endl;
    GE.createEncodePipe(AA);

    cout<< "setup RTMP pipe" << endl;
    RP.rtmpPipe(string("0dvg-jqed-4vc4-g6az-1qgd"));

    thread t1(controlButton,GE,RP,AA);
    t1.join();



    RP.freeRtmpPipe();
    GE.freeEncodePipe();
    

}