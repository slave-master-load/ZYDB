#include <iostream>
#include <cstring>

#include "core/db_client.h"

int main(int argc, char** argv) {
    zydb::DBClient client("192.168.153.128", 9999);

    std::string operation;
    std::string db_name;
    std::string key;
    std::string value;

    while (true) {
        std::cout << "zydb> ";
        std::cin >> operation;
        if (operation == "create") {
            std::cin >> db_name;
            int length = sizeof(int8_t) + sizeof(int32_t) * 3 + db_name.length();
            IOBuf buffer(length);
            auto& request = *reinterpret_cast<PutRequest*>(buffer.data());
            request.put_type = static_cast<int8_t>(PutType::Create);
            request.db_len = db_name.length();
            memcpy(&request.body[0], db_name.data(), db_name.length());

            PutResponse response;
            client.Put(request, &response);
            if (static_cast<State>(response.state) == State::kOK) {
                std::cout << "create db " << db_name << " sucessfully\n";
            } else {
                std::cout << "create db " << db_name << " failed\n";
            }
        } else if (operation == "set") {
            std::cin >> db_name;
            std::cin >> key;
            std::cin >> value;
            int length = sizeof(int8_t) + sizeof(int32_t) * 3
                    + db_name.length() + key.length() + value.length();
            IOBuf buffer(length);
            auto& request = *reinterpret_cast<PutRequest*>(buffer.data());
            request.put_type = static_cast<int8_t>(PutType::Update);
            request.db_len = db_name.length();
            request.key_len = key.length();
            request.value_len = value.length();
            char* ptr = &request.body[0];
            memcpy(ptr, db_name.data(), db_name.length());
            memcpy(ptr + db_name.length(), key.data(), key.length());
            memcpy(ptr + db_name.length() + key.length(), value.data(), value.length());

            PutResponse response;
            client.Put(request, &response);
            if (static_cast<State>(response.state) == State::kOK) {
                std::cout << "set sucessfully\n";
            } else {
                std::cout << "set failed\n";
            }
        } else if (operation == "get") {
            std::cin >> db_name;
            std::cin >> key;
            int length = sizeof(int32_t) * 2 + db_name.length() + key.length();
            IOBuf buffer(length);
            auto& request = *reinterpret_cast<GetRequest*>(buffer.data());
            request.db_len = db_name.length();
            request.key_len = key.length();
            char* ptr = &request.body[0];
            memcpy(ptr, db_name.data(), db_name.length());
            memcpy(ptr + db_name.length(), key.data(), key.length());

            GetResponse response;
            client.Get(request, &response);
            if (static_cast<State>(response.state) == State::kOK) {
                std::string_view key(response.key, response.key_len);
                std::string_view value(response.value, response.value_len);
                std::cout << key << " : " << value << std::endl;
                free(const_cast<char*>(response.key));
                free(const_cast<char*>(response.value));
            } else {
                std::cout << "Not Found\n";
            }
        } else if (operation == "del") {
            std::cin >> db_name;
            std::cin >> key;
            int length = sizeof(int8_t) + sizeof(int32_t) * 3
                    + db_name.length() + key.length();
            IOBuf buffer(length);
            auto& request = *reinterpret_cast<PutRequest*>(buffer.data());
            request.put_type = static_cast<int8_t>(PutType::Delete);
            request.db_len = db_name.length();
            request.key_len = key.length();
            char* ptr = &request.body[0];
            memcpy(ptr, db_name.data(), db_name.length());
            memcpy(ptr + db_name.length(), key.data(), key.length());

            PutResponse response;
            client.Put(request, &response);
            if (static_cast<State>(response.state) == State::kOK) {
                std::cout << "del sucessfully\n";
            } else {
                std::cout << "del failed\n";
            }
        }
    }

    return 0;
}
