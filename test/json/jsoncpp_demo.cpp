#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <json/value.h>
#include <json/json.h>
using namespace std;
 
int main() {
    Json::Reader reader;  //for reading the data
    Json::Value root; //for modifying and storing new values
    Json::StyledStreamWriter writer; //for writing in json files
    ofstream newFile;
 
    //opening file using fstream
    ifstream file("sample.json");
 
    // check if there is any error is getting data from the json file
        if (!reader.parse(file, root)) {
        cout << reader.getFormattedErrorMessages();
        exit(1);
        }
       // cout << root << endl; // show the contain of file
        //Updating the json data
        cout << root["Input"][0]["type"]["SDI"]["active"] << endl;
 
        //we can add new values as well
       root["Input"][0]["type"]["SDI"]["active"] = "no";
      /*  root["last"] = "Jain";*/
 
        // make the changes in a new file/original file
        newFile.open("sample.json");
        writer.write(newFile, root);
        newFile.close();
    return 0;
}
