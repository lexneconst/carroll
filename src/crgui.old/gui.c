#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "gui.h"
#include "commons.h"
#include "utils.h"


#define TEXT_SURFACE_SIZE 100
#define TEXT_BASE 32.0
#define PADDING 1.0


// In initialized in "active" function.
static GtkStatusIcon *tray_icon;
// TODO: https://developer.gnome.org/gtk3/3.0/GtkStatusIcon.html#gtk-status-icon-set-tooltip-text

GtkWidget *window;

static struct tray_icon_data current_tid;


// ******************************************************************
//  PRIVATE FUNCTIONS IMPLEMENTATION
// ******************************************************************

static GdkPixbuf *getPixBuf(const char *utf8, struct rgb_color *color)
{
   // Preprare Cairo objects.
   cairo_surface_t *surface = cairo_image_surface_create(
      CAIRO_FORMAT_ARGB32, TEXT_SURFACE_SIZE, TEXT_SURFACE_SIZE);
   g_assert(surface != NULL);
   cairo_t *cr = cairo_create(surface);
   g_assert(cr != NULL);

   // Clean surface.
   cairo_set_source_rgba(cr, 0, 0, 0, 0);
   cairo_paint(cr);

   // Paint the text.
   cairo_select_font_face(
      cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size(cr, TEXT_BASE);
   cairo_move_to(cr, TEXT_BASE, TEXT_BASE);
   cairo_set_source_rgba(cr, color->r, color->g, color->b, 1.0);
   cairo_show_text(cr, utf8);

   // Get sizes.
   cairo_text_extents_t extends;
   cairo_text_extents(cr, utf8, &extends);

   // Debug output.
   //printf("ex: %f %f %f %f", extends.width, extends.height,
   //		extends.x_bearing, extends.y_bearing);

   // Get pixbuf from surface.
   double x = (TEXT_BASE - PADDING) + extends.x_bearing;
   double y = (TEXT_BASE - PADDING) + extends.y_bearing;
   double w = extends.width + (2 * PADDING);
   double h = extends.height + (2 * PADDING);

   // TODO: Crop surface to size of trace icon?

   // Output must be a rectangle, we do not want to scale anything.
   // So take the larger dimension, the other one move to center.
   double fs = MAX(w, h); // Final size.
   GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(
      surface, (x-(fs-w)/2), (y-(fs-h)/2), fs, fs);

   // Clean up Cairo objects.
   cairo_surface_destroy(surface);
   cairo_destroy(cr);

   return pixbuf;
}


static gboolean update_entry(struct tray_icon_data *tid)
{
   GdkPixbuf *pixbufout = getPixBuf(tid->msg, &tid->color);
   gtk_status_icon_set_from_pixbuf(tray_icon, pixbufout);
   gtk_status_icon_set_tooltip_text(tray_icon, tid->tooltip);

   // Remember this as last entry.
   current_tid = *tid;

   free(tid);

   return G_SOURCE_REMOVE;
}


static gboolean on_click(GtkStatusIcon *status_icon, GdkEvent *event, gpointer user_data)
{
   if (0 < strlen(current_tid.on_click_command)) {
      system(current_tid.on_click_command);
   }
   return TRUE; // Stop propagation.
}


static void activate(GtkApplication* app, gpointer gui_started)
{
   // TODO: zkouknout jeste
   GdkPixbuf *pixbufout = getPixBuf(current_tid.msg, &current_tid.color);

   window = gtk_application_window_new(app);

#ifdef DEBUG
   // In debug mode display also window with larger pixbuf.
   gtk_window_set_title(GTK_WINDOW(window), "Welcome to GNOME");
   gtk_window_set_default_size(GTK_WINDOW(window), 100, 100);

   GtkWidget *pb = gtk_image_new_from_pixbuf(pixbufout);
   gtk_container_add(GTK_CONTAINER(window), pb);
#endif
   gtk_widget_show_all(window);
#ifndef DEBUG
   gtk_widget_hide(window);
#endif

   // Show status icon.
   tray_icon = gtk_status_icon_new();
   gtk_status_icon_set_from_pixbuf(tray_icon, pixbufout);
   gtk_status_icon_set_visible(tray_icon, TRUE);
   gtk_status_icon_set_tooltip_text(tray_icon, current_tid.tooltip);

   g_signal_connect(
      tray_icon, "button-release-event", G_CALLBACK(on_click), NULL);


   // Notify that thread is activated.
   ((gui_start_callback_t) gui_started)();
}



// ******************************************************************
//  PUBLIC FUNCTIONS IMPLEMENTATION
// ******************************************************************

void gui_start(gui_start_callback_t callback, struct tray_icon_data *tid)
{
   // Set initial state as current state.
   current_tid = *tid;

   // Start GTK.
   GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
   g_signal_connect(
      app, "activate", G_CALLBACK(activate), (gpointer) callback);

   g_application_run(G_APPLICATION(app), 0, NULL);
   g_object_unref(app);
}


void gui_set(struct tray_icon_data *tid)
{
   // TODO: check!!
   struct tray_icon_data *tid_copy =
      (struct tray_icon_data *) malloc(sizeof(*tid));
   memcpy(tid_copy, tid, sizeof(*tid));
   gdk_threads_add_idle((GSourceFunc) update_entry, tid_copy);
}


void gui_quit(void)
{
   gtk_status_icon_set_visible(tray_icon, FALSE);
   gtk_window_close(GTK_WINDOW(window));
   gtk_widget_destroy(window);
   g_object_unref(tray_icon);
}

