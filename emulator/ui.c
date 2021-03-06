/*
 * ui.c
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
#include <glib.h>

#include "emu.h"
#include "bsp.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

static GtkWidget* s_drawPane = NULL;
static gboolean s_dragEnabled;
static uint8_t *s_fb = NULL;
G_LOCK_DEFINE_STATIC(s_lock);

static void createWidgets(void);
static void onButtonPresed(GtkWidget *widget, gpointer param1);
static void onButtonReleased(GtkWidget *widget, gpointer param1);
static gboolean onMousePressed(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static gboolean onMouseReleased(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static gboolean onMouseMove(GtkWidget *widget, GdkEventMotion *event);
static gboolean onDrawEvent(GtkWidget *widget, cairo_t *cr, gpointer arg);

static void sendEvent(void *ptr, size_t size);

static void* readerThread (void *arg);

static int mmapPerform(char *path);


int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	gtk_init(NULL, NULL);

	int fb = mmapPerform(argv[1]);
	if (fb < 0)
		exit(0);

	createWidgets();

	gtk_main();

	return 0;
}

static void* readerThread(void *arg) {
	do {
		if (s_drawPane) {
			gtk_widget_queue_draw(s_drawPane);
		}
		usleep((1000));
	} while (1);
	return NULL;
}

static void createWidgets(void) {
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_box_new(FALSE, 0);
	GtkWidget *btnPlus, *btnMinus;

	gtk_window_set_default_size(GTK_WINDOW(window), SCREEN_WIDTH, SCREEN_HEIGHT);
	gtk_window_set_keep_above(GTK_WINDOW(window), 1);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(vbox);

	s_drawPane = gtk_drawing_area_new();
//	GdkRGBA white = {1.0, 1.0, 1.0, 1.0};
//	gtk_widget_override_background_color(s_drawPane, GTK_STATE_FLAG_NORMAL, &white);
	gtk_widget_set_size_request(s_drawPane, SCREEN_WIDTH, SCREEN_HEIGHT);

	GtkWidget *evbox = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(evbox), s_drawPane);
	gtk_widget_show (evbox);
	gtk_box_pack_start(GTK_BOX(vbox), evbox, TRUE, TRUE, 0);
	gtk_drag_dest_set_track_motion(evbox, TRUE);
	g_signal_connect(evbox, "button-press-event", G_CALLBACK(onMousePressed), (gpointer) NULL);
	g_signal_connect(evbox, "button-release-event", G_CALLBACK(onMouseReleased), (gpointer) NULL);
	g_signal_connect(evbox, "motion-notify-event", G_CALLBACK(onMouseMove), (gpointer) NULL);
	gtk_widget_show(s_drawPane);

	btnPlus = gtk_button_new_with_label("+");
	gtk_box_pack_start(GTK_BOX(vbox), btnPlus, FALSE, FALSE, 0);
	g_signal_connect(btnPlus, "pressed",  G_CALLBACK(onButtonPresed),	(gpointer) '+');
	g_signal_connect(btnPlus, "released", G_CALLBACK(onButtonReleased), (gpointer) '+');
	gtk_widget_show(btnPlus);

	btnMinus = gtk_button_new_with_label("-");
	gtk_box_pack_start(GTK_BOX(vbox), btnMinus, FALSE, FALSE, 0);
	g_signal_connect(btnMinus, "pressed",	G_CALLBACK(onButtonPresed),	 (gpointer) '-');
	g_signal_connect(btnMinus, "released", G_CALLBACK(onButtonReleased), (gpointer) '-');
	gtk_widget_show(btnMinus);

	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_widget_show(window);

	g_signal_connect(G_OBJECT(s_drawPane), "draw", G_CALLBACK(onDrawEvent), NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GThread *thread = g_thread_new("reader", readerThread, NULL);
	if (!thread) {
		g_print("Error: failed to create NRF main thread\n");
	}
}

static void onButtonPresed(GtkWidget *widget, gpointer param1) {
	uint32_t packed = (intptr_t)param1;
	packed = packed << 1 | 1;
	sendEvent(&packed, sizeof(packed));
}

static void onButtonReleased(GtkWidget *widget, gpointer param1) {
	uint32_t packed = (intptr_t)param1;
	packed = packed << 1 | 0;
	sendEvent(&packed, sizeof(packed));
}
static gboolean onMousePressed(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
	const int x = event->x;
	const int y = event->y;
	if (event->button == 1) {
		s_dragEnabled = TRUE;
		uint32_t buff[] = { 0x02, x, y, };
		sendEvent(buff, sizeof(buff));
	}
	return FALSE;
}
static gboolean onMouseReleased(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
	const int x = event->x;
	const int y = event->y;
	if (event->button == 1) {
		s_dragEnabled = FALSE;
		uint32_t buff[] = { 0x03, x, y, };
		sendEvent(buff, sizeof(buff));
	}
	return FALSE;
}
static gboolean onMouseMove (GtkWidget *widget, GdkEventMotion *event) {
	const int x = event->x;
	const int y = event->y;
	if (s_dragEnabled) {
		uint32_t buff[] = { 0x04, x, y, };
		sendEvent(buff, sizeof(buff));
	}
	return FALSE;
}

static void sendEvent(void *ptr, size_t size) {

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP & 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(BUTTONS_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
//		perror("connect");
		close(sock);
		return;
	}

	int err = send(sock, ptr, size, 0);
	if (err < 0) {
		perror("send");
		close(sock);
		return;
	}
	close(sock);
}

static gboolean onDrawEvent(GtkWidget *widget, cairo_t *cr, gpointer arg) {

	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, SCREEN_WIDTH);
	G_LOCK(s_lock);
	static uint8_t *buffer = NULL;
	if (!buffer) {
		buffer = malloc(stride * SCREEN_HEIGHT);
	}
	if (s_fb) {
		uint16_t *ptr = (uint16_t*)s_fb;
		size_t k = 0;
		memset(buffer, 0x00, stride * SCREEN_HEIGHT);
		for (size_t i = 0; i < SCREEN_SIZE/2; i++) {
			buffer[k++] = (ptr[i] & 0x001F) << 3;
			buffer[k++] = (ptr[i] & 0x07E0) >> 3;
			buffer[k++] = (ptr[i] & 0xF100) >> 8;
			buffer[k++] = 0x00;
		}
	}
	cairo_surface_t *sf = cairo_image_surface_create_for_data(buffer, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, stride);
	cairo_set_source_surface(cr, sf, 0, 0);
	cairo_paint(cr);
	cairo_fill(cr);
	cairo_surface_destroy(sf);
	G_UNLOCK(s_lock);
	return FALSE;
}

static int mmapPerform(char *path) {
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "chmod 777 %s", path);
	system(cmd);
	int fd = -1;
	do {
		fd = open(path, O_RDONLY);
		printf("Open [%s] fd %d\n", path, fd);
		if (fd < 0) {
			return 0;
			usleep((1000*1000));
			continue;
		}
	} while (fd < 0);
	lseek (fd, SCREEN_SIZE + 1, SEEK_SET);
	write (fd, "", 1);
	lseek (fd, 0, SEEK_SET);

	void *p = mmap (0, SCREEN_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		perror("mmap");
		close(fd);
		return -1;
	}

	struct stat sb;
	if (fstat(fd, &sb) == -1)  {
		perror("fstat");
		close(fd);
		return -1;
	}
	s_fb = (uint8_t*)p;
	return fd;
}
