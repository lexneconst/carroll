#include <libcrt.h>

struct l2cap_data {
	const void *l2cap_data;
	struct mgmt *mgmt;
	uint16_t mgmt_index;
	struct hciemu *hciemu;
	enum hciemu_type hciemu_type;
	unsigned int io_id;
};

struct l2cap_client_data {
	uint16_t client_psm;
	uint16_t server_psm;
	int expect_err;
};

struct l2cap_server_data {
	uint16_t server_psm;
	uint8_t send_req_code;
	const void *send_req;
	uint16_t send_req_len;
	uint8_t expect_rsp_code;
	const void *expect_rsp;
	uint16_t expect_rsp_len;
};

static void mgmt_debug(const char *str, void *user_data)
{
	const char *prefix = user_data;

	l2cap_print("%s%s", prefix, str);
}

static void read_info_callback(uint8_t status, uint16_t length,
					const void *param, void *user_data)
{
	struct l2cap_data *data = l2cap_get_data();
	const struct mgmt_rp_read_info *rp = param;
	char addr[18];
	uint16_t manufacturer;
	uint32_t supported_settings, current_settings;

	l2cap_print("Read Info callback");
	l2cap_print("  Status: 0x%02x", status);

	if (status || !param) {
		l2cap_pre_setup_failed();
		return;
	}

	ba2str(&rp->bdaddr, addr);
	manufacturer = btohs(rp->manufacturer);
	supported_settings = btohl(rp->supported_settings);
	current_settings = btohl(rp->current_settings);

	l2cap_print("  Address: %s", addr);
	l2cap_print("  Version: 0x%02x", rp->version);
	l2cap_print("  Manufacturer: 0x%04x", manufacturer);
	l2cap_print("  Supported settings: 0x%08x", supported_settings);
	l2cap_print("  Current settings: 0x%08x", current_settings);
	l2cap_print("  Class: 0x%02x%02x%02x",
			rp->dev_class[2], rp->dev_class[1], rp->dev_class[0]);
	l2cap_print("  Name: %s", rp->name);
	l2cap_print("  Short name: %s", rp->short_name);

	if (strcmp(hciemu_get_address(data->hciemu), addr)) {
		l2cap_pre_setup_failed();
		return;
	}

	l2cap_pre_setup_complete();
}

static void index_added_callback(uint16_t index, uint16_t length,
					const void *param, void *user_data)
{
	struct l2cap_data *data = l2cap_get_data();

	l2cap_print("Index Added callback");
	l2cap_print("  Index: 0x%04x", index);

	data->mgmt_index = index;

	mgmt_send(data->mgmt, MGMT_OP_READ_INFO, data->mgmt_index, 0, NULL,
					read_info_callback, NULL, NULL);
}

static void index_removed_callback(uint16_t index, uint16_t length,
					const void *param, void *user_data)
{
	struct l2cap_data *data = l2cap_get_data();

	l2cap_print("Index Removed callback");
	l2cap_print("  Index: 0x%04x", index);

	if (index != data->mgmt_index)
		return;

	mgmt_unregister_index(data->mgmt, data->mgmt_index);

	mgmt_unref(data->mgmt);
	data->mgmt = NULL;

	l2cap_post_teardown_complete();
}

static void read_index_list_callback(uint8_t status, uint16_t length,
					const void *param, void *user_data)
{
	struct l2cap_data *data = l2cap_get_data();

	l2cap_print("Read Index List callback");
	l2cap_print("  Status: 0x%02x", status);

	if (status || !param) {
		l2cap_pre_setup_failed();
		return;
	}

	mgmt_register(data->mgmt, MGMT_EV_INDEX_ADDED, MGMT_INDEX_NONE,
					index_added_callback, NULL, NULL);

	mgmt_register(data->mgmt, MGMT_EV_INDEX_REMOVED, MGMT_INDEX_NONE,
					index_removed_callback, NULL, NULL);

	data->hciemu = hciemu_new(data->hciemu_type);
	if (!data->hciemu) {
		l2cap_warn("Failed to setup HCI emulation");
		l2cap_pre_setup_failed();
	}

	l2cap_print("New hciemu instance created");
}

