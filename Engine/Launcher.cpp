/***********************************************************************
Entry point for the engine.
Launches master system, that is responsible for the whole engine managing.

Window minimize by the bar in windows crash or just minimize is somehow related with renderer.

https://www.youtube.com/watch?v=TBe1AVFmKEo

http://freepbr.com/
https://forum.allegorithmic.com/index.php?topic=504.0
https://vk.com/some_itshneg

***********************************************************************/

#include "Engine/MasterSystem.h"

#ifdef _DEBUG
#include <iostream>
#endif

int main()
{
	std::cout << "Engine" << std::endl;
	std::cout << BEngine::ENGINE_NAME <<  std::endl;
	
	BEngine::MasterSystemConstrInfo cInfo
	{
		"config.xml",
	};

	BEngine::CMasterSystem* eMaster = new BEngine::CMasterSystem(cInfo);
	eMaster->initialize();
	delete eMaster;

#ifdef _DEBUG
// 	std::cout << "Shutting down, enter any character.\n";
// 	char x;
// 	std::cin >> x;
#endif
	return 0;
}