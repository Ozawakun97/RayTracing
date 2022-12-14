//
// Created by mac on 2022/6/19.
//
#define M_PI 3.141592
#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H
#include "ray.h"
class camera{
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov,float aspectratio ){
        vec3 u,v,w;
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2);//因为距离为1所以不需要别的
        float half_width = half_height * aspectratio;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup,w));
        v = cross(w,u);
        //lower_left_corner = vec3(-half_width,-half_height,-1.0);
        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
    }
    ray get_ray(float u, float v){ return ray(origin,lower_left_corner + u * horizontal + v*vertical - origin);}
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};

#endif //RAYTRACER_CAMERA_H
