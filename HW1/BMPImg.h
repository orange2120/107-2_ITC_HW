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
		loadPic(picPath);
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

				//cout << "(" << w << "," << h <<") IDX: " << tmpIdx << ", n: " << n << endl;
				for (uint8_t k = 0; k < 3; k++)
				{
					newData[3 * n + k] = data[3 * tmpIdx + k];
					//cout << 3 * tmpIdx + k<< "->" << 3 * n + k << endl;
				}
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
		unsigned char y = 0;
		for (uint32_t i = 0, n = getPxlNum() * getBytesPerPixel(); i < n; i += 3)
		{
			y = 0.3 * data[i] + 0.59 * data[i + 1] + 0.11 * data[i + 2];
			data[i + 1] = data[i + 2] = data[i + 3] = y;
		}
		return true;
	}

	bool PrewittFilter() {
		
		uint32_t dataSize = getPxlNum() * getBytesPerPixel();
		newData = new unsigned char[dataSize];
		uint16_t gx = 0;
		uint16_t gy = 0;

		//Convert image to grayscale
		RGB2Y();

		for (uint32_t i = 1, n = header.Width - 1; i < n; i++)
		{
			for (uint32_t j = 1, m = header.Height - 1; j < m; j++)
			{
				gx = -(data[i * j - 1] + data[i * j + header.Width - 1] + data[i * j - header.Width - 1]) + data[i * j + 1] + data[i * j - header.Width + 1] + data[i * j + header.Width + 1];
				gx = -(data[i * j + header.Width - 1] + data[i * j + header.Width] + data[i * j + header.Width + 1]) + data[i * j - header.Width - 1] + data[i * j - header.Width] + data[i * j - header.Width + 1];
				data[i * j] = sqrt(gx*gx + gy*gy);
			}
		}

		for (uint32_t i = 0; i < header.Width; i++)
			data[i] = data[i + 1] = data[i + 2] = 255;
		for (uint32_t i = 1; i < header.Height - 2; i++)
		{
			data[3 * i * header.Width] = data[3 * header.Width + 1] = data[3 * header.Width + 2] = 255;
			data[3*(i*header.Width+header.Width-1)] = data[3*(i*header.Width+header.Width-1) + 1] = data[3*(i*header.Width+header.Width-1) + 2] = 255;
		}
		for (uint32_t i = header.Width * (header.Height - 1); i < getPxlNum(); i++)
			data[i] = data[i + 1] = data[i + 2] = 255;

			return true;
	}

};

#endif // __BMPIMG_H__
