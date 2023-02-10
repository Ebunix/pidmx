#pragma once

#include <nbt_tags.h>
#include "pidmx_utils.h"
#include "ISer.h"
#include "Collections.h"

namespace nbt {
    static tag_byte Serialize(const bool &input) { return {(int8_t) input}; }

    static tag_byte Serialize(const int8_t &input) { return {input}; }

    static tag_short Serialize(const int16_t &input) { return {input}; }

    static tag_int Serialize(const int32_t &input) { return {input}; }

    static tag_long Serialize(const int64_t &input) { return {input}; }

    static tag_byte Serialize(const uint8_t &input) { return {(int8_t) input}; }

    static tag_short Serialize(const uint16_t &input) { return {(int16_t) input}; }

    static tag_int Serialize(const uint32_t &input) { return {(int32_t) input}; }

    static tag_long Serialize(const uint64_t &input) { return {(int64_t) input}; }

    static tag_float Serialize(const float &input) { return {input}; }

    static tag_double Serialize(const double &input) { return {input}; }

    static tag_string Serialize(const std::string &input) { return {input}; }

    static tag_compound Serialize(const std::shared_ptr<ISer> &input) { return input ? input->Save() : tag_compound(); }

    static tag_compound Serialize(const ISer &input) { return input.Save(); }

    static tag_byte_array Serialize(const signed char *data, size_t len) {
        tag_byte_array bytes;
        bytes.get().resize(len);
        memcpy(&bytes.get()[0], data, len);
        return bytes;
    }


    static bool Deserialize(const value &input, const bool &fallback) {
        bool valid = input.get_type() == tag_type::Byte;
        return (bool) (valid ? input.as<tag_byte>().get() : fallback);
    }

    static int8_t Deserialize(const value &input, const int8_t &fallback) {
        bool valid = input.get_type() == tag_type::Byte;
        return (int8_t) (valid ? input.as<tag_byte>().get() : fallback);
    }

    static int16_t Deserialize(const value &input, const int16_t &fallback) {
        bool valid = input.get_type() == tag_type::Short;
        return (int16_t) (valid ? input.as<tag_short>().get() : fallback);
    }

    static int32_t Deserialize(const value &input, const int32_t &fallback) {
        bool valid = input.get_type() == tag_type::Int;
        return (int32_t) (valid ? input.as<tag_int>().get() : fallback);
    }

    static int64_t Deserialize(const value &input, const int64_t &fallback) {
        bool valid = input.get_type() == tag_type::Long;
        return (int64_t) (valid ? input.as<tag_long>().get() : fallback);
    }

    static uint8_t Deserialize(const value &input, const uint8_t &fallback) {
        bool valid = input.get_type() == tag_type::Byte;
        return (uint8_t) (valid ? input.as<tag_byte>().get() : fallback);
    }

    static uint16_t Deserialize(const value &input, const uint16_t &fallback) {
        bool valid = input.get_type() == tag_type::Short;
        return (uint16_t) (valid ? input.as<tag_short>().get() : fallback);
    }

    static uint32_t Deserialize(const value &input, const uint32_t &fallback) {
        bool valid = input.get_type() == tag_type::Int;
        return (uint32_t) (valid ? input.as<tag_int>().get() : fallback);
    }

    static uint64_t Deserialize(const value &input, const uint64_t &fallback) {
        bool valid = input.get_type() == tag_type::Long;
        return (uint64_t) (valid ? input.as<tag_long>().get() : fallback);
    }

    static float Deserialize(const value &input, const float &fallback) {
        bool valid = input.get_type() == tag_type::Float;
        return (float) (valid ? input.as<tag_float>().get() : fallback);
    }

    static double Deserialize(const value &input, const double &fallback) {
        bool valid = input.get_type() == tag_type::Double;
        return (double) (valid ? input.as<tag_double>().get() : fallback);
    }

    static std::string Deserialize(const value &input, const std::string &fallback) {
        bool valid = input.get_type() == tag_type::String;
        return (std::string) (valid ? input.as<tag_string>().get() : fallback);
    }

    template<typename T>
    static std::shared_ptr<T> Deserialize(const value &input, const std::shared_ptr<T> &fallback) {
        bool valid = input.get_type() == tag_type::Compound && input.as<tag_compound>().has_key("id");
        if (!valid) {
            return fallback;
        }
        auto ptr = std::make_shared<T>();
        ptr->Load(input.as<tag_compound>());
        return ptr;
    }
    template<typename T>
    static T Deserialize(const value &input, const T &fallback) {
        bool valid = input.get_type() == tag_type::Compound && input.as<tag_compound>().has_key("id");
        if (!valid) {
            return fallback;
        }
        T val;
        val.Load(input.as<tag_compound>());
        return val;
    }

    static const signed char *Deserialize(const value &input, const signed char *fallback, size_t *len) {
        bool valid = input.get_type() == tag_type::Byte_Array;
        auto data = input.as<tag_byte_array>().get();
        if (valid) {
            *len = data.size();
            signed char* result = new signed char[*len];
            memcpy(result, data.data(), *len);
            return result;
        }
        return fallback;
    }


    static bool Deserialize(const tag_compound &input, const std::string &key, const bool &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Byte;
        return (bool) (valid ? input.at(key).as<tag_byte>().get() : fallback);
    }

