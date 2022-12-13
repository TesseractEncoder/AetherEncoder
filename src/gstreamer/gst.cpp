/*
gst-launch-1.0 videotestsrc ! video/x-raw,width=1920,height=1080,format=NV12,framerate=60000/1001 ! videoconvert ! video/x-raw,width=1920,height=1080,format=NV12 ! 
queue leaky=downstream max-size-buffers=3 ! interpipesink name=camsrc caps=video/x-raw,format=NV12 sync=true async=false

gst-launch-1.0 decklinkvideosrc device-number=0 connection=sdi mode=1080p5994 buffer-size=5 ! "video/x-raw,format=UYVY" ! videoconvert ! "video/x-raw,format=NV12" ! 
nvh264enc  zerolatency=true strict-gop=false gop-size=60 bframes=0  nonref-p=false  rc-lookahead=0 qp-const=0 rc-mode=2 preset=4 vbv-buffer-size=1024 bitrate=8000 max-bitrate=8000 ! 
"video/x-h264,profile=(string)main" ! h264parse config-interval=1 ! mpegtsmux alignment=7 name=mux decklinkaudiosrc device-number=0 channels=2 buffer-size=5 ! "audio/x-raw,format=S16LE" ! 
audioconvert ! voaacenc bitrate=128000 ! aacparse ! mux. mux. ! tee name=t ! queue ! srtsink streamid=1 uri=srt://104.189.184.146:23002 wait-for-connection=false  sync=true
*/

#include "../AetherEncoder.h"
#include "gst.h"


static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data);



int gstEncodeElement::createEncodePipe(AetherAction AA)
{
    
    string pipelineStr = "videotestsrc ! video/x-raw,format=UYVY,height=1080,width=1920 ! videoconvert ! video/x-raw,format=NV12,width=1920,height=1080 ! nvh264enc  zerolatency=true strict-gop=false gop-size=60 bframes=0  nonref-p=false  rc-lookahead=0 qp-const=0 rc-mode=2 preset=4 vbv-buffer-size=1024 bitrate=8000 max-bitrate=8000 ! video/x-h264,profile=(string)main ! h264parse config-interval=-1 ! flvmux streamable=true name=mux audiotestsrc ! audio/x-raw,format=S16LE ! audioconvert ! voaacenc bitrate=128000 ! aacparse ! mux. mux. ! queue ! interpipesink name=camsrc sync=true async=false";

    P = gst_parse_launch(pipelineStr.c_str(), &error);

    /* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
    bus = gst_element_get_bus (P);   
    gst_bus_add_signal_watch (bus);
    
    /* Start playing the pipeline */
    gst_element_set_state (P, GST_STATE_PLAYING);

    /* Create a GLib Main Loop and set it to run */
    loop = g_main_loop_new (NULL, FALSE);
   // g_main_loop_run (loop);
    
    return 0;
}

int gstEncodeElement::freeEncodePipe()
{
    /* Free resources */
    gst_element_set_state (P, GST_STATE_NULL);
    gst_object_unref (P);
    return 0;
}

void createRtmpPipe::rtmpPipe(string rtmpKey)
{
    cout << "set rtmp pipe" << endl;
    string rtmpStr = "interpipesrc listen-to=camsrc is-live=true allow-renegotiation=true stream-sync=restart-ts ! queue ! rtmp2sink location=\"rtmp://a.rtmp.youtube.com/live2/x/" + rtmpKey +"\"";
    cout << rtmpStr << endl;
    P1 = gst_parse_launch(rtmpStr.c_str(), &error);

    loop1 = g_main_loop_new (NULL, FALSE);
    bus1 = gst_element_get_bus (P1);
    gst_bus_add_signal_watch (bus1);
    gst_element_set_state (P1, GST_STATE_PLAYING);
  //  g_main_loop_run (loop1);
    
}
void createRtmpPipe::freeRtmpPipe()
{   
    cout << "free rtmp pipe" << endl;
  //  g_main_loop_run (loop1);
    gst_element_set_state (P1, GST_STATE_NULL);
    gst_object_unref (P1);
    gst_object_unref (bus1);
}