


#include <libcrt.h>

#define MAXPATHLEN 256

#ifdef O_NOFOLLOW
  #define OPEN_FLAGS O_NOFOLLOW | O_NONBLOCK
#else
  #define OPEN_FLAGS O_NONBLOCK
#endif

int libvbt_test(struct libvbt_t *st, unsigned int p)
{
    int ret = 0;
    char buffer[256];
    libvbt_update(st, p);
    //printf("func = [%s] addr:%s, name:%s\n", __FUNCTION__, st->addr, st->name );
    libvbt_init(st);
    //printf("+modify microphone library .%d \n", p);
    if(libvbt_read(st, buffer, 256)){ ret = 0; }
    libvbt_close(st);
    //printf("-modify microphone library .%d \n", p);
    return ret;
}

int libvbt_micp(struct libvbt_t *st, unsigned int list)
{
    int mic = 0;
    int i;

    //printf("check microphone library .\n");
    for(i=1;i<=st->count;i++){
        if(libvbt_test(st,i)){
            mic = i;
            break;
        }  
    }
    //printf("final microphone library .\n");
    return mic;
}

int libvbt_update(struct libvbt_t *st, unsigned int p)
{
    struct libvbt_list_t  *prev=NULL,*next=NULL;
    int i;
    next = st->list;
    for(i=0;i<st->count;i++){
        //printf("func = [%s] addr:%s, name:%s\n", __FUNCTION__, next->addr, next->name );
        if(p-1 == i){ 
            strcpy(st->name, next->name);
            strcpy(st->addr, next->addr);
            strcpy(st->device, next->device);
            strcpy(st->uclass, next->uclass);
            strcpy(st->udesc, next->udesc);

            bacpy(&st->bdaddr, &next->bdaddr);

            st->dev_id = next->dev_id;
            //str2ba( st->addr, &st->address.rc_bdaddr );
            break;
        }
        prev = next->next;
        next = prev;
    }
    //printf("update = [%s] addr:%s, name:%s\n", __FUNCTION__, st->addr, st->name );
    return 0;
}

int libvbt_info(struct libvbt_t *st)
{
    //char addr[18] = { 0 };
    //char name[248] = { 0 };
    printf("Addr:%s    Name:%s\n", st->addr, st->name);
    //ba2str(&st->address.rc_bdaddr ,addr );
}

int libvbt_release_dev(int ctl, int dev, uint32_t flags)
{
	struct rfcomm_dev_req req;
	int err;

	memset(&req, 0, sizeof(req));
	req.dev_id = dev;

	err = ioctl(ctl, RFCOMMRELEASEDEV, &req);
	if (err < 0){
		perror("Can't release device");
    }
	return err;
}

int libvbt_release_all(int ctl)
{
	struct rfcomm_dev_list_req *dl;
	struct rfcomm_dev_info *di;
	int i;

	dl = malloc(sizeof(*dl) + RFCOMM_MAX_DEV * sizeof(*di));
	if (!dl) {
		perror("Can't allocate memory");
		//exit(1);
        goto END;
	}

	dl->dev_num = RFCOMM_MAX_DEV;
	di = dl->dev_info;

	if (ioctl(ctl, RFCOMMGETDEVLIST, (void *) dl) < 0) {
		perror("Can't get device list");
		free(dl);
		//exit(1);
        goto END;
	}

	for (i = 0; i < dl->dev_num; i++)
		libvbt_release_dev(ctl, (di + i)->id, 0);
    
    END:
	free(dl);
	return 0;
}


int libvbt_list(struct libvbt_t *st)
{
    int num = 0;
    num = libvbt_select(st,-1);
    //libvbt_close(st);
    return num;
}

