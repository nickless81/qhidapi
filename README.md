		QHidApi Library
		===============
		
About
=====

QHidApi is a wrapper library for the HidApi library from Signal11. This doesn't
add much functionality to the original library, it primarily only allows the use 
of QString and QVariant for data transfers.

The only major difference is that all USB access is via quint32 id numbers 
rather than via hid_device structures. All of the functionality of the underlying
hidapi classes are available via QHidApi methods.


