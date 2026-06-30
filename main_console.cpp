#include<iostream>
#include <memory>
#include<vector>
#include<string>
#include<fstream>

#include <crow.h>    
#include "json.hpp"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/options/replace.hpp>
using json = nlohmann::json;


using namespace std;

//Abstraction for document element

class DocumentElement
{
   public:

   virtual string render()=0;
   virtual string getType() =0;
   virtual string serialize() = 0;
   virtual ~DocumentElement() {}
};

//concerete implementation for text elements

class TextElement:  public DocumentElement
{
    private:
     string text;
     public:
     TextElement(string text)
     {
      this->text=text;
     }
     string render() override{
      return text;
     }
     string getType()override
     {
      return "text";
     }
      string getText()
    {
        return text;
    }
      string serialize() override
{
    return "{\"type\":\"text\",\"content\":\"" +
           text +
           "\"}";
}
};

//Concrete implementation for image elements

class ImageElement :public DocumentElement
{
   private:
   string imagepath;

   public: 
      ImageElement(string imagepath)
      {
         this->imagepath=imagepath;
      }

      string render()override{
        return "[Image : " +imagepath+"]";
      }
      string getType() override
      {
        return "image"; 
      }
      string getImagePath()
    {
        return imagepath;
    }
      string serialize() override
      {
         return "{\"type\":\"image\",\"path\":\"" +
               imagepath + "\"}";
      }
    
};
// NewLineElement represents a line break in the document.
class NewLineElement : public DocumentElement {
public:
    string render() override {
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

//Tabspace Element represents a tab space in the document
class TabSpaceElement : public DocumentElement {
public:
    string render() override {
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
class Document
{
private:
    string id;
    string title;
    string createdAt;
    string updatedAt;

    vector<std::unique_ptr<DocumentElement>> documentelements;

    public:

    // =========================
    // Constructor
    // =========================
    Document() {}

    Document(string id, string title, string createdAt, string updatedAt)
    {
        this->id = id;
        this->title = title;
        this->createdAt = createdAt;
        this->updatedAt = updatedAt;
    }

    // =========================
    // Add Element (OWNERSHIP TRANSFER)
    // =========================
    void addElement(std::unique_ptr<DocumentElement> element)
    {
        documentelements.push_back(std::move(element));
    }

    // =========================
    // FIXED RENDER FUNCTION
    // =========================
    string render() const
    {
        string result;

        for (const auto& element : documentelements)
        {
            result += element->render();
        }

        return result;
    }

    // =========================
    // GETTERS (CONST CORRECT)
    // =========================
    string getid() const { return id; }

    string getTitle() const { return title; }

    string getcreatedAt() const { return createdAt; }

    string getupdatedAt() const { return updatedAt; }

    const vector<std::unique_ptr<DocumentElement>>& getElements() const
    {
        return documentelements;
    }

    // =========================
    // SETTERS
    // =========================
    void setId(string id) { this->id = id; }
   
    void setTitle(string title) { this->title = title; }

    void setCreatedAt(string createdAt) { this->createdAt = createdAt; }


    void setUpdatedAt(string updatedAt) { this->updatedAt = updatedAt; }
};


class ElementFactory
{
public:
    static DocumentElement* create(const json& j)
    {
        string type = j["type"];

        if(type == "text")
        {
            return new TextElement(j.value("content", ""));
        }

        if(type == "image")
        {
            return new ImageElement(j.value("path", ""));
        }

        if(type == "newline")
        {
            return new NewLineElement();
        }

        if(type == "tab")
        {
            return new TabSpaceElement();
        }

        return nullptr;
    }
};

//serializing the content 


class DocumentSerializer
{
public:
    string serialize(Document* document)
    {
        string json = "{";

        json += "\n\t\"id\":\"" + document->getid() + "\",";
        json += "\n\t\"title\":\"" + document->getTitle() + "\",";
        json += "\n\t\"createdAt\":\"" + document->getcreatedAt() + "\",";
        json += "\n\t\"updatedAt\":\"" + document->getupdatedAt() + "\",";

        json += "\n\t\"elements\" : [";

        const vector<std::unique_ptr<DocumentElement>>& elements =
            document->getElements();

        for (size_t i = 0; i < elements.size(); i++)
        {
            json += "\n\t\t";

            // safe access with smart pointer
            json += elements[i]->serialize();

            if (i < elements.size() - 1)
            {
                json += ",";
            }
        }

        json += "\n\t]";
        json += "\n}";

        return json;
    }
};

//deserializing the content 
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

        if(j.contains("elements") && j["elements"].is_array())
        {
            for(const auto& elem : j["elements"])
            {
                DocumentElement* element = ElementFactory::create(elem);

                if(element != nullptr)
                {
                    document->addElement(
                        std::unique_ptr<DocumentElement>(element)
                    );
                }
            }
        }

        return document;
    }
};

//Persistence abstraction
class Persistance
{
   public:
   virtual void save( const string& data)=0;
   virtual string load(const string& id) = 0;
   virtual bool remove(const string& id) = 0; 
   virtual ~Persistance() =default;
  
};


//FileStorage implementatio of persistance 

class Filestorage:public Persistance
{
    void save (const string& data) override
    {
      ofstream outfile("document.txt");
      if(outfile)
      {
         outfile<<data;
         outfile.close();
         cout<<"Document saved to document .txt"<<endl;

      }
      else
      {
         cout<<"Error: Unable to open file for writing ."<<endl;
      }
    }
    string load(const string& id) override
    {
        return "";
    }
    bool remove(const string& id) override
    {
        cout << "Delete operation is not implemented for FileStorage." << endl;
        return false;
    }
};

//Placeholder DBStorage implementaion


  class DBstorage : public Persistance
{
   private:
   mongocxx::instance instance{};
   mongocxx::client client{
    mongocxx::uri{"mongodb://localhost:27017"}
   };
   public:
   DBstorage()
   {
    cout<<"MongoDB storage Initailized"<<endl;
   }
   void save(const string&data)override
   {
        try
        {
            auto db =client["TextEditorDB"];
            auto collection=db["documents"];
            auto bsonDoc=bsoncxx::from_json(data);
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
            if(result)
            {
                cout<<"Document saved to MongoDB"<<endl;
            }
        }
        catch (const exception&e)
        {
            cerr<<"MongoDB Error: "
            <<e.what()
            <<endl;
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
            cerr << "MongoDB Error: "
                    << e.what()
                    << endl;

            return "";
        }
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
            cerr << "MongoDB Error: "
                << e.what()
                << endl;

            return false;
        }
    }
};


class DocumentEditor
{
private:
    Document* document;
    Persistance* storage;
    DocumentSerializer serializer;

