//*****************************************************************************
//
// Copyright (C) 2000-2005 Quantic Dream SA
//
// These coded instructions, statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are 
// protected by French and EEC copyright laws. They may not be 
// disclosed to third parties or copied or duplicated, in whole or in part, 
// without prior written consent of Quantic Dream SA
//
// Unpublished-rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
//	CLASS:	TRIGGER
//	The TRIGGER class implements ...
//
//	02-03-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_TRIGGER_H__
#define __M3D_SCENEGRAPH_ENTITIES_TRIGGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(OBB)
DECL_KRENDERER(I_VERTEX_BUFFER)
DECL_KRENDERER(I_VERTEX_DECLARATION)
DECL_KRENDERER(S_VERTEX_ELEMENT)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron/Hexahedron)
#include	INCL_KMATH(Math/Color/Color)

namespace QDT
{
	namespace M3D
	{
		class	TRIGGER : public ENTITY_ABC
		{
		public:

			enum
			{
				CMD_TRGR_SET_COLOR = CMD_ENTITY_LAST,
				CMD_TRGR_LAST,
			};

			enum	TRIGGER_ACTION
			{
				TA_OUTSIDE,					// The entity remains outside.
				TA_INSIDE_MATCH,			// The entity remains inside with a matching direction.
				TA_INSIDE_NO_MATCH,			// The entity remains inside with but does not match direction.
				TA_ENTRY_MATCH_DIR,			// The entity enters the zone with the right direction.
				TA_ENTRY_NO_MATCH_DIR,		// The entity enters the zone but directions don't match.
				TA_INSIDE_GAIN_MATCH_DIR,	// The entity is inside and now matches the direction.
				TA_INSIDE_LOSE_MATCH_DIR,	// The entity was inside and losses the direction.
				TA_EXIT_MATCH_DIR,			// The entity exits the zone with maching directions.
				TA_EXIT_NO_MATCH_DIR,		// The entity exits the zone with no maching directions.
			};

			// Constructor & destructor
										TRIGGER();
			virtual						 ~TRIGGER();

			virtual void				Register();
			virtual void				Unregister();

			void						AddHexahedron(const HEXAHEDRON & Hexahedron);
			void						AddHexahedron(const HEXAHEDRON & Hexahedron, const COLOR & Color);
			void						RemoveHexahedron(const int nHexahedronIndex);
			int							GetNbrHexahedrons() const;
			const HEXAHEDRON &			GetHexahedron(const int nIndex) const;
			const COLOR &				GetColor(const int nIndex) const;
			void						SetHexahedron(const int nIndex, const HEXAHEDRON & Hexahedron);
			void						SetColor(const int nIndex, const COLOR & Color);
			void						SetColor(const COLOR & Color);
			void						ClearHexahedrons();

			TRIGGER_ACTION				Update(NODE * pEntityNode, const AABB & Box, float rAngle);
			bool						Intersect(const AABB & Box) const;

			bool						GetActive() const;
			void						SetActive(const bool bActive);

			void						SetBoundingSphere(const SPHERE & S) { _BoundingSphere = S; }
			const SPHERE &				GetBoundingSphere() const			{ return (_BoundingSphere); }
										
			void						Clean();
										
			void						SetFromObb(const OBB & Obb);
										
			void						Copy(const TRIGGER & Trigger);
										
			void						Transform(const MATRIX & Matrix);
										
			void						SetDirection(const float rAngle);
			float						GetDirection() const;
			void						SetAperture(const float rAngle);
			float						GetAperture() const;
										
			bool						GetCenter(VECTOR & Center) const;

			// Derivation from ENTITY_ABC
			virtual void				Display();

			// Communication
			static  void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static  COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static  COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

					bool						Intersect(const VECTOR& EntityPos) const;
					void						RemoveInside(NODE * pNode);
		private:								
												
			enum	PREVIOUS_STATE				
			{									
				PS_OUTSIDE,						
				PS_INSIDE_MATCH,				
				PS_INSIDE_NO_MATCH,				
			};									
												
					void						FillVertexBuffer(const HEXAHEDRON & Hexahedron, const COLOR_FLOAT & Color, Float32 rGlobalAngle, I_VERTEX_BUFFER * pVertexBuffer);
												
					void						AddInside(NODE * pNode, const bool bMatchDir);
					void						SetInsideMatch(NODE * pNode, const bool bMatchDir);
					PREVIOUS_STATE				GetPreviousState(NODE * pNode);
												
			static	bool						DirectionsMatch(float rAngle1, float rAngle2, float rAperture);

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetColor(void * const pO, void * const pR, void * const pP);
		#ifndef _MASTER_ONLY_DATABANK
			static	void						CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void						CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static	void						CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

			struct	COLOR_HEXAHEDRON
			{
				COLOR_HEXAHEDRON();
				~COLOR_HEXAHEDRON();

				COLOR				_Color;
				HEXAHEDRON			_Hexahedron;
				I_VERTEX_BUFFER *	_pVertexBuffer;
			};

			struct	INSIDE_NODE
			{
				bool	_bMatchDir;
				NODE *	_pNode;
			};

			struct	S_TRIG_VERTEX
			{
				Float32 _rVx;
				Float32 _rVy;
				Float32 _rVz; 
				UInt32	_cDiffuse;
			};

			static	const COLOR	ENTER_DIRECTION_COLOR;
			static	const COLOR	ENTER_COLOR;
			static	const COLOR	ACTIVE_COLOR;
			static	const COLOR	DEFAULT_COLOR;
			
			static S_VERTEX_ELEMENT			_aVertexElts[2];
			static I_VERTEX_DECLARATION *	_pVertexDeclaration;

			QDT_VECTOR<COLOR_HEXAHEDRON>	_vHexahedrons;
			QDT_VECTOR<INSIDE_NODE>			_vInsideNodes;
			SPHERE							_BoundingSphere;

			float							_rDirection;
			float							_rAperture;
			bool							_bActive;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_TRIGGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
