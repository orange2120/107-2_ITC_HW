#include <string>
#include "BMPImg.h"

int main(int argc, char* argv[]) {
    // main function must not change
	if (argc <= 1)
	{
		cerr << "ERROR: Missing argument" << endl;
		return 0;
	}

	string filename = "";
	filename.assign(argv[1]);

	BMPImg img("./img/" + filename);
	img.printHeader();
	img.PrewittFilter();
	img.storePic("./output/test_" + filename);

	return 0;
}


