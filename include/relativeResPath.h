#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace rrp{
    inline std::string getParentDir() {
        return fs::current_path().parent_path().string();
    }
    
    inline std::string getResourcesPath() {
        return getParentDir() + "/res";
    }

    inline std::string getFragmentPath() {
        return getResourcesPath() + "/fragment/";
    }

    inline std::string getTexturePath() {
        return getResourcesPath() + "/texture/";
    }

    inline std::string getVertexPath() {
        return getResourcesPath() + "/vertex/";
    }
}