#ifndef SURFACE_H
#define SURFACE_H
#include "scene.hpp"
#include "ray.hpp"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include <vector>

class Surface {
    Scene& scene;
    int width;
    int height;
    bool rendered;
    cairo_surface_t* cairo_surface;
    cairo_t* cr;
    Display *dsp;
    std::vector<std::vector<Color> > buffer;

public:
    Ray get_ray(int x, int y) {
        return scene.get_ray((x - width/2.0) / width, (y - height/2.0) / width);
    }

    std::pair<int, int> get_coord(Point p) {
        auto pair = scene.get_coord(p);
        return {(pair.first*width + width)/2, (pair.second*width + height)/2};
    }

    void draw_pixel(int x, int y, Color c) {
        cairo_set_source_rgb(cr, c.x, c.y, c.z);
        cairo_rectangle(cr, x, y, 1, 1);
        cairo_fill(cr);
    }
    
    void enchance_balance() {
        double factor = -INFINITY;
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                for (int comp = 0; comp < 3; ++comp) {
                    double cur = buffer[x][y].coord[comp];
                    if (cur > factor) {
                        factor = cur;
                    }
                }
            }
        }
        if (almost_zero(factor)) {
            return;
        }
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                buffer[x][y] /= factor;
            }
        }
    };

    void render() {
        if (width == 0 || rendered) {
            return;
        }
        buffer = std::vector<std::vector<Color> >(width, std::vector<Color>(height));
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                Color c = scene.trace_ray(get_ray(x, y));
                buffer[x][y] = c;
            }
        }
        enchance_balance();
        rendered = true;
    }

    void draw() {
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                draw_pixel(x, y, buffer[x][y]);
            }
        }
    }

    Surface(Scene& scene, int width = 0, int height = 0) : scene(scene), width(width), height(height), rendered(false) {
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
        render();
    }

    void event_loop() {
        double angle = 0;
        while (1)  {
            XEvent report;
            XNextEvent(dsp, &report);
            switch  (report.type) {
                case Expose:
                    if (report.xexpose.count != 0) {
                        break;
                    }
                    render();
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
                    render();
                    draw();
                    break;
                case ButtonPress:
                case KeyPress:
                    switch (report.xkey.keycode) {
                        case 113:
                            angle += 0.6;
                            break;
                        case 114:
                            angle -= 0.6;
                            break;
                        default:
                            continue;
                    }
                    scene.find_best_view(angle);
                    rendered = false;
                    render();
                    draw();
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
