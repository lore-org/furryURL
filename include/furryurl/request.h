#pragma once

#include <string>

#include <curl/curl.h>

#include "method.h"
#include "response.h"
#include "headers.h"

namespace furryurl {

class Request {
public:
    Request(std::string url);
    
    Headers m_headers;
    std::string m_url;
    HTTPMethod m_method;
    std::string m_body;

    Response send();

protected:
    CURL* m_curlInst;

    void prepareRequest();
};

}