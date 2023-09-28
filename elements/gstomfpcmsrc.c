/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wim@fluendo.com>
 *
 * gstomfpcmsrc.c:
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
 * SECTION:element-omfpcmsrc
 * @title: omfpcmsrc
 * @see_also: #GstPcmSink
 *
 * The pcmsrc element.
 *
 * ## Example launch line
 * |[
 * gst-launch-1.0 -v omfpcmsrc ! fakesink
 * ]| This pipeline will push pcm buffers to the fakesink element.
 *
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

//#include "gstelements_private.h"
#include "gstomfpcmsrc.h"

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


static GstStaticPadTemplate srctemplate = 
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("audio/x-raw, "
    	"format = (string) S16LE , "
    	"layout = (string) interleaved, "
		"rate = (int) { " SAMPLE_RATES " } , "
		"channels = (int) [ 1, 2 ]"
	));

GST_DEBUG_CATEGORY_STATIC (gst_omf_pcm_src_debug);
#define GST_CAT_DEFAULT gst_omf_pcm_src_debug

/* OmfPcmSrc signals and args */
enum
{
  /* FILL ME */
  SIGNAL_HANDOFF,
  LAST_SIGNAL
};

#define DEFAULT_SIGNAL_HANDOFFS FALSE
#define DEFAULT_SILENT          TRUE
#define DEFAULT_DUMP            FALSE
#define DEFAULT_FORMAT          GST_FORMAT_TIME
#define DEFAULT_SAMPLES			1600
#define DEFAULT_RATE			16000
#define DEFAULT_CHANNEL			1
#define DEFAULT_AEC				OMF_PCM_SRC_AEC_LEVEL_NONE
#define	DEFAULT_ANS				OMF_PCM_SRC_ANS_MODE_NONE
#define	DEFAULT_PREREC_IDX		0
#define DEFAULT_PREREC_PIPE		NULL
#define DEFAULT_CACHE			0


#define DEFAULT_FORMAT_STR 		"S16LE"

enum
{
  PROP_0, 
  PROP_SIGNAL_HANDOFFS,
  PROP_SILENT,
  PROP_DUMP,
  PROP_LAST_MESSAGE,
  PROP_FORMAT,
  ///
  PROP_SAMPELS,
  PROP_RATE,
  PROP_CHANNEL,
  PROP_AEC,
  PROP_ANS,
  PROP_PREREC_IDX,
  PROP_PREREC_PIPE,
  PROP_CACHE,  
  PROP_MEDIA,
  PROP_LAST
};

#define GST_OMF_PCM_AEC_LEVEL (gst_omf_pcm_src_get_aec_level())
static GType
gst_omf_pcm_src_get_aec_level (void)
{
  static GType omfpcmsrc_aec_level_type = 0;
  static const GEnumValue omfpcmsrc_aec_level[] = {
	{ OMF_PCM_SRC_AEC_LEVEL_NONE, "Do not use AEC", "" },
	{ OMF_PCM_SRC_AEC_LEVEL_L, "Level low", "Low" },
	{ OMF_PCM_SRC_AEC_LEVEL_M, "Level middle", "Middle" },
	{ OMF_PCM_SRC_AEC_LEVEL_H, "Level high", "High" },
	{ 0, NULL, NULL },
  };

  if (!omfpcmsrc_aec_level_type) {
	omfpcmsrc_aec_level_type =
		g_enum_register_static ("GstOmfPcmSrcAecLevel", omfpcmsrc_aec_level);
  }
  return omfpcmsrc_aec_level_type;
}
	
#define GST_OMF_PCM_ANS_MODE (gst_omf_pcm_src_get_ans_mode())
static GType
gst_omf_pcm_src_get_ans_mode (void)
{
  static GType omfpcmsrc_ans_mode_type = 0;
  static const GEnumValue omfpcmsrc_ans_mode[] = {
	{ OMF_PCM_SRC_ANS_MODE_NONE, "Do not use ANS", "" },
	{ OMF_PCM_SRC_ANS_MODE_MILD, "Mode mild", "Mild" },
	{ OMF_PCM_SRC_ANS_MODE_MEDIUM, "Mode medium", "Medium" },
	{ OMF_PCM_SRC_ANS_MODE_AGGRESSIVE, "Mode aggresive", "Aggressive" },
	{ 0, NULL, NULL },
  };

  if (!omfpcmsrc_ans_mode_type) {
	omfpcmsrc_ans_mode_type =
		g_enum_register_static ("GstOmfPcmSrcAnsMode", omfpcmsrc_ans_mode);
  }
  return omfpcmsrc_ans_mode_type;
}


