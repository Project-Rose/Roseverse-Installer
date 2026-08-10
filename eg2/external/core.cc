#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "eg2/libs/typedefs.h"
#include "eg2/external/core.hh"
#include "eg2/heap/mem2.hh"

namespace IO {
    static const char* SD_PATH = "fs:/vol/external01/";

    static void CreateDirectories(const std::string& path) {
        std::string current;

        for (char c : path) {
            current += c;
            if (c == '/') mkdir(current.c_str(), 0777);
        }
    }

    void* ReadFile(const char* path, FileMode mode, u32* outSize) {
        const char* modeStr = (mode == FILEMODE_BIN) ? "rb" : "r";

        FILE* file = fopen(path, modeStr);
        if (!file) return nullptr;

        if (fseek(file, 0, SEEK_END) != 0) {
            fclose(file);
            return nullptr;
        }

        long fileSize = ftell(file);
        if (fileSize < 0) {
            fclose(file);
            return nullptr;
        }
        rewind(file);

        void* data = Heap::MEM2::Alloc((u32)fileSize, MEM_DEFAULT_ALIGN);
        if (!data) {
            fclose(file);
            return nullptr;
        }

        if (fread(data, 1, (usize)fileSize, file) != (usize)fileSize) {
            Heap::MEM2::Free(data);
            fclose(file);
            return nullptr;
        }

        fclose(file);

        if (outSize) *outSize = (u32)fileSize;
        return data;
    }

    bool WriteFile(const char* path, const void* data, usize size, FileMode mode) {
        const char* modeStr = (mode == FILEMODE_BIN) ? "wb" : "w";

        std::string pathStr = path;

        if (pathStr.find(SD_PATH) == 0) {
            usize lastSlash = pathStr.find_last_of('/');

            if (lastSlash != std::string::npos) {
                std::string folderPath = pathStr.substr(0, lastSlash + 1);
                CreateDirectories(folderPath);
            }
        }

        FILE* file = fopen(path, modeStr);

        if (!file) return false;

        usize written = fwrite(data, 1, size, file);

        fclose(file);

        return (written == size);
    }

    std::vector<std::string> ListDirs(const std::string& path) {
        std::vector<std::string> dirs;

        DIR* dir = opendir(path.c_str());

        if (!dir) return dirs;

        struct dirent* entry;

        while ((entry = readdir(dir)) != nullptr) {

            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

            std::string fullPath = path + "/" + entry->d_name;

            struct stat st;

            if (stat(fullPath.c_str(), &st) != 0) continue;
            if (S_ISDIR(st.st_mode)) dirs.push_back(entry->d_name);
        }

        closedir(dir);

        return dirs;
    }
}