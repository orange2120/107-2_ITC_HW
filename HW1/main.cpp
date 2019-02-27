#include <string>
#include "BMPImg.h"

int main(int argc, char* argv[]) {
    // main function must not change
	
	// Test
	/*
	BMPImg imgtest("./img/test.bmp");
	imgtest.printHeader();
	imgtest.rotate();
	imgtest.storePic("./output/test_ans.bmp");
	return 0;
	*/
	
	BMPImg imgrabbit("./img/rabbit.bmp");
	imgrabbit.printHeader();
	imgrabbit.rotate();
	imgrabbit.storePic("./output/rabbit_ans.bmp");

	BMPImg imgflower("./img/flower.bmp");
	imgflower.PrewittFilter();
	imgflower.storePic("./output/flower_ans.bmp");

	return 0;
}


