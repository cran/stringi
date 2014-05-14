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
#include "stri_container_base.h"
#include "stri_container_utf8.h"
#include "stri_container_integer.h"
#include "stri_container_listutf8.h"
#include "stri_string8buf.h"
#include <vector>
using namespace std;


/** Duplicate given strings
 *
 *
 *  @param str character vector
 *  @param times integer vector
 *  @return character vector
 *
 *  The function is vectorized over str and times
 *  if str is NA or times is NA the result will be NA
 *  if times<0 the result will be NA
 *  if times==0 the result will be an empty string
 *  if str or times is an empty vector then the result is an empty vector
 *
 * @version 0.1-?? (Marek Gagolewski)
 *
 * @version 0.1-?? (Marek Gagolewski)
 *                  use StriContainerUTF8's vectorization
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-15)
 *                  use StriContainerInteger
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-16)
 *                  make StriException friendly
*/
SEXP stri_dup(SEXP str, SEXP times)
{
   str = stri_prepare_arg_string(str, "str"); // prepare string argument
   times = stri_prepare_arg_integer(times, "times"); // prepare string argument
   R_len_t vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(times));
   if (vectorize_length <= 0) return Rf_allocVector(STRSXP, 0);

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 str_cont(str, vectorize_length);
   StriContainerInteger times_cont(times, vectorize_length);

   // STEP 1.
   // Calculate the required buffer length
   R_len_t bufsize = 0;
   for (R_len_t i=0; i<vectorize_length; ++i) {
      if (str_cont.isNA(i) || times_cont.isNA(i))
         continue;

      R_len_t cursize = times_cont.get(i) * str_cont.get(i).length();
      if (cursize > bufsize)
         bufsize = cursize;
   }

   // STEP 2.
   // Alloc buffer & result vector
   String8buf buf(bufsize);
   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length));

   // STEP 3.
   // Duplicate
   const String8* str_last = NULL; // this will allow for reusing buffer...
   R_len_t str_last_index  = 0;    // ...useful for stri_dup('a', 1:1000) or stri_dup('a', 1000:1)

   for (R_len_t i = str_cont.vectorize_init(); // this iterator allows for...
         i != str_cont.vectorize_end();        // ...smart buffer reusage
         i = str_cont.vectorize_next(i))
   {
      R_len_t times_cur;
      if (str_cont.isNA(i) || times_cont.isNA(i) || (times_cur = times_cont.get(i)) < 0) {
         SET_STRING_ELT(ret, i, NA_STRING);
         continue;
      }

      const String8* str_cur = &(str_cont.get(i));
      R_len_t str_cur_n = str_cur->length();
      if (times_cur <= 0 || str_cur_n <= 0) {
         SET_STRING_ELT(ret, i, Rf_mkCharLen("", 0));
         continue;
      }

      // all right, here the result will neither be NA nor an empty string

      if (str_cur != str_last) {
         // well, no reuse possible - resetting
         str_last = str_cur;
         str_last_index = 0;
      }

      // we paste only "additional" duplicates
      R_len_t max_index = str_cur_n*times_cur;
      for (; str_last_index < max_index; str_last_index += str_cur_n) {
         memcpy(buf.data()+str_last_index, str_cur->c_str(), (size_t)str_cur_n);
      }

      // the result is always in UTF-8
      SET_STRING_ELT(ret, i, Rf_mkCharLenCE(buf.data(), max_index, CE_UTF8));
   }


   // STEP 4.
   // Clean up & finish

   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/** Join two character vectors, element by element, no separator, no collapse
 *
 * Vectorized over e1 and e2. Optimized for |e1| >= |e2|
 * (but no harm otherwise)
 *
 * This is used by %+% operator in stringi R code.
 *
 * @param e1 character vector
 * @param e2 character vector
 * @return character vector, res_i=s1_i + s2_i for |e1|==|e2|
 *  if e1 or e2 is NA then result is NA
 *  if e1 or e2 is empty, then the result is just e1 or e2
 *
 *
 * @version 0.1-?? (Marek Gagolewski)
 *
 * @version 0.1-?? (Marek Gagolewski)
 *          use StriContainerUTF8's vectorization
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-16)
 *          make StriException friendly
*/
SEXP stri_join2_nocollapse(SEXP e1, SEXP e2)
{
   e1 = stri_prepare_arg_string(e1, "e1"); // prepare string argument
   e2 = stri_prepare_arg_string(e2, "e2"); // prepare string argument

   R_len_t e1_length = LENGTH(e1);
   R_len_t e2_length = LENGTH(e2);
   R_len_t vectorize_length = stri__recycling_rule(true, 2, e1_length, e2_length);

   if (e1_length <= 0) return e1;
   if (e2_length <= 0) return e2;

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 e1_cont(e1, vectorize_length);
   StriContainerUTF8 e2_cont(e2, vectorize_length);

   // 1. find maximal length of the buffer needed
   R_len_t nchar = 0;
   for (int i=0; i<vectorize_length; ++i) {
      if (e1_cont.isNA(i) || e2_cont.isNA(i))
         continue;

      R_len_t c1 = e1_cont.get(i).length();
      R_len_t c2 = e2_cont.get(i).length();

      if (c1+c2 > nchar) nchar = c1+c2;
   }

   // 2. Create buf & retval
   String8buf buf(nchar);
   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length)); // output vector

   // 3. Set retval
   const String8* last_string_1 = NULL;
   R_len_t last_buf_idx = 0;
   for (R_len_t i = e1_cont.vectorize_init(); // this iterator allows for...
         i != e1_cont.vectorize_end();        // ...smart buffer reusage
         i = e1_cont.vectorize_next(i))
   {
      if (e1_cont.isNA(i) || e2_cont.isNA(i)) {
         SET_STRING_ELT(ret, i, NA_STRING);
         continue;
      }

      // If e1 has length < length of e2, this will be faster:
      const String8* cur_string_1 = &(e1_cont.get(i));
      if (cur_string_1 != last_string_1) {
         last_string_1 = cur_string_1;
         last_buf_idx = cur_string_1->length();
         memcpy(buf.data(), cur_string_1->c_str(), (size_t)last_buf_idx);
      }
      // else reuse string #1

      const String8* cur_string_2 = &(e2_cont.get(i));
      R_len_t  cur_len_2 = cur_string_2->length();
      memcpy(buf.data()+last_buf_idx, cur_string_2->c_str(), (size_t)cur_len_2);
      // the result is always in UTF-8
      SET_STRING_ELT(ret, i, Rf_mkCharLenCE(buf.data(), last_buf_idx+cur_len_2, CE_UTF8));
   }

   // 4. Cleanup & finish
   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/** Join and flatten two character vectors, no separator between elements but possibly with collapse
 *
 * Vectorized over e1 and e2.
 *
 * @param e1 character vector
 * @param e2 character vector
 * @param collapse single string or NULL
 * @return character vector
 *
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-03-18)
 *          first version;
 *          This is much faster than stri_flatten(stri_join2(...), ...)
*/
SEXP stri_join2_withcollapse(SEXP e1, SEXP e2, SEXP collapse)
{
   e1 = stri_prepare_arg_string(e1, "e1"); // prepare string argument
   e2 = stri_prepare_arg_string(e2, "e2"); // prepare string argument

   if (isNull(collapse)) {
      // no collapse - used e.g. by %+% operator
      return stri_join2_nocollapse(e1, e2);
   }

   collapse = stri_prepare_arg_string_1(collapse, "collapse");
   if (STRING_ELT(collapse, 0) == NA_STRING)
      return stri__vector_NA_strings(1);


   R_len_t e1_length = LENGTH(e1);
   R_len_t e2_length = LENGTH(e2);
   R_len_t vectorize_length = stri__recycling_rule(true, 2, e1_length, e2_length);

   if (e1_length <= 0) return e1;
   if (e2_length <= 0) return e2;

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 e1_cont(e1, vectorize_length);
   StriContainerUTF8 e2_cont(e2, vectorize_length);
   StriContainerUTF8 collapse_cont(collapse, 1);
   R_len_t collapse_nbytes = collapse_cont.get(0).length();
   const char* collapse_s = collapse_cont.get(0).c_str();


   // find maximal length of the buffer needed:
   R_len_t nchar = 0;
   for (int i=0; i<vectorize_length; ++i) {
      if (e1_cont.isNA(i) || e2_cont.isNA(i))
         return stri__vector_NA_strings(1); // at least 1 NA => return NA

      nchar += e1_cont.get(i).length() + e2_cont.get(i).length()
               + ((i>0)?collapse_nbytes:0);
   }


   String8buf buf(nchar);
   R_len_t last_buf_idx = 0;
   for (R_len_t i = e1_cont.vectorize_init(); // this iterator allows for...
         i != e1_cont.vectorize_end();        // ...smart buffer reusage
         i = e1_cont.vectorize_next(i))
   {
      // not need to detect NAs - they already have been excluded

      if (collapse_nbytes > 0 && i > 0) { // copy collapse (separator)
         memcpy(buf.data()+last_buf_idx, collapse_s, (size_t)collapse_nbytes);
         last_buf_idx += collapse_nbytes;
      }

      const String8* cur_string_1 = &(e1_cont.get(i));
      R_len_t  cur_len_1 = cur_string_1->length();
      memcpy(buf.data()+last_buf_idx, cur_string_1->c_str(), (size_t)cur_len_1);
      last_buf_idx += cur_len_1;

      const String8* cur_string_2 = &(e2_cont.get(i));
      R_len_t  cur_len_2 = cur_string_2->length();
      memcpy(buf.data()+last_buf_idx, cur_string_2->c_str(), (size_t)cur_len_2);
      last_buf_idx += cur_len_2;
   }

   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, 1)); // output vector
   SET_STRING_ELT(ret, 0, Rf_mkCharLenCE(buf.data(), last_buf_idx, CE_UTF8));
   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/**
 * Concatenate Character Vectors, possibly with collapse
 *
 * @param strlist list of character vectors
 * @param sep single string
 * @param collapse single string or NULL
 * @return character vector
 *
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-03-18)
 *          a specialized version of the original stri_join, which
 *          called stri_flatten at the end, if it was requested;
 *          now collapsing is done directly (for time and memory efficiency);
 *          Now calling specialized functions
 *          stri_join2_withcollapse and stri_flatten_withressep, if needed.
 *          If collapse!=NULL and sep=NA, then the result will be single NA
 *          (and not n*NA);
 */
