/ Copyright (c) 1996 by Oracle Corporation.  All Rights Reserved.
/
/ OMNYDRT.msg - OMN YDRT messages
/
// Range of messages: 1000-2000
// 1000
// 1100-1500 yd and subcomponents
//	1100 - yd main
//	1150 - ydrt
//	1200 - ydca
//	1250 - ydim
//	1300
//	1350
//	1400 - ydsp
// 1500 utilities
// 1600 ydmtd and subcomponents
// 1700
// 1800
// 1900

1150, "ydrtInit: routing objects ready"
// *Cause: router is coming up.
// *Action: none.
1151, "ydrtTerm: routing objects terminating"
// *Cause: router is going down
// *Action: none.
1152, "ydrtRoute: got request for %s|%s o:%s qlen %d"
// *Cause: router received a request to forward.
// *Action: none.
1153, "ydrtRoute: non-persistant"
// *Cause: the request is for a non-persistant object.
// *Action: none.
1154, "ydrtRoute: persistant stateful"
// *Cause: for a stateful object; the clearinghouse must be consulted
// *Action: none.
1155, "ydrtRoute: persistant request has tight binding!?"
// *Cause: 
// *Action: internal error - contact your support representative.
1156, "ydrtRoute: forwarding"
// *Cause: the router is forwarding the request.
// *Action: none.
1157, "ydrtRoute: place not ready, request queued"
// *Cause: the dest must be started, so the request will be queued for it
// *Action: none.
1158, "ydrtRoute: No place for %s|%s %s -- rejecting"
// *Cause: no destination was found for the request.  It is being bounced.
// *Action: none.
1159, "ydrtForwardHandler: %s|%s OK to %s"
// *Cause: The request was sucessfully forwarded to the destination
// *Action: none.
1160, "ydrtForwardHandler: Exception %s sending to %s"
// *Cause: The destination has probably died.
// *Action: none.
1161, "ydrtForwardHandler: No more forwards to %s"
// *Cause: Router is giving up sends to that destination.
// *Action: none.
1162, "ydrtForwardHandler: rerouting request"
// *Cause: Router is looking for another destination.
// *Action: none.
1163, "ydrtGoodSpot: %d candidate info entries"
// *Cause: router found some qualifying implementations.
// *Action: none.
1164, "ydrtGoodSpot: %d active entries"
// *Cause: router found some active implementations.
// *Action: none.
1165, "ydrtGoodSpot: cost of %s|%s is %d"
// *Cause: the routing cost of this destination has been determined.
// *Action: none.
1166, "ydrtGoodSpot: new cost %d < old cost %d"
// *Cause: this is a better place to send it than the previous best.
// *Action: none.
1167, "ydrtGoodSpot: equal to best cost %d"
// *Cause: this is as good a place to send as the current best.
// *Action: none.
1168, "ydrtGoodSpot: %d in route list"
// *Cause: there are this many equally best destinations to pick from.
// *Action: none.
1169, "ydrtGoodSpot: nothing found at all"
// *Cause: no qualifying destinations were found.
// *Action: none.
1170, "ydrtGoodSpot: selecting entry %d of %d"
// *Cause: router is picking one of the good destinations to send to.
// *Action: none.
1171, "ydrtGoodSpot: doing launch"
// *Cause: router has picked a launchable destination and is starting it.
// *Action: none.
1172, "ydrtGoodSpot: queueing for already launching proc"
// *Cause: router has selected a destination that is currently starting.
// *Action: none.
1173, "ydrtGoodSpot: picked spot %s, ready %s"
// *Cause: the selected destination is or isn't ready.
// *Action: none.
1174, "ydrtActivation: bad state %d"
// *Cause: internal error
// *Action: report to your support contact.
1175, "ydrtLaunchForward: %d reqs, %d active, %d timedout"
// *Cause: A launching destination has come active.
// *Action: none.
1176, "ydrtLaunchForward: request for %s|%s"
// *Cause: this request was queued.
// *Action: none.
1177, "ydrtLaunchForward: %s|%s active"
// *Cause: this implementation is active
// *Action: none.
1178, "ydrtLaunchForward: sent"
// *Cause: the request was sent to that destionation.
// *Action: none.
1179, "ydrtLaunchForward: rejected request %s|%s"
// *Cause: launch timed out, queued requests are being bounced back.
// *Action: none.
1180, "ydrtLaunchForward: exit, %d requests remaining"
// *Cause: this check is complete, maybe waiting for other impls to activate.
// *Action: none.
1181, "ydrtMetricCost: not found"
// *Cause: no metrics found for the implementation.
// *Action: none.
1182, "ydrtMetricCost: queue cost cost ad:%d + cd:%d + cl:%d * %d"
// *Cause: this is the metric cost for queue length.
// *Action: none.
1183, "ydrtMetricCost: op %s cost %d + %d"
// *Cause: this is the metric cost for the operation.
// *Action: none.
1184, "ydrtMetricCost: returning cost %d"
// *Cause: this is the returned metric cost.
// *Action: none.
1185, "ydrtLaunchForward: sending rqst %s|%s to %s"
// *Cause: The request is being sent to this destination.
// *Action: none.
1186, "ydrtGoodSpot: can't launch %s:%s/%s -- too many (%d) now"
// *Cause: Won't launch new process.  Too many exist now.
// *Action: none.
1187, "ydrtLaunching: entry exid %s, state %d"
// *Cause: trace statement
// *Action: none.
1188, "ydrtLaunching: locking %s"
// *Cause: trace statement
// *Action: none.
1189, "ydrtLaunching: lock try returned %d"
// *Cause: trace statement
// *Action: none.
1190, "ydrtLaunching: Considering launch - have %d, max %d"
// *Cause: trace statement
// *Action: none.
1191, "ydrtLaunching: No launch allowed, at limit %d"
// *Cause: trace statement
// *Action: none.
1192, "ydrtLaunching: spawning %s: %s"
// *Cause: trace statement
// *Action: none.
1193, "ydrtLaunching: sleeping to retry lock"
// *Cause: trace statement
// *Action: none.
1194, "ydrtLaunching: spawn returned exid %s"
// *Cause: trace statement
// *Action: none.
1195, "ydrtLaunching: re-routing"
// *Cause: trace statement
// *Action: none.
1196, "ydrtLaunching: exit state %d"
// *Cause: trace statement
// *Action: none.
1197, "ydrtLaunching: shutting down"
// *Cause: trace statement
// *Action: none.
1198, "ydrtLaunching:  imr_startLaunch"
// *Cause: trace statement
// *Action: none.



