http:1:/* -*-c-*- */
http:118:    -1,				/* Socket number */
http:124:    -1,				/* Data size */
http:125:    -1,				/* Data so far */
http:127:    -1,				/* Return code */
http:190:	next_one = c->next;
http:192:	fprintf(stderr, "Closeing connection at %p on socket %d\n", c, c->socket);
http:254:	    for (c = http_cons_list; c != NULL; c = c->next)
http:257:		       c, c->socket, c->status,
http:258:		       c->data_so_far, c->data_size,
http:259:		       c->fname ? c->fname : "<garbage>",
http:260:		       c->rc,
http:261:		       c->message ? c->message : "<none>" );
http:304:	return http_open(regs->r[0],
http:305:			 regs->r[1],
http:306:			 (char *) regs->r[2],
http:307:			 (http_header_item *) regs->r[3],
http:308:			 (char *) regs->r[4],
http:309:			 (char *) regs->r[5],
http:310:			 (http_connection **) &regs->r[0] );	   
http:319:	c = (http_connection *) regs->r[0];
http:324:	regs->r[0] = c->status ;
http:325:	regs->r[1] = (int) &(c->poll_word) ;
http:326:	regs->r[2] = (int) c->fname ;
http:327:	regs->r[3] = c->data_size ;
http:328:	regs->r[4] = c->data_so_far ;
http:329:	regs->r[5] = (int) c->headers ;
http:330:	regs->r[6] = (int) c->rc ;
http:331:	regs->r[7] = (int) c->message ;
http:341:	c = (http_connection *) regs->r[0];
http:345:	return http_close_handle(c, regs->r[1]);
http:348:	regs->r[0] = mime_map_to_riscos((char *) regs->r[0]);
http:352:	regs->r[0] = (int) mime_map_from_riscos(regs->r[0]);
http:361:	c = (http_connection *) regs->r[0];
http:380:    if (regs->r[0] == Internet_Event)
http:382:	if ( FD_ISSET(regs->r[2], &fd_read))
http:438:		    if (c->status == status_CONNECTING)
http:441:			fprintf(stderr, "Trying another connect of %d\n", c->socket);
http:443:			rc = connect(c->socket, (struct sockaddr *) &(c->sa), sizeof(struct sockaddr));
http:445:			if (rc > -1)
http:448:			    c->status = status_REQUESTING;
http:456:				c->status = status_REQUESTING;
http:464:				c->status = status_CONNECT_FAILED;
http:465:				if (c->fh)
http:467:				    fclose(c->fh);
http:468:				    c->fh = NULL;
http:475:		    if (c->status == status_REQUESTING)
http:482:		    while (c->status == status_REQUEST_HEADERS && rc != EWOULDBLOCK)
http:487:		    while (c->status == status_REQUEST_BODY && rc != EWOULDBLOCK)
http:548:			switch (c->status)
http:560:				while (l && c->status == status_GETTING_HEADERS)
http:563:				    if (c->status == status_GETTING_HEADERS)
http:566:				if (c->status == status_GETTING_BODY)
http:574:			    while (l && c->status == status_GETTING_HEADERS)
http:577:				if (c->status == status_GETTING_HEADERS)
http:580:			    if (c->status == status_GETTING_BODY)
http:588:				rc = recv(c->socket, tmp_buffer, TMP_BUF_SIZE, 0);
http:593:					c->status = status_COMPLEATED;
http:594:					if (c->fh)
http:596:					    fclose(c->fh);
http:597:					    c->fh = NULL;
http:605:				    c->data_so_far += rc;
http:606:				    if (c->fh)
http:607:					fwrite(tmp_buffer, rc, 1, c->fh);
http:614:			if (c->status == status_CONNECT_FAILED ||
http:615:			    c->status == status_REQUEST_FAILED ||
http:616:			    c->status == status_COMPLEATED)
http:622:		    n--;
http:640:    if (c == NULL || c->magic != HTTP_MAGIC)
http:653:	return -1;
http:660:    if (c->fname)
http:664:	for (hi = c->headers; hi != NULL; hi = hi->next)
http:666:	    if (strcasecmp(hi->key, "CONTENT-TYPE") == 0)
http:670:		ft = mime_map_to_riscos(hi->value);
http:671:		if (ft != -1)
http:675:		    fprintf(stderr, "Setting file type of '%s' to 0x%03X\n", c->fname, ft);
http:678:		    _kernel_osfile(18, c->fname, &fb);
http:694:    http_cons[c->socket] = NULL;
http:696:    if (c->fh != NULL)
http:701:	fclose(c->fh);
http:704:    if (c->bh != NULL)
http:709:	fclose(c->bh);
http:720:	if (c->fname)
http:723:	    fprintf(stderr, "Removing file '%s'\n", c->fname);
http:725:	    remove(c->fname);
http:731:	if (c->bname)
http:734:	    fprintf(stderr, "Removing file '%s'\n", c->bname);
http:736:	    remove(c->bname);
http:742:    if (c->fname)
http:747:	free(c->fname);
http:750:    if (c->bname)
http:755:	free(c->bname);
http:758:    if (c->message)
http:763:	free(c->message);
http:766:    if (c->object)
http:771:	free(c->object);
http:774:    http_free_headers(c->headers); /* The function does its own check for NULL pointers */
http:777:    c->magic = 0;
http:782:    if (c->prev == NULL)
http:783:	http_cons_list = c->next;
http:785:	c->prev->next = c->next;
http:787:    if (c->next != NULL)
http:788:	c->next->prev = c->prev;
http:813:    (*new)->key = strdup(h->key);
http:814:    (*new)->value = strdup(h->value);
http:816:    http_copy_headers(h->next, &((*new)->next));
http:825:	hn = h->next;
http:830:	if (h->key)
http:831:	    free(h->key);
http:832:	if (h->value)
http:833:	    free(h->value);
http:860:    new->buffer = (char*) (new + 1);
http:861:    new->buffer[0] = 0;
http:863:    memcpy((char *)&(new->sa.sin_addr),(char *) &host,  sizeof(host) );
http:864:    new->sa.sin_family = AF_INET;    
http:865:    new->sa.sin_port = port;    
http:867:    if ( (new->socket = socket(AF_INET, SOCK_STREAM, 0)) <0)
http:873:    fprintf(stderr, "New socket is %d\n", new->socket);
http:877:    rc = socketioctl(new->socket, FIONBIO, &i);
http:878:    if (rc == -1)
http:882:    rc = socketioctl(new->socket, FIOASYNC, &i);
http:883:    if (rc == -1)
http:890:    rc = connect(new->socket, (struct sockaddr *) &(new->sa), sizeof(struct sockaddr));
http:914:    new->object = strdup(object);
http:915:    if (new->object == NULL)
http:920:    http_copy_headers(headers, &(new->sendhead) );
http:927:    new->fname = strdup(fname);
http:928:    if (new->fname == NULL)
http:934:    fprintf(stderr, "Opening file '%s'\n", new->fname);
http:937:    new->fh = fopen(new->fname, "wb");
http:938:    if (new->fh == NULL)
http:945:	new->bname = strdup(bname);
http:946:	if (new->bname == NULL)
http:952:	fprintf(stderr, "Opening body file '%s'\n", new->bname);
http:955:	new->bh = fopen(new->bname, "rb");
http:956:	if (new->bh == NULL)
http:962:    rc = connect(new->socket, (struct sockaddr *) &(new->sa), sizeof(struct sockaddr));
http:969:	    new->status = status_REQUESTING;
http:972:	    new->status = status_CONNECTING;
http:982:	new->status = status_REQUESTING;
http:986:    http_cons[new->socket] = new;
http:988:    FD_SET(new->socket, &fd_write);
http:991:    fprintf(stderr, "ISSET: %ld %ld\n", FD_ISSET(new->socket, &fd_read), FD_ISSET(new->socket, &fd_write) );
http:994:    if (new->status == status_REQUESTING)
http:1001:    while (new->status == status_REQUEST_HEADERS && rc != EWOULDBLOCK)
http:1006:    while (new->status == status_REQUEST_BODY && rc != EWOULDBLOCK)
http:1011:    new->next = http_cons_list;
http:1012:    /* new->prev is set to NULL in the prototype */
http:1015:	http_cons_list->prev = new;
http:1113:    sprintf(tmp_buffer, "%s %s %s\r\n", c->bh ? "POST" : "GET", c->object, http_version);
http:1118:    if (send(c->socket, tmp_buffer, strlen(tmp_buffer), 0) < 0)
http:1120:	c->status = status_REQUEST_FAILED;
http:1121:	if (c->fh)
http:1123:	    fclose(c->fh);
http:1124:	    c->fh = NULL;
http:1129:	c->status = status_REQUEST_HEADERS;
http:1136:    if (c->sendhead)
http:1137:	sprintf(tmp_buffer, "%s: %s\r\n", c->sendhead->key, c->sendhead->value);
http:1140:	if (c->bh)
http:1142:	    if (c->data_size == -1)
http:1143:		c->data_size = http_file_size(c->bname);
http:1144:	    sprintf(tmp_buffer, "Content-length: %d\r\n\r\n", c->data_size);
http:1154:    if (send(c->socket, tmp_buffer, strlen(tmp_buffer), 0) < 0)
http:1158:	    c->status = status_REQUEST_FAILED;
http:1159:	    if (c->fh)
http:1161:		fclose(c->fh);
http:1162:		c->fh = NULL;
http:1173:	if (c->sendhead)
http:1174:	    c->sendhead = c->sendhead->next;
http:1177:	    if (c->bh)
http:1179:		c->status = status_REQUEST_BODY;
http:1180:		fread(c->buffer, 1, c->data_size > MAX_INPUT ? MAX_INPUT : c->data_size, c->bh);
http:1181:		c->data_so_far = 0;
http:1185:		shutdown(c->socket, 1);
http:1186:		c->status = status_WAITING;
http:1187:		FD_CLR(c->socket, &fd_write);
http:1188:		FD_SET(c->socket, &fd_read);
http:1202:    /* This is always entered with the c->buffer as full as it can be,
http:1203:     * c->data_size the size of the file to be sent and c->data_so_far
http:1208:    fprintf(stderr, "Sending data: so far %d of %d\n", c->data_so_far, c->data_size);
http:1211:    to_go = c->data_size - c->data_so_far;
http:1214:	sent = send(c->socket, c->buffer, to_go > MAX_INPUT ? MAX_INPUT : to_go, 0);
http:1222:	    c->status = status_REQUEST_FAILED;
http:1223:	    if (c->fh)
http:1225:		fclose(c->fh);
http:1226:		c->fh = NULL;
http:1237:	c->data_so_far += sent;
http:1238:	to_go -= sent;
http:1243:		memmove(c->buffer, c->buffer + sent, MAX_INPUT - sent);
http:1245:	    fread(c->buffer + MAX_INPUT - sent, 1, sent > to_go ? to_go : sent, c->bh);
http:1249:	    c->data_size = c->data_so_far = -1;
http:1250:	    shutdown(c->socket, 1);
http:1251:	    c->status = status_WAITING;
http:1252:	    FD_CLR(c->socket, &fd_write);
http:1253:	    FD_SET(c->socket, &fd_read);
http:1267:    if (c->buf_used)
http:1269:	memmove(c->buffer, c->buffer + c->buf_used, c->buf_off - c->buf_used);
http:1270:	c->buf_off -= c->buf_used;
http:1271:	c->buf_used = 0;
http:1277:	rc = recv(c->socket, c->buffer + c->buf_off, MAX_INPUT - c->buf_off - 1, 0);
http:1282:		c->status = status_REQUEST_FAILED;
http:1283:		if (c->fh)
http:1285:		    fclose(c->fh);
http:1286:		    c->fh = NULL;
http:1293:	    c->buf_off += rc;
http:1294:	    c->buffer[c->buf_off] = 0; /* Make use we always have a NULL at the end */
http:1298:    end = strchr(c->buffer, '\n');
http:1302:	c->buf_used = (end - c->buffer) + 1;
http:1303:	if (end[-1] == '\r')
http:1304:	    end[-1] = 0;
http:1306:	return c->buffer;
http:1308:    else if (MAX_INPUT - c->buf_off - 1 == 0) /* Phrased like this as this is the formula used in the recv() call */
http:1311:	c->buf_used = c->buf_off + 1;
http:1312:	return c->buffer;
http:1326:	c->status = status_GETTING_BODY;
http:1342:		hi->key = strdup(l);
http:1343:		hi->value = strdup(p); 
http:1345:		if (hi->key == NULL || hi->value == NULL)
http:1347:		    if (hi->key)
http:1348:			free(hi->key);
http:1349:		    if (hi->value)
http:1350:			free(hi->value);
http:1356:		hi->next = c->headers;
http:1357:		c->headers = hi;
http:1383:	c->rc = atoi(p);
http:1387:	c->message = strdup(p);
http:1389:	fprintf(stderr, "Response: %d '%s'\n", c->rc, c->message ? c->message : "<no message>");
http:1391:	c->status = status_GETTING_HEADERS;
http:1398:	c->status = status_REQUEST_FAILED;
http:1399:	if (c->fh)
http:1401:	    fclose(c->fh);
http:1402:	    c->fh = NULL;
http:1409:    c->data_so_far = 0;
http:1411:    if (c->fh && (c->buf_off != c->buf_used))
http:1415:	n = c->buf_off - c->buf_used;
http:1416:	fwrite(c->buffer + c->buf_used, n, 1, c->fh);
http:1417:	c->data_so_far = n;
http:1423:    if (c->socket != -1)
http:1426:	fprintf(stderr, "Closing socket %d\n", c->socket);
http:1429:	FD_CLR(c->socket, &fd_read);
http:1430:	FD_CLR(c->socket, &fd_write);
http:1432:	http_cons[c->socket] = NULL;
http:1434:	socketclose(c->socket);
http:1435:	c->socket = -1;
http:1448:	    return c1-c2;
http:1451:	    t = toupper(c1) - toupper(c2);
http:1460:    if (strcasecmp(hi->key, "CONTENT-LENGTH") == 0)
http:1462:	c->data_size = atoi(hi->value);
