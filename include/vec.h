#ifndef INCLUDED_VECTOR3F_H
#define INCLUDED_VECTOR3F_H
#include <iostream>
#include <math.h>

#include "BitStream.h"

//-----------------------------------------------
//------------------   VEC   --------------------
//-----------------------------------------------
template<typename tX, size_t dim>
class vec {
protected:
    tX internal[dim];

    vec(const tX *data) {
        for(size_t i=0; i<dim; i++) {
            internal[i] = data[i];
        }
    }
public:
    vec() {}

    vec(tX s) {
        for(size_t i=0; i<dim; i++) {
            internal[i] = 0;
        }
    }

    vec(const vec &v) {
		for(size_t i=0; i<dim; i++) {
			internal[i] = v.internal[i];
		}
    }

    const tX* data() const {
        return internal;
    }

    vec& operator=(const vec &v) {
        vec(v.internal);
        return *this;
    }

	vec& operator+=(const vec &v) {
	    for(size_t i=0; i<dim; i++) {
            internal[i] += v.internal[i];
	    }
		return *this;
	}

	vec& operator-=(const vec &v) {
	    for(size_t i=0; i<dim; i++) {
            internal[i] -= v.internal[i];
	    }
		return *this;
	}

	vec operator*(tX s) const {
		vec res;
		for(size_t i=0; i<dim; i++) {
            res.internal[i] = internal[i]*s;
		}
		return res;
	}

	vec operator/(tX s) const {
		vec res;
		for(size_t i=0; i<dim; i++) {
            res.internal[i] = internal[i]/s;
		}
		return res;
	}

    template<typename tY, size_t dimy>
	friend vec<tY, dimy> operator/(const vec<tY,dimy> &u, const vec<tY,dimy> &v);

	template<typename tY, size_t dimy>
	friend vec<tY, dimy> operator+(const vec<tY,dimy> &u, const vec<tY,dimy> &v);

	template<typename tY, size_t dimy>
	friend vec<tY, dimy> operator-(const vec<tY,dimy> &u, const vec<tY,dimy> &v);

	template<typename tY, size_t dimy>
	friend vec<tY, dimy> operator-(const vec<tY, dimy> &v);

	template<typename tY, size_t dimy>
	friend tY length(const vec<tY,dimy> &v);

	template<typename tY, size_t dimy>
	friend tY dot(const vec<tY, dimy> &v1, const vec<tY, dimy> &v2);

	template<typename tY, size_t dimy>
	friend vec<tY, dimy> normalize(const vec<tY, dimy> &v);
};

template<typename tX, size_t dim>
vec<tX,dim> operator/(const vec<tX,dim> &u, const vec<tX,dim> &v) {
	vec<tX,dim> ret;
	for(size_t i=0; i<dim; i++) {
        ret.internal[i] = u.internal[i]/v.internal[i];
	}
	return ret;
}

template<typename tX, size_t dim>
vec<tX,dim> operator+(const vec<tX,dim> &u, const vec<tX,dim> &v) {
	vec<tX,dim> ret;
	for(size_t i=0; i<dim; i++) {
        ret.internal[i] = u.internal[i]+v.internal[i];
	}
	return ret;
}

template<typename tX, size_t dim>
vec<tX,dim> operator-(const vec<tX,dim> &u, const vec<tX,dim> &v) {
	vec<tX,dim> ret;
	for(size_t i=0; i<dim; i++) {
        ret.internal[i] = u.internal[i]-v.internal[i];
	}
	return ret;
}

template<typename tX, size_t dim>
vec<tX, dim> operator-(const vec<tX, dim> &v) {
	vec<tX, dim> res;
    for(size_t i=0; i<dim; i++) {
        res.internal[i] = -v.internal[i];
    }
    return res;
}

template<typename tX, size_t dim>
tX dot(const vec<tX, dim> &v1, const vec<tX, dim> &v2) {
    tX res=0;
    for(size_t i=0; i<dim; i++) {
        res += v1.internal[i]*v2.internal[i];
    }
    return res;
}

template<typename tX, size_t dim>
tX length(const vec<tX, dim> &v) {
	return sqrt(dot(v,v));
}

template<typename tX, size_t dim>
vec<tX, dim> normalize(const vec<tX, dim> &v) {
	return vec<tX,dim>(v/length(v));
}

