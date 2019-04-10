#pragma once

#include <string>

namespace vbk {

    class SimpleResult {
    private:
        std::string errorString;
        bool success = false;

    public:
        SimpleResult(bool success, const std::string &error);

        virtual std::string getError();

        virtual bool wasSuccessful();
    };
}
