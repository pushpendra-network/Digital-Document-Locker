#ifndef DOCUMENTELEMENT_H
#define DOCUMENTELEMENT_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// Base class
class DocumentElement
{
public:
    virtual string render() = 0;
    virtual string getType() = 0;
    virtual string serialize() = 0;
    virtual ~DocumentElement() = default;
};

// Text Element
class TextElement : public DocumentElement
{
private:
    string text;

public:
    TextElement(string text)
    {
        this->text = text;
    }

    string render() override
    {
        return text;
    }

    string getType() override
    {
        return "text";
    }

    string getText() const
    {
        return text;
    }

    string serialize() override
    {
        return "{\"type\":\"text\",\"content\":\"" + text + "\"}";
    }
};

// Image Element
class ImageElement : public DocumentElement
{
private:
    string imagepath;

public:
    ImageElement(string imagepath)
    {
        this->imagepath = imagepath;
    }

    string render() override
    {
        return "[Image : " + imagepath + "]";
    }

    string getType() override
    {
        return "image";
    }

    string getImagePath() const
    {
        return imagepath;
    }

    string serialize() override
    {
        return "{\"type\":\"image\",\"path\":\"" + imagepath + "\"}";
    }
};

// New Line Element
class NewLineElement : public DocumentElement
{
public:
    string render() override
    {
        return "\n";
    }

    string getType() override
    {
        return "newline";
    }

    string serialize() override
    {
        return "{\"type\":\"newline\"}";
    }
};

// Tab Space Element
class TabSpaceElement : public DocumentElement
{
public:
    string render() override
    {
        return "\t";
    }

    string getType() override
    {
        return "tab";
    }

    string serialize() override
    {
        return "{\"type\":\"tab\"}";
    }
};

#endif