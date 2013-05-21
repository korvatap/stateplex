/*
 * arrayTests.cpp
 *
 *  Created on: May 20, 2013
 *      Author: maaalto
 */

#include "gtest/gtest.h"
#include <iostream>
#include "../../stateplex/core/array.h"

class ArrayTests:public ::testing::Test{

protected:
        static Stateplex::Allocator *myAllocator;
        static void SetUpTestCase(){
                myAllocator = new Stateplex::Allocator();
        }
        virtual void SetUp(){}
        virtual void TearDown(){}
        static void TearDownTestCase()
        {
                delete myAllocator;
        }

};

Stateplex::Allocator *ArrayTests::myAllocator;

TEST_F(ArrayTests, UnitializedArrayTest)
{
        Stateplex::Array<char> *myArray;
        myArray->uninitialised(myAllocator, 3);

        EXPECT_TRUE(myArray);
        EXPECT_TRUE(myArray->element(0));
        EXPECT_TRUE(myArray->element(1));
        EXPECT_TRUE(myArray->element(2));
        EXPECT_EQ(3, myArray->length());// Fails, actual length is 72

        myArray->destroy(myAllocator);// Segmentation fault in Allocator::deallocateSlice()
}

TEST_F(ArrayTests, OtherElementTests)
{
        Stateplex::Array<char> *myArray;
        myArray->uninitialised(myAllocator, 50);
        //myArray->setElement(0, 'Y');
        //EXPECT_EQ('Y', myArray->element(0));
        myArray->copy(myAllocator, "Missing the white snow.", 23);
        //EXPECT_EQ('M', myArray->element(1));
        //EXPECT_STREQ("Missing the", myArray->elements());

        Stateplex::Array<char> *array;
        array->copy(myAllocator, myArray);
        std::cout << "array length is : " << array->length() << "\n";
        myArray->destroy(myAllocator);
 }

/*
 *      Type *elements() const;
        Type element(Size index) const;
        Size length() const;

        void setElement(Size index, Type value);
        void destroy(Allocator *allocator);

        static Array<Type> *uninitialised(Allocator *allocator, Size length);
        static Array<Type> *copy(Allocator *allocator, Type *const cArray, Size length);
        static Array<Type> *copy(Allocator *allocator, Array<Type> *array);
 */

