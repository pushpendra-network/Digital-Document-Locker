#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "Persistence.h"

using namespace std;

class Filestorage : public Persistence
{
public:

    void save(const string& data) override
    {
        ofstream outfile("document.txt");

        if(outfile)
        {
            outfile << data;
            outfile.close();

            cout << "Document saved to document.txt" << endl;
        }
        else
        {
            cout << "Error: Unable to open file." << endl;
        }
    }

    string load(const string& id) override
    {
        return "";
    }

    bool remove(const string& id) override
    {
        cout << "Delete operation is not implemented for FileStorage."
             << endl;

        return false;
    }
};

#endif