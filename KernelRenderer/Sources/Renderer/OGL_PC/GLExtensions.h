//*****************************************************************************
//
// Copyright (C) 2000-2005 Quantic Dream SA
//
// These coded instructions, statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are 
// protected by French and EEC copyright laws. They may not be 
// disclosed to third parties or copied or duplicated, in whole or in part, 
// without prior written consent of Quantic Dream SA
//
// Unpublished-rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
//	05-12-15:	SBE - Created
//*****************************************************************************

// ARB_vertex_buffer_object
extern PFNGLBINDBUFFERARBPROC           glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC           glGenBuffersARB;
extern PFNGLISBUFFERARBPROC             glIsBufferARB;
extern PFNGLBUFFERDATAARBPROC           glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC        glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC     glGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC            glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC          glUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC    glGetBufferPointervARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC  glClientActiveTextureARB;
extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLCOMPRESSEDTEXIMAGE1DARBPROC	glCompressedTexImage1DARB;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC	glCompressedTexImage2DARB;
extern PFNGLCOMPRESSEDTEXIMAGE3DARBPROC	glCompressedTexImage3DARB;
extern PFNGLCOPYTEXSUBIMAGE2DEXTPROC	glCopyTexSubImage2DEXT;
extern PFNGLBLENDEQUATIONPROC			glBlendEquation;

extern PFNGLPROGRAMSTRINGARBPROC			glProgramStringARB;
extern PFNGLBINDPROGRAMARBPROC				glBindProgramARB;
extern PFNGLDELETEPROGRAMSARBPROC			glDeleteProgramsARB;
extern PFNGLGENPROGRAMSARBPROC				glGenProgramsARB;
extern PFNGLPROGRAMENVPARAMETER4DARBPROC	glProgramEnvParameter4dARB;
extern PFNGLPROGRAMENVPARAMETER4DVARBPROC	glProgramEnvParameter4dvARB; 
extern PFNGLPROGRAMENVPARAMETER4FARBPROC	glProgramEnvParameter4fARB; 
extern PFNGLPROGRAMENVPARAMETER4FVARBPROC	glProgramEnvParameter4fvARB; 
extern PFNGLPROGRAMLOCALPARAMETER4DARBPROC	glProgramLocalParameter4dARB; 
extern PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB; 
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC	glProgramLocalParameter4fARB; 
extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB; 

// EXT
extern PFNGLDRAWRANGEELEMENTSEXTPROC					glDrawRangeElementsEXT;
extern PFNGLTEXIMAGE3DPROC								glTexImage3D;
extern PFNGLISRENDERBUFFEREXTPROC						glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC						glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC						glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC			glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC						glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC						glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC 				glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC 				glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC 				glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC						glGenerateMipmapEXT;
extern PFNWGLSWAPINTERVALEXTPROC						wglSwapIntervalEXT;

// NV
extern PFNGLGENPROGRAMSNVPROC				glGenProgramsNV;
extern PFNGLLOADPROGRAMNVPROC				glLoadProgramNV;
extern PFNGLBINDPROGRAMNVPROC				glBindProgramNV;
extern PFNGLPROGRAMPARAMETER4FNVPROC		glProgramParameter4fNV;
extern PFNGLPROGRAMPARAMETER4FVNVPROC		glProgramParameter4fvNV;
extern PFNGLPROGRAMNAMEDPARAMETER4FNVPROC	glProgramNamedParameter4fNV;
extern PFNGLPROGRAMNAMEDPARAMETER4DNVPROC	glProgramNamedParameter4dNV;
extern PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC	glProgramNamedParameter4fvNV;
extern PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC	glProgramNamedParameter4dvNV;
extern PFNGLISPROGRAMNVPROC					glIsProgramNV;
extern PFNGLVERTEXATTRIBPOINTERNVPROC		glVertexAttribPointerNV;
extern PFNGLREQUESTRESIDENTPROGRAMSNVPROC	glRequestResidentProgramsNV;
