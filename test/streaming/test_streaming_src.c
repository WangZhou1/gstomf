#include <gst/gst.h>
#include "stdio.h"

#define TEST_AUD_SRC 	1
#define TEST_H264_SRC	1
#define TEST_H265_SRC	0
#define TEST_JPEG_SRC	1
#define TEST_YUV_SRC	1

typedef struct _App App;

struct _App
{
  GstElement *pipeline;
  GstElement *src;
  GstElement *sink;
};

static App _app;

static gint _number = 100;
static gboolean _silent = FALSE;
static gboolean _dump = FALSE;
static gboolean _file = FALSE;

static GOptionEntry _entries[] = {
  {"num-buffers", 'n', 0, G_OPTION_ARG_INT, &_number,"Test buffer numbers ",   "B"},
  {"silent", 's', 0, G_OPTION_ARG_NONE, &_silent, "Slient test", NULL},
  {"dump", 'd', 0, G_OPTION_ARG_NONE, &_dump, "Dump buffer contents to stdout", NULL},
  {"file", 'f', 0, G_OPTION_ARG_NONE, &_file, "Save to file", NULL},
  {NULL,}
};


static gboolean
bus_call (GstBus * bus, GstMessage * msg, gpointer data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_EOS:{
      g_print ("End-of-stream\n");
      g_main_loop_quit (loop);
      break;
    }
    case GST_MESSAGE_ERROR:{
      gchar *debug;
      GError *err;

      gst_message_parse_error (msg, &err, &debug);
      g_printerr ("Debugging info: %s\n", (debug) ? debug : "none");
      g_free (debug);

      g_print ("Error: %s\n", err->message);
      g_error_free (err);

      g_main_loop_quit (loop);

      break;
    }
    default:
      break;
  }
  return TRUE;
}

static void test_audsrc(gint argc, gchar * argv[]){
  g_print("\ntest_audsrc... s\n");

  App *app = &_app;

  GMainLoop *loop;
  GstBus *bus;
  guint bus_watch_id;

  gchar* media;

  gst_init(&argc, &argv);

  app->pipeline = gst_pipeline_new("pipeline");

  app->src = gst_element_factory_make("omfaudsrc", "audsrc");
  if(_file){
	app->sink = gst_element_factory_make("filesink", "sink");
    g_object_set(app->sink, "location" ,"/tmp/g_test.pcm", "silent", _silent,  NULL);
  }
  else{
	app->sink = gst_element_factory_make("fakesink", "sink");
	g_object_set(app->sink, "silent", _silent, NULL);
  }
  g_object_set(app->src, "num-buffers", _number, "dump", _dump, NULL);
  g_object_set(app->src, "samples", 1600, "channel", 1, "rate", 16000, NULL);

  loop = g_main_loop_new(NULL, FALSE);

  bus = gst_element_get_bus(app->pipeline);
  bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
  g_object_unref(bus);

  gst_bin_add_many(GST_BIN(app->pipeline), app->src, app->sink, NULL);
  gst_element_link_many(app->src, app->sink, NULL);

  gst_element_set_state(app->pipeline, GST_STATE_PLAYING);

  g_object_get(app->src, "media-info", &media, NULL);
  g_print("media-info:%s\n", media);  

  g_main_loop_run(loop); 

  gst_element_set_state(app->pipeline, GST_STATE_NULL);
  g_object_unref(app->pipeline);
  g_source_remove(bus_watch_id);
  g_main_loop_unref(loop);

  g_print("test_audsrc... e\n");
}

static void test_h264src(gint argc, gchar * argv[]){
  g_print("\ntest_h264src... s\n");

  App *app = &_app;

  GMainLoop *loop;
  GstBus *bus;
  guint bus_watch_id;
  gchar *media;

  gst_init(&argc, &argv);

  app->pipeline = gst_pipeline_new("pipeline");

  app->src = gst_element_factory_make("omfh264src", "h264src");
  if(_file){
	app->sink = gst_element_factory_make("filesink", "sink");
	g_object_set(app->sink, "location" ,"/tmp/g_test.h264", "silent", _silent,  NULL);
  }
  else{
	app->sink = gst_element_factory_make("fakesink", "sink");
	g_object_set(app->sink, "silent", _silent, NULL);
  }
  g_object_set(app->src, "num-buffers", _number, "dump", _dump, NULL);
  g_object_set(app->src, "width", 1920, "height", 1080, NULL);
  g_object_set(app->src, "framerate", 30, "bitrate", 2000000, NULL);
  g_object_set(app->src, "goptype", 1, "gop", 30, NULL);

  loop = g_main_loop_new(NULL, FALSE);

  bus = gst_element_get_bus(app->pipeline);
  bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
  g_object_unref(bus);

  gst_bin_add_many(GST_BIN(app->pipeline), app->src, app->sink, NULL);
  gst_element_link_many(app->src, app->sink, NULL);

  gst_element_set_state(app->pipeline, GST_STATE_PLAYING);

  g_object_get(app->src, "media-info", &media, NULL);
  g_print("media-info:%s\n", media);  

  g_main_loop_run(loop);

  gst_element_set_state(app->pipeline, GST_STATE_NULL);
  g_object_unref(app->pipeline);
  g_source_remove(bus_watch_id);
  g_main_loop_unref(loop);

  g_print("test_h264src... e\n");
}

