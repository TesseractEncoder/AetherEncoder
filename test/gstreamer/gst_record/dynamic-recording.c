#include <string.h>
#include <gst/gst.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// v4l2src ! tee name=t t. ! x264enc ! mp4mux ! filesink location=/home/rish/Desktop/okay.264 t. ! videoconvert ! autovideosink

static GMainLoop *loop;
static GstElement *pipeline, *pipeline1, *src, *tee, *encoder, *muxer, *filesink, *videoconvert, *videosink, *queue_record, *queue_display, *parse;
static GstBus *bus;
static GstPad *teepad;
static GError *error=NULL;
static gboolean recording = FALSE;
static gint counter = 0;
static char *file_path;
static void finalize();

static gboolean
message_cb (GstBus * bus, GstMessage * message, gpointer user_data)
{
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR:{
      GError *err = NULL;
      gchar *name, *debug = NULL;

      name = gst_object_get_path_string (message->src);
      gst_message_parse_error (message, &err, &debug);

      g_printerr ("ERROR: from element %s: %s\n", name, err->message);
      if (debug != NULL)
        g_printerr ("Additional debug info:\n%s\n", debug);

      g_error_free (err);
      g_free (debug);
      g_free (name);

      g_main_loop_quit (loop);
      break;
    }
    case GST_MESSAGE_WARNING:{
		GError *err = NULL;
		gchar *name, *debug = NULL;

		name = gst_object_get_path_string (message->src);
		gst_message_parse_warning (message, &err, &debug);

		g_printerr ("ERROR: from element %s: %s\n", name, err->message);
		if (debug != NULL)
		g_printerr ("Additional debug info:\n%s\n", debug);

		g_error_free (err);
		g_free (debug);
		g_free (name);
		break;
    }
    case GST_MESSAGE_EOS: {
		g_print ("Got EOS\n");
		// g_main_loop_quit (loop);
		// gst_element_set_state (pipeline, GST_STATE_NULL);
		// g_main_loop_unref (loop);
		// gst_object_unref (pipeline);
		// exit(0);
		break;
	}
	case GST_MESSAGE_ELEMENT: {
    	const GstStructure *s = gst_message_get_structure (message);

    	if (gst_structure_has_name (s, "GstBinForwarded")) {
	        GstMessage *forward_msg = NULL;

	        gst_structure_get (s, "message", GST_TYPE_MESSAGE, &forward_msg, NULL);
	        if (GST_MESSAGE_TYPE (forward_msg) == GST_MESSAGE_EOS) {
	            g_print ("EOS from element %s\n", GST_OBJECT_NAME (GST_MESSAGE_SRC (forward_msg)));	
	            finalize();
	        }
	        gst_message_unref (forward_msg);
    	}
    	break;
	}
    default:
		break;
  }

  return TRUE;
}

static void finalize() {
	gst_bin_remove(GST_BIN (pipeline), queue_record);
	gst_bin_remove(GST_BIN (pipeline), encoder);
	gst_bin_remove(GST_BIN (pipeline), muxer);
	gst_bin_remove(GST_BIN (pipeline), filesink);

	gst_element_set_state(queue_record, GST_STATE_NULL);
	gst_element_set_state(encoder, GST_STATE_NULL);
	gst_element_set_state(muxer, GST_STATE_NULL);
	gst_element_set_state(filesink, GST_STATE_NULL);

	gst_object_unref(queue_record);
	gst_object_unref(encoder);
	gst_object_unref(muxer);
	gst_object_unref(filesink);

	gst_element_release_request_pad (tee, teepad);
	gst_object_unref (teepad);

	recording = FALSE;
	g_print("Unlinked\n");
}

static GstPadProbeReturn unlink_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
	g_print("Unlinking...");
	GstPad *sinkpad;
	sinkpad = gst_element_get_static_pad (queue_record, "sink");
	gst_pad_unlink (teepad, sinkpad);
	gst_object_unref (sinkpad);

	gst_element_send_event(encoder, gst_event_new_eos()); 
	//finalize();
	return GST_PAD_PROBE_REMOVE;
}

