//
// Created by mac on 2022/6/20.
//

#ifndef RAYTRACER_RANDOM_H
#define RAYTRACER_RANDOM_H
#include "vec3.h"
vec3 random_in_unit_sphere(){
    vec3 p;
    do {
        p = 2.0f * vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(p.squared_length() >= 1.0);
    return p;
}
#endif //RAYTRACER_RANDOM_H
