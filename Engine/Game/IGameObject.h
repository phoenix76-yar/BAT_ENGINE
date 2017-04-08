#ifndef IGAME_OBJ_H
#define IGAME_OBJ_H

namespace BEngine
{
	namespace BGame
	{
		//-----------------------------------------------------------------------
		//Based on game objects, triggers, AI, player controller and other stuff
		//could be implemented.
		//Do more research.
		//-----------------------------------------------------------------------

		// ****** NOTE: Prototype
		class IGameObject
		{
		public:
			IGameObject() = default;
			virtual ~IGameObject() = default;

			virtual void update() = 0;
		};

		//-----------------------------------------------------------------------
		//Trig Event Info
		//
		//Structure that could potentially contain information,
		//at what place it happened, time, etc.
		//-----------------------------------------------------------------------

		struct TrigEventInfo
		{

		};

		//-----------------------------------------------------------------------
		//ITrigger
		//
		//Prototype ITrigger class
		//-----------------------------------------------------------------------

		class ITrigger : public IGameObject
		{
		public:
			ITrigger() = default;
			virtual ~ITrigger() = default;

			virtual void update() = 0;

			//Probably the method passed for callback will need to be a static method (needs checking)
			void registerTriggerCallback(void(*triggerEvent)(IGameObject*, const TrigEventInfo&))
			{
				if (triggerEvent != nullptr)
				{
					m_triggerEvent = triggerEvent;
				}
			}

			virtual void trigEvent(IGameObject* gameObj, const TrigEventInfo& tInfo)
			{
				if (m_triggerEvent != nullptr)
				{
					m_triggerEvent(gameObj, tInfo);
				}
			}

		private:
			void(*m_triggerEvent)(IGameObject*, const TrigEventInfo&);
		};
	} //namespace BGame
} //namespace BEngine
#endif //IGAME_OBJ_H