static void l2cap_pre_setup(const void *l2cap_data)
{
	struct l2cap_data *data = l2cap_get_data();

	data->mgmt = mgmt_new_default();
	if (!data->mgmt) {
		l2cap_warn("Failed to setup management interface");
		l2cap_pre_setup_failed();
		return;
	}

	if (l2cap_use_debug())
		mgmt_set_debug(data->mgmt, mgmt_debug, "mgmt: ", NULL);

	mgmt_send(data->mgmt, MGMT_OP_READ_INDEX_LIST, MGMT_INDEX_NONE, 0, NULL,
					read_index_list_callback, NULL, NULL);
}

static void l2cap_post_teardown(const void *l2cap_data)
{
	struct l2cap_data *data = l2cap_get_data();

	if (data->io_id > 0) {
		g_source_remove(data->io_id);
		data->io_id = 0;
	}

	hciemu_unref(data->hciemu);
	data->hciemu = NULL;
}

static void l2cap_data_free(void *l2cap_data)
{
	struct l2cap_data *data = l2cap_data;

	free(data);
}

#define l2cap_bredr(name, data, setup, func) \
	do { \
		struct l2cap_data *user; \
		user = malloc(sizeof(struct l2cap_data)); \
		if (!user) \
			break; \
		user->hciemu_type = HCIEMU_TYPE_BREDR; \
		user->io_id = 0; \
		user->l2cap_data = data; \
		l2cap_add_full(name, data, \
				l2cap_pre_setup, setup, func, NULL, \
				l2cap_post_teardown, 2, user, l2cap_data_free); \
	} while (0)

#define l2cap_l2cap_le(name, data, setup, func) \
	do { \
		struct l2cap_data *user; \
		user = malloc(sizeof(struct l2cap_data)); \
		if (!user) \
			break; \
		user->hciemu_type = HCIEMU_TYPE_LE; \
		user->io_id = 0; \
		user->l2cap_data = data; \
		l2cap_add_full(name, data, \
				l2cap_pre_setup, setup, func, NULL, \
				l2cap_post_teardown, 2, user, l2cap_data_free); \
	} while (0)

static const struct l2cap_client_data client_connect_success_l2cap = {
	.client_psm = 0x1001,
	.server_psm = 0x1001,
};

static const struct l2cap_client_data client_connect_nval_psm_l2cap = {
	.client_psm = 0x1001,
	.expect_err = ECONNREFUSED,
};

static const uint8_t l2cap_connect_req[] = { 0x01, 0x10, 0x41, 0x00 };

static const struct l2cap_server_data l2cap_server_success_l2cap = {
	.server_psm = 0x1001,
	.send_req_code = BT_L2CAP_PDU_CONN_REQ,
	.send_req = l2cap_connect_req,
	.send_req_len = sizeof(l2cap_connect_req),
	.expect_rsp_code = BT_L2CAP_PDU_CONN_RSP,
};

static const uint8_t l2cap_nval_psm_rsp[] = {	0x00, 0x00,	/* dcid */
						0x41, 0x00,	/* scid */
						0x02, 0x00,	/* nval PSM */
						0x00, 0x00	/* status */
					};

static const struct l2cap_server_data l2cap_server_nval_psm_l2cap = {
	.send_req_code = BT_L2CAP_PDU_CONN_REQ,
	.send_req = l2cap_connect_req,
	.send_req_len = sizeof(l2cap_connect_req),
	.expect_rsp_code = BT_L2CAP_PDU_CONN_RSP,
	.expect_rsp = l2cap_nval_psm_rsp,
	.expect_rsp_len = sizeof(l2cap_nval_psm_rsp),
};

static const uint8_t l2cap_nval_conn_req[] = { 0x00 };
static const uint8_t l2cap_nval_pdu_rsp[] = { 0x00, 0x00 };

static const struct l2cap_server_data l2cap_server_nval_pdu_l2cap1 = {
	.send_req_code = BT_L2CAP_PDU_CONN_REQ,
	.send_req = l2cap_nval_conn_req,
	.send_req_len = sizeof(l2cap_nval_conn_req),
	.expect_rsp_code = BT_L2CAP_PDU_CMD_REJECT,
	.expect_rsp = l2cap_nval_pdu_rsp,
	.expect_rsp_len = sizeof(l2cap_nval_pdu_rsp),
};

static const uint8_t l2cap_nval_dc_req[] = { 0x12, 0x34, 0x56, 0x78 };
static const uint8_t l2cap_nval_cid_rsp[] = { 0x02, 0x00 };