//-----------------------------------------------
//----------------   VEC2,3,4  ------------------
//-----------------------------------------------

template<typename tX>
class vec2 : public vec<tX,2> {
public:
	vec2(const vec<tX,2> &v) : vec<tX,2>(v) {}

	vec2& operator=(const vec2 &v) {
		this->internal[0] = v.internal[0];
        this->internal[1] = v.internal[1];
		return *this;
	}

    vec2(const vec2 &v) : vec2(v.internal[0],v.internal[1]) {}
    vec2(tX s) : vec2(s,s) {}
	vec2(tX x, tX y) {
        this->internal[0] = x;
        this->internal[1] = y;
	}

	tX& x() { return this->internal[0]; }
	tX& y() { return this->internal[1]; }

	tX x() const { return this->internal[0]; }
	tX y() const { return this->internal[1]; }
};
typedef vec2<float> vec2f;

inline RakNet::BitStream& operator<<(RakNet::BitStream& out, vec2f& in) {
    out.Write(in.x());
    out.Write(in.y());
    return out;
}

inline RakNet::BitStream& operator>>(RakNet::BitStream& in, vec2f& out) {
    in.Read(out.x());
    in.Read(out.y());
    return in;
}

template<typename tX>
class vec3 : public vec<tX,3> {
public:
    vec3(const vec<tX,3> &v) : vec<tX,3>(v) {}

	vec3& operator=(const vec3 &v) {
		this->internal[0] = v.internal[0];
        this->internal[1] = v.internal[1];
        this->internal[2] = v.internal[2];
		return *this;
	}

    vec3(const vec3 &v) : vec3(v.internal[0],v.internal[1],v.internal[2]) {}
    vec3(tX s) : vec3(s,s,s) {}
	vec3(tX x, tX y, tX z) {
	    this->internal[0] = x;
        this->internal[1] = y;
        this->internal[2] = z;
	}

	tX& x() { return this->internal[0]; }
	tX& y() { return this->internal[1]; }
	tX& z() { return this->internal[2]; }

    //If vec is const, return by value instead
	tX x() const { return this->internal[0]; }
	tX y() const { return this->internal[1]; }
	tX z() const { return this->internal[2]; }
};
typedef vec3<float> vec3f;

inline RakNet::BitStream& operator<<(RakNet::BitStream& out, vec3f& in) {
    out.WriteVector(in.x(), in.y(), in.z());
    return out;
}

inline RakNet::BitStream& operator>>(RakNet::BitStream& in, vec3f& out) {
    in.ReadVector(out.x(), out.y(), out.z());
    return in;
}

template<typename tX>
class vec4 : public vec<tX,4> {
public:
    vec4(const vec<tX,4> &v) : vec<tX,4>(v) {}

	vec4& operator=(const vec4 &v) {
		this->internal[0] = v.internal[0];
        this->internal[1] = v.internal[1];
        this->internal[2] = v.internal[2];
		this->internal[3] = v.internal[3];
		return *this;
	}

    vec4(const vec4 &v) : vec4(v.internal[0],v.internal[1],v.internal[2],v.internal[3]) {}
    vec4(tX s) : vec4(s,s,s,s) {}
    vec4(vec3<tX> u, tX w) : vec4(u.x(), u.y(), u.z(), w) {}
	vec4(tX x, tX y, tX z, tX w) {
	    this->internal[0] = x;
        this->internal[1] = y;
        this->internal[2] = z;
		this->internal[3] = w;
	}

	vec3<tX> xyz() {
	    return vec3<tX>(this->internal[0],this->internal[1],this->internal[2]);
    }

	tX& x() { return this->internal[0]; }
	tX& y() { return this->internal[1]; }
	tX& z() { return this->internal[2]; }
	tX& w() { return this->internal[3]; }
};
typedef vec4<float> vec4f;

inline RakNet::BitStream& operator<<(RakNet::BitStream& out, vec4f& in) {
    out.WriteVector(in.x(), in.y(), in.z());
    out.Write(in.w());
    return out;
}

inline RakNet::BitStream& operator>>(RakNet::BitStream& in, vec4f& out) {
    in.ReadVector(out.x(), out.y(), out.z());
    in.Read(out.w());
    return in;
}

#endif
