EMDL
=======

EMDL is a C++14 library based on [Odil](https://github.com/lamyj/odil) for the manipulation of Dicom objects.

It aims to offer a few improvements over Odil:
- Lazy parse of the Dicom data set
- Reduced memory usage
- Fewer objects copies using views into shared buffers
- Fewer strings manipulation in favor of more enumerations
- Faster access to data sets elements, and in fewer function calls (with respect to the user)
- Greater use of templates, use of `boost::variant` and `boost::optional`
