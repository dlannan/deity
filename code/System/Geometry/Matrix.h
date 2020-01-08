// Matrix Structure

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <math.h>

void PIII_Mult44( float *src1, float *src2, float *dst);
void PIII_Mult44Vec4( float *src1, float *src2, float *dst);

struct Vector2
{
	union {float x; float u;};
	union {float y; float v;};

	// Constructors
	Vector2();
	Vector2(float, float);
	Vector2(const Vector2 &);
	~Vector2() {}


	// Operators
	Vector2	operator +  () const;
	Vector2	operator -  () const;
	Vector2	operator +  (const Vector2 &) const;
	Vector2	operator -  (const Vector2 &) const;
	Vector2	operator /  (const float) const;
	Vector2	operator *  (const float) const;
	Vector2	operator *  (const Vector2 &) const;
	void		operator  = (const Vector2 &);
	void		operator += (const Vector2 &);
	void		operator -= (const Vector2 &);
	void		operator *= (const float);
	void		operator *= (const Vector2 &);
	void		operator /= (const float);
	int			operator == (const Vector2 &);
	int			operator != (const Vector2 &);
	float&		operator []	(const int);
	float&		operator []	(const int) const;
				operator float* () {return (float*)this;}

	// Methods
	void		Set(float, float);
	void		Lerp(const Vector2 &, const Vector2 &, float t);
	float		Len(void);	
	float		LenSq(void);
	void		Normalize();
	float		Dot(const Vector2 &);

};

typedef struct Vector2		Vector2;
//typedef struct Vector2		Point2;
typedef struct Vector2		Vertex2;
typedef struct Vector2		Texel;

struct Vector4;

struct Vector3
{
	union {float x; float r;};
	union {float y; float g;};
	union {float z; float b;};

	Vector3();
	Vector3(float, float, float);
	Vector3(const Vector3 &);
	~Vector3() {}


	// Operators
	Vector3	operator +  () const;
	Vector3	operator -  () const;
	Vector3	operator +  (const Vector3 &) const;
	Vector3	operator -  (const Vector3 &) const;
	Vector3	operator /  (const float) const;
	Vector3	operator *  (const float) const;
	Vector3	operator *  (const Vector3 &) const;
	void		operator  = (const Vector3 &);
	void		operator  = (const Vector4 &);
	void		operator += (const Vector3 &);
	void		operator -= (const Vector3 &);
	void		operator *= (const float);
	void		operator *= (const Vector3 &);
	void		operator /= (const float);
	int			operator == (const Vector3 &);
	int			operator != (const Vector3 &);
	float&		operator []	(const int);
	float&		operator []	(const int) const;
				operator float* () {return (float*)this;}

	// Methods
	void		Set(float, float, float);
	void		Lerp(const Vector3 &, const Vector3 &, float t);
	float		Len(void) const;	
	float		Len2(void) const;	// Length squared

	float		Distance(const Vector3 &) const;	// Distance between two points
	float		Distance2(const Vector3 &) const; // Distance squared

	float		Normalize();						// Return size or length
	float		Dot(const Vector3 &);
	Vector3		Cross(const Vector3 &) const;
};

typedef struct Vector3		Vector3;
//typedef struct Vector3		Point3;
typedef struct Vector3		Vertex3;
typedef struct Vector3		Colour3;

struct Vector4
{
	union {float x; float r;};
	union {float y; float g;};
	union {float z; float b;};
	union {float w; float a;};

	Vector4();
	Vector4(float, float, float, float);
	Vector4(const Vector4 &);
	~Vector4() {}


	// Operators
	Vector4	operator +  () const;
	Vector4	operator -  () const;
	Vector4	operator +  (const Vector4 &) const;
	Vector4	operator -  (const Vector4 &) const;
	Vector4	operator /  (const float) const;
	Vector4	operator *  (const float) const;
	Vector4	operator *  (const Vector4 &) const;
	void		operator  = (const Vector4 &);
	void		operator  = (const Vector3 &);
	void		operator += (const Vector4 &);
	void		operator -= (const Vector4 &);
	void		operator *= (const float);
	void		operator *= (const Vector4 &);
	void		operator /= (const float);
	int			operator == (const Vector4 &);
	int			operator != (const Vector4 &);
	float&		operator []	(const int);
	float&		operator []	(const int) const;
				operator float* () {return (float*)this;}