    static int8_t Deserialize(const tag_compound &input, const std::string &key, const int8_t &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Byte;
        return (int8_t) (valid ? input.at(key).as<tag_byte>().get() : fallback);
    }

    static int16_t Deserialize(const tag_compound &input, const std::string &key, const int16_t &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Short;
        return (int16_t) (valid ? input.at(key).as<tag_short>().get() : fallback);
    }

    static int32_t Deserialize(const tag_compound &input, const std::string &key, const int32_t &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Int;
        return (int32_t) (valid ? input.at(key).as<tag_int>().get() : fallback);
    }

    static int64_t Deserialize(const tag_compound &input, const std::string &key, const int64_t &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Long;
        return (int64_t) (valid ? input.at(key).as<tag_long>().get() : fallback);
    }

    static uint8_t Deserialize(const tag_compound &input, const std::string &key, const uint8_t &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Byte;
        return (uint8_t) (valid ? input.at(key).as<tag_byte>().get() : fallback);
    }

    static uint16_t Deserialize(const tag_compound &input, const std::string &key, const uint16_t &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Short;
        return (uint16_t) (valid ? input.at(key).as<tag_short>().get() : fallback);
    }

    static uint32_t Deserialize(const tag_compound &input, const std::string &key, const uint32_t &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Int;
        return (uint32_t) (valid ? input.at(key).as<tag_int>().get() : fallback);
    }

    static uint64_t Deserialize(const tag_compound &input, const std::string &key, const uint64_t &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Long;
        return (uint64_t) (valid ? input.at(key).as<tag_long>().get() : fallback);
    }

    static float Deserialize(const tag_compound &input, const std::string &key, const float &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Float;
        return (float) (valid ? input.at(key).as<tag_float>().get() : fallback);
    }

    static double Deserialize(const tag_compound &input, const std::string &key, const double &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Double;
        return (double) (valid ? input.at(key).as<tag_double>().get() : fallback);
    }

    static std::string Deserialize(const tag_compound &input, const std::string &key, const std::string &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::String;
        return (std::string) (valid ? input.at(key).as<tag_string>().get() : fallback);
    }

    static const signed char *Deserialize(const tag_compound &input, const std::string &key, const signed char *fallback, size_t *len) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::Byte_Array;
        auto data = input.at(key).as<tag_byte_array>().get();
        if (valid) {
            *len = data.size();
            signed char* result = new signed char[*len];
            memcpy(result, data.data(), *len);
            return result;
        }
        return fallback;
    }




    template<typename T>
    static std::shared_ptr<T> Deserialize(const value &input, const std::string &key, const std::shared_ptr<T> &fallback) {
        bool valid = input.get_type() == tag_type::Compound && input.as<tag_compound>().has_key(key);
        return valid ? Deserialize(input.at(key).as<tag_compound>(), fallback) : fallback;
    }

    static const signed char *Deserialize(const value &input, const std::string &key, const signed char *fallback, size_t *len) {
        bool valid = input.get_type() == tag_type::Compound && input.as<tag_compound>().has_key(key);
        return valid ? Deserialize(input.at(key), fallback, len) : fallback;
    }





    template<typename T, typename U>
    static tag_compound Serialize(const std::pair<T, U> &input) {
        tag_compound container;
        container.insert("first", Serialize(input.first));
        container.insert("second", Serialize(input.second));
        return container;
    }



    template<typename It>
    static tag_list Serialize(It begin, It end) {
        tag_list list;
        while (begin != end) {
            list.push_back(Serialize(*begin));
            begin++;
        }
        return list;
    }

    template<typename T, typename U>
    static Map<T, U> Deserialize(const tag_compound &input, const std::string &key, const Map<T, U> &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::List;
        if (!valid) {
            return fallback;
        }
        tag_list list = input.at(key).as<tag_list>();
        Map<T, U> result;
        for (const auto& val : list) {
            if (val.get_type() != tag_type::Compound) {
                return fallback;
            }
            const value &first = val.at("first");
            const value &second = val.at("second");
            result.insert(std::make_pair(Deserialize(first, T()), Deserialize(second, U())));
        }
        return result;
    }
    template<typename T>
    static Set<T> Deserialize(const tag_compound &input, const std::string &key, const Set<T> &fallback) {
        bool valid = input.has_key(key) && input.at(key).get_type() == tag_type::List;
        if (!valid) {
            return fallback;
        }
        tag_list list = input.at(key).as<tag_list>();
        Set<T> result;
        for (const auto& val : list) {
            result.insert(Deserialize(val, T()));
        }
        return result;
    }

    template<typename T>
    static std::shared_ptr<T> Deserialize(const tag_compound &input, const std::string &key, const std::shared_ptr<T> &fallback) {
        bool valid = input.has_key(key);
        if (!valid) {
            return fallback;
        }
        return Deserialize(input.at(key), fallback);
    }

}

#define NBT_ENUM_SERIALIZER_IMPL(type) namespace nbt { \
    static tag_int Serialize(const type &input) { return {(int)input}; } \
    static type Deserialize(const value &input, const type &fallback) { \
        bool valid = input.get_type() == tag_type::Int; \
        return (valid ? (type)input.as<tag_int>().get() : fallback); \
    } \
}