SEXP stri_join_withcollapse(SEXP strlist, SEXP sep, SEXP collapse)
{
   // no collapse-case is handled separately:
   if (isNull(collapse))
      return stri_join_nocollapse(strlist, sep);

   // *result will surely be a single string*

   strlist = stri_prepare_arg_list_string(strlist, "...");
   R_len_t strlist_length = LENGTH(strlist);
   if (strlist_length <= 0) return stri__vector_empty_strings(0);

   sep = stri_prepare_arg_string_1(sep, "sep");
   collapse = stri_prepare_arg_string_1(collapse, "collapse");
   if (STRING_ELT(sep, 0) == NA_STRING || STRING_ELT(collapse, 0) == NA_STRING) {
      return stri__vector_NA_strings(1);
   }

   // * special cases: *
   if (LENGTH(STRING_ELT(sep, 0)) == 0 && strlist_length == 2) {
      // sep==empty string and 2 vectors --
      // an often occuring case - we have some specialized functions for this :-)
      return stri_join2_withcollapse(VECTOR_ELT(strlist, 0), VECTOR_ELT(strlist, 1), collapse);
   }
   else if (strlist_length == 1) {
      // one vector + collapse string -- another frequently occuring case
      // sep is ignored here
      return stri_flatten_withressep(VECTOR_ELT(strlist, 0), collapse);
   }

   // get length of the longest character vector on the list, i.e. vectorize_length
   R_len_t vectorize_length = 0;
   for (R_len_t i=0; i<strlist_length; ++i) {
      R_len_t strlist_cur_length = LENGTH(VECTOR_ELT(strlist, i));
      if (strlist_cur_length <= 0) return stri__vector_empty_strings(0);
      if (strlist_cur_length > vectorize_length) vectorize_length = strlist_cur_length;
   }


   STRI__ERROR_HANDLER_BEGIN

   StriContainerListUTF8 strlist_cont(strlist, vectorize_length);

   StriContainerUTF8 sep_cont(sep, 1); // definitely not NA
   const char* sep_s = sep_cont.get(0).c_str();
   R_len_t     sep_n = sep_cont.get(0).length();

   StriContainerUTF8 collapse_cont(collapse, 1); // definitely not NA
   const char* collapse_s = collapse_cont.get(0).c_str();
   R_len_t     collapse_n = collapse_cont.get(0).length();

   // Get required buffer size
   R_len_t buf_maxbytes = 0;
   for (R_len_t i=0; i<vectorize_length; ++i) {   // for each vectorized string (vertically)
      for (R_len_t j=0; j<strlist_length; ++j) {  // for each character vector  (horizontally)
         if (strlist_cont.get(j).isNA(i))
            return stri__vector_NA_strings(1);

         buf_maxbytes += strlist_cont.get(j).get(i).length()+ ((j>0)?sep_n:0);
      }

      if (i>0) buf_maxbytes += collapse_n;
   }

   // 5. Create ret val
   String8buf buf(buf_maxbytes);
   R_len_t last_buf_idx = 0;

   for (R_len_t i=0; i<vectorize_length; ++i) {
      // there is no NA anywhere

      if (collapse_n > 0 && i > 0) {
         memcpy(buf.data()+last_buf_idx, collapse_s, (size_t)collapse_n);
         last_buf_idx += collapse_n;
      }

      for (R_len_t j=0; j<strlist_length; ++j) {

         if (sep_n > 0 && j > 0) {
            memcpy(buf.data()+last_buf_idx, sep_s, (size_t)sep_n);
            last_buf_idx += sep_n;
         }

         const String8* curstring = &(strlist_cont.get(j).get(i));
         R_len_t curstring_n = curstring->length();
         memcpy(buf.data()+last_buf_idx, curstring->c_str(), (size_t)curstring_n);
         last_buf_idx += curstring_n;
      }
   }

#ifndef NDEBUG
   if (buf_maxbytes != last_buf_idx)
      throw StriException("stri_join_withcollapse: buffer overrun");
#endif

   // we are done
   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, 1));
   SET_STRING_ELT(ret, 0, Rf_mkCharLenCE(buf.data(), last_buf_idx, CE_UTF8));
   STRI__UNPROTECT_ALL
   return ret;

   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/**
 * Concatenate Character Vectors, with no collapse
 *
 * @param strlist list of character vectors
 * @param sep single string
 * @return character vector
 *
 *
 * @version 0.1-?? (Marek Gagolewski)
 *
 * @version 0.1-?? (Marek Gagolewski)
 *          use StriContainerUTF8's vectorization
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-16)
 *          make StriException-friendly, useStriContainerListUTF8
 *
 * @version 0.1-12 (Marek Gagolewski, 2013-12-04)
 *          fixed bug #49
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-03-18)
 *          stri_join has been splitted to stri_join_nocollapse
 *          and stri_join_withcollapse (for efficiency reasons)
 */
