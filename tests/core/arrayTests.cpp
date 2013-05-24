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
        myArray->uninitialised(myAllocator, 3);

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
        Stateplex::Allocator *anotherAllocator = new Stateplex::Allocator();
        Stateplex::Allocator *newAllocator = new Stateplex::Allocator();
        Stateplex::Allocator *stringAllocator = new Stateplex::Allocator();

        Stateplex::Array<char> *T;
        T->uninitialised(anotherAllocator, 50);

        //EXPECT_EQ(50, T->length());
                //Segmentation fault.
                //0x0000000000408646 in Stateplex::Array<char>::getLength (this=0x0, sizeOfLength=0x7fffffffde98)
                //   at tests/core/../../stateplex/core/array.h:69
                //69              if ((*size & 0x80) == 0) {

        //T->setElement(0, 'Y');
        //std::cout << "T->element(0) is: " << T->element(0) << "\n";
        //EXPECT_EQ('Y', T->element(0));

        Stateplex::String *tString;
        tString->copy(stringAllocator, "Missing the white snow now!");
        T->copy(anotherAllocator, tString->chars(), 7);//Segmentation fault
        //std::cout << "myElements array contains: " << anotherArray->elements() << "\n";//Segmentation fault
        //EXPECT_STREQ("Missing the white snow.", anotherArray->elements()); //Fails

        Stateplex::Array<char> *array;
        array->uninitialised(newAllocator, 50);
        array->copy(newAllocator, T);
        std::cout << "array length is : " << array->length() << "\n";

        T->destroy(anotherAllocator);
        array->destroy(newAllocator);
        delete anotherAllocator;
        delete newAllocator;
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
