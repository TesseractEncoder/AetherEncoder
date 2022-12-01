#include "AetherEncoder.h"
#include "gstreamer/gst.h"

using namespace std;
int main(int argc, char *argv[])
{
    AetherAction AA;
    gst_init (&argc, &argv);

    AA.parseJson("../sample.json");
    
    cout<< "calling gstreamer" << endl;
    check_gstreamer();
}