static const struct l2cap_server_data l2cap_server_nval_cid_l2cap1 = {
	.send_req_code = BT_L2CAP_PDU_DISCONN_REQ,
	.send_req = l2cap_nval_dc_req,
	.send_req_len = sizeof(l2cap_nval_dc_req),
	.expect_rsp_code = BT_L2CAP_PDU_CMD_REJECT,
	.expect_rsp = l2cap_nval_cid_rsp,
	.expect_rsp_len = sizeof(l2cap_nval_cid_rsp),
};

static const uint8_t l2cap_nval_cfg_req[] = { 0x12, 0x34, 0x00, 0x00 };
static const uint8_t l2cap_nval_cfg_rsp[] = { 0x02, 0x00 };

static const struct l2cap_server_data l2cap_server_nval_cid_l2cap2 = {
	.send_req_code = BT_L2CAP_PDU_CONFIG_REQ,
	.send_req = l2cap_nval_cfg_req,
	.send_req_len = sizeof(l2cap_nval_cfg_req),
	.expect_rsp_code = BT_L2CAP_PDU_CMD_REJECT,
	.expect_rsp = l2cap_nval_cfg_rsp,
	.expect_rsp_len = sizeof(l2cap_nval_cfg_rsp),
};

static void client_connectable_complete(uint16_t opcode, uint8_t status,
					const void *param, uint8_t len,
					void *user_data)
{
	switch (opcode) {
	case BT_HCI_CMD_WRITE_SCAN_ENABLE:
	case BT_HCI_CMD_LE_SET_ADV_ENABLE:
		break;
	default:
		return;
	}

	l2cap_print("Client set connectable status 0x%02x", status);

	if (status)
		l2cap_setup_failed();
	else
		l2cap_setup_complete();
}

static void setup_powered_client_callback(uint8_t status, uint16_t length,
					const void *param, void *user_data)
{
	struct l2cap_data *data = l2cap_get_data();
	struct bthost *bthost;

	if (status != MGMT_STATUS_SUCCESS) {
		l2cap_setup_failed();
		return;
	}

	l2cap_print("Controller powered on");

	bthost = hciemu_client_get_host(data->hciemu);
	bthost_set_cmd_complete_cb(bthost, client_connectable_complete, data);
	if (data->hciemu_type == HCIEMU_TYPE_LE)
		bthost_set_adv_enable(bthost, 0x01);
	else
		bthost_write_scan_enable(bthost, 0x03);
}

static void setup_powered_server_callback(uint8_t status, uint16_t length,
					const void *param, void *user_data)
{
	if (status != MGMT_STATUS_SUCCESS) {
		l2cap_setup_failed();
		return;
	}

	l2cap_print("Controller powered on");

	l2cap_setup_complete();
}

static void setup_powered_client(const void *l2cap_data)
{
	struct l2cap_data *data = l2cap_get_data();
	unsigned char param[] = { 0x01 };

	l2cap_print("Powering on controller");

	if (data->hciemu_type == HCIEMU_TYPE_BREDR)
		mgmt_send(data->mgmt, MGMT_OP_SET_SSP, data->mgmt_index,
				sizeof(param), param, NULL, NULL, NULL);
	else
		mgmt_send(data->mgmt, MGMT_OP_SET_LE, data->mgmt_index,
				sizeof(param), param, NULL, NULL, NULL);

	mgmt_send(data->mgmt, MGMT_OP_SET_POWERED, data->mgmt_index,
			sizeof(param), param, setup_powered_client_callback,
			NULL, NULL);
}

static void setup_powered_server(const void *l2cap_data)
{
	struct l2cap_data *data = l2cap_get_data();
	unsigned char param[] = { 0x01 };

	l2cap_print("Powering on controller");

	if (data->hciemu_type == HCIEMU_TYPE_BREDR) {
		mgmt_send(data->mgmt, MGMT_OP_SET_CONNECTABLE, data->mgmt_index,
				sizeof(param), param,
				NULL, NULL, NULL);
		mgmt_send(data->mgmt, MGMT_OP_SET_SSP, data->mgmt_index,
				sizeof(param), param, NULL, NULL, NULL);
	} else {
		mgmt_send(data->mgmt, MGMT_OP_SET_LE, data->mgmt_index,
				sizeof(param), param, NULL, NULL, NULL);
		mgmt_send(data->mgmt, MGMT_OP_SET_ADVERTISING, data->mgmt_index,
				sizeof(param), param, NULL, NULL, NULL);
	}

	mgmt_send(data->mgmt, MGMT_OP_SET_POWERED, data->mgmt_index,
			sizeof(param), param, setup_powered_server_callback,
			NULL, NULL);
}

