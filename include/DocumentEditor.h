#ifndef DOCUMENTEDITOR_H
#define DOCUMENTEDITOR_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "Document.h"
#include "Persistence.h"
#include "DocumentSerializer.h"
#include "DocumentDeserializer.h"
#include "DocumentElement.h"

using namespace std;

class DocumentEditor
{
private:
    Document* document;
    Persistence* storage;
    DocumentSerializer serializer;

public:
    DocumentEditor(Document* document, Persistence* storage)
    {
        this->document = document;
        this->storage = storage;
    }

    void setDocument(Document* document)
    {
        this->document = document;
    }

    void addText(string text)
    {
        document->addElement(make_unique<TextElement>(text));
    }

    void addImage(string path)
    {
        document->addElement(make_unique<ImageElement>(path));
    }

    void addTabSpace()
    {
        document->addElement(make_unique<TabSpaceElement>());
    }

    void addNewLine()
    {
        document->addElement(make_unique<NewLineElement>());
    }

    string renderDocument() const
    {
        if (document == nullptr)
        return "";
        return document->render();
    }

    void saveDocument()
    {
        if (document == nullptr)
        return;

        string json = serializer.serialize(document);
         cout << "\n===== JSON SENT TO MONGODB =====\n";
            cout << json << endl;
            cout << "================================\n";
        storage->save(json);
    }
    bool updateDocument(const string& id)
    {
        if (document == nullptr)
            return false;

        string json = serializer.serialize(document);

        return storage->update(id, json);
    }

    Document* loadDocument(const string& id)
    {
        string jsonData = storage->load(id);

        if (jsonData.empty())
        {
            cout << "Document not found." << endl;
            return nullptr;
        }

        DocumentDeserializer deserializer;
        return deserializer.deserialize(jsonData);
    }

    bool deleteDocument(const string& id)
    {
        return storage->remove(id);
    }

    void showDocumentInfo()
    {
         if (document == nullptr)
        {
            cout << "No document loaded." << endl;
            return;
        }
        cout << "Document ID : " << document->getid() << endl;
        cout << "Title : " << document->getTitle() << endl;
        cout << "Created At : " << document->getcreatedAt() << endl;
        cout << "Updated At : " << document->getupdatedAt() << endl;
    }
};

#endif