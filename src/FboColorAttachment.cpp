#include "FboColorAttachment.h"

namespace Ezr
{
    FboColorAttachment::FboColorAttachment(Texture* texture, int attachmentId)
        :_attachmentId(attachmentId),
         _texture(texture)
    {}
     
    FboColorAttachment::~FboColorAttachment()
    {}
}