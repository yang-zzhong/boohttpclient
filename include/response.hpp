#ifndef _BOO_HTTP_CLIENT_RESPONSE_H_
#define _BOO_HTTP_CLIENT_RESPONSE_H_

#include "message.hpp"
#include <sstream>

namespace boohttp
{

    class Response : public Message
    {
	public:
		Response() {};
		~Response() {};
	public:
		int statusCode = 404;
		string statusMessage = "Not Found";
    };
}

#endif