    public:
    DocumentEditor(Document* document, Persistance* storage)
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
        document->addElement(
            std::make_unique<TextElement>(text)
        );
    }

    void addImage(string path)
    {
        document->addElement(
            std::make_unique<ImageElement>(path)
        );
    }

    void addTabSpace()
    {
        document->addElement(
            std::make_unique<TabSpaceElement>()
        );
    }

    void addNewLine()
    {
        document->addElement(
            std::make_unique<NewLineElement>()
        );
    }

    string renderDocument()
    {
        return document->render();
    }

    void saveDocument()
    {
        string json = serializer.serialize(document);
        storage->save(json);
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
        cout << "Document ID : "
             << document->getid()
             << endl;

        cout << "Title : "
             << document->getTitle()
             << endl;

        cout << "Created At : "
             << document->getcreatedAt()
             << endl;

        cout << "Updated At : "
             << document->getupdatedAt()
             << endl;
    }
};


int main()
{
    Persistance* storage = new DBstorage();

    int choice;

    do
    {
        cout << "\n====================================\n";
        cout << "        TEXT EDITOR SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Create Document\n";
        cout << "2. Load Document\n";
        cout << "3. Update Document\n";
        cout << "4. Delete Document\n";
        cout << "5. Exit\n";
        cout << "====================================\n";
        cout << "Enter your choice : ";
        cin >> choice;

        switch(choice)
        {
            case 1:
            {
                cout << "\n========== CREATE DOCUMENT ==========\n";

                string id;
                string title;
                string createdAt;
                string updatedAt;

                cout << "Enter Document ID: ";
                cin >> id;

                cin.ignore();

                cout << "Enter Title: ";
                getline(cin, title);

                cout << "Enter Created Date: ";
                getline(cin, createdAt);

                cout << "Enter Updated Date: ";
                getline(cin, updatedAt);

                Document* document = new Document(
                    id,
                    title,
                    createdAt,
                    updatedAt
                );

                DocumentEditor* editor = new DocumentEditor(document, storage);

                cout << "\nDocument Created Successfully.\n";

                int option;

                do
                {
                    cout << "\n========== DOCUMENT EDITOR ==========\n";
                    cout << "1. Add Text\n";
                    cout << "2. Add New Line\n";
                    cout << "3. Add Tab Space\n";
                    cout << "4. Add Image\n";
                    cout << "5. Preview Document\n";
                    cout << "6. Save Document\n";
                    cout << "7. Finish Editing\n";
                    cout << "Choose Option: ";

                    cin >> option;
                    cin.ignore();

                    switch(option)
                    {
                        case 1:
                        {
                            string text;

                            cout << "Enter Text: ";
                            getline(cin, text);

                            editor->addText(text);

                            cout << "Text Added Successfully.\n";
                            break;
                        }

                        case 2:
                        {
                            editor->addNewLine();

                            cout << "New Line Added.\n";
                            break;
                        }

                        case 3:
                        {
                            editor->addTabSpace();

                            cout << "Tab Added.\n";
                            break;
                        }

                        case 4:
                        {
                            string path;

                            cout << "Enter Image Path: ";
                            getline(cin, path);

                            editor->addImage(path);

                            cout << "Image Added.\n";
                            break;
                        }

                        case 5:
                        {
                            cout << "\n========== DOCUMENT ==========\n";

                            editor->showDocumentInfo();

                            cout << "\nRendered Output:\n";
                            cout << editor->renderDocument() << endl;

                            break;
                        }

                        case 6:
                        {
                            editor->saveDocument();

                            cout << "Document Saved Successfully.\n";
                            break;
                        }

                        case 7:
                        {
                            cout << "Finished Editing.\n";
                            break;
                        }

                        default:
                        {
                            cout << "Invalid Choice.\n";
                        }
                    }

                } while(option != 7);

                delete editor;
                delete document;

                break;
            }

            case 2:
            {
                
                cout << "\n========== LOAD DOCUMENT ==========\n";

                string id;

                cout << "Enter Document ID: ";
                cin >> id;

                // Temporary editor (no document yet)
                DocumentEditor* editor =
                    new DocumentEditor(nullptr, storage);

                Document* document = editor->loadDocument(id);

                if(document != nullptr)
                {
                    editor->setDocument(document);

                    cout << "\nDocument Loaded Successfully.\n\n";

                    editor->showDocumentInfo();

                    cout << "\nRendered Document:\n";
                    cout << editor->renderDocument() << endl;

                    delete document;
                }

                delete editor;


                break;
            }

            case 3:
            {
                
                 cout << "\n========== UPDATE DOCUMENT ==========\n";

                string id;
                cout << "Enter Document ID: ";
                cin >> id;

                // Temporary editor to load the document
                DocumentEditor loader(nullptr, storage);

                Document* document = loader.loadDocument(id);

                if (document == nullptr)
                {
                    cout << "Document not found.\n";
                    break;
                }

                // Create editor for the loaded document
                DocumentEditor editor(document, storage);

                cout << "\nCurrent Document:\n";
                editor.showDocumentInfo();

                cout << "\nContent:\n";
                cout << editor.renderDocument() << endl;

                int option;

                do
                {
                    cout << "\n========== UPDATE MENU ==========\n";
                    cout << "1. Add Text\n";
                    cout << "2. Add New Line\n";
                    cout << "3. Add Tab Space\n";
                    cout << "4. Add Image\n";
                    cout << "5. Preview Document\n";
                    cout << "6. Save Changes\n";
                    cout << "7. Finish\n";
                    cout << "Choose Option: ";

                    cin >> option;
                    cin.ignore();

                    switch(option)
                    {
                        case 1:
                        {
                            string text;
                            cout << "Enter Text: ";
                            getline(cin, text);
                            editor.addText(text);
                            cout << "Text Added.\n";
                            break;
                        }

                        case 2:
                        {
                            editor.addNewLine();
                            cout << "New Line Added.\n";
                            break;
                        }

                        case 3:
                        {
                            editor.addTabSpace();
                            cout << "Tab Added.\n";
                            break;
                        }

                        case 4:
                        {
                            string path;
                            cout << "Enter Image Path: ";
                            getline(cin, path);
                            editor.addImage(path);
                            cout << "Image Added.\n";
                            break;
                        }

                        case 5:
                        {
                            cout << "\nCurrent Document:\n";
                            cout << editor.renderDocument() << endl;
                            break;
                        }

                        case 6:
                        {
                            editor.saveDocument();
                            cout << "Document Updated Successfully.\n";
                            break;
                        }

                        case 7:
                        {
                            cout << "Finished Updating.\n";
                            break;
                        }

                        default:
                        {
                            cout << "Invalid Choice.\n";
                        }
                    }

                } while(option != 7);

                delete document;

                break;
            }

           case 4:
            {
                cout << "\n========== DELETE DOCUMENT ==========\n";

                string id;

                cout << "Enter Document ID: ";
                cin >> id;

                DocumentEditor editor(nullptr, storage);

                char confirm;

                cout << "Are you sure you want to delete this document? (Y/N): ";
                cin >> confirm;

                if(confirm == 'Y' || confirm == 'y')
                {
                    if(editor.deleteDocument(id))
                    {
                        cout << "\nDocument Deleted Successfully.\n";
                    }
                    else
                    {
                        cout << "\nDocument Not Found.\n";
                    }
                }
                else
                {
                    cout << "\nDelete Cancelled.\n";
                }

                break;
            }
            case 5:
            {
                cout << "\nThank you for using Text Editor.\n";
                break;
            }

            default:
            {
                cout << "\nInvalid Choice!\n";
            }
        }

    } while(choice != 5);

    delete storage;

    return 0;
}