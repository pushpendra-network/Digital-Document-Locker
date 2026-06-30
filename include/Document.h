#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "DocumentElement.h"

using namespace std;

class Document
{
private:
    string id;
    string title;
    string createdAt;
    string updatedAt;

    vector<unique_ptr<DocumentElement>> documentelements;

   public:

    Document() {}

    Document(string id, string title, string createdAt, string updatedAt)
    {
        this->id = id;
        this->title = title;
        this->createdAt = createdAt;
        this->updatedAt = updatedAt;
    }

    void addElement(unique_ptr<DocumentElement> element)
    {
        documentelements.push_back(move(element));
    }

    string render() const
    {
        string result;

        for (const auto& element : documentelements)
        {
            result += element->render();
        }

        return result;
    }

    string getid() const
    {
        return id;
    }

    string getTitle() const
    {
        return title;
    }

    string getcreatedAt() const
    {
        return createdAt;
    }

    string getupdatedAt() const
    {
        return updatedAt;
    }

    const vector<unique_ptr<DocumentElement>>& getElements() const
    {
        return documentelements;
    }

    void setId(string id)
    {
        this->id = id;
    }

    void setTitle(string title)
    {
        this->title = title;
    }

    void setCreatedAt(string createdAt)
    {
        this->createdAt = createdAt;
    }

    void setUpdatedAt(string updatedAt)
    {
        this->updatedAt = updatedAt;
    }
};

#endif