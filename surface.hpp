#ifndef SURFACE_H
#define SURFACE_H
#include "scene.hpp"
#include "ray.hpp"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include <stdio.h>
#include <stdlib.h>

class Surface {
    const Scene& scene;
    int width;
    int height;
    cairo_surface_t* cairo_surface;
    cairo_t* cr;
    Display *dsp;

public:
    Ray get_ray(int x, int y) {
        return scene.get_ray((x - width/2.0) / width*2, (y - height/2.0) / width*2);
    }

    void draw_pixel(int x, int y, Color c) {
        cairo_set_source_rgb(cr, c.x, c.y, c.z);
        cairo_rectangle(cr, x, y, 1, 1);
        cairo_fill(cr);
    }

    void draw_border() {
        const Color c(1, 1, 0);
        for (int x = 0; x < width; ++x) {
            draw_pixel(x, 0, c);
            draw_pixel(x, height-1, c);
        }
        for (int y = 0; y < height; ++y) {
            draw_pixel(0, y, Color(0, 1, 1));
            draw_pixel(width-1, y, Color(0, 1, 1));
        }
    }

    void draw() {
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                Color c = scene.trace_ray(get_ray(x, y));
                draw_pixel(x, y, c);
            }
        }
        draw_border();
    }

    Surface(const Scene& scene, int width = 0, int height = 0) : scene(scene), width(width), height(height) {
        Drawable da;
        Screen *scr;
        int screen;
        cairo_surface_t *sfc;

        if ((dsp = XOpenDisplay(NULL)) == NULL) {
            exit(1);
        }
        screen = DefaultScreen(dsp);
        scr = DefaultScreenOfDisplay(dsp);
        width = WidthOfScreen(scr), height = HeightOfScreen(scr);
        da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, width, height, 0, 0, 0);
        XSelectInput(dsp, da, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);
        XMapWindow(dsp, da);
        sfc = cairo_xlib_surface_create(dsp, da, DefaultVisual(dsp, screen), width, height);
        cairo_xlib_surface_set_size(sfc, width, height);

        cairo_surface = sfc;
        cr = cairo_create(sfc);
    }

    void event_loop() {
        while (1)  {
            XEvent report;
            XNextEvent(dsp, &report);
            switch  (report.type) {
                case Expose:
                    if (report.xexpose.count != 0) {
                        break;
                    }
                    draw();
                    break;
                case ConfigureNotify:
                    /* Window has been resized; change width
                     * and height to send to place_text and
                     * place_graphics in next Expose */
                    if (width == report.xconfigure.width && height == report.xconfigure.height) {
                        break;
                    }
                    width = report.xconfigure.width;
                    height = report.xconfigure.height;
                    draw();
                    break;
                case ButtonPress:
                    break;
                case KeyPress:
                    if (report.xkey.keycode == 9) {
                        return;
                    }
                default:
                    /* All events selected by StructureNotifyMask
                     * except ConfigureNotify are thrown away here,
                     * since nothing is done with them */
                    break;
            } /* End switch */
        }
    }

    ~Surface() {
        cairo_destroy(cr);
        cairo_surface_destroy(cairo_surface);
        XCloseDisplay(dsp);
    }
};
#endif
