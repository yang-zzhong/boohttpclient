#ifndef _BOO_HTTP_CLIENT_POSTDATA_H_
#define _BOO_HTTP_CLIENT_POSTDATA_H_

#include "container.hpp"

namespace boohttp
{
    class PostData : public Container
    {
    public:
        PostData();
        ~PostData();
    };
}

#endif