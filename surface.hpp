#ifndef SPHERE_H
#define SPHERE_H
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
    int height;
    int width;
    cairo_surface_t* cairo_surface;
    cairo_t* cr;

public:
    Ray get_ray(int x, int y) {
        return scene.get_ray((x - height/2) / height * 2, (y - width/2) / width*2);
    }

    void draw_pixel(int x, int y, Color c) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x, y);
        cairo_set_line_width(cr, 1);
        cairo_set_source_rgb(cr, c.x, c.y, c.z);
        cairo_stroke(cr);
    }

    void draw() {
        for (int x = 0; x < height; ++x) {
            for (int y = 0; y < width; ++y) {
                Color c = scene.trace_ray(get_ray(x, y));
                draw_pixel(x, y, c);
            }
        }
    }

    Surface(Scene scene, int height = 0, int width = 0) : scene(scene), height(height), width(width) {
        Display *dsp;
        Drawable da;
        Screen *scr;
        int screen;
        cairo_surface_t *sfc;

        if ((dsp = XOpenDisplay(NULL)) == NULL) {
            exit(1);
        }
        screen = DefaultScreen(dsp);
        scr = DefaultScreenOfDisplay(dsp);
        if (!height || !width) {
            height = WidthOfScreen(scr), width = HeightOfScreen(scr);
            da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, height, width, 0, 0, 0);
        }
        else {
            da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, height, width, 0, 0, 0);
        }
        XSelectInput(dsp, da, ButtonPressMask | KeyPressMask);
        XMapWindow(dsp, da);

        sfc = cairo_xlib_surface_create(dsp, da, DefaultVisual(dsp, screen), height, width);
        cairo_xlib_surface_set_size(sfc, height, width);

        cairo_surface = sfc;
        cr = cairo_create(sfc);
    }

    ~Surface() {
        Display *dsp = cairo_xlib_surface_get_display(cairo_surface);

        cairo_destroy(cr);
        cairo_surface_destroy(cairo_surface);
        XCloseDisplay(dsp);
    }
};
#endif
