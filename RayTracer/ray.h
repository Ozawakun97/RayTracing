//
// Created by mac on 2022/6/8.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H
#include "vec3.h"
class ray{
public:
    ray() {}
    //点和方向
    ray(const vec3& a, const vec3& b) {A=a; B=b;}
    vec3 origin() const {return A;}
    vec3 direction() const {return B;}
    vec3 point_at_parameter(float t) const {return A + t*B;}
    vec3 A;
    vec3 B;
};
#endif //RAYTRACER_RAY_H
