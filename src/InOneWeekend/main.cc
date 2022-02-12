#include "rtweekend.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

class camera {
  public:
    camera() {
        const auto aspect_ratio = 16.0 / 9.0;
        const auto viewport_height = 2.0;
        const auto viewport_width = aspect_ratio * viewport_height;
        const auto focal_length = 1.0;

        focal_point = point3(0, 0, 0);
        viewport_origin = point3(-viewport_width / 2, viewport_height / 2, -focal_length);
        @@@
    }

    // ??? Should the camera care about the image resolution?

    ray get_ray(double s, double t) const {
        auto u = s * step_x;
        auto v = t * step_y;
        auto pixel_loc = viewport_origin + u + v;
        return ray(focal_point, pixel_loc - focal_point);
    }

  private:
    point3 focal_point;
    point3 viewport_origin;
}

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;
    auto focal_point = point3(0,0,0);

    auto viewport_origin = point3(-viewport_width / 2, viewport_height / 2, -focal_length);
    const int step_count_x = (image_width  < 2) ? 1 : image_width  - 1;
    const int step_count_y = (image_height < 2) ? 1 : image_height - 1;
    const auto step_x = vec3(static_cast<double>(viewport_width) / step_count_x, 0, 0);
    const auto step_y = vec3(0, static_cast<double>(-viewport_height) / step_count_y, 0);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        auto v = j * step_y;
        for (int i = 0; i < image_width; ++i) {
            auto u = i * step_x;
            auto pixel_loc = viewport_origin + u + v;
            ray r(focal_point, pixel_loc - focal_point);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color, 1);
        }
    }

    std::clog << "\rDone.                        \n";
}
