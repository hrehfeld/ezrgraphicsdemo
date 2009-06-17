#ifndef _UTILITIES_H_
#define _UTILITIES_H_

namespace Ezr {
	class Utilities
	{
	public:
		Utilities();
		virtual ~Utilities();

		/**
		 * Read the contents of a file into a string
		 */
		static std::string loadFile(std::string filename);
	};
}

#endif /* _UTILITIES_H_ */
