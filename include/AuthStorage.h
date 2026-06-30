#ifndef AUTHSTORAGE_H
#define AUTHSTORAGE_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "User.h"

using namespace std;

class AuthStorage
{
private:

    mongocxx::client client;

    static string getMongoUri()
    {
        const char* mongoUri = std::getenv("MONGO_URI");

        if (!mongoUri)
        {
            throw runtime_error("MONGO_URI environment variable not set.");
        }

        return string(mongoUri);
    }

public:

    AuthStorage()
        : client(mongocxx::uri{getMongoUri()})
    {
        cout << "Auth Storage Initialized" << endl;
    }

    bool registerUser(const User& user)
    {
        try
        {
            auto db = client["TextEditorDB"];
            auto collection = db["users"];

            auto existingUser =
                collection.find_one(
                    bsoncxx::builder::stream::document{}
                    << "email" << user.getEmail()
                    << bsoncxx::builder::stream::finalize
                );

            if (existingUser)
            {
                cout << "User already exists." << endl;
                return false;
            }

            auto document =
                bsoncxx::builder::stream::document{}
                << "name" << user.getName()
                << "email" << user.getEmail()
                << "password" << user.getPassword()
                << bsoncxx::builder::stream::finalize;

            collection.insert_one(document.view());

            cout << "User registered successfully." << endl;

            return true;
        }
        catch (const exception& e)
        {
            cerr << "Register Error: " << e.what() << endl;
            return false;
        }
    }

    bool loginUser(const string& email,
                   const string& password)
    {
        try
        {
            auto db = client["TextEditorDB"];
            auto collection = db["users"];

            auto result =
                collection.find_one(
                    bsoncxx::builder::stream::document{}
                    << "email" << email
                    << "password" << password
                    << bsoncxx::builder::stream::finalize
                );

            if (result)
            {
                cout << "Login successful." << endl;
                return true;
            }

            cout << "Invalid email or password." << endl;

            return false;
        }
        catch (const exception& e)
        {
            cerr << "Login Error: " << e.what() << endl;
            return false;
        }
    }
};

#endif