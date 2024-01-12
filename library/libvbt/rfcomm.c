#include <libcrt.h>

#define MAXPATHLEN 256

static int rfcomm_raw_tty = 1;
static int auth = 1;
static int encryption = 1;
static int secure = 1;
static int master = 1;
static int linger = 1;

static char *rfcomm_state[] = {
	"unknown",
	"connected",
	"clean",
	"bound",
	"listening",
	"connecting",
	"connecting",
	"config",
	"disconnecting",
	"closed"
};

static volatile sig_atomic_t __io_canceled = 0;

static void sig_hup(int sig)
{
	return;
}

void sig_term(int sig)
{
	__io_canceled = 1;
}

char *rfcomm_flagstostr(uint32_t flags)
{
	static char str[100];
	str[0] = 0;

	strcat(str, "[");

	if (flags & (1 << RFCOMM_REUSE_DLC))
		strcat(str, "reuse-dlc ");

	if (flags & (1 << RFCOMM_RELEASE_ONHUP))
		strcat(str, "release-on-hup ");

	if (flags & (1 << RFCOMM_TTY_ATTACHED))
		strcat(str, "tty-attached");

	strcat(str, "]");
	return str;
}


int create_dev(int ctl, int dev, uint32_t flags, bdaddr_t *bdaddr, struct libvbt_t *st)
{
	struct rfcomm_dev_req req;
	int err;

	memset(&req, 0, sizeof(req));
	req.dev_id = dev;
	req.flags = flags;
	bacpy(&req.src, bdaddr);

	//if (argc < 2) {
	//	fprintf(stderr, "Missing dev parameter");
	//	return -EINVAL;
	//}

	str2ba(st->addr, &req.dst);

	if (st->channel > 2)
		req.channel = st->channel;
	else
		req.channel = 1;

	err = ioctl(ctl, RFCOMMCREATEDEV, &req);
	if (err == -1) {
		err = -errno;

		if (err == -EOPNOTSUPP)
			fprintf(stderr, "RFCOMM TTY support not available\n");
		else
			perror("Can't create device");
	}

	return err;
}

int release_dev(int ctl, int dev, uint32_t flags)
{
	struct rfcomm_dev_req req;
	int err;

	memset(&req, 0, sizeof(req));
	req.dev_id = dev;

	err = ioctl(ctl, RFCOMMRELEASEDEV, &req);
	if (err < 0)
		perror("Can't release device");

	return err;
}

int release_all(int ctl)
{
	struct rfcomm_dev_list_req *dl;
	struct rfcomm_dev_info *di;
	int i;

	dl = malloc(sizeof(*dl) + RFCOMM_MAX_DEV * sizeof(*di));
	if (!dl) {
		perror("Can't allocate memory");
		exit(1);
	}

	dl->dev_num = RFCOMM_MAX_DEV;
	di = dl->dev_info;

	if (ioctl(ctl, RFCOMMGETDEVLIST, (void *) dl) < 0) {
		perror("Can't get device list");
		free(dl);
		exit(1);
	}

	for (i = 0; i < dl->dev_num; i++)
		release_dev(ctl, (di + i)->id, 0);

	free(dl);
	return 0;
}

void cmd_create(int ctl, int dev, bdaddr_t *bdaddr, struct libvbt_t *st)
{
	create_dev(ctl, dev, 0, bdaddr, st);
}

void cmd_release(int ctl, int dev, bdaddr_t *bdaddr, struct libvbt_t *st)
{
	if (st->count > 1)
		release_all(ctl);
	else
		release_dev(ctl, dev, 0);
}

