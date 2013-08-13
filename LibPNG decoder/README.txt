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