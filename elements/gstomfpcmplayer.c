/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2005 Wim Taymans <wim@fluendo.com>
 *
 * gstomfpcmplayer.c:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
/**
 * SECTION:element-omfpcmplayer
 * @title: omfpcmplayer
 * @see_also: #GstOmfPcmPlayer
 *
 * Playing audio data.
 *
 * ## Example launch line
 * |[
 * gst-launch-1.0 audiotestsrc ! omfpcmplayer media=pcm
 * ]| Get data from test src to play.
 *
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

//#include "gstelements_private.h"
#include "gstomfpcmplayer.h"
#include <string.h>

#define SAMPLE_RATES " 8000, " \
                    "11025, " \
                    "12000, " \
                    "16000, " \
                    "22050, " \
                    "24000, " \
                    "32000, " \
                    "44100, " \
                    "48000, " \
                    "64000, " \
                    "88200, " \
                    "96000"


static GstStaticPadTemplate sinktemplate =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("audio/x-raw, "
    	"format = (string) S16LE , "
    	"layout = (string) interleaved, "
		"rate = (int) { " SAMPLE_RATES " } , "
		"channels = (int) [ 1, 2 ]"
	));

GST_DEBUG_CATEGORY_STATIC (gst_omf_pcm_player_debug);
#define GST_CAT_DEFAULT gst_omf_pcm_player_debug

/* OmfPcmPlayer signals and args */
enum
{
  /* FILL ME */
  SIGNAL_HANDOFF,
  SIGNAL_PREROLL_HANDOFF,
  LAST_SIGNAL
};

#define DEFAULT_SYNC 				FALSE
#define DEFAULT_DROP_OUT_OF_SEGMENT TRUE

#define DEFAULT_SILENT 				TRUE
#define DEFAULT_DUMP 				FALSE
#define DEFAULT_SIGNAL_HANDOFFS 	FALSE
#define DEFAULT_LAST_MESSAGE 		NULL
#define DEFAULT_RATE				16000
#define DEFAULT_CHANNEL				1
#define DEFAULT_LIVE_LIMIT			NULL

enum
{
  PROP_0,
  PROP_STATE_ERROR,
  PROP_SILENT,
  PROP_DUMP,
  PROP_SIGNAL_HANDOFFS,
  PROP_DROP_OUT_OF_SEGMENT,
  PROP_LAST_MESSAGE,
  PROP_RATE,
  PROP_CHANNEL,
  PROP_LIVE_LIMIT,
  PROP_MEDIA,
  PROP_LAST
};

#define _do_init \
    GST_DEBUG_CATEGORY_INIT (gst_omf_pcm_player_debug, "omfpcmplayer", 0, "omf pcm player element");
#define gst_omf_pcm_player_parent_class parent_class
G_DEFINE_TYPE_WITH_CODE (GstOmfPcmPlayer, gst_omf_pcm_player, GST_TYPE_BASE_SINK,
    _do_init);

static void gst_omf_pcm_player_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_omf_pcm_player_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);
static void gst_omf_pcm_player_finalize (GObject * obj);
static gboolean gst_omf_pcm_player_start (GstBaseSink * bsink);
static gboolean gst_omf_pcm_player_stop (GstBaseSink * bsink);

static GstFlowReturn gst_omf_pcm_player_preroll (GstBaseSink * bsink,
    GstBuffer * buffer);
static GstFlowReturn gst_omf_pcm_player_render (GstBaseSink * bsink,
    GstBuffer * buffer);
static gboolean gst_omf_pcm_player_event (GstBaseSink * bsink, GstEvent * event);
static guint gst_omf_pcm_player_signals[LAST_SIGNAL] = { 0 };

static GParamSpec *pspec_last_message = NULL;

