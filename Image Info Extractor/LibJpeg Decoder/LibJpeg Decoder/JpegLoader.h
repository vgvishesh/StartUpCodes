//For mapping RGB , 3 channel textures
// Include this header file for loading Jpeg images for texture mapping 
//instrutions to use
//
//using this function image data can be loaded in data buffer-
//stes to use
//1. initialize an object of this strucure in your application code.
//2. declare an array of type unsigned chracter.
//3. call function SetJpegImage() on the Png object created above.
//4. get the width and the height of the png file ,passed in the above function call as argument.
//5. assign the memory to the array declared above.
//6. pass the array as argument in the call to function LoadJpegImage().


//	Code Organizaion:-
//	Allocate and initialize a JPEG decompression object
//	Specify the source of the compressed data (eg, a file)
//	Call jpeg_read_header() to obtain image info
//	Set parameters for decompression
//	jpeg_start_decompress(...);
//	while (scan lines remain to be read)
//		jpeg_read_scanlines(...);
//	jpeg_finish_decompress(...);
//	Release the JPEG decompression object
//
//Decompression details
//---------------------
//
//Here we revisit the JPEG decompression outline given in the overview.
//
//1. Allocate and initialize a JPEG decompression object.
//
//This is just like initialization for compression, as discussed above,
//except that the object is a "struct jpeg_decompress_struct" and you
//call jpeg_create_decompress().  Error handling is exactly the same.
//
//Typical code:
//
//	struct jpeg_decompress_struct cinfo;
//	struct jpeg_error_mgr jerr;
//	...
//	cinfo.err = jpeg_std_error(&jerr);
//	jpeg_create_decompress(&cinfo);
//
//(Both here and in the IJG code, we usually use variable name "cinfo" for
//both compression and decompression objects.)
//
//
//2. Specify the source of the compressed data (eg, a file).
//
//As previously mentioned, the JPEG library reads compressed data from a "data
//source" module.  The library includes one data source module which knows how
//to read from a stdio stream.  You can use your own source module if you want
//to do something else, as discussed later.
//
//If you use the standard source module, you must open the source stdio stream
//beforehand.  Typical code for this step looks like:
//
//	FILE * infile;
//	...
//	if ((infile = fopen(filename, "rb")) == NULL) {
//	    fprintf(stderr, "can't open %s\n", filename);
//	    exit(1);
//	}
//	jpeg_stdio_src(&cinfo, infile);
//
//where the last line invokes the standard source module.
//
//WARNING: it is critical that the binary compressed data be read unchanged.
//On non-Unix systems the stdio library may perform newline translation or
//otherwise corrupt binary data.  To suppress this behavior, you may need to use
//a "b" option to fopen (as shown above), or use setmode() or another routine to
//put the stdio stream in binary mode.  See cjpeg.c and djpeg.c for code that
//has been found to work on many systems.
//
//You may not change the data source between calling jpeg_read_header() and
//jpeg_finish_decompress().  If you wish to read a series of JPEG images from
//a single source file, you should repeat the jpeg_read_header() to
//jpeg_finish_decompress() sequence without reinitializing either the JPEG
//object or the data source module; this prevents buffered input data from
//being discarded.
//
//
//3. Call jpeg_read_header() to obtain image info.
//
//Typical code for this step is just
//
//	jpeg_read_header(&cinfo, TRUE);
//
//This will read the source datastream header markers, up to the beginning
//of the compressed data proper.  On return, the image dimensions and other
//info have been stored in the JPEG object.  The application may wish to
//consult this information before selecting decompression parameters.
//
//More complex code is necessary if
//  * A suspending data source is used --- in that case jpeg_read_header()
//    may return before it has read all the header data.  See "I/O suspension",
//    below.  The normal stdio source manager will NOT cause this to happen.
//  * Abbreviated JPEG files are to be processed --- see the section on
//    abbreviated datastreams.  Standard applications that deal only in
//    interchange JPEG files need not be concerned with this case either.
//
//It is permissible to stop at this point if you just wanted to find out the
//image dimensions and other header info for a JPEG file.  In that case,
//call jpeg_destroy() when you are done with the JPEG object, or call
//jpeg_abort() to return it to an idle state before selecting a new data
//source and reading another header.
//
//
//4. Set parameters for decompression.
//
//jpeg_read_header() sets appropriate default decompression parameters based on
//the properties of the image (in particular, its colorspace).  However, you
//may well want to alter these defaults before beginning the decompression.
//For example, the default is to produce full color output from a color file.
//If you want colormapped output you must ask for it.  Other options allow the
//returned image to be scaled and allow various speed/quality tradeoffs to be
//selected.  "Decompression parameter selection", below, gives details.
//
//If the defaults are appropriate, nothing need be done at this step.
//
//Note that all default values are set by each call to jpeg_read_header().
//If you reuse a decompression object, you cannot expect your parameter
//settings to be preserved across cycles, as you can for compression.
//You must set desired parameter values each time.
//
//
//5. jpeg_start_decompress(...);
//
//Once the parameter values are satisfactory, call jpeg_start_decompress() to
//begin decompression.  This will initialize internal state, allocate working
//memory, and prepare for returning data.
//
//Typical code is just
//
//	jpeg_start_decompress(&cinfo);
//
//If you have requested a multi-pass operating mode, such as 2-pass color
//quantization, jpeg_start_decompress() will do everything needed before data
//output can begin.  In this case jpeg_start_decompress() may take quite a while
//to complete.  With a single-scan (non progressive) JPEG file and default
//decompression parameters, this will not happen; jpeg_start_decompress() will
//return quickly.
//
//After this call, the final output image dimensions, including any requested
//scaling, are available in the JPEG object; so is the selected colormap, if
//colormapped output has been requested.  Useful fields include
//
//	output_width		image width and height, as scaled
//	output_height
//	out_color_components	# of color components in out_color_space
//	output_components	# of color components returned per pixel
//	colormap		the selected colormap, if any
//	actual_number_of_colors		number of entries in colormap
//
//output_components is 1 (a colormap index) when quantizing colors; otherwise it
//equals out_color_components.  It is the number of JSAMPLE values that will be
//emitted per pixel in the output arrays.
//
//Typically you will need to allocate data buffers to hold the incoming image.
//You will need output_width * output_components JSAMPLEs per scanline in your
//output buffer, and a total of output_height scanlines will be returned.
//
//Note: if you are using the JPEG library's internal memory manager to allocate
//data buffers (as djpeg does), then the manager's protocol requires that you
//request large buffers *before* calling jpeg_start_decompress().  This is a
//little tricky since the output_XXX fields are not normally valid then.  You
//can make them valid by calling jpeg_calc_output_dimensions() after setting the
//relevant parameters (scaling, output color space, and quantization flag).
//
//
//6. while (scan lines remain to be read)
//	jpeg_read_scanlines(...);
//
//Now you can read the decompressed image data by calling jpeg_read_scanlines()
//one or more times.  At each call, you pass in the maximum number of scanlines
//to be read (ie, the height of your working buffer); jpeg_read_scanlines()
//will return up to that many lines.  The return value is the number of lines
//actually read.  The format of the returned data is discussed under "Data
//formats", above.  Don't forget that grayscale and color JPEGs will return
//different data formats!
//
//Image data is returned in top-to-bottom scanline order.  If you must write
//out the image in bottom-to-top order, you can use the JPEG library's virtual
//array mechanism to invert the data efficiently.  Examples of this can be
//found in the sample application djpeg.
//
//The library maintains a count of the number of scanlines returned so far
//in the output_scanline field of the JPEG object.  Usually you can just use
//this variable as the loop counter, so that the loop test looks like
//"while (cinfo.output_scanline < cinfo.output_height)".  (Note that the test
//should NOT be against image_height, unless you never use scaling.  The
//image_height field is the height of the original unscaled image.)
//The return value always equals the change in the value of output_scanline.
//
//If you don't use a suspending data source, it is safe to assume that
//jpeg_read_scanlines() reads at least one scanline per call, until the
//bottom of the image has been reached.
//
//If you use a buffer larger than one scanline, it is NOT safe to assume that
//jpeg_read_scanlines() fills it.  (The current implementation returns only a
//few scanlines per call, no matter how large a buffer you pass.)  So you must
//always provide a loop that calls jpeg_read_scanlines() repeatedly until the
//whole image has been read.
//
//
//7. jpeg_finish_decompress(...);
//
//After all the image data has been read, call jpeg_finish_decompress() to
//complete the decompression cycle.  This causes working memory associated
//with the JPEG object to be released.
//
//Typical code:
//
//	jpeg_finish_decompress(&cinfo);
//
//If using the stdio source manager, don't forget to close the source stdio
//stream if necessary.
//
//It is an error to call jpeg_finish_decompress() before reading the correct
//total number of scanlines.  If you wish to abort decompression, call
//jpeg_abort() as discussed below.
//
//After completing a decompression cycle, you may dispose of the JPEG object as
//discussed next, or you may use it to decompress another image.  In that case
//return to step 2 or 3 as appropriate.  If you do not change the source
//manager, the next image will be read from the same source.
//
//
//8. Release the JPEG decompression object.
//
//When you are done with a JPEG decompression object, destroy it by calling
//jpeg_destroy_decompress() or jpeg_destroy().  The previous discussion of
//destroying compression objects applies here too.
//
//Typical code:
//
//	jpeg_destroy_decompress(&cinfo);
//
//
//9. Aborting.
//
//You can abort a decompression cycle by calling jpeg_destroy_decompress() or
//jpeg_destroy() if you don't need the JPEG object any more, or
//jpeg_abort_decompress() or jpeg_abort() if you want to reuse the object.
//The previous discussion of aborting compression cycles applies here too.
#pragma once
#include<iostream>
#include<stdio.h>

extern "C" {
	#include<jpeglib.h>
	#include<jerror.h>
}



class Jpeg
{
private:
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

public:
	int Width;
	int Height;
	int Bitdepth;
	int Channels;

	void SetJpegImage(char *filename)
	{
		FILE *infile;
		cinfo.err=jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);

		if ((infile = fopen(filename, "rb")) == NULL) 
		{
			fprintf(stderr, "can't open %s\n", filename);
			exit(0);
		}
		jpeg_stdio_src(&cinfo, infile);

		jpeg_read_header(&cinfo, TRUE);
		jpeg_start_decompress(&cinfo);
		
		Width=cinfo.output_width;
		//cout<<Width;
		Height=cinfo.output_height;
		Bitdepth=cinfo.out_color_space;
		Channels=cinfo.output_components;
	}

	void LoadJpegImage(unsigned char *data)
	{
		int row_stride = cinfo.output_width * cinfo.output_components;
		while (cinfo.output_scanline < cinfo.output_height) 
		{
			unsigned char *rowp[1];
			rowp[0] = (unsigned char *) data + row_stride * cinfo.output_scanline;	//top down reading order
			jpeg_read_scanlines(&cinfo, rowp, 1);
		}
	}
};

