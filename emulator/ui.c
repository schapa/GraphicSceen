/*
 * bsp.c
 *
 *  Created on: Dec 29, 2016
 *      Author: shapa
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <cairo.h>
#include <gtk/gtk.h>
#include "glib.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 64

#define SCREEN_BYTES_PERLINE (SCREEN_WIDTH/2)
#define SCREEN_SIZE (SCREEN_BYTES_PERLINE*SCREEN_HEIGHT)

static GtkWidget* g_draw_widget = NULL;
G_LOCK_DEFINE_STATIC(s_lock);

static void createWidgets(void);
static void onButtonPresed(GtkWidget *widget, gpointer param1);
static void onButtonReleased(GtkWidget *widget, gpointer param1);
static gboolean onDrawEvent(GtkWidget *widget, cairo_t *cr, gpointer arg);

static void* readerThread (void *arg);

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	if (!g_thread_supported())
		g_thread_init(NULL);
	gdk_threads_init();
	gdk_threads_enter();
	gtk_init(NULL, NULL);

	createWidgets();

	gtk_main();
	gdk_threads_leave();

	return 0;
}

static void* readerThread (void *arg) {
	do {

	} while (0);
	return NULL;
}

static void createWidgets(void) {
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_box_new(FALSE, 0);
	GtkWidget *btnPlus, *btnMinus;

	gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
	gtk_window_set_keep_above(GTK_WINDOW(window), 1);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(vbox);

	g_draw_widget = gtk_drawing_area_new();
	GdkRGBA white = {1.0, 1.0, 1.0, 1.0};
	gtk_widget_override_background_color(g_draw_widget, GTK_STATE_FLAG_NORMAL, &white);
	gtk_widget_set_size_request(g_draw_widget, SCREEN_WIDTH, SCREEN_HEIGHT);

//	gtk_drawing_area_size (GTK_DRAWING_AREA (g_draw_widget), SCREEN_WIDTH, SCREEN_HEIGHT);
	gtk_box_pack_start(GTK_BOX(vbox), g_draw_widget, TRUE, TRUE, 0);
	gtk_widget_show(g_draw_widget);

	btnPlus = gtk_button_new_with_label("+");
	gtk_box_pack_start(GTK_BOX(vbox), btnPlus, FALSE, FALSE, 0);
	g_signal_connect(btnPlus, "pressed",  G_CALLBACK(onButtonPresed),	(gpointer) true);
	g_signal_connect(btnPlus, "released", G_CALLBACK(onButtonReleased), (gpointer) true);
	gtk_widget_show(btnPlus);

	btnMinus = gtk_button_new_with_label("-");
	gtk_box_pack_start(GTK_BOX(vbox), btnMinus, FALSE, FALSE, 0);
	g_signal_connect(btnMinus, "pressed",	G_CALLBACK(onButtonPresed),	 (gpointer) false);
	g_signal_connect(btnMinus, "released", G_CALLBACK(onButtonReleased), (gpointer) false);
	gtk_widget_show(btnMinus);

	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_widget_show(window);

	g_signal_connect(G_OBJECT(g_draw_widget), "draw", G_CALLBACK(onDrawEvent),
			NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
			NULL);

	GThread *thread = g_thread_new("reader", readerThread, NULL);
	if (!thread) {
		g_print("Error: failed to create NRF main thread\n");
	}
}

static void onButtonPresed(GtkWidget *widget, gpointer param1) {
	uint32_t packed = (intptr_t)param1;
	printf("Button %d\n", packed);
//	packed = packed<<1 |0;
//	EventQueue_Push(FSM_EXTI, (void*)(intptr_t)packed);
}

static void onButtonReleased(GtkWidget *widget, gpointer param1) {
	uint32_t packed = (intptr_t)param1;
	printf("Button %d\n", packed);
//	packed = packed<<1 | 1;
//	EventQueue_Push(FSM_EXTI, (void*)(intptr_t)packed);
}

static gboolean onDrawEvent(GtkWidget *widget, cairo_t *cr, gpointer arg) {

	static uint8_t buffer[SCREEN_WIDTH*SCREEN_HEIGHT];
	printf("Draw\n");
	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, SCREEN_WIDTH);
	G_LOCK(s_lock);
	memset(buffer, 0x1F, SCREEN_SIZE);

	cairo_surface_t * sf = cairo_image_surface_create_for_data(buffer,
			CAIRO_FORMAT_A8, SCREEN_WIDTH, SCREEN_HEIGHT, stride);
	cairo_set_source_surface(cr, sf, 0, 0);
	cairo_paint(cr);
	cairo_fill(cr);
	cairo_surface_destroy(sf);
	G_UNLOCK(s_lock);
	return FALSE;
}
