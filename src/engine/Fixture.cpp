#include "Fixture.h"
#include "Engine.h"
#include "JsTools.h"

using namespace v8;

#define PARAM(param) else if (name.compare(#param) == 0) { return FixtureControlType::param; }
FixtureControlType ControlTypeFromName(const std::string &name)
{	
	if (name.compare("Simple") == 0) {
		return FixtureControlType::Simple;
	}
	PARAM(Color)
	PARAM(Select)

	return FixtureControlType::Simple;
}
#undef PARAM

#define PARAM(param) case FixtureControlType::param: return #param;
const std::string NameFromControlType(FixtureControlType param)
{	
	switch (param) {
		PARAM(Simple)
		PARAM(Color)
		PARAM(Select)
	}
	return "Simple";
}
#undef PARAM

Fixture::Fixture(uint16_t startSlot, uint16_t slotCount, Universe* universe, FixturePreset* preset):
	slot(startSlot), slotCount(slotCount), sourcePreset(preset)
{
	SetUniverse(universe);
}

void Fixture::RegisterPreset(const v8::FunctionCallbackInfo<v8::Value> &info) {
	if (info.Length() < 1) {
		LogMessage(ConsoleMessageType_Error,  "Wrong number of arguments. Expected 1, got %i", info.Length());
		js::global::isolate->ThrowError(V8StrCheck("Wrong number of arguments."));
		return;
	}

	FixturePreset* preset = new FixturePreset();

	Local<Context> ctx = info.GetIsolate()->GetCurrentContext();
	Local<Object> data = info[0].As<Object>();
	Local<Value> temp;
	
	preset->name = V8CStr(ctx, data->Get(ctx, V8StrCheck("name")).ToLocalChecked());
	
	Local<Array> slotsList;
	if (Walk(ctx, data, "slots", &slotsList)) {;
		for (int i = 0; i < slotsList->Length(); i++) {
			FixtureSlot slot;
			ImVec4 tint;
			if (Walk(ctx, slotsList, i, &temp)) {
				if (!JsValueToSlot(ctx, temp, &slot.name, &tint)) {
					LogMessage(ConsoleMessageType_Error,  "Failed to read slot %i on preset %s", i, preset->name.c_str());
				}
				else {
					slot.tint = ImGui::MakeTintPreset(ImGui::sliderTintPresetAssigned, tint);
					preset->slots.push_back(slot);
				}
			}
		}
	}
	Local<Array> controlsList;
	if (Walk(ctx, data, "controls", &controlsList)) {
		for (int i = 0; i < controlsList->Length(); i++) {
			Local<Object> ctrlData;
			if (!Walk(ctx, controlsList, i, &ctrlData)) {
				continue;
			}

			FixtureControl ctrl;
			if (Walk(ctx, ctrlData, "slots", &temp)) {
				ctrl.slots = JsValueToIntArray(ctx, temp);
			}
			if (Walk(ctx, ctrlData, "type", &temp)) {
				ctrl.type = ControlTypeFromName(V8CStr(ctx, temp));
			}
			switch (ctrl.type) {
				case FixtureControlType::Color: {
					if (Walk(ctx, ctrlData, "order", &temp)) {
						ctrl.colorOrder = V8CStr(ctx, temp);
					}
					break;
				}
				case FixtureControlType::Select: {
					Local<Array> selectList;
					if (Walk(ctx, ctrlData, "slots", &selectList)) {
						for (int i = 0; i < selectList->Length(); i++) {
							FixtureControlRange fcr;
							if (Walk(ctx, selectList, i, &temp) && temp->IsObject()) {
								Local<Value> v;
								if (Walk(ctx, temp.As<Object>(), "from", &v) && v->IsNumber()) {
									fcr.from = (int)v.As<Number>()->Value();
								}
								if (Walk(ctx, temp.As<Object>(), "to", &v) && v->IsNumber()) {
									fcr.to = (int)v.As<Number>()->Value();
								}
								if (Walk(ctx, temp.As<Object>(), "label", &v) && v->IsString()) {
									fcr.label = V8CStr(ctx, v);
								}
							}
							ctrl.selects.push_back(fcr);
						}
					}
					break;
				}
				default: break;
			}

			preset->controls.push_back(std::move(ctrl));
		}
	}

	engine->fixturePresets.push_back(preset);
}

void Fixture::UnOccupy()
{
	if (!universe) {
		return;
	}
	universe->SetSlotRangeOccupied(slot, slot + slotCount, nullptr);
}

void Fixture::Occupy()
{
	if (!universe) {
		return;
	}
	universe->SetSlotRangeOccupied(slot, slot + slotCount, this);
}

bool Fixture::SetUniverse(Universe *u)
{
	if (u && !u->GetSlotRangeAvailable(slot, slotCount)) {
		LogMessage(ConsoleMessageType_Warn,  "Tried to occupy slot range %i-%i on %s, which is already (partially) occupied!", slot, slot + slotCount, universe->name.c_str());
		return false;
	}
	UnOccupy();
	universe = u;
	Occupy();

	return true;
}



bool FixturePatchStringToUniverseChannel(char *text, size_t len, int *universe, int *channel)
{
	char data[6];
	if (len > sizeof(data) || strlen(text) == 0) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if ((text[i] < '0' || text[i] > '9') && text[i] != '.' && text[i] != 0) {
			return false;
		}
	}
	strncpy(data, text, sizeof(data));

	char* channelText = nullptr;
	char* universeText = nullptr;

	char* separator = strchr(data, '.');
	if (separator) {
		*separator = 0;
		channelText = separator + 1;
		universeText = data;
	}
	else {
		channelText = data;
	}

	if (universeText) {
		*universe = atoi(universeText);
	}
	else {
		*universe = 0;
	}
	if (channelText) {
		*channel = atoi(channelText);
	}
	else {
		*channel = 0;
	}

	return *channel >= 1 && *channel <= 512;
}

bool FixtureNew::RedoPatch(FixturePatch* patch)
{
	if (!engine->GetFixture(patch->fixtureId)) {
		engine->showFile->fixtures.push_back(std::make_shared<FixtureNew>(*patch));
	}
	return true;
}

bool FixtureNew::UndoPatch(FixturePatch* patch)
{
	for (auto iter = engine->showFile->fixtures.begin(); iter < engine->showFile->fixtures.end(); iter++) {
		if ((*iter)->patch.fixtureId == patch->fixtureId) {
			engine->showFile->fixtures.erase(iter);
			return true;
		}
	}
	return false;
}

void FixtureNew::PatchFixture(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	Local<Value> temp;
	Local<Context> ctx = info.GetIsolate()->GetCurrentContext();
	Local<Object> data = info[0].As<Object>();
	FixturePatch* patch = new FixturePatch();

	Local<String> v8Name;
	if (Walk(ctx, data, "name", &v8Name)) {
		patch->name = V8CStr(ctx, v8Name);
	}
	Local<Number> v8Channel;
	if (Walk(ctx, data, "channel", &v8Channel)) {
		patch->channel = (int)v8Channel->Value();
	}
	Local<Number> v8Universe;
	if (Walk(ctx, data, "universe", &v8Universe)) {
		patch->universe = (int)v8Universe->Value();
	}
	Local<Number> v8FixtureId;
	if (Walk(ctx, data, "id", &v8FixtureId)) {
		patch->fixtureId = (int)v8FixtureId->Value();
	}

	Rewind::AddDeallocResource(patch);
	Rewind::RecordStep((Rewind::Step)FixtureNew::UndoPatch, (Rewind::Step)FixtureNew::RedoPatch, patch);
}

