/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstomfaudplayer.h:
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


#ifndef __GST_AUD_PLAYER_H__
#define __GST_AUD_PLAYER_H__

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

#include "omfcstream.h"
#include "OmfAudPlayer.h"

G_BEGIN_DECLS


#define GST_TYPE_OMF_AUD_PLAYER \
  (gst_omf_aud_player_get_type())
#define GST_OMF_AUD_PLAYER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMF_AUD_PLAYER,GstOmfAudPlayer))
#define GST_OMF_AUD_PLAYER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMF_AUD_PLAYER,GstOmfAudPlayerClass))
#define GST_IS_OMF_AUD_PLAYER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMF_AUD_PLAYER))
#define GST_IS_OMF_AUD_PLAYER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMF_AUD_PLAYER))
#define GST_OMF_AUD_PLAYER_CAST(obj) ((GstOmfAudPlayer *)obj)

typedef struct _GstOmfAudPlayer GstOmfAudPlayer;
typedef struct _GstOmfAudPlayerClass GstOmfAudPlayerClass;

/**
 * GstOmfAudPlayer:
 *
 * The opaque #GstOmfAudPlayer data structure.
 */
struct _GstOmfAudPlayer {
  GstBaseSink		element;

  gboolean		silent;
  gboolean		dump;
  gboolean		signal_handoffs;
  gchar			*last_message;

  guint			rate;
  guint			channel;
  gchar*		media;

  gboolean		codecOnLinux;

  gchar*		liveLimit;

  gpointer 		omf_hd;
};

struct _GstOmfAudPlayerClass {
  GstBaseSinkClass parent_class;

  /* signals */
  void (*handoff) (GstElement *element, GstBuffer *buf, GstPad *pad);
  void (*preroll_handoff) (GstElement *element, GstBuffer *buf, GstPad *pad);
};

G_GNUC_INTERNAL GType gst_omf_aud_player_get_type (void);

G_END_DECLS

#endif /* __GST_AUD_PLAYER_H__ */
