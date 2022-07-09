#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "rtweekend.h"
//用于管理虚拟相机以及场景采样相关的任务
//在这里，使用类似轴对齐相机的实现，实现了一个简单的相机
class camera {
public:
    camera(
        point3 lookfrom,
        point3 lookat,
        vec3   vup,
        double vfov, // vertical field-of-view in degrees
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double _time0 = 0,
        double _time1 = 0
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = (lookfrom - lookat).normalized();//相机视角方向的反方向
        u = cross(vup, w).normalized();//相机右侧
        v = cross(w, u);//相机上方朝向

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
        lens_radius = aperture / 2;

        time0 = _time0;
        time1 = _time1;
    }

    ray get_ray(double s, double t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(
            origin + offset, 
            lower_left_corner + s * horizontal + t * vertical - origin - offset,
            random_double(time0, time1)
        );
    }

    camera(camera& cam) {
        origin = cam.origin;
        lower_left_corner = cam.lower_left_corner;
        horizontal = cam.horizontal;
        vertical = cam.vertical;
        u = cam.u; v = cam.v; w = cam.w;
        lens_radius = cam.lens_radius;
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;
};
#endif