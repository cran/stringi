/* This file is part of the 'stringi' package for R.
 * Copyright (c) 2013-2014, Marek Gagolewski and Bartek Tartanus
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "stri_stringi.h"

// this function will be available in version 0.2

//
///**
// * Pad a string
// *
// * vectorized over str, length and pad
// * if str or pad or length is NA the result will be NA
// *
// * @param str character vector
// * @param length integer vector
// * @param pad character vector
// * @return character vector
// *
// * @version 0.1 (Bartlomiej Tartanus)
// * @version 0.2 (Bartlomiej Tartanus, 2013-06-21) uses StriContainerUTF16 & ICU's padLeading
//*/
//SEXP stri_pad(SEXP str, SEXP length, SEXP pad)
//{
//   str    = stri_prepare_arg_string(str, "str"); // prepare string argument
//   length = stri_prepare_arg_integer(length, "length");
//   pad    = stri_prepare_arg_string(pad, "pad");
//
//   R_len_t vectorize_length = stri__recycling_rule(true, 3, LENGTH(str), LENGTH(length), LENGTH(pad));
//
//   SEXP ret;
//   PROTECT(ret = allocVector(STRSXP, vectorize_length));
//
//   STRI__ERROR_HANDLER_BEGIN
//   StriContainerUTF16 str_cont(str, vectorize_length, false);
//   StriContainerUTF16 pad_cont(pad, vectorize_length);
//   StriContainerInteger length_cont(length, vectorize_length);
//
//   for (R_len_t i = 0; i < vectorize_length; i++)
//   {
//      if (pad_cont.isNA(i) || str_cont.isNA(i) || length_cont.isNA(i)) {
//         SET_STRING_ELT(ret, i, NA_STRING);
//         continue;
//      }
//
//      if (pad_cont.get(i).length() > 0) {
//         UChar cur_pad = (pad_cont.get(i))[0]; // This is Uchar - 16 bit.....
//         str_cont.getWritable(i).padLeading(length_cont.get(i), cur_pad);
//      }
//
//      SET_STRING_ELT(ret, i, str_cont.toR(i));
//   }
//
//   UNPROTECT(1);
//   return ret;
//   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
//}



// OLD VERSION

///**
// * Pad a string
// *
// * vectorized over s, width and side
// *  if s is NA the result will be NA
// *
// * @param s
// * @param width
// * @param side
// * @param pad
// * @return character vector
// *
// * @version 0.1 (Bartek Tartanus)
//*/
//SEXP stri_pad(SEXP s, SEXP width, SEXP side, SEXP pad)
//{
//   s     = stri_prepare_arg_string(s, "str"); // prepare string argument
//   width = stri_prepare_arg_integer(width, "width");
//   side  = stri_prepare_arg_integer(side, "side");
//   pad   = stri_prepare_arg_string(pad, "pad");
//
//   R_len_t ns     = LENGTH(s);
//   R_len_t nside  = LENGTH(side);
//   R_len_t nwidth = LENGTH(width);
//   R_len_t np     = LENGTH(pad);
//
//   //check if pad is single character
//   int* iplen = INTEGER(stri_length(pad));
//   for(R_len_t i=0; i<np; ++i){
//      if(iplen[i] != 1)
//         error("pad must be single character");
//   }
//
//   R_len_t nmax = stri__recycling_rule(true, 4, ns, nside, nwidth, np);
//
//   int needed=0;
//   SEXP ret, curs;
//   PROTECT(ret = allocVector(STRSXP, nmax));
//
//   int* iwidth = INTEGER(width);
//   int* iside  = INTEGER(side);
//   int* islen  = INTEGER(stri_length(s));
//   int* isnum  = INTEGER(stri_numbytes(s));
//   int* ipnum  = INTEGER(stri_numbytes(pad));
//
//   for (R_len_t i=0; i<nmax; ++i){
//      curs = STRING_ELT(s, i % ns);
//      const char* p = CHAR(STRING_ELT(pad, i % np));
//      if(curs == NA_STRING || iwidth[i % nwidth] == NA_INTEGER){
//         SET_STRING_ELT(ret, i, NA_STRING);
//         continue;
//      }
//      //if current string is long enough - return with no change
//      needed = max(0, iwidth[i % nwidth] - islen[i % ns]);
//      if(needed == 0){
//         SET_STRING_ELT(ret, i, curs);
//         continue;
//      }
//      char* buf = R_alloc(isnum[i%ns]+ipnum[i%np]*needed, (int)sizeof(char));
//      char* buf2 = buf;
//      switch(iside[i % nside]){
//         //pad from left
//         case 1:
//         for(int j=0; j<needed; ++j){
//            memcpy(buf2, p, (size_t)ipnum[i%np]);
//            buf2 += ipnum[i%np];
//         }
//         memcpy(buf2, CHAR(curs), (size_t)isnum[i % ns]);
//         break;
//         //right
//         case 2:
//         memcpy(buf2, CHAR(curs), (size_t)isnum[i % ns]);
//         buf2 += isnum[i % ns];
//         for(int j=0; j<needed; ++j){
//            memcpy(buf2, p, (size_t)ipnum[i%np]);
//            buf2 += ipnum[i%np];
//         }
//         break;
//         //both
//         case 3:
//         for(int j=0; j<floor(needed/2); ++j){
//            memcpy(buf2, p, (size_t)ipnum[i%np]);
//            buf2 += ipnum[i%np];
//         }
//         memcpy(buf2, CHAR(curs), (size_t)isnum[i % ns]);
//         buf2 += isnum[i % ns];
//         for(int j=0; j<ceil(double(needed)/2); ++j){
//            memcpy(buf2, p, (size_t)ipnum[i%np]);
//            buf2 += ipnum[i%np];
//         }
//         break;
//      }
//      SET_STRING_ELT(ret, i, mkCharLen(buf, isnum[i%ns]+ipnum[i%np]*needed));
//   }
//
//   UNPROTECT(1);
//   return ret;
//}