	// Methods
	void		Set(float, float, float, float);
	void		Lerp(const Vector4 &, const Vector4 &, float t);
	float		Len(void);	
	float		LenNoSqr(void);	
	void		Normalize();
	void		NormalizeH();
	float		Dot(const Vector4 &);
	Vector4		Cross(const Vector4 &);

	// Helper functions for Colour4
	Vector4(unsigned int rgba)
	{
		r = (float) ( (0xFF000000 & rgba) >> 24 );
		g = (float) ( (0x00FF0000 & rgba) >> 16 );
		b = (float) ( (0x0000FF00 & rgba) >> 8 );
		a = (float) ( (0x000000FF & rgba) );
	}
};

typedef struct Vector4		Vector4;
typedef struct Vector4		Vertex4;
typedef struct Vector4		Colour4;



// Specialised use of a Vector - A sphere is a Vector3(x,y,z) and a Radius(w)
typedef struct Vector4		Sphere;

struct Matrix44
{
	Vector4	right;		// xaxis
	Vector4	up;			// yaxis
	Vector4	view;		// zaxis
	Vector4	pos;		// position info

	Matrix44();
	Matrix44(Vector4 , Vector4 , Vector4 , Vector4 );
	Matrix44(const Matrix44 &);

	~Matrix44() {}

	// Operators
	void		Identity();
	Matrix44	operator +  () const;
	Matrix44	operator -  () const;
	Matrix44	operator +  (const Matrix44 &) const;
	Matrix44	operator -  (const Matrix44 &) const;
	Matrix44	operator /  (const float) const;
	Matrix44	operator *  (const float) const;
	Matrix44	operator *  (const Matrix44 &) const;
	Vector4		operator *  (const Vector4 &v) const;
	void		operator  = (const Matrix44 &);
	void		operator += (const Matrix44 &);
	void		operator -= (const Matrix44 &);
	void		operator *= (const float);
	void		operator *= (const Matrix44 &);
	void		operator /= (const float);
	int			operator == (const Matrix44 &);
	int			operator != (const Matrix44 &);
	float&		operator []	(const int);
	float&		operator []	(const int) const;
				operator float* () {return (float*)this;}

	void		Inverse();
	void		Set(Vector4, Vector4, Vector4, Vector4);
	void		Lerp(const Matrix44 &, const Matrix44 &, float t);
	float		Len(void);	
	void		Transform(Vector4 , Vector4 &);
	void		Transform(Vector3 , Vector3 &);

	void		LookAt(float x, float y, float z);
	void		Scale(float, float, float);
	void		RotationAA(float x, float y, float z, float t);
	void		RotationHPR(float, float, float);
	void		RotateHPR(float, float, float);
	void		RotationXYZ(float, float, float);
	void		RotateXYZ(float, float, float);
	void		Translate(float, float, float);
	void		Position(float, float, float);

};

// Will not bother supporting any other types yet. If the need arises.. maybe
typedef struct Matrix44	Matrix44;		
typedef struct Matrix44	Matrix;		

struct Matrix43
{
	struct 
	{
		Vector4	right;		// xaxis
		Vector4	up;			// yaxis
		Vector4	view;		// zaxis
	};

	Matrix43() {}
	~Matrix43() {}
};

typedef struct Matrix43	Matrix43;

struct Matrix33
{
	struct 
	{
		Vector3	right;		// xaxis
		Vector3	up;			// yaxis
		Vector3	view;		// zaxis
	};

	Matrix33() {}
	~Matrix33() {}
};

typedef struct Matrix33	Matrix33;

#endif