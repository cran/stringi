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
#' Convert Strings Between Given Encodings
#'
#' @description
#' These functions convert strings between encodings.
#' They aim to serve as a more portable and faster replacement
#' for \R's own \code{\link{iconv}}.
#'
#' @details
#' \code{stri_conv} is an alias for \code{stri_encode}.
#'
#' Refer to \code{\link{stri_enc_list}} for the list
#' of supported encodings and \link{stringi-encoding}
#' for a general discussion.
#'
#' If \code{from} is either missing, \code{''}, or \code{NULL},
#' and if \code{str} is a character vector
#' then the marked encodings are used
#' (see \code{\link{stri_enc_mark}}) -- in such a case \code{bytes}-declared
#' strings are disallowed.
#' Otherwise, i.e., if \code{str} is a \code{raw}-type vector
#' or a list of raw vectors,
#' we assume that the input encoding is the current default encoding
#' as given by \code{\link{stri_enc_get}}.
#'
#' However, if \code{from} is given explicitly,
#' the internal encoding declarations are always ignored.
#'
#' For \code{to_raw=FALSE}, the output
#' strings always have the encodings marked according to the target converter
#' used (as specified by \code{to}) and the current default Encoding
#' (\code{ASCII}, \code{latin1}, \code{UTF-8}, \code{native},
#' or \code{bytes} in all other cases).
#'
#'
#' Note that some issues might occur if \code{to} indicates, e.g.,
#' UTF-16 or UTF-32, as the output strings may have embedded NULs.
#' In such cases, please use \code{to_raw=TRUE} and consider
#' specifying a byte order marker (BOM) for portability reasons
#' (e.g., set \code{UTF-16} or \code{UTF-32} which automatically
#' adds the BOMs).
#'
#' Note that \code{stri_encode(as.raw(data), 'encodingname')}
#' is a clever substitute for \code{\link{rawToChar}}.
#'
#' In the current version of \pkg{stringi}, if an incorrect code point is found
#' on input, it is replaced with the default (for that target encoding)
#' 'missing/erroneous' character (with a warning), e.g.,
#' the SUBSTITUTE character (U+001A) or the REPLACEMENT one (U+FFFD).
#' Occurrences thereof can be located in the output string to diagnose
#' the problematic sequences, e.g., by calling:
#' \code{stri_locate_all_regex(converted_string, '[\\ufffd\\u001a]'}.
#'
#' Because of the way this function is currently implemented,
#' maximal size of a single string to be converted cannot exceed ~0.67 GB.
#'
#'
#' @param str a character vector, a raw vector, or
#' a list of \code{raw} vectors to be converted
#' @param from input encoding:
#'       \code{NULL} or \code{''} for the default encoding
#'       or internal encoding marks' usage (see Details);
#'       otherwise, a single string with encoding name,
#'       see \code{\link{stri_enc_list}}
#' @param to target encoding:
#'       \code{NULL} or \code{''} for default encoding
#'       (see \code{\link{stri_enc_get}}),
#'       or a single string with encoding name
#' @param to_raw a single logical value; indicates whether a list of raw vectors
#' rather than a character vector should be returned
#'
#' @return If \code{to_raw} is \code{FALSE},
#' then a character vector with encoded strings (and appropriate
#' encoding marks) is returned.
#' Otherwise, a list of vectors of type raw is produced.
#'
#' @references
#' \emph{Conversion} -- ICU User Guide,
#' \url{https://unicode-org.github.io/icu/userguide/conversion/}
#'
#' @family encoding_conversion
#' @rdname stri_encode
#' @export
stri_encode <- function(str, from = NULL, to = NULL, to_raw = FALSE) {
    .Call(C_stri_encode, str, from, to, to_raw)
}


#' @rdname stri_encode
#' @export
stri_conv <- stri_encode


#' @title
#' Convert Strings To UTF-32
#'
#' @description
#' UTF-32 is a 32-bit encoding where each Unicode code point
#' corresponds to exactly one integer value.
#' This function converts a character vector to a list
#' of integer vectors so that, e.g.,
#' individual code points may be easily accessed, changed, etc.
#'
#' @details
#' See \code{\link{stri_enc_fromutf32}} for a dual operation.
#'
#' This function is roughly equivalent to a vectorized call
#' to \code{\link{utf8ToInt}(enc2utf8(str))}.
#' If you want a list of raw vectors on output,
#' use \code{\link{stri_encode}}.
#'
#' Unlike \code{utf8ToInt}, if ill-formed UTF-8 byte sequences are detected,
#' a corresponding element is set to NULL and a warning is generated.
#' To deal with such issues, use, e.g., \code{\link{stri_enc_toutf8}}.
#'
#' @param str a character vector (or an object coercible to)
#'        to be converted
#' @return Returns a list of integer vectors.
#' Missing values are converted to \code{NULL}s.
#'
#' @family encoding_conversion
#' @export
stri_enc_toutf32 <- function(str)
{
    .Call(C_stri_enc_toutf32, str)
}


