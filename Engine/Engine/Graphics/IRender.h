/***********************************************************************
IRENDER

Base abstract render class.
***********************************************************************/

#ifndef IRENDER_H
#define IRENDER_H

#include "GraphicsDefinitions.h"
#include "../EngineContainers.h"

static constexpr int DRAW_DIST_MULTIPLIER = 1.5;

namespace BEngine 
{
	namespace BGraphics
	{
		class IShape;
		class ICamera;
		class CLODGroup;
		class CShaderManager;

		class IRender
		{
		public:
			IRender(BGRenderAPIType apiType) { m_renderAPI = apiType; }
			virtual ~IRender() {}

			virtual bool initialize(const BGRenderConstrInfo& cInfo) = 0;
			virtual void shutDown() = 0;

			//Updates static world
			virtual void update() = 0; // ****** TODO: rename to updateStaticWorld() /Shapes()
			//virtual void updateDynamic() = 0;
			//update must be definitely split into two separate methods
			//to update static shapes and to update dynamic

			virtual void render() = 0;
			virtual void resize(uint32 width, uint32 height) = 0;
			// ****** TODO:  do a private general methods swapLODS(blist<IShape*>& sCont);
			//then add two different methods swapLODSForStatic(), swapLODSForDynamic(), instead of one swapLODS,
			//as dynamic and static shapes can be processed in different times !!!
			virtual void swapLODS();
			virtual void processLODGroups();
			virtual void processUnusedShapes(); 
			virtual void sortSolidFrontToBack();
			virtual void sortTransparentBackToFront();

			// ****** TODO: Implement method to determine if a shape is in camera's view frustum and make a flag coordinately!
			//Implement frustum culling basically
			//virtual void checkWhichShapesAreInViewFrustum();

			virtual void* getRenderDevice() const = 0;
			virtual void* getRenderDeviceContext() const = 0;

			BGRenderAPIType getRenderAPIType() const { return m_renderAPI; }

			ICamera* getActiveCamera() { return m_camera; }
			void setActiveCamera(ICamera* cam) { m_camera = cam; }

			virtual void drawElements(BG1PrimitiveTopology, uint32 indexCount, const void* indices = 0) = 0;
			virtual void drawInstanced(BG1PrimitiveTopology, uint32 indexCount, uint32 instanceCount = 1, const void* indices = 0) = 0; 

		public: // ****** NOTE: Add methods.
			virtual void addStaticShape(IShape*);
			virtual void removeStaticShpe(IShape*);

			virtual void addDynamicShape(IShape*);
			virtual void removeDynamicShape(IShape*);


			//virtual void addLODGroup(CLODGroup*);

			// 			virtual void addStaticLight(ILight*);
			// 			virtual void removeStaticLight(ILight*);
		public: // ****** NOTE: Create methods.
			virtual void createShape(IShape**, const PritimiveShapeConstrInfo& cInfo);
// 			BG1GraphicsStatus createBG1Buffer(BG1Buffer**, BG1BufferConstrInfo&);
// 			BG1GraphicsStatus createBG1VertexArray(BG1VertexArray**);
		protected:
			bool m_isInitialized;
			void* m_windowHandle; //HWND
			uint32 m_renderAreaX;
			uint32 m_renderAreaY;
			BGRenderInfo m_renderInfo; // ****** NOTE: as m_drawInfo;

			//Camera related stuff
			ICamera* m_camera;

			CShaderManager* m_shaderManager;

		protected:
			//Those two containers could be combined, 
			//shapes that are dynamic or belong to some groups must be flagged
			blist<IShape*> m_staticShapeCont;
			blist<IShape*> m_dynamicShapeCont;

			blist<IShape*> m_transparentShapeCont;

			blist<IShape*> m_unusedShapeCont; //Shapes out of draw range
		private:
			bool sortBackToFront(const IShape*, const IShape*);
			bool sortFrontToBack(const IShape*, const IShape*);

		private:
			BGRenderAPIType m_renderAPI;
		};
	}
} //namespace BEngine
#endif //IRENDER_H