void cmd_listen(int ctl, int dev, bdaddr_t *bdaddr, struct libvbt_t *st)
{
	struct sockaddr_rc laddr, raddr;
	struct rfcomm_dev_req req;
	struct termios ti;
	struct sigaction sa;
	struct pollfd p;
	sigset_t sigs;
	socklen_t alen;
	char dst[18], devname[MAXPATHLEN];
	int sk, nsk, fd, lm, try = 30;
    char addr[MAXPATHLEN];

    if(bdaddr == NULL) bdaddr = BDADDR_ANY;

	laddr.rc_family = AF_BLUETOOTH;
	bacpy(&laddr.rc_bdaddr, bdaddr);
	laddr.rc_channel = (st->channel < 2) ? 1 : st->channel;

	sk = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if (sk < 0) {
		perror("Can't create RFCOMM socket");
		return;
	}

    alen = sizeof(laddr);
	if (getsockname(sk, (struct sockaddr *)&laddr, &alen) < 0) {
		perror("Can't get RFCOMM socket name");
		close(sk);
		return;
	}
    laddr.rc_family = AF_BLUETOOTH;
	//bacpy(&laddr.rc_bdaddr, bdaddr);
	laddr.rc_channel = (st->channel < 2) ? 1 : st->channel;

	lm = 0;
	if (master)
		lm |= RFCOMM_LM_MASTER;
	if (auth)
		lm |= RFCOMM_LM_AUTH;
	if (encryption)
		lm |= RFCOMM_LM_ENCRYPT;
	if (secure)
		lm |= RFCOMM_LM_SECURE;

	if (lm && setsockopt(sk, SOL_RFCOMM, RFCOMM_LM, &lm, sizeof(lm)) < 0) {
		perror("Can't set RFCOMM link mode");
		close(sk);
		return;
	}

	if (bind(sk, (struct sockaddr *)&laddr, sizeof(laddr)) < 0) {
		perror("Can't bind RFCOMM socket");
		close(sk);
		return;
	}

	printf("Waiting for connection on channel %d\n", laddr.rc_channel);

	listen(sk, 10);
    
    ba2str(&laddr.rc_bdaddr, addr);
    printf("Server initialized for address %s\n", addr);

	alen = sizeof(raddr);
	nsk = accept(sk, (struct sockaddr *) &raddr, &alen);

	alen = sizeof(laddr);
	if (getsockname(nsk, (struct sockaddr *)&laddr, &alen) < 0) {
		perror("Can't get RFCOMM socket name");
		close(nsk);
		return;
	}

	if (linger) {
		struct linger l = { .l_onoff = 1, .l_linger = linger };

		if (setsockopt(nsk, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0) {
			perror("Can't set linger option");
			close(nsk);
			return;
		}
	}

	memset(&req, 0, sizeof(req));
	req.dev_id = dev;
	req.flags = (1 << RFCOMM_REUSE_DLC) | (1 << RFCOMM_RELEASE_ONHUP);

	bacpy(&req.src, &laddr.rc_bdaddr);
	bacpy(&req.dst, &raddr.rc_bdaddr);
	req.channel = raddr.rc_channel;

	dev = ioctl(nsk, RFCOMMCREATEDEV, &req);
	if (dev < 0) {
		perror("Can't create RFCOMM TTY");
		close(sk);
		return;
	}


	snprintf(devname, MAXPATHLEN - 1, "/dev/rfcomm%d", dev);
	while ((fd = open(devname, O_RDONLY | O_NOCTTY)) < 0) {
		if (errno == EACCES) {
			perror("Can't open RFCOMM device");
			goto release;
		}

		snprintf(devname, MAXPATHLEN - 1, "/dev/bluetooth/rfcomm/%d", dev);
		if ((fd = open(devname, O_RDONLY | O_NOCTTY)) < 0) {
			if (try--) {
				snprintf(devname, MAXPATHLEN - 1, "/dev/rfcomm%d", dev);
				usleep(100 * 1000);
				continue;
			}
			perror("Can't open RFCOMM device");
			goto release;
		}
	}

	if (rfcomm_raw_tty) {
		tcflush(fd, TCIOFLUSH);

		cfmakeraw(&ti);
		tcsetattr(fd, TCSANOW, &ti);
	}

	close(sk);
	close(nsk);

	ba2str(&req.dst, dst);
	printf("Connection from %s to %s\n", dst, devname);
	printf("Press CTRL-C for hangup\n");

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags   = SA_NOCLDSTOP;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGCHLD, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);

	sa.sa_handler = sig_term;
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT,  &sa, NULL);

	sa.sa_handler = sig_hup;
	sigaction(SIGHUP, &sa, NULL);

	sigfillset(&sigs);
	sigdelset(&sigs, SIGCHLD);
	sigdelset(&sigs, SIGPIPE);
	sigdelset(&sigs, SIGTERM);
	sigdelset(&sigs, SIGINT);
	sigdelset(&sigs, SIGHUP);

	p.fd = fd;
	p.events = POLLERR | POLLHUP;

	//if (argc <= 2) {
	//	while (!__io_canceled) {
	//		p.revents = 0;
	//		if (ppoll(&p, 1, NULL, &sigs) > 0)
	//			break;
	//	}
	//} //else
	//	run_cmdline(&p, &sigs, devname, argc - 2, argv + 2);

	sa.sa_handler = NULL;
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT,  &sa, NULL);

	printf("Disconnected\n");

	close(fd);
	return;

