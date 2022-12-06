#include "hexTranslator.h"

#include <ios>
#include <sstream>
#include <bitset>
#include <string>

#include "properties.h"


void HexTranslator::numToHex(IEEE754 binn) {
	str_hex_num = "";

    bin_num = std::to_string(binn.sign);
    for (int i : binn.exp_bias) {
        bin_num += std::to_string(i);
    }

	for (int i : binn.mantissa){
        bin_num += std::to_string(i);
	}
	for (int i = 0; i < HEX_LEN; i++) { 
		str_hex_num += hex_dic[std::string(bin_num.substr(i*4, 4))];
	}
}
