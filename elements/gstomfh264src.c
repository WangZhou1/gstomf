/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wim@fluendo.com>
 *
 * gstomfh264src.c:
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
 * SECTION:element-omfh264src
 * @title: omfh264src
 * @see_also: #GstAudSrc
 *
 * The omfh264src element is a h264 source element.
 *
 * ## Example launch line
 * |[
 * gst-launch-1.0 -v omfh264src goptype=IPPP gop=30 ! fakesink
 * ]| This pipeline will send h264 to the fakesink element.
 *
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

//#include "gstelements_private.h"
#include "gstomfh264src.h"

static GstStaticPadTemplate srctemplate = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("video/x-h264,"
		"width = (int) [ 480, 4096 ],"
		"height = (int) [ 320, 2160 ],"
		"framerate = (int) [ 1, 60 ]"
	));

GST_DEBUG_CATEGORY_STATIC (gst_omf_h264_src_debug);
#define GST_CAT_DEFAULT gst_omf_h264_src_debug

/* OmfH264Src signals and args */
enum
{
  /* FILL ME */
  SIGNAL_HANDOFF,
  LAST_SIGNAL
};

#define DEFAULT_SIGNAL_HANDOFFS FALSE
#define DEFAULT_SILENT          TRUE
#define DEFAULT_DUMP            FALSE
#define DEFAULT_FORMAT          GST_FORMAT_BYTES
#define DEFAULT_SENID			0
#define DEFAULT_WIDTH			1920
#define DEFAULT_HEIGHT			1080
#define	DEFAULT_FRAMERATE		30
#define DEFAULT_BITRATE			2000000
#define DEFAULT_GOP				15
#define DEFAULT_GOPTYPE			OMF_H264_SRC_GOP_TYPE_IPPP
#define DEFAULT_CODEC			NULL
#define	DEFAULT_PREREC_IDX		0
#define DEFAULT_PREREC_PIPE		NULL
#define DEFAULT_SHARE			0
#define DEFAULT_CACHE			0
#define DEFAULT_LOW_BW			FALSE

enum
{
  PROP_0, 
  PROP_SIGNAL_HANDOFFS,
  PROP_SILENT,
  PROP_DUMP,
  PROP_LAST_MESSAGE,
  PROP_FORMAT,
  ///
  PROP_SENID,
  PROP_WIDTH,
  PROP_HEIGHT,
  PROP_FRAMERATE,
  PROP_BITRATE,
  PROP_GOP,
  PROP_GOPTYPE,
  PROP_CODEC,
  PROP_PREREC_IDX,
  PROP_PREREC_PIPE,
  PROP_SHARE,
  PROP_CACHE,  
  PROP_LOW_BW,
  PROP_MEDIA,
  PROP_LAST,
};

#define GST_OMF_H264_GOP_TYPE (gst_omf_h264_src_get_gop_type())
static GType
gst_omf_h264_src_get_gop_type (void)
{
  static GType omfh264src_gop_type_type = 0;
  static const GEnumValue omfh264src_gop_type[] = {
	{OMF_H264_SRC_GOP_TYPE_IBBP, "Gop type IBBP", "IBBP"},
	{OMF_H264_SRC_GOP_TYPE_IPPP, "Gop type IPPP", "IPPP"},
	{OMF_H264_SRC_GOP_TYPE_IIII, "Gop type IIII", "IIII"},
	{0, NULL, NULL},
  };

  if (!omfh264src_gop_type_type) {
	omfh264src_gop_type_type =
		g_enum_register_static ("GstOmfH264SrcGopType", omfh264src_gop_type);
  }
  return omfh264src_gop_type_type;
}

#define _do_init \
    GST_DEBUG_CATEGORY_INIT (gst_omf_h264_src_debug, "omfh264src", 0, "omfh264src element");
#define gst_omf_h264_src_parent_class parent_class
G_DEFINE_TYPE_WITH_CODE (GstOmfH264Src, gst_omf_h264_src, GST_TYPE_BASE_SRC, _do_init);

static void gst_omf_h264_src_finalize (GObject * object);
static void gst_omf_h264_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_omf_h264_src_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_omf_h264_src_start (GstBaseSrc * basesrc);
static gboolean gst_omf_h264_src_stop (GstBaseSrc * basesrc);
static gboolean gst_omf_h264_src_is_seekable (GstBaseSrc * basesrc);

static gboolean gst_omf_h264_src_event_handler (GstBaseSrc * src, GstEvent * event);
static void gst_omf_h264_src_get_times (GstBaseSrc * basesrc, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end);
static GstFlowReturn gst_omf_h264_src_create (GstBaseSrc * src, guint64 offset,
    guint length, GstBuffer ** buf);

static guint gst_omf_h264_src_signals[LAST_SIGNAL] = { 0 };

