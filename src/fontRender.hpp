﻿#pragma once

//
// fontstashのレンダリング
//

#include <cinder/gl/Texture.h>
#include <cinder/TriMesh.h>


namespace ngs { namespace FontRender {

struct Context {
  ci::gl::Texture2dRef tex;
  int width, height;
};


int create(void* userPtr, int width, int height)
{
  Context* gl = (Context*)userPtr;

  // TIPS:テクスチャ形式をGL_R8にしといて
  //      シェーダーでなんとかする方式(from nanoVG)
  gl->tex = ci::gl::Texture2d::create(width, height,
                                      ci::gl::Texture2d::Format().dataType(GL_UNSIGNED_BYTE).minFilter(GL_LINEAR).internalFormat(GL_R8));

  gl->width  = width;
  gl->height = height;

  return 1;
}

int resize(void* userPtr, int width, int height)
{
	// Reuse create to resize too.
	return create(userPtr, width, height);
}

void update(void* userPtr, int* rect, const unsigned char* data)
{
	Context* gl = (Context*)userPtr;
	if (!gl->tex.get()) return;

	int w = rect[2] - rect[0];
	int h = rect[3] - rect[1];

  // TIPS:data側も切り抜いて転送するので
  //      その指定も忘れない
	// glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, gl->width);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect[0]);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, rect[1]);

  gl->tex->update(data, GL_RED, GL_UNSIGNED_BYTE, 0, w, h, ci::ivec2(rect[0], rect[1]));
}

void draw(void* userPtr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts)
{
  using namespace ci;

	Context* gl = (Context*)userPtr;
	if (!gl->tex.get()) return;

  auto* ctx = gl::context();
  const gl::GlslProg* curGlslProg = ctx->getGlslProg();

  size_t totalArrayBufferSize = sizeof(float) * nverts * 2
    + sizeof(float) * nverts * 2
    + nverts * 4;

	ctx->pushVao();
	ctx->getDefaultVao()->replacementBindBegin();

  gl::VboRef defaultArrayVbo = ctx->getDefaultArrayVbo( totalArrayBufferSize );

  gl::ScopedBuffer vboScp( defaultArrayVbo );
  gl::ScopedTextureBind texScp( gl->tex );

  size_t curBufferOffset = 0;
  {
		int loc = curGlslProg->getAttribSemanticLocation( geom::Attrib::POSITION );
		gl::enableVertexAttribArray( loc );
		gl::vertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)curBufferOffset );
		defaultArrayVbo->bufferSubData( curBufferOffset, sizeof(float) * nverts * 2, verts );
		curBufferOffset += sizeof(float) * nverts * 2;
  }

  {
		int loc = curGlslProg->getAttribSemanticLocation( geom::Attrib::TEX_COORD_0 );
		gl::enableVertexAttribArray( loc );
		gl::vertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)curBufferOffset );
		defaultArrayVbo->bufferSubData( curBufferOffset, sizeof(float) * nverts * 2, tcoords );
		curBufferOffset += sizeof(float)* nverts * 2;
  }

  {
		int loc = curGlslProg->getAttribSemanticLocation( geom::Attrib::COLOR );
		gl::enableVertexAttribArray( loc );
		gl::vertexAttribPointer( loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)curBufferOffset );
		defaultArrayVbo->bufferSubData( curBufferOffset, nverts * 4, colors );
		curBufferOffset += nverts * 4;
  }

	ctx->getDefaultVao()->replacementBindEnd();

  ctx->setDefaultShaderVars();
	ctx->drawArrays( GL_TRIANGLES, 0, nverts );
	ctx->popVao();
}


void destroy(void* userPtr)
{
	Context* gl = (Context*)userPtr;
  delete gl;
}

} }
