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
//	CLASS:	CLOTH_BUILDER
//
//	02-02-27:	JLT - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Physic/Cloth/ClothBuilder)

//#include	INC_KRNSERV(Physic/Lattice)
//#include	INC_KRNSERV(Physic/Spring)
//#include	INC_KRNSERV(Physic/Particle)
//#include	INC_KRNSERV(Physic/Section)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CLOTH_BUILDER constructor
//	Object:		
//	02-02-27:	JLT - Created
//-----------------------------------------------------------------------------
CLOTH_BUILDER::CLOTH_BUILDER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CLOTH_BUILDER destructor
//	Object:		
//	02-02-27:	JLT - Created
//-----------------------------------------------------------------------------
CLOTH_BUILDER::~CLOTH_BUILDER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	02-02-27:	JLT - Created
//-----------------------------------------------------------------------------
p_bool CLOTH_BUILDER::Build(LATTICE *	pLattice,
							int			nNbrLayers)
{
	p_bool bResult = p_false;

	if (!pLattice)
		return false;

	if	(nNbrLayers >= 1)
	{
		bResult |= BuildStretch(pLattice);
	}

	if	(nNbrLayers >= 2)
	{
		bResult |= BuildShear(pLattice);
	}

	if	(nNbrLayers >= 3)
	{
		bResult |= BuildBend(pLattice);
	}

	return bResult;
}

//-----------------------------------------------------------------------------
//	Name:		BuildStretch
//	Object:		
//	02-02-28:	JLT - Created
//-----------------------------------------------------------------------------
p_bool CLOTH_BUILDER::BuildStretch(LATTICE *pLattice)
{
	p_uint32 i, j, SectionCount, ParticleCount;
	DYN_SECTION *S1, *S2;
	PHYSIC_PARTICLE *P1, *P2;
	SPRING *Spring;
	
	float	rStiffness = 75.0f;
	float	rDamping = 25.0f;

	if (!pLattice)
		return p_false;
	
	SectionCount = pLattice->GetDynSectionCount();
	for (i = 0 ; i < SectionCount ; ++i)
	{
		S1 = pLattice->GetDynSection(i);
		QDT_ASSERT(S1);
		
		ParticleCount = S1->GetParticleCount();
		for (j = 0 ; j < ParticleCount - 1 ; ++j)
		{
			P1 = S1->GetParticle(j + 0);
			P2 = S1->GetParticle(j + 1);

			QDT_ASSERT(P1 && P2);

			if (P1->IsAnchor() && P2->IsAnchor())
				continue;

			Spring = (SPRING*)pLattice->CreateForce(LATTICE::SPRING_FORCE, 0);

			QDT_ASSERT(Spring);

			Spring->Attach(P1, P2);
			Spring->SetStiffness(rStiffness);
			Spring->SetDamping(rDamping);

			Spring->SetIKType(SPRING::IK_MAX, SPRING::IK_MAX);
			Spring->SetIKType(SPRING::IK_MIN, 0);
		}	
	}

	for (i = 0 ; i < SectionCount - 1 ; ++i)
	{
		S1 = pLattice->GetDynSection(i + 0);
		S2 = pLattice->GetDynSection(i + 1);

		if (!S1 || !S2)
			continue;

		if (S1->GetParticleCount() != S2->GetParticleCount())
			continue;

		ParticleCount = S1->GetParticleCount();
		for (j = 0 ; j < ParticleCount ; ++j)
		{
			P1 = S1->GetParticle(j);
			P2 = S2->GetParticle(j);
			
			QDT_ASSERT(P1 && P2);

			if (P1->IsAnchor() && P2->IsAnchor())
				continue;
			
			Spring = (SPRING*)pLattice->CreateForce(LATTICE::SPRING_FORCE, 0);
			
			QDT_ASSERT(Spring);
			
			Spring->Attach(P1, P2);
			Spring->SetStiffness(rStiffness);
			Spring->SetDamping(rDamping);

			Spring->SetIKType(SPRING::IK_MAX, SPRING::IK_MAX);
			Spring->SetIKType(SPRING::IK_MIN, 0);
		}
	}
	
	return p_true;
}

