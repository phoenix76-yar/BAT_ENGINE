/***********************************************************************
RSC Message Types

class CStateVariable can be found in the following file:
"../../../EngineSpecialTypes.h"
***********************************************************************/

#ifndef RSC_MSGS_H
#define RSC_MSGS_H

#include "RSCefinitions.h"
#include "../Level Manager/SectionDefinitions.h"

namespace BEngine
{
	enum class RSCMSGType : uchar
	{
		RSC_LOAD_REQUEST,
		RSC_DESTROY_REQUEST,
	};

	enum class RSCLoadReqMSGType
	{
		ENTITY,
		TEXTURE, //rename somehow different
		SHADER,
		SCRIPT,
	};

	class CEntitityDef;
	class CTextureDictionary;
	class CShaderDictionary;
	class CSriptRecource;
	class CStateVariable;

	class CRSCMSGObject
	{
	public:
		RSCLoadReqMSGType type;
		void* resourceObj;
// 		union
// 		{
// 			CEntitityDef* entityDef;
// 			CTextureDictionary* textureDict;
// 			CShaderDictionary* shaderDict;
// 			CSriptRecource* scriptRes;
// 		};
	};

	class CRSCLoadReqMSG
	{
	public:
		CRSCLoadReqMSG() = default;
		~CRSCLoadReqMSG() = default;

// 		RSCLoadReqMSGType type;
// 
// 
// 		//Current C++ standard does not allow non - POD types inside unions.You will get this compiler error
// 		//but http://stackoverflow.com/questions/1786066/questions-about-vector-union-and-pointers-in-c
// 		bvector<CEntitityDef*> entityDefs; // ****** TODO: To be removed

		//So one workaround would be to not to use CEntityDef* but instead create custom object
		//which would store the RSC type
		bvector<CRSCMSGObject>* rscObjs;

		CStateVariable* notifyObject;
	};


	//-----------------------------------------------------------------------
	//CRSCMSG
	//
	//Base Resource Message
	//-----------------------------------------------------------------------

	class CRSCMSG
	{
	public:
		CRSCMSG() = default;
		~CRSCMSG() = default;

		RSCMSGType msgType;
		struct
		{
			CRSCLoadReqMSG loadReqMSG;
		} rscMSG;
	};
} //namespace BEngine

#endif //RSC_MSGS_H


/*
class CRSCLoadReqMSG
{
public:
CRSCLoadReqMSG() = default;
~CRSCLoadReqMSG() = default;

RSCLoadReqMSGType type;


//Current C++ standard does not allow non - POD types inside unions.You will get this compiler error
//but http://stackoverflow.com/questions/1786066/questions-about-vector-union-and-pointers-in-c
bvector<CEntitityDef*> entityDefs;

//So one workaround would be to not to use CEntityDef* but instead create custom object
//which would store the RSC type
bvector<CRSCMSGObject*> rscObjs;



// 		union
// 		{
// 			struct
// 			{
// 				bvector<CEntitityDef> entityDefs;
// 			};
//
// 			//Some types
// 		};

CNotifyObject* notifyObject;
};
*/
