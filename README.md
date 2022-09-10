# RayTracing
## Overview
A demo for brute-forcced RayTracing, written in raw C-Language styled way, with lambertian, glass, metal material and MSAA implemented 
This random scene shows all the feature together and the material and positions of the balls are randomly generated
![image](RayTracer/coverpage.png)

## Features
### Material
#### Diffuse
This is the image with Lambertian Diffuse material implemented. 

![image](RayTracer/LambertianDiffuse.png#pic_center)

This is implemented using the hit normal added with a random direction vector in unit sphere
```cpp
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

```
#### Metal
Metal material based on reflection 
