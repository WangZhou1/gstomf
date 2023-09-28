/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstomfpcmplayer.h:
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


#ifndef __GST_PCM_PLAY_H__
#define __GST_PCM_PLAY_H__

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

#include "omfcstream.h"
#include "OmfAudPlayer.h"

G_BEGIN_DECLS


#define GST_TYPE_OMF_PCM_PLAYER \
  (gst_omf_pcm_player_get_type())
#define GST_OMF_PCM_PLAYER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMF_PCM_PLAYER,GstOmfPcmPlayer))
#define GST_OMF_PCM_PLAYER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMF_PCM_PLAYER,GstOmfPcmPlayerClass))
#define GST_IS_OMF_PCM_PLAYER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMF_PCM_PLAYER))
#define GST_IS_OMF_PCM_PLAYER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMF_PCM_PLAYER))
#define GST_OMF_PCM_PLAYER_CAST(obj) ((GstOmfPcmPlayer *)obj)

typedef struct _GstOmfPcmPlayer GstOmfPcmPlayer;
typedef struct _GstOmfPcmPlayerClass GstOmfPcmPlayerClass;

/**
 * GstOmfPcmPlayer:
 *
 * The opaque #GstOmfPcmPlayer data structure.
 */
struct _GstOmfPcmPlayer {
  GstBaseSink		element;

  gboolean		silent;
  gboolean		dump;
  gboolean		signal_handoffs;
  gchar			*last_message;

  guint			rate;
  guint			channel;
  gchar*		codec;

  gboolean		codecOnLinux;

  gchar*		liveLimit;

  const char*   media;

  gpointer 		omf_hd;
};

struct _GstOmfPcmPlayerClass {
  GstBaseSinkClass parent_class;

  /* signals */
  void (*handoff) (GstElement *element, GstBuffer *buf, GstPad *pad);
  void (*preroll_handoff) (GstElement *element, GstBuffer *buf, GstPad *pad);
};

G_GNUC_INTERNAL GType gst_omf_pcm_player_get_type (void);

G_END_DECLS

#endif /* __GST_PCM_PLAYER_H__ */
