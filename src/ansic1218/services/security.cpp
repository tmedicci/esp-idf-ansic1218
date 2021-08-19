
#include <algorithm>
#include "security.hpp"
#include "mbedtls/md.h"

using namespace std;
using namespace ansic1218::service;

static const char *TAG = "ansic1218::services:security";

Security::Security(const vector<uint8_t> &identity, const vector<uint8_t> &password)
    : Service(__PRETTY_FUNCTION__), _request(), _response()
{
    if (identity.size() < IDENTITY_LAST_N_BYTES) {
        ESP_LOGE(TAG, "Invalid identity size");
        return;
    }

    if (password.size() < IDENTITY_LAST_N_BYTES) {
        ESP_LOGE(TAG, "Invalid password size");
        return;
    }

    unsigned int md_len = MBEDTLS_MD_MAX_SIZE;
    vector<uint8_t> md(md_len);

    if (mbedtls_md_hmac(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), password.data(), password.size(),
                        identity.data() + identity.size() - IDENTITY_LAST_N_BYTES, IDENTITY_LAST_N_BYTES, md.data())) {
        ESP_LOGE(TAG, "Couldn't generate HMAC");
    }

    _request.resize(md_len);
    _request.clear();
    _request.push_back(uint8_t(SECURITY));

    copy(md.begin(), md.begin() + std::min(md_len, static_cast<unsigned int>(SECURITY_PASSWORD_SIZE)),
         back_inserter(_request));
}

bool Security::request(std::vector<uint8_t> &buffer)
{
    copy(_request.cbegin(), _request.cend(), back_inserter(buffer));

    return true;
}

bool Security::response(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    return Service::validate(first, last);
}
