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
// CLASS: QDT_MAP
//
//
//01-03-21 DAC Created: 
//*****************************************************************************

#ifndef __KCORE_TEMPLATE_QDT_MAP_H__
#define __KCORE_TEMPLATE_QDT_MAP_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Template/Qdt_Pair)

namespace QDT
{
	namespace KCORE
	{
		template <class	T, class U >
		class QDT_MAP
		{
		public:
		/************* ITERATOR **********************************************************/
		/************* ITERATOR **********************************************************/
		/************* ITERATOR **********************************************************/
			class	ITERATOR
			{
			public:

				//***********************************************************************************************
				/**	Constructeur de l'iterateur
				*	pQM :		DLIST référence pour vérifications diverses en mode debug
				*	FlagEnd :	Si false l'itérateur construit pointe sur le début de la liste
				*				si true l'itérateur construit pointe sur la fin de la liste (iterator de list nul) */
				explicit ITERATOR(	const QDT_MAP* const	pQM = NULL,
							const bool				bFlagEnd = false)
				{
					if	(pQM)
					{
						_pQM = const_cast<QDT_MAP*>(pQM);
						if	(bFlagEnd == false)
						{
							_I = _pQM->GetQDT_DLIST()->GetHead(); //begin();
							
							#ifdef _DEBUG
								if( _pQM->GetQDT_DLIST()->IsEmpty() )
								{
									_pData = NULL;
								}
								else
								{
									_pData = &(*_I);
								}
							#endif	//_DEBUG
						}
						else
						{
							_I = _pQM->GetQDT_DLIST()->GetTail(); //end();
							
							#ifdef _DEBUG
								_pData = NULL;
							#endif	//_DEBUG
						}
					}
					else
					{
						_pQM = NULL;
						//_I = NULL;

						#ifdef _DEBUG
							_pData = NULL;
						#endif	//_DEBUG
					}
				}

				//***********************************************************************************************
				/**	Destructeur */
				~ITERATOR()
				{
				}

				//***********************************************************************************************
				/**	Pré-incrémention de l'itérateur */
				ITERATOR&	operator++()
				{
					Increase();
					return	(*this);
				}

				//***********************************************************************************************
				/**	Post-incrémentation de l'itérateur */
				ITERATOR	operator++(int)
				{
					ITERATOR	Tmp = *this;
					Increase();
					return	(Tmp);
				}

				//***********************************************************************************************
				/**	Pré-décrémention de l'itérateur */
				ITERATOR&	operator--()
				{
					Decrease();
					return	(*this);
				}

				//***********************************************************************************************
				/**	Post-décrémentation de l'itérateur */
				ITERATOR	operator--(int)
				{
					ITERATOR	Tmp = *this;
					Decrease();
					return	(Tmp);
				}

				//***********************************************************************************************
				/**	Opérateur + pour obtenir un itérateur relatif a l'iterateur courant */
				ITERATOR	operator+(int n) const
				{
					//	On vérifie que l'itérator était déjà initialisé
					QDT_ASSERT(_pQM);
					
					ITERATOR	Tmp = *this;

					//	On augmente Tmp de n
					for ( ; n > 0; Tmp.Increase(), --n)
					{
					}
					return	(Tmp);
				}

				//***********************************************************************************************
				/**	Opérateur += pour obtenir un itérateur relatif a l'iterateur courant */
				ITERATOR &	operator+=(int n)
				{
					//	On vérifie que l'itérator était déjà initialisé
					QDT_ASSERT(_pQM);
					//QDT_ASSERT(_I != NULL);

					//	On decremente Tmp de n
					for ( ; n > 0; Increase(), --n)
					{
					}
					return	(*this);
				}

				//***********************************************************************************************
				/**	Opérateur - pour obtenir un itérateur relatif a l'iterateur courant */
				ITERATOR	operator-(int n) const
				{
					//	On vérifie que l'itérator était déjà initialisé
					QDT_ASSERT(_pQM);
					//QDT_ASSERT(_I != NULL);
					ITERATOR	Tmp = *this;

					//	On decremente Tmp de n
					for ( ; n > 0; Tmp.Decrease(), --n)
					{
					}
					return	(Tmp);
				}

				//***********************************************************************************************
				/**	Opérateur -= pour obtenir un itérateur relatif a l'iterateur courant */
				ITERATOR &	operator-=(int n)
				{
					//	On vérifie que l'itérator était déjà initialisé
					QDT_ASSERT(_pQM);
					//QDT_ASSERT(_I != NULL);

					//	On decremente Tmp de n
					for ( ; n > 0; Decrease(), --n)
					{
					}
					return	(*this);
				}

