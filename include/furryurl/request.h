#pragma once

#include <string>

#include <curl/curl.h>

#include <furryurl/methods.h>

namespace furryurl {
    class Request {
    public:
        Request(std::string url);

        inline std::string getUrl() { return m_url; };
        inline HTTPMethod getMethod() { return m_method; };
        
        void setUrl(std::string url);
        void setMethod(HTTPMethod method);

        Response send();

    protected:
        CURL* m_curlInst;

        std::string m_url;
        HTTPMethod m_method;
        std::string m_postFields;
    };
}