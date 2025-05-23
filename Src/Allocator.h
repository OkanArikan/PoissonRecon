/*
Copyright (c) 2006, Michael Kazhdan and Matthew Bolitho
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#ifndef ALLOCATOR_INCLUDED
#define ALLOCATOR_INCLUDED
#include <vector>
#include "Array.h"

namespace PoissonRecon
{
	struct AllocatorState
	{
		size_t index , remains;
		AllocatorState( void ) : index(0) , remains(0) {}
	};

	/** This templated class assists in memory allocation and is well suited for instances
	* when it is known that the sequence of memory allocations is performed in a stack-based
	* manner, so that memory allocated last is released first. It also preallocates memory
	* in chunks so that multiple requests for small chunks of memory do not require separate
	* system calls to the memory manager.
	* The allocator is templated off of the class of objects that we would like it to allocate,
	* ensuring that appropriate constructors and destructors are called as necessary.
	*/
	template< class T >
	class Allocator
	{
		size_t _blockSize;
		AllocatorState _state;
		std::vector< Pointer( T ) > _memory;
	public:
		Allocator( void ) : _blockSize(0) {}
		~Allocator( void ){ reset(); }

		/** This method is the allocators destructor. It frees up any of the memory that
		* it has allocated. */
		void reset( void )
		{
			for( size_t i=0 ; i<_memory.size() ; i++ ) DeletePointer( _memory[i] );
			_memory.clear();
			_blockSize = 0;
			_state = AllocatorState();
		}
		/** This method returns the memory state of the allocator. */
		AllocatorState getState( void ) const { return _state; }

		/** This method initiallizes the constructor and the blockSize variable specifies the
		* the number of objects that should be pre-allocated at a time. */
		void set( size_t blockSize )
		{
			reset();
			_blockSize = blockSize;
			_state.index = -1;
			_state.remains = 0;
		}

		/** This method returns a pointer to an array of elements objects. If there is left over pre-allocated
		* memory, this method simply returns a pointer to the next free piece of memory, otherwise it pre-allocates
		* more memory. Note that if the number of objects requested is larger than the value blockSize with which
		* the allocator was initialized, the request for memory will fail.
		*/
		Pointer( T ) newElements( size_t elements=1 )
		{
			Pointer( T ) mem;
			if( !elements ) return NullPointer( T );
			if( elements>_blockSize ) MK_THROW( "elements bigger than block-size: " , elements , " > " , _blockSize );
			if( _state.remains<elements )
			{
				if( _state.index==_memory.size()-1 )
				{
					mem = NewPointer< T >( _blockSize );
					if( !mem ) MK_THROW( "Failed to allocate memory" );
					_memory.push_back( mem );
				}
				_state.index++;
				_state.remains = _blockSize;
			}
			mem = _memory[ _state.index ] + ( _blockSize-_state.remains );
			_state.remains -= elements;
			return mem;
		}
	};
}
#endif // ALLOCATOR_INCLUDE
