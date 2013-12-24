#include <iostream>
#include <stdio.h>
#include "JpegLoader.h"
using namespace std;


int main()
{
	Jpeg image;
	image.SetJpegImage("test.jpg");
	cout<<"Width\t"<<image.Width<<endl;
	cout<<"Height\t"<<image.Height<<endl;
	cout<<"BitDepth\t"<<image.Bitdepth<<endl;
	cout<<"Channels\t"<<image.Channels<<endl;

	system("pause");
}
