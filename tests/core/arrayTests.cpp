/*
 * arrayTests.cpp
 *
 *  Created on: May 20, 2013
 *      Author: Marja Aalto
 */

#include "gtest/gtest.h"
#include <iostream>
#include "../../stateplex/core/array.h"
#include "../../stateplex/core/string.h"

/*
 * A Google testcase class for testing array.h
 */

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
        myArray = Stateplex::Array<char>::uninitialised(myAllocator, 10);
        EXPECT_TRUE(myArray);

        Stateplex::Array<char> *bigArray;
        bigArray = Stateplex::Array<char>::uninitialised(myAllocator, 128);// Array size < 2 ^ 7
        EXPECT_TRUE(bigArray);

        Stateplex::Array<char> *veryBigArray;
        EXPECT_TRUE(veryBigArray);
        //veryBigArray = Stateplex::Array<char>::uninitialised(myAllocator, 16384);// Array size < 2 ^ 14
        //Segmentation fault.
        //0x000000000040886f in Stateplex::Array<char>::setLength (this=0x0, length=16384)
        //    at tests/core/../../stateplex/core/array.h:88
        //                      *size = length | 0x80;
        //(this=0x664e00)at tests/core/arrayTests.cpp:47

        Stateplex::Array<char> *tooBigArray;
        EXPECT_FALSE(tooBigArray); //This test is supposed to fail, since it points to bigger than allowed size of an array.
        //tooBigArray = Stateplex::Array<char>::uninitialised(myAllocator, 17000); // Array size > 2 ^ 14
                //This should throw an exception in stead of segmentation fault like currently:
                //Segmentation fault in Stateplex::Array<char>::setLength (this=0x0, length=17000)
                //at tests/core/../../stateplex/core/array.h:88
                //                      *size = length | 0x80;

        myArray->destroy(myAllocator);
        bigArray->destroy(myAllocator);
        veryBigArray->destroy(myAllocator);
     //   tooBigArray->destroy(myAllocator);

}

TEST_F(ArrayTests, elementTests)
{
        Stateplex::Array<char> *T;
        T = Stateplex::Array<char>::uninitialised(myAllocator, 50);
        EXPECT_EQ(50, T->length());

        T->setElement(0, 'Y');
        T->setElement(9, 'A');
        EXPECT_EQ('Y', T->element(0));
        EXPECT_EQ('A', T->element(9));

        Stateplex::String *tString;
        tString = Stateplex::String::copy(myAllocator, "Missing the white snow now!");
        EXPECT_STREQ("Missing the white snow now!", tString->chars());

        T->copy(myAllocator, tString->chars() + '\0', 8);
        //EXPECT_STREQ("Missing", T->elements()); // A bug in Type *elements() const or Size getLength(Size *sizeOfLength) const in array.h
        // Segmentation fault.
        // 0x00007ffff6eebfca in __strcmp_ssse3 () from /lib64/libc.so.6

        Stateplex::Array<char> *array;
        array = Stateplex::Array<char>::uninitialised(myAllocator, 50);
        EXPECT_EQ(50, array->length());
        array->copy(myAllocator, T);

        //EXPECT_STREQ("Missing", array->elements()); // Bug, see above

        T->destroy(myAllocator);
        tString->destroy(myAllocator);
        array->destroy(myAllocator);
 }

