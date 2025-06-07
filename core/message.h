#pragma once

#include <vector>

enum struct Method : int8_t {
    GET,
    PUT
};

enum struct PutType : int8_t {
    Create,    // create new db
    Delete,    // delete key
    Update     // update key/value pair
};

enum struct State : int8_t {
    kOK,
    kInvDB,
    kInvKey
};

using IOBuf = std::vector<char>;

#pragma pack(1)

struct MessageHead {
    int32_t length;      // message length
    int8_t method;       // GET or PUT
};

struct GetRequest {
    int32_t db_len;
    int32_t key_len;
    char body[];
};

struct GetResponse {
    int8_t state;
    int32_t key_len;
    int32_t value_len;
    const char* key;
    const char* value;
};

struct PutRequest {
    int8_t put_type;    // for PutType
    int32_t db_len;
    int32_t key_len;
    int32_t value_len;
    char body[];
};

struct PutResponse {
    int8_t state;
};

#pragma pack()
