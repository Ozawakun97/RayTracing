//
// Created by mac on 2022/6/16.
//

#ifndef RAYTRACER_HITABLE_H
#define RAYTRACER_HITABLE_H
#include "ray.h"
//#include "material.h"
class material;
struct hit_record{
    float t;//碰到光线表面t的值
    vec3 p;//碰撞点
    vec3 normal;//碰撞点的法线
    material *mat_ptr;
};
class hitable {
public:
    virtual bool hit(const ray& r,float t_min,float t_max, hit_record& rec) const = 0;
};
#endif //RAYTRACER_HITABLE_H
