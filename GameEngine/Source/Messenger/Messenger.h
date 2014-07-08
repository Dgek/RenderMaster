#pragma once

#include "Message.h"
#include "MessageHandler.h"

class IMessenger
{
	//TODO: define interface
};

class Messenger : public IMessenger
{
public:
	//Constructor and Destructor
	Messenger();
	~Messenger();

	/** ============= **/
	//	  Singleton	   //
	/** ============= **/
	static Messenger * const Get();

private:

	//Usefull typedefs
	typedef vector<MessageHandlerPtr>			Handlers;					//list of handlers
	typedef pair<MessageType &, Handlers>		MessageTypeAndHandlersPair; //element in handlers map
	typedef list<MessageTypeAndHandlersPair>	HandlersMap;				//maps message type to handlers

	/*****
	Declare Messenger lists
	*****/
	HandlersMap	m_register;

	/** ============================================ **/
	//		Private methods for internal stuff		  //
	/**	============================================ **/

	//register message type
	bool RegisterMessageTypeInternal(MessageType & type);

	//register handler to react to certain message
	bool RegisterHandlerInternal(MessageHandler const * const pHandler,
		MessageType & type);

	//stop caring about certain message
	bool StopHandlingInternal(MessageHandler const * const pHandler, MessageType & type);

	//delete handler from messenger
	bool RemoveHandlerInternal(MessageHandler const * const pHandler);

	//send message to the handlers
	bool SendMessageInternal(Message const & msg);

	//handle all events
private:

	/** =========================================== **/
	//		Global methods for message-handling		 //
	//												 //
	//	 These are external methods (basically -	 //
	//   wrappers around !Internal! methods) and	 //
	//	 make needed assertions to safely use them   //
	//	 throught the code.							 //
	//												 //	
	/**	=========================================== **/

	//register message type
	friend bool RegisterMessageType(MessageType & type);

	//register handler to react to certain message
	friend bool RegisterHandler(MessageHandler const * const pHandler,
		MessageType & type);

	//stop caring about certain message
	friend bool StopHandling(MessageHandler const * const pHandler, MessageType & type);

	//delete handler from messenger
	friend bool RemoveHandler(MessageHandler const * const pHandler);

	//send message to the handlers
	friend bool SendGameMessage(Message const & msg);
};