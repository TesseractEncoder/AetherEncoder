#include "../AetherEncoder.h"
#include "gst.h"


void gstEncodeElement::startRecording()
{
    g_print("startRecording\n");
	GstPad *sinkpad;
    GstPadTemplate *templ;
	
	GstElement *tee = gst_bin_get_by_name( GST_BIN( P ), "vraw" );
	templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(tee), "src_%u");
    teepad = gst_element_request_pad(tee, templ, NULL, NULL);

	queue_record = gst_element_factory_make("queue", "queue_record");
	encoder = gst_element_factory_make("nvh264enc", NULL);
	parse = gst_element_factory_make("h264parse", NULL);
	//muxer = gst_element_factory_make("mp4mux", NULL);
	filesink = gst_element_factory_make("splitmuxsink", NULL);
	//char *file_name = (char*) malloc(255 * sizeof(char));
	//sprintf(file_name, "%s.%05d.mp4", file_path);
	//g_print("Recording to file %s\n", file_name);
	//g_object_set(filesink, "location", "/root/Desktop/my_R&D/test.%05d.ts", "next-file", 2, "min-keyframe-distance", 10000000000, "aggregate-gops", true, NULL);
	g_object_set(filesink, "location", "/root/Desktop/my_R&D/test.%05d.mp4", "max-size-time", 30000000000, NULL);
	
	g_object_set(encoder, "bitrate", 100000, "gop-size", 120, "rc-mode", 1, "preset", 4, NULL);
	g_object_set(parse, "config", -1, NULL);
	//free(file_name);

	gst_bin_add_many(GST_BIN(P), gst_object_ref(queue_record), gst_object_ref(encoder), gst_object_ref(parse), /*gst_object_ref(muxer),*/gst_object_ref(filesink), NULL);
	gst_element_link_many(queue_record, encoder, parse, /*muxer,*/ filesink, NULL);
	
	


	gst_element_sync_state_with_parent(queue_record);
	gst_element_sync_state_with_parent(encoder);
	gst_element_sync_state_with_parent(parse);
	//gst_element_sync_state_with_parent(muxer);
	gst_element_sync_state_with_parent(filesink);
	
	//gst_element_set_state(pipeline, GST_STATE_PLAYING);

	sinkpad = gst_element_get_static_pad(queue_record, "sink");
	gst_pad_link(teepad, sinkpad);
	gst_object_unref(sinkpad);

	recording = TRUE;
}

static GstPadProbeReturn unlink_cb(GstPad *pad, GstPadProbeInfo *info, gstEncodeElement data) {
	g_print("Unlinking...");
	GstPad *sinkpad;
	sinkpad = gst_element_get_static_pad (data.queue_record, "sink");
	gst_pad_unlink (data.teepad, sinkpad);
	gst_object_unref (sinkpad);

	gst_element_send_event(encoder, gst_event_new_eos()); 
	//finalize();
	return GST_PAD_PROBE_REMOVE;
}

void gstEncodeElement::stopRecording()
{
    g_print("stopRecording\n");
	gst_pad_add_probe(teepad, GST_PAD_PROBE_TYPE_IDLE, unlink_cb, gstEncodeElement GE, (GDestroyNotify) g_free);
}