#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gst/gst.h>
#include <gst/gstversion.h>

#include "gstomfaudsrc.h"
#include "gstomfaudplayer.h"
#include "gstomfh264src.h"
#include "gstomfh265src.h"
#include "gstomfjpegsrc.h"
#include "gstomfpcmsrc.h"


static gboolean
plugin_init (GstPlugin * plugin)
{
	if (!gst_element_register (plugin, "omfaudsrc", GST_RANK_NONE,
		gst_omf_aud_src_get_type()))
		return FALSE;
	if (!gst_element_register (plugin, "omfaudplayer", GST_RANK_NONE,
		gst_omf_aud_player_get_type()))
		return FALSE;
	if (!gst_element_register (plugin, "omfh264src", GST_RANK_NONE,
		gst_omf_h264_src_get_type()))
		return FALSE;
	if (!gst_element_register (plugin, "omfh265src", GST_RANK_NONE,
		gst_omf_h265_src_get_type()))
		return FALSE;
	//if (!gst_element_register (plugin, "omfpcmsrc", GST_RANK_NONE,
	//	gst_omf_pcm_src_get_type()))	
	//	return FALSE;
	if (!gst_element_register (plugin, "omfjpegsrc", GST_RANK_NONE,
		gst_omf_jpeg_src_get_type()))
		return FALSE;

    return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR, omfelements,
    "GStreamer omf elements", plugin_init, VERSION, GST_LICENSE,
    GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN);
