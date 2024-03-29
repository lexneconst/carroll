#include <libcrt.h>

#define NIBBLE_TO_ASCII(c)  ((c) < 0x0a ? (c) + 0x30 : (c) + 0x57)

/* Test modes */
enum {
	SEND,
	RECV,
	RECONNECT,
	MULTY,
	DUMP,
	CONNECT,
	CRECV,
	LSEND,
	SENDDUMP,
	LSENDDUMP,
	LSENDRECV,
	CSENDRECV,
	INFOREQ,
	PAIRING,
};

 unsigned char *buf;

/* Default mtu */
 int imtu = 672;
 int omtu = 0;

/* Default FCS option */
 int fcs = 0x01;

/* Default Transmission Window */
 int txwin_size = 63;

/* Default Max Transmission */
 int max_transmit = 3;

/* Default data size */
 long data_size = -1;
 long buffer_size = 2048;

/* Default addr and psm and cid */
 bdaddr_t bdaddr;
 unsigned short psm = 0x1011;
 unsigned short cid = 0;

/* Default number of frames to send (-1 = infinite) */
 int num_frames = -1;

/* Default number of consecutive frames before the delay */
 int count = 1;

/* Default delay after sending count number of frames */
 unsigned long send_delay = 0;

/* Default delay before receiving */
 unsigned long recv_delay = 0;

 char *filename = NULL;

 int rfcmode = 0;
 int master = 0;
 int auth = 0;
 int encr = 0;
 int secure = 0;
 int socktype = SOCK_SEQPACKET;
 int linger = 0;
 int reliable = 0;
 int timestamp = 0;
 int defer_setup = 0;
 int priority = -1;
 int rcvbuf = 0;
 int chan_policy = -1;
 int bdaddr_type = 0;

