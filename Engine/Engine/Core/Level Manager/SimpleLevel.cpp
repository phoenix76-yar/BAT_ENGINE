#include "SimpleLevel.h"
#include "StreamingSection.h"

#include <iostream>

BEngine::CSimpleLevel::CSimpleLevel()
{
	ISection* streamSect = new CStreamingSection("C:\\Users\\akaln\\Documents\\Visual Studio 2015\\Projects\\Engine\\Data\\World\\part_1_type.xml",
			"C:\\Users\\akaln\\Documents\\Visual Studio 2015\\Projects\\Engine\\Data\\World\\part_1_map.xml");
	m_levelSections.push_back(streamSect);
}

void BEngine::CSimpleLevel::loadLevel(const LevelUpdateInfo& lInfo)
{
	for (auto section : m_levelSections)
	{
		section->preLoadData();
	}
}

void BEngine::CSimpleLevel::reload()
{

}

void BEngine::CSimpleLevel::update(const LevelUpdateInfo& lInfo)
{
	for (auto section : m_levelSections)
	{
		section->update();
	}
}

void BEngine::CSimpleLevel::shutDown()
{
	if (!m_levelSections.empty())
	{
		while (!m_levelSections.empty())
		{
			if (m_levelSections.back())
			{
				m_levelSections.back()->unloadData();
				delete m_levelSections.back();
			}
			m_levelSections.pop_back();
		}
	}
}
