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
//	CLASS:	QDT_LIST, QDT_LIST_ELEMENT
//	The SCRIPT_MANAGER class implements ...
//
//	05-10-24:	NHO - Created
//*****************************************************************************
//
// HOW TO USE IT :
//
// Just make your class heritate from QDT_LIST_ELEMENT
// Ex : 
//		class myclass : public QDT_LIST_ELEMENT
//		{
//			public:
//				...
//			private:
//				...
//		}
// Then use : 
//		myclass*		instance;
//		QDT_LIST		mylist;
//		instance = new myclass;
// 		mylist.InsertHead(instance);
// 		mylist.DeleteElement(instance);
// 
// Remarks : 
// 		- Sup method just break links inside the list but do not delete the 
//        element itself. To delete an element, use method 
//		  DeleteElement(QDT_LIST_ELEMENT*).
// 		- This class is fully independant. No need anything else to compile and run. 
// 		Keep it like this !
// 		- Take care of the basic error : put a "static element".
// Ex : 
//		myclass			instance;
//		QDT_LIST		mylist;
//		mylist.InsertHead(instance);
//		mylist.DeleteElement(instance);	
//		-> This will cause an assertion failure because cannot delete something 
//         not alloacted by a "new" !
// 
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TOOLS_QDT_LIST_H__
#define __KCORE_TOOLS_QDT_LIST_H__

