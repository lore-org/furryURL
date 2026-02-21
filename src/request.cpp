#include <furryurl/request.h>

#include <stdexcept>

#include <furryurl/methods.h>

using namespace furryurl;

Request::Request(std::string url) {
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        curl_global_cleanup();
        throw std::runtime_error("furryURL: could not initialise global cURL");   
    }

    m_curlInst = curl_easy_init();
    if (!m_curlInst) {
        curl_easy_cleanup(m_curlInst);
        throw std::runtime_error("furryURL: could not initialise local cURL");
    }

    this->setUrl(url);
    this->setMethod(HTTPMethod::Get);

    curl_easy_setopt(m_curlInst, CURLOPT_CA_CACHE_TIMEOUT, 604800);
}

void Request::setUrl(std::string url) {
    m_url = url;
    curl_easy_setopt(m_curlInst, CURLOPT_URL, m_url.c_str());
}

void Request::setMethod(HTTPMethod method) {
    m_method = method;

    // Reset all options
    curl_easy_setopt(m_curlInst, CURLOPT_HTTPGET, 0);
    curl_easy_setopt(m_curlInst, CURLOPT_NOBODY, 0);
    curl_easy_setopt(m_curlInst, CURLOPT_POST, 0);
    curl_easy_setopt(m_curlInst, CURLOPT_POSTFIELDS, NULL);
    curl_easy_setopt(m_curlInst, CURLOPT_UPLOAD, 0);
    curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, NULL);

    switch(m_method) {
        case HTTPMethod::Get:
            curl_easy_setopt(m_curlInst, CURLOPT_HTTPGET, 1);
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "GET");
        case HTTPMethod::Head:
            curl_easy_setopt(m_curlInst, CURLOPT_NOBODY, 1);
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "HEAD");
        case HTTPMethod::Post:
            curl_easy_setopt(m_curlInst, CURLOPT_POST, 1);
            curl_easy_setopt(m_curlInst, CURLOPT_POSTFIELDS, m_postFields.c_str());
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "POST");
        case HTTPMethod::Put:
            curl_easy_setopt(m_curlInst, CURLOPT_UPLOAD, 1);
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "PUT");
        case HTTPMethod::Delete:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "DELETE");
        case HTTPMethod::Connect:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "CONNECT");
        case HTTPMethod::Options:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "OPTIONS");
        case HTTPMethod::Trace:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "TRACE");
        case HTTPMethod::Patch:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "PATCH");
    }
}