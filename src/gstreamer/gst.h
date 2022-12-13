#ifndef GST_INCLUDE
#define GST_INCLUDE

#include <memory>
#include <gst/gst.h>
#include <glib.h>
#include "../json/jsonParser.h"

class gstEncodeElement{
    public:
        GMainLoop *loop;
        GstElement *P, *src, *vconv, *vrate,*capsfiltersrc, *capsfilterRate, *queue;
        GstElement *mux, *sink, *H264Encode, *H264Parse, *capsfilterConvert, *capsfilterH264enc;
        GstCaps *srccaps,*capsRate,*capsConvert, *capsH264enc;   
        GstBus *bus;
        guint bus_watch_id;
        GError *error = NULL;
        GstStateChangeReturn ret;
        GIOChannel *io_stdin;
       // GMainLoop *loop;
       // GstElement *P;
       // GstBus *bus;

    public:
        int createEncodePipe(AetherAction AA);
        int freeEncodePipe();
};

class createMulticastPipe{
   
    public:
        createMulticastPipe(AetherAction AA);
};

class createRtmpPipe{
     public:
        GMainLoop *loop1;
        GstElement *P1;
        GstBus *bus1;
        GError *error = NULL;
    public:
        void rtmpPipe(string rtmpKey);
        void freeRtmpPipe();
};

//int check_gstreamer(void);

#endif