/*
 * Stateplex - A server-side actor model library.
 *
 * core/buffer.h
 *
 * (c) 2013 Henrik Hedberg <henrik.hedberg@innologies.fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Authors: Henrik Hedberg
 */

#ifndef INCLUDED_STATEPLEX_BUFFER_H
#define INCLUDED_STATEPLEX_BUFFER_H

#include "list.h"
#include "types.h"
#include "allocator.h"

namespace Stateplex {

/**
 * Receives/pushes data to blocks and then consumes then. 
 * This makes it possible to receive/pop messages fast and then we can “consume” the data as fast as we can process it.
 * The data just stays in the buffer block for the time it takes to consume them.
 */

template<Size16 blockSize = 4096>
class Buffer {
	struct Block : public ListItem<Block> {
		Size16 mStart;
		Size16 mEnd;
	};

	List<Block> mBlocks;
	Size mSize;

	Block *mHere;
	Size16 mPosition;
	Size mOffset;

	Block *allocateBlock();
	void deallocateBlock(Block *block);
	void pushToBlock(const char *cString, Size length, Block *block);

public:
	Buffer();
	Buffer(const char *string);

	void push(Buffer *buffer);
	void push(const char *cString);
	void push(const char *cString, Size length);

	void ensurePushLength(Size16 length);
	char *pushPointer();
	Size16 pushLength();
	void pushed(Size16 length);

	char peek();
	char pop();

	char *popPointer();
	Size16 popLength();
	void popped(Size16 length);

	Size size();

	char here();
	void next();
	Size left();
	void popHere();

	void compress();
};

}

/*** Template implementations ***/

#include <string.h>

