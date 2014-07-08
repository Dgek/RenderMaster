#pragma once

#include "../Help/HashedString.h"

//define message type
typedef HashedString MessageType;

/** ======================================
Define Basic Message Structure
====================================== **/
struct Message
{
	//Get Message Type
	virtual const MessageType & VGetMessageType() const = 0;
};

struct EmptyMessage : public Message
{
	//Define constructor which is empty...
	__forceinline EmptyMessage() {}
};