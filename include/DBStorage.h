#ifndef DBSTORAGE_H
#define DBSTORAGE_H

#include <iostream>
#include <string>
#include <exception>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/options/replace.hpp>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "Persistence.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class DBStorage : public Persistence
{
private:
    mongocxx::instance instance{};

    mongocxx::client client{
        mongocxx::uri{
            "mongodb+srv://push2005st_db_user:6bVpyU1Dq1VaENUy@cluster0.4psf4sm.mongodb.net/TextEditorDB?retryWrites=true&w=majority&appName=Cluster0"
        }
    };

public:
    DBStorage()
    {
        cout << "MongoDB Storage Initialized" << endl;
    }

    void save(const string& data) override
    {
        try
        {
            auto db = client["TextEditorDB"];
            auto collection = db["documents"];

            auto bsonDoc = bsoncxx::from_json(data);

            json j = json::parse(data);
            string id = j["id"];

            auto filter =
                bsoncxx::builder::stream::document{}
                << "id" << id
                << bsoncxx::builder::stream::finalize;

            mongocxx::options::replace options;
            options.upsert(true);

            auto result = collection.replace_one(
                filter.view(),
                bsonDoc.view(),
                options
            );

            if (result)
            {
                cout << "Document saved successfully." << endl;
            }
        }
        catch (const exception& e)
        {
            cerr << "MongoDB Error: " << e.what() << endl;
        }
    }

    string load(const string& id) override
    {
        try
        {
            auto db = client["TextEditorDB"];
            auto collection = db["documents"];

            auto result = collection.find_one(
                bsoncxx::builder::stream::document{}
                    << "id" << id
                    << bsoncxx::builder::stream::finalize
            );

            if (result)
            {
                return bsoncxx::to_json(result->view());
            }

            return "";
        }
        catch (const exception& e)
        {
            cerr << "MongoDB Error: " << e.what() << endl;
            return "";
        }
    }

    bool update(const string& id, const string& data) override
    {
        save(data);
        return true;
    }

    bool remove(const string& id) override
    {
        try
        {
            auto db = client["TextEditorDB"];
            auto collection = db["documents"];

            auto result = collection.delete_one(
                bsoncxx::builder::stream::document{}
                    << "id" << id
                    << bsoncxx::builder::stream::finalize
            );

            if (result && result->deleted_count() > 0)
            {
                cout << "Document deleted successfully." << endl;
                return true;
            }

            cout << "Document not found." << endl;
            return false;
        }
        catch (const exception& e)
        {
            cerr << "MongoDB Error: " << e.what() << endl;
            return false;
        }
    }
};

#endif