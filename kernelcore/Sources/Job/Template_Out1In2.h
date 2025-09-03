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
//	06-01-12:	SBE - Created
//*****************************************************************************

#ifndef _TEMPLATE_OUT1_IN2_H_
#define _TEMPLATE_OUT1_IN2_H_

#ifdef PLATFORM_PS3 // todo
	#include <stdint.h>
	struct JOB_PARAM_OUT1_IN2
	{
		uint64_t		pOut __attribute__ ((aligned(16)));
		uint64_t		pIn1;
		uint64_t		pIn2;
		unsigned int	nCount;
	}  __attribute__ ((aligned(16)));
#endif

#if defined(PLATFORM_PS3)
#define DECL_MAINJOB(JOB_NAME)									\
extern char _binary_ ## JOB_NAME ## _elf_start[];				\
void	JOB_NAME(void *			pOut,							\
				const void *	pIn1,							\
				const void *	pIn2,							\
				unsigned int	nCount);							
#else
#define DECL_MAINJOB(JOB_NAME)									\
void	JOB_NAME(void *			pOut,							\
				const void *	pIn1,							\
				const void *	pIn2,							\
				unsigned int	nCount);							
#endif

#ifdef MAIN_LOOP
#define myCREATE_MAINJOB(JOB_NAME, OUT_TYPE, IN1_TYPE, IN2_TYPE)	\
void	JOB_NAME(void *			pO,									\
				const void *	pI1,								\
				const void *	pI2,								\
				unsigned int	nCount)								\
{																	\
		OUT_TYPE *			pOut = (OUT_TYPE *) pO;					\
		const IN1_TYPE *	pIn1 = (const IN1_TYPE *) pI1;			\
		const IN2_TYPE *	pIn2 = (const IN2_TYPE *) pI2;			\
		for (unsigned int i=0; i<nCount;)							\
		{															\
			MAIN_LOOP												\
		}															\
}
#else
#define myCREATE_MAINJOB(JOB_NAME, OUT_TYPE, IN1_TYPE, IN2_TYPE)	\
void	JOB_NAME(void *			pO,									\
				const void *	pI1,								\
				const void *	pI2,								\
				unsigned int	nCount);
#endif