release:
	memset(&req, 0, sizeof(req));
	req.dev_id = dev;
	req.flags = (1 << RFCOMM_HANGUP_NOW);
	ioctl(ctl, RFCOMMRELEASEDEV, &req);

	close(sk);
}

void cmd_connect(int ctl, int dev, bdaddr_t *bdaddr, struct libvbt_t *st)
{
	struct sockaddr_rc laddr, raddr;
	struct rfcomm_dev_req req;
	struct termios ti;
	struct sigaction sa;
	struct pollfd p;
	sigset_t sigs;
	socklen_t alen;
	char dst[18], devname[MAXPATHLEN];
	int sk, fd, try = 30;
    char host[MAXPATHLEN],conn[MAXPATHLEN];
    
    memset(&raddr, 0, sizeof(raddr));
    memset(&laddr, 0, sizeof(laddr));

	laddr.rc_family = AF_BLUETOOTH;
	bacpy(&laddr.rc_bdaddr, bdaddr);
	laddr.rc_channel = 0;

	//if (argc < 2) {
	//	fprintf(stderr, "Missing dev parameter");
	//	return;
	//}

	raddr.rc_family = AF_BLUETOOTH;
	str2ba(st->addr, &raddr.rc_bdaddr);
    

    ba2str(&laddr.rc_bdaddr, host);
    ba2str(&raddr.rc_bdaddr, conn);


    printf("host [%s] conn [%s] \n", host, conn);

	if (st->channel > 2)
		raddr.rc_channel = st->channel;
	else
		raddr.rc_channel = 1;

	sk = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if (sk < 0) {
		perror("Can't create RFCOMM socket");
		return;
	}


	if (linger) {
		struct linger l = { .l_onoff = 1, .l_linger = linger };

		if (setsockopt(sk, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0) {
			perror("Can't set linger option");
			return;
		}
	}

	if (bind(sk, (struct sockaddr *) &laddr, sizeof(laddr)) < 0) {
		perror("Can't bind RFCOMM socket");
		close(sk);
		return;
	}

	if (connect(sk, (struct sockaddr *) &raddr, sizeof(raddr)) < 0) {
		perror("Can't connect RFCOMM socket");
		close(sk);
		return;
	}

	alen = sizeof(laddr);
	if (getsockname(sk, (struct sockaddr *)&laddr, &alen) < 0) {
		perror("Can't get RFCOMM socket name");
		close(sk);
		return;
	}

	memset(&req, 0, sizeof(req));
	req.dev_id = dev;
	req.flags = (1 << RFCOMM_REUSE_DLC) | (1 << RFCOMM_RELEASE_ONHUP);

	bacpy(&req.src, &laddr.rc_bdaddr);
	bacpy(&req.dst, &raddr.rc_bdaddr);
	req.channel = raddr.rc_channel;

	dev = ioctl(sk, RFCOMMCREATEDEV, &req);
	if (dev < 0) {
		perror("Can't create RFCOMM TTY");
		close(sk);
		return;
	}

	snprintf(devname, MAXPATHLEN - 1, "/dev/rfcomm%d", dev);
	while ((fd = open(devname, O_RDONLY | O_NOCTTY)) < 0) {
		if (errno == EACCES) {
			perror("Can't open RFCOMM device");
			goto release;
		}

		snprintf(devname, MAXPATHLEN - 1, "/dev/bluetooth/rfcomm/%d", dev);
		if ((fd = open(devname, O_RDONLY | O_NOCTTY)) < 0) {
			if (try--) {
				snprintf(devname, MAXPATHLEN - 1, "/dev/rfcomm%d", dev);
				usleep(100 * 1000);
				continue;
			}
			perror("Can't open RFCOMM device");
			goto release;
		}
	}

	if (rfcomm_raw_tty) {
		tcflush(fd, TCIOFLUSH);

		cfmakeraw(&ti);
		tcsetattr(fd, TCSANOW, &ti);
	}

	close(sk);

	ba2str(&req.dst, dst);
	printf("Connected %s to %s on channel %d\n", devname, dst, req.channel);
	printf("Press CTRL-C for hangup\n");

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags   = SA_NOCLDSTOP;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGCHLD, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);

	sa.sa_handler = sig_term;
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT,  &sa, NULL);

	sa.sa_handler = sig_hup;
	sigaction(SIGHUP, &sa, NULL);

	sigfillset(&sigs);
	sigdelset(&sigs, SIGCHLD);
	sigdelset(&sigs, SIGPIPE);
	sigdelset(&sigs, SIGTERM);
	sigdelset(&sigs, SIGINT);
	sigdelset(&sigs, SIGHUP);

	p.fd = fd;
	p.events = POLLERR | POLLHUP;

	while (!__io_canceled) {
		p.revents = 0;
		if (ppoll(&p, 1, NULL, &sigs) > 0)
			break;
	}

	printf("Disconnected\n");

	close(fd);
	return;

