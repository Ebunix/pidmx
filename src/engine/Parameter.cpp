#include "Parameter.h"

#define PARAM(param) else if (name.compare(#param) == 0) { return Parameter::param; }
Parameter ParameterFromName(const std::string &name) {
	if (name.compare("Undefined") == 0 || name.compare("undefined") == 0) {
		return Parameter::Undefined;
	}
	PARAM(Unassigned)
	PARAM(Brightness)
	PARAM(Red)
	PARAM(Green)
	PARAM(Blue)
	PARAM(Strobe)

	return Parameter::Undefined;
}
#undef PARAM

#define PARAM(param) case Parameter::param: return #param;
const std::string NameFromParameter(Parameter param) {
	switch (param) {
		PARAM(Unassigned)
		PARAM(Undefined)
		PARAM(Brightness)
		PARAM(Red)
		PARAM(Green)
		PARAM(Blue)
		PARAM(Strobe)
	}
	return "unknown";
}
#undef PARAM