static void test_h265src(gint argc, gchar * argv[]){
  g_print("\ntest_h265src... s\n");

  App *app = &_app;

  GMainLoop *loop;
  GstBus *bus;
  guint bus_watch_id;
  gchar *media;

  gst_init(&argc, &argv);

  app->pipeline = gst_pipeline_new("pipeline");

  app->src = gst_element_factory_make("omfh265src", "h265src");
  if(_file){
	app->sink = gst_element_factory_make("filesink", "sink");
	g_object_set(app->sink, "location" ,"/tmp/g_test.h265", "silent", _silent,  NULL);
  }
  else{	
  	app->sink = gst_element_factory_make("fakesink", "sink");
	g_object_set(app->sink, "silent", _silent,  NULL);
  }

  g_object_set(app->src, "num-buffers", _number, "dump", _dump, NULL);    
  g_object_set(app->src, "width", 1920, "height", 1080, NULL);
  g_object_set(app->src, "framerate", 30, "bitrate", 2000000, NULL);
  g_object_set(app->src, "goptype", 1, "gop", 30, NULL);

  loop = g_main_loop_new(NULL, FALSE);

  bus = gst_element_get_bus(app->pipeline);
  bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
  gst_object_unref(bus);

  gst_bin_add_many(GST_BIN(app->pipeline), app->src, app->sink, NULL);
  gst_element_link_many(app->src, app->sink, NULL);

  gst_element_set_state(app->pipeline, GST_STATE_PLAYING);

  g_object_get(app->src, "media-info", &media, NULL);
  g_print("media-info:%s\n", media);  

  g_main_loop_run(loop);

  gst_element_set_state(app->pipeline, GST_STATE_NULL);
  g_object_unref(app->pipeline);
  g_source_remove(bus_watch_id);
  g_main_loop_unref(loop);
  
  g_print("test_h265src... e\n");
}

static void test_jpegsrc(gint argc, gchar * argv[]){
  g_print("\ntest_jpegsrc... s\n");

  App *app = &_app;

  GMainLoop *loop = NULL;
  GstBus *bus;
  guint bus_watch_id;
  gchar *media;

  gst_init(&argc, &argv);

  app->pipeline = gst_pipeline_new("pipeline");

  app->src = gst_element_factory_make("omfjpegsrc", "jpegsrc");
  app->sink = gst_element_factory_make("fakesink", "sink");

  g_object_set(app->src, "num-buffers", _number, "dump", _dump, NULL);
  g_object_set(app->src, "width", 1920, "height", 1080, "qp", 80, NULL);
  g_object_set(app->sink, "silent", _silent, NULL);

  loop = g_main_loop_new(NULL, FALSE);

  bus = gst_element_get_bus(app->pipeline);
  bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
  gst_object_unref(bus);

  gst_bin_add_many(GST_BIN(app->pipeline), app->src, app->sink, NULL);
  gst_element_link_many(app->src, app->sink, NULL);

  gst_element_set_state(app->pipeline, GST_STATE_PLAYING);

  g_object_get(app->src, "media-info", &media, NULL);
  g_print("media-info:%s\n", media);  

  g_main_loop_run(loop);

  gst_element_set_state(app->pipeline, GST_STATE_NULL);
  g_object_unref(app->pipeline);
  g_source_remove(bus_watch_id);
  g_main_loop_unref(loop);
  
  g_print("test_jpegsrc... e\n");
}

static void test_yuvsrc(gint argc, gchar * argv[]){

  g_print("\ntest_yuvsrc... s\n");

  App *app = &_app;

  GMainLoop *loop = NULL;
  GstBus *bus;
  guint bus_watch_id;
  gchar *media;

  gst_init(&argc, &argv);

  app->pipeline = gst_pipeline_new("pipeline");

  app->src = gst_element_factory_make("omfyuvsrc", "yuvsrc");
  app->sink = gst_element_factory_make("fakesink", "sink");

  g_object_set(app->src, "num-buffers", _number, "dump", _dump, NULL);
  g_object_set(app->src, "width", 1920, "height", 1080, NULL);
  g_object_set(app->sink, "silent", _silent,  NULL);

  loop = g_main_loop_new(NULL, FALSE);

  bus = gst_element_get_bus(app->pipeline);
  bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
  gst_object_unref(bus);

  gst_bin_add_many(GST_BIN(app->pipeline), app->src, app->sink, NULL);
  gst_element_link_many(app->src, app->sink, NULL);	

  gst_element_set_state(app->pipeline, 	GST_STATE_PLAYING);

  g_object_get(app->src, "media-info", &media, NULL);
  g_print("media-info:%s\n", media);  

  g_main_loop_run(loop);

  gst_element_set_state(app->pipeline, GST_STATE_NULL);
  gst_object_unref(app->pipeline);
  g_source_remove(bus_watch_id);
  g_main_loop_unref(loop);
  
  g_print("test_yuvsrc... e\n");
}

static test_process(gint argc, gchar * argv[]){

#if TEST_YUV_SRC
	test_yuvsrc(argc, argv);
#endif

#if TEST_JPEG_SRC
	test_jpegsrc(argc, argv);
#endif

#if TEST_H264_SRC
	test_h264src(argc, argv);
#endif

#if TEST_H265_SRC
	test_h265src(argc, argv);
#endif

#if TEST_AUD_SRC
	test_audsrc(argc, argv);
#endif
}

gint
main (gint argc, gchar * argv[])
{

  GError *error = NULL;
  GOptionContext *context;
  gchar *line;
  int ret = 1;

  context = g_option_context_new(NULL);
  g_option_context_add_main_entries(context, _entries, NULL);
  g_option_context_add_group(context, gst_init_get_option_group());

  if(!g_option_context_parse(context, &argc, &argv, &error)){
	g_print("Faild to parse option: %s \n\n", error->message);
	g_error_free(error);
  	return ret;
  }

  g_print("test start......\n\n");

  test_process(argc, argv);

  g_print("test end...\n\n");


  return ret;
}

