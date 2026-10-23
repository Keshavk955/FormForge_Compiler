#include "generator.h"

string escapeHTML(const string& text)
{
    string result;

    for(char c : text)
    {
        switch(c)
        {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += c;
        }
    }

    return result;
}

using namespace std;

bool CodeGenerator::generate(FormIR& ir)
{
    string fileName = ir.formName + ".html";

    ofstream file(fileName);

    if(!file)
    {
        return false;
    }

    file << "<!DOCTYPE html>\n";
    file << "<html>\n";
    file << "<head>\n";
    file << "<meta charset=\"UTF-8\">\n";
    file << "<title>" << ir.formName << "</title>\n";

    file << "<style>\n";
    file << "body { font-family: Arial, sans-serif; margin: 40px; }\n";
    file << "form { width: 400px; }\n";
    file << "label { display: block; margin-top: 15px; font-weight: bold; }\n";
    file << "input { width: 100%; padding: 8px; margin-top: 5px; box-sizing: border-box; }\n";
    file << "button { margin-top: 20px; padding: 10px 20px; }\n";
    file << "</style>\n";

    file << "</head>\n";
    file << "<body>\n";

    file << "<h1>" << ir.formName << "</h1>\n";

    file << "<form>\n";

    for(IRField field : ir.fields)
    {
        file << "<label for=\"" << field.name << "\">";
        file << field.name;
        file << "</label>\n";

        file << "<input ";

        file << "id=\"" << field.name << "\" ";
        file << "name=\"" << field.name << "\" ";

        if(field.type == "string")
        {
            file << "type=\"text\" ";
        }
        else if(field.type == "integer")
        {
            file << "type=\"number\" ";
        }
        else if(field.type == "email")
        {
            file << "type=\"email\" ";
        }

        if(field.required)
        {
            file << "required ";
        }

        if(field.hasMin)
        {
            file << "min=\"" << field.minValue << "\" ";
        }

        if(field.hasMax)
        {
            file << "max=\"" << field.maxValue << "\" ";
        }

        file << ">\n";
    }

    file << "<button type=\"submit\">";
    file << escapeHTML(ir.submitText);
    file << "</button>\n";

    file << "</form>\n";

    file << "</body>\n";
    file << "</html>\n";

    file.close();

    return true;
}