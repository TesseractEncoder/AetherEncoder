#include "../AetherEncoder.h"
using namespace std;
 
 int AetherAction::parseJson(string fpath) 
 {
    
    //opening file using fstream
    ifstream json(fpath);
 
    // check if there is any error is getting data from the json file
        if (!reader.parse(json, root)) {
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
        newJson.open(fpath);
        writer.write(newJson, root);
        newJson.close();
    return 0;
}
