#include "IEEE754.h"
#include <algorithm>

IEEE754::IEEE754(double dec_num) {

	if (dec_num < 0) {
		sign = 1;
		dec_num = std::abs(dec_num);
	}
	else
		sign = 0;

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

}

bool IEEE754::isInf() {
	bool res = exp_bias[0] >= MAX_BIAS;
	if (exp_bias[0] >= MAX_BIAS) {
		std::copy(inf0, inf0 + BIT, exp_bias);
		exp_bias[0] = sign;
	}
	return res;
}

std::vector<int> IEEE754::toBin(long int dec_num) {
	std::vector<int> character;
	int i = 0;
	while (!dec_num) {
		character.push_back(dec_num % 2);
		dec_num /= 2;
	}
	std::reverse(character.begin(), character.end());
	return character;
}

void IEEE754::doExpBias() {
	std::vector<int>exp_biasV = toBin(exp_bias[0]);
	int exp_len = exp_biasV.size();
	for (int i = 0; i < exp_len; i++) {
		exp_bias[i] = exp_biasV[i];
	}

}

void IEEE754::convertIntPart(long int dec_num) {
	std::vector<int> character = toBin(dec_num);

	exp_bias[0] = character.size()-1 + BIAS;
	for (int i = 0; i < character.size(); i++) {
		mantissa[i] = character[i];
		last_in_mant = i;
	}
}

void IEEE754::convertFloatPart(double dec_num) {
	for (int i = last_in_mant + 1; i < MANTISSA; i++) {
		mantissa[i] = floor(dec_num * 2);
	}
}

IEEE754 IEEE754::operator + (IEEE754  other) {

	return other;
}

IEEE754 IEEE754::operator - (IEEE754  other) {

	return other;
}

IEEE754 IEEE754::operator * (IEEE754  other) {

	return other;
}

IEEE754 IEEE754::operator / (IEEE754  other) {

	return other;
}