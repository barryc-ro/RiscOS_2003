/ Copyright (c) 1996 by Oracle Corporation.  All Rights Reserved.
/
/ OMNYD.msg - OMN YD messages
/
// YD range of messages: 1000-2000
// 1000 
// 1100-1400 yd and subcomponents
//	1100 - yd main
//	1150 - ydrt
//	1200 - ydca
//	1250 - ydim
//	1300
//	1350
// 1400 ydmtd and subcomponents
// 1500 utilities
// 1600
// 1700
// 1800
// 1900

1100, "ydServer: Becoming ORBD routing server"
// *Cause: mnorbsrv is starting.
// *Action: none.
1101, "ydServer: %d orbds, getting claim"
// *Cause: mnorbsrv getting exclusive rights to start and synchronize.
// *Action: none.
1102, "ydServer: got claim, syncing"
// *Cause: mnorbsrv got rights to start and sync.
// *Action: none.
1103, "ydServer: sync completed"
// *Cause: startup synchronization is complete.
// *Action: none.
1104, "ydServer: no orbds, getting rpc lock"
// *Cause: first mnorbsrv getting bootstrap lock with RPC name server.
// *Action: none.
1105, "ydServer: got rpc lock, starting"
// *Cause: first mnorbsrv is becoming a request router.
// *Action: none.
1106, "ydServer: routing started, starting service loop"
// *Cause: normal startup after sync completed.
// *Action: none.
1107, "ydServer: sync failed, abandoning startup"
// *Cause: Startup sync failed, so this mnorbsrv can't start.
// *Action: check the other mnorbsrv programs.
1108, "ydServer: exited service loop"
// *Cause: server is done normal object processing.
// *Action: none.
1109, "ydServer: exiting"
// *Cause: server is about to do yoTerm and transport term.
// *Action: expect sometimes lengthy transport termination.
1110, "ydServerTerm: stopping object services"
// *Cause: mnorbsrv is stopping handling object requeste.
// *Action: none.
1111, "ydPanicHdlr: exception %s"
// *Cause: internal error.
// *Action: report to your support contact.
1112, "ydIntrHdlr: program interrupted, shutting down"
// *Cause: user interrupt detected.
// *Action: none.
1113, "ydSyncOrbds: syncing from %s"
// *Cause: normal start of synchronization with another mnorbsrv.
// *Action: none.
1114, "ydSyncOrbds: waiting for sync with %d orbds"
// *Cause: normal wait for startup synchronization
// *Action: none.
1115, "ydSyncHandler: sync operations completed normally"
// *Cause: normal completion of startup sync operations.
// *Action: none.
1116, "ydSyncHandler: sync operations completed with an error"
// *Cause: Something bad happened synchronizing with existing mnorbsrvs
// *Action: check the existing mnorbsrv processes

1201, "ydcaSetInactiveEvt: wait for %d to become inactive"
// *Cause: someone is waiting for claim/abandon to be inactive.
// *Action: none.

1202, "ydcaInit: claim/abandon ready to use",
// *Cause: the claim abandon module has come up.
// *Action: none.

1203, "ydcaInit: claim/abandon shut down",
// *Cause: claim/abandon has been shut down in a process.
// *Action: none.
1204, "ydcaStakeFor: property %s"
// *Cause: someone is trying to stake a claim on the property.
// *Action: none.
1205, "ydcaTransfer: property %s"
// *Cause: someone is abandoning a claim on the property.
// *Action: none.
1206, "ydcaAbandon: property %s"
// *Cause: someone is trying to transfer the claim on the property.
// *Action: none.
1207, "ydcaTryStake: property %s""
// *Cause: first phase of a claim on the property
// *Action: none.
1208, "ydcaTransferStake: property %s"
// *Cause: first phase of a claim transfer on property.
// *Action: none.
1209, "ydcaAbortStake: property %s"
// *Cause: abort local claim on property.
// *Action: none.
1210, "ydcaCommitStake: property %s"
// *Cause: local commit of claim on property.
// *Action:
1211, "ydcaListNext: next of property %s is %s"
// *Cause: listing of claims, in order.
// *Action: none.
1212, "ydcaStartMachine: unexpected op %s"
// *Cause: internal error.
// *Action: report to your support contact.
1213, "ydcaKillMachine: called"
// *Cause: async machine processing a claim is being killed.
// *Action: none.
1214, "ydcaMachine: got %d orbds in location %d"
// *Cause: These orbds will be updated to process the request.
// *Action:
1215, "ydcaMachine: op %s, exid %s"
// *Cause: async machine operation, processing a claim request.
// *Action: none.
1216, "ydcaMachine: %s result was %s"
// *Cause: try or transfer operation intermediate result
// *Action: none.
1217, "ydcaMachine: transfer failed, abandoning %s"
// *Cause: error during transfer.   The property will be abandoned.
// *Action: none.
1218, "ydcaMachine: unexpected op %d in location %d"
// *Cause: internal error
// *Action: report to your support representative.
1219, "ydcaDeath: owner of property %s died, abandoning it" */
// *Cause: the owner of record died.  The system is cleaning up.
// *Action: none.
1220, "ydcaCommitStake: bad state %d"
// *Cause: internal error.
// *Action: report to your support contact.