SEXP stri_join_nocollapse(SEXP strlist, SEXP sep)
{
   strlist = stri_prepare_arg_list_string(strlist, "...");
   R_len_t strlist_length = LENGTH(strlist);
   if (strlist_length <= 0) return stri__vector_empty_strings(0);

   // get length of the longest character vector on the list, i.e. vectorize_length
   R_len_t vectorize_length = 0;
   for (R_len_t i=0; i<strlist_length; ++i) {
      R_len_t strlist_cur_length = LENGTH(VECTOR_ELT(strlist, i));
      if (strlist_cur_length <= 0) return stri__vector_empty_strings(0);
      if (strlist_cur_length > vectorize_length) vectorize_length = strlist_cur_length;
   }

   sep = stri_prepare_arg_string_1(sep, "sep");
   if (STRING_ELT(sep, 0) == NA_STRING) {
         return stri__vector_NA_strings(vectorize_length);
   }


   // * special case *
   if (LENGTH(STRING_ELT(sep, 0)) == 0 && strlist_length == 2) {
      // sep==empty string and 2 vectors --
      // an often occuring case - we have some specialized functions for this :-)
      return stri_join2_nocollapse(VECTOR_ELT(strlist, 0), VECTOR_ELT(strlist, 1));
   }

   // note that if 1 vector is given
   // we cannot return VECTOR_ELT(strlist, 0) directly
   // -- it needs to be converted to UTF8
   // so we proceed

   SEXP ret;
   STRI__ERROR_HANDLER_BEGIN

   StriContainerUTF8 sep_cont(sep, 1);
   const char* sep_char = sep_cont.get(0).c_str();
   R_len_t     sep_len  = sep_cont.get(0).length();

   StriContainerListUTF8 strlist_cont(strlist, vectorize_length);


   // 4. Get buf size and determine where NAs will occur
   R_len_t buf_maxbytes = 0;
   vector<bool> whichNA(vectorize_length, false); // where are NAs in out?
   for (R_len_t i=0; i<vectorize_length; ++i) {

      R_len_t curchar = 0;
      for (R_len_t j=0; j<strlist_length; ++j) {
         if (strlist_cont.get(j).isNA(i)) {
            whichNA[i] = true;
            break;
         }
         else {
            curchar += strlist_cont.get(j).get(i).length()
               + ((j>0)?sep_len:0);
         }
      }
      if (!whichNA[i] && curchar > buf_maxbytes)
         buf_maxbytes = curchar;
   }

   // 5. Create ret val
   String8buf buf(buf_maxbytes);
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length));

   for (R_len_t i=0; i<vectorize_length; ++i) {
      if (whichNA[i]) {
         SET_STRING_ELT(ret, i, NA_STRING);
         continue;
      }

      R_len_t cursize = 0;
      for (R_len_t j=0; j<strlist_length; ++j) {

         if (sep_len >= 0 && j > 0) {
            memcpy(buf.data()+cursize, sep_char, (size_t)sep_len);
            cursize += sep_len;
         }

         const String8* curstring = &(strlist_cont.get(j).get(i));
         R_len_t curstring_n = curstring->length();
         memcpy(buf.data()+cursize, curstring->c_str(), (size_t)curstring_n);
         cursize += curstring_n;
      }

      SET_STRING_ELT(ret, i, Rf_mkCharLenCE(buf.data(), cursize, CE_UTF8));
   }

   // nothing more to do:
   STRI__UNPROTECT_ALL
   return ret;

   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/** String vector flatten, with no separator (i.e. empty) between each string
 *
 *  if any of s is NA, the result will be NA_character_
 *
 *  @param s character vector
 *  @return if s is not empty, then a character vector of length 1
 *
 * @version 0.1-?? (Marek Gagolewski)
 *
 * @version 0.1-?? (Marek Gagolewski)
 *          StriContainerUTF8 - any R Encoding
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-16)
 *          make StriException friendly
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-03-18)
 *          This function hasn't been used at all before (strange, isn't it?);
 *          From now on it's being called by stri_flatten_withressep
 *          (a small performance gain)
 */