#define _do_init \
    GST_DEBUG_CATEGORY_INIT (gst_omf_pcm_src_debug, "omfpcmsrc", 0, "omfpcmsrc element");
#define gst_omf_pcm_src_parent_class parent_class
G_DEFINE_TYPE_WITH_CODE (GstOmfPcmSrc, gst_omf_pcm_src, GST_TYPE_BASE_SRC, _do_init);

static void gst_omf_pcm_src_finalize (GObject * object);
static void gst_omf_pcm_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_omf_pcm_src_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_omf_pcm_src_setcaps (GstBaseSrc * basesrc,
    GstCaps * caps);
static GstCaps *gst_omf_pcm_src_fixate (GstBaseSrc * bsrc, GstCaps * caps);

static gboolean gst_omf_pcm_src_start (GstBaseSrc * basesrc);
static gboolean gst_omf_pcm_src_stop (GstBaseSrc * basesrc);
static gboolean gst_omf_pcm_src_is_seekable (GstBaseSrc * basesrc);

static gboolean gst_omf_pcm_src_event_handler (GstBaseSrc * src, GstEvent * event);
static void gst_omf_pcm_src_get_times (GstBaseSrc * basesrc, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end);
static GstFlowReturn gst_omf_pcm_src_create (GstBaseSrc * src, guint64 offset,
    guint length, GstBuffer ** buf);

static guint gst_omf_pcm_src_signals[LAST_SIGNAL] = { 0 };

static GParamSpec *pspec_last_message = NULL;

