// General Matrix Utilities

// Matrix Structure
#include <math.h>
#include "Matrix.h"

__declspec(naked) void PIII_Mult44( float *src1, float *src2, float *dst)
{
	__asm 
	{
		mov edx, dword ptr [esp+4] ; src1
		mov eax, dword ptr [esp+0Ch] ; dst
		mov ecx, dword ptr [esp+8] ; src2
		movss xmm0, dword ptr [edx]
		movaps xmm1, xmmword ptr [ecx]
		shufps xmm0, xmm0, 0
		movss xmm2, dword ptr [edx+4]
		mulps xmm0, xmm1
		shufps xmm2, xmm2, 0
		movaps xmm3, xmmword ptr [ecx+10h]
		movss xmm7, dword ptr [edx+8]
		mulps xmm2, xmm3
		shufps xmm7, xmm7, 0
		addps xmm0, xmm2
		movaps xmm4, xmmword ptr [ecx+20h]
		movss xmm2, dword ptr [edx+0Ch]
		mulps xmm7, xmm4
		shufps xmm2, xmm2, 0
		addps xmm0, xmm7
		movaps xmm5, xmmword ptr [ecx+30h]
		movss xmm6, dword ptr [edx+10h]
		mulps xmm2, xmm5
		movss xmm7, dword ptr [edx+14h]
		shufps xmm6, xmm6, 0
		addps xmm0, xmm2
		shufps xmm7, xmm7, 0
		movlps qword ptr [eax], xmm0
		movhps qword ptr [eax+8], xmm0
		mulps xmm7, xmm3
		movss xmm0, dword ptr [edx+18h]
		mulps xmm6, xmm1
		shufps xmm0, xmm0, 0
		addps xmm6, xmm7
		mulps xmm0, xmm4
		movss xmm2, dword ptr [edx+24h]
		addps xmm6, xmm0
		movss xmm0, dword ptr [edx+1Ch]
		movss xmm7, dword ptr [edx+20h]
		shufps xmm0, xmm0, 0
		shufps xmm7, xmm7, 0
		mulps xmm0, xmm5
		mulps xmm7, xmm1
		addps xmm6, xmm0
		shufps xmm2, xmm2, 0
		movlps qword ptr [eax+10h], xmm6
		movhps qword ptr [eax+18h], xmm6
		mulps xmm2, xmm3
		movss xmm6, dword ptr [edx+28h]
		addps xmm7, xmm2
		shufps xmm6, xmm6, 0
		movss xmm2, dword ptr [edx+2Ch]
		mulps xmm6, xmm4
		shufps xmm2, xmm2, 0
		addps xmm7, xmm6
		mulps xmm2, xmm5
		movss xmm0, dword ptr [edx+34h]
		addps xmm7, xmm2
		shufps xmm0, xmm0, 0
		movlps qword ptr [eax+20h], xmm7
		movss xmm2, dword ptr [edx+30h]
		movhps qword ptr [eax+28h], xmm7
		mulps xmm0, xmm3
		shufps xmm2, xmm2, 0
		movss xmm6, dword ptr [edx+38h]
		mulps xmm2, xmm1
		shufps xmm6, xmm6, 0
		addps xmm2, xmm0
		mulps xmm6, xmm4
		movss xmm7, dword ptr [edx+3Ch]
		shufps xmm7, xmm7, 0
		addps xmm2, xmm6
		mulps xmm7, xmm5
		addps xmm2, xmm7
		movaps xmmword ptr [eax+30h], xmm2
	}
	__asm ret
}


__declspec(naked) void PIII_Mult44Vec4( float *src1, float *src2, float *dst)
{
	__asm 
	{
		mov ecx, dword ptr [esp+8] ; src2
		mov edx, dword ptr [esp+4] ; src1
		movss xmm0, dword ptr [ecx]
		mov eax, dword ptr [esp+0Ch] ; dst
		shufps xmm0, xmm0, 0
		movss xmm1, dword ptr [ecx+4]
		mulps xmm0, xmmword ptr [edx]
		shufps xmm1, xmm1, 0
		movss xmm2, dword ptr [ecx+8]
		mulps xmm1, xmmword ptr [edx+16]
		shufps xmm2, xmm2, 0
		movss xmm3, dword ptr [ecx+12]
		mulps xmm2, xmmword ptr [edx+32]
		shufps xmm3, xmm3, 0
		addps xmm0, xmm1
		mulps xmm3, xmmword ptr [edx+48]
		addps xmm2, xmm3
		addps xmm0, xmm2
		movaps xmmword ptr [eax], xmm0
	}
	__asm ret
}


