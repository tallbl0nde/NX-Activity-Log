#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <switch.h>

namespace NX {
    // The User class stores all relevant/useful information about a user
    // on the console. All this is read using the provided userID
    class User {
        private:
            // The selected user's ID (set to zero when an error occurs)
            AccountUid ID_;
            // The user's username
            std::string username_;
            // The user's profile image (pointer to and size)
            u8 * ptr;
            u32 size;

        public:
            // Constructor takes ID and derives username and icon texture
            User(AccountUid);

            // Returns private members
            AccountUid ID();
            std::string username();
            u8 * imgPtr();
            u32 imgSize();

            // Destructor frees memory containing image
            ~User();
    };
};

#endif