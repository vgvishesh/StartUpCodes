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
	int width,height,bitdepth,numColors,row_stride;
	RGBApixel Temp;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE *outfile;
	
	Image.ReadFromFile("M2.BMP");
	bitdepth=Image.TellBitDepth();
	height=Image.TellHeight();
	width=Image.TellWidth();
	numColors=Image.TellNumberOfColors();

	cout<<"Width\t"<<width;
	cout<<"\nHeight\t"<<height;
	cout<<"\nbitdepth\t"<<bitdepth;
	cout<<"\nNo of Colors\t"<<numColors;

	//--------------------------------------------------------------LIBJPEG Code-------------------------------------------------------------------//	
	cinfo.err=jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	
	if ((outfile = fopen("bmp_DCT_COLOR.jpg", "wb")) == NULL) 
	{
	    fprintf(stderr, "can't open jpegDCT.jpg\n");
	    exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_height=height;
	cinfo.image_width=width;
	
	if(bitdepth==1)
	{
		cinfo.input_components=1;
		cinfo.in_color_space=JCS_GRAYSCALE;
	}
	else
	{
		cinfo.input_components=3;
		cinfo.in_color_space=JCS_RGB;
	}
	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);

	row_stride=cinfo.image_width*cinfo.input_components;
	//JSAMPROW row_pointer[1];
	unsigned char *scanline=new unsigned char[row_stride];
	//JSAMPLE row_data;
	JSAMPROW *jrow=new JSAMPLE*[1];
	//JSAMPARRAY jarray=&jrow;
	
	cout<<"\nEncoding..... ";
	if(cinfo.input_components==3)
	{
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				Temp=Image.GetPixel(j,i);
				scanline[j*cinfo.input_components + 0]=(int)Temp.Red;
				scanline[j*cinfo.input_components + 1]=(int)Temp.Green;
				scanline[j*cinfo.input_components + 2]=(int)Temp.Blue;
				//cout<<Image(i,j)->Red<<"\t"<<Image(j,i)->Green<<"\t"<<Image(i,j)->Blue<<"\n";
				//cout<<"\n"<<(int)Temp.Red<<"\t"<<(int)Temp.Green<<"\t"<<(int)Temp.Blue<<"\t"<<(int)Temp.Alpha;
				//cout<<"\t"<<i<<"\t"<<j;
			}
			//row_data=scanline;
			jrow[0]=scanline;
			//jpeg_write_scanlines(&cinfo,jarray,1);
			jpeg_write_scanlines(&cinfo,jrow,1);
		}
	}
	else if(cinfo.input_components==1)
	{
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				Temp=Image.GetPixel(j,i);
				scanline[j]=(int)Temp.Red;
			}
			//row_data=scanline;
			jrow[0]=scanline;
			//jpeg_write_scanlines(&cinfo,jarray,1);
			jpeg_write_scanlines(&cinfo,jrow,1);
		}
	}
	//cout<<"\n"<<scanline[0]<<"\t"<<scanline[1]<<"\t"<<scanline[2];
	cout<<"\n\nFinished Encoding....\n\n";
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
	system("pause");
}