#ifndef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		// Usefulls macros

		// use it like a classic for
		#define FOR_EACH_ELEMENT(LIST,ITER,ETYPE)	for(ETYPE* ITER=(ETYPE*)(LIST).GetHead(); ITER!=NULL; ITER=(ETYPE*)ITER->GetNext())

		// find a specific element result in ITER (if ITER=NULL element is not found)
		#define FIND_ELEMENT(LIST,ITER,ETYPE,EXP)	ETYPE* ITER=(ETYPE*)LIST.GetHead();		\
													while(ITER)								\
													if	(EXP)								\
														break;								\
													else									\
														ITER=(ETYPE*)ITER->GetNext();		\

		// ------------------------------------------------------------------------------------------------------------------------

		class QDT_LIST_ELEMENT
		{
		protected:
			QDT_LIST_ELEMENT*	_Prev;
			QDT_LIST_ELEMENT*	_Next;

		public:
			inline			QDT_LIST_ELEMENT()													{ _Prev=(QDT_LIST_ELEMENT*)NULL; _Next=(QDT_LIST_ELEMENT*)NULL; }
			inline			QDT_LIST_ELEMENT(QDT_LIST_ELEMENT* Prev, QDT_LIST_ELEMENT* Next)	{ _Prev=Prev; _Next=Next; }
			inline virtual ~QDT_LIST_ELEMENT()													{}
			inline void		SetNext(QDT_LIST_ELEMENT* Next)										{ _Next=Next; }
			inline void		SetPrev(QDT_LIST_ELEMENT* Prev)										{ _Prev=Prev; }
			inline			QDT_LIST_ELEMENT*	GetNext() const									{ return _Next; }
			inline			QDT_LIST_ELEMENT*	GetPrev() const									{ return _Prev; }
		};

		// ------------------------------------------------------------------------------------------------------------------------

		typedef bool	(*ListApplyFunc)	(QDT_LIST_ELEMENT*, unsigned long);
		typedef bool	(*ListSortFunc)		(QDT_LIST_ELEMENT*, QDT_LIST_ELEMENT*);

		class QDT_LIST
		{
		protected:
			QDT_LIST_ELEMENT*			_Head;
			QDT_LIST_ELEMENT*			_Queue;
			unsigned long				_nNbElements;

		public:
			inline						QDT_LIST()											{ Init(); }
			inline						~QDT_LIST();

			inline void					Init(void)											{ _Head=(QDT_LIST_ELEMENT*)NULL; _Queue=(QDT_LIST_ELEMENT*)NULL; _nNbElements=0; }

			inline void					InsertHead(QDT_LIST_ELEMENT*);
			inline void					InsertQueue(QDT_LIST_ELEMENT*);

			inline void					InsertBefore(QDT_LIST_ELEMENT*,QDT_LIST_ELEMENT* );
			inline void					InsertAfter(QDT_LIST_ELEMENT*,QDT_LIST_ELEMENT* );

			inline void					Sort(ListSortFunc);

			inline QDT_LIST_ELEMENT*	GetHead() const										{ return _Head; }
			inline QDT_LIST_ELEMENT*	GetQueue() const									{ return _Queue; }
			inline QDT_LIST_ELEMENT*	GetIndex(unsigned long Index) const;

			inline QDT_LIST_ELEMENT*	Sup(QDT_LIST_ELEMENT*);						// Do not delete the element! Just break links
			inline void					DeleteElement(QDT_LIST_ELEMENT* pElement);	// Delete element, using virtual destructor
			inline void					DeleteAllElements(void);

			inline unsigned long		GetNbElements() const								{ return _nNbElements; }

			inline bool							IsEmpty() const { return _nNbElements == 0; }

			inline QDT_LIST_ELEMENT*	operator[](unsigned long);

			inline int					Find(const QDT_LIST_ELEMENT* pElement) const;

			inline QDT_LIST_ELEMENT*	Apply(ListApplyFunc, unsigned long);
		};

		QDT_LIST::~QDT_LIST()
		{
			QDT_LIST_ELEMENT* pElement;
			QDT_LIST_ELEMENT* pElementNext;

			pElement = GetHead();
			while (pElement)
			{
				pElementNext = pElement->GetNext();
				delete pElement;
				pElement = pElementNext;
			}
			_Head = _Queue = (QDT_LIST_ELEMENT*)NULL;
		}

		//	Usage : u must suply a function which compares two elements and return true if element should be zapped
		//  bubble sort, quite slow except in particular cases, add other sorting methods !
		void inline QDT_LIST::Sort(ListSortFunc Function)
		{
			int					Zapped = true;
			QDT_LIST_ELEMENT	*Element;

			if(_nNbElements < 2)
				return;

			while(Zapped)
			{
				Zapped = 0;
				Element = _Head;
				while(Element)
				{
					if(Element->GetNext())
					{
						if(Function(Element,Element->GetNext()))
						{
							// Zap
							Zapped = true;
							QDT_LIST_ELEMENT *Element2;
							QDT_LIST_ELEMENT *bckp2;

							Element2	=	Element->GetNext();
							bckp2		=	Element->GetPrev();

							if(Element->GetPrev())	Element->GetPrev()->SetNext(Element2);
							if(Element2->GetNext())	Element2->GetNext()->SetPrev(Element);

							Element->SetNext(Element2->GetNext());
							Element->SetPrev(Element2);

							Element2->SetNext(Element);
							Element2->SetPrev(bckp2);

							if(Element == _Head)
								_Head = Element2;

							if(Element2 == _Queue)
								_Queue = Element;

							Element = Element2;
						}
					}
					Element = Element->GetNext();
				}
			}
		}

		void QDT_LIST::InsertHead(QDT_LIST_ELEMENT* pElement)
		{
			QDT_ASSERT(pElement != NULL && "QDT_LIST: trying to insert a NULL element");

			pElement->SetNext(_Head);
			pElement->SetPrev((QDT_LIST_ELEMENT*)NULL);

			if (_Head)
				_Head->SetPrev(pElement);
			else
				_Queue = pElement;

			_Head = pElement;
			_nNbElements++;
		}

		void QDT_LIST::InsertQueue(QDT_LIST_ELEMENT* pElement)
		{
			QDT_ASSERT(pElement != NULL && "QDT_LIST: trying to insert a NULL element");

			pElement->SetPrev(_Queue);
			pElement->SetNext((QDT_LIST_ELEMENT*)NULL);

			if (_Queue)
				_Queue->SetNext(pElement);
			else
				_Head = pElement;

			_Queue = pElement;
			_nNbElements++;
		}

		void QDT_LIST::InsertBefore(QDT_LIST_ELEMENT* pExistingElement,QDT_LIST_ELEMENT* pElementToInsert)
		{
			QDT_ASSERT(pElementToInsert != NULL && "QDT_LIST: trying to insert a NULL element");
			QDT_ASSERT(pExistingElement != NULL && "QDT_LIST: trying to insert an element before a NULL element");

			pElementToInsert->SetPrev(pExistingElement->GetPrev());
			pElementToInsert->SetNext(pExistingElement);

			if (pExistingElement->GetPrev())
				pExistingElement->GetPrev()->SetNext(pElementToInsert);
			else
				_Head = pElementToInsert;

			pExistingElement->SetPrev(pElementToInsert);
			_nNbElements++;
		}

		void QDT_LIST::InsertAfter(QDT_LIST_ELEMENT* pExistingElement,QDT_LIST_ELEMENT* pElementToInsert)
		{
			QDT_ASSERT(pElementToInsert != NULL && "QDT_LIST: trying to insert a NULL element");
			QDT_ASSERT(pExistingElement != NULL && "QDT_LIST: trying to insert an element after a NULL element");

			pElementToInsert->SetNext(pExistingElement->GetNext());
			pElementToInsert->SetPrev(pExistingElement);

			if (pExistingElement->GetNext())
				pExistingElement->GetNext()->SetPrev(pElementToInsert);
			else
				_Queue = pElementToInsert;

			pExistingElement->SetNext(pElementToInsert);
			_nNbElements++;
		}

		QDT_LIST_ELEMENT* QDT_LIST::Sup(QDT_LIST_ELEMENT* pElement)
		{
			QDT_ASSERT(pElement != NULL && "QDT_LIST: trying to sup a NULL element");

			if (pElement->GetPrev())
			{
				pElement->GetPrev()->SetNext(pElement->GetNext());
			}
			else
			{
				_Head = pElement->GetNext();
			}

			if (pElement->GetNext())
			{
				pElement->GetNext()->SetPrev(pElement->GetPrev());
			}
			else
			{
				_Queue = pElement->GetPrev();
			}

			_nNbElements--;

			return pElement;
		}

		QDT_LIST_ELEMENT* QDT_LIST::Apply(ListApplyFunc pFunc, unsigned long UserValue=0)
		{
			QDT_LIST_ELEMENT* pElement;

			pElement = _Head;
			while (pElement)
			{
				if (!pFunc(pElement,UserValue))
					return pElement;
				pElement = pElement->GetNext();
			}
			
			return (QDT_LIST_ELEMENT*)NULL;
		}

		int	QDT_LIST::Find(const QDT_LIST_ELEMENT* pElement) const
		{
			QDT_LIST_ELEMENT* pElem = GetHead();
			int	iElement= 0;
			while (pElem)
			{
				if (pElem == pElement)
					return iElement;

				iElement++;
				pElem = pElem->GetNext();
			}

			return -1;
		}

		QDT_LIST_ELEMENT* QDT_LIST::GetIndex(unsigned long Index) const
		{
			QDT_LIST_ELEMENT*	pElement	= GetHead();
			unsigned long		i			= 0;

			if( Index >= _nNbElements)
				return (QDT_LIST_ELEMENT*)NULL;
			
			while (pElement && i!=Index)
			{
				pElement = pElement->GetNext();
				i++;
			}

			return pElement;
		}

		QDT_LIST_ELEMENT* QDT_LIST::operator[](unsigned long Index)
		{
			QDT_LIST_ELEMENT*	pElement=GetHead();
			unsigned long		i=0;

			if( Index >= _nNbElements)
				return (QDT_LIST_ELEMENT*)NULL;
			
			while (pElement && i!=Index)
			{
				pElement = pElement->GetNext();
				i++;
			}

			return pElement;
		}

		void QDT_LIST::DeleteElement(QDT_LIST_ELEMENT* pElmt)
		{
			Sup(pElmt);
			delete pElmt;
		}

		void QDT_LIST::DeleteAllElements(void)
		{
			while( GetNbElements() )
			{
				DeleteElement(GetHead());
			}
		}
	}
}

#endif
