#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

//是基础的三维向量定义，其中包含了向量的基本操作等：
// 初始化：(x, y, z)，默认为0
// 访问：可以通过vec3.e[]方式以及vec3.x()等方式访问
// 运算重载：我们重载了以下方式的运算：
//		vec3 + vec3	vec3 + double	double + vec3	vec3 += vec3	vec3 += double
//		vec3 - vec3	vec3 - double	vec3 -= vec3		vec3 -= double
//		vec3 * vec3	vec3 *= double		double * vec3
//		vec3 / double	vec3 /= double
// 此外，重载了流运算符<<，可用于输出
// 自身运算：
//		length_squared()、length()、normalized()
//		dot(vec3, vec3)、cross(vec3, vec3)
//

using std::sqrt;
using std::cout;
using std::endl;

class vec3 {
public:
	double e[3];
public:
	//初始化函数
	vec3(double e1 = 0, double e2 = 0, double e3 = 0) :e{ e1, e2, e3 } {}

	//x、y、z方式访问函数
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	//四则运算的重载
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	//vec3 operator-(vec3 vec) { return vec3(e[0] - vec[0], e[1] - vec[1], e[2] - vec[2]); }
	vec3 operator-(const vec3& vec) { return vec3(e[0] - vec.x(), e[1] - vec.y(), e[2] - vec.z()); }
	vec3 operator-(const double t) { return vec3(e[0] - t, e[1] - t, e[2] - t); }
	vec3& operator-=(const vec3& vec) {
		e[0] -= vec.x();
		e[1] -= vec.y();
		e[2] -= vec.z();
		return *this;
	}
	vec3& operator-=(const double t) {
		e[0] -= t;
		e[1] -= t;
e[2] -= t;
return *this;
	}
	vec3 operator+(const vec3& vec) { return vec3(e[0] + vec.x(), e[1] + vec.y(), e[2] + vec.z()); }
	vec3 operator+(const double t) { return vec3(e[0] + t, e[1] + t, e[2] + t); }
	vec3& operator+=(const vec3& vec) {//右值引用
		e[0] += vec.x();
		e[1] += vec.y();
		e[2] += vec.z();
		return *this;
	}
	vec3& operator+=(const double t) {
		e[0] += t;
		e[1] += t;
		e[2] += t;
		return *this;
	}
	vec3 operator*(const double t) { return vec3(e[0] * t, e[1] * t, e[2] * t); }
	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	vec3 operator/(const double t) {
		if (t == 0) {
			cout << "divided 0 error" << endl;
			return vec3();
		}
		return *this * (1 / t);
	}
	vec3& operator/=(const double t) {
		if (t == 0) {
			cout << "divided 0 error" << endl;
			vec3 a;
			return a;
		}
		return *this *= (1 / t);
	}
	//重载[ ]运算符，实现脚标访问
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	//定义函数length返回向量长度
	double length_squared() const {
		return pow(e[0], 2) + pow(e[1], 2) + pow(e[2], 2);
	}
	double length() const {
		return sqrt(length_squared());
	}

	vec3 normalized() {
		return vec3(e[0] /= length(),
			e[1] /= length(),
			e[2] /= length());
	}

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	//这是一个功能函数，为了防止我们创建散射方向，使用rec.p+random_unit_vector时得到近似零向量结果导致NAN等不良结果
	//我们使用这个函数，其功能是向量在所有维度都非常接近0那么返回true，用于后续判断
	bool near_zero() {
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}
};

//为方便，定义vec3的别名：color、point3
using point3 = vec3;
using color = vec3;

//其他一些功能的完善
//输出流重载，输出格式为{ x y z }
std::ostream& operator<<(std::ostream& out, vec3&& vec) {
	return out /* << "{ " */ << vec.e[0] << ' ' << vec.e[1] << ' ' << vec.e[2]/*  << " }"*/;
}
std::ostream& operator<<(std::ostream& out, vec3& vec) {
	return out <</* "{ " <<*/ vec.e[0] << ' ' << vec.e[1] << ' ' << vec.e[2] /* << " }"*/;
}

vec3 operator*(double t, const vec3& vec) { return vec3(t * vec.e[0], t * vec.e[1], t * vec.e[2]); }
vec3 operator*(vec3 v1, vec3 v2) { return vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z()); }
vec3 operator+(double t, const vec3& vec) { return t + vec; }
vec3 operator-(double t, const vec3& vec) { return t - vec; }
//vec3 operator/(vec3& v, double t) { return (1 / t) * v; }

double dot(const vec3& u, const vec3& v) {
	return u.x() * v.x()
		+ u.y() * v.y()
		+ u.z() * v.z();
}

vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x());
}

//用于生成漫反射反弹光线
//我们认为漫反射的反弹光线，是在光线照入点对外的一个单位球中随机取一个点，作为方向得到的
vec3 random_in_unit_shpere() {
	while (true) {
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 random_unit_vector() {
	return random_in_unit_shpere().normalized();
}

vec3 random_in_hemisphere(vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_shpere();
	return dot(in_unit_sphere, normal) > 0 ? in_unit_sphere : -in_unit_sphere;
}

//根据入射光v和法线n的方向，生成镜面反射光的方向
vec3 reflect(vec3& v, vec3& n) {
	return v - 2 * dot(v, n) * n;
}
vec3 reflect(vec3&& v, vec3& n) {
	return v - 2 * dot(v, n) * n;
}

//计算折射光方向
vec3 refract(vec3& uv, vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}
#endif