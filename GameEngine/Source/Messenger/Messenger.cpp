#include "Core.h"

#include "Messenger.h"

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	Global pointer to the messenger
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
static Messenger * g_pMessenger = nullptr;

/*** Constructor ***/
Messenger::Messenger()
{
	//assign global pointer to the instance
	g_pMessenger = this;
}

/*** Destructor ***/
Messenger::~Messenger()
{
	//set global pointer to NULL
	if (g_pMessenger)
		g_pMessenger = nullptr;
}

/*** Sengleton ***/
Messenger * const Messenger::Get()
{
	return g_pMessenger;
}

// ===================================
//	Private methods for internal stuff
// ===================================

//register message type
bool Messenger::RegisterMessageTypeInternal(MessageType & type)
{
	//Iterate through the register
	//auto reg = m_register.begin();
	auto reg = m_register.begin();

	for (; reg != m_register.end(); ++reg)
	{
		//such message type already registered
		if (reg->first == type)
		{
			assert(0 && "Such message type has been already registered!");
			return false;
		}
	}

	//Add new message type to the register
	//m_register.push_back(make_pair(type, decltype(reg->second)()));
	m_register.push_back(make_pair(type, reg->second));

	return true;
}

//register handler to react to certain message
bool Messenger::RegisterHandlerInternal(MessageHandler const * const pHandler, MessageType & type)
{
	//Iterate though the register
	for (auto reg = m_register.begin(); reg != m_register.end(); reg++)
	{
		//we have found message type to subscribe handler to
		if (reg->first == type)
		{
			//now, iterate through the handlers and make we don't duplicate one
			auto handlers = &reg->second;
			for (auto handler = handlers->begin(); handler != handlers->end(); handler++)
			{
				//this handler has already been registered
				if ((&**handler) == pHandler) //unpack iterator and retrieve pointer from shared_ptr
				{
					assert(0 && "Handler for the message type has already been registered!");
					return false;
				}
			}

			//add new handler to the list for this message type
			handlers->push_back(make_shared<MessageHandler>(*pHandler));

			return true; //handler has been added
		}
	}

	//Such message type is not registered
	return false;
}

//stop caring about certain message
bool Messenger::StopHandlingInternal(MessageHandler const * const pHandler, MessageType & type)
{
	//Iterate though the register
	auto reg = m_register.begin();
	for (; reg != m_register.end(); ++reg)
	{
		//we have found message type to unsubscribe handler from
		if (reg->first == type)
		{
			//now, iterate through the handlers
			auto handlers = &reg->second;
			auto handler = handlers->begin();
			for (; handler != handlers->end(); ++handler)
			{
				//the registered handler was found
				if ((&**handler) == pHandler) //unpack iterator and retrieve pointer from shared_ptr
					break;
			}

			if (handler != handlers->end())
			{
				//remove handler from the list of this message type
				handlers->erase(handler);
				return true;
			}

			//the handler wasn't handling this message type
			assert(0 && "The Handler wasn't handling this message type!");
			return false;
		}
	}

	assert(0 && "Such message type is not registered!");
	return true;
}

//delete handler from messenger
bool Messenger::RemoveHandlerInternal(MessageHandler const * const pHandler)
{
	//flag to track whether the handler was registered
	bool registered = false;

	//Iterate though the register
	for (auto reg = m_register.begin(); reg != m_register.end(); ++reg)
	{
		//now, iterate through the handlers
		auto handlers = &reg->second;
		auto handler = handlers->begin();
		for (; handler != handlers->end(); ++handler)
		{
			//the registered handler was found
			if ((&**handler) == pHandler) //unpack iterator and retrieve pointer from shared_ptr
				break;
		}

		//the handler was subscribed for this message, so delete it
		if (handler != handlers->end())
		{
			handlers->erase(handler);
			registered = true;
		}
	}

	//return false if listener wasn't deleted from anywhere
	return registered;
}

/*
bool Messenger::RemoveHandlerInternal(ACHAR const * const handlerName)
{
return true;
} */

//send message to the handlers
bool Messenger::SendMessageInternal(Message const & msg)
{
	//Iterate through register and find message we are interested in
	for (auto reg = m_register.begin(); reg != m_register.end(); ++reg)
	{
		//message type is found
		if (msg.VGetMessageType() == reg->first)
		{
			//true if message was handled by at least one of the listeners
			bool handled = false;

			//now, iterate through the handlers
			auto handlers = &reg->second;
			for (auto handler = handlers->begin(); handler != handlers->end(); ++handler)
			{
				if ((*handler)->VOnEvent(msg))
					handled = true;
			}

			//return false is no listener handled the event
			return handled;
		}
	}

	assert(0 && "Such event type is not registered!");
	return false;
}

//=================================================
//	Global external methods for using the messenger
//=================================================

//register message type
bool RegisterMessageType(MessageType & type)
{
	//assert that we have valid pointer to the messenger
	assert(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->RegisterMessageTypeInternal(type);
}

//register handler to react to certain message
bool RegisterHandler(MessageHandler const * const pHandler, MessageType & type)
{
	//assert that we have valid pointer to the messenger
	assert(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->RegisterHandlerInternal(pHandler, type);
}

//stop caring about certain message
bool StopHandling(MessageHandler const * const pHandler, MessageType & type)
{
	//assert that we have valid pointer to the messenger
	assert(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->StopHandlingInternal(pHandler, type);
}

//delete handler from messenger
bool RemoveHandler(MessageHandler const * const pHandler)
{
	//assert that we have valid pointer to the messenger
	assert(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->RemoveHandlerInternal(pHandler);
}

//send message to the handlers
bool SendGameMessage(Message const & msg)
{
	//assert that we have valid pointer to the messenger
	assert(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->SendMessageInternal(msg);
}
