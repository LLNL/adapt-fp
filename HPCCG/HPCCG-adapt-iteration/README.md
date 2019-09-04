* Required several bug fixes to compile with C++ compiler (missing "extern C"
  closing bracket, missing pointer casts, invalid/deprecated type conversions)
* There are three separate kernels: Matvec, Relax, and Axpy. The the second is
  the longest and the third is much shorter than the other two.