#' @title
#' Convert From UTF-32
#'
#' @description
#' This function converts integer vectors,
#' representing sequences of UTF-32 code points, to UTF-8 strings.
#'
#' @details
#' UTF-32 is a 32-bit encoding where each Unicode code point
#' corresponds to exactly one integer value.
#'
#' This function is a vectorized version of
#' \code{\link{intToUtf8}}. As usual in \pkg{stringi},
#' it returns character strings in UTF-8.
#' See \code{\link{stri_enc_toutf32}} for a dual operation.
#'
#' If an ill-defined code point is given, a warning is generated
#' and the corresponding string is set to \code{NA}.
#' Note that \code{0}s are not allowed in \code{vec}, as they are used
#' internally to mark the end of a string (in the C API).
#'
#'
#' See also \code{\link{stri_encode}} for decoding arbitrary byte sequences
#' from any given encoding.
#'
#'
#' @param vec a list of integer vectors (or objects coercible to such vectors)
#'    or \code{NULL}s. For convenience, a single integer vector can also
#'    be given.
#' @return Returns a character vector (in UTF-8).
#' \code{NULL}s in the input list are converted to \code{NA_character_}.
#'
#' @family encoding_conversion
#' @export
stri_enc_fromutf32 <- function(vec)
{
    .Call(C_stri_enc_fromutf32, vec)
}


#' @title
#' Convert Strings To UTF-8
#'
#' @description
#' Converts character strings with declared marked encodings
#' to UTF-8 strings.
#'
#' @details
#' If \code{is_unknown_8bit} is set to \code{FALSE} (the default),
#' then R encoding marks are used, see \code{\link{stri_enc_mark}}.
#' Bytes-marked strings will cause the function to fail.
#'
#' If a string is in UTF-8 and has a byte order mark (BOM),
#' then the BOM will be silently removed from the output string.
#'
#' If the default encoding is UTF-8, see \code{\link{stri_enc_get}},
#' then strings marked with \code{native} are -- for efficiency reasons --
#' returned as-is, i.e., with unchanged markings.
#' A similar behavior is observed when calling \code{\link{enc2utf8}}.
#'
#' For \code{is_unknown_8bit=TRUE}, if a string is declared to be neither
#' in ASCII nor in UTF-8, then all byte codes > 127 are replaced with
#' the Unicode REPLACEMENT CHARACTER (\\Ufffd).
#' Note that the REPLACEMENT CHARACTER may be interpreted as Unicode
#' missing value for single characters.
#' Here a \code{bytes}-marked string is assumed to use an 8-bit encoding
#' that extends the ASCII map.
#'
#' What is more, setting \code{validate} to \code{TRUE}
#' or \code{NA} in both cases validates the resulting UTF-8 byte stream.
#' If \code{validate=TRUE}, then
#' in case of any incorrect byte sequences, they will be
#' replaced with the REPLACEMENT CHARACTER.
#' This option may be used in a case
#' where you want to fix an invalid UTF-8 byte sequence.
#' For \code{NA}, a bogus string will be replaced with a missing value.
#'
#' @param str a character vector to be converted
#' @param is_unknown_8bit a single logical value, see Details
#' @param validate a single logical value (can be \code{NA}), see Details
#' @return Returns a character vector.
#'
#' @family encoding_conversion
#' @export
stri_enc_toutf8 <- function(str, is_unknown_8bit = FALSE, validate = FALSE)
{
    .Call(C_stri_enc_toutf8, str, is_unknown_8bit, validate)
}


#' @title
#' Convert Strings To Native Encoding
#'
#' @description
#' Converts character strings with declared encodings
#' to the current native encoding.
#'
#' @details
#' This function just calls \code{\link{stri_encode}(str, NULL, NULL)}.
#' The current native encoding can be read with \code{\link{stri_enc_get}}.
#' Character strings declared to be in \code{bytes} encoding will fail here.
#'
#' Note that if working in a UTF-8 environment,
#' resulting strings will be marked with \code{UTF-8}
#' and not \code{native}, see \code{\link{stri_enc_mark}}.
#'
#' @param str a character vector to be converted
#' @return Returns a character vector.
#'
#' @family encoding_conversion
#' @export
stri_enc_tonative <- function(str)
{
    stri_encode(str, NULL, NULL)
}


#' @title
#' Convert To ASCII
#'
#' @description
#' This function converts input strings to ASCII,
#' i.e., to character strings consisting of bytes not greater than 127.
#'
#' @details
#' All code points greater than 127 are replaced with the ASCII SUBSTITUTE
#' CHARACTER (0x1A).
#' \R encoding declarations are always used to determine
#' which encoding is assumed for each input, see \code{\link{stri_enc_mark}}.
#' If ill-formed byte sequences are found in UTF-8 byte
#' streams, a warning is generated.
#'
#' A \code{bytes}-marked string is assumed to be in an 8-bit encoding
#' extending the ASCII map (a common assumption in \R itself).
#'
#' Note that the SUBSTITUTE CHARACTER (\code{\\x1a == \\032}) may be interpreted
#' as the ASCII missing value for single characters.
#'
#' @param str a character vector to be converted
#' @return Returns a character vector.
#'
#' @family encoding_conversion
#' @export
stri_enc_toascii <- function(str)
{
    .Call(C_stri_enc_toascii, str)
}
