/***********************************************************************************************
 *
 * Copyright � DreamWorks Interactive. 1996
 *
 * Contents:
 *		Implementation of "QueueMessage.hpp."
 *
 ***********************************************************************************************
 *
 * $Log:: /JP2_PC/Source/Lib/EntityDBase/QueueMessage.cpp                                      $
 * 
 * 10    9/30/98 9:36p Rwyatt
 * Implementation of Reset()
 * 
 * 9     8/25/98 2:47p Rvande
 * Fix required when using MSL (instead of the HP STL)
 * 
 * 8     97/09/11 20:54 Speter
 * .Play() and CMessageStep() now optionally take a step time.  
 * 
 * 7     5/30/97 11:14a Agrant
 * LINT tidying
 * 
 * 6     4/09/97 10:44a Rwyatt
 * Added default parameter to CQueueMessage::Play
 * so that step messages are not generated during a 
 * replay but the queue is processed as normal
 * 
 * 5     2/03/97 10:35p Agrant
 * Query.hpp and MessageTypes.hpp have been split into
 * myriad parts so that they may have friends.
 * Or rather, so compile times go down.
 * Look for your favorite query in Lib/EntityDBase/Query/
 * Look for messages in                Lib/EntityDBase/MessageTypes/
 * 
 * 4     96/11/11 17:02 Speter
 * Added specific sending of a CMessageStep in Play().
 * 
 * 3     96/10/24 4:31p Mlange
 * Moved the body of the << function to the header file.
 * 
 * 2     96/10/23 6:41p Mlange
 * Added some comments. Fixed paths in #include statements. Play() now no longer calls Flip().
 * Fixed minor bug in the PushNext() function where messages weren't actually wrapped before
 * they were put in the queue. The message wrapper now allocates the message on the frame heap.
 * 
 * 1     10/21/96 4:12p Pkeet
 * Initial implementation.
 * 
 **********************************************************************************************/

//
// Includes.
//
#include "GblInc/Common.hpp"
#include "QueueMessage.hpp"

#include "Lib/EntityDBase/FrameHeap.hpp"
#include "Lib/EntityDBase/Message.hpp"
#include "Lib/EntityDBase/MessageTypes/MsgStep.hpp"



//*********************************************************************************************
//
// CQueueMessage implementation.
//

	//*****************************************************************************************
	CQueueMessage::CQueueMessage()
	{
		// Create current and next message queues.
	#ifdef __MWERKS__
		pdqwmCurrentMessages = new queue< const CMessage*>();
		pdqwmNextMessages    = new queue< const CMessage*>();
	#else
		pdqwmCurrentMessages = new std::queue< std::deque<const CMessage*> >();   //lint !e1732 !e1733
		pdqwmNextMessages    = new std::queue< std::deque<const CMessage*> >();	//lint !e1732 !e1733
	#endif

		Assert(pdqwmCurrentMessages);
		Assert(pdqwmNextMessages);
	}
		
	//*****************************************************************************************
	CQueueMessage::~CQueueMessage()
	{
		delete pdqwmCurrentMessages;
		delete pdqwmNextMessages;
	}


	//*****************************************************************************************
	void CQueueMessage::Reset()
	{
		//
		// Empty the current queue by simply deleting them and recreating them
		//
		delete pdqwmCurrentMessages;
		delete pdqwmNextMessages;

	#ifdef __MWERKS__
		pdqwmCurrentMessages = new queue< const CMessage*>();
		pdqwmNextMessages    = new queue< const CMessage*>();
	#else
		pdqwmCurrentMessages = new std::queue< std::deque<const CMessage*> >();   //lint !e1732 !e1733
		pdqwmNextMessages    = new std::queue< std::deque<const CMessage*> >();	//lint !e1732 !e1733
	#endif

		frhFrameHeap.Reset();
	}

		
	//*****************************************************************************************
	void CQueueMessage::Flip()
	{
		Assert(pdqwmCurrentMessages);
		Assert(pdqwmNextMessages);

		//
		// Flip the message queues and the temporary frame heaps.
		//
		Swap(pdqwmCurrentMessages, pdqwmNextMessages);
	
		frhFrameHeap.Flip();
	}
		
	//*****************************************************************************************
	void CQueueMessage::Play(bool b_step, TSec s_step) const
	{
		Assert(pdqwmCurrentMessages);

		//
		// Dispatch and pop messages from the queue until the queue is empty.
		//
		// Note that even though the message has been allocated from the frame heap, we do not worry
		// about freeing that memory because the frame heaps are reset every flip.
		//
		while (!pdqwmCurrentMessages->empty())
		{
			(pdqwmCurrentMessages->front())->Send();
			pdqwmCurrentMessages->pop();
		}
		
		//
		// Only send the step message if b_step is true, this is the general case of action. 
		// The only time that things change are when a replay is being processed, in this case
		// the queue needs to be processed as normal but the step messages are generated by the
		// replay not by this function...
		//
		if (b_step)
		{
			CMessageStep msgstep(s_step);
			msgstep.Send();
		}
	}
		
//
// Global variables.
//
CQueueMessage qmQueueMessage;
