/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef IW_DEV_NET_H
#define IW_DEV_NET_H

#include "iw_if.h"
#include "iw_dev.h"

#define MAX_NET_DRIVER		5u
#define MAX_NET_DEV 		5u
#define MAX_NAME_STR_SIZE  32u 	
#define MAX_SERVER_ADDRESS	256u


#define PROTO_TCP           1
#define PROTO_UDP           2

#define LORA_JOIN_DEV_ADDR	8u
#define LORA_JOIN_DEV_EUI   16u
#define LORA_JOIN_APP_EUI	16u
#define LORA_JOIN_APP_KEY   32u
#define LORA_JOIN_NWKS_KEY  32u
#define LORA_JOIN_APPS_KEY  32u

#define LORA_BAND_NAME_LEN	5u

#define LORA_RSSI_INVALID        (-200)

#define MAX_BLE_NAME_LEN	64u
#define PINCODE_LEN			4u

typedef enum{
	NET_DEV_TYPE_NONE		= 0,
 	NET_DEV_TYPE_WIFI		= (1 << 0),
 	NET_DEV_TYPE_BLE		= (1 << 1),
 	NET_DEV_TYPE_LORA		= (1 << 2),		
}net_type_t;

typedef enum{	
 	NET_PWR_RUN_MODE 		=0,
 	NET_PWR_SLEEP_MODE,
 	NET_PWR_LOWPOWER_MODE,
	NET_PWR_OFF_MODE,
 	NET_PWR_AUTO_MODE, 
 	NET_PWR_MAX,
}net_pwr_mode_t;

typedef enum{
	NET_DISCONNECTED = 0,
 	NET_CONNECTED,
 	NET_GOT_IP,
}net_con_t;

typedef enum{
	NET_SUCCESS = 0,
	NET_FAIL = -1,
	NET_TIMEOUT = -2,
	NET_AP_NOT_FOUND = -3,
}net_error_t;

typedef enum{ 	
 	SERVER_DISCONNECTED = 0,
 	SERVER_CONNECTED = 1,
 	SERVER_ACONNECTED = 2,
	SERVER_GOTIP = 3,
}net_server_t;

typedef enum{
	NET_RES_OK = 0,
	NET_RES_ERROR,
	NET_RES_FAIL,
	NET_RES_NO,
}net_response_t;

typedef struct{
	net_response_t res;
	bool ready;
	int rssi;
	unsigned int available;
}net_com_status_t;

typedef enum{
	WAIT_OK = 0,
	WAIT_READY,
}net_res_wait_t;

typedef enum{
	WIFI_AP_FOUND = 1,
	WIFI_AP_NOT_FOUND = 0,
}wifi_ap_t;

typedef struct{
	wifi_ap_t ap_found;
	net_con_t con_status;
}wifi_ap_status_t;

typedef enum{
	JOIN_ABP = 1,
	JOIN_OTAA
}lora_join_mode_t;

typedef struct{
	lora_join_mode_t join_mode;
	char dev_addr[LORA_JOIN_DEV_ADDR + 1];
	char dev_eui[LORA_JOIN_DEV_EUI + 1];
	char nwks_key[LORA_JOIN_NWKS_KEY + 1];
	char app_eui[LORA_JOIN_APP_EUI + 1];
	char apps_key[LORA_JOIN_APPS_KEY +  1];
	char app_key[LORA_JOIN_APP_KEY +  1];
}lora_join_info_t;

typedef enum
{
	BLE_MASTER_MODE = 1,
	BLE_SLAVE_MODE = 2,
}ble_mode_t;

typedef struct{
	char name[MAX_BLE_NAME_LEN];
	char pincode[PINCODE_LEN+1];
	ble_mode_t mode;
}ble_info_t;

typedef struct{
	iw_error_t (*init)(void *param);
	iw_error_t (*deinit)(void *param);	
	iw_error_t (*is_ready)(void *param, dev_init_status_t *status); 
	iw_error_t (*available)( void *param, unsigned int *len);
	iw_error_t (*recv_data)( void *param, char *buff, unsigned int *len);
	iw_error_t (*send_data)( void *param, char *buff, unsigned int len);
	int (*connect)(void *param, unsigned int timeout);
	iw_error_t (*disconnect)(void *param);
	iw_error_t (*is_connected)(void *param, int *connected);
	int (*scan)(void * param, int *rssi);
	iw_error_t (*get_capa)(void *param, wake_src_type_t *wake_src);
	iw_error_t (*set_config)(void *param, wake_src_type_t wake_src, void * config);	
	iw_error_t (*get_config)(void *param, wake_src_type_t *wake_src, void * config);	
	iw_error_t (*set_power)(void *param, net_pwr_mode_t pwr_mode);
	iw_error_t (*get_power)(void *param, net_pwr_mode_t *pwr_mode);
	iw_error_t (*get_rssi)(void *param, int *rssi);
	int (*connect_server)(void *param, char *address, int port, unsigned int timeout, int proto);
	int (*disconnect_server)(void *param);
}net_func_t;