static void
gst_omf_pcm_src_class_init (GstOmfPcmSrcClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSrcClass *gstbase_src_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gstelement_class = GST_ELEMENT_CLASS (klass);
  gstbase_src_class = GST_BASE_SRC_CLASS (klass);

  gobject_class->finalize = gst_omf_pcm_src_finalize;

  gobject_class->set_property = gst_omf_pcm_src_set_property;
  gobject_class->get_property = gst_omf_pcm_src_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent",
          "Don't produce last_message events", DEFAULT_SILENT,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_SIGNAL_HANDOFFS,
      g_param_spec_boolean ("signal-handoffs", "Signal handoffs",
          "Send a signal before pushing the buffer", DEFAULT_SIGNAL_HANDOFFS,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_DUMP,
      g_param_spec_boolean ("dump", "Dump", "Dump buffer contents to stdout",
          DEFAULT_DUMP, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  pspec_last_message = g_param_spec_string ("last-message", "last-message",
      "The last status message", NULL,
      G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
  g_object_class_install_property (gobject_class, PROP_LAST_MESSAGE,
      pspec_last_message);

  /*Audio parameters*/
  g_object_class_install_property (gobject_class, PROP_RATE,
      g_param_spec_int ("rate", "Rate", "Pcm samplerate", 8000,
          48000, DEFAULT_RATE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_CHANNEL,
      g_param_spec_int ("channels", "Channels", "Pcm channel", 1,
          2, DEFAULT_CHANNEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_AEC,
      g_param_spec_enum ("aec-level", "AEC level",
          "Automatic echo cancellation level", GST_OMF_PCM_AEC_LEVEL,
          DEFAULT_AEC, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_AEC,
	   g_param_spec_enum ("ans-mode", "ANS mode",
		   "Automatic noise suppression mode", GST_OMF_PCM_ANS_MODE,
		   DEFAULT_ANS, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_PREREC_IDX,
	   g_param_spec_int ("prerec-idx", "Prerecord index", "Link the default prerecord pipeline", 0,
		   1, DEFAULT_PREREC_IDX,
		   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_PREREC_PIPE,
	   g_param_spec_string ("prerec-pipe", "Prerecord pipeline", "Set prerecord pipeline and link it", NULL,
		   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));  
  g_object_class_install_property (gobject_class, PROP_CACHE,
	   g_param_spec_int ("cache", "Cache", "Set cache for frame", 0,
		   1024, DEFAULT_CACHE,
		   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_MEDIA,
	   g_param_spec_string ("media-info", "Media information", "Get media information", NULL,
	   	   G_PARAM_READABLE | G_PARAM_STATIC_STRINGS)); 

  gst_element_class_set_static_metadata (gstelement_class,
      "Omf Pcm Source",
      "Source",
      "Push pcm data out",
      "wang.zhou@icatchtek.com");
  gst_element_class_add_static_pad_template (gstelement_class, &srctemplate);

  gstbase_src_class->set_caps = GST_DEBUG_FUNCPTR (gst_omf_pcm_src_setcaps);
  gstbase_src_class->fixate = GST_DEBUG_FUNCPTR (gst_omf_pcm_src_fixate);
  gstbase_src_class->is_seekable = GST_DEBUG_FUNCPTR (gst_omf_pcm_src_is_seekable);
  gstbase_src_class->start = GST_DEBUG_FUNCPTR (gst_omf_pcm_src_start);
  gstbase_src_class->stop = GST_DEBUG_FUNCPTR (gst_omf_pcm_src_stop);
  gstbase_src_class->event = GST_DEBUG_FUNCPTR (gst_omf_pcm_src_event_handler);
  gstbase_src_class->get_times = GST_DEBUG_FUNCPTR (gst_omf_pcm_src_get_times);
  gstbase_src_class->create = GST_DEBUG_FUNCPTR (gst_omf_pcm_src_create);
}

static void
gst_omf_pcm_src_init (GstOmfPcmSrc * omfpcmsrc)
{
  omfpcmsrc->silent = DEFAULT_SILENT;
  omfpcmsrc->signal_handoffs = DEFAULT_SIGNAL_HANDOFFS;
  omfpcmsrc->dump = DEFAULT_DUMP;
  omfpcmsrc->format = DEFAULT_FORMAT;
  omfpcmsrc->last_message = NULL;

  /* we operate in time */
  gst_base_src_set_format (GST_BASE_SRC (omfpcmsrc), DEFAULT_FORMAT);
  gst_base_src_set_live (GST_BASE_SRC (omfpcmsrc), FALSE);

  omfpcmsrc->samples = DEFAULT_SAMPLES;
  omfpcmsrc->rate = DEFAULT_RATE;
  omfpcmsrc->channels = DEFAULT_CHANNEL;
  omfpcmsrc->aec = DEFAULT_AEC;
  omfpcmsrc->ans = DEFAULT_ANS;
  omfpcmsrc->prerecidx = DEFAULT_PREREC_IDX;
  omfpcmsrc->prerecpipe = DEFAULT_PREREC_PIPE ? strdup(DEFAULT_PREREC_PIPE) : NULL;
  omfpcmsrc->cache = DEFAULT_CACHE;
  omfpcmsrc->spts_ns = -1;
  omfpcmsrc->lpts_ns = -1;
  omfpcmsrc->media = NULL;

  omfpcmsrc->omf_hd = OmfPcmSrcCreate();

}

static GstCaps *
gst_omf_pcm_src_fixate (GstBaseSrc * bsrc, GstCaps * caps)
{
  GstOmfPcmSrc *src = GST_OMF_PCM_SRC (bsrc);
  GstStructure *structure;
  gint channels;

  caps = gst_caps_make_writable (caps);
  
  structure = gst_caps_get_structure (caps, 0);

  gst_structure_fixate_field_nearest_int (structure, "rate", src->rate);

  gst_structure_fixate_field_string (structure, "format", DEFAULT_FORMAT_STR);
  
  gst_structure_fixate_field_nearest_int (structure, "channels", src->channels);  

  if (gst_structure_get_int (structure, "channels", &channels) && channels > 2) {
    if (!gst_structure_has_field_typed (structure, "channel-mask",
            GST_TYPE_BITMASK))
      gst_structure_set (structure, "channel-mask", GST_TYPE_BITMASK, 0ULL,
          NULL);
  }

  caps = GST_BASE_SRC_CLASS (parent_class)->fixate (bsrc, caps);

  return caps;
}

static gboolean
gst_omf_pcm_src_setcaps (GstBaseSrc * bsrc, GstCaps * caps)
{
  GstOmfPcmSrc *src = GST_OMF_PCM_SRC (bsrc);

  GST_DEBUG_OBJECT (src, "negotiated to caps %" GST_PTR_FORMAT, caps);

  return TRUE;

  /* ERROR */
invalid_caps:
  {
	GST_ERROR_OBJECT (bsrc, "received invalid caps");
	return FALSE;
  }

}

static void
gst_omf_pcm_src_finalize (GObject * object)
{
  GstOmfPcmSrc *src;

  src = GST_OMF_PCM_SRC (object);

  g_free (src->last_message);

  if(src->omf_hd){
	 OmfPcmSrcDestory(src->omf_hd);
  }

  g_free(src->prerecpipe);
  src->prerecpipe = NULL;

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gboolean
gst_omf_pcm_src_event_handler (GstBaseSrc * basesrc, GstEvent * event)
{
  GstOmfPcmSrc *src;

  src = GST_OMF_PCM_SRC (basesrc);

  if (!src->silent) {
    const GstStructure *s;
    const gchar *tstr;
    gchar *sstr;

    GST_OBJECT_LOCK (src);
    g_free (src->last_message);

    tstr = gst_event_type_get_name (GST_EVENT_TYPE (event));

    if ((s = gst_event_get_structure (event)))
      sstr = gst_structure_to_string (s);
    else
      sstr = g_strdup ("");

    src->last_message =
        g_strdup_printf ("event   ******* (%s:%s) E (type: %s (%d), %s) %p",
        GST_DEBUG_PAD_NAME (GST_BASE_SRC_CAST (src)->srcpad),
        tstr, GST_EVENT_TYPE (event), sstr, event);
    g_free (sstr);
    GST_OBJECT_UNLOCK (src);

    g_object_notify_by_pspec ((GObject *) src, pspec_last_message);
  }

  return GST_BASE_SRC_CLASS (parent_class)->event (basesrc, event);
}

static void
gst_omf_pcm_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstOmfPcmSrc *src;
  GstBaseSrc *basesrc;

  src = GST_OMF_PCM_SRC (object);
  basesrc = GST_BASE_SRC (object);

  switch (prop_id) {
    case PROP_SILENT:
      src->silent = g_value_get_boolean (value);
      break;
    case PROP_SIGNAL_HANDOFFS:
      src->signal_handoffs = g_value_get_boolean (value);
      break;
    case PROP_DUMP:
      src->dump = g_value_get_boolean (value);
      break;
	case PROP_SAMPELS:
		src->samples = g_value_get_int(value);
		break;
	case PROP_RATE:
		src->rate = g_value_get_int(value);
		break;
	case PROP_CHANNEL:
		src->channels = g_value_get_int(value);
		break;
	case PROP_AEC:
		src->aec = (GstOmfPcmSrcAecLevel)g_value_get_enum(value);
		break;
	case PROP_ANS:
		src->ans = (GstOmfPcmSrcAnsMode)g_value_get_enum(value);
		break;
	case PROP_PREREC_IDX:
		src->prerecidx = g_value_get_int(value);
		break;
	case PROP_PREREC_PIPE:
		g_free(src->prerecpipe);
		src->prerecpipe = g_strdup(g_value_get_string(value));
		break;
	case PROP_CACHE:
		src->cache = g_value_get_int(value);
		break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_omf_pcm_src_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstOmfPcmSrc *src;
  GstBaseSrc *basesrc;

  g_return_if_fail (GST_IS_OMF_PCM_SRC (object));

  src = GST_OMF_PCM_SRC (object);
  basesrc = GST_BASE_SRC (object);

  switch (prop_id) {
    case PROP_SILENT:
      	g_value_set_boolean (value, src->silent);
      	break;
    case PROP_SIGNAL_HANDOFFS:
      	g_value_set_boolean (value, src->signal_handoffs);
      	break;
    case PROP_DUMP:
      	g_value_set_boolean (value, src->dump);
      	break;
    case PROP_LAST_MESSAGE:
      	GST_OBJECT_LOCK (src);
      	g_value_set_string (value, src->last_message);
      	GST_OBJECT_UNLOCK (src);
      	break;
	case PROP_SAMPELS:
		g_value_set_int(value, src->samples);
		break;
	case PROP_RATE:
		g_value_set_int(value, src->rate);
		break;
	case PROP_CHANNEL:
		g_value_set_int(value, src->channels);
		break;
	case PROP_AEC:
		g_value_set_enum(value, src->aec);
		break;
	case PROP_ANS:
		g_value_set_enum(value, src->ans);
		break;
	case PROP_PREREC_IDX:
		g_value_set_int(value, src->prerecidx);
		break;
	case PROP_PREREC_PIPE:
		g_value_set_string(value, src->prerecpipe);
		break;
	case PROP_CACHE:
		g_value_set_int(value, src->cache);
		break;
	case PROP_MEDIA:
		g_value_set_string(value, src->media);
		break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void data_free(void* data){
	if(data) free(data);
}	

static GstBuffer *
gst_omf_pcm_src_alloc_buffer (GstOmfPcmSrc * src, guint size, gpointer pdata, GDestroyNotify pfree)
{
  GstBuffer *buf;
  gpointer data;
  gboolean do_prepare = FALSE;

  buf = gst_buffer_new ();

  if (size != 0) {
  	#if 0
    data = g_malloc (size);
    gst_buffer_append_memory (buf,
        gst_memory_new_wrapped (0, data, size, 0, size, data, g_free));
	#else
	 gst_buffer_append_memory (buf,
        gst_memory_new_wrapped (0, pdata, size, 0, size, pdata, pfree));
	#endif
  }

  return buf;
}

static void
gst_omf_pcm_src_get_times (GstBaseSrc * basesrc, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end)
{
  GstOmfPcmSrc *src;

  src = GST_OMF_PCM_SRC (basesrc);
#if 0
  /* sync on the timestamp of the buffer if requested. */
  if (src->sync) {
    GstClockTime timestamp, duration;

    /* first sync on DTS, else use PTS */
    timestamp = GST_BUFFER_DTS (buffer);
    if (!GST_CLOCK_TIME_IS_VALID (timestamp))
      timestamp = GST_BUFFER_PTS (buffer);

    if (GST_CLOCK_TIME_IS_VALID (timestamp)) {
      /* get duration to calculate end time */
      duration = GST_BUFFER_DURATION (buffer);
      if (GST_CLOCK_TIME_IS_VALID (duration)) {
        *end = timestamp + duration;
      }
      *start = timestamp;
    }
  } else {
    *start = -1;
    *end = -1;
  }
#endif  
}

static GstFlowReturn
gst_omf_pcm_src_create (GstBaseSrc * basesrc, guint64 offset, guint length,
    GstBuffer ** ret)
{
  GstOmfPcmSrc *src;
  GstBuffer *buf;
  GstMapInfo info;
  GstClockTime time;
  gsize size = 0;

  src = GST_OMF_PCM_SRC (basesrc);

  OmfFrameC_t frame;
  memset(&frame, 0, sizeof(OmfFrameC_t));
  
  if(OmfPcmSrcGetFrame(src->omf_hd, &frame)){
  	if(frame.data){
	  size = frame.size;
	 
	  buf =  gst_omf_pcm_src_alloc_buffer (src, size, frame.data, frame.free);
	  if(buf){
	  	if(src->spts_ns == -1){
			src->spts_ns = frame.pts_ns;
		}
		///
		GST_BUFFER_PTS(buf) = frame.pts_ns - src->spts_ns;
		GST_BUFFER_DTS(buf) = GST_CLOCK_TIME_NONE;
		
	  	GST_BUFFER_OFFSET(buf) = frame.index;
		GST_BUFFER_OFFSET_END(buf) = frame.index + 1;

		GST_BUFFER_DURATION (buf) = src->samples * 1000000000 / src->rate;

		GST_MINI_OBJECT_CAST (buf)->flags = frame.iskeyframe;

    	gst_object_sync_values (GST_OBJECT (src), GST_BUFFER_TIMESTAMP (buf));
		
	  }
	}	
  }
  
  if (!src->silent) {
    gchar dts_str[64], pts_str[64], dur_str[64];

    GST_OBJECT_LOCK (src);
    g_free (src->last_message);

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

    src->last_message =
        g_strdup_printf ("create   ******* (%s:%s) (%u bytes, dts: %s, pts:%s"
        ", duration: %s, offset: %" G_GINT64_FORMAT ", offset_end: %"
        G_GINT64_FORMAT ", flags: %08x ) %p",
        GST_DEBUG_PAD_NAME (GST_BASE_SRC_CAST (src)->srcpad), (guint) size,
        dts_str, pts_str, dur_str, GST_BUFFER_OFFSET (buf),
        GST_BUFFER_OFFSET_END (buf), GST_MINI_OBJECT_CAST (buf)->flags,
         buf);
	
    GST_OBJECT_UNLOCK (src);

    g_object_notify_by_pspec ((GObject *) src, pspec_last_message);
  }

  if (src->signal_handoffs) {
    GST_LOG_OBJECT (src, "pre handoff emit");
    g_signal_emit (src, gst_omf_pcm_src_signals[SIGNAL_HANDOFF], 0, buf,
        basesrc->srcpad);
    GST_LOG_OBJECT (src, "post handoff emit");
  }

  if (src->dump) {
    if (gst_buffer_map (buf, &info, GST_MAP_READ)) {
      gst_util_dump_mem (info.data, 16);
      gst_buffer_unmap (buf, &info);
    }
  }
  
  src->bytes_sent += size;

  *ret = buf;
  
  return GST_FLOW_OK;
}

static gboolean
gst_omf_pcm_src_start (GstBaseSrc * basesrc)
{
  GstOmfPcmSrc *src;

  src = GST_OMF_PCM_SRC (basesrc);

  src->bytes_sent = 0;

  gst_base_src_set_format (basesrc, src->format);

  g_return_val_if_fail(src->omf_hd, FALSE);

  OmfPcmSrcSetMicSamples(src->omf_hd, src->samples);	
  OmfPcmSrcSetSampleRate(src->omf_hd, src->rate);
  OmfPcmSrcSetChannel(src->omf_hd, src->channels);

  if(src->prerecidx){
  	OmfPcmSrcSetPreRecord(src->omf_hd, src->prerecidx);
  }		
  if(src->prerecpipe){
	OmfPcmSrcSetPreRecordPipe(src->omf_hd, src->prerecpipe);
  }
  if(src->aec != OMF_PCM_SRC_AEC_LEVEL_NONE){
	OmfPcmSrcSetAEC(src->omf_hd, src->aec);	
  }
  if(src->ans != OMF_PCM_SRC_ANS_MODE_NONE){
	OmfPcmSrcSetANS(src->omf_hd, src->ans);
  }
  if(src->cache){
	OmfPcmSrcSetCache(src->omf_hd, src->cache);
  }
	
  OmfPcmSrcGetMediaInfo(src->omf_hd, &src->media);
   
  return OmfPcmSrcStatusUp(src->omf_hd, OMF_STATE_PLAYING);
}

static gboolean
gst_omf_pcm_src_stop (GstBaseSrc * basesrc)
{
  GstOmfPcmSrc *src;

  src = GST_OMF_PCM_SRC (basesrc);

  GST_OBJECT_LOCK (src);

  g_free (src->last_message);
  src->last_message = NULL;
  GST_OBJECT_UNLOCK (src);

  if(src->omf_hd){
  	OmfPcmSrcStatusDown(src->omf_hd, OMF_STATE_NULL);
  }

  return TRUE;
}

static gboolean
gst_omf_pcm_src_is_seekable (GstBaseSrc * basesrc)
{
  GstOmfPcmSrc *src = GST_OMF_PCM_SRC (basesrc);

  return FALSE;
}
