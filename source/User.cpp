#include "User.hpp"

User::User(u128 ID, std::string name, SDL_Texture * tex){
    this->ID = ID;
    this->username = name;
    this->image = tex;
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
    SDL_DestroyTexture(this->image);
}