Vector4 Matrix44::operator * (const Vector4 &v) const
{
	__declspec(align(16)) Vector4 dest;
	__declspec(align(16)) Matrix44 src1 = *this;
	__declspec(align(16)) Vector4 src2 = v;
	PIII_Mult44Vec4( (float *)&src1, (float *)&src2, (float *)&dest);
	return dest;
}

Matrix44	Matrix44::operator * (const Matrix44 &m) const 
{ 
	__declspec(align(16)) Matrix44 dest;
	__declspec(align(16)) Matrix44 src1 = *this;
	__declspec(align(16)) Matrix44 src2 = m;
	PIII_Mult44((float *)&src1, (float *)&src2, (float *)&dest);
	return dest;
}

// Make a rotation matrix using the Axis/Angle method
void	Matrix44::RotationAA(float x, float y, float z, float t)
{
}

// Vector 2 inlines
Vector2::Vector2()					{ x = 0.0f; y = 0.0f; }
Vector2::Vector2(float x, float y)	{	Vector2::x = x; Vector2::y = y; }
Vector2::Vector2(const Vector2 &v)	{	*this = v; }

Vector2	Vector2::operator + () const { return *this; }
Vector2	Vector2::operator - () const { return Vector2(-x, -y); }
Vector2	Vector2::operator + (const Vector2 &v) const { return Vector2(x+v.x, y+v.y); }
Vector2	Vector2::operator - (const Vector2 &v) const { return Vector2(x-v.x, y-v.y); }
Vector2	Vector2::operator / (const float f) const { float	oof = 1.0f / f;	return Vector2(x*oof, y*oof); }
Vector2	Vector2::operator * (const float f) const { return Vector2(x*f, y*f); }
Vector2	Vector2::operator * (const Vector2 &v) const { return Vector2(x*v.x, y*v.y); }

void		Vector2::operator  = (const Vector2 &v) { x = v.x; y = v.y; }
void		Vector2::operator += (const Vector2 &v) { *this = *this + v; }
void		Vector2::operator -= (const Vector2 &v) { *this = *this - v; }
void		Vector2::operator *= (const float f) { *this = *this * f; }
void		Vector2::operator *= (const Vector2 &v) { *this = *this * v; }
void		Vector2::operator /= (const float f) { *this = *this / f; }
int			Vector2::operator == (const Vector2 &v) { return x==v.x && y==v.y; }
int			Vector2::operator != (const Vector2 &v) { return !(*this==v); }
float&		Vector2::operator [] (const int i) {	return ((float*)this)[i]; }
float&		Vector2::operator [] (const int i) const { return ((float*)this)[i]; }

void		Vector2::Set(float x, float y) {	Vector2::x = x; Vector2::y = y; }

float		Vector2::Len(void) { return sqrtf(Vector2::x * Vector2::x + Vector2::y * Vector2::y); }

void		Vector2::Lerp(const Vector2 &p1, const Vector2 &p2, float t)
{
	const Vector2		s = p2 - p1;
	x = s.x * t;
	y = s.y * t;
}

void		Vector2::Normalize()
{
	Vector2::x /= Vector2::Len();
	Vector2::y /= Vector2::Len();
}

float		Vector2::Dot(const Vector2 &src)
{
	return (Vector2::x * src.x + Vector2::y * src.y); 
}

// Vector 3 s
Vector3::Vector3()							{ x = 0.0f; y = 0.0f; z = 0.0f; }
Vector3::Vector3(float x, float y, float z) {	Vector3::x = x; Vector3::y = y; Vector3::z = z; }
Vector3::Vector3(const Vector3 &v)			{	*this = v; }

Vector3		Vector3::operator + () const { return *this; }
Vector3		Vector3::operator - () const { return Vector3(-x, -y, -z); }
Vector3		Vector3::operator + (const Vector3 &v) const { return Vector3(x+v.x, y+v.y ,z+v.z); }
Vector3		Vector3::operator - (const Vector3 &v) const { return Vector3(x-v.x, y-v.y ,z-v.z); }
Vector3		Vector3::operator / (const float f) const { float	oof = 1.0f / f;	return Vector3(x*oof, y*oof, z*oof); }
Vector3		Vector3::operator * (const float f) const { return Vector3(x*f, y*f, z*f); }
Vector3		Vector3::operator * (const Vector3 &v) const { return Vector3(x*v.x, y*v.y, z*v.z); }

