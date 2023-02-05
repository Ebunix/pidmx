#pragma once

#include <nbt_tags.h>
#include <pidmx_utils.h>
#include <vector>
#include <memory>
#include <fstream>
#include "engine/Console.h"
#include <filesystem>
#include <imgui_internal.h>

#ifdef NBT_USE_ZLIB
#include "io/izlibstream.h"
#include "io/ozlibstream.h"
#endif

namespace fs = std::filesystem;

class ISerializable {
public:
    Hash id = INVALID_HASH;
    std::string name;

    virtual void Load(const nbt::tag_compound &pack) {
        id = pack.at("id").as<nbt::tag_long>().get();
        name = pack.at("name").as<nbt::tag_string>().get();
    }

    virtual nbt::tag_compound Save() {
        return nbt::tag_compound{
                {"id",   id},
                {"name", name}
        };
    }

    virtual void afterLoad() {}
};

#define NBT_SAVE(type, code) namespace nbt { inline void Save(nbt::tag_compound &comp, const std::string &tag, const type& value) code }
#define NBT_LOAD(type, code) namespace nbt { inline type Load(const nbt::tag_compound &comp, const std::string &tag, const type& fallback) code }
#define NBT_SAVE_MEMBER(member) Save(comp, #member, value.member)
#define NBT_SAVE_MEMBER_COND(member, condition) if (condition) Save(comp, #member, value.member)
#define NBT_LOAD_MEMBER(member) value.member = Load(comp, #member, value.member)
#define NBT_SAVE_VEC(type) NBT_SAVE(std::vector<type>, { \
    tag_list list;                          \
    for (const auto& val : value) {         \
        tag_compound obj;                   \
        Save(obj, "", val);                 \
        list.push_back(std::move(obj));     \
    }                                       \
    comp.insert(tag, std::move(list));      \
})
#define NBT_LOAD_VEC(type) NBT_LOAD(std::vector<type>, { \
    if (!comp.has_key(tag) || comp.at(tag).get_type() != tag_type::List) return fallback; \
    tag_list list = comp.at(tag).as<tag_list>(); \
    if (list.el_type() != tag_type::Compound) return fallback; \
    std::vector<type> result;                            \
    for (const auto& val : list) {                       \
        result.push_back(Load(val.as<tag_compound>(), "", type {}));       \
    }                                                    \
    return result;                                       \
})

namespace nbt {
    inline std::unique_ptr<tag_compound> LoadFromFile(const std::string &path) {
        if (!fs::exists(path)) {
            LogMessage(LogMessageType_Error, "File %s not found while loading NBT data", path.c_str());
            return std::make_unique<nbt::tag_compound>();
        }
        LogMessage(LogMessageType_Info, "Loading %s", path.c_str());
        std::ifstream in(path);
        if (in.peek() == 0x0a)
            return nbt::io::read_compound(in).second;
#if NBT_USE_ZLIB
        zlib::izlibstream igzs(in);
        return nbt::io::read_compound(igzs).second;
#else
        LogMessage(LogMessageType_Error, "Trying to load compressed NBT file, but zlib was disabled during build!");
#endif
        return std::make_unique<nbt::tag_compound>();
    }

    inline void SaveToFile(const tag_compound &comp, const std::string &path, bool compress = false) {
        LogMessage(LogMessageType_Info, "Saving %s", path.c_str());
        std::ofstream out(path);
        if (compress) {
#if NBT_USE_ZLIB
            zlib::ozlibstream ogzs(out, -1, true);
            nbt::io::write_tag("", comp, ogzs);
            ogzs.close();
#else
            LogMessage(LogMessageType_Warn, "Trying to save compressed NBT file, but zlib was disabled during build. Saving uncompressed file instead.");
            nbt::io::write_tag("", comp, out);
            out.close();
#endif
        } else {
            nbt::io::write_tag("", comp, out);
            out.close();
        }
    }
}

