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
#ifndef COMPRESSOR_DOT_H
#define COMPRESSOR_DOT_H

#include "model.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <bitset>
using namespace std;
#ifdef LOG
#include <iomanip>
#endif


#if OMP
#include <omp.h>
#endif


//
// The arithmetic compressor is a general purpose compressor that
// is parameterized on the types of the input, output, and
// model objects, in an attempt to make it as flexible as
// possible. It is easiest to use by calling the compress()
// convenience function found at the bottom of this header file
//

typedef vector<tuple<double, double, double>> prob_vect;

void convertToChars(vector<bool> &bits, vector<char> &m_output) {
  char buffer = 0;
  for (uint i = 0; i < bits.size(); i++) {
    buffer |= (bits[i] << (7 - (i % 8)));
    if (i % 8 == 7) {
      m_output.push_back(buffer);
      buffer = 0;
    }
  }
  if (bits.size() % 8 != 0) {
    m_output.push_back(buffer);
  }
}

inline void put_bit_plus_pending(bool bit, int &pending_bits,
                                 vector<bool> &bits) {
  bits.push_back(bit);
  for (int i = 0; i < pending_bits; i++)
    bits.push_back(!bit);
  pending_bits = 0;
}

void compress(vector<char> &input, vector<char> &output, int startInd, int endInd,
         Model &model) {
#ifdef LOG
  std::ofstream log("compressor.log");
  log << std::dec;
#endif
  int pending_bits = 0;
  uint low = 0;
  uint high = Model::MAX;
  vector<bool> bits;
  for(int file_index = startInd; file_index <= endInd; file_index++) {
    char c = input[file_index];
    if(file_index % 100000 == 0){
#if OMP
      cout << "tid " << omp_get_thread_num() << ": " << file_index << endl;
#else
      cout << file_index << endl;
#endif
    }
    if (c == -1)
      c = 255;
#ifdef LOG
    log << std::dec  << std::setw(2) << std::setfill('0') << c;
    if (c > 0x20 && c <= 0x7f)
      log << "(" << char(c) << ")";
    log << low  << " " << high << " " << bitset<17>(low) << " " << bitset<17>(high) << " => ";
#endif
    // prob p = m_model.getProbability( c );
    prob_vect p = model.getProbability(c);
    for (auto i = p.begin(); i != p.end(); ++i) {
      if (get<2>(*i) == 0) {
        continue;
      }
      // cout << get<0>(*i) << " " <<get<1>(*i) << " " <<get<2>(*i)  << endl;
      uint range = high - low + 1;
      // high = low + (range * p.high / p.count) - 1;
      high = low + (range * (uint) (get<1>(*i) * 2) / (uint)(get<2>(*i)*2)) - 1;
      // low = low + (range * p.low / p.count);
      low = low + (range * (uint)(get<0>(*i)*2) / (uint)(get<2>(*i)*2));
#ifdef LOG
      log << "NEW" << endl;
      log  << low << " " <<  high << " " << bitset<17>(low) << " " << bitset<17>(high) << endl;
#endif
      //
      // On each pass there are six possible configurations of high/low,
      // each of which has its own set of actions. When high or low
      // is converging, we output their MSB and upshift high and low.
      // When they are in a near-convergent state, we upshift over the
      // next-to-MSB, increment the pending count, leave the MSB intact,
      // and don't output anything. If we are not converging, we do
      // no shifting and no output.
      // high: 0xxx, low anything : converging (output 0)
      // low: 1xxx, high anything : converging (output 1)
      // high: 10xxx, low: 01xxx : near converging
      // high: 11xxx, low: 01xxx : not converging
      // high: 11xxx, low: 00xxx : not converging
      // high: 10xxx, low: 00xxx : not converging
      //
      for (;;) {
        if (high < Model::ONE_HALF)
          put_bit_plus_pending(0, pending_bits, bits);
        else if (low >= Model::ONE_HALF)
          put_bit_plus_pending(1, pending_bits, bits);
        else if (low >= Model::ONE_FOURTH && high < Model::THREE_FOURTHS) {
          pending_bits++;
          low -= Model::ONE_FOURTH;
          high -= Model::ONE_FOURTH;
        } else
          break;
        high <<= 1;
        high++;
        low <<= 1;
//         #ifdef LOG
//       log  << low << " " <<  high << " " << bitset<17>(low) << " " << bitset<17>(high) << endl;
// #endif
        high &= Model::MAX;
        low &= Model::MAX;
        #ifdef LOG
      log  << low << " " <<  high << " " << bitset<17>(low) << " " << bitset<17>(high) << endl;
#endif
      }
    }
    model.updateModel(c);
    if (c == 256) // 256 is the special EOF code
      break;
  }
  pending_bits++;
  if (low < Model::ONE_FOURTH)
    put_bit_plus_pending(0, pending_bits, bits);
  else
    put_bit_plus_pending(1, pending_bits, bits);
  cout << "# bits = " << bits.size() << endl;
  convertToChars(bits, output);
#ifdef LOG
  log.close();
#endif
}



//
// This convenience function takes care of
// constructing the compressor and the
// input and output objects, then calling
// the compressor. Letting the user of the class
// call a template function instead of instantating
// the template class object eases syntax
// requirements a bit.
//

#endif //#ifndef COMPRESSOR_DOT_H