int libvbt_init(struct libvbt_t *st)
{
    struct sockaddr_rc laddr, raddr;
	struct rfcomm_dev_req req;
    socklen_t len;
    char      devname[MAXPATHLEN];
    int       try = 10;

    const short BLUETOOTH_CHANNEL=1;
 // allocate a socket
    if(st->s == SOCKET_ERROR){
        st->s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
        ///create a socket
        if(st->s == SOCKET_ERROR){
            printf("create socket bluetooth \n");
            goto END;
        }
    }
    memset(&st->address, 0, sizeof(st->address));
    // fd = socket(PF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC | SOCK_NONBLOCK, BTPROTO_HCI);
    //printf("func = [%s] addr:%s, name:%s\n", __FUNCTION__, st->addr, st->name );
    // set the connection parameters (who to connect to)
    st->address.rc_family = AF_BLUETOOTH;
    if(st->address.rc_channel == 0) st->address.rc_channel = (uint8_t) BLUETOOTH_CHANNEL;//(uint8_t) 1;//must use sdp to work in real devices
    //may this channel not ready
    //ba2str(&st->address.rc_bdaddr, st->addr);
    //bacpy(&st->address.rc_bdaddr, &st->bdaddr);    
    str2ba(st->addr, &st->address.rc_bdaddr);
    // connect to server, throw socket s
    st->status = connect(st->s, (struct sockaddr *)&st->address, sizeof(st->address));
    //successful, connect() returns 0.

    printf("connection: %s status: %d socket: %d - message: %s\n", st->addr, st->status, st->s, strerror( errno));//0 show OK
   
    // send a message to server
    //if( status == 0 ) {
        
    //}
    //else 
    if( st->status < 0 ){ 
        printf("send message to server Failed\n");
        goto END;
        
    }
    len = sizeof(laddr);
	if (getsockname(st->s, (struct sockaddr *)&laddr, &len) < 0) {
		perror("Can't get RFCOMM socket name");
		goto END;
	}
    len = sizeof(raddr);
    if (getpeername(st->s, (struct sockaddr *)&raddr, &len) < 0) {
		perror("Can't get RFCOMM socket name");
		goto END;
	}
    
    memset(&req, 0, sizeof(req));
	req.dev_id = st->dev;
	req.flags = (1 << RFCOMM_HANGUP_NOW);
	st->dev = ioctl(st->s, RFCOMMRELEASEDEV, &req);
    if (st->dev < 0) {
		//perror("Can't release RFCOMM TTY");
		//goto END;
	}

	memset(&req, 0, sizeof(req));
	req.dev_id = st->dev;
	req.flags = (1 << RFCOMM_REUSE_DLC) | (1 << RFCOMM_RELEASE_ONHUP) | (1 << RFCOMM_TTY_ATTACHED);

	bacpy(&req.src, &laddr.rc_bdaddr);
	bacpy(&req.dst, &raddr.rc_bdaddr);
	req.channel = raddr.rc_channel;
    
	st->dev = ioctl(st->s, RFCOMMCREATEDEV, &req);
	if (st->dev < 0) {
		perror("Can't create RFCOMM TTY");
		goto END;
	}
    if(fchmod(st->s, S_IXUSR | S_IRUSR | S_IWUSR )!=0){
        fatal("chmod file ", __FILE__, __FUNCTION__, __LINE__);
        
    }

    
    //int on = 0;
    //printf("%x \n", ioctl (st->s, FIONBIO, &on));

	snprintf(devname, MAXPATHLEN - 1, "/dev/rfcomm%d", st->dev);
    //printf("bluetooth: %s \n", devname);
    
    //if (chmod (devname,0777) < 0)
    //    perror("error in chmod");

    while ((st->fd = open(devname, OPEN_FLAGS | O_RDWR | O_NOCTTY ,  S_IXUSR | S_IRUSR | S_IWUSR )) < 0) {
		if (errno == EACCES) {
			perror("Can't open RFCOMM device global");
			goto release;
		}

		snprintf(devname, MAXPATHLEN - 1, "/dev/bluetooth/rfcomm/%d", st->dev);
		if ((st->fd = open(devname, OPEN_FLAGS | O_RDWR | O_NOCTTY , S_IXUSR | S_IRUSR | S_IWUSR )) < 0) {
			if (try--) {
				snprintf(devname, MAXPATHLEN - 1, "/dev/rfcomm%d", st->dev);
				usleep(100 * 1000);
				continue;
			}
			perror("Can't open RFCOMM device local");
			goto release;
		}
	}

    release:

    if(st->fd == -1){
        memset(&req, 0, sizeof(req));
	    req.dev_id = st->dev;
	    req.flags = (1 << RFCOMM_HANGUP_NOW);
	    st->dev = ioctl(st->s, RFCOMMRELEASEDEV, &req);
        if (st->dev < 0) {
		    perror("Can't release RFCOMM TTY");
		//goto END;
	    }
        goto END;
    }else{
        st->handle = 1;
    }
    return 1;
    END:
    close(st->s); st->s= -1;
    //printf("Closing socket\n");
    return 0;
}

