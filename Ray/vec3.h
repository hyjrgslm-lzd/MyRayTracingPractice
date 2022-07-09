#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

//�ǻ�������ά�������壬���а����������Ļ��������ȣ�
// ��ʼ����(x, y, z)��Ĭ��Ϊ0
// ���ʣ�����ͨ��vec3.e[]��ʽ�Լ�vec3.x()�ȷ�ʽ����
// �������أ��������������·�ʽ�����㣺
//		vec3 + vec3	vec3 + double	double + vec3	vec3 += vec3	vec3 += double
//		vec3 - vec3	vec3 - double	vec3 -= vec3		vec3 -= double
//		vec3 * vec3	vec3 *= double		double * vec3
//		vec3 / double	vec3 /= double
// ���⣬�������������<<�����������
// �������㣺
//		length_squared()��length()��normalized()
//		dot(vec3, vec3)��cross(vec3, vec3)
//

using std::sqrt;
using std::cout;
using std::endl;

class vec3 {
public:
	double e[3];
public:
	//��ʼ������
	vec3(double e1 = 0, double e2 = 0, double e3 = 0) :e{ e1, e2, e3 } {}

	//x��y��z��ʽ���ʺ���
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	//�������������
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
	vec3& operator+=(const vec3& vec) {//��ֵ����
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
	//����[ ]�������ʵ�ֽű����
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	//���庯��length������������
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

	//����һ�����ܺ�����Ϊ�˷�ֹ���Ǵ���ɢ�䷽��ʹ��rec.p+random_unit_vectorʱ�õ������������������NAN�Ȳ������
	//����ʹ������������书��������������ά�ȶ��ǳ��ӽ�0��ô����true�����ں����ж�
	bool near_zero() {
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}
};

//Ϊ���㣬����vec3�ı�����color��point3
using point3 = vec3;
using color = vec3;

//����һЩ���ܵ�����
//��������أ������ʽΪ{ x y z }
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

//�������������䷴������
//������Ϊ������ķ������ߣ����ڹ������������һ����λ�������ȡһ���㣬��Ϊ����õ���
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

//���������v�ͷ���n�ķ������ɾ��淴���ķ���
vec3 reflect(vec3& v, vec3& n) {
	return v - 2 * dot(v, n) * n;
}
vec3 reflect(vec3&& v, vec3& n) {
	return v - 2 * dot(v, n) * n;
}

//��������ⷽ��
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