static void
gst_omf_pcm_player_class_init (GstOmfPcmPlayerClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSinkClass *gstbase_sink_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gstelement_class = GST_ELEMENT_CLASS (klass);
  gstbase_sink_class = GST_BASE_SINK_CLASS (klass);

  gobject_class->set_property = gst_omf_pcm_player_set_property;
  gobject_class->get_property = gst_omf_pcm_player_get_property;
  gobject_class->finalize = gst_omf_pcm_player_finalize;

  pspec_last_message = g_param_spec_string ("last-message", "Last Message",
      "The message describing current status", DEFAULT_LAST_MESSAGE,
      G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
  g_object_class_install_property (gobject_class, PROP_LAST_MESSAGE,
      pspec_last_message);
  g_object_class_install_property (gobject_class, PROP_SIGNAL_HANDOFFS,
      g_param_spec_boolean ("signal-handoffs", "Signal handoffs",
          "Send a signal before unreffing the buffer", DEFAULT_SIGNAL_HANDOFFS,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_DROP_OUT_OF_SEGMENT,
      g_param_spec_boolean ("drop-out-of-segment",
          "Drop out-of-segment buffers",
          "Drop and don't render / hand off out-of-segment buffers",
          DEFAULT_DROP_OUT_OF_SEGMENT,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent",
          "Don't produce last_message events", DEFAULT_SILENT,
          G_PARAM_READWRITE | GST_PARAM_MUTABLE_PLAYING |
          G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_DUMP,
      g_param_spec_boolean ("dump", "Dump", "Dump buffer contents to stdout",
          DEFAULT_DUMP,
          G_PARAM_READWRITE | GST_PARAM_MUTABLE_PLAYING |
          G_PARAM_STATIC_STRINGS));

  /**
   * GstOmfPcmPlayer::handoff:
   * @omfpcmplayer: the omfpcmplayer instance
   * @buffer: the buffer that just has been received
   * @pad: the pad that received it
   *
   * This signal gets emitted before unreffing the buffer.
   */
  gst_omf_pcm_player_signals[SIGNAL_HANDOFF] =
      g_signal_new ("handoff", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_LAST,
      G_STRUCT_OFFSET (GstOmfPcmPlayerClass, handoff), NULL, NULL,
      g_cclosure_marshal_generic, G_TYPE_NONE, 2,
      GST_TYPE_BUFFER | G_SIGNAL_TYPE_STATIC_SCOPE, GST_TYPE_PAD);

  /**
   * GstOmfPcmPlayer::preroll-handoff:
   * @omfpcmplayer: the omfpcmplayer instance
   * @buffer: the buffer that just has been received
   * @pad: the pad that received it
   *
   * This signal gets emitted before unreffing the buffer.
   */
  gst_omf_pcm_player_signals[SIGNAL_PREROLL_HANDOFF] =
      g_signal_new ("preroll-handoff", G_TYPE_FROM_CLASS (klass),
      G_SIGNAL_RUN_LAST, G_STRUCT_OFFSET (GstOmfPcmPlayerClass, preroll_handoff),
      NULL, NULL, g_cclosure_marshal_generic, G_TYPE_NONE, 2,
      GST_TYPE_BUFFER | G_SIGNAL_TYPE_STATIC_SCOPE, GST_TYPE_PAD);

  //pcmio player param
  g_object_class_install_property (gobject_class, PROP_RATE,
      g_param_spec_int ("rate", "Rate", "Pcm samplerate", 8000,
          48000, DEFAULT_RATE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_CHANNEL,
     g_param_spec_int ("channel", "Channel", "Pcm channel", 1,
          2, DEFAULT_CHANNEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_LIVE_LIMIT,
	 g_param_spec_string ("live-limit", "live delay limit", "Limit the play delay when live streaming, optional:\n"
	  													"\t\t\t   (1): g1={duration=300,speed=1.2},g2={duration=600,speed=1.5}", NULL,
		  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_MEDIA,
	 g_param_spec_string ("media-info", "Media information", "Get media information", NULL,
	      G_PARAM_READABLE | G_PARAM_STATIC_STRINGS)); 

  gst_element_class_set_static_metadata (gstelement_class,
      "Omf Pcm Player",
      "Sink",
      "player for pcm",
      "wang.zhou@icatchtek.com");

  gst_element_class_add_static_pad_template (gstelement_class, &sinktemplate);
  
  gstbase_sink_class->start = GST_DEBUG_FUNCPTR (gst_omf_pcm_player_start);
  gstbase_sink_class->stop = GST_DEBUG_FUNCPTR (gst_omf_pcm_player_stop);
  gstbase_sink_class->event = GST_DEBUG_FUNCPTR (gst_omf_pcm_player_event);
  gstbase_sink_class->preroll = GST_DEBUG_FUNCPTR (gst_omf_pcm_player_preroll);
  gstbase_sink_class->render = GST_DEBUG_FUNCPTR (gst_omf_pcm_player_render);
}

static void
gst_omf_pcm_player_init (GstOmfPcmPlayer * sink)
{
  sink->silent = DEFAULT_SILENT;
  sink->dump = DEFAULT_DUMP;
  sink->last_message = g_strdup (DEFAULT_LAST_MESSAGE);
  sink->signal_handoffs = DEFAULT_SIGNAL_HANDOFFS;

  gst_base_sink_set_sync (GST_BASE_SINK (sink), DEFAULT_SYNC);
  gst_base_sink_set_drop_out_of_segment (GST_BASE_SINK (sink),
      DEFAULT_DROP_OUT_OF_SEGMENT);

  sink->rate = DEFAULT_RATE;
  sink->channel = DEFAULT_CHANNEL;
  sink->liveLimit = DEFAULT_LIVE_LIMIT ? g_strdup(DEFAULT_LIVE_LIMIT) : NULL;
  sink->media = NULL;

  sink->omf_hd = OmfAudPlayerCreate();
}

static void
gst_omf_pcm_player_finalize (GObject * obj)
{
  GstOmfPcmPlayer *sink;
  
  sink = GST_OMF_PCM_PLAYER (obj);

  if(sink->omf_hd){
	 OmfAudPlayerDestory(sink->omf_hd);
  }

  g_free( sink->liveLimit);
  sink->liveLimit = NULL;

  G_OBJECT_CLASS (parent_class)->finalize (obj);
}

static void
gst_omf_pcm_player_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstOmfPcmPlayer *sink;

  sink = GST_OMF_PCM_PLAYER (object);

  switch (prop_id) {
    case PROP_SILENT:
      sink->silent = g_value_get_boolean (value);
      break;
    case PROP_DUMP:
      sink->dump = g_value_get_boolean (value);
      break;
    case PROP_SIGNAL_HANDOFFS:
      sink->signal_handoffs = g_value_get_boolean (value);
      break;
    case PROP_DROP_OUT_OF_SEGMENT:
      gst_base_sink_set_drop_out_of_segment (GST_BASE_SINK (object),
          g_value_get_boolean (value));
      break;
    case PROP_RATE:
		sink->rate = g_value_get_int(value);
		break;
	case PROP_CHANNEL:
		sink->channel = g_value_get_int(value);
		break;
	case PROP_LIVE_LIMIT:
		g_free (sink->liveLimit);
		sink->liveLimit = g_strdup(g_value_get_string(value));
		break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_omf_pcm_player_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstOmfPcmPlayer *sink;

  sink = GST_OMF_PCM_PLAYER (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, sink->silent);
      break;
    case PROP_DUMP:
      g_value_set_boolean (value, sink->dump);
      break;
    case PROP_SIGNAL_HANDOFFS:
      g_value_set_boolean (value, sink->signal_handoffs);
      break;
    case PROP_DROP_OUT_OF_SEGMENT:
      g_value_set_boolean (value,
          gst_base_sink_get_drop_out_of_segment (GST_BASE_SINK (object)));
      break;
    case PROP_LAST_MESSAGE:
      GST_OBJECT_LOCK (sink);
      g_value_set_string (value, sink->last_message);
      GST_OBJECT_UNLOCK (sink);
      break;
    case PROP_RATE:
		g_value_set_int(value, sink->rate);
		break;
	case PROP_CHANNEL:
		g_value_set_int(value, sink->channel);
		break;
	case PROP_LIVE_LIMIT:
		g_value_set_string(value, sink->liveLimit);
		break;
	case PROP_MEDIA:
		g_value_set_string(value, sink->media);
		break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_omf_pcm_player_notify_last_message (GstOmfPcmPlayer * sink)
{
  g_object_notify_by_pspec ((GObject *) sink, pspec_last_message);
}

static gboolean
gst_omf_pcm_player_event (GstBaseSink * bsink, GstEvent * event)
{
  GstOmfPcmPlayer *sink = GST_OMF_PCM_PLAYER (bsink);

  if (!sink->silent) {
    const GstStructure *s;
    const gchar *tstr;
    gchar *sstr;

    GST_OBJECT_LOCK (sink);
    g_free (sink->last_message);

    if (GST_EVENT_TYPE (event) == GST_EVENT_SINK_MESSAGE) {
      GstMessage *msg;
      const GstStructure *structure;

      gst_event_parse_sink_message (event, &msg);
      structure = gst_message_get_structure (msg);
      sstr = gst_structure_to_string (structure);
      sink->last_message =
          g_strdup_printf ("message ******* (%s:%s) M (type: %d, %s) %p",
          GST_DEBUG_PAD_NAME (GST_BASE_SINK_CAST (sink)->sinkpad),
          GST_MESSAGE_TYPE (msg), sstr, msg);
      gst_message_unref (msg);
    } else {
      tstr = gst_event_type_get_name (GST_EVENT_TYPE (event));

      if ((s = gst_event_get_structure (event))) {
        sstr = gst_structure_to_string (s);
      } else {
        sstr = g_strdup ("");
      }

      sink->last_message =
          g_strdup_printf ("event   ******* (%s:%s) E (type: %s (%d), %s) %p",
          GST_DEBUG_PAD_NAME (GST_BASE_SINK_CAST (sink)->sinkpad),
          tstr, GST_EVENT_TYPE (event), sstr, event);
    }
    g_free (sstr);
    GST_OBJECT_UNLOCK (sink);

    gst_omf_pcm_player_notify_last_message (sink);
  }

  return GST_BASE_SINK_CLASS (parent_class)->event (bsink, event);
}

static GstFlowReturn
gst_omf_pcm_player_preroll (GstBaseSink * bsink, GstBuffer * buffer)
{
  GstOmfPcmPlayer *sink = GST_OMF_PCM_PLAYER (bsink);

  if (!sink->silent) {
    GST_OBJECT_LOCK (sink);
    g_free (sink->last_message);

    sink->last_message = g_strdup_printf ("preroll   ******* ");
    GST_OBJECT_UNLOCK (sink);

    gst_omf_pcm_player_notify_last_message (sink);
  }
  if (sink->signal_handoffs) {
    g_signal_emit (sink,
        gst_omf_pcm_player_signals[SIGNAL_PREROLL_HANDOFF], 0, buffer,
        bsink->sinkpad);
  }
  return GST_FLOW_OK;

  /* ERRORS */
eos:
  {
    GST_DEBUG_OBJECT (sink, "we are EOS");
    return GST_FLOW_EOS;
  }
}

static GstFlowReturn
gst_omf_pcm_player_render (GstBaseSink * bsink, GstBuffer * buf)
{
  GstOmfPcmPlayer *sink = GST_OMF_PCM_PLAYER_CAST (bsink);

  if(sink->omf_hd && buf){
	OmfFrameC_t frame;
	memset(&frame, 0, sizeof(OmfFrameC_t));

	GstMapInfo info;
	gst_buffer_map(buf, &info, GST_MAP_READ);
	frame.data = info.data;
	frame.size = info.size;
	//memcpy(frame.data, info.data, info.size);
	gst_buffer_unmap(buf, &info);

	frame.pts_ns = GST_BUFFER_PTS(buf);
	frame.iskeyframe = TRUE;
  
	OmfAudPlayerPushFrame(sink->omf_hd, &frame);
  }

  if (!sink->silent) {
    gchar dts_str[64], pts_str[64], dur_str[64];
    gchar *meta_str;

    GST_OBJECT_LOCK (sink);
    g_free (sink->last_message);

    if (GST_BUFFER_DTS (buf) != GST_CLOCK_TIME_NONE) {
      g_snprintf (dts_str, sizeof (dts_str), "%" GST_TIME_FORMAT,
          GST_TIME_ARGS (GST_BUFFER_DTS (buf)));
    } else {
      g_strlcpy (dts_str, "none", sizeof (dts_str));
    }

    if (GST_BUFFER_PTS (buf) != GST_CLOCK_TIME_NONE) {
      g_snprintf (pts_str, sizeof (pts_str), "%" GST_TIME_FORMAT,
          GST_TIME_ARGS (GST_BUFFER_PTS (buf)));
    } else {
      g_strlcpy (pts_str, "none", sizeof (pts_str));
    }

    if (GST_BUFFER_DURATION (buf) != GST_CLOCK_TIME_NONE) {
      g_snprintf (dur_str, sizeof (dur_str), "%" GST_TIME_FORMAT,
          GST_TIME_ARGS (GST_BUFFER_DURATION (buf)));
    } else {
      g_strlcpy (dur_str, "none", sizeof (dur_str));
    }

	meta_str = (gchar*)sink->media;
    sink->last_message =
        g_strdup_printf ("chain   ******* (%s:%s) (%u bytes, dts: %s, pts: %s"
        ", duration: %s, offset: %" G_GINT64_FORMAT ", offset_end: %"
        G_GINT64_FORMAT ", flags: %08x , meta: %s) %p",
        GST_DEBUG_PAD_NAME (GST_BASE_SINK_CAST (sink)->sinkpad),
        (guint) gst_buffer_get_size (buf), dts_str, pts_str,
        dur_str, GST_BUFFER_OFFSET (buf), GST_BUFFER_OFFSET_END (buf),
        GST_MINI_OBJECT_CAST (buf)->flags, 
        meta_str ? meta_str : "none", buf);
 
    g_free (meta_str);
    GST_OBJECT_UNLOCK (sink);

    gst_omf_pcm_player_notify_last_message (sink);
  }
  if (sink->signal_handoffs)
    g_signal_emit (sink, gst_omf_pcm_player_signals[SIGNAL_HANDOFF], 0, buf,
        bsink->sinkpad);

  if (sink->dump) {
    GstMapInfo info;

    if (gst_buffer_map (buf, &info, GST_MAP_READ)) {
      gst_util_dump_mem (info.data, 16);
      gst_buffer_unmap (buf, &info);
    }
  }

  return GST_FLOW_OK;

  /* ERRORS */
eos:
  {
    GST_DEBUG_OBJECT (sink, "we are EOS");
    return GST_FLOW_EOS;
  }
}

static gboolean
gst_omf_pcm_player_start (GstBaseSink * bsink)
{
  GstOmfPcmPlayer *sink;

  sink = GST_OMF_PCM_PLAYER (bsink);

  g_return_val_if_fail(sink->omf_hd, FALSE);

  OmfAudPlayerSetSampleRate(sink->omf_hd, sink->rate);
  OmfAudPlayerSetChannel(sink->omf_hd, sink->channel);
  OmfAudPlayerSetMediaInfo(sink->omf_hd, "pcm");
 
  if(sink->liveLimit){
	OmfAudPlayerSetLiveDelayLimit(sink->omf_hd, sink->liveLimit);
  }

  OmfAudPlayerGetMediaInfo(sink->omf_hd, &sink->media);
   
  return OmfAudPlayerStatusUp(sink->omf_hd, OMF_STATE_PLAYING);
}

static gboolean
gst_omf_pcm_player_stop (GstBaseSink * bsink)
{
  GstOmfPcmPlayer *sink;

  sink = GST_OMF_PCM_PLAYER (bsink);

  GST_OBJECT_LOCK (sink);

  g_free (sink->last_message);
  sink->last_message = NULL;
  GST_OBJECT_UNLOCK (sink);

  if(sink->omf_hd){
  	OmfAudPlayerStatusDown(sink->omf_hd, OMF_STATE_NULL);
  }

  return TRUE;
}
