//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#ifndef BACKBUFFER_H
#define BACKBUFFER_H

#include <Windows.h>


class BackBuffer
{
public:
	//BackBuffer constructor that take the handle to the window from
	//which it will create a device context and the width and height.
	BackBuffer(HWND hWnd, int width, int height);
	//BackBuffer destructor
	~BackBuffer();

	//Data member getters
	HDC getDC();
	int width();
	int height();

	//Method that presents the back buffer to the client window
	void present();

private:
	// Make copy constructor and assignment operator private
	// so client cannot copy BackBuffers.  We do this because
	// this class is not designed to be copied because it
	// is not efficient--copying bitmaps is slow (lots of memory).
	// In addition, most applications will probably only need one
	// BackBuffer anyway.  
	BackBuffer(const BackBuffer& rhs);
	BackBuffer& operator=(const BackBuffer& rhs);

private:
	//Data members
	HWND    mhWnd;			//Window handle
	HDC     mhDC;			//Device context handle
	HBITMAP mhSurface;		//Bitmap surfave to be drawn upon
	HBITMAP mhOldObject;	//Old bitmap that was selected before the draw
	int     mWidth;			//Width of client area
	int     mHeight;		//Height of client area
};


#endif //BACKBUFFER_H
