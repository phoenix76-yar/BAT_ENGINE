/***********************************************************************
Audio MSGS

All possible audio message objects are defined here.
***********************************************************************/

#ifndef AUDIO_MSGS_H
#define AUDIO_MSGS_H

#include "../EngineDefinitions.h"

namespace BEngine
{
	namespace BAudio
	{
		enum class AudioPlayMode : uchar
		{
			LOOP,
		};

		class CAudioPlayMSG
		{
		public:
			AudioPlayMode playMode;
			//Some sound identifier
			uint32 soundHash;
		};

		class CAudioPlaySreamableMSG
		{
		public:

		};

		class CAudioAdd2DSound
		{
		public:

		};

		class CAudioAdd3DSound
		{
		public:
			//vec3 position;
			//other parameters ...
		};

		enum class AudioMSGType : uchar
		{
			PLAY_SOUND,
			//PLAY_STREAMABLE_SOUND,
			ADD_2D_SOUND,
			ADD_3D_SOUND,
		};

		class CAudioMSG
		{
		public:
			CAudioMSG() = default;
			~CAudioMSG() = default;

			AudioMSGType msgType;
			union
			{
				CAudioPlayMSG playSoundMSG;
			} audioMSG;
		};

	} //namespace BAudio
} //namespace BEngine

#endif //AUDIO_MSGS_H
