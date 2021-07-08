#include <cstring>
#include <fstream>
#include "nx/Title.hpp"

namespace NX {
    Title::Title(TitleID titleID, bool installed) {
        this->titleID_ = titleID;
        this->is_installed = installed;
        this->name_ = "";
        this->ptr = nullptr;

        // Get name and icon
        NsApplicationControlData data;
        NacpLanguageEntry * lang = nullptr;
        size_t nacp_size;
        Result rc = nsGetApplicationControlData(NsApplicationControlSource_Storage, this->titleID_, &data, sizeof(NsApplicationControlData), &nacp_size);
        if (R_SUCCEEDED(rc)){
            // Get name
            rc = nacpGetLanguageEntry(&data.nacp, &lang);
            if (R_SUCCEEDED(rc)){
                this->name_ = std::string(lang->name);
            }

            // Get icon
            this->size = nacp_size - sizeof(data.nacp);
            this->ptr = (u8 *) malloc(this->size);
            memcpy(this->ptr, data.icon, this->size);
        }
    }

    Title::Title(const TitleID titleID, const std::string & name) {
        this->titleID_ = titleID;
        this->is_installed = false;
        this->name_ = name;

        // Load in image
        std::ifstream file("romfs:/icon/no_icon.jpg", std::ios::binary);
        file.unsetf(std::ios::skipws);
        file.seekg(0, std::ios::end);
        this->size = file.tellg();
        file.seekg(0, std::ios::beg);

        this->ptr = static_cast<u8 *>(malloc(this->size));
        u8 byte;
        size_t pos = 0;
        while (file >> byte && pos < this->size) {
            this->ptr[pos] = byte;
            pos++;
        }
    }

    TitleID Title::titleID() {
        return this->titleID_;
    }

    bool Title::isInstalled() {
        return this->is_installed;
    }

    std::string Title::name() {
        return this->name_;
    }

    u8 * Title::imgPtr() {
        return this->ptr;
    }

    u32 Title::imgSize() {
        return this->size;
    }

    Title::~Title() {
        if (this->ptr != nullptr) {
            free(this->ptr);
        }
    }
};
