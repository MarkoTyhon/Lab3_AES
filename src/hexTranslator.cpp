#include "hexTranslator.h"

#include <ios>
#include <sstream>
#include <bitset>

#include "properties.h"


void HexTranslator::numToHex(IEEE754 binn) {
	bin_num = std::to_string(binn.sign);
	for (int i : binn.exp_bias)
		bin_num += std::to_string(i);
	for (int i : binn.mantissa)
		bin_num += std::to_string(i);

	toHex();
	toNHex();

}

void HexTranslator::toNHex() {
	int hex_len;
	hex_len = str_hex_num.length();

	str_hex_num = std::string(HEX_LEN - hex_len, '0') + str_hex_num;
}

void HexTranslator::toHex()
{
	std::bitset<BIT> set(bin_num);
	std::stringstream res;
	res << std::hex << std::uppercase << set.to_ulong();
	str_hex_num = res.str();
}
