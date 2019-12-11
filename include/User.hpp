#ifndef USER_H
#define USER_H

#include "SDLHelper.hpp"
#include <string>
#include <switch.h>

// The User class stores all relevant/useful information about a user
// on the console. All this is read using the provided userID
class User {
    private:
        // The selected user's ID (set to zero when an error occurs)
        AccountUid ID;
        // The user's username
        std::string username;
        // The user's profile image
        SDL_Texture * image;

    public:
        // Constructor takes ID and derives username and icon texture
        User(AccountUid);

        // Returns private members
        AccountUid getID();
        std::string getUsername();
        SDL_Texture * getImage();

        // Destructor frees texture if necessary
        ~User();
};

#endif