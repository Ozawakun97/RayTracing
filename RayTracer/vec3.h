//
// Created by mac on 2022/6/8.
//

#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H
#include <math.h>
#include <stdlib.h>
#include <iostream>
class  vec3 {
public :
    vec3(){}
    vec3(float e0,float e1,float e2){e[0] = e0; e[1] = e1;e[2] = e2;}//构造函数
    inline float x() const {return e[0];} //获取vector3的分量(x,y,z)或(r,g,b)
    inline float y() const {return e[1];}
    inline float z() const {return e[2];}
    inline float r() const {return e[0];}
    inline float g() const {return e[1];}
    inline float b() const {return e[2];}
    //操作符符重载
    inline const vec3& operator+() const {return  *this;}
    inline vec3 operator-() const{return vec3(-e[0],-e[1],-e[2]);}
    inline float operator[](int i) const {return e[i];}
    inline float& operator[](int i) {return e[i];}
    //运算符重载
    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);
    //需要加上friend
    inline friend std::istream& operator>> (std::istream &is,vec3 &t){
        is >> t.e[0] >> t.e[1] >> t.e[2];
        return is;
    }
    inline friend std::ostream& operator<<(std::ostream &os, const vec3 &t){
        os << t.e[0] << " " << t.e[1] << " " << t.e[2];
        return os;
    }
    //方法
    inline  float length() const{
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }
    inline float  squared_length() const{
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    inline void make_unit_vector();
    float e[3];

};
inline void vec3::make_unit_vector() {
    e[0] /= this->length();
    e[1] /= this->length();
    e[2] /= this->length();
}
inline vec3 operator+(const vec3 &v1, const vec3 &v2){
    return vec3(v1.e[0]+v2.e[0],v1.e[1]+v2.e[1],v1.e[2]+v2.e[2]);
}
inline vec3 operator-(const vec3 &v1, const vec3 &v2){
    return vec3(v1.e[0]-v2.e[0],v1.e[1]-v2.e[1],v1.e[2]-v2.e[2]);
}
//这个奇奇怪怪的乘法我也没有看懂是干啥的
inline vec3 operator* (const vec3 &v1, const vec3 &v2){
    return vec3(v1.e[0]* v2.e[0],v1.e[1]*v2.e[1],v1.e[2]*v2.e[2]);
}
inline vec3 operator* (float t, const vec3 &v){
    return vec3(t * v.e[0],t * v.e[1],t * v.e[2]);
}inline vec3 operator* (const vec3 &v,float t){
    return vec3(t * v.e[0],t * v.e[1],t * v.e[2]);
}
inline vec3 operator/ (const vec3 &v1, const vec3 &v2){
    return vec3(v1.e[0]/ v2.e[0],v1.e[1]/v2.e[1],v1.e[2]/v2.e[2]);
}
inline vec3 operator/ (float t, const vec3 &v){
    return vec3( v.e[0] / t, v.e[1] / t,v.e[2] / t);
}
inline vec3 operator/ ( const vec3 &v,float t){
    return vec3( v.e[0] / t, v.e[1] / t,v.e[2] / t);
}
inline float dot(const vec3 &v1, const vec3 &v2){
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}
vec3 reflect(const vec3& v, const vec3& n){
    return v - 2* dot(v,n)*n;
}
inline vec3 cross(const vec3 &v1, const vec3 &v2){
    return vec3((v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
                (v1.e[2]*v2.e[0] - v1.e[0] * v2.e[2]),
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])
                );
}
inline vec3& vec3::operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}
inline vec3& vec3::operator*=(const vec3 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}
inline vec3& vec3::operator/=(const vec3 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}
inline vec3& vec3::operator-=(const vec3 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}
inline vec3& vec3::operator*=(const float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}
inline vec3& vec3::operator/=(const float t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}
inline vec3 unit_vector(vec3 v){
    return v / v.length();
}
// 计算折射向量的代码
// 参数 uv 为入射向量，需为单位向量，即上面推导公式的V向量
// 参数 n 为表面法线向量，需为单位向量，即上面推导公式的N向量
// 参数 etai_over_etat为 介质的折射率
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squared_length())) * n;
    return r_out_perp + r_out_parallel;
}
#endif //RAYTRACER_VEC3_H
