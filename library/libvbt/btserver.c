
#include <libcrt.h>

int libvbt_smcl(struct libvbt_t *st)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int  client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
    int  lm;
    int linger = 1;
    // allocate socket
    
    st->s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if(st->s == SOCKET_ERROR){
        perror("create socket");
        goto END;
    }

    lm = 0;
	//if (master)
		lm |= RFCOMM_LM_MASTER;
	//if (auth)
		lm |= RFCOMM_LM_AUTH;
	//if (encryption)
	//	lm |= RFCOMM_LM_ENCRYPT;
	//if (secure)
	//	lm |= RFCOMM_LM_SECURE;

	if (lm && setsockopt(st->s, SOL_RFCOMM, RFCOMM_LM, &lm, sizeof(lm)) < 0) {
		perror("Can't set RFCOMM link mode");
		goto END;
	}

    struct linger l = { .l_onoff = 1, .l_linger = linger };

	if (setsockopt(st->s, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0) {
		perror("Can't set linger option");
		goto END;
	}
    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    st->address.rc_family = AF_BLUETOOTH;
    st->address.rc_bdaddr = *BDADDR_ANY;
    st->address.rc_channel = (uint8_t) 6;
    if(bind(st->s, (struct sockaddr *)&st->address, sizeof(st->address)) == SOCKET_ERROR){
        perror("bind socket");
        goto END;
    }
    
    //get local address ?
    //~ ba2str( &loc_addr.rc_bdaddr, buf );
    //~ fprintf(stdout, "local %s\n", buf);

    // put socket into listening mode
    listen(st->s, 1);

    printf("create socket bluetooth \n");
    // accept one connection
    client = accept(st->s, (struct sockaddr *)&rem_addr, &opt);


    ba2str( &rem_addr.rc_bdaddr, buf );
    printf("accepted connection from %s\n", buf);


    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));

    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }else{
        printf("not received [%d]\n", bytes_read);
    }

    return 1;

    END:
    // close connection
    close(client);
    close(st->s);
    return 0;
}

