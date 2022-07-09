#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H
//hittable.h是一个抽象类，包含一个接受光线命中的函数与光线命中点后返回信息的结构体
//它是后面创建物体的基础，计算光线与物体碰撞时，通过这里的函数实现

//为了添加多种材质，我们增加可命中类material，以及一个指向他的指针 

#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"

class material;

//定义光线命中后返回的结构体
//它本身只提供一种将一系列参数填充到结构中的方法。
struct hit_record {
	//光线命中的点
	point3 p;
	//这个点表面的法线
	vec3 normal;
	//指向材料的指针
	shared_ptr<material> mat_ptr;
	//光线命中时的t
	double t;
	//表明光线是否是照到物体对外的表面（即光线从物体外照到物体表面）
	bool front_face;
	//根据光线和法线点乘的正负来确定法线从物体指向物体外的方向，并将其设为normal
	//实际上，让法线始终指向光线方向和法线始终指向物体外侧，对应在着色时还是几何阶段判定曲面的方向。
	//因为法线始终指向物体外侧相对容易理解，且资料较多，这里采用这种实现
	void set_face_normal(const ray r, const vec3 outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

//定义光线命中类，其中包含一个计算光线是否命中并返回hit_record结构体的函数
//在具体对象时，我们再实现对应的hit函数，因此这里定义为纯虚函数
class hittable {
public:
	virtual bool hit(ray& r, double t_min, double t_max, hit_record& rec)=0;

	//在定义AABB包围盒后，我们要添加一个函数来计算当前可命中目标的包围盒
	//在此基础上，我们创建BVH结构，对其进行划分
	//所有实际上的物体最后会被分配到叶子节点中
	//函数返回一个bool，因为对一些物体，比如无限平面，不需要包围盒
	//对于运动物体，要有一个能够包围住其整个运动空间的包围盒
	virtual bool bounding_box(double time0, double time1, aabb& output_box) = 0;
};
#endif