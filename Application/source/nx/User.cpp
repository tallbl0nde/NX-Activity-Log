#include "nx/User.hpp"

namespace NX {
    User::User(AccountUid ID){
        this->ID_ = ID;

        // Set values in case of an error
        this->username_ = "";
        this->ptr = nullptr;
        this->size = 0;

        // Get name and icon
        AccountProfile profile;
        AccountProfileBase profile_base;

        Result rc = accountGetProfile(&profile, this->ID_);
        if (R_SUCCEEDED(rc)){
            // Get username
            rc = accountProfileGet(&profile, NULL, &profile_base);
            if (R_SUCCEEDED(rc)){
                this->username_ = profile_base.nickname;
            }

            u32 tmp;
            // Get image size and allocate memory
            rc = accountProfileGetImageSize(&profile, &this->size);
            if (R_SUCCEEDED(rc)) {
                this->ptr = (u8 *) malloc(this->size);

                // Get image
                rc = accountProfileLoadImage(&profile, this->ptr, this->size, &tmp);
            }

            // Close profile
            accountProfileClose(&profile);
        }
    }

    AccountUid User::ID() {
        return this->ID_;
    }

    std::string User::username() {
        return this->username_;
    }

    u8 * User::imgPtr() {
        return this->ptr;
    }

    u32 User::imgSize() {
        return this->size;
    }

    User::~User() {
        // Free memory used by image texture
        if (this->ptr != nullptr) {
            free(this->ptr);
        }
    }
};