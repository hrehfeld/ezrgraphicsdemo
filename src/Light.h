#ifndef _LIGHT_H_
#define _LIGHT_H_

namespace Ezr
{
	class DeferredRenderer;

	class Light
	{
	public:        
	    virtual ~Light() = 0;
		virtual void renderLight(DeferredRenderer* r) = 0;
		
	};
}

#endif /* _LIGHT_H_ */
