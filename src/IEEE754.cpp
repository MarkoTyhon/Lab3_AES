#include "IEEE754.h"
#include <algorithm>
#include <iostream>


IEEE754::IEEE754() {
	number[0] = 1;

}
IEEE754::IEEE754(double dec_num) {
	//init some values
	std::fill(exp_bias, exp_bias + EXP, 0);
	last_in_mant = 0;
	man_sign = 1;

	//sign of number{
	if (dec_num < 0)
		sign = 1;
	else
		sign = 0;
	dec_num = std::abs(dec_num);
	//}sign of number
	
	long int integer_part = floor(dec_num);
	double floating_part = dec_num - integer_part;

	if (integer_part != 0)
		convertIntPart(integer_part); 

	if ((integer_part == 0) || (!isInf())) {
		convertFloatPart(floating_part);
		if (exp_bias[0] == 0) 
			man_sign = 0;
		else
			doExpBias();
		doNum();
	}
	
}



void IEEE754::doNum() {
	number[0] = sign;
	std::copy(exp_bias, exp_bias + EXP, number + 1);
	number[10] = man_sign;
	std::copy(mantissa, mantissa + MANTISSA, number + 11);
}

bool IEEE754::isInf() {
	bool res = exp_bias[0] >= MAX_BIAS;
	if (res) {
		*this = getInf(sign);
	}
	return res;
}

std::vector<int> IEEE754::toBin(long int dec_num) {
	std::vector<int> character;
	while (dec_num) {
		character.push_back(dec_num % 2);
		dec_num /= 2;
	}
	std::reverse(character.begin(), character.end());
	return character;
}



void IEEE754::doExpBias() {
	std::vector<int>exp_biasV = toBin(exp_bias[0]);
	std::fill(exp_bias, exp_bias + EXP, 0);
	int exp_len = exp_biasV.size();
	for (int i = 1; i <= exp_len; i++) {
		exp_bias[EXP-i] = exp_biasV[exp_len -i];
	}

}

void IEEE754::convertIntPart(long int dec_num) {
	std::vector<int> character = toBin(dec_num);
	exp_bias[0] = character.size()-1 + BIAS;
	for (int i = 1; i < character.size(); i++) {
		mantissa[i-1] = character[i];
		last_in_mant = i;
	}
}

void IEEE754::convertFloatPart(double dec_num) {
	int couner = 0;
	int dec_floor = 0;
	if (exp_bias[0] == 0) {
		while ((couner <= BIAS-1) && (dec_floor == 0)) {
			++couner;
			dec_num = dec_num * 2;
			dec_floor = floor(dec_num);
			dec_num -= dec_floor;
		}
		exp_bias[0] = BIAS - couner;
	}
	

	for (int i = last_in_mant; i < MANTISSA; i++) {		
		dec_num = dec_num * 2;
		dec_floor = floor(dec_num);
		mantissa[i] = dec_floor;
		dec_num -= dec_floor;
	}
	
}

bool IEEE754::operator>(const IEEE754 obj) {
	if (number[0] < obj.number[0])
		return true;
	for (int i = 1; i < MANTISSA + 1; i++){
		if ((i != 10) && (number[i] != obj.number[i])) 
			return (number[i] * std::pow(-1, number[0]) > obj.number[i] * std::pow(-1, obj.number[0]));
		}
	return false;
}
bool IEEE754::operator==(const IEEE754 obj) {
	for (int i = 0; i < MANTISSA + 1; i++) {
		if (number[i] != obj.number[i])
			return false;
	}
	return true;
}

bool IEEE754::operator!=(const IEEE754 obj) {
	return !(*this == obj);
}

bool IEEE754::operator<(const IEEE754 obj) {
	if (number[0] > obj.number[0])
		return true;
	for (int i = 1; i < MANTISSA + 1; i++) {
		if ((i != 10) && (number[i] != obj.number[i]))
			return (number[i]*std::pow(-1, number[0]) < obj.number[i] * std::pow(-1, obj.number[0]));
	}
	return false;
}

bool IEEE754::operator<=(const IEEE754 obj) {
	return ((*this < obj) || (*this == obj));
}

bool IEEE754::operator>=(const IEEE754 obj) {
	return ((*this > obj) || (*this == obj));
}

