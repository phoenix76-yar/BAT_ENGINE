/***********************************************************************
IGAME

Game must define somehow game rules, possibly, 
camera angle, type, and other properties related to the game.

Don't forget that game might consist of game levels ?
How will it tie up with the game level manager ?

Should game class load some config which will define all available game levels,
and other stuff and request the engine to load it and process it ?

Timecyclce is kind of game type isn't it?

***********************************************************************/

#ifndef IGAME_H
#define IGAME_H

namespace BEngine 
{
	namespace BGame
	{	
		class IGame
		{
		public:
			IGame() = default;
			virtual ~IGame() = default;

			virtual void initialize() = 0;
			virtual void update() = 0;
			virtual void shutDown() = 0;

		};
	} //namespace BGame
} //namespace BEngine
#endif //IGAME_H