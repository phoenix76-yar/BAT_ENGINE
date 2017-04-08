/***********************************************************************
OpenGLX Render

Physics manager will send a message that dynamic shape graphical
representing shapes can be updated when physics calculation will be finished.
If renderer will have a bit of time, it can wait (recalculate some stuff)
if it can't wait anymore, it will update shape transformation buffers on next call.

For IShape class it will be necessary to make another method to set shape's world transformation
setShapeWorldTransformationSafe(mat4x4), this method will be used by physics manager
to set a new transformation for the shape.
Renderer before update will get the new transformation via call:
getShapeWorldTransformationSafe() const, these two method will be synchronised via spinlock.


***********************************************************************/

#ifndef CGLXRENDER_H
#define CGLXRENDER_H

#define GLEW_STATIC

#include "IRender.h"

namespace BEngine 
{
	namespace BGraphics
	{
		// ****** NOTE: 64 is one matrix size in bytes.
		static constexpr uint32	CAMERA_BUFFER_SIZE = 64 * 4;

		class BG1GLXBuffer;
		class IShape;

		class CGLXRender : public IRender
		{
		public:
			CGLXRender();
			~CGLXRender();

			bool initialize(const BGRenderConstrInfo& cInfo) override final;
			void shutDown() override;

			void update() override;
			void render() override;
			void resize(uint32 width, uint32 height) override;

			void* getRenderDevice() const override final { return m_hangleDeviceContext; }
			void* getRenderDeviceContext() const override final { return m_handleRenderingContext; }

			virtual void drawElements(BG1PrimitiveTopology, uint32 indexCount, const void* indices = 0) override final;
			virtual void drawInstanced(BG1PrimitiveTopology, uint32 indexCount, uint32 instanceCount = 1, const void* indices = 0) override final;
		private:
			void* m_hangleDeviceContext; //HDC
			void* m_handleRenderingContext; //HGLRC, 

			//Camera related stuff
			BG1GLXBuffer*	m_camUniformBuffer; //Updated once per frame
			uchar			m_camRamBuffer[CAMERA_BUFFER_SIZE];		
		};
	} //namespace BGraphics	
} //namespace BEngine
#endif //CGLXRENDER_H
