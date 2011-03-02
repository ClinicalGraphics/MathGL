//***************************************************************************
//
//  Copyright (c) 1999 - 2006 Intel Corporation
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//***************************************************************************

/**
	@file	IFXModifierChain.h

			The header file that defines the IFXModifierChain interface.
*/

#ifndef IFXModifierChain_H
#define IFXModifierChain_H

#include "IFXUnknown.h"

class IFXSubject;
class IFXModifier;
class IFXModifierDataPacket;

// {5D5757C0-B3A7-41f1-BB69-2BCD8D053D82}
IFXDEFINE_GUID(IID_IFXModifierChain,
0x5d5757c0, 0xb3a7, 0x41f1, 0xbb, 0x69, 0x2b, 0xcd, 0x8d, 0x5, 0x3d, 0x82);

#define IFX_E_MODIFIERCHAIN_INCOMPATIBLE_MODIFIER \
		MAKE_IFXRESULT_FAIL( IFXRESULT_COMPONENT_MODIFIERCHAIN, 0X0001)

#define IFX_E_MODIFIERCHAIN_EMPTY \
		MAKE_IFXRESULT_FAIL( IFXRESULT_COMPONENT_MODIFIERCHAIN, 0X0002)

#define IFX_W_MODIFIERCHAIN_NOT_APPENDED \
		MAKE_IFXRESULT_PASS( IFXRESULT_COMPONENT_MODIFIERCHAIN, 0X0003)

#define IFX_E_MODIFIER_CHAIN_NOT_VALID \
		MAKE_IFXRESULT_FAIL( IFXRESULT_COMPONENT_MODIFIERCHAIN, 0X0004)

#define IFX_E_MODIFIERCHAIN_VALIDATION_FAILED \
		MAKE_IFXRESULT_FAIL( IFXRESULT_COMPONENT_MODIFIERCHAIN, 0X0005)


/**
    The ModifierChain interface supplies the services for managing a chain
	of Modifiers and provides access to the resultant DataPacket.

	-	ModifierChain IO                                                     \n
		ModifierChains are not written to U3D files.  ModifierChains are
		re-constructed when Modifiers are loaded.

	-	DataElement propagation                                            \n\n
		DataElements that are generated by a Modifier are forward propagated
		through each Modifier's DataPacket.

	-	DataElement namespaces                                             \n\n
		Due to DataElement propagation, all DataPackets within a
		ModifierChain share a common DataElement name space.               
*/
class IFXModifierChain : virtual public IFXUnknown
{
public:
	enum
	{
		END_OF_MODCHAIN = (U32)-1
	};
	enum
	{
		NODE = 0,
		RESOURCE = 1,
		TEXTURE = 2
	};
	virtual IFXRESULT IFXAPI  Initialize() = 0;
	/**<
	        Initializes the ModifierChain.

	@pre    None; re-initialization is supported.

	@post   The chain is empty.

	@return One of the following IFXRESULT codes:                          \n\n
			-	IFX_OK                                                       \n
				No error.                                                  \n\n
			-	IFX_E_OUT_OF_MEMORY                                          \n
				Not enough memory was available to initialize.               */

	virtual IFXRESULT IFXAPI PrependModifierChain(
									IFXModifierChain* pInModifierChain ) = 0;
	/**<
			Effectively prepends the provided @e pInModifierChain.

	@param	pInModifierChain
			A pointer to the incoming ModifierChain that will precede the
			ModifierChain. May be Null, to imply that there should be no
			prepended chain after this call succeeds.

	@pre    The first Modifier in the ModifierChain is compatible with the
	        resultant DataPacket of @e pInModifierChain.

	@post   The resultant DataPacket contents from @e pInModifierChain is
	        treated as input to the first Modifier in the ModifierChain.

	@return One of the following IFXRESULT codes:                          \n\n
			-	IFX_OK                                                       \n
				No error.                                                  \n\n
			-	IFX_E_MODIFIERCHAIN_INCOMPATIBLE_MODIFIER                    \n
				The specified ModifierChain was NOT prepended because the
				resultant DataPacket at the end of the specified chain did not
				satisfy the input requirements of the first Modifier.      \n\n
			-	IFX_E_OUT_OF_MEMORY                                          \n
				The ModifierChain was NOT prepended because not enough memory 
				was available.                                             \n\n
			-	IFX_E_NOT_INITIALIZED                                        \n
				The ModifierChain was NOT prepended because the ModifierChain
				was not initialized properly.                                */