void stopRecording() {
	g_print("stopRecording\n");
	gst_pad_add_probe(teepad, GST_PAD_PROBE_TYPE_IDLE, unlink_cb, NULL, (GDestroyNotify) g_free);
}

void startRecording() {
	g_print("startRecording\n");
	GstPad *sinkpad;
    GstPadTemplate *templ;
	
	GstElement *tee = gst_bin_get_by_name( GST_BIN( pipeline ), "vraw" );
	templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(tee), "src_%u");
    	teepad = gst_element_request_pad(tee, templ, NULL, NULL);
    	//pipeline = gst_parse_launch("queue name=queue ! x264enc bitrate=100000 name=nvenc ! mp4mux name=mux ! filesink name=sink", &error);
    	//GstElement *queue_record = gst_bin_get_by_name( GST_BIN( pipeline ), "queue" );
    	//GstElement *encoder = gst_bin_get_by_name( GST_BIN( pipeline ), "nvenc" );
	//GstElement *muxer = gst_bin_get_by_name( GST_BIN( pipeline ), "mux" );
	//GstElement *filesink = gst_bin_get_by_name( GST_BIN( pipeline ), "sink" );
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

	gst_bin_add_many(GST_BIN(pipeline), gst_object_ref(queue_record), gst_object_ref(encoder), gst_object_ref(parse), /*gst_object_ref(muxer), */gst_object_ref(filesink), NULL);
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

int sigintHandler(int unused) {
	g_print("You ctrl-c!\n");
	if (recording)
		stopRecording();
	else
		startRecording();
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		g_printerr("Enter commandline argument folder path to save recorded video to.\nExample : ./program_name /home/xyz/Desktop/\n");
		return -1;
	}

	if (strstr(argv[1],".mp4") != NULL) {
		g_printerr("Please specify folder path only.\nExample : ./a.out /home/xyz/Desktop/\n");
		return -1;
	}

	file_path = (char*) malloc(255 * sizeof(char));
	file_path = argv[1];
	
	signal(SIGINT, sigintHandler);
	gst_init (&argc, &argv);

	pipeline = gst_pipeline_new(NULL);
	pipeline = gst_parse_launch("decklinkvideosrc device-number=0 connection=sdi mode=1080p5994 buffer-size=5 ! video/x-raw,width=1920,height=1080 ! videoconvert ! video/x-raw,format=NV12 ! tee name=vraw ! nvh264enc  zerolatency=true strict-gop=false gop-size=120 bframes=0  nonref-p=false  rc-lookahead=0 rc-mode=1 preset=4 vbv-buffer-size=1024 bitrate=8000 max-bitrate=8000 ! video/x-h264,profile=main ! h264parse config-interval=-1 ! flvmux streamable=true name=mux ! interpipesink name=youtube sync=true async=false decklinkaudiosrc device-number=0 channels=2 buffer-size=5 ! audio/x-raw,format=S16LE ! audioconvert ! voaacenc bitrate=128000 ! tee name=at ! aacparse ! mux. vraw. ! queue ! videorate ! video/x-raw,framerate=30000/1001 ! videoscale ! video/x-raw,width=320,height=240 ! nvh264enc  zerolatency=true strict-gop=false gop-size=60 bframes=0  nonref-p=false  rc-lookahead=0 rc-mode=1 preset=4 vbv-buffer-size=512 bitrate=500 max-bitrate=600 ! video/x-h264,profile=baseline ! h264parse config-interval=-1 ! flvmux streamable=true name=mmux ! interpipesink name=monitor sync=true async=false at. ! queue ! aacparse ! mmux.", &error);
	

	g_object_set(GST_BIN(pipeline), "message-forward", TRUE, NULL);

	startRecording();
	loop = g_main_loop_new(NULL, FALSE);

	bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
	gst_bus_add_signal_watch(bus);
	g_signal_connect(G_OBJECT(bus), "message", G_CALLBACK(message_cb), NULL);
	gst_object_unref(GST_OBJECT(bus));

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	g_print("Starting loop\n");
	g_main_loop_run(loop);

	return 0;
}