SEXP stri_flatten_noressep(SEXP str)
{
   str = stri_prepare_arg_string(str, "str");
   R_len_t str_length = LENGTH(str);
   if (str_length <= 0) return str;

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 str_cont(str, str_length);

   // 1. Get required buffer size
   R_len_t nchar = 0;
   for (int i=0; i<str_length; ++i) {
      if (str_cont.isNA(i)) {
         return stri__vector_NA_strings(1); // at least 1 NA => return NA
      }
      nchar += str_cont.get(i).length();
   }

   // 2. Fill the buf!
   String8buf buf(nchar);
   R_len_t cur = 0;
   for (int i=0; i<str_length; ++i) {
      R_len_t ncur = str_cont.get(i).length();
      memcpy(buf.data()+cur, str_cont.get(i).c_str(), (size_t)ncur);
      cur += ncur;
   }


   // 3. Get ret val & good bye
   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, 1));
   SET_STRING_ELT(ret, 0, Rf_mkCharLenCE(buf.data(), cur, CE_UTF8));
   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/** String vector flatten, with separator between each string
 *
 *  if any of str is NA, the result will be NA_character_
 *
 *  @param str character vector
 *  @param collapse a single string
 *  @return if s is not empty, then a character vector of length 1
 *
 * @version 0.1-?? (Marek Gagolewski)
 *
 * @version 0.1-?? (Bartek Tartanus)
 *          collapse arg added (1 sep supported)
 *
 * @version 0.1-?? (Marek Gagolewski)
 *          StriContainerUTF8 - any R Encoding
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-16)
 *          make StriException friendly
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-03-18)
 *          Call stri_flatten_noressep if needed
 *
 */
