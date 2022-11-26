#pragma once
#include "properties.h"
#include <algorithm>
#include <vector>

class IEEE754
{
public:
	IEEE754();
	IEEE754(double x);
	//IEEE754 operator + (IEEE754 y);
	//IEEE754 operator - (IEEE754 y);
	//IEEE754 operator * (IEEE754 y);
	//IEEE754 operator / (IEEE754 y);
	friend std::ostream& operator<< (std::ostream& os, const IEEE754& num);

	int zero[BIT+1] = { 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int inf0[BIT+1] = { 0,   1, 1, 1, 1, 1, 1, 1, 1, 1,   1,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int inf1[BIT+1] = { 1,   1, 1, 1, 1, 1, 1, 1, 1, 1,   1,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


	int number[BIT+1];
	int man_sign;
	int mantissa[MANTISSA];
	int sign;
	int exp_bias[EXP];
	int last_in_mant;
	void convertIntPart(long int dec_num);
	void convertFloatPart(double dec_num);
	void doExpBias();
	bool isInf();
	void doNum();


	std::vector<int> toBin(long int dec_num);


};

