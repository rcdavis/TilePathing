#include "Utils/FileUtils.h"

#include "Log.h"

#include <fstream>

namespace FileUtils
{
    std::string ReadText(const std::filesystem::path& filepath)
    {
        std::ifstream file(filepath, std::ios::in);
        if (!file)
        {
            LOG_ERROR("Failed to open file {0}", filepath.string());
            return "";
        }

		return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
}
