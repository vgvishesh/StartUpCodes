#include<iostream>
#include<stdio.h>
#include<png.h>

using namespace std;

bool validate(FILE *fp)
{
	png_byte head[8];
	
	if(!fp)
	{
		return 0;
	}
	fread(head,1,8,fp);
	if(!png_sig_cmp(head,0,8))
		return 1;
	return 0;
}

void userReadData(png_structp pngPtr, png_bytep data, png_size_t length)
{
    //Here we get our IO pointer back from the read struct.
    //This is the parameter we passed to the png_set_read_fn() function.
    //Our std::istream pointer.
    png_voidp a = png_get_io_ptr(pngPtr);
    //Cast the pointer to std::istream* and read 'length' bytes into 'data'
    //fread(&data,1,length,(FILE *)a);
	((istream *)a)->read((char *)data,length);
}

void main()
{
	FILE *fp;
	fp=fopen("3.png","rb");
	if(!validate(fp))
	{
		return;
	}

	png_structp pngPtr=png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngPtr)
	{
		cerr<<"Error: Couldn't initialize png read struct";
		return;
	}
	png_infop pngInfo=png_create_info_struct(pngPtr);
	if(!pngInfo)
	{
		cerr << "ERROR: Couldn't initialize png info struct" <<endl;
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		return; //Do your own error recovery/handling here
	}

	if (setjmp(png_jmpbuf(pngPtr))) 
	{
    //An error occured, so clean up what we have allocated so far...
    png_destroy_read_struct(&pngPtr, &pngInfo,(png_infopp)0);
    
    std::cout << "ERROR: An error occured while reading the PNG file";
	fclose(fp);
	return; // Do your own error handling here.
	}

	//png_set_read_fn(pngPtr,&stdin, userReadData);
	
	png_init_io(pngPtr, fp);
	png_set_sig_bytes(pngPtr,8);	
	png_read_info(pngPtr,pngInfo);

	png_uint_32 imgWidth =  png_get_image_width(pngPtr, pngInfo);
    png_uint_32 imgHeight = png_get_image_height(pngPtr, pngInfo);

    //bits per CHANNEL! note: not per pixel!
    png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, pngInfo);
    //Number of channels
    png_uint_32 channels   = png_get_channels(pngPtr, pngInfo);
    //Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
    png_uint_32 color_type = png_get_color_type(pngPtr, pngInfo);

	cout<<"\nwidth\t"<<imgWidth;
	cout<<"\nHeight\t"<<imgHeight;
	cout<<"\nColor Depth\t"<<bitdepth;
	cout<<"\nColor Channels\t"<<channels;
	cout<<"\nColor type"<<color_type;

	int size=imgWidth*imgHeight*bitdepth*channels/8;
	char *data=new char[size];
	png_byte **rowPtrs=new png_byte* [imgHeight];
	const unsigned int stride=imgWidth*bitdepth*channels/8;

	for(int i=0;i<imgHeight;i++)
	{
		rowPtrs[i]=new png_byte[stride];
		rowPtrs[i]=(png_byte*)&data[i*stride];
	}
	png_read_image(pngPtr,rowPtrs);
	//try{
	//	png_read_image(pngPtr,rowPtrs);
	//	//png_read_end(pngPtr,pngInfo);
	//	throw 20;
	//}
	//catch(...)
	//{
	//	cout<<"VIshesh";
	//}

	FILE *out=fopen("out.abc","wb");
	for(int i=0;i<size;i++)
	{
		fprintf(out,"%c",data[i]);
	}
	
	delete[] rowPtrs;
    //And don't forget to clean up the read and info structs !
    png_destroy_read_struct(&pngPtr, &pngInfo,(png_infopp)0);
	delete[] data;
	fclose(fp);
	fclose(out);


	system("pause");
}