void		Vector3::operator  = (const Vector3 &v) { x = v.x; y = v.y; z = v.z; }
void		Vector3::operator  = (const Vector4 &v) { x = v.x; y = v.y; z = v.z; }
void		Vector3::operator += (const Vector3 &v) { *this = *this + v; }
void		Vector3::operator -= (const Vector3 &v) { *this = *this - v; }
void		Vector3::operator *= (const float f) { *this = *this * f; }
void		Vector3::operator *= (const Vector3 &v) { *this = *this * v; }
void		Vector3::operator /= (const float f) { *this = *this / f; }
int			Vector3::operator == (const Vector3 &v) { return x==v.x && y==v.y && z==v.z; }
int			Vector3::operator != (const Vector3 &v) { return !(*this==v); }
float&		Vector3::operator [] (const int i) {	return ((float*)this)[i]; }
float&		Vector3::operator [] (const int i) const { return ((float*)this)[i]; }

void		Vector3::Set(float x, float y, float z) {	Vector3::x = x; Vector3::y = y; Vector3::z = z; }
float		Vector3::Dot(const Vector3 &p1) { return ( Vector3::x * p1.x + Vector3::y * p1.y + Vector3::z * p1.z ); }
float		Vector3::Len(void) const
{ 
	float len = sqrtf(Vector3::x * Vector3::x + Vector3::y * Vector3::y + Vector3::z * Vector3::z);
	return len; 
}

float	Vector3::Len2(void) const
{ 
	float len = Vector3::x * Vector3::x + Vector3::y * Vector3::y + Vector3::z * Vector3::z;
	return len; 
}

float	Vector3::Distance(const Vector3 &v2) const
{ 
	Vector3 vec(v2.x - x, v2.y - y, v2.z - z);
    return vec.Len(); 
}

float	Vector3::Distance2(const Vector3 &v2) const
{ 
	Vector3 vec(v2.x - x, v2.y - y, v2.z - z);
    return vec.Len2(); 
}

void		Vector3::Lerp(const Vector3 &p1, const Vector3 &p2, float t)
{
	const Vector3		s = p2 - p1;
	x = s.x * t;
	y = s.y * t;
	z = s.z * t;
}

float		Vector3::Normalize()
{
	float l = Vector3::Len();
	if(l == 0.0f) return 0.0f;
	float l2 = 1.0f/l;
	Vector3::x *= l2;
	Vector3::y *= l2;
	Vector3::z *= l2;
	return l;
}

Vector3	Vector3::Cross(const Vector3 &src) const
{
	return Vector3(Vector3::y * src.z - Vector3::z * src.y, Vector3::z * src.x - Vector3::x * src.z, Vector3::x * src.y - Vector3::y * src.x);
}

// Vector4 s
Vector4::Vector4()										{ x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }
Vector4::Vector4(float x, float y, float z, float w)	{	Vector4::x = x; Vector4::y = y; Vector4::z = z; Vector4::w = w; }
Vector4::Vector4(const Vector4 &v)						{	*this = v; }

Vector4		Vector4::operator + () const { return *this; }
Vector4		Vector4::operator - () const { return Vector4(-x, -y, -z, -w); }
Vector4		Vector4::operator + (const Vector4 &v) const { return Vector4(x+v.x, y+v.y ,z+v.z ,w+v.w); }
Vector4		Vector4::operator - (const Vector4 &v) const { return Vector4(x-v.x, y-v.y ,z-v.z ,w-v.w); }
Vector4		Vector4::operator / (const float f) const { float	oof = 1.0f / f;	return Vector4(x*oof, y*oof, z*oof, w*oof); }
Vector4		Vector4::operator * (const float f) const { return Vector4(x*f, y*f, z*f, w*f); }
Vector4		Vector4::operator * (const Vector4 &v) const { return Vector4(x*v.x, y*v.y, z*v.z, w*v.w); }

