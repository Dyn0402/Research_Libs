/*
 * lconfig.h
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */

#ifndef CONFIG_LIB_H_
#define CONFIG_LIB_H_

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace io {
	//File io
	extern string nproton_file_pre;
	extern vector<string> nproton_file_fields;
	extern char file_name_delimeter;
	extern vector<char> data_delimeters;
	extern string ratios_file_pre;
	extern vector<string> ratios_file_fields;
	extern string file_ext;
}




#endif /* CONFIG_LIB_H_ */