std::ostream& operator << (std::ostream& os ,const IEEE754&  obj){
	for (int i = 0; i < BIT+1; i++) {
		os << obj.number[i];
		if ((i == 0) || (i == 9) || (i == 10)) {
			os <<  "  ";
		}
	}
	return os;
}

unsigned long long IEEE754::toDec(int* bin_num) {
	unsigned long long res = 0;
	unsigned long long mult = 1;
	for (int i = EXP-1; i >= 0; i--) {
		res += bin_num[i] * mult;
		mult *= 2;
	}
	return res;
}

int IEEE754::getBias(IEEE754 obj) {
	if (obj.number[10] == 0)
		return 0;
	else {
		int res = toDec(obj.exp_bias);
		return res;
	}
}

int* IEEE754::doNewMan(int newBias,int oldBias, IEEE754 obj) {
	int k = newBias- oldBias;
	static int new_man[MANTISSA + 2];
	if (k == 0) {
		new_man[0] = obj.sign;
		new_man[1] = obj.man_sign;
		std::copy(obj.mantissa, obj.mantissa + (MANTISSA), new_man+2);
	}
	else if (k > oldBias) {
		std::fill(new_man + 1, new_man + MANTISSA + 2, 0);
		new_man[0] = obj.sign;

	}
	else {
		std::fill(new_man+1, new_man + MANTISSA + 2, 0);
		
		std::copy(obj.mantissa, obj.mantissa + (MANTISSA - k +1), new_man+k+1);
		
		new_man[0] = obj.sign;
		new_man[k+1] = obj.man_sign;
		
	}

	return new_man;
}

void IEEE754::doOldMan(int bias, IEEE754& obj, int* man) {
	int counter = 1;
	
	while ((man[counter] != 1)&&((bias + (counter-1)) < 0)&&(counter < MANTISSA)) {
		++counter;
	}
	obj.exp_bias[0] = bias - (counter-1);
	if (bias - (counter - 1) != 0) 
		obj.man_sign = 1;
	else
		obj.man_sign = 0;
	std::fill(obj.mantissa, obj.mantissa + MANTISSA, 0);
	std::copy(man + counter+1, man + MANTISSA + 2, obj.mantissa);
}

int* IEEE754::add(int* firstMan, int* secondMan, IEEE754& res) {
	static int result[MANTISSA + 2]; 
	int buf = 0;
	result[0] = firstMan[0];
	
	for (int i = MANTISSA + 1; i > 0; i--) {
		
 		buf = firstMan[i] + secondMan[i] + buf;
		if ((buf == 0) || (buf == 1)) {
			result[i] = buf;
			buf = 0;
		}
		else if (buf == 3) {
			result[i] = 1;
			buf = 1;
		}
		else if (buf == 2) {
			result[i] = 0;
			buf = 1;
		}
	}
	if (buf == 1) {
		for (int i = MANTISSA + 1; i > 1; i--) {
			result[i] = result[i-1];
		}
		result[1] = 2;
	}
	return result;
}

IEEE754 IEEE754::abss(IEEE754 obj) {
	obj.sign = 0;
	obj.doNum();
	return obj;
}


int* IEEE754::sub(int* firstMan, int* secondMan, IEEE754& res) {
	static int result[MANTISSA + 2];
	int first_sig = firstMan[0];
	int second_sig = secondMan[0];
	int buf = 0;


	for (int i = MANTISSA + 1; i > 0; i--) {

		buf = firstMan[i] - secondMan[i] + buf;
		if ((buf == 0) || (buf == 1)) {
			result[i] = buf;
			buf = 0;
		}
		else if (buf == -1) {
			result[i] = 1;
			int j = i;
			while (j > 1) {
				if(firstMan[j] == 1){
					firstMan[j] = 0;
					break;
				}
				firstMan[j] = 1;
				j--;
			} 
			buf = 0;
		}
	}
	return result;
}


