#ifndef PERSISTANCE_H
#define PERSISTANCE_H

#include <string>

using namespace std;

class Persistence
{
public:
    virtual void save(const string& data) = 0;
    virtual string load(const string& id) = 0;
    virtual bool remove(const string& id) = 0;
      virtual bool update(const string& id, const string& data) = 0;
    virtual ~Persistence() = default;
};

#endif