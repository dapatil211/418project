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
 published at: http://marknelson.us/2014/10/19/data-compression-with-arithmetic-coding

*/
#include <iostream>
#include <fstream>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <exception>
#include <iostream>
#include <algorithm>
#include <omp.h>

#include "model.h"
#include "aricompressor.h"

int num_of_threads = 2;


static void run_compress(
        string &infile_name,
        bool is_seq,
        bool check_correctness){

    struct stat sbuf;
    stat(infile_name.c_str(), &sbuf);
    size_t file_size = sbuf.st_size;
    unsigned char* in_data = new unsigned char[file_size];

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        FILE* in_file = fopen(infile_name.c_str(), "rb");
        size_t chunk_size = UPDIV(file_size, num_of_threads);
        size_t start_offset = tid * chunk_size;
        size_t end_offset = min(start_offset + chunk_size, file_size);
        fseek(in_file, start_offset, SEEK_SET);
        fread(in_data + start_offset, 1,end_offset - start_offset, in_file);
        fclose(in_file);
    }
    // Buffer that stores input file bytes
    data_buf in_buf(in_data, file_size);
    // Buffer that store compressed bytes
    data_buf tmp_buf;
    // Buffer that stores decompressed bytes. It should be the same as input bytes
    data_buf out_buf;

    // Encode input buffer to intermediate buffer
    string tmpfile_name;
    if (is_seq) {
        tmpfile_name = "compressed_seq";
        compress(in_buf, tmp_buf, c_model, start_offset, end_offset);
    } else {
        tmpfile_name = "compressed_parallel";
        compress(in_buf, tmp_buf, c_model, start_offset, end_offset);
    }

    if (check_correctness) {
        // Write the intermediate result to the file
        FILE *tmp_file = fopen(tmpfile_name.c_str(), "wb");
        fwrite(tmp_buf.data, 1, tmp_buf.size, tmp_file);
        fclose(tmp_file);
    }





}


long get_file_size(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}



int main(int argc, char* argv[])
{
  if ( argc < 3 ) {
    std::cerr << "missing command line arguments\n";
    return 255;
  }
  try {
    //std::ifstream input(argv[1], std::ifstream::binary);
    //std::ofstream output(argv[2], std::ofstream::binary);
    //modelA<int, 16, 14> cmodel;
    //cmodel.dump("cmodel", std::clog);
    string infile_name = argv[1];
    string outfile_name = argv[2];

    std::cout << "compressing...\n";

    //parallel stuff that'll hopefully work in the future
    //omp_set_num_threads(num_of_threads);
    //run_compress(infile_name, true, false);

    infile_size = get_file_size(infile_name);



    std::cout << cmodel.m_bytesProcessed << "\n";


    return 0;
  }
  catch (std::exception &ex)
  {
    std::cerr << "Failed with exception: " << ex.what() << "\n";
  }
  return 255;
}
