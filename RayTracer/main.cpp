#include <iostream>
#include <fstream>
#include <string>
#include "ray.h"
#include "hitablelist.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"
#include "material.h"
using namespace std;
//解二次方程的代码实现
float hit_sphere(const vec3& center, float radius, const ray& r){
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(),r.direction());
    float b = 2.0f * dot(oc,r.direction());
    float c = dot(oc,oc) - radius * radius;
    float discriminant = b * b - 4.0f * a * c;
    if(discriminant < 0){
        return -1;//不存在交点
    }
    else{
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }
}
hitable *random_scene(){
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0,-1000,0),1000,new lambertian(vec3(0.5,0.5,0.5)));
    int i = 1;
    for(int a = -11; a < 11; a++){
        for(int b = -11; b < 11; b++){
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
            if((center - vec3(4,0.2,0)).length() > 0.9){
                if(choose_mat < 0.8){
                    list[i++] = new sphere(center,0.2,new lambertian(vec3(drand48()*drand48(),drand48()*drand48(),drand48()*drand48())));
                }
                else if (choose_mat < 0.95){
                    list[i++] = new sphere(center,0.2,new metal(vec3(0.5*(1+drand48()),0.5*(1+drand48()),0.5*(1+drand48())),0.5*drand48()));
                }
                else{
                    list[i++] = new sphere(center,0.2,new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0,1,0),1.0,new dielectric(1.5));
    list[i++] = new sphere(vec3(-4,1,0),1.0,new lambertian(vec3(0.4,0.2,0.1)));
    list[i++] = new sphere(vec3(4,1,0),1.0,new metal(vec3(0.7,0.6,0.5),0.0));
    return new hitable_list(list,i);
}
vec3 color(const ray& r,hitable *world,int depth){
    //float t = hit_sphere(vec3(0,0,-1),0.5f,r);
    hit_record rec;
    if(world->hit(r,0.001f,MAXFLOAT,rec))
    {
        ray scattered ;
        vec3 attenuation;
        if(depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered)){
            return attenuation * color(scattered, world,depth + 1);
        }
        else{
            return vec3(0,0,0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t)*vec3(1,1,1) + t*vec3(0.5f,0.7f,1.0f);
    }

}
int main() {
    int nx = 1000;//定义大小
    int ny = 500;
    int ns = 100;//采样次数
    ofstream myImage;		//输出文件流
    myImage.open("image_random_scene.ppm");//文件路径
    if (myImage.fail())//合法性检验
    {
        cout << "Unable to create image.ppm" << endl;
        getchar();
        getchar();
        return 0;
    }

    myImage << "P3\n" << nx << " " << ny << "\n255\n";//ppm格式头定义
   /* hitable *list[5];
    list[0] = new sphere(vec3(0,0,-1),0.5f, new lambertian(vec3(0.8,0.3,0.3)));
    list[1] = new sphere(vec3(0,-100.5f,-1),100,new lambertian(vec3(0.8,0.8,0.0)));
    list[2] = new sphere(vec3(1,0,-1),0.5f, new metal(vec3(0.8,0.6,0.2),1.0));
    list[3] = new sphere(vec3(-1,0,-1),0.5f, new dielectric(1.5f));//new metal(vec3(0.8,0.8,0.8),0.3));
    list[4] = new sphere(vec3(-1,0,-1),-0.45f, new dielectric(1.5f));//new metal(vec3(0.8,0.8,0.8),0.3));
*/
    hitable *world = random_scene();//new hitable_list(list,5);
    camera cam(vec3(13,2,3),vec3(0,0,0),vec3(0,1,0),20,float(nx)/float(ny) );
    for (int j = ny - 1; j >= 0; j --) {//逐像素点计算颜色（RGB）值
        for (int i = 0;i < nx; i++) {
            vec3 col(0,0,0);
            //重采样ns次
            for(int s = 0; s < ns; s++){
                float  u = float(i + drand48())/float(nx);
                float  v = float(j + drand48())/float(ny);
                ray r = cam.get_ray(u,v);
                col += color(r,world,0);
            }
            col /= float(ns);//求重采样完了的平均值
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99* col[0]);
            int ig = int(255.99* col[1]);
            int ib = int(255.99* col[2]);
            myImage << ir << " " << ig << " " << ib << "\n";//输出计算好的值到文件里面

        }

    }//std::endl;
    myImage.close();
    return 0;
}