				//***********************************************************************************************
				/**	Opérateur * pour gérer un Itérateur comme un pointeur */
				QDT_PAIR<T,U>&	operator*() const
				{
					QDT_ASSERT(_pQM);

					QDT_ASSERT(_I != _pQM->GetTail()._I);
					return	(*_I);
				}

				//***********************************************************************************************
				/**	Fonction Get() pour récupérer l'objet
				*	Identique à l'opérateur * en lecture seule mais reconnu par le debogueur !	*/
				const QDT_PAIR<T,U>&	Get() const
				{
					return	(operator*());
				}

				//***********************************************************************************************
				/**	Opérateur d'assignement */
				ITERATOR&	operator=(const ITERATOR&	I)
				{
					//	On vérifie que l'itérator était déjà initialisé
					QDT_ASSERT(I._pQM);
					_pQM = I._pQM;
					_I = I._I;

					#ifdef _DEBUG			
						//if tail
						if( _pQM->GetTail()._I == I._I )
						{
							_pData = NULL;
						}
						else
						{
							_pData = &(*_I);
						}
					#endif	//_DEBUG
					return	(*this);
				}


				//***********************************************************************************************
				/**	Opérateur égalité retournant true si les deux itérateurs sont égaux */
				bool	operator==(const ITERATOR&	I) const
				{
					//	On vérifie que l'itérator était déjà initialisé
					QDT_ASSERT(_pQM);
					//QDT_ASSERT(_I);				//	Permet la comparaison de deux itérateurs NULL
					//QDT_ASSERT(I._I);				//	Au cas où le container n'aurait pas de buffer
					QDT_ASSERT(_pQM == I._pQM);		//	On vérifie que les itérateurs pointent sur le même vector !
					if	(_I != I._I)
					{
						return	false;
					}
					return	true;
				}

				//***********************************************************************************************
				/**	Opérateur différent retournant true si les deux itérateurs sont différents */
				bool	operator!=(const ITERATOR&	I) const
				{
					return	(!((*this) == I));
				}

				//***********************************************************************************************
				/**	Accesseur en lecture sur l'itérateur des STL (pour le wrapper de insert et erase) */
		//		const std::list<T>::iterator&	GetSTLIterator() const
				const typename QDT_DLIST< QDT_PAIR<T,U> >::ITERATOR&	GetSTLIterator() const
				{
					return	(_I);
				}

				//***********************************************************************************************
				/**	Accesseur en ecriture  sur l'itérateur des STL */
				void	SetSTLIterator(const typename QDT_DLIST< QDT_PAIR<T,U> >::ITERATOR& I)
				{
					_I = I;
					#ifdef _DEBUG
						_pData = &(*_I);
					#endif	//_DEBUG

				}

			//***********************************************************************************************
			private:
				typename QDT_DLIST< QDT_PAIR<T, U> >::ITERATOR	_I; /** Iterator STL */

				QDT_MAP					*_pQM;		/** Liste sur laquelle pointe l'iterator */

				#ifdef _DEBUG
					QDT_PAIR< T, U >	*_pData;	/** Pointeur sur l'element referencé par l'iterator, ATTENTION seulement en Debug */
				#endif	//_DEBUG

				//***********************************************************************************************
				/**	Incrément de l'itérateur courrant */
				void	Increase()
				{
					//	On vérifie que l'itérateur est valide en entrée
					QDT_ASSERT(_pQM);
		//			QDT_ASSERT(_I != NULL);

					//	On vérifie que l'on n'est pas déjà à la fin du vector !
					QDT_ASSERT(_I != _pQM->GetTail()._I);
					//	On appelle l'incrément de l'itérateur de base
					++_I;

					#ifdef _DEBUG			
						//if tail
						if( _pQM->GetTail()._I == this->_I )
						{
							_pData = NULL;
						}
						else
						{
							_pData = &(*_I);
						}
					#endif	//_DEBUG

					//	Un itérateur qui pointe sur le dernier élément
					//	est un itérateur contenant un _pQM valide et _I différent de NULL sur une dlist !
				}