SEXP stri_flatten_withressep(SEXP str, SEXP collapse)
{
   collapse = stri_prepare_arg_string_1(collapse, "collapse");

   if (STRING_ELT(collapse, 0) == NA_STRING)
      return stri__vector_NA_strings(1);

   // if collapse is an empty string, we may use the following
   // specialized function:
   if (LENGTH(STRING_ELT(collapse, 0)) == 0)
      return stri_flatten_noressep(str);


   str = stri_prepare_arg_string(str, "str"); // prepare string argument
   R_len_t str_length = LENGTH(str);
   if (str_length <= 0) return stri__vector_empty_strings(0);

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 str_cont(str, str_length);
   StriContainerUTF8 collapse_cont(collapse, 1);
   R_len_t collapse_nbytes = collapse_cont.get(0).length();
   const char* collapse_s = collapse_cont.get(0).c_str();


   // 1. Get required buffer size
   R_len_t nbytes = 0;
   for (int i=0; i<str_length; ++i) {
      if (str_cont.isNA(i)) {
         return stri__vector_NA_strings(1); // at least 1 NA => return NA
      }
      nbytes += str_cont.get(i).length() + ((i>0)?collapse_nbytes:0);
   }


   // 2. Fill the buf!
   String8buf buf(nbytes);
   R_len_t cur = 0;
   for (int i=0; i<str_length; ++i) {
      R_len_t ncur = str_cont.get(i).length();
      memcpy(buf.data()+cur, str_cont.get(i).c_str(), (size_t)ncur);
      cur += ncur;
      if (collapse_nbytes > 0 && i < str_length-1) {
         memcpy(buf.data()+cur, collapse_s, (size_t)collapse_nbytes);
         cur += collapse_nbytes;
      }
   }


   // 3. Get ret val & return
   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, 1));
   SET_STRING_ELT(ret, 0, Rf_mkCharLenCE(buf.data(), cur, CE_UTF8));
   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}
