/*

Copyright (C) 2008 Jaroslav Hajek <highegg@gmail.com>

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.

*/

#if !defined (octave_base_diag_h)
#define octave_base_diag_h 1

#include <cstdlib>

#include <iostream>
#include <string>

#include "mx-base.h"
#include "str-vec.h"

#include "oct-obj.h"
#include "ov-base.h"
#include "ov-typeinfo.h"

class tree_walker;

// Real matrix values.

template <class DMT, class MT>
class
octave_base_diag : public octave_base_value
{

public:

  octave_base_diag (void)
    : octave_base_value () { }

  octave_base_diag (const DMT& m)
    : octave_base_value (), matrix (m)
  { }

  octave_base_diag (const octave_base_diag& m)
    : octave_base_value (), matrix (m.matrix) { }

  ~octave_base_diag (void) { }

  size_t byte_size (void) const { return matrix.byte_size (); }

  octave_value squeeze (void) const { return matrix; }

  octave_value subsref (const std::string& type,
			const std::list<octave_value_list>& idx);

  octave_value_list subsref (const std::string& type,
			     const std::list<octave_value_list>& idx, int)
    { return subsref (type, idx); }

  octave_value do_index_op (const octave_value_list& idx,
			    bool resize_ok = false);

  dim_vector dims (void) const { return matrix.dims (); }

  octave_idx_type nnz (void) const { return to_dense ().nnz (); }

  octave_value reshape (const dim_vector& new_dims) const
    { return to_dense ().reshape (new_dims); }

  octave_value permute (const Array<int>& vec, bool inv = false) const
    { return to_dense ().permute (vec, inv); }

  octave_value resize (const dim_vector& dv, bool fill = false) const;

  octave_value all (int dim = 0) const { return MT (matrix).all (dim); }
  octave_value any (int dim = 0) const { return MT (matrix).any (dim); }

  MatrixType matrix_type (void) const { return MatrixType::Diagonal; }
  MatrixType matrix_type (const MatrixType&) const
    { return matrix_type (); }

  octave_value diag (octave_idx_type k = 0) const
    { return octave_value (matrix.diag (k)); }

  octave_value sort (octave_idx_type dim = 0, sortmode mode = ASCENDING) const
    { return to_dense ().sort (dim, mode); }
  octave_value sort (Array<octave_idx_type> &sidx, octave_idx_type dim = 0,
		     sortmode mode = ASCENDING) const
    { return to_dense ().sort (sidx, dim, mode); }

  bool is_matrix_type (void) const { return true; }

  bool is_numeric_type (void) const { return true; }

  bool is_defined (void) const { return true; }

  bool is_constant (void) const { return true; }

  bool is_true (void) const;

  bool is_diag_matrix (void) const { return true; }

  bool valid_as_scalar_index (void) const;

  double double_value (bool = false) const;

  float float_value (bool = false) const;

  double scalar_value (bool frc_str_conv = false) const
    { return double_value (frc_str_conv); }

  idx_vector index_vector (void) const;

  virtual DiagMatrix diag_matrix_value (bool = false) const = 0;

  virtual FloatDiagMatrix float_diag_matrix_value (bool = false) const = 0;

  virtual ComplexDiagMatrix complex_diag_matrix_value (bool = false) const = 0;

  virtual FloatComplexDiagMatrix float_complex_diag_matrix_value (bool = false) const = 0;

  Matrix matrix_value (bool = false) const;

  FloatMatrix float_matrix_value (bool = false) const;

  Complex complex_value (bool = false) const;

  FloatComplex float_complex_value (bool = false) const;

  ComplexMatrix complex_matrix_value (bool = false) const;

  FloatComplexMatrix float_complex_matrix_value (bool = false) const;

  ComplexNDArray complex_array_value (bool = false) const;
   
  FloatComplexNDArray float_complex_array_value (bool = false) const;
   
  boolNDArray bool_array_value (bool warn = false) const;

  charNDArray char_array_value (bool = false) const;
  
  NDArray array_value (bool = false) const; 

  FloatNDArray float_array_value (bool = false) const;

  SparseMatrix sparse_matrix_value (bool = false) const;

  SparseComplexMatrix sparse_complex_matrix_value (bool = false) const;

  int8NDArray
  int8_array_value (void) const { return to_dense ().int8_array_value (); }

  int16NDArray
  int16_array_value (void) const { return to_dense ().int16_array_value (); }

  int32NDArray
  int32_array_value (void) const { return to_dense ().int32_array_value (); }

  int64NDArray
  int64_array_value (void) const { return to_dense ().int64_array_value (); }

  uint8NDArray
  uint8_array_value (void) const { return to_dense ().uint8_array_value (); }

  uint16NDArray
  uint16_array_value (void) const { return to_dense ().uint16_array_value (); }

  uint32NDArray
  uint32_array_value (void) const { return to_dense ().uint32_array_value (); }

  uint64NDArray
  uint64_array_value (void) const { return to_dense ().uint64_array_value (); }

  octave_value convert_to_str_internal (bool pad, bool force, char type) const;

  void print_raw (std::ostream& os, bool pr_as_read_syntax = false) const;

  bool save_ascii (std::ostream& os);

  bool load_ascii (std::istream& is);

  int write (octave_stream& os, int block_size,
	     oct_data_conv::data_type output_type, int skip,
	     oct_mach_info::float_format flt_fmt) const;

  mxArray *as_mxArray (void) const;

  bool print_as_scalar (void) const;

  void print (std::ostream& os, bool pr_as_read_syntax = false) const;

  void print_info (std::ostream& os, const std::string& prefix) const;

  // We forward everything except abs, real, imag, conj.
  octave_value erf (void) const;
  octave_value erfc (void) const;
  octave_value gamma (void) const;
  octave_value lgamma (void) const;
  octave_value acos (void) const;
  octave_value acosh (void) const;
  octave_value angle (void) const;
  octave_value arg (void) const;
  octave_value asin (void) const;
  octave_value asinh (void) const;
  octave_value atan (void) const;
  octave_value atanh (void) const;
  octave_value ceil (void) const;
  octave_value cos (void) const;
  octave_value cosh (void) const;
  octave_value exp (void) const;
  octave_value expm1 (void) const;
  octave_value fix (void) const;
  octave_value floor (void) const;
  octave_value log (void) const;
  octave_value log2 (void) const;
  octave_value log10 (void) const;
  octave_value log1p (void) const;
  octave_value round (void) const;
  octave_value roundb (void) const;
  octave_value signum (void) const;
  octave_value sin (void) const;
  octave_value sinh (void) const;
  octave_value sqrt (void) const;
  octave_value tan (void) const;
  octave_value tanh (void) const;
  octave_value finite (void) const;
  octave_value isinf (void) const;
  octave_value isna (void) const;
  octave_value isnan (void) const;

protected:

  DMT matrix;

  octave_value to_dense () const;

private:

  mutable octave_value dense_cache;

};

#endif
