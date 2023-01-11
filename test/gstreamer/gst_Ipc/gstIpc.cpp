/*
livepipe = GST_PIPELINE (gst_parse_launch("interpipesrc listen-to=camsrc is-live=true allow-renegotiation=true stream-sync=restart-ts ! queue max-size-buffers=3 leaky=downstream"
      " ! nvvidconv ! video/x-raw(memory:NVMM),format=I420,width=1280,height=720 "
      " ! videorate ! video/x-raw(memory:NVMM),framerate=10/1 "
      " ! nvv4l2vp8enc bitrate=1000000 ! rtpvp8pay pt=96 ! udpsink host=127.0.0.1 port=5004", &error));
campipe = GST_PIPELINE (gst_parse_launch("nvarguscamerasrc ! video/x-raw(memory:NVMM),width=1932,height=1090,format=NV12,framerate=30/1 "
      " ! nvvidconv left=6 right=1926 top=6 bottom=1086 ! video/x-raw(memory:NVMM),width=1920,height=1080,format=RGBA "
      " ! nvvidconv ! video/x-raw(memory:NVMM),format=NV12 "
      " ! queue leaky=downstream max-size-buffers=3 "
      " ! interpipesink name=camsrc caps=video/x-raw(memory:NVMM),format=NV12 sync=true async=false", &error));

*/

#include <iostream>
#include <memory>
#include <thread>
#include <gst/gst.h>
#include <glib.h>

using namespace std;

#define GST_PLAY_KB_ARROW_UP    "\033[A"
#define GST_PLAY_KB_ARROW_DOWN  "\033[B"
#define GST_PLAY_KB_ARROW_RIGHT "\033[C"
#define GST_PLAY_KB_ARROW_LEFT  "\033[D"

class GSTElement{
    public:
        GMainLoop *loop1, *loop2, *loop3;
        GstElement *P1,*P2, *P3, *Src, *Vconv, *Vrate,*capsfilter, *capsfilterRate, *Queue;
        GstElement *Mux, *Sink, *H264Parse, *TimeOvlay, *Paktize, *capsfilterConvert;
        GstCaps *srccaps,*capsRate,*capsConvert;   
        GstBus *bus1, *bus2, *bus3;
        guint bus_watch_id;
        GError *error = NULL;
        GstStateChangeReturn ret;
        GIOChannel *io_stdin;

    public:
        int createEncodingPipe();
        int createRtmpPipe();
        int creteMulticatsPipe();
        int createDecodePipe();
};
/*
int GSTElement::createEncodingPipe()
{

}*/

void controlButton(GSTElement GE)
{
    cout << "i am thread" << endl;
    int i;

   // GSTElement GE;

    while(true)
    {
            cout << "waiting for key input" << endl;
            cin >> i;

            switch (i)
            {
            case 1:
                cout << "get key 1" << endl;
                gst_element_set_state (GE.P2, GST_STATE_PAUSED);
                break;
            case 2:
                cout << "get key 2" << endl;
                gst_element_set_state (GE.P2, GST_STATE_PLAYING);
                break;
            case 3:
                cout << "get key 1" << endl;
                gst_element_set_state (GE.P3, GST_STATE_PAUSED);
                break;
            case 4:
                cout << "get key 2" << endl;
                gst_element_set_state (GE.P3, GST_STATE_PLAYING);
                break;
            default:
                //gst_element_set_state (GE.P2, GST_STATE_NULL);
                break;
            }
    }
}

int main(int argc, char *argv[])
{
    GSTElement GE;
    gst_init (&argc, &argv);

    //GE.P1 = gst_parse_launch("videotestsrc ! video/x-raw,width=1920,height=1080,format=NV12,framerate=60000/1001 ! videoconvert ! video/x-raw,width=1920,height=1080,format=NV12 ! queue  ! interpipesink name=camsrc caps=video/x-raw,format=NV12 sync=true async=false", &GE.error);

   // GE.P2 = gst_parse_launch("interpipesrc listen-to=camsrc is-live=true allow-renegotiation=true stream-sync=restart-ts ! queue ! videoconvert ! video/x-raw,format=NV12,width=1920,height=1080 ! videorate ! video/x-raw,framerate=60000/1001 ! nvh264enc bitrate=10000 ! h264parse config-interval=-1 ! mpegtsmux name=mux alignment=7 ! udpsink host=226.1.1.1 port=23000", &GE.error);

    GE.P1 = gst_parse_launch("videotestsrc ! video/x-raw,width=1920,height=1080,format=NV12,framerate=60000/1001 ! videoconvert ! video/x-raw,width=1920,height=1080,format=NV12 ! videorate ! video/x-raw,framerate=60000/1001 ! nvh264enc bitrate=10000 ! h264parse config-interval=-1 ! mpegtsmux name=mux alignment=7 ! queue ! interpipesink name=camsrc sync=true async=false", &GE.error);

    GE.P2 = gst_parse_launch("interpipesrc listen-to=camsrc is-live=true allow-renegotiation=true stream-sync=restart-ts ! udpsink host=226.1.1.1 port=23000", &GE.error);

    GE.P3 = gst_parse_launch("interpipesrc listen-to=camsrc is-live=true allow-renegotiation=true stream-sync=restart-ts ! udpsink host=226.1.1.1 port=23002", &GE.error);


   // g_signal_connect (GE.P1, "source-setup", G_CALLBACK (source_setup), &data);
    GE.io_stdin = g_io_channel_unix_new (fileno (stdin));
  
    /* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
    GE.bus1 = gst_element_get_bus (GE.P1);
    GE.bus2 = gst_element_get_bus (GE.P2);
    GE.bus3 = gst_element_get_bus (GE.P3);
    gst_bus_add_signal_watch (GE.bus1);
    gst_bus_add_signal_watch (GE.bus2);
    gst_bus_add_signal_watch (GE.bus3);
   // g_signal_connect (G_OBJECT (bus), "message::error", (GCallback)error_cb, &data);
   // gst_object_unref (GE.bus1);
   // gst_object_unref (GE.bus2);
   // gst_object_unref (GE.bus3);
    
    /* Start playing the pipeline */
    gst_element_set_state (GE.P1, GST_STATE_PLAYING);
    gst_element_set_state (GE.P2, GST_STATE_PLAYING);
    gst_element_set_state (GE.P3, GST_STATE_PLAYING);

    thread t1(controlButton,GE);
    t1.join();
    
    /* Create a GLib Main Loop and set it to run */
    GE.loop1 = g_main_loop_new (NULL, FALSE);
    GE.loop2 = g_main_loop_new (NULL, FALSE);
    GE.loop3 = g_main_loop_new (NULL, FALSE);
    cout<<"run loop 1"<<endl;
    g_main_loop_run (GE.loop1);
    cout<<"run loop 2"<<endl;
    g_main_loop_run (GE.loop2);
    cout<<"run loop 3"<<endl;
    g_main_loop_run (GE.loop3);
    cout<<"run all loop"<<endl;
    
    /* Free resources */
    gst_element_set_state (GE.P1, GST_STATE_NULL);
    gst_element_set_state (GE.P2, GST_STATE_NULL);
    gst_element_set_state (GE.P3, GST_STATE_NULL);
    gst_object_unref (GE.P1);
    gst_object_unref (GE.P2);
    gst_object_unref (GE.P3);
    
}