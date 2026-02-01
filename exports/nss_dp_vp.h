/*
 * Copyright (c) 2022, 2024-2025 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __NSS_DP_VP_H__
#define __NSS_DP_VP_H__

#include <linux/bitmap.h>
#include <ppe_drv_public.h>

/*
 * nss_dp_vp_tx_info
 *	VP Tx info.
 */
struct nss_dp_vp_tx_info {
	uint32_t flags;			/**< VP Tx flags. */
	uint8_t sc;			/**< Service code. */
	uint8_t svp;			/**< Source VP number. */
	uint8_t dvp;			/**< Destination VP number. */
	uint8_t egress_macid;		/**< Egress Port Mac Id. */
	bool fake_mac;			/**< Needs Fake Mac. */
};

/*
 * nss_dp_vp_rx_info
 *	VP info struct struct
 */
struct nss_dp_vp_rx_info {
	struct napi_struct *napi;	/* RX NAPI */
	uint32_t batch_bytes;		/* Total bytes carried by batch of skbs */
	int32_t flow_idx;		/* Flow index of a packet */
	uint16_t l3offset;		/* L3 offset of packet */
	uint8_t dvp;			/* Destination VP number */
	uint8_t svp;			/* Source VP number */
	uint8_t ip_summed;		/* IP checksum */
	bool fake_mac;			/* Fake Mac Present */
	bool qdisc_valid;		/* Qdisc valid */
};

/*
 * nss_dp_vp_node_info
 *	PPE VP node info
 */
struct nss_dp_vp_node_info {
	uint32_t bytes;			/* Total bytes carried batch of skbs */
	uint8_t dvp;			/* Destination VP */
};

/*
 * nss_dp_vp_node
 *	Node for VP specific operations(batching)
 */
struct nss_dp_vp_node {
	struct sk_buff_head head;		/* Skb list */
	struct nss_dp_vp_node_info info;	/* VP node info */
};

/*
 * nss_dp_vp_list_rx_cb_t
 *	Vp rx handler callback typedef
 */
typedef void (*nss_dp_vp_list_rx_cb_t)(struct sk_buff_head *head, struct nss_dp_vp_rx_info *rx_info);

/*
 * nss_dp_vp_rx_cb_t
 *	Vp rx handler callback typedef
 */
typedef void (*nss_dp_vp_rx_cb_t)(struct sk_buff *skb, struct nss_dp_vp_rx_info *vprxi);

/*
 * nss_dp_vp_rx_ops
 *	VP rx operations
 */
struct nss_dp_vp_rx_ops {
	nss_dp_vp_list_rx_cb_t list_cb;
	nss_dp_vp_rx_cb_t cb;
	void *app_data;
};

/*
 * nss_dp_vp_ctx
 *	Context per VP node
 */
struct nss_dp_vp_ctx {
	DECLARE_BITMAP(active_vps, PPE_DRV_VIRTUAL_MAX);
	struct nss_dp_vp_node nodes[PPE_DRV_VIRTUAL_MAX];
	struct nss_dp_vp_rx_ops ops;
};

/**
 * nss_dp_vp_rx_register_cb
 *	Register handler for VP rx processing.
 *
 * @datatypes
 * nss_dp_vp_rx_cb_t
 *
 * @param[in] nss_dp_vp_tx_info Pointer to VP rx handler.
 *
 * @return
 * True or false.
 *
 * @note: This API needs to deprecated and replaced with nss_dp_vp_rx_register_ops()
 */
bool nss_dp_vp_rx_register_cb(nss_dp_vp_rx_cb_t cb);

/**
 * nss_dp_vp_rx_register_ops
 *	Register ops for VP rx processing.
 *
 * @datatypes
 * struct nss_dp_vp_rx_ops
 *
 * @param[in] ops Pointer to VP rx ops.
 *
 * @return
 * True or false.
 */
void nss_dp_vp_rx_register_ops(struct nss_dp_vp_rx_ops *ops);

/**
 * nss_dp_vp_rx_unregister_ops
 *	Unregister ops for VP rx processing.
 *
 * @datatypes
 * None
 *
 * @param[in]
 *
 * @return
 * None.
 */
void nss_dp_vp_rx_unregister_ops(void);

/**
 * nss_dp_vp_rx_unregister_cb
 *	Unregister VP handler for VP rx processing.
 *
 * @datatypes
 * None.
 *
 * @param[in] nss_dp_vp_tx_info Pointer to VP rx handler.
 *
 * @return
 * None.
 */
void nss_dp_vp_rx_unregister_cb(void);

/**
 * nss_dp_vp_xmit
 *	Transmits a packet to the appropriate VP netdevice.
 *
 * @datatypes
 * net_device
 * nss_dp_vp_tx_info
 * sk_buff
 *
 * @param[in] net_device Pointer to the netdev structure.
 * @param[in] nss_dp_vp_tx_info Pointer to the VP info structure.
 * @param[in] skb Pointer to the packet.
 *
 * @return
 * Tx status.
 */
netdev_tx_t nss_dp_vp_xmit(struct net_device *netdev, struct nss_dp_vp_tx_info *info, struct sk_buff *skb);

/**
 * nss_dp_vp_init()
 *	Initialize virtual port netdevice.
 *
 * @return
 * Netdevice for the VP port.
 */
struct net_device *nss_dp_vp_init(void);

/**
 * nss_dp_vp_deinit()
 *	De-initialize virtual port netdevice.
 *
 * @return
 * Status of virtual port deinit.
 */
bool nss_dp_vp_deinit(struct net_device *netdev);

#endif	/** __NSS_DP_VP_H__ */
