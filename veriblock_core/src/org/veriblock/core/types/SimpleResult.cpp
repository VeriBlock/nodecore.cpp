#include "SimpleResult.h"

namespace vbk {
    SimpleResult::SimpleResult(bool success, const std::string &error) {
        this->success = success;
        this->errorString = error;
    }

    std::string SimpleResult::getError() {
        return errorString;
    }

    bool SimpleResult::wasSuccessful() {
        return success;
    }
}