IEEE754 IEEE754::operator + (IEEE754 obj) {
	IEEE754 res = *this;
	if (abss(obj) == getInf(0)) {
		if (abss(res) == getInf(1))
			return getNaN();
	}
	if (abss(res) == getInf(1)) {
		if (abss(obj) == getInf(0))
			return getNaN();
	}
	
	int thisBias = 0, objBias= 0;



	thisBias = getBias(*this);
	objBias = getBias(obj);

	int firstMan[MANTISSA + 2];
	int secondMan[MANTISSA + 2];
	int* ptr;
	int resultBias = std::max(thisBias, objBias);

	ptr = doNewMan(resultBias, thisBias, *this);
	std::copy(ptr, ptr + MANTISSA + 2, firstMan);
	ptr = doNewMan(resultBias, objBias, obj);
	std::copy(ptr, ptr + MANTISSA + 2, secondMan);


	if (number[0] == obj.number[0])
		ptr = add(firstMan, secondMan, res);
	else {
		if (abss(*this) == abss(obj))
			return 0;
		else if(abss(*this) < abss(obj)){
			ptr = sub(secondMan, firstMan, res);
			ptr[0] = secondMan[0];}
		else {
			ptr = sub(firstMan, secondMan, res);
			ptr[0] = firstMan[0];
		}
	}
	

	if (ptr[1] == 2) {
		ptr[1] = 1;
		resultBias += 1;
	}
	
	res.sign = ptr[0];
	
	doOldMan(resultBias, res, ptr);
	res.doExpBias();
	res.doNum();
	return res;
}

IEEE754 IEEE754::operator-()const {
	IEEE754 res = *this;
	res.sign = !(res.sign);
	res.doNum();
	return res;
}

IEEE754 IEEE754::operator - (IEEE754 obj) {
	IEEE754 res = *this;
	return res + (-obj);
}

int* IEEE754::mult(int* firstMan, int* secondMan) {
	static int result[(MANTISSA * 2) + 3];
	std::fill(result, result + (MANTISSA*2) + 3, 0);
	result[0] = firstMan[0] != secondMan[0];
	int buf = 0;

	for (int i = 0; i < MANTISSA + 1; i++) {
		if (secondMan[MANTISSA+1-i] != 0) {
			
			for (int j = 0; j < MANTISSA + 1; j++) {
				buf = result[((MANTISSA * 2) + 2) - i - j] + firstMan[MANTISSA + 1-j] + buf;
				if ((buf == 0) || (buf == 1)) {
					result[((MANTISSA * 2) + 2) - i - j] = buf;
					buf = 0;
				}
				else if (buf == 3) {
					result[((MANTISSA * 2) + 2) - i - j] = 1;
					buf = 1;
				}
				else if (buf == 2) {
					result[((MANTISSA * 2) + 2) - i - j] = 0;
					buf = 1;
				}
			}
			
			if (buf == 1) {
				result[(MANTISSA * 2) - i - (MANTISSA-1)] = buf;
				buf = 0;

			}
		}
	}
	return result;
}

IEEE754 IEEE754::operator *(IEEE754 obj) {
	IEEE754 res = *this;
	if ((abss(res) == getInf(0)) || abss(res) == getZero()) {
		if ((abss(obj) != getInf(0)) || (abss(obj) != getZero()))//work
			return res;
	}
	if ((abss(obj) == getInf(0)) || abss(obj) == getZero()) {
		if ((abss(res) != getInf(0)) || (abss(res) != getZero()))
			return obj;
	}
	if (abss(obj) == getZero()) {
		if (abss(res) == getInf(0))
			return getNaN();
	}
	if (abss(res) == getZero()) {
		if (abss(obj) == getInf(0))
			return getNaN();
	}

	
	int thisBias;
	int* ptr;
	int firstMan[MANTISSA + 2];
	int secondMan[MANTISSA + 2];
	thisBias = getBias(res) + getBias(obj);
	thisBias -= BIAS;

	ptr = doNewMan(0, 0, *this);
	std::copy(ptr, ptr + MANTISSA + 2, firstMan);
	ptr = doNewMan(0, 0, obj);
	std::copy(ptr, ptr + MANTISSA + 2, secondMan);

	ptr = mult(firstMan, secondMan);
	res.sign = ptr[0];
	if (ptr[1] != 0) {
		thisBias += 1;
	}
	else
		std::copy(ptr + 1, ptr + MANTISSA + 3, ptr);

	doOldMan(thisBias, res, ptr);
	res.doExpBias();
	res.doNum();
	return res;
}



