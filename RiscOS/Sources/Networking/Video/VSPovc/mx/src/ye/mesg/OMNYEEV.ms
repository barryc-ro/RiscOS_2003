/ Copyright (c) 1996 by Oracle Corporation.  All Rights Reserved.
/
/ OMNYEEV.msg - OMN YEEV messages
/
// Range of messages: 100-299

//----------------------------------------------------------------
// yeev.c:

150, "yeevTerm: Waiting for %d events to drain to logger(s)"
// *Cause: buffered events have not yet been sent to the loger.
// *Action: none.

160, "yeevSinkDestroy: not currently attached"
// *Cause: no one has done a yeevSinkDestroy first. 
// *Action: report to Oracle.

170, "yeevCompileQual: error compiling '%s'"
// *Cause: the qualification string has an invalid syntax.
// *Action: use a legal syntax.

180, "yeevGetLog: have dest!?"
// *Cause: have already located a logger
// *Action: none.

181, "yeevGetLog: dest search; logimpl %s, logres %s"
// *Cause: looking for a logger, trying this first.
// *Action: none.

182, "yeevGetLog: search 1 - match logimpl/logres"
// *Cause: doing first log search.
// *Action: none.

183, "yeevGetLog: search 2 - match logres"
// *Cause: doing second, wider log search
// *Action: none.

184, "yeevGetLog: search 3 - match anything"
// *Cause: doing third log search, for any logger.
// *Action: none.

185, "yeevGetLog: nqual %d, qual %s, dest %s"
// *Cause: Found a logger, received this data from it.
// *Action: none.

186, "yeevGetLog: no logger found."
// *Cause: No logger was found in the search path of implementations.
// *Action: start a logger somewhere.

190, "yeevBadDest: md %s, victim r %s"
// *Cause: the pushMany dest md is bad to the victim; it will be cleared up
// *Action: none 

191, "yeevBadDest: breaking logger connection"
// *Cause: The bad destination is the main logger.
// *Action: You may need to start a new logger.

195, "yeevSend: local"
// *Cause: the event is being raised to the local handler.
// *Action: none.

200, "yeevSend: global limit start, %d outstanding"
// *Cause: There are too many events in flight; new events will be dropped
// *Action: None, really; perhaps configure for more logger throughput.

201, "yeevSend: batched"
// *Cause: the event is being added to a batch
// *Action: none.

202, "yeevSend: sent"
// *Cause: the event is being sent to a remote receiver.
// *Action: none.

205, "yeevComplete: global limit over, dropped %d events"
// *Cause: global limit has ended, we will start sending events again.
// *Action: none.

206, "yeevComplete: exid %s on completion to md %s"
// *Cause: some error happened sending the event.
// *Action:

210, "yeevAllBatch"
// *Cause: event send batching has been turned on.
// *Action: none.

211, "yeevAllSend"
// *Cause: all batched events are being sent.
// *Action: none.

212, "yeevDiscList_destroy_i %s"
// *Cause: the discriminator list is being destroyed.
// *Action: none.

213, "yeevDiscList_replace_i %s"
// *Cause: the disc list value is being replaced
// *Action: none.

214, "yeevDiscList_append_i %s"
// *Cause: the disc list is being appended
// *Action: none

215, "yeevDiscList_delete_i %s"
// *Cause: an element in a disc list is being deleted. 
// *Action: none.

216, "yeevDiscList_listDest_i %s"
// *Cause: a disc listing for a destination is being done.
// *Action: none.

217, "yeevDiscList_replaceDest_i %s"
// *Cause: discrs for a destination are being replaced
// *Action: none.

218, "yeevDiscList__get_dlist_i %s"
// *Cause: getting a complete disc list.
// *Action: none.

219, "yeevDiscList__get_numEntries_i %s"
// *Cause: getting the number of entried in the disc list.
// *Action: none.

220, "yeevReceiver_destroy_i %s"
// *Cause: a receiver is being destroyed
// *Action: none.

221, "yeevReceiver_push_i %s"
// *Cause: an event is being pushed into a receiver.
// *Action: none.

222, "yeevReceiver_pushMany_i %s (%d)"
// *Cause: many events are being pushed into the receiver.
// *Action: none

223, "yeevReceiver_pull_i %s"
// *Cause: Event is being pulled from the receiver.
// *Action: none.

224, "yeevReceiver_pullMany_i %s"
// *Cause: Many events being pulled from the receiver
// *Action: none.

225, "yeevReceiver_tryPull_i %s"
// *Cause: Non-blocking pull from a receiver.
// *Action: none.

226, "yeevReceiver_tryPullMany_i %s"
// *Cause: Non-blocking pull of many events from a receiver.
// *Action: none.

227, "yeevReceiver__get_name_i %s"
// *Cause: get the name of a receiver.
// *Action: none

228, "yeev_createReceiver_i %s %s"
// *Cause: making a receiver with the name
// *Action: none.

229, "yeev_createReceiver_i: ref %s rcv %s name %s"
// *Cause: created a receiver with the ref
// *Action: none.

230, "yeev_createDiscList_i %s"
// *Cause: creating a discriminator list.
// *Action: none.

231, "yeev_createDiscList_i: ref %s, dl %s with %d quals"
// *Cause: made a discriminator list.
// *Action: none.

232, "yeev_raise_i %s"
// *Cause: raising an event.
// *Action: none.

233, "yeev_raiseMany_i %s - %d events"
// *Cause: raising multiple events.
// *Action: none.

234, "yeev_forward_i %s"
// *Cause: forwarding an event to filtered receivers.
// *Action: none. 

235, "yeev__get_info_i %s"
// *Cause: getting information about the yeev entity.
// *Action: none.

236, "yeev__get_receivers_i %s"
// *Cause: getting list of receivers in the yeev.
// *Action: none.

237, "yeev__get_filters_i %s"
// *Cause: getting the filter list for the yeev.
// *Action: none.

238, "yeev__set_filters_i %s"
// *Cause: setting the filters to a new list of discriminator values.
// *Action: none.

239, "yeev__get_globalEventHighWater_i %s, is %d"
// *Cause: getting the shut off point for events-in-transit.
// *Action: none.

240, "yeev__set_globalEventHighWater_i %s, new %d"
// *Cause: setting the event-in-transit shutoff point.
// *Action: none.

241, "yeev__get_globalEventRestart_i %s, is %d"
// *Cause: getting the restart point for events-in-transit.
// *Action: none.

242, "yeev__set_globalEventRestart_i %s, new %d"
// *Cause:
// *Action:

243, "yeev__get_limit_drop_i %s, is %s"
// *Cause: getting the limit_drop state.
// *Action: none.

244, "yeev__get_limit_drop_i %s, new %s"
// *Cause:
// *Action:

245, "yeevDiscList_destroyDest:  or %s, dest %s" 
// *Cause: 
// *Action:

// *Cause: 
// *Action:

