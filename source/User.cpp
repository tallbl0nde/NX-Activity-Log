#include "User.h"

User::User(u128 ID, char * username){
    this->ID = ID;

    // Copy the username string
    std::string s(username);
    this->username = s;
}

u128 User::getID(){
    return ID;
}

std::string User::getUsername(){
    return username;
}