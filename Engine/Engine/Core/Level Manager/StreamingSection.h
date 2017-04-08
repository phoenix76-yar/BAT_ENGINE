#ifndef STREAMING_SECTION_H
#define STREAMING_SECTION_H

#include "Section.h"

namespace BEngine
{
	class CStateVariable;

	//-----------------------------------------------------------------------
	//Streaming Section Class
	//
	//Holds a list of entity definitions, and their placements.
	//The main feature of this type of section is that, it can stream it's entities,
	//request RSC manager to load and unload data on the fly.
	//-----------------------------------------------------------------------

	class CStreamingSection : public ISection
	{
	public:
		CStreamingSection(const char* entityDefXML, const char* entityPlacementsXML);
		~CStreamingSection();

		void update() override final;
		void preLoadData() override final;
		void unloadData() override final;

	private:
		void processLoadedRSCs();

		CStateVariable* m_notifyObject;

	};
} //namespace BEngine

#endif //STREAMING_SECTION_H