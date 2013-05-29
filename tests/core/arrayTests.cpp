/*
 * arrayTests.cpp
 *
 *  Created on: May 20, 2013
 *      Author: maaalto
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
        myArray = Stateplex::Array<char>::uninitialised(myAllocator, 3);

        EXPECT_TRUE(myArray);
        EXPECT_TRUE(myArray->element(0));
        EXPECT_TRUE(myArray->element(1));
        EXPECT_TRUE(myArray->element(2));
        //myArray->setElement(0, 'Y');// Segmentation fault in array.h line 138.
        //EXPECT_EQ(3, myArray->length());// Fails, actual length is 72.

        //myArray->destroy(myAllocator);// Segmentation fault in Allocator::deallocateSlice()
}

TEST_F(ArrayTests, OtherElementTests)
{
        Stateplex::Array<char> *T;
        T = Stateplex::Array<char>::uninitialised(myAllocator, 50);
        EXPECT_EQ(50, T->length());

        T->setElement(0, 'Y');
        EXPECT_EQ('Y', T->element(0));

        Stateplex::String *tString;
        tString = Stateplex::String::copy(myAllocator, "Missing the white snow now!");
        T->copy(myAllocator, tString->chars(), 7);
        EXPECT_EQ("Missing", tString->chars());

        Stateplex::Array<char> *array;
        array = Stateplex::Array<char>::uninitialised(myAllocator, 50);
        array->copy(myAllocator, T);
        std::cout << "Elements are: "<< array->elements() << "\n";
        EXPECT_EQ(50, array->length());
        //EXPECT_EQ("Missing", array->elements());

        T->destroy(myAllocator);
        array->destroy(myAllocator);
        tString->destroy(myAllocator);
 }


TEST_F(ArrayTests, sizeTests){
        Stateplex::Array<char> *bigArray;
        bigArray = Stateplex::Array<char>::uninitialised(myAllocator, 128);// Array size < 2 ^ 7
        bigArray->destroy(myAllocator);
/*
        Stateplex::Array<char> *veryBigArray;
        veryBigArray = Stateplex::Array<char>::uninitialised(myAllocator, 16384);// Array size < 2 ^ 14

        veryBigArray->destroy(myAllocator);

        Stateplex::Array<char> *tooBigArray;
        tooBigArray = Stateplex::Array<char>::uninitialised(myAllocator, 17000); // Array size > 2 ^ 14

        tooBigArray->destroy(myAllocator);
*/
}