typedef struct
{
	net_type_t 	type;
	if_type_t 	iface;
	if_id_t 	iface_id;
	unsigned char   dev_id;
	char  	name[MAX_DRIVER_NAME_LEN];
	//sensor_pwr_mode_t pwr_mode;
	net_func_t	*func;
}net_dev_info_t;


typedef struct{
	unsigned char name[MAX_DRIVER_NAME_LEN];
	net_func_t *func;
}net_driver_t;

typedef struct
{
	unsigned char dev_num;
	net_dev_info_t dev[MAX_NET_DEV];
	unsigned char driver_num;
	net_driver_t driver[MAX_NET_DRIVER];
}net_info_t;

typedef struct
{	
	unsigned char dev_id;
	net_type_t type;
	unsigned char name[MAX_DRIVER_NAME_LEN];
}registered_net_info_t;

typedef struct{
	char ssid[MAX_NAME_STR_SIZE];
	char pw[MAX_NAME_STR_SIZE];	
}wifi_ap_info_t;

//iw_error_t iw_get_registered_net_num(unsigned char *num);
void iw_register_network(net_type_t type, if_type_t iface, if_id_t id, int info, char *name);
void iw_register_network_driver(net_driver_t *net_driver);
iw_error_t iw_register_network_(net_type_t dev_type, const char *name, if_type_t if_type, uint32_t if_num, void *if_arg, net_driver_t *dev_driver);
uint8_t iw_get_registered_net_num(void);


iw_error_t iw_get_all_networks_info(registered_net_info_t *info);
iw_error_t iw_init_network(unsigned char dev_id);
iw_error_t iw_deinit_network (unsigned char dev_id);
iw_error_t iw_is_ready_network(unsigned char dev_id, dev_init_status_t *status); 
iw_error_t iw_available_network(unsigned char dev_id, unsigned int *len);
iw_error_t iw_recv_data_network (unsigned char dev_id, char *buff, unsigned int *len);
iw_error_t iw_send_data_network (unsigned char dev_id, char *buff, unsigned int len);
int iw_connect_network(unsigned char dev_id, unsigned int timeout);
iw_error_t iw_disconnect_network(unsigned char dev_id);
iw_error_t iw_is_connected_network(unsigned char dev_id, int *connected);
int iw_scan_network(unsigned char dev_id, int *rssi);
iw_error_t iw_get_capa_network(unsigned char dev_id, wake_src_type_t *wake_src);
iw_error_t iw_set_config_network(unsigned char dev_id, wake_src_type_t wake_src, void *config);
iw_error_t iw_get_config_network(unsigned char dev_id, wake_src_type_t *wake_src, void *config);
iw_error_t iw_set_pwrmode_network(unsigned char dev_id, net_pwr_mode_t pwr_mode);
iw_error_t iw_get_pwrmode_network(unsigned char dev_id, net_pwr_mode_t *pwr_mode);
iw_error_t iw_get_rssi_network(unsigned char dev_id, int *rssi);
int iw_connect_server(unsigned char dev_id, char *address, int port, unsigned int timeout, int proto);
int iw_disconnect_server(unsigned char dev_id);

iw_error_t iw_get_network_info(net_type_t type, char *name, registered_net_info_t *info);
iw_error_t iw_get_wifi_info(wifi_ap_info_t *ap);

iw_error_t iw_get_network_devid(net_type_t type, char *name, unsigned char *devid);
iw_error_t iw_get_network_dev_id_by_type(net_type_t type, uint8_t *dev_id);
iw_error_t iw_get_network_dev_id_by_name(char *name, uint8_t *dev_id);

iw_error_t iw_register_lora_abp_info(char *dev_addr, char *nwks_key, char *apps_key);
iw_error_t iw_register_lora_otaa_info(char *dev_eui, char *app_eui, char *app_key);
iw_error_t iw_get_lora_info(lora_join_info_t *join);
iw_error_t iw_register_lora_band_info(char *band);
iw_error_t iw_get_lora_band_info(char *band);

iw_error_t iw_register_ble_info(char *name, char *pincode, ble_mode_t mode);
iw_error_t iw_get_ble_info(ble_info_t *info);
#endif /* IW_DEV_NET_H */

