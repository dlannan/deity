#ifndef N_Quat_H
#define N_Quat_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//-------------------------------------------------------------------
//  Quat
//-------------------------------------------------------------------
class Quat {
public:
    float x,y,z,w;

    //-- constructors -----------------------------------------------
    Quat()
        : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
    {};
    Quat(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w)
    {};
    Quat(const Quat& q)
        : x(q.x), y(q.y), z(q.z), w(q.w)
    {};

    //-- convertors -----------------------------------------------
	Matrix44 ToMatrix44() ;
	void FromMatrix44(Matrix44 mat);

    //-- setting elements -------------------------------------------
    void set(float _x, float _y, float _z, float _w) ;
    void set(const Quat& q);

    //-- misc operations --------------------------------------------
    void ident(void);

    void conjugate(void);

    void scale(float s);

    float norm(void);

    float magnitude(void);

    void invert(void);

    void normalize(void);

    //-- operators --------------------------------------------------
    bool operator==(const Quat& q);

    bool operator!=(const Quat& q);

    const Quat& operator+=(const Quat& q);

    const Quat& operator-=(const Quat& q);

    const Quat& operator*=(const Quat& q);

    /// rotate vector by Quat
    Vector3 rotate(const Vector3& v) ;

    /// convert from two directions, must be unit vectors
    void set_from_axes(const Vector3& from, const Vector3& to) ;

    //-- convert from euler angles ----------------------------------
    void set_rotate_axis_angle(const Vector3& v, float a);

    void set_rotate_x(float a);

    void set_rotate_y(float a);

    void set_rotate_z(float a);

    void set_rotate_xyz(float ax, float ay, float az);

	//--- fuzzy compare operators -----------------------------------
    bool isequal(const Quat& v, float tol) const;

    //-- rotation interpolation, set this matrix to the -------------
    //-- interpolated result of q0->q1 with l as interpolator -------
    void Slerp(const Quat& q0, const Quat& q1, float l);
};

//--- global operators ----------------------------------------------
static inline Quat operator+(const Quat& q0, const Quat& q1) {
    return Quat(q0.x+q1.x, q0.y+q1.y, q0.z+q1.z, q0.w+q1.w);
};

static inline Quat operator-(const Quat& q0, const Quat& q1) {
    return Quat(q0.x-q1.x, q0.y-q1.y, q0.z-q1.z, q0.w-q1.w);
};

static inline Quat operator*(const Quat& q0, const Quat& q1) {
    return Quat(q0.w*q1.x + q0.x*q1.w + q0.y*q1.z - q0.z*q1.y,
                      q0.w*q1.y + q0.y*q1.w + q0.z*q1.x - q0.x*q1.z,
                      q0.w*q1.z + q0.z*q1.w + q0.x*q1.y - q0.y*q1.x,
                      q0.w*q1.w - q0.x*q1.x - q0.y*q1.y - q0.z*q1.z);
};
//-------------------------------------------------------------------
#endif