	virtual IFXRESULT IFXAPI  AddModifier( 
									IFXModifier& rInModifier,
									U32 index = END_OF_MODCHAIN, 
									BOOL isReqValidation = TRUE ) = 0;
	/**<
	        Appends the incoming Modifier at the specified index.


	@param  rInModifier
			The incoming Modifier that will be added.

	@param	index
			The index where the modifier is to be inserted defaults to the
			end of the modifier chain. Otherwise the parameter must be 
			< Number of modifiers in the modifier chain. If @e isReqValidation is
			FALSE, then the index can be past the end of the chain and
			NULL modifier entries will be inserted as place holders. If the
			current modifier at the index is NULL, the entry will be replaced
			with the new modifier.

  	@param  isReqValidation
			Defines if incomming Modifier requires validation or no. If this is TRUE,
			the modifier chain will return an error code. This occurs when a modifier 
			cannot be enabled or if any other modifier moves to a disabled state.
			If this parameter is FALSE then the operation will succeed unless
			a modifier in the chain returns an error code during the validation
			process.

	@pre    The dependencies of @e rInModifier is satisfied by the resultant
            ModifierChain DataPacket.

	@return One of the following IFXRESULT codes:                          \n\n
			-	IFX_OK														 \n
				No error.                                                  \n\n
			-	IFX_E_MODIFIERCHAIN_INCOMPATIBLE_MODIFIER                    \n
				The specified Modifier was NOT added because the resultant
				DataPacket at the end of the chain did not satisfy the
				input requirements specified by rInModifier.               \n\n
			-	IFX_E_OUT_OF_MEMORY                                          \n
				The Modifier was not appended because not enough memory was
				available.                                                   */


	virtual IFXRESULT IFXAPI  SetModifier( 
									IFXModifier& rInModifier,
									U32 index,
									BOOL isReqValidation = TRUE ) = 0;
	/**<
			Replaces a modifier at a given index.

	@param	rInModifier
			The incoming Modifier that will be added.

	@param	index
			The index  where the modifier is to be added. If
			@e isReqValidation is TRUE, the parameter must be
			<= Number of modifiers in the modifier chain. If @e isReqValidation 
			is FALSE then the index can be past the end of the chain and
			NULL modifier entries will be inserted as place holders.

  	@param  isReqValidation
			Defines if incomming Modifier requires validation or no. If this is TRUE,
			the modifier chain will return an error code. This occurs when a modifier 
			cannot be enabled or if any other modifier moves to a disabled state.
			If this parameter is FALSE then the operation will succeed unless
			a modifier in the chain returns an error code during the validation
			process.

	@pre	The dependencies of @e rInModifier is satisfied by the resultant
			ModifierChain DataPacket.

	@return One of the following IFXRESULT codes:                          \n\n
			-	FX_OK                                                        \n
				No error.                                                  \n\n
			-	IFX_E_MODIFIERCHAIN_INCOMPATIBLE_MODIFIER                    \n
				The specified Modifier was NOT added because the resultant
				DataPacket at the end of the chain did not satisfy the
				input requirements specified by pInModifier.               \n\n
			-	IFX_E_OUT_OF_MEMORY                                          \n
				The Modifier was not appended because not enough memory was
				available.                                                   */


	virtual IFXRESULT IFXAPI RemoveModifier( U32 index = END_OF_MODCHAIN ) = 0;
	/**<
	        Removes the modifier from the modifier chain.

	@param  index
			The index of the modifier to be removed, defaults to end of modifier
			chain.

	@pre    There are one or more Modifiers in the chain.

	@return One of the following IFXRESULT codes:                          \n\n
			-	IFX_OK                                                       \n
				The Modifier was successfully removed.                     \n\n
			-	IFX_E_MODIFIERCHAIN_EMPTY                                    \n
				There are no Modifiers in the ModifierChain.               \n\n
			-	IFX_E_NOT_INITIALIZED                                        \n
				The ModifierChain was not initialized properly.              */


	virtual IFXRESULT IFXAPI GetDataPacket( 
								IFXModifierDataPacket*& rpOutDataPacket ) = 0;
	/**<
			Hands back the ModifierChain's resultant DataPacket.

	@param	rpOutDataPacket
			The DataPacket pointer to contain the address of the
			ModifierChain's resultant DataPacket.

	@return One of the following IFXRESULT codes:                          \n\n
			-	IFX_OK                                                       \n
				No error.                                                  \n\n
			-	IFX_E_NOT_INITIALIZED                                        \n
				The ModifierChain was not initialized properly.              */



	virtual IFXRESULT IFXAPI  GetModifierCount( U32& ruOutModifierCount )=0;
	/**<
	        Hands back the number of Modifiers in the chain.

	@param	rOutModifierCount
			The U32 to contain the number of Modifiers in the ModifierChain.

	@return Returns IFX_OK if no errors occurs.
	*/


	virtual IFXRESULT IFXAPI  GetModifier( U32 index, IFXModifier*& rpOutModifier ) = 0;
	/**<
	        Hands back a pointer to a specified Modifier in the ModifierChain.

	@param  index
	         A U32 specifying the index of the Modifier.

	@param  rpOutModifier
	         A IFXModifier pointer to contain the address of the Modifier.

	@return One of the following IFXRESULT codes:                          \n\n
			-	IFX_OK                                                       \n
				No error.                                                  \n\n
			-	IFX_E_INVALID_RANGE                                          \n
				The specified Modifier does not exist.                     \n\n
			-	IFX_E_NOT_INITIALIZED                                        \n
				The ModifierChain was not initialized properly.              */


  	virtual IFXRESULT IFXAPI SetClock( IFXSubject* pInClockSubject ) = 0;
	/**<
	        Passes in the simulation clock into the modifier chain.

	@param	pInClockSubject
			The IFXClock which contains the simulation time data.

	@return One of the following IFXRESULT codes:                          \n\n
			-	IFX_OK                                                       \n
				No error.                                                  \n\n
	*/

};

#endif
