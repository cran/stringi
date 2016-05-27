/* This file is part of the 'stringi' package for R.
 * Copyright (C) 2013-2016, Marek Gagolewski and Bartek Tartanus
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
#include "stri_container_listint.h"


/**
 * Default constructor
 *
 */
StriContainerListInt::StriContainerListInt()
   : StriContainerBase()
{
   data = NULL;
}


/**
 * Construct Container from R cobject
 * @param rstr R object
 *
 * if you want nrecycle > n, call set_nrecycle
 */
StriContainerListInt::StriContainerListInt(SEXP rstr)
{
   this->data = NULL;

   if (isNull(rstr)) {
      this->init_Base(1, 1, true);
      this->data = new IntVec[this->n]; // 1 vector, NA/NULL
      if (!this->data) throw StriException(MSG__MEM_ALLOC_ERROR);
   }
   else if (Rf_isInteger(rstr)) {
      this->init_Base(1, 1, true);
      this->data = new IntVec[this->n];
      if (!this->data) throw StriException(MSG__MEM_ALLOC_ERROR);
      this->data[0].initialize((const int*)INTEGER(rstr), LENGTH(rstr)); // shallow copy
   }
   else // if (Rf_isVectorList(rstr)) -- args already checked
   {
      R_len_t nv = LENGTH(rstr);
      this->init_Base(nv, nv, true);
      this->data = new IntVec[this->n];
      if (!this->data) throw StriException(MSG__MEM_ALLOC_ERROR);
      for (R_len_t i=0; i<this->n; ++i) {
         SEXP cur = VECTOR_ELT(rstr, i);
         if (!isNull(cur))
            this->data[i].initialize((const int*)INTEGER(cur), LENGTH(cur)); // shallow copy
         // else leave as-is, i.e. NULL/NA
      }
   }
}


StriContainerListInt::StriContainerListInt(StriContainerListInt& container)
   :    StriContainerBase((StriContainerBase&)container)
{
   if (container.data) {
      this->data = new IntVec[this->n];
      if (!this->data) throw StriException(MSG__MEM_ALLOC_ERROR);
      for (int i=0; i<this->n; ++i) {
         this->data[i] = container.data[i];
      }
   }
   else {
      this->data = NULL;
   }
}


StriContainerListInt& StriContainerListInt::operator=(StriContainerListInt& container)
{
   this->~StriContainerListInt();
   (StriContainerBase&) (*this) = (StriContainerBase&)container;

   if (container.data) {
      this->data = new IntVec[this->n];
      if (!this->data) throw StriException(MSG__MEM_ALLOC_ERROR);
      for (int i=0; i<this->n; ++i) {
         this->data[i] = container.data[i];
      }
   }
   else {
      this->data = NULL;
   }
   return *this;
}


StriContainerListInt::~StriContainerListInt()
{
   if (data) {
      delete [] data;
      data = NULL;
   }
}
