#include "request.hpp"

namespace boohttp
{
    Request::Request(string u): Message()
    {
		this->url = Url(u);
		this->method = GET;
    }

    Request::Request(string u, Method method): Message()
    {
        this->url = Url(u);
        this->method = method;
    }

	void Request::withAuth(string user, string passwd, AuthType type)
	{
		auth = Auth{user, passwd, type};
	}

	void Request::withAuth(Auth auth)
	{
		this->auth = auth;
	}

    Request::Request(Url url): Message()
    {
        this->url = url;
        this->method = GET;
    }

    Request::Request(Url url, Method method): Message()
    {
        this->url = url;
        this->method = method;
    }

	void Request::withPostData(const PostData * postData)
	{
		this->postData.merge(postData);
	}
}