int libvbt_select(struct libvbt_t *st,int fx)
{
    int flag=0;

    //struct sockaddr_rc addrress = { 0 };
    struct hci_conn_list_req *cl=NULL;
    struct hci_conn_info *ci=NULL;
    struct libvbt_list_t  *prev=NULL,*next=NULL;
    struct libvbt_list_t  *upd;

    char dest[18]="";// = "B0:10:41:3F:6E:80";//My destination address Laptop
    char namelaptop[20]="ss";

    //printf("Search for BT Devices...\n");

    st->count = 0;
///search   

    int max_rsp, num_rsp;
    int dev_id, len, flags;
    int i;
    struct hci_conn_info_req *cr=NULL;
    struct hci_dev_info di;
    struct hci_version version;

    dev_id = hci_get_route(NULL);
    st->sock = hci_open_dev( dev_id );
    if (dev_id < 0 || st->sock < 0) {
        perror("opening socket");
        //exit(1);
        goto END;
    }

    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    st->ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    if(st->ii==NULL){
        perror("inquiry_info");
        goto END;
    }   
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &st->ii, flags);
    if( num_rsp < 0 ) {
        perror("hci_inquiry");
        goto END;
    }
    if (hci_devinfo(dev_id, &di) < 0) {
		perror("Can't get device info");
		//exit(1);
        goto END;
	}

    cr = malloc(sizeof(*cr) + sizeof(struct hci_conn_info));
    if (!cr) {
        perror("Can't allocate memory");
        goto END;
        //    exit(1);
    }

    //printf("Show list devices %d .\n", num_rsp);

    if (!(cl = malloc(10 * sizeof(*ci) + sizeof(*cl)))) {
        perror("Can't allocate memory");
        goto END;
        //exit(1);
    }
    cl->dev_id = dev_id;
    cl->conn_num = 10;
    ci = cr; //cl->conn_info;

    if (ioctl(st->sock, HCIGETCONNLIST, (void *) cl)) {
        perror("Can't get connection list");
        goto END;
        //exit(1);
    }

    for (i = 0; i < cl->conn_num; i++, ci++){
        //if (!bacmp((bdaddr_t *) arg, &ci->bdaddr)) {
        ba2str(&ci->bdaddr, st->addr);
        printf("address: %s \n", st->addr);    
            
    }
    st->dev = dev_id;
    
    //if (hci_write_local_name(st->sock, "lexne", 2000) < 0) {
	//		printf("Can't change local name on hci%d: %s (%d)\n",
	//					dev_id, strerror(errno), errno);
			//exit(1);
	//	}

    for (i = 0; i < num_rsp; i++) {
        
        //libvbt_release_all(st->sock);
        // &(st->ii+i)->bdaddr;
        ba2str(&(st->ii+i)->bdaddr, st->addr);
        memset(st->name, 0, sizeof(st->name));
        memset(st->device, 0, sizeof(st->device));
        if (hci_read_remote_name(st->sock, &(st->ii+i)->bdaddr, sizeof(st->name),
            st->name, 0) < 0){
            strcpy(st->name, "[unknown]");
        
        }else{
            printf("\nFind #%d\n",i);
        }
        
        if (hci_read_remote_name_with_clock_offset(st->sock,
				&(st->ii+i)->bdaddr,
				(st->ii+i)->pscan_rep_mode,
				(st->ii+i)->clock_offset | 0x8000,
				sizeof(st->device), st->device, 100000) < 0) {
			
				strcpy(st->device, "n/a");
			}
        bacpy(&st->bdaddr, &(st->ii+i)->bdaddr);
        //bacpy(&cr->bdaddr, &(st->ii+i)->bdaddr);
        //cr->type = ACL_LINK;
        //if (ioctl(st->sock, HCIGETCONNINFO, (unsigned long) cr) < 0) {
        //    perror("Get connection info failed");
            //exit(1);
        //}else{
            //if (hci_create_connection(st->sock, &(st->ii+i)->bdaddr,
			//		htobs(di.pkt_type & ACL_PTYPE_MASK),
			//		0, 0x01, &cr->conn_info->handle, 25000) < 0) {
			//    perror("Can't create connection");
			    //close(dd);
			    //exit(1);
		    //}
        //}
        //if (hci_read_remote_version(st->sock, cr->conn_info->handle, &version, 20000) == 0) {
		//		char *ver = lmp_vertostr(version.lmp_ver);
		//		printf("Manufacturer:\t%s (%d)\n",
		//			bt_compidtostr(version.manufacturer),
		//			version.manufacturer);
		//		printf("LMP version:\t%s (0x%x) [subver 0x%x]\n",
		//			ver ? ver : "n/a",
		//			version.lmp_ver, version.lmp_subver);
		//		if (ver)
		//			bt_free(ver);
		//}
        
        //if (hci_authenticate_link(st->sock, htobs(cr->conn_info->handle), 25000) < 0) {
        //    perror("HCI authentication request failed");
            //exit(1);
        //}

        printf("Device: %s    Addr:%s    Name:%s\n", st->device, st->addr, st->name);

        upd = (struct libvbt_list_t *) malloc( sizeof(struct libvbt_list_t) );
        if(upd == NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;         
        }
        upd->next = NULL;
        upd->dev_id = st->dev;
        strcpy(upd->addr, st->addr);
        strcpy(upd->name, st->name);
        strcpy(upd->device, st->device);

        bacpy(&upd->bdaddr, &(st->ii+i)->bdaddr);
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        next = st->list;   
        if(!next){
            next = upd;
            next->next=NULL;        
        }else{
            prev = next;
            while(1){
               if(prev->next==NULL){
                    prev->next = upd;
                    break; 
               }
               prev = prev->next; 
            }

        }

        st->list = next;
        st->count += 1;
        
        int a=strcmp(st->name, namelaptop);
        //printf("compare: - %d\n",a);

        if (a!=0){   // if name mached 
            str2ba( st->addr, &st->address.rc_bdaddr );//copy dest-->addr.rc_bdaddr    
            flag =1;
        }
        
        if(i == fx) break;

    }

  END:
    if(cl!=NULL) free(cl),cl=NULL;
