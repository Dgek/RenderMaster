#pragma once

struct Point
{
	int x;
	int y;

	Point();
	Point(int _x, int _y);
};

__forceinline Point::Point()
	: x{ 0 }, y{ 0 }
{}

__forceinline Point::Point(int _x, int _y)
	: x{ _x }, y{ _y }
{}