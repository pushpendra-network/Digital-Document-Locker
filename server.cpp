#ifdef DELETE
#undef DELETE
#endif
#include <cstdlib>
#include "include/AuthStorage.h"
#include "include/json.hpp"
#include <crow.h>
#include <crow/middlewares/cors.h>

#include "include/DBStorage.h"
#include "include/DocumentEditor.h"
#include "include/DocumentSerializer.h"
#include "include/DocumentDeserializer.h"

int main()
{
    // =========================
    // CREATE APP
    // =========================

    crow::App<crow::CORSHandler> app;

    auto& cors = app.get_middleware<crow::CORSHandler>();

    cors.global()
        .headers("Content-Type")
        .methods(
            "GET"_method,
            "POST"_method,
            "PUT"_method,
            "DELETE"_method,
            "OPTIONS"_method)
        .origin("*");

    DBStorage storage;
    DocumentSerializer serializer;
    AuthStorage authStorage;

    // =========================
    // HOME
    // =========================

    CROW_ROUTE(app, "/")
    ([]()
    {
        return crow::response("Text Editor REST API");
    });

    // =========================
    // GET DOCUMENT
    // =========================

    CROW_ROUTE(app, "/document/<string>")
    ([&](std::string id)
    {
        DocumentEditor editor(nullptr, &storage);

        Document* doc = editor.loadDocument(id);

        if (!doc)
            return crow::response(404, "Document not found");

        std::string json = serializer.serialize(doc);

        delete doc;

        return crow::response(200, json);
    });

    // =========================
    // CREATE DOCUMENT
    // =========================

    CROW_ROUTE(app, "/document")
        .methods(crow::HTTPMethod::POST)
    ([&](const crow::request& req)
    {
        try
        {
            DocumentDeserializer deserializer;

            Document* doc = deserializer.deserialize(req.body);

            cout << "\n===== RECEIVED DOCUMENT =====\n";
            cout << "ID: " << doc->getid() << endl;
            cout << "Title: " << doc->getTitle() << endl;
            cout << "Created At: " << doc->getcreatedAt() << endl;
            cout << "Updated At: " << doc->getupdatedAt() << endl;
            cout << "=============================\n";

            DocumentEditor editor(doc, &storage);

            editor.saveDocument();

            delete doc;

            return crow::response(201,
                "Document created successfully.");
        }
        catch (const std::exception& e)
        {
            return crow::response(400, e.what());
        }
    });

    // =========================
    // UPDATE DOCUMENT
    // =========================

    CROW_ROUTE(app, "/document/<string>")
        .methods(crow::HTTPMethod::PUT)
    ([&](const crow::request& req, std::string id)
    {
        try
        {
            DocumentDeserializer deserializer;

            Document* doc = deserializer.deserialize(req.body);

            DocumentEditor editor(doc, &storage);

            bool ok = editor.updateDocument(id);

            delete doc;

            if (ok)
                return crow::response(200,
                    "Document updated successfully.");

            return crow::response(404,
                "Document not found");
        }
        catch (const std::exception& e)
        {
            return crow::response(400, e.what());
        }
    });

    // =========================
    // DELETE DOCUMENT
    // =========================

    CROW_ROUTE(app, "/document/<string>")
        .methods(crow::HTTPMethod::Delete)
    ([&](std::string id)
    {
        DocumentEditor editor(nullptr, &storage);

        if (editor.deleteDocument(id))
        {
            return crow::response(
                200,
                "Document deleted successfully.");
        }

        return crow::response(
            404,
            "Document not found.");
    });

    // =========================
    // TEST PUT
    // =========================

    CROW_ROUTE(app, "/test")
        .methods(crow::HTTPMethod::PUT)
    ([]()
    {
        return crow::response("PUT Works!");
    });

    // =========================
    // TEST DELETE
    // =========================

    CROW_ROUTE(app, "/delete-test")
        .methods(crow::HTTPMethod::Delete)
    ([]()
    {
        return crow::response("DELETE Works!");
    });

    // =========================
    // START SERVER
    // =========================
    CROW_ROUTE(app, "/login")
        .methods(crow::HTTPMethod::POST)
    ([&](const crow::request& req)
    {
        try
        {
            json body = json::parse(req.body);

            string email = body["email"];
            string password = body["password"];

            bool loggedIn = authStorage.loginUser(email, password);

            if (loggedIn)
            {
                return crow::response(200, "Login successful");
            }

            return crow::response(401, "Invalid email or password");
        }
        catch (const std::exception& e)
        {
            return crow::response(400, e.what());
        }
    });
    CROW_ROUTE(app, "/register")
        .methods(crow::HTTPMethod::POST)
    ([&](const crow::request& req)
    {
        try
        {
            json body = json::parse(req.body);

            string name = body["name"];
            string email = body["email"];
            string password = body["password"];

            User user(name, email, password);

            bool registered = authStorage.registerUser(user);

            if (registered)
            {
                return crow::response(201, "User registered successfully");
            }

            return crow::response(409, "User already exists");
        }
        catch (const std::exception& e)
        {
            return crow::response(400, e.what());
        }
    });
       int port = 18080;

        if (const char* envPort = std::getenv("PORT"))
        {
            port = std::stoi(envPort);
        }

        app.port(port)
            .bindaddr("0.0.0.0")
            .multithreaded()
            .run();

    return 0;
}