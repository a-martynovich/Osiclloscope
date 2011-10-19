#ifndef QGTCOORDINATES_H
#define QGTCOORDINATES_H

#include <qmath.h>

template<class T>
struct QgtCoordinates
{
	QgtCoordinates(T _left, T _right, T _bottom, T _top) 
		: left(_left), right(_right),
		  bottom(_bottom), top(_top)
	{

	};

	T left;
	T right;
	T leftRightInterval() const { return right - left; };

	T bottom;
	T top;
	T bottomTopInterval() const { return top - bottom; };
};

#endif // QGTCOORDINATES_H