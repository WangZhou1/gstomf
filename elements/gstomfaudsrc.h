/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstomfaudsrc.h:
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


#ifndef __GST_OMF_AUD_SRC_H__
#define __GST_OMF_AUD_SRC_H__

#include <gst/gst.h>
#include <gst/base/gstbasesrc.h>

#include "omfcstream.h"
#include "OmfAudSrc.h"

G_BEGIN_DECLS

#define GST_TYPE_OMF_AUD_SRC \
  (gst_omf_aud_src_get_type())
#define GST_OMF_AUD_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMF_AUD_SRC,GstOmfAudSrc))
#define GST_OMF_AUD_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMF_AUD_SRC,GstOmfAudSrcClass))
#define GST_IS_OMF_AUD_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMF_AUD_SRC))
#define GST_IS_OMF_AUD_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMF_AUD_SRC))

/**
 * The AEC Level.
 */

typedef enum {
  OMF_AUD_SRC_AEC_LEVEL_NONE = 0,
  OMF_AUD_SRC_AEC_LEVEL_L,
  OMF_AUD_SRC_AEC_LEVEL_M,
  OMF_AUD_SRC_AEC_LEVEL_H
} GstOmfAudSrcAecLevel;

/**
 * The ANS MODE.
 */

typedef enum {
  OMF_AUD_SRC_ANS_MODE_NONE = 0,
  OMF_AUD_SRC_ANS_MODE_MILD ,
  OMF_AUD_SRC_ANS_MODE_MEDIUM,
  OMF_AUD_SRC_ANS_MODE_AGGRESSIVE,
} GstOmfAudSrcAnsMode;


/**
 * GstOmfAudSrc:
 *
 * Opaque #GstOmfAudSrc data structure.
 */
typedef struct _GstOmfAudSrc {
  GstBaseSrc	element;

  /*< private >*/  
  gboolean		silent;
  gboolean		signal_handoffs;
  gboolean		dump;
  GstFormat 	format;

  guint64   	bytes_sent;

  gchar*		last_message;

  guint			samples;
  guint			rate;
  guint			channels;

  gchar*		codec;
  
  GstOmfAudSrcAecLevel		aec;
  GstOmfAudSrcAnsMode		ans;
  gchar*		alc;
  
  guint 		prerecidx;
  gchar*		prerecpipe;

  guint 		shareidx;

  guint			cache;

  long long 	spts_ns;	
  long long		lpts_ns;

  const gchar*	media;

  gpointer 		omf_hd;
} GstOmfAudSrc;

typedef struct _GstOmfAudSrcClass {
  GstBaseSrcClass parent_class;

  /*< public >*/
  /* signals */
  void (*handoff) (GstElement *element, GstBuffer *buf, GstPad *pad);
} GstOmfAudSrcClass;

G_GNUC_INTERNAL GType gst_omf_aud_src_get_type (void);

G_END_DECLS

#endif /* __GST_OMF_AUD_SRC_H__ */
