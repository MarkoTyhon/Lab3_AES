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
	bool operator != (const IEEE754 obj);
	bool operator < (const IEEE754 obj);
	bool operator <=(const IEEE754 obj);
	bool operator >=(const IEEE754 obj);
	IEEE754 operator + (IEEE754 y);
	IEEE754 operator - () const;
	IEEE754 operator - (IEEE754 y);
	IEEE754 operator * (IEEE754 y);
	IEEE754 operator / (IEEE754 y);
	friend std::ostream& operator<< (std::ostream& os, const IEEE754& obj);

	IEEE754 getZero();
	IEEE754 getInf(int sign);
	IEEE754 getNaN();
	IEEE754 getMaxNormal();
	IEEE754 getMinNormal();
	IEEE754 getMaxDenormal();
	IEEE754 getMinDenormal();




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
	int toDec(int* bin_num);
	int* doNewMan(int oldBias, int newBias, IEEE754 obj);
	void doOldMan(int bias, IEEE754&, int* man);
	int* add(int* objMan, int* thisMan, IEEE754& res);
	int* sub(int* objMan, int* thisMan, IEEE754& res);
	IEEE754 abss(IEEE754 obj);
	int getBias(IEEE754 exp);
	int* mult(int* objMan, int* thisMan);
	int* divis(int* objMan, int* thisMan);

};