#if defined(PLATFORM_PS3) && defined(SPU)

	#include <stdint.h>
	#include <sys/spu_thread.h>
	#include <sys/spu_event.h>
	#include <cell/dma.h>
	#include <cell/atomic.h>
	#include <cell/spurs/types.h>
	#include <cell/spurs/task.h>
	#include <spu_printf.h>

	#define DMA_UPLOAD_TAG(xfer) (2 * xfer + 1)
	#define DMA_UPLOAD_MASK(xfer) (1 << DMA_UPLOAD_TAG(xfer))
	#define DMA_DOWNLOAD_TAG(xfer) (2 * (xfer + 1))
	#define DMA_DOWNLOAD_MASK(xfer) (1 << DMA_DOWNLOAD_TAG(xfer))

	#undef spu_printf
	#define spu_printf if(0)

	uint32_t buf[128/sizeof(uint32_t)] __attribute__((aligned(128)));
	JOB_PARAM_OUT1_IN2 jobParams __attribute__ ((aligned(128)));

	#define CREATE_MAINJOB(JOB_NAME, OUT_TYPE, IN1_TYPE, IN2_TYPE)																				\
		CREATE_OUT_STATIC_ARRAY(OUT_TYPE)																										\
		CREATE_IN_STATIC_ARRAY(1, IN1_TYPE)																										\
		CREATE_IN_STATIC_ARRAY(2, IN2_TYPE)																										\
		myCREATE_MAINJOB(JOB_NAME, OUT_TYPE, IN1_TYPE, IN2_TYPE)																				\
																																				\
		void cellSpursMain(qword argTask, uint64_t argTaskset)																					\
		{																																		\
			spu_printf("SPU start\n");																											\
																																				\
			uint64_t atomicPtr = argTaskset;																									\
			uint64_t dmaStart = spu_extract((vec_ullong2)argTask, 0);																			\
			int taskId = spu_extract((vec_ullong2)argTask, 1);																					\
																																				\
			spu_printf("dmaStart: %X\n", (int)dmaStart);																						\
																																				\
			cellDmaLargeGet(&jobParams, dmaStart, sizeof(jobParams), DMA_DOWNLOAD_TAG(1), 0, 0);												\
			cellDmaWaitTagStatusAll(DMA_DOWNLOAD_MASK(1));																						\
																																				\
			spu_printf("Count: %d, CountByLoop: %d\n", jobParams.nCount, (COUNT_BY_LOOP));														\
																																				\
			int numPass = jobParams.nCount / (COUNT_BY_LOOP);																					\
			int dataRemaining = jobParams.nCount % (COUNT_BY_LOOP);																				\
																																				\
			spu_printf("numPass = %d, dataRemaining: %d\n", (int)numPass, (int)dataRemaining);													\
																																				\
			uint64_t currentInput1 = (uint64_t)jobParams.pIn1;																					\
			uint64_t currentInput2 = (uint64_t)jobParams.pIn2;																					\
			uint64_t currentOutput = (uint64_t)jobParams.pOut;																					\
																																				\
			int previousUsedBuffer = 1;																											\
			int currentUsedBuffer = 0;																											\
			int nextUsedBuffer = 1;																												\
																																				\
			if(!numPass)																														\
			{																																	\
				spu_printf("Start transfert of first input %X %X (last)\n", (int)currentInput1, (int)currentInput2);							\
				spu_printf("Destination: %X %X\n", (int)g_pIn1[0], (int)g_pIn2[0]);																\
				cellDmaLargeGet(g_pIn1[0], currentInput1, (dataRemaining)*sizeof(IN1_TYPE), DMA_DOWNLOAD_TAG(1), 0, 0);							\
				cellDmaLargeGet(g_pIn2[0], currentInput2, (dataRemaining)*sizeof(IN2_TYPE), DMA_DOWNLOAD_TAG(2), 0, 0);							\
			}																																	\
			else																																\
			{																																	\
				/* amorce first data process */																									\
				spu_printf("Start transfert of first input %X %X\n", (int)currentInput1, (int)currentInput2);									\
				spu_printf("Destination: %X %X\n", (int)g_pIn1[0], (int)g_pIn2[0]);																\
				cellDmaLargeGet(g_pIn1[0], currentInput1, (COUNT_BY_LOOP)*sizeof(IN1_TYPE), DMA_DOWNLOAD_TAG(1), 0, 0);							\
				cellDmaLargeGet(g_pIn2[0], currentInput2, (COUNT_BY_LOOP)*sizeof(IN2_TYPE), DMA_DOWNLOAD_TAG(2), 0, 0);							\
				currentInput1 += (COUNT_BY_LOOP)*sizeof(IN1_TYPE);																				\
				currentInput2 += (COUNT_BY_LOOP)*sizeof(IN2_TYPE);																				\
																																				\
				int i;																															\
				for(i=0;i<numPass;i++)																											\
				{																																\
					spu_printf("---> Starting pass %d -----------------------------------\n", i);												\
					if(i>0)																														\
					{																															\
						spu_printf("Waiting for output completion\n");																			\
						/* check data output is complete */																						\
						cellDmaWaitTagStatusAll(DMA_UPLOAD_MASK(1));																			\
					}																															\
																																				\
					spu_printf("Waiting for input completion\n");																				\
					/* check data input is complete */																							\
					cellDmaWaitTagStatusAll(DMA_DOWNLOAD_MASK(1) | DMA_DOWNLOAD_MASK(2));														\
																																				\
					/* start transfert of next */																								\
					if(i>0)																														\
					{																															\
						spu_printf("Start transfert of previous output\n");																		\
						cellDmaLargePut(g_pOut[previousUsedBuffer], currentOutput, (COUNT_BY_LOOP)*sizeof(OUT_TYPE), DMA_UPLOAD_TAG(1), 0, 0);	\
						currentOutput += (COUNT_BY_LOOP)*sizeof(OUT_TYPE);																		\
					}																															\
					if((i<numPass) || dataRemaining)																							\
					{																															\
						spu_printf("Start transfert of next input %X %X\n", (int)currentInput1, (int)currentInput2);							\
						spu_printf("Destination: %X %X\n", (int)g_pIn1[nextUsedBuffer], (int)g_pIn2[nextUsedBuffer]);							\
						cellDmaLargeGet(g_pIn1[nextUsedBuffer], currentInput1, (COUNT_BY_LOOP)*sizeof(IN1_TYPE), DMA_DOWNLOAD_TAG(1), 0, 0);	\
						cellDmaLargeGet(g_pIn2[nextUsedBuffer], currentInput2, (COUNT_BY_LOOP)*sizeof(IN2_TYPE), DMA_DOWNLOAD_TAG(2), 0, 0);	\
						currentInput1 += (COUNT_BY_LOOP)*sizeof(IN1_TYPE);																		\
						currentInput2 += (COUNT_BY_LOOP)*sizeof(IN2_TYPE);																		\
					}																															\
																																				\
					/* process */																												\
					spu_printf("processing %d entries\n", (COUNT_BY_LOOP));																		\
					JOB_NAME(g_pOut[currentUsedBuffer], g_pIn1[currentUsedBuffer], g_pIn2[currentUsedBuffer], COUNT_BY_LOOP);					\
																																				\
					previousUsedBuffer = 1 - previousUsedBuffer;																				\
					currentUsedBuffer = 1 - currentUsedBuffer;																					\
					nextUsedBuffer = 1 - nextUsedBuffer;																						\
				}																																\
																																				\
				cellDmaLargePut(g_pOut[previousUsedBuffer], currentOutput, (COUNT_BY_LOOP)*sizeof(OUT_TYPE), DMA_UPLOAD_TAG(1), 0, 0);			\
				currentOutput += (COUNT_BY_LOOP)*sizeof(OUT_TYPE);																				\
				cellDmaWaitTagStatusAll(DMA_UPLOAD_MASK(1));																					\
			}																																	\
																																				\
			if(dataRemaining)																													\
			{																																	\
				spu_printf("Remaining data: %d\n", dataRemaining);																				\
				cellDmaWaitTagStatusAll(DMA_DOWNLOAD_MASK(1) | DMA_DOWNLOAD_MASK(2));															\
																																				\
				JOB_NAME(g_pOut[currentUsedBuffer], g_pIn1[currentUsedBuffer], g_pIn2[currentUsedBuffer], dataRemaining);						\
																																				\
				spu_printf("Start transfert of remaining output to %X\n", (int)currentOutput);													\
				cellDmaLargePut(g_pOut[currentUsedBuffer], currentOutput, dataRemaining*sizeof(OUT_TYPE), DMA_UPLOAD_TAG(1), 0, 0);				\
				cellDmaWaitTagStatusAll(DMA_UPLOAD_MASK(1));																					\
			}																																	\
																																				\
			spu_printf("SPU(%d): processing done\n", taskId);																					\
																																				\
			cellAtomicDecr32(buf, atomicPtr);																									\
																																				\
			int currentValue = cellAtomicNop32(buf, atomicPtr);																					\
																																				\
			spu_printf("SPU(%d): Atomic: %d\n", taskId, currentValue);																			\
																																				\
			if(currentValue==0)																													\
			{																																	\
				spu_printf("SPU(%d): I'm the last in the taskSet. Shuting down....\n", taskId);													\
				cellSpursShutdownTaskset (cellSpursGetTasksetAddress ());																		\
			}																																	\
																																				\
			cellSpursExit();																													\
																																				\
			return;																																\
		}																													
#else
	#define CREATE_MAINJOB(JOB_NAME, OUT_TYPE, IN1_TYPE, IN2_TYPE)																				\
		myCREATE_MAINJOB(JOB_NAME, OUT_TYPE, IN1_TYPE, IN2_TYPE)																			
#endif

#endif
