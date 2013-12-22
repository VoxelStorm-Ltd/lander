#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 3;
	static const long MINOR  = 4;
	static const long BUILD  = 2689;
	static const long REVISION  = 14782;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4815;
	#define RC_FILEVERSION 3,4,2689,14782
	#define RC_FILEVERSION_STRING "3, 4, 2689, 14782\0"
	static const char FULLVERSION_STRING [] = "3.4.2689.14782";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 89;
	

}
#endif //VERSION_H
