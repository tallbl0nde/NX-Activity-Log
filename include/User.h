#ifndef USER_H
#define USER_H

#include <string>
#include <switch.h>

class User{
    private:
        u128 ID;
        std::string username;
    public:
        User(u128, char *);

        // Return User's data
        u128 getID();
        std::string getUsername();
};

#endif