static GParamSpec *pspec_last_message = NULL;

static void
gst_omf_h264_src_class_init (GstOmfH264SrcClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSrcClass *gstbase_src_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gstelement_class = GST_ELEMENT_CLASS (klass);
  gstbase_src_class = GST_BASE_SRC_CLASS (klass);

  gobject_class->finalize = gst_omf_h264_src_finalize;

  gobject_class->set_property = gst_omf_h264_src_set_property;
  gobject_class->get_property = gst_omf_h264_src_get_property;

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

  /*OMF h264 parameter*/
  g_object_class_install_property (gobject_class, PROP_SENID,
      g_param_spec_int ("senid", "Sensor id", "The sensor id", 0,
          3, DEFAULT_SENID,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));	  
  g_object_class_install_property (gobject_class, PROP_WIDTH,
      g_param_spec_int ("width", "Width", "Width of resolution", 480,
          4096, DEFAULT_WIDTH,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_HEIGHT,
      g_param_spec_int ("height", "Height", "Height of resolution", 320,
          2160, DEFAULT_HEIGHT,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_FRAMERATE,
      g_param_spec_int ("framerate", "Frame rate", "Frame per second", 1,
          60, DEFAULT_FRAMERATE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_BITRATE,
      g_param_spec_int ("bitrate", "Bitrate", "H264 bitrate", 300*1024,
          10*1024*1024, DEFAULT_BITRATE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_GOP,
      g_param_spec_int ("gop", "GOP", "H264 group of pictures", 1,
          1200, DEFAULT_FRAMERATE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_GOPTYPE,
      g_param_spec_enum ("goptype", "GOP Type",
          "H264 GOP type", GST_OMF_H264_GOP_TYPE,
          DEFAULT_GOPTYPE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)); 
  g_object_class_install_property (gobject_class, PROP_CODEC,
		g_param_spec_string ("codec", "Codec", "H264 encode parameter", NULL,
			G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));  
  g_object_class_install_property (gobject_class, PROP_PREREC_IDX,
	   g_param_spec_int ("prerec-idx", "Prerecord index", "Link the default prerecord pipeline", 0,
		   1, DEFAULT_PREREC_IDX,
		   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_PREREC_PIPE,
	   g_param_spec_string ("prerec-pipe", "Prerecord pipeline", "Set prerecord pipeline and link it", NULL,
		   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));  
  g_object_class_install_property (gobject_class, PROP_SHARE,
	   g_param_spec_int ("share-idx", "Share index", "Set the share encoder index", 0,
		   10, DEFAULT_SHARE,
		   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_CACHE,
	   g_param_spec_int ("cache", "Cache", "Set cache for frame", 0,
		   1024, DEFAULT_CACHE,
		   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_LOW_BW,
      g_param_spec_boolean ("low-bw", "Low bandwidth",
          "Set true to use low bandwidth mode", DEFAULT_LOW_BW,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_MEDIA,
	   g_param_spec_string ("media-info", "Media information", "Get media information", NULL,
		   G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));  

  gst_element_class_set_static_metadata (gstelement_class,
      "Omf H264 Source",
      "Source",
      "Push h264 data out",
      "wang.zhou@icatchtek.com");
  gst_element_class_add_static_pad_template (gstelement_class, &srctemplate);

  gstbase_src_class->is_seekable = GST_DEBUG_FUNCPTR (gst_omf_h264_src_is_seekable);
  gstbase_src_class->start = GST_DEBUG_FUNCPTR (gst_omf_h264_src_start);
  gstbase_src_class->stop = GST_DEBUG_FUNCPTR (gst_omf_h264_src_stop);
  gstbase_src_class->event = GST_DEBUG_FUNCPTR (gst_omf_h264_src_event_handler);
  gstbase_src_class->get_times = GST_DEBUG_FUNCPTR (gst_omf_h264_src_get_times);
  gstbase_src_class->create = GST_DEBUG_FUNCPTR (gst_omf_h264_src_create);
}

static void
gst_omf_h264_src_init (GstOmfH264Src * omfh264src)
{
  omfh264src->silent = DEFAULT_SILENT;
  omfh264src->signal_handoffs = DEFAULT_SIGNAL_HANDOFFS;
  omfh264src->dump = DEFAULT_DUMP;
  omfh264src->format = DEFAULT_FORMAT;
  omfh264src->last_message = NULL;

  omfh264src->senid = DEFAULT_SENID;
  omfh264src->width = DEFAULT_WIDTH;
  omfh264src->height = DEFAULT_HEIGHT;
  omfh264src->framerate = DEFAULT_FRAMERATE;
  omfh264src->bitrate = DEFAULT_BITRATE;
  omfh264src->gop = DEFAULT_GOP;
  omfh264src->goptype = DEFAULT_GOPTYPE;
  omfh264src->codec = DEFAULT_CODEC ? strdup(DEFAULT_CODEC) : NULL;
  omfh264src->prerecidx = DEFAULT_PREREC_IDX;
  omfh264src->prerecpipe = DEFAULT_PREREC_PIPE ? strdup(DEFAULT_PREREC_PIPE) : NULL;
  omfh264src->shareidx = DEFAULT_SHARE;
  omfh264src->lowbw = DEFAULT_LOW_BW;
  omfh264src->media = NULL;

  omfh264src->omf_hd = OmfH264SrcCreate();

}

static void
gst_omf_h264_src_finalize (GObject * object)
{
  GstOmfH264Src *src;

  src = GST_OMF_H264_SRC (object);

  g_free (src->last_message);
  
  if(src->omf_hd){
	 OmfH264SrcDestory(src->omf_hd);
  }

  g_free(src->codec);
  src->codec = NULL;
  g_free(src->prerecpipe);
  src->prerecpipe = NULL;

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gboolean
gst_omf_h264_src_event_handler (GstBaseSrc * basesrc, GstEvent * event)
{
  GstOmfH264Src *src;

  src = GST_OMF_H264_SRC (basesrc);

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
gst_omf_h264_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstOmfH264Src *src;
  GstBaseSrc *basesrc;

  src = GST_OMF_H264_SRC (object);
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
	case PROP_SENID:
		src->senid = g_value_get_int(value);
		break;
	case PROP_WIDTH:
		src->width = g_value_get_int(value);
		break;
	case PROP_HEIGHT:
		src->height = g_value_get_int(value);
		break;
	case PROP_FRAMERATE:
		src->framerate = g_value_get_int(value);
		break;
	case PROP_BITRATE:
		src->bitrate = g_value_get_int(value);
		break;
	case PROP_GOP:
		src->gop = g_value_get_int(value);
		break;
	case PROP_GOPTYPE:
		src->goptype = (GstOmfH264SrcGopType)g_value_get_enum(value);
		break;
	case PROP_CODEC:
		g_free(src->codec);
		src->codec = g_strdup(g_value_get_string(value));
		break;
	case PROP_PREREC_IDX:
		src->prerecidx = g_value_get_int(value);
		break;
	case PROP_PREREC_PIPE:
		g_free(src->prerecpipe);
		src->prerecpipe = g_strdup(g_value_get_string(value));
		break;
	case PROP_SHARE:
		src->shareidx = g_value_get_int(value);
		break;
	case PROP_CACHE:
		src->cache = g_value_get_int(value);
		break;
	case PROP_LOW_BW:
		src->lowbw = g_value_get_boolean(value);
		break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_omf_h264_src_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstOmfH264Src *src;
  GstBaseSrc *basesrc;

  g_return_if_fail (GST_IS_OMF_H264_SRC (object));

  src = GST_OMF_H264_SRC (object);
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
	case PROP_SENID:
		g_value_set_int(value, src->senid);
		break;
	case PROP_WIDTH:
		g_value_set_int(value, src->width);
		break;
	case PROP_HEIGHT:
		g_value_set_int(value, src->height);
		break;
	case PROP_FRAMERATE:
		g_value_set_int(value, src->framerate);
		break;
	case PROP_BITRATE:
		g_value_set_int(value, src->bitrate);
		break;
	case PROP_GOP:
		g_value_set_int(value, src->gop);
		break;
	case PROP_GOPTYPE:
		g_value_set_enum(value, src->goptype);
		break;
	case PROP_CODEC:
		g_value_set_string(value, src->codec);
		break;
	case PROP_PREREC_IDX:
		g_value_set_int(value, src->prerecidx);
		break;
	case PROP_PREREC_PIPE:
		g_value_set_string(value, src->prerecpipe);
		break;
	case PROP_SHARE:
		g_value_set_int(value, src->shareidx);
		break;
	case PROP_CACHE:
		g_value_set_int(value, src->cache);
		break;
	case PROP_LOW_BW:
		g_value_set_boolean(value, src->lowbw);
		break;
	case PROP_MEDIA:
		g_value_set_string(value, src->media);
		break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static GstBuffer *
gst_omf_h264_src_alloc_buffer (GstOmfH264Src * src, guint size, gpointer pdata, GDestroyNotify pfree)
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
gst_omf_h264_src_get_times (GstBaseSrc * basesrc, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end)
{
  GstOmfH264Src *src;

  src = GST_OMF_H264_SRC (basesrc);
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
gst_omf_h264_src_create (GstBaseSrc * basesrc, guint64 offset, guint length,
    GstBuffer ** ret)
{
  GstOmfH264Src *src;
  GstBuffer *buf;
  GstMapInfo info;
  GstClockTime time;
  gsize size = 0;

  src = GST_OMF_H264_SRC (basesrc);

  OmfFrameC_t frame;
  memset(&frame, 0, sizeof(OmfFrameC_t));
  
  if(OmfH264SrcGetFrame(src->omf_hd, &frame)){
  	if(frame.data){
	  size = frame.size;
	 
	  buf =  gst_omf_h264_src_alloc_buffer (src, size, frame.data, frame.free);
	  if(buf){
	  	///
		GST_BUFFER_PTS(buf) = frame.pts_ms;
		GST_BUFFER_DTS(buf) = frame.pts_ms;
		GST_BUFFER_OFFSET(buf) = frame.index;	
		GST_MINI_OBJECT_CAST (buf)->flags = frame.iskeyframe;
	  }
	}	
  }

  if (!src->silent) {
    gchar dts_str[64], pts_str[64], dur_str[64];
    gchar *flag_str;

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

    flag_str = gst_buffer_get_flags_string (buf);
    src->last_message =
        g_strdup_printf ("create   ******* (%s:%s) (%u bytes, dts: %s, pts:%s"
        ", duration: %s, offset: %" G_GINT64_FORMAT ", offset_end: %"
        G_GINT64_FORMAT ", flags: %08x %s) %p",
        GST_DEBUG_PAD_NAME (GST_BASE_SRC_CAST (src)->srcpad), (guint) size,
        dts_str, pts_str, dur_str, GST_BUFFER_OFFSET (buf),
        GST_BUFFER_OFFSET_END (buf), GST_MINI_OBJECT_CAST (buf)->flags,
        flag_str, buf);
    g_free (flag_str);
    GST_OBJECT_UNLOCK (src);

    g_object_notify_by_pspec ((GObject *) src, pspec_last_message);
  }

  if (src->signal_handoffs) {
    GST_LOG_OBJECT (src, "pre handoff emit");
    g_signal_emit (src, gst_omf_h264_src_signals[SIGNAL_HANDOFF], 0, buf,
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
gst_omf_h264_src_start (GstBaseSrc * basesrc)
{
  GstOmfH264Src *src;

  src = GST_OMF_H264_SRC (basesrc);

  src->bytes_sent = 0;

  gst_base_src_set_format (basesrc, src->format);

  g_return_val_if_fail(src->omf_hd, FALSE);

  g_return_val_if_fail(OmfH264SrcSelectSensor(src->omf_hd, src->senid), FALSE);
  if(src->width){
  	g_return_val_if_fail(OmfH264SrcSetWidth(src->omf_hd, src->width), FALSE);
  }
  if(src->height){
  	g_return_val_if_fail(OmfH264SrcSetHeight(src->omf_hd, src->height), FALSE);
  }
  if(src->framerate){
  	g_return_val_if_fail(OmfH264SrcSetFrameRate(src->omf_hd, src->framerate), FALSE);
  }
  if(src->bitrate){
  	g_return_val_if_fail(OmfH264SrcSetBitRate(src->omf_hd, src->bitrate), FALSE);
  }
  if(src->gop){
   	g_return_val_if_fail(OmfH264SrcSetGop(src->omf_hd, src->gop), FALSE);
  }
  //if(src->goptype){
    g_return_val_if_fail(OmfH264SrcSetGopType(src->omf_hd, (int)src->goptype), FALSE);
  //}
  if(src->prerecidx){
  	g_return_val_if_fail(OmfH264SrcSetPreRecord(src->omf_hd, src->prerecidx), FALSE);
  }		
  if(src->prerecpipe){
	g_return_val_if_fail(OmfH264SrcSetPreRecordPipe(src->omf_hd, src->prerecpipe), FALSE);	
  }
  if(src->cache){
	g_return_val_if_fail(OmfH264SrcSetCache(src->omf_hd, src->cache), FALSE);
  }
  if(src->lowbw){
	g_return_val_if_fail(OmfH264SrcSetLowBandWidth(src->omf_hd, src->lowbw), FALSE);
  }

  src->media = OmfH264SrcGetMediaInfo(src->omf_hd);
  printf ("media info:%s", src->media);
   
  return OmfH264SrcStatusUp(src->omf_hd, OMF_STATE_PLAYING);
}

static gboolean
gst_omf_h264_src_stop (GstBaseSrc * basesrc)
{
  GstOmfH264Src *src;

  src = GST_OMF_H264_SRC (basesrc);

  GST_OBJECT_LOCK (src);

  g_free (src->last_message);
  src->last_message = NULL;
  
  GST_OBJECT_UNLOCK (src);

  if(src->omf_hd){
  	OmfH264SrcStatusDown(src->omf_hd, OMF_STATE_NULL);
  }

  return TRUE;
}

static gboolean
gst_omf_h264_src_is_seekable (GstBaseSrc * basesrc)
{
  GstOmfH264Src *src = GST_OMF_H264_SRC (basesrc);

  return FALSE;
}
