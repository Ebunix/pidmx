#pragma once
#include <map>
#include <string>
#include <vector>
#include <js.h>

class Modulator {
public:
	std::string name;
	std::string identifier;

	Modulator(const std::string& identifier, bool root = false): identifier(identifier), isRoot(root) {}
	static void Register(const v8::FunctionCallbackInfo<v8::Value>& info);

	void SetParameter(const std::string& name, float value = 0);
	void SetInput(const std::string& name, Modulator* mod);

	void PrepareUpdate();
	void Update();
	float GetValue();
	Modulator* Clone();
	
private:
	v8::Global<v8::Function> op;
	std::vector<std::pair<std::string, float>> parameters;
	std::map<uint64_t, Modulator*> inputs;
	float value = 0.0f;
	bool isRoot = false;
	bool isUpdated = false;
};

