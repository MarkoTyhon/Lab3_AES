#pragma once
#include "properties.h"
#include <algorithm>
#include <vector>

class IEEE754
{
public:
	IEEE754();
	IEEE754(double x);
	bool operator > (const IEEE754 obj);
	bool operator == (const IEEE754 obj);
	bool operator < (const IEEE754 obj);
	bool operator <=(const IEEE754 obj);
	bool operator >=(const IEEE754 obj);
	IEEE754 operator + (IEEE754 y);
	IEEE754 operator - () const;
	IEEE754 operator - (IEEE754 y);
	//IEEE754 operator * (IEEE754 y);
	//IEEE754 operator / (IEEE754 y);
	friend std::ostream& operator<< (std::ostream& os, const IEEE754& obj);

	IEEE754 zeroi();
	int zero[BIT+1] = { 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int inf0[BIT+1] = { 0,   1, 1, 1, 1, 1, 1, 1, 1, 1,   1,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int inf1[BIT+1] = { 1,   1, 1, 1, 1, 1, 1, 1, 1, 1,   1,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int NaN[BIT+1]	= { 0,   1, 1, 1, 1, 1, 1, 1, 1, 1,   1,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};


	int number[BIT+1];
	int man_sign;
	int mantissa[MANTISSA];
	int sign;
	int exp_bias[EXP];
	int last_in_mant = 0;
	void convertIntPart(long int dec_num);
	void convertFloatPart(double dec_num);
	void doExpBias();
	bool isInf();
	void doNum();


	std::vector<int> toBin(long int dec_num);
	int toDec(int bin_num[EXP]);
	int* doNewMan(int k, IEEE754& obj);
	void doOldMan(int bias, IEEE754&, int* man);
	int* add(int* objMan, int* thisMan);




};