				//***********************************************************************************************
				/**	Décrément de l'itérateur courrant */
				void	Decrease()
				{
					//	On vérifie que l'itérateur est valide en entrée
					QDT_ASSERT(_pQM);
					//	On vérifie que l'on n'est pas déjà au début du vector !
					QDT_ASSERT(_I != _pQM->GetHead()._I);
					//	On appelle le décrément de l'itérateur de base
					--_I;

					//Only for DEBUGGING, You will never use it in code
					#ifdef _DEBUG
						_pData = &(*_I);
					#endif	//_DEBUG

					//	Un itérateur qui pointe sur le dernier élément
					//	est un itérateur contenant un _pQM valide et _I différent de NULL sur une dlist !
				}
			};


		/************* ITERATOR **********************************************************/
		/************* ITERATOR **********************************************************/
		/************* ITERATOR **********************************************************/

			QDT_MAP()
			{
			}


			//***********************************************************************************************
			/** Constructeur par copie de QDT_MAP */
			explicit QDT_MAP(const QDT_MAP&	QM)
			{
				//	la liste d'initialisation sur le constructeur par copie ne fonctionne pas !
				//	On est obligé de passer par l'opérateur d'assignation
				_L = QM._L;
			}


			~QDT_MAP()
			{
			}
			
			//operator ==
			bool	operator==(const QDT_MAP&	QM) const
			{
				if	( GetSize() != QM.GetSize() )
				{
					return ( false );
				}
				else
				{
					return	(_L == QM._L);
				}
			}
			
			//operator !=
			bool	operator!=(const QDT_MAP&	QM) const
			{
				return ( !( (*this) == QM ) );
			}

			//operator[]
			U&	operator[](const T& Key) const
			{
				ITERATOR it = Find( Key );
				QDT_ASSERT( it != GetTail() ); //ask for something that exist
				
				return ( (*it).GetSecond() );
			}

			//-----------------------------------------------------------------------------
			// Name: InsertTail
			// Object: 
			//01-03-22 DAC Created: 
			//-----------------------------------------------------------------------------
			QDT_PAIR<ITERATOR, bool> InsertTail( const QDT_PAIR< T, U>& newPair )
			{
				T Key = newPair.GetFirst();

				ITERATOR it = Find( Key );

				QDT_PAIR<ITERATOR,bool> retPair;

				if ( it == GetTail() )
				{
					_L.InsertTail( newPair );
					retPair.SetFirst( GetLast() );
					retPair.SetSecond( true );
				}
				else
				{
					retPair.SetFirst( GetTail() );
					retPair.SetSecond( false );
				}

				return ( retPair );
			}

			//-----------------------------------------------------------------------------
			// Name: Insert
			// Object: Insère un élément avant l'iterateur donné
			//01-08-28 DAC Created: 
			//-----------------------------------------------------------------------------
			
			ITERATOR	Insert(	const ITERATOR&	I,
								const QDT_PAIR< T, U>& N)
			{
				//Il faut initialiser Cur en fonction de la liste sur laquelle on bosse
				ITERATOR	Cur=I;	//C'est obligatoire car le pb avec SetSTLIterator c'est qu'il ne met pas a jour _pQD
									//et donc il vaut NULL.
									//Or on en a besoin un peu partout dans des QDT_ASSERT donc plantage obligatoire un jour
				Cur.SetSTLIterator( GetQDT_DLIST()->Insert( I.GetSTLIterator(), N ) );
				
				return ( Cur );
			}

			//-----------------------------------------------------------------------------
			// Name: Insert
			// Object: Insère un élément avant l'iterateur donné
			//01-08-28 DAC Created: 
			//-----------------------------------------------------------------------------
			
			ITERATOR	Insert(	const ITERATOR&	I,
								const T& key,  const U& val)
			{
				QDT_PAIR< T, U> newPair;
				newPair.SetFirst( key );
				newPair.SetSecond( val );

				return( Insert( I, newPair ) );
			}

			//-----------------------------------------------------------------------------
			// Name: Insert
			// Object: 
			//01-03-22 DAC Created: 
			//-----------------------------------------------------------------------------
			QDT_PAIR<ITERATOR, bool> InsertTail( const T& Key , const U& Value)
			{
				QDT_PAIR< T , U> thePair( Key, Value);

				return ( InsertTail( thePair ) );	
			}

			//-----------------------------------------------------------------------------
			// Name: Remove
			// Object: 
			//01-03-21 DAC Created: 
			//-----------------------------------------------------------------------------	
			bool Remove( const T& Key)
			{
				//check if exists
				ITERATOR it = Find( Key );

				if( it != GetTail() )
				{
					_L.Remove( it.GetSTLIterator() );
					return ( true );
				}

				return ( false );
			}

