//
// Created by mac on 2022/6/20.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H
#include "hitable.h"
//#include "vec3.h"
#include "random.h"

class material{
public:
    virtual bool scatter(const ray& r_in,const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material{
public:
    lambertian(const vec3&a) : albedo(a) {}
    virtual bool scatter(const ray& r_in,const hit_record& rec, vec3& attenuation, ray& scattered) const override{
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p,target-rec.p);
        attenuation = albedo;
        return  true;
    }
    vec3 albedo;
};

class metal : public material{
public:
    metal(const vec3&a,float f): albedo(a) {if(f < 1) fuzz = f; else fuzz = 1;}
    virtual bool scatter(const ray& r_in,const hit_record& rec, vec3& attenuation, ray& scattered)const override{
        vec3 reflected = reflect(unit_vector(r_in.direction()),rec.normal);
        scattered = ray(rec.p,reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(),rec.normal)>0);
    }
    vec3 albedo;
    float fuzz;
};
class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
    ) const override {
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 outward_normal;
        float refraction_ratio;
        double cos_theta;
        float reflect_prob;
        vec3 unit_direction = unit_vector(r_in.direction());
        cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        bool can_reflect = refraction_ratio * sin_theta > 1.0f;
        if(dot(r_in.direction(),rec.normal) > 0){
            outward_normal = -rec.normal;
            refraction_ratio = ir;//出射光的cos theta
            cos_theta = ir * dot(unit_direction,rec.normal);
        }
        else{
            outward_normal = rec.normal;
            refraction_ratio = 1.0 / ir;

        }
       // double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 direction;
        if(can_reflect){
            reflect_prob = 1.0f;
        }
        else{
            reflect_prob = reflectance(cos_theta,refraction_ratio);
        }
        if(drand48() < reflect_prob){
            direction = reflect(unit_direction,rec.normal);
        }
        else{
            direction = refract(unit_direction, outward_normal, refraction_ratio);
        }
        scattered = ray(rec.p, direction);
        return true;
    }

public:
    double ir; // Index of Refraction
private:
    static float reflectance(double cosine, double ref_idx) {
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};
#endif //RAYTRACER_MATERIAL_H
