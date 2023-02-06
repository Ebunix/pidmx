#pragma once

#include <nbt_tags.h>
#include <pidmx_utils.h>
#include <vector>
#include <memory>
#include <fstream>
#include "engine/Console.h"
#include "Collections.h"
#include <filesystem>
#include <imgui_internal.h>
#include <set>
#ifdef NBT_USE_ZLIB
#include "io/izlibstream.h"
#include "io/ozlibstream.h"
#endif

namespace fs = std::filesystem;

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
            LogMessage(LogMessageType_Warn,
                       "Trying to save compressed NBT file, but zlib was disabled during build. Saving uncompressed file instead.");
            nbt::io::write_tag("", comp, out);
            out.close();
#endif
        } else {
            nbt::io::write_tag("", comp, out);
            out.close();
        }
    }
}


