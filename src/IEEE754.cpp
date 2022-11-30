#include "IEEE754.h"
#include <algorithm>
#include <iostream>


IEEE754::IEEE754() {
	std::copy(zero, zero + BIT+1, number);

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

	if (integer_part == 0 or !isInf()) {
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

int IEEE754::toDec(int bin_num[EXP]) {
	int res = 0;
	int mult = 1;
	for (int i = EXP-1; i >= 0; i--) {
		res += bin_num[i] * mult;
		mult *= 2;
	}
	return res;
}

int* IEEE754::doNewMan(int k, IEEE754 obj) {
	static int new_man[MANTISSA + 2];
	if (k == 0) {
		new_man[0] = obj.sign;
		new_man[1] = obj.man_sign;
		std::copy(obj.mantissa, obj.mantissa + (MANTISSA - 1), new_man+2);

	}
	else {
		new_man[0] = obj.sign;
		std::fill(new_man+1, new_man + k, 0);
		new_man[k + 1] = obj.man_sign;
		std::copy(obj.mantissa, obj.mantissa + (MANTISSA - k - 1), new_man+k+2);
	}

	return new_man;
}

void IEEE754::doOldMan(int bias, IEEE754& obj, int* man) {
	int counter = 1;
	for (int i = 0; i < MANTISSA + 2; i++) {
		std::cout << man[i];
	}
	std::cout << "\\" << std::endl;
	while ((man[counter] != 1)&&((bias + counter) != BIAS)) {
		++counter;
	}
	std::cout << counter << "\n";
	obj.exp_bias[0] = BIAS + bias - (counter-1);
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
	IEEE754 res;
	int thisBias, objBias;
	if (number[10] == 0)
		thisBias = 0;
	else {
		thisBias = toDec(exp_bias);
		thisBias -= BIAS;
	}
	if (obj.number[10] == 0)
		objBias = 0;
	else {
		objBias = toDec(obj.exp_bias);
		objBias -= BIAS;
	}

	int firstMan[MANTISSA + 2];
	int secondMan[MANTISSA + 2];
	int* ptr;
	int resultBias = std::max(thisBias, objBias);

	ptr = doNewMan(resultBias - thisBias, *this);
	std::copy(ptr, ptr + MANTISSA + 2, firstMan);
	ptr = doNewMan(resultBias - objBias, obj);
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
	res.sign = 1;
	res.doNum();
	return res;
}

IEEE754 IEEE754::operator - (IEEE754 obj) {
	IEEE754 res = *this;
	return res + (-obj);
}

IEEE754 IEEE754::operator *(IEEE754 obj) {
	IEEE754 res = *this;
	return res + (-obj);
}
