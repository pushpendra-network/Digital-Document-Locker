#ifndef DOCUMENTDESERIALIZER_H
#define DOCUMENTDESERIALIZER_H

#include <memory>
#include <string>

#include "json.hpp"
#include "Document.h"
#include "ElementFactory.h"

using json = nlohmann::json;
using namespace std;

class DocumentDeserializer
{
public:

    Document* deserialize(const string& jsonString)
    {
        json j = json::parse(jsonString);

        Document* document = new Document();

        document->setId(j.value("id", ""));
        document->setTitle(j.value("title", ""));
        document->setCreatedAt(j.value("createdAt", ""));
        document->setUpdatedAt(j.value("updatedAt", ""));

        if (j.contains("elements") && j["elements"].is_array())
        {
            for (const auto& elem : j["elements"])
            {
                DocumentElement* element = ElementFactory::create(elem);

                if (element != nullptr)
                {
                    document->addElement(
                        unique_ptr<DocumentElement>(element)
                    );
                }
            }
        }

        return document;
    }
};

#endif