//-----------------------------------------------------------------------------
//	Name:		BuildShear
//	Object:		
//	02-02-28:	JLT - Created
//-----------------------------------------------------------------------------
p_bool CLOTH_BUILDER::BuildShear(LATTICE *pLattice)
{
	p_uint32 i, j, SectionCount, ParticleCount;
	DYN_SECTION *S1, *S2;
	PHYSIC_PARTICLE *P1, *P2;
	SPRING *Spring;

	float	rStiffness = 200.0f;
	float	rDamping = 50.0f;

	if (!pLattice)
		return p_false;

	SectionCount = pLattice->GetDynSectionCount();
	for (i = 0 ; i < SectionCount - 1 ; ++i)
	{
		S1 = pLattice->GetDynSection(i + 0);
		S2 = pLattice->GetDynSection(i + 1);

		if (!S1 || !S2)
			continue;

		if (S1->GetParticleCount() != S2->GetParticleCount())
			continue;

		ParticleCount = S1->GetParticleCount();
		for (j = 0 ; j < ParticleCount - 1 ; ++j)
		{
			P1 = S1->GetParticle(j + 0);
			P2 = S2->GetParticle(j + 1);
			
			QDT_ASSERT(P1 && P2);

			if (P1->IsAnchor() && P2->IsAnchor())
				continue;
			
			Spring = (SPRING*)pLattice->CreateForce(LATTICE::SPRING_FORCE, 1);
			
			QDT_ASSERT(Spring);
			
			Spring->Attach(P1, P2);
			Spring->SetStiffness(rStiffness);
			Spring->SetDamping(rDamping);

			Spring->SetIKType(SPRING::IK_MAX, SPRING::IK_MAX);
			Spring->SetIKType(SPRING::IK_MIN, 0);
		}
	}

	for (i = 0 ; i < SectionCount - 1 ; ++i)
	{
		S1 = pLattice->GetDynSection((SectionCount - 1) - i - 0);
		S2 = pLattice->GetDynSection((SectionCount - 1) - i - 1);

		if (!S1 || !S2)
			continue;

		if (S1->GetParticleCount() != S2->GetParticleCount())
			continue;

		ParticleCount = S1->GetParticleCount();
		for (j = 0 ; j < ParticleCount - 1 ; ++j)
		{
			P1 = S1->GetParticle(j + 0);
			P2 = S2->GetParticle(j + 1);
			
			QDT_ASSERT(P1 && P2);

			if (P1->IsAnchor() && P2->IsAnchor())
				continue;
			
			Spring = (SPRING*)pLattice->CreateForce(LATTICE::SPRING_FORCE, 1);
			
			QDT_ASSERT(Spring);
			
			Spring->Attach(P1, P2);
			Spring->SetStiffness(rStiffness);
			Spring->SetDamping(rDamping);

			Spring->SetIKType(SPRING::IK_MAX, SPRING::IK_MAX);
			Spring->SetIKType(SPRING::IK_MIN, 0);
		}
	}

	return p_true;
}

//-----------------------------------------------------------------------------
//	Name:		BuildBend
//	Object:		
//	02-02-28:	JLT - Created
//-----------------------------------------------------------------------------
p_bool CLOTH_BUILDER::BuildBend(LATTICE *pLattice)
{
	p_uint32 i, j, SectionCount, ParticleCount;
	DYN_SECTION *S1, *S2, *S3;
	PHYSIC_PARTICLE *P1, *P2, *P3;
	SPRING *Spring;

	float	rStiffness = 200.0f;
	float	rDamping = 50.0f;

	if (!pLattice)
		return p_false;

	SectionCount = pLattice->GetDynSectionCount();
	for (i = 0 ; i < SectionCount ; ++i)
	{
		S1 = pLattice->GetDynSection(i);
		QDT_ASSERT(S1);
		
		ParticleCount = S1->GetParticleCount();
		for (j = 0 ; j < ParticleCount - 2 ; ++j)
		{
			P1 = S1->GetParticle(j + 0);
			P2 = S1->GetParticle(j + 1);
			P3 = S1->GetParticle(j + 2);
			
			QDT_ASSERT(P1 && P2 && P3);
			
			if (P1->IsAnchor() && P2->IsAnchor() && P3->IsAnchor())
				continue;
			
			Spring = (SPRING*)pLattice->CreateForce(LATTICE::SPRING_FORCE, 2);
			
			QDT_ASSERT(Spring);
			
			Spring->Attach(P1, P3);
			Spring->SetStiffness(rStiffness);
			Spring->SetDamping(rDamping);

			Spring->SetIKType(SPRING::IK_MAX, SPRING::IK_MAX);
			Spring->SetIKType(SPRING::IK_MIN, 0);
		}
	}
	
	for (i = 0 ; i < SectionCount - 2 ; ++i)
	{
		S1 = pLattice->GetDynSection(i + 0);
		S2 = pLattice->GetDynSection(i + 1);
		S3 = pLattice->GetDynSection(i + 2);
		
		if (!S1 || !S2 || !S3)
			continue;
		
		if (S1->GetParticleCount() != S3->GetParticleCount())
			continue;
		
		ParticleCount = S1->GetParticleCount();
		for (j = 0 ; j < ParticleCount ; ++j)
		{
			P1 = S1->GetParticle(j);
			P2 = S2->GetParticle(j);
			P3 = S3->GetParticle(j);
			
			QDT_ASSERT(P1 && P2 && P3);
			
			if (P1->IsAnchor() && P2->IsAnchor() && P3->IsAnchor())
				continue;
			
			Spring = (SPRING*)pLattice->CreateForce(LATTICE::SPRING_FORCE, 2);
			
			QDT_ASSERT(Spring);
			
			Spring->Attach(P1, P3);
			Spring->SetStiffness(rStiffness);
			Spring->SetDamping(rDamping);

			Spring->SetIKType(SPRING::IK_MAX, SPRING::IK_MAX);
			Spring->SetIKType(SPRING::IK_MIN, 0);
		}
	}
	
	return p_true;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