void		Vector4::operator  = (const Vector4 &v) { x = v.x; y = v.y; z = v.z; w = v.w; }
void		Vector4::operator  = (const Vector3 &v) { x = v.x; y = v.y; z = v.z; w = 0.0f; }
void		Vector4::operator += (const Vector4 &v) { *this = *this + v; }
void		Vector4::operator -= (const Vector4 &v) { *this = *this - v; }
void		Vector4::operator *= (const float f) { *this = *this * f; }
void		Vector4::operator *= (const Vector4 &v) { *this = *this * v; }
void		Vector4::operator /= (const float f) { *this = *this / f; }
int			Vector4::operator == (const Vector4 &v) { return x==v.x && y==v.y && z==v.z && w==v.w; }
int			Vector4::operator != (const Vector4 &v) { return !(*this==v); }
float&		Vector4::operator [] (const int i) {	return ((float*)this)[i]; }
float&		Vector4::operator [] (const int i) const { return ((float*)this)[i]; }

void		Vector4::Set(float x, float y, float z, float w) {	Vector4::x = x; Vector4::y = y; Vector4::z = z; Vector4::w = w; }
float		Vector4::Dot(const Vector4 &src) { return (Vector4::x * src.x + Vector4::y * src.y + Vector4::z * src.z); }
Vector4		Vector4::Cross(const Vector4 &src) { return Vector4(Vector4::y * src.z - Vector4::z * src.y, Vector4::z * src.x - Vector4::x * src.z, Vector4::x * src.y - Vector4::y * src.x, 1.0f); }
float		Vector4::Len(void)
{ 
	float len = sqrtf(Vector4::x * Vector4::x + Vector4::y * Vector4::y + Vector4::z * Vector4::z);
	return len; 
}

float	Vector4::LenNoSqr(void)
{
	float len = Vector4::x * Vector4::x + Vector4::y * Vector4::y + Vector4::z * Vector4::z;
	return len; 
}


void		Vector4::Lerp(const Vector4 &p1, const Vector4 &p2, float t)
{
	const Vector4		s = p2 - p1;
	(*this) = s * t + p1;
}

void		Vector4::Normalize() 
{ 
	float l = Vector4::Len();
	Vector4::x /= l; 
	Vector4::y /= l; 
	Vector4::z /= l; 
}

void		Vector4::NormalizeH() 
{ 
	float l = Vector4::Len();
	Vector4::x /= l; 
	Vector4::y /= l; 
	Vector4::z /= l; 
	Vector4::w = l;
}

// Matrix44 s
Matrix44::Matrix44()											{ Identity(); }
Matrix44::Matrix44(Vector4 r, Vector4 u, Vector4 v, Vector4 p)	{ right = r; up = u; view = v; pos = p; }
Matrix44::Matrix44(const Matrix44 &m)							{	*this = m; }

Matrix44	Matrix44::operator + () const { return *this; }
Matrix44	Matrix44::operator - () const { return Matrix44(-right, -up, -view, -pos); }
Matrix44	Matrix44::operator + (const Matrix44 &m) const { return Matrix44(right+m.right, up+m.up ,view+m.view ,pos+m.pos); }
Matrix44	Matrix44::operator - (const Matrix44 &m) const { return Matrix44(right-m.right, up-m.up ,view-m.view ,pos-m.pos); }
Matrix44	Matrix44::operator / (const float f) const { float	oof = 1.0f / f;	return Matrix44(right*oof, up*oof, view*oof, pos*oof); }
Matrix44	Matrix44::operator * (const float f) const { return Matrix44(right*f, up*f, view*f, pos*f); }

void		Matrix44::operator  = (const Matrix44 &m) { right = m.right; up = m.up; view = m.view; pos = m.pos; }
void		Matrix44::operator += (const Matrix44 &m) { *this = *this + m; }
void		Matrix44::operator -= (const Matrix44 &m) { *this = *this - m; }
void		Matrix44::operator *= (const float f) { *this = *this * f; }
void		Matrix44::operator *= (const Matrix44 &m) { *this = *this * m; }
void		Matrix44::operator /= (const float f) { *this = *this / f; }
int			Matrix44::operator == (const Matrix44 &m) { return right==m.right && up==m.up && view==m.view && pos==m.pos; }
int			Matrix44::operator != (const Matrix44 &m) { return !(*this==m); }
float&	Matrix44::operator [] (const int i) {	return ((float*)this)[i]; }
float&	Matrix44::operator [] (const int i) const { return ((float*)this)[i]; }


