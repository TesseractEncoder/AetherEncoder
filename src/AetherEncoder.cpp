#include "AetherEncoder.h"
#include "gstreamer/gst.h"
#include "json/jsonParser.h"
#include <thread>

using namespace std;

void controlButton(gstEncodeElement GE,createRtmpPipe RP,AetherAction AA)
{
    cout << "i am thread" << endl;
    int i;
    bool S1=true, S2=true;
    GstStateChangeReturn ret;
    createRtmpPipe *newRtmpPipe = new createRtmpPipe;
    while(true)
    {
            cout << "waiting for key input" << endl;
            cin >> i;

            switch (i)
            {
            case 1:
                cout << "get key 1" << endl;
                if(S1){
                    ret=gst_element_set_state (RP.P1, GST_STATE_PAUSED);
                    g_print("youtube pipe OFF\n");
                }
                else{
                    gst_element_set_state (RP.P1, GST_STATE_READY);
                    g_usleep(500);
                    ret=gst_element_set_state (RP.P1, GST_STATE_PLAYING);
                    g_print("youtube pipe ON\n");
                }
                S1=!S1;
                break;
            case 2:
                cout << "get key 2" << endl;
                RP.freeRtmpPipe();
                break;
            case 3:
                cout << "get key 3" << endl;
                RP.rtmpPipe(string("0dvg-jqed-4vc4-g6az-1qgd"));
                break;
            case 4:
                cout << "get key 4" << endl;
               
               newRtmpPipe->rtmpPipe(string("bfa2-41gs-vz04-wcje-05u9"));
                break;
            case 5:
                cout << "get key 5" << endl;
                 newRtmpPipe->freeRtmpPipe();
               delete newRtmpPipe;
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