#ifndef __BMPIMG_H__
#define __BMPIMG_H__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

static const int headerNum = 15;
static const int headerSize[headerNum] = {2,4,4,4,4,4,4,2,2,4,4,4,4,4,4};
static const string headerInfo[headerNum] = { "Identifier", "FileSize",
		"Reserved", "BitmapDataOffset", "BitmapHeaderSize", "Width", "Height",
		"Planes", "BitsPerPixel", "Compression", "BitmapDataSize",
		"H_Resolution", "V_Resolution", "UsedColors", "ImportantColors" };

class BMPHead {
private:
	char Identifier[2];
	unsigned int FileSize;
	unsigned int Reserved;
	unsigned int BitmapDataOffset;
	unsigned int BitmapHeaderSize;
	unsigned int Width;
	unsigned int Height;
	unsigned short Planes;
	unsigned short BitsPerPixel;
	unsigned int Compression;
	unsigned int BitmapDataSize;
	unsigned int H_Resolution;
	unsigned int V_Resolution;
	unsigned int UsedColors;
	unsigned int ImportantColors;
    
public:
	friend class BMPImg;
	void* pFlag(const int i) const { //return pointers of flags according to its order(int)
		switch (i) {
		default:
		case 0:
			return (void*) Identifier;
		case 1:
			return (void*) &FileSize;
		case 2:
			return (void*) &Reserved;
		case 3:
			return (void*) &BitmapDataOffset;
		case 4:
			return (void*) &BitmapHeaderSize;
		case 5:
			return (void*) &Width;
		case 6:
			return (void*) &Height;
		case 7:
			return (void*) &Planes;
		case 8:
			return (void*) &BitsPerPixel;
		case 9:
			return (void*) &Compression;
		case 10:
			return (void*) &BitmapDataSize;
		case 11:
			return (void*) &H_Resolution;
		case 12:
			return (void*) &V_Resolution;
		case 13:
			return (void*) &UsedColors;
		case 14:
			return (void*) &ImportantColors;
		}
	}
};

class BMPImg {
private:
	BMPHead header;
public:
	unsigned char* data;
	unsigned char *newData;

	int getWidth() const {
		return header.Width;
	}
	int getHeight() const {
		return header.Height;
	}
	int getPxlNum() const {
		return header.Width * header.Height;
	}
	int getBytesPerPixel() const {
		return header.BitsPerPixel / 8;
	}

	BMPImg() {
	}
	BMPImg(string picPath) {
		if(!loadPic(picPath))
		{
			cerr << "ERROR: File open failed!" << endl;
			exit(1);
		}
	}
	~BMPImg() {
		delete[] data;
	}

	void copyHead(const BMPImg& ori) {
		for (int i = 0; i < headerNum; ++i) {
			if (headerSize[i] == 4)
				*((unsigned int*) (header.pFlag(i))) =
						*((unsigned int*) (ori.header.pFlag(i)));
			else
				*((unsigned short*) (header.pFlag(i))) =
						*((unsigned short*) (ori.header.pFlag(i)));
		}
		data = new unsigned char[getPxlNum() * getBytesPerPixel()];
	}

	bool loadPic(string picPath) {
		ifstream pic;
		pic.open(picPath.c_str(), ios::in | ios::binary);
        
		if (!pic.is_open())
			return false;

		for (int i = 0; i < headerNum; ++i) {
			pic.read((char*) (header.pFlag(i)), headerSize[i]);
		}
		int dataSize = getPxlNum() * getBytesPerPixel();
		data = new unsigned char[dataSize]; // Bitmap pixel data here
		pic.read((char*) data, dataSize);
        
		return true;
	}

	void printHeader() const {
		for (int i = 0; i < headerNum; ++i) {
			cout << headerInfo[i] + ":";

			if (i == 0) {
				cout << header.Identifier[0] << header.Identifier[1];
			} else {
				if (headerSize[i] == 4)
					cout << *((unsigned int*) (header.pFlag(i)));
				else
					cout << *((unsigned short*) (header.pFlag(i)));
			}
			cout << endl;
		}
	}

	bool storePic(string outPath) {
		ofstream picOut;
		picOut.open(outPath.c_str(), ios::out | ios::binary);
		for (int i = 0; i < headerNum; ++i) {
			picOut.write((char*) (header.pFlag(i)), headerSize[i]);
		}
		picOut.write((char*) data, getPxlNum() * getBytesPerPixel());
		picOut.close();

		return true;
	}

	bool rotate() {
		uint32_t dataSize = getPxlNum() * getBytesPerPixel();
		newData = new unsigned char[dataSize];
		
		uint32_t tmpIdx = 0;
		uint32_t n = 0;

		for (uint32_t w = 1; w <= header.Width; w++)
		{
			for (int h = header.Height; h > 0; h--)
			{
				tmpIdx = (h - 1) * header.Width + w - 1;
				newData[3 * n] = data[3 * tmpIdx];
				newData[3 * n + 1] = data[3 * tmpIdx + 1];
				newData[3 * n + 2] = data[3 * tmpIdx + 2];
				n++;
			}
		}

		// Replace old data
		delete[] data;
		data = newData;
		newData = NULL;

		swap(header.Height, header.Width);
		return true;
	}

	bool RGB2Y() {
		for (uint32_t i = 0, n = getPxlNum() * getBytesPerPixel(); i < n; i += 3)
			data[i + 1] = data[i + 2] = data[i + 3] = 0.3 * data[i] + 0.59 * data[i + 1] + 0.11 * data[i + 2];
		return true;
	}

	bool PrewittFilter() {
		uint32_t dataSize = getPxlNum() * getBytesPerPixel();
		newData = new unsigned char[dataSize];
		uint32_t tmpIdx = 0;
		int32_t gx = 0; // NOTICE: gx,gy may be NEGATIVE!!!
		int32_t gy = 0;
		uint32_t tmpG = 0;

		if (header.Height < 3 || header.Width < 3)
			return false;

		for (uint32_t i = 0; i < dataSize; i++)
			newData[i] = 255;

		//Convert image to grayscale
		RGB2Y();

		//storePic("./output/gray.bmp"); // save grayscale image

		for (uint32_t i = 1, n = header.Height; i < n; ++i)
		{
			for (uint32_t j = 1, m = header.Width; j < m; ++j)
			{
				tmpIdx = i * header.Width + j;
				//cout << "(" << i << "," << j << ") ";
				//cout << tmpIdx << endl;
				gx = -(data[3 * (tmpIdx - 1)] + data[3 * (tmpIdx + header.Width - 1)] + data[3 * (tmpIdx - header.Width - 1)]) + data[3 * (tmpIdx + 1)] + data[3 * (tmpIdx - header.Width + 1)] + data[3 * (tmpIdx + header.Width + 1)];
				gy = -(data[3 * (tmpIdx + header.Width - 1)] + data[3 * (tmpIdx + header.Width)] + data[3 * (tmpIdx + header.Width + 1)]) + data[3 * (tmpIdx - header.Width - 1)] + data[3 * (tmpIdx - header.Width)] + data[3 * (tmpIdx - header.Width + 1)];
				tmpG = sqrt(gx*gx + gy*gy);
				if (tmpG > 255)
					tmpG = 255;
				newData[3 * tmpIdx] = newData[3 * tmpIdx + 1] = newData[3 * tmpIdx + 2] = (unsigned char)tmpG;
			}
		}

		// Replace old data
		delete[] data;
		data = newData;
		newData = NULL;

		return true;
	}

};

#endif // __BMPIMG_H__
