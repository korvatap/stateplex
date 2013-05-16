/*
 * allocatorTests.cpp
 *
 *  Created on: May 8, 2013
 *      Author: maaalto
 */
#include "gtest/gtest.h"
#include "../../stateplex/core/allocator.h"
#include "../../stateplex/core/dispatcher.h"
#include "../../stateplex/core/actor.h"
#include <iostream>

class AllocatorTests : public ::testing::Test
{
protected:
        static Stateplex::Dispatcher *dispatcher;
        static Stateplex::Actor *actor;

        static void SetUpTestCase()
        {
                dispatcher = new Stateplex::Dispatcher();
                actor = new Stateplex::Actor(dispatcher);
        }
        static void TearDownTestCase()
        {
                delete dispatcher;
                delete actor;
        }

        virtual void SetUp(){}

        virtual void TearDown(){}
};
/*
TEST_F(AllocatorTests, FirstTest)
{
        Stateplex::Allocator *myAllocator = new Stateplex::Allocator();
        myAllocator->
}
*/
/*
Size16 sliceSizeToIndex(Size size) const;
        Size sliceSizeFromIndex(Size16 index) const;
        Block *blockFromAllocation(void *memory) const;
        void allocateBlocks();
        void sliceBlock(Size16 index);
        void *allocateSlice(Size16 index);
        void *allocateChunk(Size size);
        void deallocateSlice(void *memory, Size16 index);
        void deallocateChunk(void *memory, Size size);

*/