			//-----------------------------------------------------------------------------
			// Name: Find
			// Object: 
			//01-03-21 DAC Created: 
			//-----------------------------------------------------------------------------
			ITERATOR Find( const T& Key) const
			{
				//if emtpy return tail
				if( IsEmpty() )
					return GetTail();

				//else proceed
				ITERATOR	Cur = GetHead();
				ITERATOR	End = GetTail();

				for ( ; Cur != End; ++Cur )	//on parcoure la liste
				{
					//if compile error here ! Key must implement operator== 
					if ((*Cur).GetFirst() == Key)		//si on trouve l'element
					{
						return ( Cur );
					}
				}
				return (Cur);				//on retourne l'element si on l'a trouvé, ou GetTail sinon

			}

			//-----------------------------------------------------------------------------
			// Name: GetPairFromKey
			// Object: 
			//01-03-21 DAC Created: 
			//-----------------------------------------------------------------------------
			QDT_PAIR< T ,U>& GetPairFromKey(const T& Key, bool& bKeyFound)
			{
				ITERATOR it = Find( Key );
				if( it != GetTail() )
				{
					bKeyFound = true;
				}
				else
				{
					bKeyFound = false;
				}

				return ( (*it) );
			}

			//-----------------------------------------------------------------------------
			// Name: GetKeyList
			// Object: 
			//01-03-21 DAC Created: 
			//-----------------------------------------------------------------------------
			QDT_DLIST< T > GetKeyList() const
			{
				
				QDT_DLIST< T > tListKey;

				ITERATOR itCur = GetHead();
				ITERATOR itEnd = GetTail();
				
				while( itCur != itEnd )
				{
					T Key = (*itCur).GetFirst();
					tListKey.InsertTail( Key );
					++itCur;
				}
				
				return ( tListKey );
			}

			//-----------------------------------------------------------------------------
			// Name: GetSize
			// Object: 
			//01-03-21 DAC Created: 
			//-----------------------------------------------------------------------------
			unsigned int	GetSize() const
			{
				return ( _L.GetSize() );
			}

			//-----------------------------------------------------------------------------
			// Name: IsEmpty
			// Object: 
			//01-03-21 DAC Created: 
			//-----------------------------------------------------------------------------
			bool IsEmpty() const
			{
				return ( _L.IsEmpty() );
			}

			//-----------------------------------------------------------------------------
			// Name: Clear
			// Object: 
			//01-03-21 DAC Created: 
			//-----------------------------------------------------------------------------
			void Clear()
			{
				_L.Clear();
			}

			//-----------------------------------------------------------------------------
			// Name: ClearAll
			// Object: 
			//01-03-21 DAC Created: 
			//-----------------------------------------------------------------------------
			void ClearAll()
			{
				QDT_NOT_IMPLEMENTED();
				
				//**********************************************************
				//there is no clearAll()
				// use auto_ptr or free memory yourself because there is no way to know if you've created a
				// QDT_MAP< int * , int *> or a QDT_MAP< int * , int > or even a QDT_MAP<int , int*>
				// I can't find if one or both the parametric types are pointers.
				//**********************************************************
			}
			
			//***********************************************************************************************
			/**	Accesseur sur la tête du Map */
			ITERATOR	GetHead() const
			{
				ITERATOR	I(this);
				return	(I);
			}

			//***********************************************************************************************
			/**	Accesseur sur le dernier element du Map ( si il existe, sinon ca plante ) */
			ITERATOR	GetLast() const
			{
				QDT_ASSERT( _L.GetSize() > 0);	//la liste DOIT contenir au moins un element
				//	Iterateur pointant sur le dernier élément...
				ITERATOR	I(this, true);
				return	(--I);
			}

			//***********************************************************************************************
			/**	Accesseur sur la queue du Map (iterator nul) */
			ITERATOR	GetTail() const
			{
				//	Iterateur pointant sur le dernier élément...
				ITERATOR	I(this, true);
				return	(I);
			}

			//***********************************************************************************************
			/**	Accesseur sur la QDT_DLIST de base */
			QDT_DLIST< QDT_PAIR< T , U> >*	GetQDT_DLIST()
			{
				return	(&_L);
			}

			//***********************************************************************************************
			/**	Accesseur sur la QDT_DLIST de base (version const) */
			const QDT_DLIST< QDT_PAIR< T , U> >*	GetQDT_DLIST() const
			{
				return	(&_L);
			}

			//***********************************************************************************************
		private:

			QDT_DLIST< QDT_PAIR< T , U> >	_L;
		};
	}
}

#endif //__KCORE_TEMPLATE_QDT_MAP_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