static void l2cap_basic(const void *l2cap_data)
{
	int sk;

	sk = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (sk < 0) {
		l2cap_warn("Can't create socket: %s (%d)", strerror(errno),
									errno);
		l2cap_l2cap_failed();
		return;
	}

	close(sk);

	l2cap_l2cap_passed();
}

static gboolean l2cap_connect_cb(GIOChannel *io, GIOCondition cond,
							gpointer user_data)
{
	struct l2cap_data *data = l2cap_get_data();
	const struct l2cap_client_data *l2data = data->l2cap_data;
	int err, sk_err, sk;
	socklen_t len = sizeof(sk_err);

	data->io_id = 0;

	sk = g_io_channel_unix_get_fd(io);

	if (getsockopt(sk, SOL_SOCKET, SO_ERROR, &sk_err, &len) < 0)
		err = -errno;
	else
		err = -sk_err;

	if (err < 0)
		l2cap_warn("Connect failed: %s (%d)", strerror(-err), -err);
	else
		l2cap_print("Successfully connected");

	if (-err != l2data->expect_err)
		l2cap_l2cap_failed();
	else
		l2cap_l2cap_passed();

	return FALSE;
}

static int create_l2cap_sock(struct l2cap_data *data, uint16_t psm)
{
	const uint8_t *master_bdaddr;
	struct sockaddr_l2 addr;
	int sk, err;

	sk = socket(PF_BLUETOOTH, SOCK_SEQPACKET | SOCK_NONBLOCK,
							BTPROTO_L2CAP);
	if (sk < 0) {
		err = -errno;
		l2cap_warn("Can't create socket: %s (%d)", strerror(errno),
									errno);
		return err;
	}

	master_bdaddr = hciemu_get_master_bdaddr(data->hciemu);
	if (!master_bdaddr) {
		l2cap_warn("No master bdaddr");
		return -ENODEV;
	}

	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	addr.l2_psm = htobs(psm);
	bacpy(&addr.l2_bdaddr, (void *) master_bdaddr);
	if (data->hciemu_type == HCIEMU_TYPE_LE)
		addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
	else
		addr.l2_bdaddr_type = BDADDR_BREDR;

	if (bind(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		err = -errno;
		l2cap_warn("Can't bind socket: %s (%d)", strerror(errno),
									errno);
		close(sk);
		return err;
	}

	return sk;
}

static int connect_l2cap_sock(struct l2cap_data *data, int sk, uint16_t psm)
{
	const uint8_t *client_bdaddr;
	struct sockaddr_l2 addr;
	int err;

	client_bdaddr = hciemu_get_client_bdaddr(data->hciemu);
	if (!client_bdaddr) {
		l2cap_warn("No client bdaddr");
		return -ENODEV;
	}

	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, (void *) client_bdaddr);
	addr.l2_psm = htobs(psm);
	if (data->hciemu_type == HCIEMU_TYPE_LE)
		addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
	else
		addr.l2_bdaddr_type = BDADDR_BREDR;

	err = connect(sk, (struct sockaddr *) &addr, sizeof(addr));
	if (err < 0 && !(errno == EAGAIN || errno == EINPROGRESS)) {
		err = -errno;
		l2cap_warn("Can't connect socket: %s (%d)", strerror(errno),
									errno);
		return err;
	}

	return 0;
}

static void l2cap_connect(const void *l2cap_data)
{
	struct l2cap_data *data = l2cap_get_data();
	const struct l2cap_client_data *l2data = data->l2cap_data;
	GIOChannel *io;
	int sk;

	if (l2data->server_psm) {
		struct bthost *bthost = hciemu_client_get_host(data->hciemu);
		bthost_set_server_psm(bthost, l2data->server_psm);
	}

	sk = create_l2cap_sock(data, 0);
	if (sk < 0) {
		l2cap_l2cap_failed();
		return;
	}

	if (connect_l2cap_sock(data, sk, l2data->client_psm) < 0) {
		close(sk);
		l2cap_l2cap_failed();
		return;
	}

	io = g_io_channel_unix_new(sk);
	g_io_channel_set_close_on_unref(io, TRUE);

	data->io_id = g_io_add_watch(io, G_IO_OUT, l2cap_connect_cb, NULL);

	g_io_channel_unref(io);

	l2cap_print("Connect in progress");
}

