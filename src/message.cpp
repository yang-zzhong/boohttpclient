#include "message.hpp"

namespace boohttp 
{
	Message::Message()
	{
	}

	void Message::withHeaderPair(string key, string value)
	{
		_header.set(key, value);
	}

	void Message::withHeader(const Header & header)
	{
		_header.merge(&header);
	}

	void Message::withHeader(const Header * header)
	{
		_header.merge(header);
	}

	void Message::withBody(const Body & body)
	{
		_body = body;
	}

	void Message::withBody(string body)
	{
		withBody(Body(body));
	}

	void Message::appendBody(string body)
	{
		_body.append(body);
	}

	const Header * Message::header() const
	{
		return &_header;
	}

	const Body * Message::body() const
	{
		return &_body;
	}

	Message::~Message()
	{
	}
}
