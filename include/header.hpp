#ifndef _BOO_HTTP_CLIENT_HEADER_H_
#define _BOO_HTTP_CLIENT_HEADER_H_

#include "container.hpp"

namespace boohttp
{
    class Header : public Container 
    {
    public:
        Header();
        ~Header();
    };
}

#endif