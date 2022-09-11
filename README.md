# RayTracing
## Overview
A demo for brute-forcced RayTracing, written in raw C-Language styled way, with lambertian, glass, metal material and MSAA implemented 
This random scene shows all the feature together and the material and positions of the balls are randomly generated
![image](RayTracer/coverpage.png)

## Features
### Material
#### Diffuse
This is the image with Lambertian Diffuse material implemented. 

![image](RayTracer/image_LambertianMat.png)

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
This image is the Metal material based on reflection with the parameter of fuzz,if fuzz = 0 it is completely a smooth surface.
![image](RayTracer/image_MetalMat.png)
 When fuzz = 1, this will lead to some roughness on the surface of the sphere 
![image](RayTracer/image_MetalFuzzMat.png)
And the metal material is implemented by the following code
```cpp
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
```
##### Reflection
The relfected ray is computed using the following method(Explanantion on the picture below and also seen in the code behind)
![image](RayTracer/Reflection.png)
```cpp
vec3 reflect(const vec3& v, const vec3& n){
    return v - 2* dot(v,n)*n;
}
```
##### Fuzz
In general the fuzz term controlls the surface roughness of the material. It basically controlls the reflection direction of the rays with a little bias added on the reflection direction of the ray. This is done by generating a random point in the sphere to adjust the reflection ray directions.  
![image](RayTracer/fuzz.png)
```cpp
scattered = ray(rec.p,reflected + fuzz * random_in_unit_sphere());
```
### Glass
Glass is such a special material that when the light hit the surface, it splits into a reflected ray and a refracted ray.This is implemented through randomly choosing between reflection or refraction then generating just one scattered ray every iteration. The picture below shows the  glass ball based on this method.
![image](RayTracer/image_GlassBall.png)
#### Refraction
According to the Snell's law the refraction is defined by the following equation(seen in the picture below)
![image](RayTracer/refraction.png)

And one thing worth mentioning is that when the ray is in some kind of material with higher refractive index and this will lead to no real number solution according to Snell's law. This is called total internal reflection, e.g the light inside water can not go through the surfaces and into the air.Thus the way to compute refraction is more complicated than reflection. The image and code below illustrates how this would be computed.  
![image](RayTracer/RefractionComputing.png)

```cpp
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) 
{
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
```
#### Hollow Glass Ball

One interesting thing to mention about the implementation of this hollow glass ball is that if the radius of the glass ball turns into negative, the geometry of the ball is unchanged due to the r^2 term, but the surface normal would be inverted to point inward. So the hollow glass ball is made up with a bigger ball with glass material and positive radius and a smaller ball with the same material but the radius is negative. The result can be seen in the following picture

![image](RayTracer/image_GlassMat.png)

``` cpp
list[3] = new sphere(vec3(-1,0,-1),0.5f, new glass(1.5f));
list[4] = new sphere(vec3(-1,0,-1),-0.45f, new glass(1.5f));

```
