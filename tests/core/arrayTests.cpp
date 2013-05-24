/*
 * arrayTests.cpp
 *
 *  Created on: May 20, 2013
 *      Author: maaalto
 */

#include "gtest/gtest.h"
#include <iostream>
#include "../../stateplex/core/array.h"

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
        myArray->uninitialised(myAllocator, 3);

        EXPECT_TRUE(myArray);
        EXPECT_TRUE(myArray->element(0));
        EXPECT_TRUE(myArray->element(1));
        EXPECT_TRUE(myArray->element(2));
        EXPECT_FALSE(myArray->element(3));
        myArray->setElement(0, 'Y');// Segmentation fault in array.h line 138.
        EXPECT_EQ(3, myArray->length());// Fails, actual length is 72.

        myArray->destroy(myAllocator);// Segmentation fault in Allocator::deallocateSlice()
}

TEST_F(ArrayTests, OtherElementTests)
{
        Stateplex::Array<char> *myArray;
        myArray->uninitialised(myAllocator, 50);
        std::cout << "myArray length is :" << myArray->length() << "\n";// Gives 72??
        EXPECT_EQ(50, myArray->length());
        //myArray->setElement(0, 'Y');
        //Segmentation fault.
        //0x00000000004080c4 in Stateplex::Array<char>::setElement (this=0x41aaf0,
        //index=0, value=89 'Y') at tests/core/../../stateplex/core/array.h:138
        //138             *(reinterpret_cast<Type *>(reinterpret_cast<char *>(this) + size) + index) = value;

        //EXPECT_EQ('Y', myArray->element(0));
        char *testString = "Missing the white snow.";
        myArray->copy(myAllocator, testString, 23);//Segmentation fault
        //std::cout << "myElements array contains: " << myArray->elements() << "\n";//Segmentation fault
        //EXPECT_STREQ("Missing the white snow.", myArray->elements()); //Fails

        Stateplex::Array<char> *array;
        array->copy(myAllocator, myArray);
        std::cout << "array length is : " << array->length() << "\n";
        myArray->destroy(myAllocator); //Segmentation fault
        array->destroy(myAllocator);
 }

TEST_F(ArrayTests, sizeTests){
        Stateplex::Array<char> *bigArray;
        bigArray->uninitialised(myAllocator, 128);// Array size 2 ^ 7
        bigArray->destroy(myAllocator);

        //Stateplex::Array<char> *veryBigArray;
        //veryBigArray->uninitialised(myAllocator, 16384);// Array size 2 ^ 14
                                //Segmentation fault.
                                //0x00000000004081db in Stateplex::Array<char>::setLength (this=0x0, length=16384)
                                //at tests/core/../../stateplex/core/array.h:88
                                //*size = length | 0x80;
        //veryBigArray->destroy(myAllocator);

        //Stateplex::Array<char> *tooBigArray;
        //tooBigArray->uninitialised(myAllocator, 17000); // Array size > 2 ^ 14

}
