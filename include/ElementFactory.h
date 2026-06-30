#ifndef ELEMENTFACTORY_H
#define ELEMENTFACTORY_H


#include <string>
#include <vector>
#include <memory>
#include "json.hpp"
#include "DocumentElement.h"

using json = nlohmann::json;
using namespace std;

class ElementFactory
{
public:
    static DocumentElement* create(const json& j)
    {
        if (!j.contains("type"))
        {
            return nullptr;
        }

        string type = j["type"];

        if (type == "text")
        {
            return new TextElement(j.value("content", ""));
        }

        if (type == "image")
        {
            return new ImageElement(j.value("path", ""));
        }

        if (type == "newline")
        {
            return new NewLineElement();
        }

        if (type == "tab")
        {
            return new TabSpaceElement();
        }

        return nullptr;
    }
};

#endif