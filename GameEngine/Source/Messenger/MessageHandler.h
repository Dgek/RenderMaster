#pragma once

struct Message;
class MessageHandler
{
public:

	//Constructor and Destructor
	__forceinline MessageHandler() {}

	// =======================
	// Get the handler's name
	// =======================
	virtual char const * VGetName() const { return "Default Handler"; }

	/** ==============================
	Method to handle event
	============================== **/
	virtual bool VOnEvent(const Message & msg) { return true; }
};

//usefull typedefs
typedef shared_ptr<MessageHandler> MessageHandlerPtr;