#include "User.hpp"

User::User(u128 ID){
    this->ID = ID;

    // Get username and icon
    this->username = "";
    this->image = nullptr;
    AccountProfile profile;
    AccountProfileBase profile_base;

    Result rc = accountGetProfile(&profile, this->ID);
    if (R_SUCCEEDED(rc)){
        // Get username
        rc = accountProfileGet(&profile, NULL, &profile_base);
        if (R_SUCCEEDED(rc)){
            this->username = profile_base.username;
        }

        // Get icon and render to texture
        u8 * buffer;
        size_t img_size, tmp;
        // Get image size and allocate memory
        rc = accountProfileGetImageSize(&profile, &img_size);
        if (R_SUCCEEDED(rc)) {
            buffer = (u8 *) malloc(img_size);

            // Get image
            rc = accountProfileLoadImage(&profile, buffer, img_size, &tmp);
            if (R_SUCCEEDED(rc)) {
                this->image = SDLHelper::renderImage(buffer, img_size);
            }

            free(buffer);
        }

        // Close profile
        accountProfileClose(&profile);
    }
}

u128 User::getID() {
    return this->ID;
}

std::string User::getUsername() {
    return this->username;
}


SDL_Texture * User::getImage() {
    return this->image;
}

User::~User() {
    // Free memory used by image texture
    SDLHelper::destroyTexture(this->image);
}