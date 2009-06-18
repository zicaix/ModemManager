/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef MM_PLUGIN_H
#define MM_PLUGIN_H

#include <glib-object.h>
#include <mm-modem.h>

#define MM_PLUGIN_MAJOR_VERSION 3
#define MM_PLUGIN_MINOR_VERSION 0

#define MM_TYPE_PLUGIN      (mm_plugin_get_type ())
#define MM_PLUGIN(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), MM_TYPE_PLUGIN, MMPlugin))
#define MM_IS_PLUGIN(obj)   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MM_TYPE_PLUGIN))
#define MM_PLUGIN_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), MM_TYPE_PLUGIN, MMPlugin))

typedef struct _MMPlugin MMPlugin;

typedef MMPlugin *(*MMPluginCreateFunc) (void);

struct _MMPlugin {
    GTypeInterface g_iface;

    /* Methods */
    const char *(*get_name)   (MMPlugin *self);

    /* Check whether a plugin supports a particular modem port, and what level
     * of support the plugin has for the device.  The plugin should return a
     * value between 0 and 100 inclusive, where 0 means the plugin has no
     * support for the device, and 100 means the plugin has full support for the
     * device.
     */
    guint32 (*supports_port)  (MMPlugin *self,
                               const char *subsys,
                               const char *name);

    /* Will only be called if the plugin returns a value greater than 0 for
     * the supports_port() method for this port.  The plugin should create and
     * return a  new modem for the port's device if there is no existing modem
     * to handle the port's hardware device, or should add the port to an
     * existing modem and return that modem object.  If an error is encountered
     * while claiming the port, the error information should be returned in the
     * error argument, and the plugin should return NULL.
     */
    MMModem * (*grab_port)    (MMPlugin *self,
                               const char *subsys,
                               const char *name,
                               GError **error);
};

GType mm_plugin_get_type (void);

const char *mm_plugin_get_name   (MMPlugin *plugin);

guint32 mm_plugin_supports_port  (MMPlugin *plugin,
                                  const char *subsys,
                                  const char *name);

MMModem *mm_plugin_grab_port     (MMPlugin *plugin,
                                  const char *subsys,
                                  const char *name,
                                  GError **error);

#endif /* MM_PLUGIN_H */

