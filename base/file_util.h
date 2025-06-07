#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <string_view>
#include <iostream>

#include "base/random.h"

namespace zydb::base {

namespace fs = std::filesystem;

class FileUtil {
public:
    static std::string GetParentPath(std::string_view filename) {
        fs::path path(filename);
        return path.parent_path().string();
    }

    static bool Exists(std::string_view filename) {
        return fs::exists(filename);
    }

    static bool CreateDirectory(std::string_view dirname) {
        return fs::create_directory(dirname);
    }

    static bool CreateFile(std::string_view filename) {
        if (Exists(filename)) return true;
        fs::path file_path(filename);
        CreateDirectory(file_path.parent_path().string());
        int fd = open(filename.data(), O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1) return false;
        close(fd);
        return true;
    }

    static void DeleteFile(std::string_view filename) {
        if (Exists(filename)) {
            fs::remove(filename);
        }
    }

    static int64_t GetFileSize(std::string_view filename) {
        if (!Exists(filename)) return -1;
        return fs::file_size(filename);
    }
};

class ScopeTempFile {
public:
    ScopeTempFile(bool create = true): create_(create) {}

    ~ScopeTempFile() {
        for (auto& filename : filenames_) {
            std::cout << "delete file: " << filename << std::endl;
            FileUtil::DeleteFile(filename);
        }
    }

    std::string_view GetName() const {
        filenames_.emplace_back("/tmp/" + random_.GetString(25));
        auto& filename = filenames_.back();
        if (create_) FileUtil::CreateFile(filename);
        return filename;
    }

private:
    bool create_;
    Random random_;
    mutable std::vector<std::string> filenames_;
};

} // namespace zydb::base