static gboolean l2cap_listen_cb(GIOChannel *io, GIOCondition cond,
							gpointer user_data)
{
	struct l2cap_data *data = l2cap_get_data();
	int sk, new_sk;

	data->io_id = 0;

	sk = g_io_channel_unix_get_fd(io);

	new_sk = accept(sk, NULL, NULL);
	if (new_sk < 0) {
		l2cap_warn("accept failed: %s (%u)", strerror(errno), errno);
		l2cap_l2cap_failed();
		return FALSE;
	}

	l2cap_print("Successfully connected");

	close(new_sk);

	l2cap_l2cap_passed();

	return FALSE;
}

static void client_l2cap_rsp(uint8_t code, const void *data, uint16_t len,
							void *user_data)
{
	struct l2cap_data *l2cap_data = user_data;
	const struct l2cap_server_data *l2data = l2cap_data->l2cap_data;

	l2cap_print("Client received response code 0x%02x", code);

	if (code != l2data->expect_rsp_code) {
		l2cap_warn("Unexpected L2CAP response code (expected 0x%02x)",
						l2data->expect_rsp_code);
		goto failed;
	}

	if (!l2data->expect_rsp) {
		l2cap_l2cap_passed();
		return;
	}

	if (l2data->expect_rsp_len != len) {
		l2cap_warn("Unexpected L2CAP response length (%u != %u)",
						len, l2data->expect_rsp_len);
		goto failed;
	}

	if (memcmp(l2data->expect_rsp, data, len) != 0) {
		l2cap_warn("Unexpected L2CAP response");
		goto failed;
	}

	l2cap_l2cap_passed();
	return;

failed:
	l2cap_l2cap_failed();
}

static void client_new_conn(uint16_t handle, void *user_data)
{
	struct l2cap_data *data = user_data;
	const struct l2cap_server_data *l2data = data->l2cap_data;
	struct bthost *bthost;

	l2cap_print("New client connection with handle 0x%04x", handle);

	if (l2data->send_req) {
		bthost_l2cap_rsp_cb cb;

		if (l2data->expect_rsp_code)
			cb = client_l2cap_rsp;
		else
			cb = NULL;

		l2cap_print("Sending L2CAP Request from client");

		bthost = hciemu_client_get_host(data->hciemu);
		bthost_l2cap_req(bthost, handle, l2data->send_req_code,
					l2data->send_req, l2data->send_req_len,
					cb, data);
	}
}

static void l2cap_server(const void *l2cap_data)
{
	struct l2cap_data *data = l2cap_get_data();
	const struct l2cap_server_data *l2data = data->l2cap_data;
	const uint8_t *master_bdaddr;
	uint8_t addr_type;
	struct bthost *bthost;
	GIOChannel *io;
	int sk;

	if (l2data->server_psm) {
		sk = create_l2cap_sock(data, l2data->server_psm);
		if (sk < 0) {
			l2cap_l2cap_failed();
			return;
		}

		if (listen(sk, 5) < 0) {
			l2cap_warn("listening on socket failed: %s (%u)",
					strerror(errno), errno);
			l2cap_l2cap_failed();
			return;
		}

		io = g_io_channel_unix_new(sk);
		g_io_channel_set_close_on_unref(io, TRUE);

		data->io_id = g_io_add_watch(io, G_IO_IN, l2cap_listen_cb,
									NULL);
		g_io_channel_unref(io);

		l2cap_print("Listening for connections");
	}

	master_bdaddr = hciemu_get_master_bdaddr(data->hciemu);
	if (!master_bdaddr) {
		l2cap_warn("No master bdaddr");
		l2cap_l2cap_failed();
		return;
	}

	bthost = hciemu_client_get_host(data->hciemu);
	bthost_set_connect_cb(bthost, client_new_conn, data);

	if (data->hciemu_type == HCIEMU_TYPE_BREDR)
		addr_type = BDADDR_BREDR;
	else
		addr_type = BDADDR_LE_PUBLIC;

	bthost_hci_connect(bthost, master_bdaddr, addr_type);
}

