/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstomfh265src.h:
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


#ifndef __GST_OMF_H265_SRC_H__
#define __GST_OMF_H265_SRC_H__

#include <gst/gst.h>
#include <gst/base/gstbasesrc.h>

#include "omfcstream.h"
#include "OmfH265Src.h"

G_BEGIN_DECLS

#define GST_TYPE_OMF_H265_SRC \
  (gst_omf_h265_src_get_type())
#define GST_OMF_H265_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMF_H265_SRC,GstOmfH265Src))
#define GST_OMF_H265_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMF_H265_SRC,GstOmfH265SrcClass))
#define GST_IS_OMF_H265_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMF_H265_SRC))
#define GST_IS_OMF_H265_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMF_H265_SRC))

/**
 * The gop type h265 support.
 */

typedef enum {
  OMF_H265_SRC_GOP_TYPE_IBBP = 0,
  OMF_H265_SRC_GOP_TYPE_IPPP,
  OMF_H265_SRC_GOP_TYPE_IIII
} GstOmfH265SrcGopType;


/**
 * GstOmfH265Src:
 *
 * Opaque #GstOmfH265Src data structure.
 */
typedef struct _GstOmfH265Src {
  GstBaseSrc     element;

  /*< private >*/  
  gboolean	 	silent;
  gboolean	 	signal_handoffs;
  gboolean	 	dump;
  GstFormat     format;

  guint64       bytes_sent;

  gchar*		last_message;

  guint			senid;
  guint			width;
  guint			height;
  guint			gop;
  GstOmfH265SrcGopType		goptype;
  guint			framerate;
  guint			bitrate;
  gchar*		codec;

  guint			prerecidx;
  gchar*		prerecpipe;

  guint			shareidx;

  guint			cache;

  gboolean		lowbw;

  gchar*		media;

  gpointer 		omf_hd;
} GstOmfH265Src;

typedef struct _GstOmfH265SrcClass {
  GstBaseSrcClass parent_class;

  /*< public >*/
  /* signals */
  void (*handoff) (GstElement *element, GstBuffer *buf, GstPad *pad);
} GstOmfH265SrcClass;

G_GNUC_INTERNAL GType gst_omf_h265_src_get_type (void);

G_END_DECLS

#endif /* __GST_OMF_H265_SRC_H__ */