/// End Search 
    if(cr!=NULL) free(cr),cr=NULL;


///Connect and send

    if (flag==0){
        printf("Not find dest: %s\n",st->name);
        //exit(0);
        return 0;
    }
    return num_rsp;
}

int libvbt_write(struct libvbt_t *st, char *buffer, unsigned int len)
{
    if(st->status < 0){
        printf("socket connect status %x \n", st->status);
        goto END;
    }
    if(st->handle < 0){ 
        if(st->s == -1){
            printf("socket connect error %x %x\n", st->s, st->fd);
            //if(st->fd != -1) goto FDS;
            return 0;
        }
    }else{
        if(st->fd == -1){
            printf("file connect error %x %x\n", st->fd, st->s);
            //if(st->s != -1) goto SCK;
            return 0;
        }
    }
    if(st->handle < 0){
        //SCK:
        st->status = write(st->s, "hello!", 6);
    }else{
        //FDS:
        st->status = write(st->fd, "hello!", 6);
    }
    if (st->status == 6){
        printf("Send data to server done\n");
    }
    END:
    return 0;
}

int libvbt_read(struct libvbt_t *st, char *buffer, unsigned int len)
{
    if(st->status < 0){
        printf("socket connect status %x \n", st->status);
        goto END;
    }
    if(st->handle < 0){ 
        if(st->s == -1){
            printf("socket connect error %x %x\n", st->s, st->fd);

            //if(st->fd != -1) goto FDS;
            return 0;
        }
    }else{
        if(st->fd == -1){
            printf("file connect error %x %x\n", st->fd, st->s);

            //if(st->s != -1) goto SCK;
            return 0;
        }
    }
    printf("+recv data to server done %x\n", st->s);
    if(st->handle < 0){    
        //SCK:        
        st->status = read(st->s, buffer, len);
    }else{
        //FDS:
        st->status = read(st->fd, buffer, len);    
    }
    printf("-recv data to server done %x\n", st->s);
    if (st->status == 6){
        printf("#recv data to server done\n");
        return 0;
    }
    printf("$recv data to server done\n");
    END:
    return len;
}

int libvbt_callback(struct libvbt_t *st, callback_handle_t cb)
{
    char buffer[MGC_SEEK1];

    while(libvbt_state){
        libvbt_read(st, buffer, MGC_SEEK1);
        if(cb!=NULL) cb(buffer, MGC_SEEK1);
    }
    return 0;
}

int libvbt_close(struct libvbt_t *st)
{

    ///close the socket
    if(st->s > 0) close(st->s), st->s=-1;
    if(st->fd > 0) close(st->s), st->s=-1;
///End connect and send


    if(st->ii  !=NULL) free( st->ii ), st->ii=NULL;
    if(st->sock > 0) close( st->sock ), st->sock = -1;

    return 0;
}





