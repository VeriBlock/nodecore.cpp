#pragma once

#include <string>

namespace vbk {

    class FileProvider {
    public:
        virtual std::ofstream *getDataFile(const std::wstring &fileName) = 0;
    };
}
