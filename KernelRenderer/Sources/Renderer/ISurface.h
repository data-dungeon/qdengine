#ifndef _ISURFACE_H
#define _ISURFACE_H

#include	"Include.h"
#include	"RendererTypeDefs.h"

#include	NEW_OFF
#include	<string>
#include	NEW_ON

class ISurface
{
public:

	virtual				   ~ISurface() {}

	virtual bool			VIsTexture()		{ return false; }
	virtual bool			VIsRenderTarget()	{ return false; }

	virtual uint32			VWidth() const = 0;
	virtual uint32			VHeight() const = 0;
	virtual PIXEL_FORMAT	VPixelFormat() const = 0;
	virtual void *			VGetSurfaceData() const = 0;

	virtual void			VColorFill(uint8 a, uint8 r, uint8 g, uint8 b) = 0;

	// Very useful for debugging
	virtual void			VDrawInBMPFile(const std::string & sFileName) const = 0;
};

#endif