#include "body.hpp"

namespace boohttp {

	Body::Body(){}

	Body::Body(string body)
	{
		this->body = body;
	}

	void Body::append(string body)
	{
		this->body += body;
	}

	const string Body::str() const
	{
		return body;
	}

	Body::~Body() { }
}