IEEE754 IEEE754::getZero() {
	IEEE754 zero;
	zero.sign = 0;
	zero.man_sign = 0;
	std::fill(zero.mantissa, zero.mantissa + MANTISSA, 0);
	std::fill(zero.exp_bias, zero.exp_bias + EXP, 0);
	zero.doNum();
	return zero;
}
IEEE754 IEEE754::getInf(int signv) {
	IEEE754 inf_val;
	inf_val.sign = signv;
	inf_val.man_sign = 1;
	std::fill(inf_val.mantissa, inf_val.mantissa + MANTISSA, 1);
	std::fill(inf_val.exp_bias, inf_val.exp_bias + EXP, 0);
	inf_val.doNum();
	return inf_val;
}

IEEE754 IEEE754::getNaN() {
	IEEE754 NaN;
	NaN.sign = 0;
	NaN.man_sign = 1;
	std::fill(NaN.mantissa, NaN.mantissa + MANTISSA-1, 0);
	std::fill(NaN.exp_bias, NaN.exp_bias + EXP, 1);
	NaN.mantissa[MANTISSA - 1] = 1;
	NaN.doNum();
	return NaN;
}

IEEE754 IEEE754::getMaxNormal() {
	IEEE754 mxnorm;
	mxnorm.sign = 0;
	mxnorm.man_sign = 1;
	std::fill(mxnorm.mantissa, mxnorm.mantissa + MANTISSA, 1);
	std::fill(mxnorm.exp_bias, mxnorm.exp_bias + EXP - 1, 1);
	mxnorm.exp_bias[EXP-1] = 0;
	mxnorm.doNum();
	return mxnorm;
}

IEEE754 IEEE754::getMinNormal() {
	IEEE754 mnnorm;
	mnnorm.sign = 0;
	mnnorm.man_sign = 1;
	std::fill(mnnorm.mantissa, mnnorm.mantissa + MANTISSA, 0);
	std::fill(mnnorm.exp_bias, mnnorm.exp_bias + EXP - 1, 0);
	mnnorm.exp_bias[EXP - 1] = 1;
	mnnorm.doNum();
	return mnnorm;
}

IEEE754 IEEE754::getMaxDenormal() {
	IEEE754 mxdnorm;
	mxdnorm.sign = 0;
	mxdnorm.man_sign = 0;
	std::fill(mxdnorm.mantissa, mxdnorm.mantissa + MANTISSA, 1);
	std::fill(mxdnorm.exp_bias, mxdnorm.exp_bias + EXP, 0);
	mxdnorm.doNum();
	return mxdnorm;
}

IEEE754 IEEE754::getMinDenormal() {
	IEEE754 mndnorm;
	mndnorm.sign = 0;
	mndnorm.man_sign = 0;
	std::fill(mndnorm.mantissa, mndnorm.mantissa + MANTISSA-1, 0);
	std::fill(mndnorm.exp_bias, mndnorm.exp_bias + EXP, 0);
	mndnorm.mantissa[MANTISSA - 1] = 1;
	mndnorm.doNum();
	return mndnorm;
}


IEEE754 IEEE754::get10E0() {
	IEEE754 one;
	one.sign = 0;
	one.man_sign = 1;
	std::fill(one.mantissa, one.mantissa + MANTISSA, 0);
	std::fill(one.exp_bias+1, one.exp_bias + EXP, 1);
	one.exp_bias[0] = 0;
	one.doNum();
	return one;
}

double IEEE754::getManD(IEEE754 obj) {
	double res = obj.man_sign;
	for (int i = 1; i < MANTISSA + 1; i++) {
		res += obj.mantissa[i-1]*pow(2, -i);
	}
	return res;
}

IEEE754 IEEE754::operator / (IEEE754 obj) {
	IEEE754 res = *this;
	if ((abss(res) == getInf(0)) || abss(res) == getZero()) {
		if ((abss(obj) == getInf(0)) || (abss(obj) == getZero()))
			return getNaN();
		else
			return res;
	}
	else if (abss(obj) == getInf(0)) {
		if ((abss(res) != getInf(0)) || (abss(res) != getZero()))
			return getZero();
	}
	else if (abss(obj) == getZero()) {
		if ((abss(res) != getInf(0)) || (abss(res) != getZero()))
			return getInf(res.sign);
	}

	if (res == obj)
		return get10E0();

	int firstBias = getBias(res);
	int secondBias = getBias(obj) ;
	double man1 = getManD(res);
	double man2 = getManD(obj);
	double first = pow(-1, res.sign) * man1 * pow(2, firstBias);
	double second = pow(-1, obj.sign) * man2 * pow(2, secondBias);

	return IEEE754(first/second);
}

