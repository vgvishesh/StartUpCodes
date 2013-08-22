How to Run the VC++ Project-

1. Copy the jpeg-8d and lpng162 folders any where in your computer.
2. open the visual c++ project .sln file
3. Go to the properties->vc++ directories and
	replace 
	a. "c:\lpng162" by the address where lpng162 folder is copied in step 1.
	b. "c:\jpeg-8d" by the address where jpeg-8d folder is copied in step 1.
4. Click ok.
5. Compile and Run.



File Description :

pngDecoder.cpp =The main file having the c++ code to extract image meta data and 
				color dataa.
				-It extracts the color data from image ad stores it in a character array.
				 Then this data is written in an output file "out.abc";
				-Data is extracted in a row major order, from the image.
				
This data isn the output file will be fed to the LibJpeg Encoder to get a DCT encoded image data.

1.png,2.png,3.png are the sample images used to test this program.

lpng162 folder contains dll and library files needed to link the Libpng code and 
run it in microsoft visual c++2010.

pngDecoder_jpegEncoder.cpp is the c++ file having the code which takes in png Image as Input and gives 
DCT encoded jpeg image as output
	Known bug-
	1. libpng handles 5 different color spaces namely-
		1.RGBA
		2.RGB
		3.GRAYSCALE
		4.GRAYSCALE_ALPHA
		5.PALETTE
		
		but libpng handles different color spaces like 
		RGB,GRAYSCALE,YCbCr,CMYK
		
		if the inputpng image has color space as RBG or GRAYSALE then there is no
		problem in converting it to DCT image , else
		there are problems.....
