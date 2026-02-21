#include <furryurl/request.h>

#include <stdexcept>

#include <curl/curl.h>
#include <curl/easy.h>

#include <furryurl/method.h>

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

    m_url = url;
    m_method = HTTPMethod::Get;

    curl_easy_setopt(m_curlInst, CURLOPT_CA_CACHE_TIMEOUT, 604800);
}

void Request::prepareRequest() {
    // Set Options for the given HTTP Method
    switch(m_method) {
        case HTTPMethod::Delete:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        case HTTPMethod::Get:
            curl_easy_setopt(m_curlInst, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "GET");
            break;
        case HTTPMethod::Head:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "HEAD");
            break;
        case HTTPMethod::Options:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "OPTIONS");
            break;
        case HTTPMethod::Patch:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "PATCH");
            break;
        case HTTPMethod::Post:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "POST");
            break;
        case HTTPMethod::Put:
            curl_easy_setopt(m_curlInst, CURLOPT_CUSTOMREQUEST, "PUT");
            break;
    }

    // Set Headers
    curl_slist* headerList = nullptr;
    for (auto& header : m_headers) {
        std::string headerString = header.first;
        if (!header.second.empty()) headerString += ": " + header.second;

        if (auto newList = curl_slist_append(headerList, headerString.c_str())) {
            headerList = newList;
        }

        curl_easy_setopt(m_curlInst, CURLOPT_HTTPHEADER, headerList);
        curl_slist_free_all(headerList);
    }

    // Set URL
    curl_easy_setopt(m_curlInst, CURLOPT_URL, m_url.c_str());

    // Allow retreival of Certificate Info
    curl_easy_setopt(m_curlInst, CURLOPT_CERTINFO, 1L);

    // Set Body
    if (
        m_method == HTTPMethod::Patch ||
        m_method == HTTPMethod::Post ||
        m_method == HTTPMethod::Put
    ) {
        curl_easy_setopt(m_curlInst, CURLOPT_POSTFIELDS, m_body.data());
        curl_easy_setopt(m_curlInst, CURLOPT_POSTFIELDSIZE, m_body.size());
    }
}