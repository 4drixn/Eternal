#pragma once

#include "Minecraft.hpp"
#include "vec3.hpp"

struct s_axisalignedbb {
	double minX, minY, minZ,
		maxX, maxY, maxZ;
};

class c_axisalignedbb {
private:
	jobject axis_aligned_obj;
	JNIEnv* env;
public:
	c_axisalignedbb(jobject, JNIEnv* env_);
	~c_axisalignedbb();

	s_axisalignedbb get_native_boundingbox();
	void set_native_boundingbox(s_axisalignedbb target);

	void set(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);

	std::shared_ptr<c_axisalignedbb>  addCoord(double x, double y, double z);
	std::shared_ptr<c_axisalignedbb>  expand(double x, double y, double z);
	std::shared_ptr<c_axisalignedbb>  expand(double x, double y, double z, double x2, double y2, double z2);

	std::shared_ptr<c_axisalignedbb> offset(double x, double y, double z);

	bool isVecInside(std::shared_ptr<c_vec3> vec);
	jobject calculateIntercept(std::shared_ptr<c_vec3> first, std::shared_ptr<c_vec3> second);

	jobject get_object();
};