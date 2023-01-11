

#include <iostream>
#include <memory>
#include <thread>
#include <gst/gst.h>
#include <glib.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>

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
        GstElement *testsink = NULL;
  	GstElement *testsource = NULL;

    public:
        int createEncodingPipe();
        int createRtmpPipe();
        int creteMulticatsPipe();
        int createDecodePipe();
};

static GstFlowReturn on_new_sample_from_sink (GstElement * elt, ProgramData * data)
{
  GstSample *sample;
  GstBuffer *app_buffer, *buffer;
  GstElement *source;
  GstFlowReturn ret;
  GstMapInfo map;
  guint8 *rdata;
  int dataLength;
  int i;
  g_print ("%s\n", __func__);
 
  /* get the sample from appsink */
  sample = gst_app_sink_pull_sample (GST_APP_SINK (elt));
  buffer = gst_sample_get_buffer (sample);
 
  /* make a copy */
  app_buffer = gst_buffer_copy_deep (buffer);
 
  gst_buffer_map (app_buffer, &map, GST_MAP_WRITE);
 
  /* Here You modify your buffer data */
  modify_in_data (app_buffer);
 
  /* get source an push new buffer */
  source = gst_bin_get_by_name (GST_BIN (data->sink), "testsource");
  ret = gst_app_src_push_buffer (GST_APP_SRC (source), app_buffer);
  gst_sample_unref (sample);
  /* we don't need the appsink sample anymore */
  gst_buffer_unmap (app_buffer, &map);
  gst_object_unref (source);
 
  return ret;
}

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


    GE.P1 = gst_parse_launch("videotestsrc ! video/x-raw,width=1920,height=1080,format=NV12,framerate=60000/1001 ! videoconvert ! video/x-raw,width=1920,height=1080,format=NV12 ! videorate ! video/x-raw,framerate=60000/1001 ! nvh264enc bitrate=10000 ! h264parse config-interval=-1 ! mpegtsmux name=mux alignment=7 ! queue ! appsink name=testsink sync=true async=false", &GE.error);

    GE.P2 = gst_parse_launch("appsrc name=testsrc ! udpsink host=226.1.1.1 port=23000", &GE.error);

    GE.P3 = gst_parse_launch("appsrc name=testsrc ! udpsink host=226.1.1.1 port=23002", &GE.error);


    GE.testsink = gst_bin_get_by_name (GST_BIN (GE.P1), "testsink");
  g_object_set (G_OBJECT (GE.testsink), "emit-signals", TRUE, "sync", FALSE, NULL);
  g_signal_connect (GE.testsink, "new-sample",G_CALLBACK (on_new_sample_from_sink), GE);
  gst_object_unref (GE.testsink);
  
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

    //thread t1(controlButton,GE);
   // t1.join();
    
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
