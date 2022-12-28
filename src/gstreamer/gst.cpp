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
    
    //string pipelineStr = "decklinkvideosrc device-number=0 connection=sdi mode=1080p5994 buffer-size=5 ! video/x-raw,format=UYVY,height=1080,width=1920 ! videoconvert ! video/x-raw,format=NV12,width=1920,height=1080 ! nvh264enc  zerolatency=true strict-gop=false gop-size=120 bframes=0  nonref-p=false  rc-lookahead=0 rc-mode=1 preset=4 vbv-buffer-size=1024 bitrate=8000 max-bitrate=8000 ! video/x-h264,profile=(string)main ! h264parse config-interval=-1 ! flvmux streamable=true name=mux decklinkaudiosrc device-number=0 channels=2 buffer-size=5 ! audio/x-raw,format=S16LE ! audioconvert ! voaacenc bitrate=128000 ! aacparse ! mux. mux. ! queue ! interpipesink name=camsrc sync=true async=false";
    string pipelineStr = "decklinkvideosrc device-number=0 connection=sdi mode=1080p5994 buffer-size=5 ! video/x-raw,width=1920,height=1080 ! videoconvert ! video/x-raw,format=NV12 ! tee name=vraw ! nvh264enc  zerolatency=true strict-gop=false gop-size=120 bframes=0  nonref-p=false  rc-lookahead=0 rc-mode=1 preset=4 vbv-buffer-size=1024 bitrate=8000 max-bitrate=8000 ! video/x-h264,profile=main ! h264parse config-interval=-1 ! flvmux streamable=true name=mux ! interpipesink name=youtube sync=true async=false decklinkaudiosrc device-number=0 channels=2 buffer-size=5 ! audio/x-raw,format=S16LE ! audioconvert ! voaacenc bitrate=128000 ! tee name=at ! aacparse ! mux. vraw. ! queue ! videorate ! video/x-raw,framerate=30000/1001 ! videoscale ! video/x-raw,width=320,height=240 ! nvh264enc  zerolatency=true strict-gop=false gop-size=60 bframes=0  nonref-p=false  rc-lookahead=0 rc-mode=1 preset=4 vbv-buffer-size=512 bitrate=500 max-bitrate=600 ! video/x-h264,profile=baseline ! h264parse config-interval=-1 ! flvmux streamable=true name=mmux ! interpipesink name=monitor sync=true async=false at. ! queue ! aacparse ! mmux.";
    P = gst_parse_launch(pipelineStr.c_str(), &error);
    if (error) {
    g_print ("Error while parsing pipeline description: %s\n", error->message);
    return -1;
    }

    /* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
    bus = gst_element_get_bus (P);   
    gst_bus_add_signal_watch (bus);
    
    /* Start playing the pipeline */
    gst_element_set_state (P, GST_STATE_PLAYING);

    /* Create a GLib Main Loop and set it to run */
    loop = g_main_loop_new (NULL, FALSE);
   // g_main_loop_run (loop);
    if (gst_element_get_state (P, &state, NULL, GST_CLOCK_TIME_NONE) == GST_STATE_CHANGE_FAILURE || state != GST_STATE_PLAYING) {
    g_warning ("State change to playing failed");
    }
   
    
    return 0;
}

int gstEncodeElement::freeEncodePipe()
{
    /* Free resources */
    gst_element_set_state (P, GST_STATE_NULL);
    gst_object_unref (P);
    return 0;
}

int createRtmpPipe::rtmpPipe(string rtmpKey, string node)
{
    cout << "set rtmp pipe" << endl;
    string rtmpStr = "interpipesrc listen-to=" + node + " is-live=true allow-renegotiation=true stream-sync=restart-ts ! queue ! rtmp2sink location=\"rtmp://a.rtmp.youtube.com/live2/x/" + rtmpKey +"\"";
    cout << rtmpStr << endl;
    P1 = gst_parse_launch(rtmpStr.c_str(), &error);

    if (error) {
    g_print ("Error while parsing pipeline description: %s\n", error->message);
    return -1;
    }

    loop1 = g_main_loop_new (NULL, FALSE);
    bus1 = gst_element_get_bus (P1);
    gst_bus_add_signal_watch (bus1);
    gst_element_set_state (P1, GST_STATE_PLAYING);
    /* lets check it gets to PLAYING */
    if (gst_element_get_state (P1, &state, NULL, GST_CLOCK_TIME_NONE) == GST_STATE_CHANGE_FAILURE || state != GST_STATE_PLAYING) {
    g_warning ("State change to playing failed");
    }
    
    return 0;
}
void createRtmpPipe::freeRtmpPipe()
{   
    cout << "free rtmp pipe" << endl;
  //  g_main_loop_run (loop1);
    gst_element_set_state(P1, GST_STATE_PAUSED);
    g_usleep(100);
    gst_element_set_state (P1, GST_STATE_NULL);
    gst_object_unref (P1);
    gst_object_unref (bus1);
}