release:
	memset(&req, 0, sizeof(req));
	req.dev_id = dev;
	req.flags = (1 << RFCOMM_HANGUP_NOW);
	ioctl(ctl, RFCOMMRELEASEDEV, &req);

	close(sk);
}

void print_dev_info(struct rfcomm_dev_info *di, struct libvbt_t *st)
{
	char src[18], dst[18], addr[40];

	ba2str(&di->src, src); ba2str(&di->dst, dst);

	if (bacmp(&di->src, BDADDR_ANY) == 0)
		sprintf(addr, "%s", dst);
	else
		sprintf(addr, "%s -> %s", src, dst);

	printf("rfcomm%d: %s channel %d %s %s\n",
		di->id, addr, di->channel,
		rfcomm_state[di->state],
		di->flags ? rfcomm_flagstostr(di->flags) : "");
}

void print_dev_list(int ctl, int flags, struct libvbt_t *st)
{
	struct rfcomm_dev_list_req *dl;
	struct rfcomm_dev_info *di;
	int i;

	dl = malloc(sizeof(*dl) + RFCOMM_MAX_DEV * sizeof(*di));
	if (!dl) {
		perror("Can't allocate memory");
		exit(1);
	}

	dl->dev_num = RFCOMM_MAX_DEV;
	di = dl->dev_info;

	if (ioctl(ctl, RFCOMMGETDEVLIST, (void *) dl) < 0) {
		perror("Can't get device list");
		free(dl);
		exit(1);
	}
    printf("%s - %d dev number: %d\n", __FUNCTION__, __LINE__, dl->dev_num);
	for (i = 0; i < dl->dev_num; i++){
		print_dev_info(di + i, st);
    }
	free(dl);
}

void cmd_list(struct libvbt_t *st)
{
    int ctl;
    ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_RFCOMM);
	if (ctl < 0) {
		perror("Can't open RFCOMM control socket");
		exit(1);
	}
    printf("%s - %d\n", __FUNCTION__, __LINE__);
    print_dev_list(ctl, 0, st);
	close(ctl);
}

void cmd_init(struct libvbt_t *st)
{
    st->ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_RFCOMM);
	if (st->ctl < 0) {
		perror("Can't open RFCOMM control socket");
		exit(1);
	}
}

void cmd_close(struct libvbt_t *st)
{
    if (st == NULL) {
		perror("Can't open RFCOMM control socket");
		exit(1);
	}
    close(st->ctl);
}