void		Matrix44::Set(Vector4 x, Vector4 y, Vector4 z, Vector4 w) {	Matrix44::right = x; Matrix44::up = y; Matrix44::view = z; Matrix44::pos = w; }
void		Matrix44::Lerp(const Matrix44 &p1, const Matrix44 &p2, float t)
{
	const Matrix44		s = p2 - p1;
	right = s.right * t;
	up = s.up * t;
	view = s.view * t;
	pos = s.pos * t;
}
void		Matrix44::Identity() 
{
	right.Set(	1.0f, 0.0f, 0.0f, 0.0f);
	up.Set(		0.0f, 1.0f, 0.0f, 0.0f);
	view.Set(	0.0f, 0.0f, 1.0f, 0.0f);
	pos.Set(	0.0f, 0.0f, 0.0f, 1.0f);
}
void		Matrix44::Transform(Vector3 src, Vector3 &dst) 
{
	dst.x = Matrix44::right.x * src.x + Matrix44::up.x * src.y + Matrix44::view.x * src.z;
	dst.y = Matrix44::right.y * src.x + Matrix44::up.y * src.y + Matrix44::view.y * src.z;
	dst.z = Matrix44::right.z * src.x + Matrix44::up.z * src.y + Matrix44::view.z * src.z;
}

void		Matrix44::Transform(Vector4 src, Vector4 &dst) 
{
	dst = *this * src;
	dst /= dst.w;
	dst.w = 1.0f;
}

void		Matrix44::Scale(float x, float y, float z) { right.x *= x; up.y *= y; view.z *= z; }

// Yaw, Pitch Roll rotation
void		Matrix44::RotationHPR(float x, float y, float z)
{
	Vector4 tright = Vector4(cosf(x),0,sinf(x),0);
	Vector4 tup = Vector4(0,1,0,0);
	Vector4 tview = Vector4(-sinf(x),0,cosf(x),0);
	Vector4 tpos = Vector4(0,0,0,1);
	Matrix44 tm1 = Matrix44(tright,tup,tview,tpos);
	
	// Pitch / Rotate about Y Axis (Up/Down)
	tright = Vector4(1,0,0,0);
	tup = Vector4(0,cosf(y),-sinf(y),0);
	tview = Vector4(0,sinf(y),cosf(y),0);
	Matrix44 tm2 = Matrix44(tright,tup,tview,tpos);

	// Roll / Rotate about Z Axis 
	tright = Vector4(cosf(z),-sinf(z),0,0);
	tup = Vector4(sinf(z),cosf(z),0,0);
	tview = Vector4(0,0,1,0);
	Matrix44 tm3 = Matrix44(tright,tup,tview,tpos);
	
	// Combine the matrices
	Matrix44 final = tm1 * tm2 * tm3;
	*this = final;
}

void	Matrix44::RotateHPR(float x, float y, float z)
{
	Vector4 tright;
	Vector4 tup;
	Vector4 tview;
	Vector4 tpos = Vector4(0,0,0,1);
	Matrix44 tm;
	if(x)
	{
		tright = Vector4(cosf(x),0,sinf(x),0);
		tup = Vector4(0,1,0,0);
		tview = Vector4(-sinf(x),0,cosf(x),0);
		tpos = Vector4(0,0,0,1);
		tm = Matrix44(tright,tup,tview,tpos);
		*this = *this * tm;
	}
	if(y)
	{
		tright = Vector4(1,0,0,0);
		tup = Vector4(0,cosf(y),-sinf(y),0);
		tview = Vector4(0,sinf(y),cosf(y),0);
		tm = Matrix44(tright,tup,tview,tpos);
		*this = *this * tm;
	}
	if(z)
	{
		tright = Vector4(cosf(z),-sinf(z),0,0);
		tup = Vector4(sinf(z),cosf(z),0,0);
		tview = Vector4(0,0,1,0);
		tm = Matrix44(tright,tup,tview,tpos);
		*this = *this * tm;
	}
}

