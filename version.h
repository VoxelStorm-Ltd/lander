#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 10;
	static const long BUILD  = 1010;
	static const long REVISION  = 5529;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1761;
	#define RC_FILEVERSION 1,10,1010,5529
	#define RC_FILEVERSION_STRING "1, 10, 1010, 5529\0"
	static const char FULLVERSION_STRING [] = "1.10.1010.5529";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 10;
	

}
#endif //VERSION_H
