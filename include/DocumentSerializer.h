#ifndef DOCUMENTSERIALIZER_H
#define DOCUMENTSERIALIZER_H

#include <string>
#include <vector>
#include <memory>
#include "Document.h"

using namespace std;

class DocumentSerializer
{
public:

    string serialize(const Document* document)
    {
        string json = "{";

        json += "\n\t\"id\":\"" + document->getid() + "\",";
        json += "\n\t\"title\":\"" + document->getTitle() + "\",";
        json += "\n\t\"createdAt\":\"" + document->getcreatedAt() + "\",";
        json += "\n\t\"updatedAt\":\"" + document->getupdatedAt() + "\",";

        json += "\n\t\"elements\" : [";

        const vector<unique_ptr<DocumentElement>>& elements =
            document->getElements();

        for (size_t i = 0; i < elements.size(); i++)
        {
            json += "\n\t\t";
            json += elements[i]->serialize();

            if (i != elements.size() - 1)
            {
                json += ",";
            }
        }

        json += "\n\t]";
        json += "\n}";

        return json;
    }
};

#endif