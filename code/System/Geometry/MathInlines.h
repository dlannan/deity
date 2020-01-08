
// Handy Math inlines that can be used to do things faster - usually

#ifndef _MATHINLINES_H_
#define _MATHINLINES_H_

inline int Float2Int(float infloat)
{
	int FltInt = *(int *)&infloat;
	int MyInt;

	int mantissa = (FltInt & 0x07fffff) | 0x800000;
	int exponent = 150 - ((FltInt >> 23) & 0xff);

	if (exponent < 0)
		MyInt = (mantissa << -exponent);		      
	else
		MyInt = (mantissa >> exponent);

	if (FltInt & 0x80000000)
    MyInt = -MyInt;
	return MyInt;
}

#endif
