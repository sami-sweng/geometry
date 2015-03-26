//=============================================================================
//                                                
//   Code framework for the lecture
//
//   "Digital 3D Geometry Processing"
//
//   Mark Pauly, Mario Botsch, Balint Miklos, Hao Li, Sofien Bouaziz,
//   Yuliy Schwartzburg, Duygu Ceylan, Mario Deuss, Bailin Deng,
//   Alexandru Ichim, Anastasia Tkach
//
//   Copyright (C) 2007-2015 by Computer Graphics and Geometry Laboratory,
//         EPF Lausanne, Applied Geometry Group and Computer Graphics
//         Laboratory, ETH Zurich
//                                                                         
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin Street, Fifth Floor, 
//   Boston, MA  02110-1301, USA.
//                                                                            
//=============================================================================
//=============================================================================
//  overload some GL functions
//=============================================================================


#ifndef GL_WRAPPERS_HH
#define GL_WRAPPERS_HH


//== INCLUDES =================================================================


// Mac OS X
#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>

// Windows
#elif _WIN32
#  include <stdlib.h>
#  include <GL/glut.h>
#  include <GL/GL.h>
#  include <GL/GLU.h>

// Unix
#else
#  include <GL/glut.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <surface_mesh/Vector.h>



//=============================================================================
namespace GL {
//=============================================================================


//-------------------------------------------------------------------- glVertex

/// Wrapper: glVertex for surface_mesh::Vec2i
inline void glVertex(const surface_mesh::Vec2i& _v)  { glVertex2i(_v[0], _v[1]); }
/// Wrapper: glVertex for surface_mesh::Vec2f
inline void glVertex(const surface_mesh::Vec2f& _v)  { glVertex2fv(_v.data()); }
/// Wrapper: glVertex for surface_mesh::Vec2d
inline void glVertex(const surface_mesh::Vec2d& _v)  { glVertex2dv(_v.data()); }

/// Wrapper: glVertex for surface_mesh::Vec3f
inline void glVertex(const surface_mesh::Vec3f& _v)  { glVertex3fv(_v.data()); }
/// Wrapper: glVertex for surface_mesh::Vec3d
inline void glVertex(const surface_mesh::Vec3d& _v)  { glVertex3dv(_v.data()); }

/// Wrapper: glVertex for surface_mesh::Vec4f
inline void glVertex(const surface_mesh::Vec4f& _v)  { glVertex4fv(_v.data()); }
/// Wrapper: glVertex for surface_mesh::Vec4d
inline void glVertex(const surface_mesh::Vec4d& _v)  { glVertex4dv(_v.data()); }



//------------------------------------------------------------------- glTexCoord

/// Wrapper: glTexCoord for 1D float
inline void glTexCoord(float _t) { glTexCoord1f(_t); }
/// Wrapper: glTexCoord for 1D double
inline void glTexCoord(double _t) { glTexCoord1d(_t); }

/// Wrapper: glTexCoord for surface_mesh::Vec2f
inline void glTexCoord(const surface_mesh::Vec2f& _t) { glTexCoord2fv(_t.data()); }
/// Wrapper: glTexCoord for surface_mesh::Vec2d
inline void glTexCoord(const surface_mesh::Vec2d& _t) { glTexCoord2dv(_t.data()); }

/// Wrapper: glTexCoord for surface_mesh::Vec3f
inline void glTexCoord(const surface_mesh::Vec3f& _t) { glTexCoord3fv(_t.data()); }
/// Wrapper: glTexCoord for surface_mesh::Vec3d
inline void glTexCoord(const surface_mesh::Vec3d& _t) { glTexCoord3dv(_t.data()); }

/// Wrapper: glTexCoord for surface_mesh::Vec4f
inline void glTexCoord(const surface_mesh::Vec4f& _t) { glTexCoord4fv(_t.data()); }
/// Wrapper: glTexCoord for surface_mesh::Vec4d
inline void glTexCoord(const surface_mesh::Vec4d& _t) { glTexCoord4dv(_t.data()); }



//--------------------------------------------------------------------- glNormal

/// Wrapper: glNormal for surface_mesh::Vec3f
inline void glNormal(const surface_mesh::Vec3f& _n)  { glNormal3fv(_n.data()); }
/// Wrapper: glNormal for surface_mesh::Vec3d
inline void glNormal(const surface_mesh::Vec3d& _n)  { glNormal3dv(_n.data()); }



//---------------------------------------------------------------------- glColor

/// Wrapper: glColor for surface_mesh::Vec3f
inline void glColor(const surface_mesh::Vec3f&  _v)  { glColor3fv(_v.data()); }
/// Wrapper: glColor for surface_mesh::Vec3uc
inline void glColor(const surface_mesh::Vec3uc& _v)  { glColor3ubv(_v.data()); }

/// Wrapper: glColor for surface_mesh::Vec4f
inline void glColor(const surface_mesh::Vec4f&  _v)  { glColor4fv(_v.data()); }
/// Wrapper: glColor for surface_mesh::Vec4uc
inline void glColor(const surface_mesh::Vec4uc&  _v) { glColor4ubv(_v.data()); }



//-------------------------------------------------------------- glVertexPointer

/// Wrapper: glVertexPointer for surface_mesh::Vec2f
inline void glVertexPointer(const surface_mesh::Vec2f* _p)
{ ::glVertexPointer(2, GL_FLOAT, 0, _p); }
/// Wrapper: glVertexPointer for surface_mesh::Vec2d
inline void glVertexPointer(const surface_mesh::Vec2d* _p)
{ ::glVertexPointer(2, GL_DOUBLE, 0, _p); }

/// Wrapper: glVertexPointer for surface_mesh::Vec3f
inline void glVertexPointer(const surface_mesh::Vec3f* _p)
{ ::glVertexPointer(3, GL_FLOAT, 0, _p); }
/// Wrapper: glVertexPointer for surface_mesh::Vec3d
inline void glVertexPointer(const surface_mesh::Vec3d* _p)
{ ::glVertexPointer(3, GL_DOUBLE, 0, _p); }

/// Wrapper: glVertexPointer for surface_mesh::Vec4f
inline void glVertexPointer(const surface_mesh::Vec4f* _p)
{ ::glVertexPointer(4, GL_FLOAT, 0, _p); }
/// Wrapper: glVertexPointer for surface_mesh::Vec4d
inline void glVertexPointer(const surface_mesh::Vec4d* _p)
{ ::glVertexPointer(4, GL_DOUBLE, 0, _p); }

/// original method
inline void glVertexPointer(GLint n, GLenum t, GLsizei s, const GLvoid *p)
{ ::glVertexPointer(n, t, s, p); }



//-------------------------------------------------------------- glNormalPointer

/// Wrapper: glNormalPointer for surface_mesh::Vec3f
inline void glNormalPointer(const surface_mesh::Vec3f* _p)
{ ::glNormalPointer(GL_FLOAT, 0, _p); }
/// Wrapper: glNormalPointer for surface_mesh::Vec3d
inline void glNormalPointer(const surface_mesh::Vec3d* _p)
{ ::glNormalPointer(GL_DOUBLE, 0, _p); }

/// original method
inline void glNormalPointer(GLenum t, GLsizei s, const GLvoid *p)
{ ::glNormalPointer(t, s, p); }



//--------------------------------------------------------------- glColorPointer

/// Wrapper: glColorPointer for surface_mesh::Vec3uc
inline void glColorPointer(const surface_mesh::Vec3uc* _p)
{ ::glColorPointer(3, GL_UNSIGNED_BYTE, 0, _p); }
/// Wrapper: glColorPointer for surface_mesh::Vec3f
inline void glColorPointer(const surface_mesh::Vec3f* _p)
{ ::glColorPointer(3, GL_FLOAT, 0, _p); }

/// Wrapper: glColorPointer for surface_mesh::Vec4uc
inline void glColorPointer(const surface_mesh::Vec4uc* _p)
{ ::glColorPointer(4, GL_UNSIGNED_BYTE, 0, _p); }
/// Wrapper: glColorPointer for surface_mesh::Vec4f
inline void glColorPointer(const surface_mesh::Vec4f* _p)
{ ::glColorPointer(4, GL_FLOAT, 0, _p); }

/// original method
inline void glColorPointer(GLint n, GLenum t, GLsizei s, const GLvoid *p)
{ ::glColorPointer(n, t, s, p); }



//------------------------------------------------------------ glTexCoordPointer

/// Wrapper: glTexCoordPointer for float
inline void glTexCoordPointer(const float* _p) 
{ ::glTexCoordPointer(1, GL_FLOAT, 0, _p); }
/// Wrapper: glTexCoordPointer for surface_mesh::Vec2d
inline void glTexCoordPointer(const double* _p) 
{ ::glTexCoordPointer(1, GL_DOUBLE, 0, _p); }

/// Wrapper: glTexCoordPointer for surface_mesh::Vec2f
inline void glTexCoordPointer(const surface_mesh::Vec2f* _p)
{ ::glTexCoordPointer(2, GL_FLOAT, 0, _p); }
/// Wrapper: glTexCoordPointer for surface_mesh::Vec2d
inline void glTexCoordPointer(const surface_mesh::Vec2d* _p)
{ ::glTexCoordPointer(2, GL_DOUBLE, 0, _p); }

/// Wrapper: glTexCoordPointer for surface_mesh::Vec3f
inline void glTexCoordPointer(const surface_mesh::Vec3f* _p)
{ ::glTexCoordPointer(3, GL_FLOAT, 0, _p); }
/// Wrapper: glTexCoordPointer for surface_mesh::Vec3d
inline void glTexCoordPointer(const surface_mesh::Vec3d* _p)
{ ::glTexCoordPointer(3, GL_DOUBLE, 0, _p); }

/// Wrapper: glTexCoordPointer for surface_mesh::Vec4f
inline void glTexCoordPointer(const surface_mesh::Vec4f* _p)
{ ::glTexCoordPointer(4, GL_FLOAT, 0, _p); }
/// Wrapper: glTexCoordPointer for surface_mesh::Vec4d
inline void glTexCoordPointer(const surface_mesh::Vec4d* _p)
{ ::glTexCoordPointer(4, GL_DOUBLE, 0, _p); }

/// original method
inline void glTexCoordPointer(GLint n, GLenum t, GLsizei s, const GLvoid *p)
{ ::glTexCoordPointer(n, t, s, p); }



//-----------------------------------------------------------------------------


/** Nice wrapper that outputs all current OpenGL errors to std::cerr.
    If no error is present nothing is printed.
**/
inline void glCheckErrors()
{
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR)
  {
    std::cerr << "GL error: " << gluErrorString(error) << std::endl;
  }
}


//=============================================================================
}
//=============================================================================
#endif // GL_WRAPPERS_HH defined
//=============================================================================
