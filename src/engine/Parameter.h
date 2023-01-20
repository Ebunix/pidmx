#pragma once
#include <string>

enum class Parameter {
	Unassigned,
	
	Undefined,
	Brightness,
	Red,
	Green,
	Blue,
	Strobe
};

Parameter ParameterFromName(const std::string& name);
const std::string NameFromParameter(Parameter param);