#define NBT_SAVE_LOAD_SIMPLE(type, nbtType, nbtTag, nbtTagEnum) \
    NBT_SAVE(type, { comp.insert(tag, nbtTag((nbtType)value)); }) \
    NBT_SAVE(std::vector<type>, { tag_list list; for (const auto& val : value) list.push_back(nbtTag((nbtType)val)); comp.insert(tag, std::move(list)); }) \
    NBT_LOAD(type, { return (type)((comp.has_key(tag) && comp.at(tag).get_type() == (nbtTagEnum)) ? comp.at(tag).as<nbtTag>().get() : fallback); })        \
    NBT_LOAD(std::vector<type>, { \
        if (!comp.has_key(tag) || comp.at(tag).get_type() != tag_type::List) return fallback; \
        tag_list list = comp.at(tag).as<tag_list>(); \
        if (comp.at(tag).get_type() != (nbtTagEnum)) return fallback; \
        std::vector<type> result; \
        for (const auto& val : list) result.push_back((type)val.as<nbtTag>().get()); \
        return result; \
    })

NBT_SAVE_LOAD_SIMPLE(int8_t, int8_t, tag_byte, tag_type::Byte)
NBT_SAVE_LOAD_SIMPLE(int16_t, int16_t, tag_short, tag_type::Short)
NBT_SAVE_LOAD_SIMPLE(int32_t, int32_t, tag_int, tag_type::Int)
NBT_SAVE_LOAD_SIMPLE(int64_t, int64_t, tag_long, tag_type::Long)
NBT_SAVE_LOAD_SIMPLE(uint8_t, int8_t, tag_byte, tag_type::Byte)
NBT_SAVE_LOAD_SIMPLE(uint16_t, int16_t, tag_short, tag_type::Short)
NBT_SAVE_LOAD_SIMPLE(uint32_t, int32_t, tag_int, tag_type::Int)
NBT_SAVE_LOAD_SIMPLE(uint64_t, int64_t, tag_long, tag_type::Long)
NBT_SAVE_LOAD_SIMPLE(float, float, tag_float, tag_type::Float)
NBT_SAVE_LOAD_SIMPLE(double, double, tag_double, tag_type::Double)
NBT_SAVE_LOAD_SIMPLE(std::string, std::string, tag_string, tag_type::String)


namespace nbt {
    inline void Save(nbt::tag_compound &comp, const std::string &tag, const ImVec2 &value) {
        tag_compound vec;
        Save(vec, "x", value.x);
        Save(vec, "y", value.y);
        comp.insert(tag, std::move(vec));
    }

    inline ImVec2 Load(const nbt::tag_compound &comp, const std::string &tag, const ImVec2 &fallback) {
        if (!comp.has_key(tag))
            return fallback;
        const auto &vec = comp.at(tag).as<tag_compound>();
        return {Load(vec, "x", 0.0f), Load(vec, "y", 0.0f)};
    }
}
NBT_SAVE_VEC(ImVec2)
NBT_LOAD_VEC(ImVec2)


namespace nbt {
    inline void Save(nbt::tag_compound &comp, const std::string &tag, const ImRect &value) {
        tag_compound vec;
        Save(vec, "min", value.Min);
        Save(vec, "max", value.Max);
        comp.insert(tag, std::move(vec));
    }

    inline ImRect Load(const nbt::tag_compound &comp, const std::string &tag, const ImRect &fallback) {
        if (!comp.has_key(tag))
            return fallback;
        const auto &vec = comp.at(tag).as<tag_compound>();
        return {Load(vec, "min", ImVec2(0, 0)), Load(vec, "max", ImVec2(0, 0))};
    }
}
NBT_SAVE_VEC(ImRect)
NBT_LOAD_VEC(ImRect)


namespace nbt {
    inline void Save(nbt::tag_compound &comp, const std::string &tag, void* data, size_t dataLen) {
        tag_byte_array bytes;
        bytes.get().resize(dataLen);
        memcpy(&bytes.get()[0], data, dataLen);
        comp.insert(tag, std::move(bytes));
    }

    inline void* Load(const nbt::tag_compound &comp, const std::string &tag, void* fallback, size_t* length) {
        if (!comp.has_key(tag)) {
            *length = 0;
            return fallback;
        }
        tag_byte_array bytes = comp.at(tag).as<tag_byte_array>();
        *length = bytes.size();
        void* data = malloc(*length);
        memcpy(data, &bytes.get()[0], *length);
        return data;
    }
}