/*
Copyright (c) 2014, James Clay

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "ComplexFirFilter.h"
#include "RealFirFilter.h"
#include "gtest/gtest.h"

using namespace NimbleDSP;

extern bool FloatsEqual(double float1, double float2);


TEST(ComplexFirFilter, ConvComplexOneShot) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8), std::complex<double>(-6, 9), std::complex<double>(-7, 10)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(1, 2), std::complex<double>(2, 7), std::complex<double>(2, 16), std::complex<double>(0, 30), std::complex<double>(-5, 50), std::complex<double>(-20, 65), std::complex<double>(-35, 80), std::complex<double>(-50, 95), std::complex<double>(-65, 110), std::complex<double>(-72, 114), std::complex<double>(-70, 106), std::complex<double>(-58, 85), std::complex<double>(-35, 50)};
    unsigned numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    filter.filtOperation = ONE_SHOT_RETURN_ALL_RESULTS;
    
    conv(buf, filter);
    EXPECT_EQ(input.size() + filter.size() - 1, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
}

TEST(ComplexFirFilter, ConvComplexOneShotTrim) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8), std::complex<double>(-6, 9), std::complex<double>(-7, 10)};
    std::complex<double> filterTaps[] = {1, 2, std::complex<double>(3, 1), 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(0.00000000, 17.00000000), std::complex<double>(-3.00000000, 30.00000000), std::complex<double>(-9.00000000, 49.00000000), std::complex<double>(-25.00000000, 63.00000000), std::complex<double>(-41.00000000, 77.00000000), std::complex<double>(-57.00000000, 91.00000000), std::complex<double>(-73.00000000, 105.00000000), std::complex<double>(-81.00000000, 108.00000000), std::complex<double>(-80.00000000, 99.00000000)};
    unsigned numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    filter.filtOperation = ONE_SHOT_TRIM_TAILS;
    
    conv(buf, filter);
    EXPECT_EQ(input.size(), buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
}

TEST(ComplexFirFilter, ConvComplexStream1) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8)};
    std::complex<double> inputData2[] = {std::complex<double>(-6, 9), std::complex<double>(-7, 10), std::complex<double>(-8, 11), std::complex<double>(-9, 12)};
    std::complex<double> inputData3[] = {std::complex<double>(-10, 13), std::complex<double>(-11, 14), std::complex<double>(-12, 15), std::complex<double>(-13, 16), std::complex<double>(-14, 17), std::complex<double>(-15, 18), std::complex<double>(-16, 19), std::complex<double>(-17, 20), std::complex<double>(-18, 21), std::complex<double>(-19, 22), std::complex<double>(-20, 23), std::complex<double>(-21, 24), std::complex<double>(-22, 25), std::complex<double>(-23, 26)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(1, 2), std::complex<double>(2, 7), std::complex<double>(2, 16), std::complex<double>(0, 30), std::complex<double>(-5, 50), std::complex<double>(-20, 65), std::complex<double>(-35, 80)};
    std::complex<double> expectedData2[] = {std::complex<double>(-50, 95), std::complex<double>(-65, 110), std::complex<double>(-80, 125), std::complex<double>(-95, 140)};
    std::complex<double> expectedData3[] = {std::complex<double>(-110, 155), std::complex<double>(-125, 170), std::complex<double>(-140, 185), std::complex<double>(-155, 200), std::complex<double>(-170, 215), std::complex<double>(-185, 230), std::complex<double>(-200, 245), std::complex<double>(-215, 260), std::complex<double>(-230, 275), std::complex<double>(-245, 290), std::complex<double>(-260, 305), std::complex<double>(-275, 320), std::complex<double>(-290, 335), std::complex<double>(-305, 350)};
    
    unsigned numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);

    numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    conv(buf, filter);
    EXPECT_EQ(input.size(), buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
    
    numElements = sizeof(inputData2)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData2, numElements);
    input = buf;
    conv(buf, filter);
    EXPECT_EQ(input.size(), buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData2[i], buf[i]);
    }
    
    numElements = sizeof(inputData3)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData3, numElements);
    input = buf;
    conv(buf, filter);
    EXPECT_EQ(input.size(), buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData3[i], buf[i]);
    }
}

TEST(ComplexFirFilter, DecimateComplexOneShot) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8), std::complex<double>(-6, 9), std::complex<double>(-7, 10)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(1, 2), std::complex<double>(2, 16), std::complex<double>(-5, 50), std::complex<double>(-35, 80), std::complex<double>(-65, 110), std::complex<double>(-70, 106), std::complex<double>(-35, 50)};
    unsigned numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    int rate = 2;
    filter.filtOperation = ONE_SHOT_RETURN_ALL_RESULTS;
    
    decimate(buf, rate, filter);
    EXPECT_EQ((input.size() + filter.size() + rate - 1)/rate, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
}

TEST(ComplexFirFilter, DecimateComplexOneShotTrim) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8), std::complex<double>(-6, 9), std::complex<double>(-7, 10)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(2, 16), std::complex<double>(-5, 50), std::complex<double>(-35, 80), std::complex<double>(-65, 110), std::complex<double>(-70, 106)};
    unsigned numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    int rate = 2;
    filter.filtOperation = ONE_SHOT_TRIM_TAILS;
    
    decimate(buf, rate, filter);
    EXPECT_EQ((input.size() + rate - 1)/rate, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
}

TEST(ComplexFirFilter, DecimateComplexStream1) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8)};
    std::complex<double> inputData2[] = {std::complex<double>(-6, 9), std::complex<double>(-7, 10), std::complex<double>(-8, 11), std::complex<double>(-9, 12)};
    std::complex<double> inputData3[] = {std::complex<double>(-10, 13), std::complex<double>(-11, 14), std::complex<double>(-12, 15), std::complex<double>(-13, 16), std::complex<double>(-14, 17), std::complex<double>(-15, 18), std::complex<double>(-16, 19), std::complex<double>(-17, 20), std::complex<double>(-18, 21), std::complex<double>(-19, 22), std::complex<double>(-20, 23), std::complex<double>(-21, 24), std::complex<double>(-22, 25), std::complex<double>(-23, 26)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(1, 2), std::complex<double>(0, 30), std::complex<double>(-35, 80)};
    std::complex<double> expectedData2[] = {std::complex<double>(-80, 125)};
    std::complex<double> expectedData3[] = {std::complex<double>(-125, 170), std::complex<double>(-170, 215), std::complex<double>(-215, 260), std::complex<double>(-260, 305), std::complex<double>(-305, 350)};
    int rate = 3;
    
    unsigned numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);

    numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    decimate(buf, rate, filter);
    EXPECT_EQ(3, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
    
    numElements = sizeof(inputData2)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData2, numElements);
    decimate(buf, rate, filter);
    EXPECT_EQ(1, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData2[i], buf[i]);
    }
    
    numElements = sizeof(inputData3)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData3, numElements);
    decimate(buf, rate, filter);
    EXPECT_EQ(5, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData3[i], buf[i]);
    }
}


TEST(ComplexFirFilter, InterpComplex1) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8), std::complex<double>(-6, 9), std::complex<double>(-7, 10)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(1, 2), std::complex<double>(2, 4), std::complex<double>(3, 6), std::complex<double>(4, 11), std::complex<double>(5, 16), std::complex<double>(0, 9), std::complex<double>(-1, 16), std::complex<double>(-2, 23), std::complex<double>(-3, 12), std::complex<double>(-6, 21), std::complex<double>(-9, 30), std::complex<double>(-6, 15), std::complex<double>(-11, 26), std::complex<double>(-16, 37), std::complex<double>(-9, 18), std::complex<double>(-16, 31), std::complex<double>(-23, 44), std::complex<double>(-12, 21), std::complex<double>(-21, 36), std::complex<double>(-30, 51), std::complex<double>(-15, 24), std::complex<double>(-26, 41), std::complex<double>(-37, 58), std::complex<double>(-18, 27), std::complex<double>(-31, 46), std::complex<double>(-44, 65), std::complex<double>(-21, 30), std::complex<double>(-28, 40), std::complex<double>(-35, 50), std::complex<double>(0, 0), std::complex<double>(0, 0)};
    unsigned numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    int rate = 3;
    filter.filtOperation = ONE_SHOT_RETURN_ALL_RESULTS;
    
    
    interp(buf, rate, filter);
    EXPECT_EQ(input.size()*rate + filter.size() - rate, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
}

TEST(ComplexFirFilter, InterpComplexTrim1) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8), std::complex<double>(-6, 9), std::complex<double>(-7, 10)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(3, 6), std::complex<double>(4, 11), std::complex<double>(5, 16), std::complex<double>(0, 9), std::complex<double>(-1, 16), std::complex<double>(-2, 23), std::complex<double>(-3, 12), std::complex<double>(-6, 21), std::complex<double>(-9, 30), std::complex<double>(-6, 15), std::complex<double>(-11, 26), std::complex<double>(-16, 37), std::complex<double>(-9, 18), std::complex<double>(-16, 31), std::complex<double>(-23, 44), std::complex<double>(-12, 21), std::complex<double>(-21, 36), std::complex<double>(-30, 51), std::complex<double>(-15, 24), std::complex<double>(-26, 41), std::complex<double>(-37, 58), std::complex<double>(-18, 27), std::complex<double>(-31, 46), std::complex<double>(-44, 65), std::complex<double>(-21, 30), std::complex<double>(-28, 40), std::complex<double>(-35, 50), std::complex<double>(0, 0), std::complex<double>(0, 0)};
    unsigned numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    int rate = 3;
    filter.filtOperation = ONE_SHOT_TRIM_TAILS;
    
    interp(buf, rate, filter);
    EXPECT_EQ(input.size()*rate, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
}

TEST(ComplexFirFilter, InterpComplexStream1) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8)};
    std::complex<double> inputData2[] = {std::complex<double>(-6, 9)};
    std::complex<double> inputData3[] = {std::complex<double>(-7, 10), std::complex<double>(-8, 11), std::complex<double>(-9, 12), std::complex<double>(-10, 13), std::complex<double>(-11, 14), std::complex<double>(-12, 15), std::complex<double>(-13, 16), std::complex<double>(-14, 17), std::complex<double>(-15, 18), std::complex<double>(-16, 19)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5};
    std::complex<double> expectedData[] = {std::complex<double>(1, 2), std::complex<double>(2, 4), std::complex<double>(3, 6), std::complex<double>(4, 8), std::complex<double>(5, 13), std::complex<double>(0, 6), std::complex<double>(0, 9), std::complex<double>(0, 12), std::complex<double>(-1, 19), std::complex<double>(-2, 8), std::complex<double>(-3, 12), std::complex<double>(-4, 16), std::complex<double>(-7, 25), std::complex<double>(-4, 10), std::complex<double>(-6, 15), std::complex<double>(-8, 20), std::complex<double>(-13, 31), std::complex<double>(-6, 12), std::complex<double>(-9, 18), std::complex<double>(-12, 24), std::complex<double>(-19, 37), std::complex<double>(-8, 14), std::complex<double>(-12, 21), std::complex<double>(-16, 28), std::complex<double>(-25, 43), std::complex<double>(-10, 16), std::complex<double>(-15, 24), std::complex<double>(-20, 32), std::complex<double>(-31, 49), std::complex<double>(-12, 18), std::complex<double>(-18, 27), std::complex<double>(-24, 36), std::complex<double>(-37, 55), std::complex<double>(-14, 20), std::complex<double>(-21, 30), std::complex<double>(-28, 40), std::complex<double>(-43, 61), std::complex<double>(-16, 22), std::complex<double>(-24, 33), std::complex<double>(-32, 44), std::complex<double>(-49, 67), std::complex<double>(-18, 24), std::complex<double>(-27, 36), std::complex<double>(-36, 48), std::complex<double>(-55, 73), std::complex<double>(-20, 26), std::complex<double>(-30, 39), std::complex<double>(-40, 52), std::complex<double>(-61, 79), std::complex<double>(-22, 28), std::complex<double>(-33, 42), std::complex<double>(-44, 56), std::complex<double>(-67, 85), std::complex<double>(-24, 30), std::complex<double>(-36, 45), std::complex<double>(-48, 60), std::complex<double>(-73, 91), std::complex<double>(-26, 32), std::complex<double>(-39, 48), std::complex<double>(-52, 64), std::complex<double>(-79, 97), std::complex<double>(-28, 34), std::complex<double>(-42, 51), std::complex<double>(-56, 68), std::complex<double>(-85, 103), std::complex<double>(-30, 36), std::complex<double>(-45, 54), std::complex<double>(-60, 72), std::complex<double>(-91, 109), std::complex<double>(-32, 38), std::complex<double>(-48, 57), std::complex<double>(-64, 76), std::complex<double>(-80, 95)};
    int rate = 4;
    
    unsigned numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);

    int numResults = 0;
    numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    interp(buf, rate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData2)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData2, numElements);
    interp(buf, rate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData3)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData3, numElements);
    interp(buf, rate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    EXPECT_LE(18*rate - (filter.size() - 1), numResults);
}

TEST(ComplexFirFilter, ResampleComplex1) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8), std::complex<double>(-6, 9), std::complex<double>(-7, 10), std::complex<double>(-8, 11), std::complex<double>(-9, 12), std::complex<double>(-10, 13), std::complex<double>(-11, 14), std::complex<double>(-12, 15), std::complex<double>(-13, 16), std::complex<double>(-14, 17), std::complex<double>(-15, 18), std::complex<double>(-16, 19), std::complex<double>(-17, 20), std::complex<double>(-18, 21), std::complex<double>(-19, 22), std::complex<double>(-20, 23), std::complex<double>(-21, 24), std::complex<double>(-22, 25), std::complex<double>(-23, 26), std::complex<double>(-24, 27), std::complex<double>(-25, 28)};
    int filterTaps[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    std::complex<double> expectedData[] = {std::complex<double>(1, 2), std::complex<double>(6, 12), std::complex<double>(11, 37), std::complex<double>(-4, 46), std::complex<double>(-15, 51), std::complex<double>(-36, 108), std::complex<double>(-51, 114), std::complex<double>(-60, 114), std::complex<double>(-69, 117), std::complex<double>(-74, 116), std::complex<double>(-75, 111), std::complex<double>(-156, 228), std::complex<double>(-156, 219), std::complex<double>(-150, 204), std::complex<double>(-149, 197), std::complex<double>(-144, 186), std::complex<double>(-135, 171), std::complex<double>(-276, 348), std::complex<double>(-261, 324), std::complex<double>(-240, 294), std::complex<double>(-229, 277), std::complex<double>(-214, 256), std::complex<double>(-195, 231), std::complex<double>(-396, 468), std::complex<double>(-366, 429), std::complex<double>(-330, 384), std::complex<double>(-309, 357), std::complex<double>(-284, 326), std::complex<double>(-255, 291), std::complex<double>(-516, 588), std::complex<double>(-471, 534), std::complex<double>(-420, 474), std::complex<double>(-389, 437), std::complex<double>(-250, 280)};
    unsigned numElements = sizeof(inputData)/sizeof(inputData[0]);
    std::vector< std::complex<double> > scratchBuf;
	NimbleDSP::ComplexVector<double> buf(inputData, numElements, TIME_DOMAIN, &scratchBuf);
    numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    int interpRate = 6;
    int decimateRate = 5;
    filter.filtOperation = ONE_SHOT_RETURN_ALL_RESULTS;
    
    resample(buf, interpRate, decimateRate, filter);
    EXPECT_EQ((input.size()*interpRate + filter.size()-1 - (interpRate-1) + decimateRate-1)/decimateRate, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
}

TEST(ComplexFirFilter, ResampleComplexTrim1) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8), std::complex<double>(-6, 9), std::complex<double>(-7, 10), std::complex<double>(-8, 11), std::complex<double>(-9, 12), std::complex<double>(-10, 13), std::complex<double>(-11, 14), std::complex<double>(-12, 15), std::complex<double>(-13, 16), std::complex<double>(-14, 17), std::complex<double>(-15, 18), std::complex<double>(-16, 19), std::complex<double>(-17, 20), std::complex<double>(-18, 21), std::complex<double>(-19, 22), std::complex<double>(-20, 23), std::complex<double>(-21, 24), std::complex<double>(-22, 25), std::complex<double>(-23, 26), std::complex<double>(-24, 27), std::complex<double>(-25, 28)};
    int filterTaps[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    std::complex<double> expectedData[] = {std::complex<double>(7, 14), std::complex<double>(10, 20), std::complex<double>(13, 32), std::complex<double>(0, 15), std::complex<double>(0, 24), std::complex<double>(0, 33), std::complex<double>(-3, 54), std::complex<double>(-6, 24), std::complex<double>(-9, 36), std::complex<double>(-14, 53), std::complex<double>(-8, 20), std::complex<double>(-14, 35), std::complex<double>(-20, 50), std::complex<double>(-32, 77), std::complex<double>(-15, 30), std::complex<double>(-24, 48), std::complex<double>(-33, 66), std::complex<double>(-54, 105), std::complex<double>(-24, 42), std::complex<double>(-36, 63), std::complex<double>(-53, 92), std::complex<double>(-20, 32), std::complex<double>(-35, 56), std::complex<double>(-50, 80), std::complex<double>(-77, 122), std::complex<double>(-30, 45), std::complex<double>(-48, 72), std::complex<double>(-66, 99), std::complex<double>(-105, 156), std::complex<double>(-42, 60), std::complex<double>(-63, 90), std::complex<double>(-92, 131), std::complex<double>(-32, 44), std::complex<double>(-56, 77), std::complex<double>(-80, 110), std::complex<double>(-122, 167), std::complex<double>(-45, 60), std::complex<double>(-72, 96), std::complex<double>(-99, 132), std::complex<double>(-156, 207), std::complex<double>(-60, 78), std::complex<double>(-90, 117), std::complex<double>(-131, 170), std::complex<double>(-44, 56), std::complex<double>(-77, 98), std::complex<double>(-110, 140), std::complex<double>(-167, 212), std::complex<double>(-60, 75), std::complex<double>(-96, 120), std::complex<double>(-132, 165), std::complex<double>(-207, 258), std::complex<double>(-78, 96), std::complex<double>(-117, 144), std::complex<double>(-170, 209), std::complex<double>(-56, 68), std::complex<double>(-98, 119), std::complex<double>(-140, 170), std::complex<double>(-212, 257), std::complex<double>(-75, 90), std::complex<double>(-120, 144), std::complex<double>(-165, 198), std::complex<double>(-258, 309), std::complex<double>(-96, 114), std::complex<double>(-144, 171), std::complex<double>(-209, 248), std::complex<double>(-68, 80), std::complex<double>(-119, 140), std::complex<double>(-170, 200), std::complex<double>(-257, 302), std::complex<double>(-90, 105), std::complex<double>(-144, 168), std::complex<double>(-198, 231), std::complex<double>(-309, 360), std::complex<double>(-114, 132), std::complex<double>(-171, 198), std::complex<double>(-248, 287), std::complex<double>(-80, 92), std::complex<double>(-140, 161), std::complex<double>(-200, 230), std::complex<double>(-302, 347), std::complex<double>(-105, 120), std::complex<double>(-168, 192), std::complex<double>(-231, 264), std::complex<double>(-360, 411), std::complex<double>(-132, 150), std::complex<double>(-198, 225), std::complex<double>(-287, 326), std::complex<double>(-92, 104), std::complex<double>(-161, 182), std::complex<double>(-230, 260), std::complex<double>(-347, 392), std::complex<double>(-120, 135), std::complex<double>(-192, 216), std::complex<double>(-264, 297), std::complex<double>(-411, 462), std::complex<double>(-150, 168), std::complex<double>(-225, 252), std::complex<double>(-300, 336), std::complex<double>(0, 0)};
    unsigned numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);
    NimbleDSP::ComplexVector<double> input = buf;
    int interpRate = 11;
    int decimateRate = 3;
    filter.filtOperation = ONE_SHOT_TRIM_TAILS;
    
    resample(buf, interpRate, decimateRate, filter, true);
    EXPECT_EQ((input.size()*interpRate + decimateRate-1)/decimateRate, buf.size());
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
}

TEST(ComplexFirFilter, ResampleComplexStream1) {
    std::complex<double> inputData[] = {std::complex<double>(1, 2), std::complex<double>(0, 3), std::complex<double>(-1, 4), std::complex<double>(-2, 5), std::complex<double>(-3, 6), std::complex<double>(-4, 7), std::complex<double>(-5, 8)};
    std::complex<double> inputData2[] = {std::complex<double>(-6, 9)};
    std::complex<double> inputData3[] = {std::complex<double>(-7, 10)};
    std::complex<double> inputData4[] = {std::complex<double>(-8, 11)};
    std::complex<double> inputData5[] = {std::complex<double>(-9, 12)};
    std::complex<double> inputData6[] = {std::complex<double>(-10, 13)};
    std::complex<double> inputData7[] = {std::complex<double>(-11, 14), std::complex<double>(-12, 15), std::complex<double>(-13, 16), std::complex<double>(-14, 17), std::complex<double>(-15, 18), std::complex<double>(-16, 19), std::complex<double>(-17, 20), std::complex<double>(-18, 21), std::complex<double>(-19, 22), std::complex<double>(-20, 23), std::complex<double>(-21, 24), std::complex<double>(-22, 25), std::complex<double>(-23, 26)};
    std::complex<double> inputData8[] = {std::complex<double>(-24, 27), std::complex<double>(-25, 28), std::complex<double>(-26, 29), std::complex<double>(-27, 30), std::complex<double>(-28, 31), std::complex<double>(-29, 32), std::complex<double>(-30, 33), std::complex<double>(-31, 34), std::complex<double>(-32, 35), std::complex<double>(-33, 36), std::complex<double>(-34, 37), std::complex<double>(-35, 38)};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    std::complex<double> expectedData[] = {std::complex<double>(1, 2), std::complex<double>(6, 12), std::complex<double>(11, 22), std::complex<double>(0, 15), std::complex<double>(0, 30), std::complex<double>(-4, 16), std::complex<double>(-9, 36), std::complex<double>(-20, 71), std::complex<double>(-16, 40), std::complex<double>(-32, 77), std::complex<double>(-21, 42), std::complex<double>(-40, 79), std::complex<double>(-24, 42), std::complex<double>(-44, 77), std::complex<double>(-25, 40), std::complex<double>(-50, 80), std::complex<double>(-24, 36), std::complex<double>(-54, 81), std::complex<double>(-105, 156), std::complex<double>(-56, 80), std::complex<double>(-107, 152), std::complex<double>(-56, 77), std::complex<double>(-105, 144), std::complex<double>(-54, 72), std::complex<double>(-99, 132), std::complex<double>(-50, 65), std::complex<double>(-100, 130), std::complex<double>(-44, 56), std::complex<double>(-99, 126), std::complex<double>(-190, 241), std::complex<double>(-96, 120), std::complex<double>(-182, 227), std::complex<double>(-91, 112), std::complex<double>(-170, 209), std::complex<double>(-84, 102), std::complex<double>(-154, 187), std::complex<double>(-75, 90), std::complex<double>(-150, 180), std::complex<double>(-64, 76), std::complex<double>(-144, 171), std::complex<double>(-275, 326), std::complex<double>(-136, 160), std::complex<double>(-257, 302), std::complex<double>(-126, 147), std::complex<double>(-235, 274), std::complex<double>(-114, 132), std::complex<double>(-209, 242), std::complex<double>(-100, 115), std::complex<double>(-200, 230), std::complex<double>(-84, 96), std::complex<double>(-189, 216), std::complex<double>(-360, 411), std::complex<double>(-176, 200), std::complex<double>(-332, 377), std::complex<double>(-161, 182), std::complex<double>(-300, 339), std::complex<double>(-144, 162), std::complex<double>(-264, 297), std::complex<double>(-125, 140), std::complex<double>(-250, 280), std::complex<double>(-104, 116), std::complex<double>(-234, 261), std::complex<double>(-445, 496), std::complex<double>(-216, 240), std::complex<double>(-407, 452), std::complex<double>(-196, 217), std::complex<double>(-365, 404), std::complex<double>(-174, 192), std::complex<double>(-319, 352), std::complex<double>(-150, 165), std::complex<double>(-300, 330), std::complex<double>(-124, 136), std::complex<double>(-279, 306), std::complex<double>(-530, 581), std::complex<double>(-256, 280), std::complex<double>(-482, 527), std::complex<double>(-231, 252), std::complex<double>(-430, 469), std::complex<double>(-204, 222), std::complex<double>(-374, 407), std::complex<double>(-175, 190), std::complex<double>(-350, 380)};
    int interpRate = 11;
    int decimateRate = 5;
    
    unsigned numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);

    int numResults = 0;
    numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData2)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData2, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData3)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData3, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData4)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData4, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData5)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData5, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData6)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData6, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData7)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData7, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData8)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData8, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    EXPECT_LT((37*interpRate - (filter.size()-1)) / decimateRate, numResults);
}

TEST(ComplexFirFilter, ResampleComplexStream2) {
    std::complex<double> inputData[] = {1, 0, -1, -2, -3, -4, -5};
    std::complex<double> inputData2[] = {-6};
    std::complex<double> inputData3[] = {-7, -8, -9, -10, -11, -12, -13, -14, -15, -16};
    std::complex<double> filterTaps[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    std::complex<double> expectedData[] = {1, 5, 9, 13, 17, 18, 18, -15, -27, -42, -66, -63, -99, -83, -126, -174, -147, -207, -162, -234, -171, -255, -183, -270, -366, -279, -387, -282, -402, -279, -411, -283, -414, -558, -411, -567, -402, -570, -387, -567, -383, -507, -631, -320, -384};
    int interpRate = 9;
    int decimateRate = 4;
    
    unsigned numElements = sizeof(filterTaps)/sizeof(filterTaps[0]);
    NimbleDSP::ComplexFirFilter<double> filter(filterTaps, numElements);

    int numResults = 0;
    numElements = sizeof(inputData)/sizeof(inputData[0]);
	NimbleDSP::ComplexVector<double> buf(inputData, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData2)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData2, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    
    numElements = sizeof(inputData3)/sizeof(inputData[0]);
	buf = NimbleDSP::ComplexVector<double>(inputData3, numElements);
    resample(buf, interpRate, decimateRate, filter);
    for (unsigned i=0; i<buf.size(); i++) {
        EXPECT_EQ(expectedData[i + numResults], buf[i]);
    }
    numResults += buf.size();
    EXPECT_LE(18*interpRate / decimateRate, numResults);
}
