# kate: default-dictionary en_US

## This file is part of the 'stringi' package for R.
## Copyright (c) 2013-2025, Marek Gagolewski <https://www.gagolewski.com/>
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
##
## 1. Redistributions of source code must retain the above copyright notice,
## this list of conditions and the following disclaimer.
##
## 2. Redistributions in binary form must reproduce the above copyright notice,
## this list of conditions and the following disclaimer in the documentation
## and/or other materials provided with the distribution.
##
## 3. Neither the name of the copyright holder nor the names of its
## contributors may be used to endorse or promote products derived from
## this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
## 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
## BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
## HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
## SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
## OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
## WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
## OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
## EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#' @title
#' Escape Unicode Code Points
#'
#' @description
#' Generates an ASCII string where all non-printable characters
#' and non-ASCII characters are converted to escape sequences.
#'
#' @details
#'
#' For non-printable and certain special (well-known,
#' see also the R man page \link{Quotes})
#' ASCII characters, the following
#' (also recognized in R) convention is used.
#' We get \code{\\a}, \code{\\b}, \code{\\t}, \code{\\n}, \code{\\v},
#' \code{\\f}, \code{\\r}, \code{\"}, \code{\'}, \code{\\\\}
#' or either \code{\\uXXXX} (4 hex digits) or \code{\\UXXXXXXXX} (8 hex digits)
#' otherwise.
#'
#'
#' As usual in stringi, any input string is converted to Unicode
#' before executing the escape process.
#'
#'
#' @param str character vector
#'
#' @return
#' Returns a character vector.
#'
#' @examples
#' stri_escape_unicode('a\u0105!')
#'
#' @family escape
#' @export
stri_escape_unicode <- function(str)
{
    .Call(C_stri_escape_unicode, str)
}


#' @title
#' Un-escape All Escape Sequences
#'
#' @description
#' Un-escapes all known escape sequences.
#'
#' @details
#' Uses \pkg{ICU}'s facilities to un-escape Unicode character sequences.
#'
#' The following escape sequences are recognized:
#' \code{\\a}, \code{\\b}, \code{\\t}, \code{\\n}, \code{\\v}, \code{\\?},
#' \code{\\e}, \code{\\f}, \code{\\r}, \code{\"}, \code{\'}, \code{\\\\},
#' \code{\\uXXXX} (4 hex digits),
#' \code{\\UXXXXXXXX} (8 hex digits),
#' \code{\\xXX} (1-2 hex digits),
#' \code{\\ooo} (1-3 octal digits),
#' \code{\\cX} (control-X; X is masked with 0x1F).
#' For \code{\\xXX} and \code{\\ooo}, beware of non-valid UTF-8 byte sequences.
#'
#' Note that some versions of R on Windows cannot handle
#' characters defined with \code{\\UXXXXXXXX}.
#'
#' @param str character vector
#'
#' @return
#' Returns a character vector.
#' If an escape sequence is ill-formed,
#' the result will be \code{NA} and a warning will be given.
#'
#' @examples
#' stri_unescape_unicode('a\\u0105!\\u0032\\n')
#'
#' @family escape
#' @export
stri_unescape_unicode <- function(str)
{
    .Call(C_stri_unescape_unicode, str)
}
