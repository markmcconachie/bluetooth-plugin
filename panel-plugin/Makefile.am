INCLUDES = \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"xfce4-bluetooth-plugin\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

#
# bluetooth plugin
#
plugin_PROGRAMS = \
	xfce4-bluetooth-plugin

plugindir = \
	$(libexecdir)/xfce4/panel-plugins

xfce4_bluetooth_plugin_SOURCES = \
	bluetooth.c \
	bluetooth.h \
	bluetooth-dialogs.c \
	bluetooth-dialogs.h \
	bluetooth-window.c \
	bluetooth-window.h

xfce4_bluetooth_plugin_CFLAGS = \
	$(LIBXFCE4UTIL_CFLAGS) \
	$(LIBXFCE4UI_CFLAGS) \
	$(LIBXFCE4PANEL_CFLAGS) \
	$(PLATFORM_CFLAGS)      \
	$(BLUEZ_CFLAGS)

xfce4_bluetooth_plugin_LDADD = \
	$(LIBXFCE4UTIL_LIBS) \
	$(LIBXFCE4UI_LIBS) \
	$(LIBXFCE4PANEL_LIBS) \
	$(BLUEZ_LIBS)

#
# Desktop file
#
desktopdir =								\
	$(datadir)/xfce4/panel-plugins

desktop_in_in_files =							\
	bluetooth.desktop.in.in

desktop_in_files =							\
	$(desktop_in_in_files:.desktop.in.in=.desktop.in)

%.desktop.in: %.desktop.in.in
	sed -e "s,\@libexecdir\@,$(libexecdir),g" < $< > $@

desktop_DATA =								\
	$(desktop_in_files:.desktop.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST =								\
	$(desktop_in_in_files)

CLEANFILES =								\
	$(desktop_in_files)						\
	$(desktop_DATA)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