struct lookup_table {
	const char *name;
	int flag;
};

 struct lookup_table l2cap_modes[] = {
	{ "basic",	L2CAP_MODE_BASIC	},
	/* Not implemented
	{ "flowctl",	L2CAP_MODE_FLOWCTL	},
	{ "retrans",	L2CAP_MODE_RETRANS	},
	*/
	{ "ertm",	L2CAP_MODE_ERTM		},
	{ "streaming",	L2CAP_MODE_STREAMING	},
	{ 0 }
};

 struct lookup_table chan_policies[] = {
	{ "bredr",	BT_CHANNEL_POLICY_BREDR_ONLY		},
	{ "bredr_pref",	BT_CHANNEL_POLICY_BREDR_PREFERRED	},
	{ "amp_pref",	BT_CHANNEL_POLICY_AMP_PREFERRED		},
	{ NULL,		0					},
};

 struct lookup_table bdaddr_types[] = {
	{ "bredr",	BDADDR_BREDR		},
	{ "le_public",	BDADDR_LE_PUBLIC	},
	{ "le_random",	BDADDR_LE_RANDOM	},
	{ NULL,		0			},
};

 int get_lookup_flag(struct lookup_table *table, char *name)
{
	int i;

	for (i = 0; table[i].name; i++)
		if (!strcasecmp(table[i].name, name))
			return table[i].flag;

	return -1;
}

 const char *get_lookup_str(struct lookup_table *table, int flag)
{
	int i;

	for (i = 0; table[i].name; i++)
		if (table[i].flag == flag)
			return table[i].name;

	return NULL;
}

 void print_lookup_values(struct lookup_table *table, char *header)
{
	int i;

	printf("%s\n", header);

	for (i = 0; table[i].name; i++)
		printf("\t%s\n", table[i].name);
}

 float tv2fl(struct timeval tv)
{
	return (float)tv.tv_sec + (float)(tv.tv_usec/1000000.0);
}

 char *ltoh(unsigned long c, char *s)
{
	int c1;

	c1     = (c >> 28) & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	c1     = (c >> 24) & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	c1     = (c >> 20) & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	c1     = (c >> 16) & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	c1     = (c >> 12) & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	c1     = (c >>  8) & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	c1     = (c >>  4) & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	c1     = c & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	*s     = 0;
	return s;
}

 char *ctoh(char c, char *s)
{
	char c1;

	c1     = (c >> 4) & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	c1     = c & 0x0f;
	*(s++) = NIBBLE_TO_ASCII (c1);
	*s     = 0;
	return s;
}

 void hexdump(unsigned char *s, unsigned long l)
{
	char bfr[80];
	char *pb;
	unsigned long i, n = 0;

	if (l == 0)
		return;

	while (n < l) {
		pb = bfr;
		pb = ltoh (n, pb);
		*(pb++) = ':';
		*(pb++) = ' ';
		for (i = 0; i < 16; i++) {
			if (n + i >= l) {
				*(pb++) = ' ';
				*(pb++) = ' ';
			} else
				pb = ctoh (*(s + i), pb);
			*(pb++) = ' ';
		}
		*(pb++) = ' ';
		for (i = 0; i < 16; i++) {
			if (n + i >= l)
				break;
			else
				*(pb++) = (isprint (*(s + i)) ? *(s + i) : '.');
		}
		*pb = 0;
		n += 16;
		s += 16;
		puts(bfr);
	}
}

 int do_connect(char *svr)
{
	struct sockaddr_l2 addr;
	struct l2cap_options opts;
	struct l2cap_conninfo conn;
	socklen_t optlen;
	int sk, opt;
	char ba[18];

	/* Create socket */
	sk = socket(PF_BLUETOOTH, socktype, BTPROTO_L2CAP);
	if (sk < 0) {
		syslog(LOG_ERR, "Can't create socket: %s (%d)",
							strerror(errno), errno);
		return -1;
	}

	/* Bind to local address */
	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, &bdaddr);
	addr.l2_bdaddr_type = bdaddr_type;

	if (bind(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		syslog(LOG_ERR, "Can't bind socket: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Get default options */
	memset(&opts, 0, sizeof(opts));
	optlen = sizeof(opts);

	if (getsockopt(sk, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get default L2CAP options: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Set new options */
	opts.omtu = omtu;
	opts.imtu = imtu;
	opts.mode = rfcmode;

	opts.fcs = fcs;
	opts.txwin_size = txwin_size;
	opts.max_tx = max_transmit;

	if (setsockopt(sk, SOL_L2CAP, L2CAP_OPTIONS, &opts, sizeof(opts)) < 0) {
		syslog(LOG_ERR, "Can't set L2CAP options: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

#if 0
	/* Enable SO_TIMESTAMP */
	if (timestamp) {
		int t = 1;

		if (setsockopt(sk, SOL_SOCKET, SO_TIMESTAMP, &t, sizeof(t)) < 0) {
			syslog(LOG_ERR, "Can't enable SO_TIMESTAMP: %s (%d)",
							strerror(errno), errno);
			goto error;
		}
	}
#endif

	if (chan_policy != -1) {
		if (setsockopt(sk, SOL_BLUETOOTH, BT_CHANNEL_POLICY,
				&chan_policy, sizeof(chan_policy)) < 0) {
			syslog(LOG_ERR, "Can't enable chan policy : %s (%d)",
							strerror(errno), errno);
			goto error;
		}
	}

	/* Enable SO_LINGER */
	if (linger) {
		struct linger l = { .l_onoff = 1, .l_linger = linger };

		if (setsockopt(sk, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0) {
			syslog(LOG_ERR, "Can't enable SO_LINGER: %s (%d)",
							strerror(errno), errno);
			goto error;
		}
	}

	/* Set link mode */
	opt = 0;
	if (reliable)
		opt |= L2CAP_LM_RELIABLE;
	if (master)
		opt |= L2CAP_LM_MASTER;
	if (auth)
		opt |= L2CAP_LM_AUTH;
	if (encr)
		opt |= L2CAP_LM_ENCRYPT;
	if (secure)
		opt |= L2CAP_LM_SECURE;

	if (setsockopt(sk, SOL_L2CAP, L2CAP_LM, &opt, sizeof(opt)) < 0) {
		syslog(LOG_ERR, "Can't set L2CAP link mode: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Set receive buffer size */
	if (rcvbuf && setsockopt(sk, SOL_SOCKET, SO_RCVBUF,
						&rcvbuf, sizeof(rcvbuf)) < 0) {
		syslog(LOG_ERR, "Can't set socket rcv buf size: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	optlen = sizeof(rcvbuf);
	if (getsockopt(sk, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get socket rcv buf size: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Connect to remote device */
	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	str2ba(svr, &addr.l2_bdaddr);
	addr.l2_bdaddr_type = bdaddr_type;
	if (cid)
		addr.l2_cid = htobs(cid);
	else if (psm)
		addr.l2_psm = htobs(psm);
	else
		goto error;

	if (connect(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0 ) {
		syslog(LOG_ERR, "Can't connect: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Get current options */
	memset(&opts, 0, sizeof(opts));
	optlen = sizeof(opts);

	if (getsockopt(sk, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get L2CAP options: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Get connection information */
	memset(&conn, 0, sizeof(conn));
	optlen = sizeof(conn);

	if (getsockopt(sk, SOL_L2CAP, L2CAP_CONNINFO, &conn, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get L2CAP connection information: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	if (priority > 0 && setsockopt(sk, SOL_SOCKET, SO_PRIORITY, &priority,
						sizeof(priority)) < 0) {
		syslog(LOG_ERR, "Can't set socket priority: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	if (getsockopt(sk, SOL_SOCKET, SO_PRIORITY, &opt, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get socket priority: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Check for remote address */
	memset(&addr, 0, sizeof(addr));
	optlen = sizeof(addr);

	if (getpeername(sk, (struct sockaddr *) &addr, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get socket name: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	ba2str(&addr.l2_bdaddr, ba);
	syslog(LOG_INFO, "Connected to %s (%s, psm %d, scid %d)", ba,
		get_lookup_str(bdaddr_types, addr.l2_bdaddr_type),
		addr.l2_psm, addr.l2_cid);

	/* Check for socket address */
	memset(&addr, 0, sizeof(addr));
	optlen = sizeof(addr);

	if (getsockname(sk, (struct sockaddr *) &addr, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get socket name: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	ba2str(&addr.l2_bdaddr, ba);
	syslog(LOG_INFO, "Local device %s (%s, psm %d, scid %d)", ba,
		get_lookup_str(bdaddr_types, addr.l2_bdaddr_type),
		addr.l2_psm, addr.l2_cid);

	syslog(LOG_INFO, "Options [imtu %d, omtu %d, flush_to %d, "
		"mode %d, handle %d, class 0x%02x%02x%02x, priority %d, rcvbuf %d]",
		opts.imtu, opts.omtu, opts.flush_to, opts.mode, conn.hci_handle,
		conn.dev_class[2], conn.dev_class[1], conn.dev_class[0], opt,
		rcvbuf);

	omtu = (opts.omtu > buffer_size) ? buffer_size : opts.omtu;
	imtu = (opts.imtu > buffer_size) ? buffer_size : opts.imtu;

	return sk;

error:
	close(sk);
	return -1;
}

 void do_listen(void (*handler)(int sk))
{
	struct sockaddr_l2 addr;
	struct l2cap_options opts;
	struct l2cap_conninfo conn;
	socklen_t optlen;
	int sk, nsk, opt;
	char ba[18];

	/* Create socket */
	sk = socket(PF_BLUETOOTH, socktype, BTPROTO_L2CAP);
	if (sk < 0) {
		syslog(LOG_ERR, "Can't create socket: %s (%d)",
							strerror(errno), errno);
		exit(1);
	}

	/* Bind to local address */
	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, &bdaddr);
	addr.l2_bdaddr_type = bdaddr_type;
	if (cid)
		addr.l2_cid = htobs(cid);
	else if (psm)
		addr.l2_psm = htobs(psm);

	if (bind(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		syslog(LOG_ERR, "Can't bind socket: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Set link mode */
	opt = 0;
	if (reliable)
		opt |= L2CAP_LM_RELIABLE;
	if (master)
		opt |= L2CAP_LM_MASTER;
	if (auth)
		opt |= L2CAP_LM_AUTH;
	if (encr)
		opt |= L2CAP_LM_ENCRYPT;
	if (secure)
		opt |= L2CAP_LM_SECURE;

	if (opt && setsockopt(sk, SOL_L2CAP, L2CAP_LM, &opt, sizeof(opt)) < 0) {
		syslog(LOG_ERR, "Can't set L2CAP link mode: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Get default options */
	memset(&opts, 0, sizeof(opts));
	optlen = sizeof(opts);

	if (getsockopt(sk, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get default L2CAP options: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Set new options */
	opts.omtu = omtu;
	opts.imtu = imtu;
	if (rfcmode > 0)
		opts.mode = rfcmode;

	opts.fcs = fcs;
	opts.txwin_size = txwin_size;
	opts.max_tx = max_transmit;

	if (setsockopt(sk, SOL_L2CAP, L2CAP_OPTIONS, &opts, sizeof(opts)) < 0) {
		syslog(LOG_ERR, "Can't set L2CAP options: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	if (socktype == SOCK_DGRAM) {
		handler(sk);
		return;
	}

	/* Enable deferred setup */
	opt = defer_setup;

	if (opt && setsockopt(sk, SOL_BLUETOOTH, BT_DEFER_SETUP,
						&opt, sizeof(opt)) < 0) {
		syslog(LOG_ERR, "Can't enable deferred setup : %s (%d)",
							strerror(errno), errno);
		goto error;
	}


	/* Listen for connections */
	if (listen(sk, 10)) {
		syslog(LOG_ERR, "Can not listen on the socket: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	/* Check for socket address */
	memset(&addr, 0, sizeof(addr));
	optlen = sizeof(addr);

	if (getsockname(sk, (struct sockaddr *) &addr, &optlen) < 0) {
		syslog(LOG_ERR, "Can't get socket name: %s (%d)",
							strerror(errno), errno);
		goto error;
	}

	psm = btohs(addr.l2_psm);
	cid = btohs(addr.l2_cid);

	syslog(LOG_INFO, "Waiting for connection on psm %d ...", psm);

	while (1) {
		memset(&addr, 0, sizeof(addr));
		optlen = sizeof(addr);

		nsk = accept(sk, (struct sockaddr *) &addr, &optlen);
		if (nsk < 0) {
			syslog(LOG_ERR, "Accept failed: %s (%d)",
							strerror(errno), errno);
			goto error;
		}
		if (fork()) {
			/* Parent */
			close(nsk);
			continue;
		}
		/* Child */
		close(sk);

		/* Set receive buffer size */
		if (rcvbuf && setsockopt(nsk, SOL_SOCKET, SO_RCVBUF, &rcvbuf,
							sizeof(rcvbuf)) < 0) {
			syslog(LOG_ERR, "Can't set rcv buf size: %s (%d)",
							strerror(errno), errno);
			goto error;
		}

		optlen = sizeof(rcvbuf);
		if (getsockopt(nsk, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &optlen)
									< 0) {
			syslog(LOG_ERR, "Can't get rcv buf size: %s (%d)",
							strerror(errno), errno);
			goto error;
		}

		/* Get current options */
		memset(&opts, 0, sizeof(opts));
		optlen = sizeof(opts);

		if (getsockopt(nsk, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen) < 0) {
			syslog(LOG_ERR, "Can't get L2CAP options: %s (%d)",
							strerror(errno), errno);
			if (!defer_setup) {
				close(nsk);
				goto error;
			}
		}

		/* Get connection information */
		memset(&conn, 0, sizeof(conn));
		optlen = sizeof(conn);

		if (getsockopt(nsk, SOL_L2CAP, L2CAP_CONNINFO, &conn, &optlen) < 0) {
			syslog(LOG_ERR, "Can't get L2CAP connection information: %s (%d)",
							strerror(errno), errno);
			if (!defer_setup) {
				close(nsk);
				goto error;
			}
		}

		if (priority > 0 && setsockopt(nsk, SOL_SOCKET, SO_PRIORITY,
					&priority, sizeof(priority)) < 0) {
			syslog(LOG_ERR, "Can't set socket priority: %s (%d)",
						strerror(errno), errno);
			close(nsk);
			goto error;
		}

		optlen = sizeof(priority);
		if (getsockopt(nsk, SOL_SOCKET, SO_PRIORITY, &opt, &optlen) < 0) {
			syslog(LOG_ERR, "Can't get socket priority: %s (%d)",
							strerror(errno), errno);
			goto error;
		}

		ba2str(&addr.l2_bdaddr, ba);
		syslog(LOG_INFO, "Connect from %s (%s, psm %d, dcid %d)", ba,
				get_lookup_str(bdaddr_types, addr.l2_bdaddr_type),
				addr.l2_psm, addr.l2_cid);

		/* Check for socket address */
		memset(&addr, 0, sizeof(addr));
		optlen = sizeof(addr);

		if (getsockname(nsk, (struct sockaddr *) &addr, &optlen) < 0) {
			syslog(LOG_ERR, "Can't get socket name: %s (%d)",
							strerror(errno), errno);
			goto error;
		}

		ba2str(&addr.l2_bdaddr, ba);
		syslog(LOG_INFO, "Local device %s (%s, psm %d, scid %d)", ba,
				get_lookup_str(bdaddr_types, addr.l2_bdaddr_type),
				addr.l2_psm, addr.l2_cid);

		syslog(LOG_INFO, "Options [imtu %d, omtu %d, "
				"flush_to %d, mode %d, handle %d, "
				"class 0x%02x%02x%02x, priority %d, rcvbuf %d]",
				opts.imtu, opts.omtu, opts.flush_to,
				opts.mode, conn.hci_handle, conn.dev_class[2],
				conn.dev_class[1], conn.dev_class[0], opt,
				rcvbuf);

		omtu = (opts.omtu > buffer_size) ? buffer_size : opts.omtu;
		imtu = (opts.imtu > buffer_size) ? buffer_size : opts.imtu;

#if 0
		/* Enable SO_TIMESTAMP */
		if (timestamp) {
			int t = 1;

			if (setsockopt(nsk, SOL_SOCKET, SO_TIMESTAMP, &t, sizeof(t)) < 0) {
				syslog(LOG_ERR, "Can't enable SO_TIMESTAMP: %s (%d)",
							strerror(errno), errno);
				goto error;
			}
		}
#endif

		/* Enable SO_LINGER */
		if (linger) {
			struct linger l = { .l_onoff = 1, .l_linger = linger };

			if (setsockopt(nsk, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0) {
				syslog(LOG_ERR, "Can't enable SO_LINGER: %s (%d)",
							strerror(errno), errno);
				close(nsk);
				goto error;
			}
		}

		/* Handle deferred setup */
		if (defer_setup) {
			syslog(LOG_INFO, "Waiting for %d seconds",
							abs(defer_setup) - 1);
			sleep(abs(defer_setup) - 1);

			if (defer_setup < 0) {
				close(nsk);
				goto error;
			}
		}

		handler(nsk);

		syslog(LOG_INFO, "Disconnect: %m");
		exit(0);
	}

	return;

error:
	close(sk);
	exit(1);
}

 void dump_mode(int sk)
{
	socklen_t optlen;
	int opt, len;

	if (data_size < 0)
		data_size = imtu;

	if (defer_setup) {
		len = read(sk, buf, sizeof(buf));
		if (len < 0)
			syslog(LOG_ERR, "Initial read error: %s (%d)",
						strerror(errno), errno);
		else
			syslog(LOG_INFO, "Initial bytes %d", len);
	}

	syslog(LOG_INFO, "Receiving ...");
	while (1) {
		fd_set rset;

		FD_ZERO(&rset);
		FD_SET(sk, &rset);

		if (select(sk + 1, &rset, NULL, NULL, NULL) < 0)
			return;

		if (!FD_ISSET(sk, &rset))
			continue;

		len = read(sk, buf, data_size);
		if (len <= 0) {
			if (len < 0) {
				if (reliable && (errno == ECOMM)) {
					syslog(LOG_INFO, "L2CAP Error ECOMM - clearing error and continuing.");
					optlen = sizeof(opt);
					if (getsockopt(sk, SOL_SOCKET, SO_ERROR, &opt, &optlen) < 0) {
						syslog(LOG_ERR, "Couldn't getsockopt(SO_ERROR): %s (%d)",
							strerror(errno), errno);
						return;
					}
					continue;
				} else {
					syslog(LOG_ERR, "Read error: %s(%d)",
							strerror(errno), errno);
				}
			}
			return;
		}

		syslog(LOG_INFO, "Recevied %d bytes", len);
		hexdump(buf, len);
	}
}

 void recv_mode(int sk)
{
	struct timeval tv_beg, tv_end, tv_diff;
	struct pollfd p;
	char ts[30];
	long total;
	uint32_t seq;
	socklen_t optlen;
	int opt, len;

	if (data_size < 0)
		data_size = imtu;

	if (defer_setup) {
		len = read(sk, buf, sizeof(buf));
		if (len < 0)
			syslog(LOG_ERR, "Initial read error: %s (%d)",
						strerror(errno), errno);
		else
			syslog(LOG_INFO, "Initial bytes %d", len);
	}

	if (recv_delay)
		usleep(recv_delay);

	syslog(LOG_INFO, "Receiving ...");

	memset(ts, 0, sizeof(ts));

	p.fd = sk;
	p.events = POLLIN | POLLERR | POLLHUP;

	seq = 0;
	while (1) {
		gettimeofday(&tv_beg, NULL);
		total = 0;
		while (total < data_size) {
			uint32_t sq;
			uint16_t l;
			int i;

			p.revents = 0;
			if (poll(&p, 1, -1) <= 0)
				return;

			if (p.revents & (POLLERR | POLLHUP))
				return;

			len = recv(sk, buf, data_size, 0);
			if (len < 0) {
				if (reliable && (errno == ECOMM)) {
					syslog(LOG_INFO, "L2CAP Error ECOMM - clearing error and continuing.\n");
					optlen = sizeof(opt);
					if (getsockopt(sk, SOL_SOCKET, SO_ERROR, &opt, &optlen) < 0) {
						syslog(LOG_ERR, "Couldn't getsockopt(SO_ERROR): %s (%d)",
							strerror(errno), errno);
						return;
					}
					continue;
				} else {
					syslog(LOG_ERR, "Read failed: %s (%d)",
						strerror(errno), errno);
				}
			}

			if (len < 6)
				break;

			if (timestamp) {
				struct timeval tv;

				if (ioctl(sk, SIOCGSTAMP, &tv) < 0) {
					timestamp = 0;
					memset(ts, 0, sizeof(ts));
				} else {
					sprintf(ts, "[%ld.%ld] ",
							tv.tv_sec, tv.tv_usec);
				}
			}

			/* Check sequence */
			sq = bt_get_le32(buf);
			if (seq != sq) {
				syslog(LOG_INFO, "seq missmatch: %d -> %d", seq, sq);
				seq = sq;
			}
			seq++;

			/* Check length */
			l = bt_get_le16(buf + 4);
			if (len != l) {
				syslog(LOG_INFO, "size missmatch: %d -> %d", len, l);
				continue;
			}

			/* Verify data */
			for (i = 6; i < len; i++) {
				if (buf[i] != 0x7f)
					syslog(LOG_INFO, "data missmatch: byte %d 0x%2.2x", i, buf[i]);
			}

			total += len;
		}
		gettimeofday(&tv_end, NULL);

		timersub(&tv_end, &tv_beg, &tv_diff);

		syslog(LOG_INFO,"%s%ld bytes in %.2f sec, %.2f kB/s", ts, total,
			tv2fl(tv_diff), (float)(total / tv2fl(tv_diff) ) / 1024.0);
	}
}

 void do_send(int sk)
{
	uint32_t seq;
	int i, fd, len, buflen, size, sent;

	syslog(LOG_INFO, "Sending ...");

	if (data_size < 0)
		data_size = omtu;

	if (filename) {
		fd = open(filename, O_RDONLY);
		if (fd < 0) {
			syslog(LOG_ERR, "Open failed: %s (%d)",
							strerror(errno), errno);
			exit(1);
		}

		sent = 0;
		size = read(fd, buf, data_size);
		while (size > 0) {
			buflen = (size > omtu) ? omtu : size;

			len = send(sk, buf + sent, buflen, 0);

			sent += len;
			size -= len;
		}
		return;
	} else {
		for (i = 6; i < data_size; i++)
			buf[i] = 0x7f;
	}

	seq = 0;
	while ((num_frames == -1) || (num_frames-- > 0)) {
		bt_put_le32(seq, buf);
		bt_put_le16(data_size, buf + 4);

		seq++;

		sent = 0;
		size = data_size;
		while (size > 0) {
			buflen = (size > omtu) ? omtu : size;

			len = send(sk, buf, buflen, 0);
			if (len < 0 || len != buflen) {
				syslog(LOG_ERR, "Send failed: %s (%d)",
							strerror(errno), errno);
				exit(1);
			}

			sent += len;
			size -= len;
		}

		if (num_frames && send_delay && count && !(seq % count))
			usleep(send_delay);
	}
}

 void send_mode(int sk)
{
	do_send(sk);

	syslog(LOG_INFO, "Closing channel ...");
	if (shutdown(sk, SHUT_RDWR) < 0)
		syslog(LOG_INFO, "Close failed: %m");
	else
		syslog(LOG_INFO, "Done");
}

 void senddump_mode(int sk)
{
	do_send(sk);

	dump_mode(sk);
}

 void send_and_recv_mode(int sk)
{
	int flags;

	if ((flags = fcntl(sk, F_GETFL, 0)) < 0)
		flags = 0;
	fcntl(sk, F_SETFL, flags | O_NONBLOCK);

	/* fork for duplex channel */
	if (fork())
		send_mode(sk);
	else
		recv_mode(sk);
	return;
}

 void reconnect_mode(char *svr)
{
	while (1) {
		int sk = do_connect(svr);
		close(sk);
	}
}

 void connect_mode(char *svr)
{
	struct pollfd p;
	int sk;

	if ((sk = do_connect(svr)) < 0)
		exit(1);

	p.fd = sk;
	p.events = POLLERR | POLLHUP;

	while (1) {
		p.revents = 0;
		if (poll(&p, 1, 500))
			break;
	}

	syslog(LOG_INFO, "Disconnected");

	close(sk);
}

 void multi_connect_mode(int argc, char *argv[])
{
	int i, n, sk;

	while (1) {
		for (n = 0; n < argc; n++) {
			for (i = 0; i < count; i++) {
				if (fork())
					continue;

				/* Child */
				sk = do_connect(argv[n]);
				usleep(500);
				close(sk);
				exit(0);
			}
		}
		sleep(4);
	}
}

 void info_request(char *svr)
{
	unsigned char buf[48];
	l2cap_cmd_hdr *cmd = (l2cap_cmd_hdr *) buf;
	l2cap_info_req *req = (l2cap_info_req *) (buf + L2CAP_CMD_HDR_SIZE);
	l2cap_info_rsp *rsp = (l2cap_info_rsp *) (buf + L2CAP_CMD_HDR_SIZE);
	uint16_t mtu;
	uint32_t channels, mask = 0x0000;
	struct sockaddr_l2 addr;
	int sk, err;

	sk = socket(PF_BLUETOOTH, SOCK_RAW, BTPROTO_L2CAP);
	if (sk < 0) {
		perror("Can't create socket");
		return;
	}

	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, &bdaddr);
	addr.l2_bdaddr_type = bdaddr_type;

	if (bind(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("Can't bind socket");
		goto failed;
	}

	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	str2ba(svr, &addr.l2_bdaddr);
	addr.l2_bdaddr_type = bdaddr_type;

	if (connect(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0 ) {
		perror("Can't connect socket");
		goto failed;
	}

	memset(buf, 0, sizeof(buf));
	cmd->code  = L2CAP_INFO_REQ;
	cmd->ident = 141;
	cmd->len   = htobs(2);
	req->type  = htobs(0x0001);

	if (send(sk, buf, L2CAP_CMD_HDR_SIZE + L2CAP_INFO_REQ_SIZE, 0) < 0) {
		perror("Can't send info request");
		goto failed;
	}

	err = recv(sk, buf, L2CAP_CMD_HDR_SIZE + L2CAP_INFO_RSP_SIZE + 2, 0);
	if (err < 0) {
		perror("Can't receive info response");
		goto failed;
	}

	switch (btohs(rsp->result)) {
	case 0x0000:
		memcpy(&mtu, rsp->data, sizeof(mtu));
		printf("Connectionless MTU size is %d\n", btohs(mtu));
		break;
	case 0x0001:
		printf("Connectionless MTU is not supported\n");
		break;
	}

	memset(buf, 0, sizeof(buf));
	cmd->code  = L2CAP_INFO_REQ;
	cmd->ident = 142;
	cmd->len   = htobs(2);
	req->type  = htobs(0x0002);

	if (send(sk, buf, L2CAP_CMD_HDR_SIZE + L2CAP_INFO_REQ_SIZE, 0) < 0) {
		perror("Can't send info request");
		goto failed;
	}

	err = recv(sk, buf, L2CAP_CMD_HDR_SIZE + L2CAP_INFO_RSP_SIZE + 4, 0);
	if (err < 0) {
		perror("Can't receive info response");
		goto failed;
	}

	switch (btohs(rsp->result)) {
	case 0x0000:
		memcpy(&mask, rsp->data, sizeof(mask));
		printf("Extended feature mask is 0x%04x\n", btohl(mask));
		if (mask & L2CAP_FEAT_FLOWCTL)
			printf("  Flow control mode\n");
		if (mask & L2CAP_FEAT_RETRANS)
			printf("  Retransmission mode\n");
		if (mask & L2CAP_FEAT_BIDIR_QOS)
			printf("  Bi-directional QoS\n");
		if (mask & L2CAP_FEAT_ERTM)
			printf("  Enhanced Retransmission mode\n");
		if (mask & L2CAP_FEAT_STREAMING)
			printf("  Streaming mode\n");
		if (mask & L2CAP_FEAT_FCS)
			printf("  FCS Option\n");
		if (mask & L2CAP_FEAT_EXT_FLOW)
			printf("  Extended Flow Specification\n");
		if (mask & L2CAP_FEAT_FIXED_CHAN)
			printf("  Fixed Channels\n");
		if (mask & L2CAP_FEAT_EXT_WINDOW)
			printf("  Extended Window Size\n");
		if (mask & L2CAP_FEAT_UCD)
			printf("  Unicast Connectionless Data Reception\n");
		break;
	case 0x0001:
		printf("Extended feature mask is not supported\n");
		break;
	}

	if (!(mask & 0x80))
		goto failed;

	memset(buf, 0, sizeof(buf));
	cmd->code  = L2CAP_INFO_REQ;
	cmd->ident = 143;
	cmd->len   = htobs(2);
	req->type  = htobs(0x0003);

	if (send(sk, buf, L2CAP_CMD_HDR_SIZE + L2CAP_INFO_REQ_SIZE, 0) < 0) {
		perror("Can't send info request");
		goto failed;
	}

	err = recv(sk, buf, L2CAP_CMD_HDR_SIZE + L2CAP_INFO_RSP_SIZE + 8, 0);
	if (err < 0) {
		perror("Can't receive info response");
		goto failed;
	}

	switch (btohs(rsp->result)) {
	case 0x0000:
		memcpy(&channels, rsp->data, sizeof(channels));
		printf("Fixed channels list is 0x%04x\n", btohl(channels));
		break;
	case 0x0001:
		printf("Fixed channels list is not supported\n");
		break;
	}

failed:
	close(sk);
}

 void do_pairing(char *svr)
{
	struct sockaddr_l2 addr;
	int sk, opt;

	sk = socket(PF_BLUETOOTH, SOCK_RAW, BTPROTO_L2CAP);
	if (sk < 0) {
		perror("Can't create socket");
		return;
	}

	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, &bdaddr);
	addr.l2_bdaddr_type = bdaddr_type;

	if (bind(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("Can't bind socket");
		goto failed;
	}

	if (secure)
		opt = L2CAP_LM_SECURE;
	else
		opt = L2CAP_LM_ENCRYPT;

	if (setsockopt(sk, SOL_L2CAP, L2CAP_LM, &opt, sizeof(opt)) < 0) {
		perror("Can't set link mode");
		goto failed;
	}

	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	str2ba(svr, &addr.l2_bdaddr);
	addr.l2_bdaddr_type = bdaddr_type;

	if (connect(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0 ) {
		perror("Can't connect socket");
		goto failed;
	}

	printf("Pairing successful\n");

failed:
	close(sk);
}

