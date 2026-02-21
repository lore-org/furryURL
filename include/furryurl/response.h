#pragma once

#include <string>

#include "method.h"

namespace furryurl {

class Response {
public:
    Response();

    

protected:
    std::string m_url;
    HTTPMethod m_method;
    std::string m_body;
};

}