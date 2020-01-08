
#define M_PI       3.1415926535f

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Matrix.h"
#include "Quat.h"

//-------------------------------------------------------------------
//  Quat
//-------------------------------------------------------------------

	//-- convertors -----------------------------------------------
	Matrix44 Quat::ToMatrix44() 
	{
		Matrix44	mat;
//		float *m	= (float *)&mat;

		float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
		x2 = x + x; y2 = y + y; z2 = z + z;
		xx = x * x2;   xy = x * y2;   xz = x * z2;
		yy = y * y2;   yz = y * z2;   zz = z * z2;
		wx = w * x2;   wy = w * y2;   wz = w * z2;

		mat.right.x = (1.0f - (yy + zz));
		mat.right.y = (xy + wz);
		mat.right.z = (xz - wy);

		mat.up.x = (xy - wz);
		mat.up.y = (1.0f - (xx + zz));
		mat.up.z = (yz + wx);

		mat.view.x = (xz + wy);
		mat.view.y = (yz - wx);
		mat.view.z = (1.0f - (xx + yy));

//		m[12] = m[13] = m[14] = 0.0f;
//		m[3] = m[7] = m[11] = 0.0f;
//		m[15] = 1.0f;

		return mat;
	}

	void Quat::FromMatrix44(Matrix44 mat)
	{
		float qa[4];
		float *m = (float *)&mat;
		float tr = m[0] + m[5] + m[10];
		if (tr > 0.0f) 
		{
			float s = sqrtf (tr + 1.0f);
			qa[3] = s * 0.5f;
			s = 0.5f / s;
			qa[0] = (m[9] - m[6]) * s;
			qa[1] = (m[2] - m[8]) * s;
			qa[2] = (m[4] - m[1]) * s;
		} 
		else 
		{
			int i, j, k, nxt[3] = {1,2,0};
			i = 0;
			if (m[5] > m[0]) i=1;
			if (m[10] > m[i * 4 + i]) i=2;
			j = nxt[i];
			k = nxt[j];
			float s = sqrtf((m[i * 4 + i] - (m[j * 4 + j] + m[k * 4 + k])) + 1.0f);
			qa[i] = s * 0.5f;
			s = 0.5f / s;
			qa[3] = (m[k * 4 + j] - m[j * 4 + k]) * s;
			qa[j] = (m[j * 4 + i] + m[i * 4 + j]) * s;
			qa[k] = (m[k * 4 + i] + m[i * 4 + k]) * s;
		}

		x = qa[0];
		y = qa[1];
		z = qa[2];
		w = qa[3];
	}

    //-- setting elements -------------------------------------------
    void Quat::set(float _x, float _y, float _z, float _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    };
    void Quat::set(const Quat& q) {
        x = q.x;
        y = q.y;
        z = q.z;
        w = q.w;
    };

    //-- misc operations --------------------------------------------
    void Quat::ident(void) {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;
    };

    void Quat::conjugate(void) {
        x = -x;
        y = -y;
        z = -z;
    };

    void Quat::scale(float s) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
    };

    float Quat::norm(void) {
        return x*x + y*y + z*z + w*w;
    };

    float Quat::magnitude(void) {
        float n = norm();
        if (n > 0.0f) return sqrtf(n);
        else          return 0.0f;
    };

    void Quat::invert(void) {
        float n = norm();
        if (n > 0.0f) scale(1.0f / norm());
        conjugate();
    };

    void Quat::normalize(void) {
        float l = magnitude();
        if (l > 0.0f) scale(1.0f / l);
        else          set(0.0f,0.0f,0.0f,1.0f);
    };

    //-- operators --------------------------------------------------
    bool Quat::operator==(const Quat& q) {
        return ((x==q.x) && (y==q.y) && (z==q.z) && (w==q.w)) ? true : false;
    };

    bool Quat::operator!=(const Quat& q) {
        return ((x!=q.x) || (y!=q.y) || (z!=q.z) || (w!=q.w)) ? true : false;
    };

    const Quat& Quat::operator+=(const Quat& q) {
        x += q.x;
        y += q.y;
        z += q.z;
        w += q.w;
        return *this;
    };

    const Quat& Quat::operator-=(const Quat& q) {
        x -= q.x;
        y -= q.y;
        z -= q.z;
        w -= q.w;
        return *this;
    };

    const Quat& Quat::operator*=(const Quat& q) {
        float qx = w*q.x + x*q.w + y*q.z - z*q.y;
        float qy = w*q.y + y*q.w + z*q.x - x*q.z;
        float qz = w*q.z + z*q.w + x*q.y - y*q.x;
        float qw = w*q.w - x*q.x - y*q.y - z*q.z;
        x = qx;
        y = qy;
        z = qz;
        w = qw;
        return *this;
    };

    /// rotate vector by Quat
    Vector3 Quat::rotate(const Vector3& v) {
        Quat q(v.x * w + v.z * y - v.y * z,
                     v.y * w + v.x * z - v.z * x,
                     v.z * w + v.y * x - v.x * y,
                     v.x * x + v.y * y + v.z * z);

        return Vector3(w * q.x + x * q.w + y * q.z - z * q.y,
                       w * q.y + y * q.w + z * q.x - x * q.z,
                       w * q.z + z * q.w + x * q.y - y * q.x);
    };

    /// convert from two directions, must be unit vectors
    void Quat::set_from_axes(const Vector3& from, const Vector3& to) 
    {
    };

    //-- convert from euler angles ----------------------------------
    void Quat::set_rotate_axis_angle(const Vector3& v, float a) {
        float sin_a = sinf(a * 0.5f);
        float cos_a = cosf(a * 0.5f);
        x = v.x * sin_a;
        y = v.y * sin_a;
        z = v.z * sin_a;
        w = cos_a;
    };

    void Quat::set_rotate_x(float a) {
        float sin_a = sinf(a * 0.5f);
        float cos_a = cosf(a * 0.5f);
        x = sin_a;
        y = 0.0f;
        z = 0.0f;
        w = cos_a;
    };

    void Quat::set_rotate_y(float a) {
        float sin_a = sinf(a * 0.5f);
        float cos_a = cosf(a * 0.5f);
        x = 0.0f;
        y = sin_a;
        z = 0.0f;
        w = cos_a;
    };

    void Quat::set_rotate_z(float a) {
        float sin_a = sinf(a * 0.5f);
        float cos_a = cosf(a * 0.5f);
        x = 0.0f;
        y = 0.0f;
        z = sin_a;
        w = cos_a;
    };

    void Quat::set_rotate_xyz(float ax, float ay, float az) {
        Quat qx, qy, qz;
        qx.set_rotate_x(ax);
        qy.set_rotate_y(ay);
        qz.set_rotate_z(az);
        *this = qx;
        *this *= qy;
        *this *= qz;
    };

	//--- fuzzy compare operators -----------------------------------
    bool Quat::isequal(const Quat& v, float tol) const
    {
        if (fabs(v.x-x) > tol)      return false;
        else if (fabs(v.y-y) > tol) return false;
        else if (fabs(v.z-z) > tol) return false;
        else if (fabs(v.w-w) > tol) return false;
        return true;
    };

    //-- rotation interpolation, set this matrix to the -------------
    //-- interpolated result of q0->q1 with l as interpolator -------
	void Quat::Slerp(const Quat& q0, const Quat& q1, float l) 
    {
        float fScale1;
        float fScale2;
        Quat A = q0;
        Quat B = q1;

        // compute dot product, aka cos(theta):
        float fCosTheta = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;

        if (fCosTheta < 0.0f) 
        {
            // flip start Quat
           A.x = -A.x; A.y = -A.y; A.z = -A.z; A.w = -A.w;
           fCosTheta = -fCosTheta;
        }

        if ((fCosTheta + 1.0f) > 0.05f) 
        {
            // If the Quats are close, use linear interploation
            if ((1.0f - fCosTheta) < 0.05f) 
            {
                fScale1 = 1.0f - l;
                fScale2 = l;
            }
            else 
            { 
                // Otherwise, do spherical interpolation
                float fTheta    = acosf(fCosTheta);
                float fSinTheta = sinf(fTheta);
                fScale1 = sinf( fTheta * (1.0f-l) ) / fSinTheta;
                fScale2 = sinf( fTheta * l ) / fSinTheta;
            }
        }
        else 
        {
            B.x = -A.y;
            B.y =  A.x;
            B.z = -A.w;
            B.w =  A.z;
            fScale1 = sinf( M_PI * (0.5f - l) );
            fScale2 = sinf( M_PI * l );
        }

        x = fScale1 * A.x + fScale2 * B.x;
        y = fScale1 * A.y + fScale2 * B.y;
        z = fScale1 * A.z + fScale2 * B.z;
        w = fScale1 * A.w + fScale2 * B.w;
    };


//-------------------------------------------------------------------
