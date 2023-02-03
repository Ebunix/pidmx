#pragma once
#include <nbt_tags.h>
#include <pidmx_utils.h>
#include <vector>
#include <memory>

class ISerializable {
public:
	Hash id = INVALID_HASH;
	std::string name;

	virtual void load(const nbt::tag_compound& pack) {
        id = pack.at("id").as<nbt::tag_long>().get();
        name = pack.at("name").as<nbt::tag_string>().get();
    }
	virtual nbt::tag_compound save() {
        return nbt::tag_compound {
                { "id", id },
                { "name", name }
        };
    }
    virtual void afterLoad() {}
};
