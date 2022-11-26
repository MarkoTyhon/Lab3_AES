#include "IEEE754.h"
#include <algorithm>
#include <iostream>


IEEE754::IEEE754() {
	std::copy(zero, zero + BIT, number);

}

IEEE754::IEEE754(double dec_num) {
	std::fill(exp_bias, exp_bias + EXP, 0);
	last_in_mant = 0;
	if (dec_num < 0) {
		sign = 1;
		dec_num = std::abs(dec_num);
	}
	else
		sign = 0;

	man_sign = 1;
	long int integer_part = floor(dec_num);
	double floating_part = dec_num - integer_part;
	convertIntPart(integer_part);
	if (!isInf()) {
		convertFloatPart(floating_part);
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
	if (exp_bias[0] >= MAX_BIAS) {
		std::copy(inf0+1, inf0 + BIT, exp_bias+1);
		exp_bias[0] = sign;
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
	exp_bias[0] = 0;
	int exp_len = exp_biasV.size();
	for (int i = 1; i <= exp_len; i++) {
		exp_bias[EXP-i] = exp_biasV[exp_len-i];
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
	for (int i = last_in_mant; i < MANTISSA; i++) {
		dec_num = dec_num * 2;
		mantissa[i] = floor(dec_num);
		dec_num -= floor(dec_num);
	
	}
}



std::ostream& operator << (std::ostream& os ,const IEEE754&  num){
	for (int i = 0; i < BIT+1; i++) {
		os << num.number[i];
		if ((i == 0) || (i == 9) || (i == 10)) {
			os <<  "  ";
		}
	}
	return os;
}