namespace Stateplex {
/**
 * Function that allocates a block.
 * 
 * @return	Allocated block
 */
 
template<Size16 blockSize>
typename Buffer<blockSize>::Block *Buffer<blockSize>::allocateBlock()
{
	Block *block;
	/* TODO: Allocate */
	mBlocks.addTail(block);

	if (!mHere)
		mHere = block;

	return block;
}
/**
 * Function that deallocates a specific block.
 * 
 * @param block		Block to deallocate
 */
 
template<Size16 blockSize>
void Buffer<blockSize>::deallocateBlock(Block *block)
{
	block->remove();
	/* TODO: deallocate */
}

/**
 * Function that is used to place chars to a specific block.
 * 
 * @param *cString	Chars to be placed
 * @param length	Number of chars to copy
 * @param *block	Target block to push
 */

template<Size16 blockSize>
void Buffer<blockSize>::pushToBlock(const char *cString, Size length, Block *block)
{
	if (!block ||block->mEnd == blockSize)
		block = allocateBlock();

	char *pointer = reinterpret_cast<char *>(block) + sizeof(Block);
	Size16 room = blockSize - block->mEnd;
	if (length < room) {
		memcpy(pointer + block->mEnd, cString, length);
		block->mEnd += length;
	} else {
		memcpy(pointer + block->mEnd, cString, room);
		pushToBlock(cString + room, length - room, 0);
	}
}

/**
 * Constructor for class Buffer
 * Initializes a new instance of buffer.
 * 
 */

template<Size16 blockSize>
Buffer<blockSize>::Buffer()
	: mSize(0), mHere(0), mPosition(0), mOffset(0)
{ }

/**
 * Constructor for class Buffer.
 * This constructor has an ability to take in a string straight to buffer.
 * 
 */

template<Size16 blockSize>
Buffer<blockSize>::Buffer(const char *string)
	: mSize(0), mHere(0), mPosition(0), mOffset(0)
{
	push(string);
}

/**
 * Function that takes in another buffer and goes through
 * every block item in the buffer and adds them to this buffer.
 * 
 * @param buffer	Target buffer to be added.
 */

template<Size16 blockSize>
void Buffer<blockSize>::push(Buffer *buffer)
{
	for (ListItem<Block> *item = buffer->mBlocks; item; item = buffer->mBlocks.next(item)) {
		Block *block = static_cast<Block *>(item);
		mBlocks.addTail(item);
		mSize += block->mEnd - block->mStart;
	}
	buffer->mSize = 0;
}

/**
 * Function that takes in a string and calls push function to place
 * the string to buffer.
 * 
 * @param *string	String to be added
 */

template<Size16 blockSize>
void Buffer<blockSize>::push(const char *string)
{
	push(string, strlen(string));
}

/**
 * Function that takes in chars and a value how many chars are to be
 * added to block. After this the function calls another function
 * to push the chars to a block.
 * 
 * @param *cString	Chars to be added.
 * @param length	Determines how many chars are to be added.
 */

template<Size16 blockSize>
void Buffer<blockSize>::push(const char *cString, Size length)
{
	pushToBlock(cString, length, static_cast<Block *>(mBlocks.last()));
	mSize += length;
}

/**
 * Function that ensures push length.
 * It check that there are blocks if none allocates a new one.
 * If there are blocks it ensures that push length is not too big
 * to be allocated.
 * 
 * @param length	Maximum push length.
 */

template<Size16 blockSize>
void Buffer<blockSize>::ensurePushLength(Size16 length)
{
	Block *block = mBlocks.last();
	if (!block || blockSize - block->mEnd < length)
		allocateBlock();
}

/**
 * Function that returns pointer to next of the last block saved.
 * 
 * @return	Pointer to return.
 */

template<Size16 blockSize>
char *Buffer<blockSize>::pushPointer()
{
	Block *block = mBlocks.last();
	return reinterpret_cast<char *>(block) + sizeof(Block) + block->mEnd;
}

/**
 * Function that returns last pushed blocks length.
 * 
 * @return	lenght of the push
 */

template<Size16 blockSize>
Size16 Buffer<blockSize>::pushLength()
{
	Block *block = mBlocks.last();
	return blockSize - block->mEnd;
}

/**
 * Function that is called when a new block is added to buffer.
 * This function takes a value and adds it to a variable that represends the
 * blocks last address space. Also this value is added to the buffer size.
 * 
 * @param length	Value to be assigned.
 */

template<Size16 blockSize>
void Buffer<blockSize>::pushed(Size16 length)
{
	Block *block = mBlocks.last();
	block->mEnd += length;
	mSize += length;
}

/**
 * Function that peeks at the value of first block in buffer.
 * Calls function popPointer to get the address of the first value in
 * buffer.
 * 
 * @return	Value that has been taken from address which was pointed.
 */

template<Size16 blockSize>
char Buffer<blockSize>::peek()
{
	return *popPointer();
}

/**
 * Function that takes the first value from buffer and stores it
 * to a variable and removes the value from the buffer.
 * 
 * @return	Value that was popped.
 */

template<Size16 blockSize>
char Buffer<blockSize>::pop()
{
	char c = *popPointer();
	popped(1);
	return c;
}

/**
 * Function that returns pointer to the starting point of the first block.
 * 
 * @return	Pointer to return.
 */

template<Size16 blockSize>
char *Buffer<blockSize>::popPointer()
{
	Block *block = mBlocks.first();
	return reinterpret_cast<char *>(block) + sizeof(Block) + block->mStart;
}

/**
 * Function that returns the length of the first block.
 * 
 * @return	The size of the first block.
 */

template<Size16 blockSize>
Size16 Buffer<blockSize>::popLength()
{
	Block *block = mBlocks.first();
	return block->mEnd - block->mStart;
}

/**
 * Function that deallocates the first block from the buffer.
 * This function is called when a value has been popped from buffer.
 *
 * @param length	Length of the value that has been popped.
 */

template<Size16 blockSize>
void Buffer<blockSize>::popped(Size16 length)
{
	ListItem<Block> *item = mBlocks.first();
	Block *block = static_cast<Block *>(item);
	block->mStart += length;
	if (block->mStart >= block->mEnd) {
		if (mBlocks.next(item))
			deallocateBlock(block);
		else
			block->mStart = block->mEnd = 0;
	}
	mSize -= length;
	/* TODO: Here */
}

/**
 * Function that returns the total size of buffer.
 * 
 * @return	Total size of buffer.
 */
 
template<Size16 blockSize>
Size Buffer<blockSize>::size()
{
	return mSize;
}

/**
 * Function that returns the value of current position in buffer.
 * 
 * @return	Value of the current position.
 */
 
template<Size16 blockSize>
char Buffer<blockSize>::here()
{
	return *reinterpret_cast<char *>(mHere) + sizeof(Block) + mPosition;
}

/**
 * Function that moves to the next position in the buffer.
 * 
 */

template<Size16 blockSize>
void Buffer<blockSize>::next()
{
	mPosition++;
	if (mPosition >= mHere->mEnd) {
		mHere = static_cast<Block *>(mBlocks.next(mHere));
		mPosition = mHere->mStart;
	}
	mOffset++;
}

/**
 * Function that calculates how much there is left to handle in the buffer.
 * 
 * @return	Value left to handle.
 */

template<Size16 blockSize>
Size Buffer<blockSize>::left()
{
	return mSize - mOffset;
}

/**
 * This function deallocates every block that is behind the current position's
 * block. After this the function calls for function popped and set current position
 * in buffer to current block.
 * 
 */

template<Size16 blockSize>
void Buffer<blockSize>::popHere()
{
	for (ListItem<Block> *item = mBlocks.previous(mHere); item; item = mBlocks.previous(item)) {
		Block *block = static_cast<Block *>(item);
		mSize -= block->mEnd - block->mStart;
		deallocateBlock(block);
	}
	popped(mPosition - mHere->mStart);

	mHere = static_cast<Block *>(mBlocks.first());
	mPosition = mHere->mStart;
	mOffset = 0;
}

template<Size16 blockSize>
void Buffer<blockSize>::compress()
{
	/* TODO: Not implemented */
}

}

#endif
