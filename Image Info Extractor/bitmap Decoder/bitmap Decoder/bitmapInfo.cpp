// runs only for RGB images

#include<iostream>
#include<stdio.h>
#include<EasyBMP.h>
extern "C" {
	#include<jpeglib.h>
	#include<jerror.h>
}

using namespace std;

void main()
{
	BMP Image;
	//int width,height,bitdepth,numColors,row_stride;
	//RGBApixel Temp;
	//
	//Image.ReadFromFile("M2.BMP");
	//bitdepth=Image.TellBitDepth();
	//height=Image.TellHeight();
	//width=Image.TellWidth();
	//numColors=Image.TellNumberOfColors();

	//cout<<"Width\t"<<width;
	//cout<<"\nHeight\t"<<height;
	//cout<<"\nbitdepth\t"<<bitdepth;
	//cout<<"\nNo of Colors\t"<<numColors;
	DisplayBitmapInfo("M2.BMP");
	system("pause");
}
	