void	Matrix44::RotationXYZ(float x, float y, float z)
{
	Vector4 tright = Vector4(cosf(y),0,sinf(y),0);
	Vector4 tup = Vector4(0,1,0,0);
	Vector4 tview = Vector4(-sinf(y),0,cosf(y),0);
	Vector4 tpos = Vector4(0,0,0,1);
	Matrix44 tm1 = Matrix44(tright,tup,tview,tpos);
	
	// Pitch / Rotate about Y Axis (Up/Down)
	tright = Vector4(1,0,0,0);
	tup = Vector4(0,cosf(x),-sinf(x),0);
	tview = Vector4(0,sinf(x),cosf(x),0);
	Matrix44 tm2 = Matrix44(tright,tup,tview,tpos);

	// Roll / Rotate about Z Axis 
	tright = Vector4(cosf(z),-sinf(z),0,0);
	tup = Vector4(sinf(z),cosf(z),0,0);
	tview = Vector4(0,0,1,0);
	Matrix44 tm3 = Matrix44(tright,tup,tview,tpos);
	
	// Combine the matrices
	Matrix44 final = tm2 * tm1 * tm3;
	*this = final;
}

void	Matrix44::RotateXYZ(float x, float y, float z)
{
	Vector4 tright;
	Vector4 tup;
	Vector4 tview;
	Vector4 tpos = Vector4(0,0,0,1);
	Matrix44 tm;
	if(x)
	{
		tright = Vector4(1,0,0,0);
		tup = Vector4(0,cosf(x),-sinf(x),0);
		tview = Vector4(0,sinf(x),cosf(x),0);
		tm = Matrix44(tright,tup,tview,tpos);
		*this = *this * tm;
	}
	if(y)
	{
		tright = Vector4(cosf(y),0,sinf(y),0);
		tup = Vector4(0,1,0,0);
		tview = Vector4(-sinf(y),0,cosf(y),0);
		tm = Matrix44(tright,tup,tview,tpos);
		*this = *this * tm;
	}
	if(z)
	{
		tright = Vector4(cosf(z),-sinf(z),0,0);
		tup = Vector4(sinf(z),cosf(z),0,0);
		tview = Vector4(0,0,1,0);
		tm = Matrix44(tright,tup,tview,tpos);
		*this = *this * tm;
	}
}

void Matrix44::Inverse()
{
	float tmp[12]; /* temp array for pairs */
	float src[16]; /* array of transpose source matrix */
	float det; /* determinant */
	Matrix44 dst;
	/* transpose matrix */
	for ( int i = 0; i < 4; i++) 
	{
	src[i] = (*this)[i*4];
	src[i + 4] = (*this)[i*4 + 1];
	src[i + 8] = (*this)[i*4 + 2];
	src[i + 12] = (*this)[i*4 + 3];
	}
	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	/* calculate first 8 elements (cofactors) */
	dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2]*src[7];
	tmp[1] = src[3]*src[6];
	tmp[2] = src[1]*src[7];
	tmp[3] = src[3]*src[5];
	tmp[4] = src[1]*src[6];
	tmp[5] = src[2]*src[5];

	tmp[6] = src[0]*src[7];
	tmp[7] = src[3]*src[4];
	tmp[8] = src[0]*src[6];
	tmp[9] = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];
	/* calculate second 8 elements (cofactors) */
	dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
	/* calculate determinant */
	det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
	if(det == 0.0) det = .0000001f;
	/* calculate matrix inverse */
	det = 1/det;
	for ( int j = 0; j < 16; j++)
	(*this)[j] = dst[j] * det;
}

void		Matrix44::Translate(float x, float y, float z)
{
	Matrix44::pos.x += x;
	Matrix44::pos.y += y;
	Matrix44::pos.z += z;
	Matrix44::pos.w = 1.0f;
}

void		Matrix44::Position(float x, float y, float z)
{
	Matrix44::pos.x = x;
	Matrix44::pos.y = y;
	Matrix44::pos.z = z;
	Matrix44::pos.w = 1.0f;
}

void		Matrix44::LookAt(float x, float y, float z)
{
	Vector4 At = Vector4(x,y,z,0);
	Vector4 Eye = pos;
	Vector4 Up(0,1,0,0);
	Vector4 zaxis = (At - Eye);
	zaxis.Normalize();
	Vector4 xaxis = Up.Cross(zaxis);
	xaxis.Normalize();
	Vector4 yaxis = zaxis.Cross(xaxis);

	right = xaxis;
	up = yaxis;
	view = zaxis;
}
