#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include "rtweekend.h"
#include "hittable.h"
//�����Ƕ���Ĳ��ϵĳ����ࡣ�为�����ɢ�����/����������ߣ�ͬʱ��������˥��ֵ

//struct hit_record;

class material {
public:
	virtual bool scatter(ray& r_in, hit_record& rec, color& attenuation, ray& scattered) = 0;
};

//ͨ������Ļ��࣬���Ǵ���ģ��������Ĳ��ʶ�����ʼ��ɢ�䣬��ͨ��������R˥��
//�������Ϲ��ɣ����ǿ��Եõ�������
class lambertian : public material {
public:
	color albedo;
public:
	lambertian(color a) :albedo(a) {}
	virtual bool scatter(ray& r_in, hit_record& rec, color& attenuation, ray& scattered) override {
		auto scatter_direction = rec.normal + random_unit_vector();
		//���ڷ�ֹ�����˽����������ķ�����
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;


		scattered = ray(rec.p, scatter_direction, r_in.time());
		attenuation = albedo;
		return true;
	}
};

//�������Ͻ�ʹ�þ��淴�乫ʽ�������
//class metal : public material {
//public:
//	color albedo;
//public:
//	metal(color a) :albedo(a) {}
//
//	virtual bool scatter(ray& r_in, hit_record& rec, color& attenuation, ray& scattered) override {
//		vec3 reflected = reflect(r_in.direction().normalized(), rec.normal);
//		scattered = ray(rec.p, reflected);
//		attenuation = albedo;
//		return (dot(scattered.direction(), rec.normal) > 0);
//	}
//};
//����ĥɰ�Ľ������ʣ�������һ���Ƚ�С��ɢ��Ƕ�
class metal : public material {
public:
    metal(color a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        ray& r_in, hit_record& rec, color& attenuation, ray& scattered
    ) override {
        vec3 reflected = reflect(r_in.direction().normalized(), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_shpere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};
//����Ĳ���
//���￼���Ƿ����ȫ����
class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        ray& r_in, hit_record& rec, color& attenuation, ray& scattered
    ) override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = r_in.direction().normalized();
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }

public:
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif