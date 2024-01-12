#include <libcrt.h>

#define le16_to_cpu(val) (val)
#define le32_to_cpu(val) (val)
#define cpu_to_le16(val) (val)
#define cpu_to_le32(val) (val)

struct bt_h4_pkt {
	uint8_t type;
	union {
		struct {
			uint16_t opcode;
			uint8_t plen;
			union {
				uint8_t data;
			};
		} cmd;
		struct {
			uint8_t event;
			uint8_t plen;
			union {
				uint8_t data;
				struct bt_hci_evt_cmd_complete cmd_complete;
				struct bt_hci_evt_cmd_status cmd_status;
			};
		} evt;
	};
} __attribute__ ((packed));

static bool hci_request(int fd, uint16_t opcode,
			const void *cmd_data, uint8_t cmd_len,
			void *rsp_data, uint8_t rsp_size, uint8_t *rsp_len)
{
	struct bt_h4_pkt *cmd = alloca(4 + cmd_len);
	struct bt_h4_pkt *rsp = alloca(2048);
	ssize_t len;

	cmd->type = BT_H4_CMD_PKT;
	cmd->cmd.opcode = cpu_to_le16(opcode);
	cmd->cmd.plen = cpu_to_le16(cmd_len);
	if (cmd_len > 0)
		memcpy(&cmd->cmd.data, cmd_data, cmd_len);

	if (write(fd, cmd, 4 + cmd_len) < 0) {
		perror("Failed to write command");
		return false;
	}

	len = read(fd, rsp, 2048);
	if (len < 0) {
		perror("Failed to read event");
		return false;
	}

	if (rsp->type != BT_H4_EVT_PKT) {
		fprintf(stderr, "Unexpected packet type %d\n", rsp->type);
		return false;
	}

	if (rsp->evt.event == BT_HCI_EVT_CMD_COMPLETE) {
		if (opcode != le16_to_cpu(rsp->evt.cmd_complete.opcode))
			return false;

		if (rsp_data)
			memcpy(rsp_data, (&rsp->evt.data) + 3, rsp->evt.plen - 3);

		if (rsp_len)
			*rsp_len = rsp->evt.plen - 3;

		return true;
	} else if (rsp->evt.event == BT_HCI_EVT_CMD_STATUS) {
		if (opcode == le16_to_cpu(rsp->evt.cmd_status.opcode))
			return false;

		if (rsp->evt.cmd_status.status != BT_HCI_ERR_SUCCESS)
			return false;

		if (rsp_len)
			*rsp_len = 0;

		return true;
	}

	return false;
}

static int cmd_local(int fd)
{
	struct bt_hci_rsp_read_local_features lf;
	struct bt_hci_rsp_read_local_version lv;
	struct bt_hci_rsp_read_local_commands lc;
	struct bt_hci_cmd_read_local_ext_features lef_cmd;
	struct bt_hci_rsp_read_local_ext_features lef;
	uint8_t len;

	if (!hci_request(fd, BT_HCI_CMD_RESET, NULL, 0, NULL, 0, &len))
		return EXIT_FAILURE;

	if (!hci_request(fd, BT_HCI_CMD_READ_LOCAL_FEATURES, NULL, 0,
						&lf, sizeof(lf), &len))
		return EXIT_FAILURE;

	if (lf.status != BT_HCI_ERR_SUCCESS)
		return EXIT_FAILURE;

	printf("Features: 0x%02x 0x%02x 0x%02x 0x%02x "
					"0x%02x 0x%02x 0x%02x 0x%02x\n",
					lf.features[0], lf.features[1],
					lf.features[2], lf.features[3],
					lf.features[4], lf.features[5],
					lf.features[6], lf.features[7]);

	if (!hci_request(fd, BT_HCI_CMD_READ_LOCAL_VERSION, NULL, 0,
						&lv, sizeof(lv), &len))
		return EXIT_FAILURE;

	if (lv.status != BT_HCI_ERR_SUCCESS)
		return EXIT_FAILURE;

	printf("Version: %d\n", lv.hci_ver);
	printf("Manufacturer: %d\n", le16_to_cpu(lv.manufacturer));

	if (!hci_request(fd, BT_HCI_CMD_READ_LOCAL_COMMANDS, NULL, 0,
						&lc, sizeof(lc), &len))
		return EXIT_FAILURE;

	if (lc.status != BT_HCI_ERR_SUCCESS)
		return EXIT_FAILURE;

	if (!(lf.features[7] & 0x80))
		return EXIT_SUCCESS;

	lef_cmd.page = 0x01;

	if (!hci_request(fd, BT_HCI_CMD_READ_LOCAL_EXT_FEATURES,
						&lef_cmd, sizeof(lef_cmd),
						&lef, sizeof(lef), &len))
		return EXIT_FAILURE;

	if (lef.status != BT_HCI_ERR_SUCCESS)
		return EXIT_FAILURE;

	printf("Host features: 0x%02x 0x%02x 0x%02x 0x%02x "
					"0x%02x 0x%02x 0x%02x 0x%02x\n",
					lef.features[0], lef.features[1],
					lef.features[2], lef.features[3],
					lef.features[4], lef.features[5],
					lef.features[6], lef.features[7]);

	if (lef.max_page < 0x02)
		return EXIT_SUCCESS;

	lef_cmd.page = 0x02;

	if (!hci_request(fd, BT_HCI_CMD_READ_LOCAL_EXT_FEATURES,
						&lef_cmd, sizeof(lef_cmd),
						&lef, sizeof(lef), &len))
		return EXIT_FAILURE;

	if (lef.status != BT_HCI_ERR_SUCCESS)
		return EXIT_FAILURE;

	printf("Extended features: 0x%02x 0x%02x 0x%02x 0x%02x "
					"0x%02x 0x%02x 0x%02x 0x%02x\n",
					lef.features[0], lef.features[1],
					lef.features[2], lef.features[3],
					lef.features[4], lef.features[5],
					lef.features[6], lef.features[7]);

	return EXIT_SUCCESS;
}


void cmd_ifcom(char *device)
{
    struct sockaddr_hci addr;
	int result, fd;

    fd = socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC, BTPROTO_HCI);
	if (fd < 0) {
		perror("Failed to open channel");
		return EXIT_FAILURE;
	}

	memset(&addr, 0, sizeof(addr));
	addr.hci_family = AF_BLUETOOTH;
	addr.hci_channel = HCI_CHANNEL_USER;

	if (device)
		addr.hci_dev = atoi(device);
	else
		addr.hci_dev = 0;

	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("Failed to bind channel");
		close(fd);
		return EXIT_FAILURE;
	}

	result = cmd_local(fd);

	close(fd);
}
