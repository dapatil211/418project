/*
The MIT License (MIT)

Copyright (c) 2014 Mark Thomas Nelson

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

 This code was written to illustrate the article:
 Data Compression With Arithmetic Coding
 by Mark Nelson
 published at:
http://marknelson.us/2014/10/19/data-compression-with-arithmetic-coding

*/
#ifndef DECOMPESSOR_DOT_H
#define DECOMPESSOR_DOT_H

#ifdef LOG
#include <iomanip>
#include <iostream>
#endif

// #include "bitio.h"
// #include "byteio.h"
#include "model.h"
//
// The arithmetic decompressor is a general purpose decompressor that
// is parameterized on the types of the input, output, and
// model objects, in an attempt to make it as flexible as
// possible. It is easiest to use by calling the compress()
// convenience function found at the bottom of this header file
//
// The INPUT class is expected to provide a get_bit() function,
// while the output function is expected to provider a put_byte()
// function. Both of these functions should throw exceptions on
// errors. We expect the EOF to be embedded in the compressed
// stream, so it needs to be extracted by the decoder. If the
// compression goes awry, the get_bit() function will be
// repeatedly called on EOF(), in which case it would be good
// for it to return an error.
//

void convertToBits(vector<char> &input, vector<bool> &bits, int startInd, int endInd){
  for(int i = startInd; i < endInd; i++){
    char c = input[i];
    int mask = 1 << 7;
    for(int j = 0; j < 8; j ++){
      bits.push_back(c | mask);
      mask >>= 1;
    }
  }
}

void decompress(vector<char> &input, vector<char> &output, Model &model, int startInd, int endInd) {
#ifdef LOG
  std::ofstream log("decompressor.log");
  log << std::hex;
#endif
  vector<bool> bits;
  convertToBits(input, bits, startInd, endInd);
  uint high = Model::MAX;
  uint low = 0;
  uint value = 0;
  uint bitIndex = 0;
  for (; bitIndex < Model::CODE_VALUE_BITS; bitIndex++) {
    value <<= 1;
    value += bits[bitIndex] ? 1 : 0;
  }
  for (; bitIndex < bits.size() + 1; bitIndex ++) {
    uint range = high - low + 1;
    cout << "RANGE: " << range << " VALUE: " << value << endl;
    uint scaled_value =
        ((value - low + 1) * model.getCount() - 1) / range;
    unsigned char c;
    auto p = model.getChar(scaled_value, c);
    // if (c == 256)
    //   break;
    if(get<0>(p)){
      output.push_back(c);
    }
#ifdef LOG
    log << std::hex << "0x" << std::setw(2) << std::setfill('0') << c;
    if (c > 0x20 && c <= 0x7f)
      log << "(" << char(c) << ")";
    log << " 0x" << low << " 0x" << high << " => ";
#endif

    high = low + (range * get<1>(p)) / get<2>(p) - 1;
    low = low + (range * get<0>(p)) / get<2>(p);
#ifdef LOG
    log << "0x" << low << " 0x" << high << "\n";
#endif
    for (;;) {
      if (high < Model::ONE_HALF) {
        // do nothing, bit is a zero
      } else if (low >= Model::ONE_HALF) {
        value -= Model::ONE_HALF; // subtract one half from all three code
                                  // values
        low -= Model::ONE_HALF;
        high -= Model::ONE_HALF;
      } else if (low >= Model::ONE_FOURTH && high < Model::THREE_FOURTHS) {
        value -= Model::ONE_FOURTH;
        low -= Model::ONE_FOURTH;
        high -= Model::ONE_FOURTH;
      } else
        break;
      low <<= 1;
      high <<= 1;
      high++;
      value <<= 1;
      value += (bitIndex < bits.size() && bits[bitIndex]) ? 1 : 0;
    }
  }
#ifdef LOG
  log << std::hex << "0x" << std::setw(2) << std::setfill('0') << 256;
  log << " 0x" << low << " 0x" << high << "\n";
#endif
}

//
// This convenience function takes care of
// constructing the decompressor and the
// input and output objects, then calling
// the decompressor.
//

#endif //#ifndef DECOMPESSOR_DOT_H
