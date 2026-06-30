#include <iostream>
#include <string>
#include <memory>

#include <crow.h>
#include "json.hpp"

#include "include/Document.h"
#include "include/DocumentElement.h"
#include "include/ElementFactory.h"
#include "include/DocumentSerializer.h"
#include "include/DocumentDeserializer.h"
#include "include/Persistence.h"
#include "include/FileStorage.h"
#include "include/DBStorage.h"
#include "include/DocumentEditor.h"

using namespace std;
using json = nlohmann::json;
int main()
{
    Persistence* storage = new DBStorage();

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