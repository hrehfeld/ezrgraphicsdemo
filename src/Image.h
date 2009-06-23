#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>
#include "IL/il.h"
#include "IL/ilu.h"

using namespace std;

namespace Ezr
{
	class Image
	{
	public:
	    Image(string filename);
	    virtual ~Image();

		int getWidth() const { return getInfo().Width; }
		int getHeight() const { return getInfo().Height; }
		int getFormat() const { return getInfo().Format; } 
		int getType() const { return getInfo().Type; }
		ILubyte* getData() const { return getInfo().Data; }
		
	private:
		ILuint _image;

		ILinfo getInfo() const;


		/**
		 * Make sure devil is initialised
		 */
		static void init();
		static bool ilInitialised;
	};
}

#endif /* _IMAGE_H_ */
