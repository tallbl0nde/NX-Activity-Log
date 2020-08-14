#include <cstring>
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