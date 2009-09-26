#ifndef FBOCOLORATTACHMENT_H
#define FBOCOLORATTACHMENT_H

#include <boost/shared_ptr.hpp>

namespace Ezr{

    class Texture;

    class FboColorAttachment
    {
    public:
        FboColorAttachment(Texture* tex, int attachmentId);
        virtual ~FboColorAttachment();

        inline void setAttachmentId(int id){
            _attachmentId = id;}

        inline int getAttachmentId() const{
            return _attachmentId;}

        Texture* getTexture(){return _texture;}

    private:
        FboColorAttachment();

        unsigned int _attachmentId;
        Texture* _texture;
    };

}
#endif
