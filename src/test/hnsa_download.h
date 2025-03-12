/*
 *@@@bF
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

 ##     ## ##    ##  ######     ###
 ##     ## ###   ## ##    ##   ## ##
 ##     ## ####  ## ##        ##   ##
 ######### ## ## ##  ######  ##     ##
 ##     ## ##  ####       ## #########
 ##     ## ##   ### ##    ## ##     ##
 ##     ## ##    ##  ######  ##     ##


 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 File                          :  hnsa_download.h
 Product                       :  HAL3-LINUX
 Language                      :  C
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 *@@@eF
 */

/*

  This file is the header file for the DOWNLOAD device as described in the document
  "HAL STB Edition HAL3 API Specifications: Part 13 Download device".
  As such it shall be considered part of the HAL3 documentation and falls under
  the following copyright.

  Copyright

  The contents of this documentation are strictly confidential and the receiver
  is obliged to use them exclusively for his or her own purposes. No part of
  SoftAtHome applications or this document may be reproduced or transmitted in
  any form or by any means, electronic or mechanical, including photocopying,
  recording, or by any information storage and retrieval system, without
  permission in writing from SoftAtHome. The information in this document is
  subject to change without notice. SoftAtHome does not warrant that this document
  is error free. If you find any problems with this documentation or wish to make
  comments, please report them to SoftAtHome in writing.

*/

#ifndef _HNSA_DOWNLOAD_H_
#define _HNSA_DOWNLOAD_H_

/*******************************************************************************
            INCLUDES
*******************************************************************************/

/*******************************************************************************
            HNSA VERSION
*******************************************************************************/
#define HNSA_DOWNLOAD_API_VERSION HNSA_DRVHAL_API_VERSION(2, 0, 0, 0)
#define HNSA_DOWNLOAD_API_REVISION 4

/*******************************************************************************
            DEFINES
*******************************************************************************/

#define HNSA_MAX_FILENAME_SIZE 150
#define HNSA_MAX_URI_SIZE 256
#define HNSA_SSU_MAX_PARAMS 5
#define HNSA_DOWNLOAD_MAX_IMAGE 5
#define HNSA_DOWNLOAD_MAX_PREFERENCE 3
#define HNSA_MAX_ID_IMAGE 256
#define HNSA_DOWNLOAD_DEF_IMAGE 256
#define HNSA_DOWNLOAD_MAX_STATUS 256
#define HNSA_DOWNLOAD_MAX_ADDRESS_SIZE 128
#define HNSA_DOWNLOAD_MAX_EVENTS_QUEUE 32
#define HNSA_MAX_URL_SIZE 256
#define HNSA_DOWNLOAD_MAX_COUNTRY_SIZE 4
#define HNSA_DOWNLOAD_MAX_WIFI_SSID 33
#define HNSA_DOWNLOAD_MAX_WIFI_KEY 65
/*******************************************************************************
            TYPES
*******************************************************************************/

typedef enum {
    HNSA_DOWNLOAD_NA,
    HNSA_DOWNLOAD_SUCCESSFULL,
    HNSA_DOWNLOAD_FAILED_TRANSFER,
    HNSA_DOWNLOAD_FAILED_INTEGRITY,
    HNSA_DOWNLOAD_FAILED_AUTH,
    HNSA_DOWNLOAD_FAILED_OTHER,
    HNSA_DOWNLOAD_STATUS_LAST
} hnsa_download_status_t;

typedef enum {
    HNSA_DOWNLOAD_TYPE_TFTP = (1 << 0),
    HNSA_DOWNLOAD_TYPE_SSU = (1 << 1),
    HNSA_DOWNLOAD_TYPE_NOT_USED = (1 << 2),
    HNSA_DOWNLOAD_TYPE_HTTP_HTTPS = (1 << 3),
    HNSA_DOWNLOAD_TYPE_USB = (1 << 4),
    HNSA_DOWNLOAD_TYPE_HDD = (1 << 5),
    HNSA_DOWNLOAD_TYPE_SYSTEM = (1 << 6),
    HNSA_DOWNLOAD_TYPE_LAST = (1 << 7)
} hnsa_download_type_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_PROTOCOL_AUTO = (1 << 0),
    HNSA_DOWNLOAD_SSU_PROTOCOL_UDP = (1 << 1),
    HNSA_DOWNLOAD_SSU_PROTOCOL_RTP = (1 << 2),
    HNSA_DOWNLOAD_SSU_PROTOCOL_TCP = (1 << 3),
    HNSA_DOWNLOAD_SSU_PROTOCOL_LAST = (1 << 4)
} hnsa_download_ssu_protocol_t;

typedef enum {
    HNSA_DOWNLOAD_DVB_S = (1 << 0),
    HNSA_DOWNLOAD_DVB_S2 = (1 << 1),
    HNSA_DOWNLOAD_DVB_T = (1 << 2),
    HNSA_DOWNLOAD_DVB_T2 = (1 << 3),
    HNSA_DOWNLOAD_DVB_LAST = (1 << 4)
} hnsa_download_dvb_type_t;

typedef enum {
    HNSA_DOWNLOAD_INVERSION_OFF = (1 << 0),
    HNSA_DOWNLOAD_INVERSION_ON = (1 << 1),
    HNSA_DOWNLOAD_INVERSION_AUTO = (1 << 2),
    HNSA_DOWNLOAD_SPECTRAL_INVERSION_LAST = (1 << 3)
} hnsa_download_spectral_inversion_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_HIERARCHY_NONE = (1 << 0),
    HNSA_DOWNLOAD_SSU_HIERARCHY_1 = (1 << 1),
    HNSA_DOWNLOAD_SSU_HIERARCHY_2 = (1 << 2),
    HNSA_DOWNLOAD_SSU_HIERARCHY_4 = (1 << 3),
    HNSA_DOWNLOAD_SSU_HIERARCHY_AUTO = (1 << 4),
    HNSA_DOWNLOAD_SSU_HIERARCHY_LAST = (1 << 5)
} hnsa_download_ssu_hierarchy_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_ROLLOFF_35 = 0,
    HNSA_DOWNLOAD_SSU_ROLLOFF_25,
    HNSA_DOWNLOAD_SSU_ROLLOFF_20,
    HNSA_DOWNLOAD_SSU_ROLLOFF_LAST
} hnsa_download_ssu_rolloff_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_TYPE_IP = (1 << 0),
    HNSA_DOWNLOAD_SSU_TYPE_DVB_S_S2_T = (1 << 1),
    HNSA_DOWNLOAD_SSU_TYPE_LAST = (1 << 2)
} hnsa_download_ssu_type_t;

typedef enum {
    HNSA_DOWNLOAD_DUMMY_PLF = (1 << 0),
    HNSA_DOWNLOAD_QPSK_14 = (1 << 1),
    HNSA_DOWNLOAD_QPSK_13 = (1 << 2),
    HNSA_DOWNLOAD_QPSK_25 = (1 << 3),
    HNSA_DOWNLOAD_QPSK_12 = (1 << 4),
    HNSA_DOWNLOAD_QPSK_35 = (1 << 5),
    HNSA_DOWNLOAD_QPSK_23 = (1 << 6),
    HNSA_DOWNLOAD_QPSK_34 = (1 << 7),
    HNSA_DOWNLOAD_QPSK_45 = (1 << 8),
    HNSA_DOWNLOAD_QPSK_56 = (1 << 9),
    HNSA_DOWNLOAD_QPSK_89 = (1 << 10),
    HNSA_DOWNLOAD_QPSK_910 = (1 << 11),
    HNSA_DOWNLOAD_8PSK_35 = (1 << 12),
    HNSA_DOWNLOAD_8PSK_23 = (1 << 13),
    HNSA_DOWNLOAD_8PSK_34 = (1 << 14),
    HNSA_DOWNLOAD_8PSK_56 = (1 << 15),
    HNSA_DOWNLOAD_8PSK_89 = (1 << 16),
    HNSA_DOWNLOAD_8PSK_910 = (1 << 17),
    HNSA_DOWNLOAD_16APSK_23 = (1 << 18),
    HNSA_DOWNLOAD_16APSK_34 = (1 << 19),
    HNSA_DOWNLOAD_16APSK_45 = (1 << 20),
    HNSA_DOWNLOAD_16APSK_56 = (1 << 21),
    HNSA_DOWNLOAD_16APSK_89 = (1 << 22),
    HNSA_DOWNLOAD_16APSK_910 = (1 << 23),
    HNSA_DOWNLOAD_32APSK_34 = (1 << 24),
    HNSA_DOWNLOAD_32APSK_45 = (1 << 25),
    HNSA_DOWNLOAD_32APSK_56 = (1 << 26),
    HNSA_DOWNLOAD_32APSK_89 = (1 << 27),
    HNSA_DOWNLOAD_32APSK_910 = (1 << 28),
    HNSA_DOWNLOAD_MODCOD_AUTO = (1 << 29),
    HNSA_DOWNLOAD_MODCOD_LAST = (1 << 30)
} hnsa_download_ssu_modulation_t;

typedef enum {
    HNSA_DOWNLOAD_QPSK = (1 << 0),
    HNSA_DOWNLOAD_QAM_16 = (1 << 1),
    HNSA_DOWNLOAD_QAM_32 = (1 << 2),
    HNSA_DOWNLOAD_QAM_64 = (1 << 3),
    HNSA_DOWNLOAD_QAM_128 = (1 << 4),
    HNSA_DOWNLOAD_QAM_256 = (1 << 5),
    HNSA_DOWNLOAD_MOD_AUTO = (1 << 6),
    HNSA_DOWNLOAD_MODULATION_LAST = (1 << 7)
} hnsa_download_ssu_constellation_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_1_32 = (1 << 0),
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_1_16 = (1 << 1),
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_1_8 = (1 << 2),
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_1_4 = (1 << 3),
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_1_128 = (1 << 4),
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_19_128 = (1 << 5),
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_19_256 = (1 << 6),
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_AUTO = (1 << 7),
    HNSA_DOWNLOAD_SSU_GUARD_INTERVAL_LAST = (1 << 8)
} hnsa_download_ssu_guard_interval_t;

typedef struct
{
    uint32_t tftp_address;
    uint16_t tftp_port;
    char tftp_file_name[HNSA_MAX_FILENAME_SIZE];
} hnsa_download_tftp_params_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_TRANSMIT_MODE_2K = (1 << 0),
    HNSA_DOWNLOAD_SSU_TRANSMIT_MODE_8K = (1 << 1),
    HNSA_DOWNLOAD_SSU_TRANSMIT_MODE_4K = (1 << 2),
    HNSA_DOWNLOAD_SSU_TRANSMIT_MODE_32K = (1 << 3),
    HNSA_DOWNLOAD_SSU_TRANSMIT_MODE_16K = (1 << 4),
    HNSA_DOWNLOAD_SSU_TRANSMIT_MODE_1K = (1 << 5),
    HNSA_DOWNLOAD_SSU_TRANSMIT_MODE_AUTO = (1 << 6),
    HNSA_DOWNLOAD_SSU_TRANSMIT_MODE_LAST = (1 << 7)
} hnsa_download_ssu_transmit_mode_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_BANDWIDTH_8_MHZ = (1 << 0),
    HNSA_DOWNLOAD_SSU_BANDWIDTH_7_MHZ = (1 << 1),
    HNSA_DOWNLOAD_SSU_BANDWIDTH_6_MHZ = (1 << 2),
    HNSA_DOWNLOAD_SSU_BANDWIDTH_5_MHZ = (1 << 3),
    HNSA_DOWNLOAD_SSU_BANDWIDTH_1_7_MHZ = (1 << 4),
    HNSA_DOWNLOAD_SSU_BANDWIDTH_AUTO = (1 << 5),
    HNSA_DOWNLOAD_SSU_BANDWIDTH_LAST = (1 << 6)
} hnsa_download_ssu_bandwidth_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_CODE_RATE_NONE = (1 << 0),
    HNSA_DOWNLOAD_SSU_CODE_RATE_1_4 = (1 << 1),
    HNSA_DOWNLOAD_SSU_CODE_RATE_1_3 = (1 << 2),
    HNSA_DOWNLOAD_SSU_CODE_RATE_2_5 = (1 << 3),
    HNSA_DOWNLOAD_SSU_CODE_RATE_1_2 = (1 << 4),
    HNSA_DOWNLOAD_SSU_CODE_RATE_2_3 = (1 << 5),
    HNSA_DOWNLOAD_SSU_CODE_RATE_3_4 = (1 << 6),
    HNSA_DOWNLOAD_SSU_CODE_RATE_3_5 = (1 << 7),
    HNSA_DOWNLOAD_SSU_CODE_RATE_4_5 = (1 << 8),
    HNSA_DOWNLOAD_SSU_CODE_RATE_5_6 = (1 << 9),
    HNSA_DOWNLOAD_SSU_CODE_RATE_7_8 = (1 << 10),
    HNSA_DOWNLOAD_SSU_CODE_RATE_8_9 = (1 << 11),
    HNSA_DOWNLOAD_SSU_CODE_RATE_9_10 = (1 << 12),
    HNSA_DOWNLOAD_SSU_CODE_RATE_AUTO = (1 << 13),
    HNSA_DOWNLOAD_SSU_CODE_RATE_LAST = (1 << 14)
} hnsa_download_ssu_code_rate_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_LNB_POWER_OFF,
    HNSA_DOWNLOAD_SSU_LNB_POWER_ON,
    HNSA_DOWNLOAD_SSU_LNB_POWER_LAST
} hnsa_download_ssu_lnb_power_t;

typedef enum {
    HNSA_DOWNLOAD_SSU_DISEQC_POS_BURST_A = (1 << 0),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_BURST_B = (1 << 1),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_OPTION_0 = (1 << 2),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_OPTION_1 = (1 << 3),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_POLAR_V = (1 << 4),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_POLAR_H = (1 << 5),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_BAND_L = (1 << 6),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_BAND_H = (1 << 7),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_POSITION_0 = (1 << 8),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_POSITION_1 = (1 << 9),
    HNSA_DOWNLOAD_SSU_DISEQC_POS_LAST = (1 << 10)
} hnsa_download_ssu_diseqc_pos_t;

typedef struct
{
    char uri_file[HNSA_MAX_URI_SIZE];
} hnsa_download_http_s_params_t;

typedef struct
{
    uint32_t ssu_address;
    uint16_t ssu_port;
    uint16_t fec_port;
    hnsa_download_ssu_protocol_t ssu_protocol;
} hnsa_download_ssu_ip_t;

typedef struct
{
    uint32_t symbol_rate;
    hnsa_download_ssu_code_rate_t fec_inner;
    hnsa_download_ssu_modulation_t modcod;
    hnsa_download_ssu_rolloff_t rolloff;
    hnsa_download_ssu_lnb_power_t lnb_power;
    uint32_t diseqc_pos;
} hnsa_download_ssu_dvb_s_t;

typedef enum {
    HNSA_DOWNLOAD_PAPR_NONE = (1 << 0),
    HNSA_DOWNLOAD_PAPR_ACE = (1 << 1),
    HNSA_DOWNLOAD_PAPR_TR = (1 << 2),
    HNSA_DOWNLOAD_PAPR_ACE_TR = (1 << 3),
    HNSA_DOWNLOAD_PAPR_AUTO = (1 << 4),
    HNSA_DOWNLOAD_PAPR_LAST = (1 << 5)
} hnsa_download_ssu_papr_t;

typedef enum {
    HNSA_DOWNLOAD_PATTERN_PP1 = (1 << 0),
    HNSA_DOWNLOAD_PATTERN_PP2 = (1 << 1),
    HNSA_DOWNLOAD_PATTERN_PP3 = (1 << 2),
    HNSA_DOWNLOAD_PATTERN_PP4 = (1 << 3),
    HNSA_DOWNLOAD_PATTERN_PP5 = (1 << 4),
    HNSA_DOWNLOAD_PATTERN_PP6 = (1 << 5),
    HNSA_DOWNLOAD_PATTERN_PP7 = (1 << 6),
    HNSA_DOWNLOAD_PATTERN_PP8 = (1 << 7),
    HNSA_DOWNLOAD_PATTERN_AUTO = (1 << 8),
    HNSA_DOWNLOAD_PATTERN_LAST = (1 << 9)
} hnsa_download_ssu_pilot_pattern_t;

typedef enum {
    HNSA_DOWNLOAD_PREAMBLE_T2_SISO = (1 << 0),
    HNSA_DOWNLOAD_PREAMBLE_T2_MISO = (1 << 1),
    HNSA_DOWNLOAD_PREAMBLE_AUTO = (1 << 2),
    HNSA_DOWNLOAD_PREAMBLE_LAST = (1 << 3)
} hnsa_download_ssu_preamble_format_t;

typedef enum {
    HNSA_DOWNLOAD_FECFRAME_LENGTH_16200 = (1 << 0),
    HNSA_DOWNLOAD_FECFRAME_LENGTH_64800 = (1 << 1),
    HNSA_DOWNLOAD_FECFRAME_LENGTH_AUTO = (1 << 2),
    HNSA_DOWNLOAD_FECFRAME_LENGTH_LAST = (1 << 3)
} hnsa_download_ssu_fecframe_length_t;

typedef enum {
    HNSA_DOWNLOAD_INPUT_MODE_A = (1 << 0),
    HNSA_DOWNLOAD_INPUT_MODE_B = (1 << 1),
    HNSA_DOWNLOAD_INPUT_MODE_AUTO = (1 << 2),
    HNSA_DOWNLOAD_INPUT_MODE_LAST = (1 << 3)
} hnsa_download_ssu_input_mode_t;

typedef enum {
    HNSA_DOWNLOAD_TFS_USED_NO = (1 << 0),
    HNSA_DOWNLOAD_TFS_USED_YES = (1 << 1),
    HNSA_DOWNLOAD_TFS_USED_AUTO = (1 << 2),
    HNSA_DOWNLOAD_TFS_USED_LAST = (1 << 3)
} hnsa_download_ssu_tfs_used_t;

typedef enum {
    HNSA_DOWNLOAD_ADAPTATION_MODULE_NM = (1 << 0),
    HNSA_DOWNLOAD_ADAPTATION_MODULE_HEM = (1 << 1),
    HNSA_DOWNLOAD_ADAPTATION_MODULE_AUTO = (1 << 2),
    HNSA_DOWNLOAD_ADAPTATION_MODULE_LAST = (1 << 3)
} hnsa_download_ssu_adaptation_module_t;

typedef enum {
    HNSA_DOWNLOAD_EVENT_BL_DOWNLOAD_SUCCESS = (1 << 0),
    HNSA_DOWNLOAD_EVENT_BL_DOWNLOAD_FAILURE = (1 << 1),
    HNSA_DOWNLOAD_EVENT_BL_SEARCH_SUCCESS = (1 << 2),
    HNSA_DOWNLOAD_EVENT_BL_SEARCH_NOTHING_TO_DO = (1 << 3),
    HNSA_DOWNLOAD_EVENT_BL_SEARCH_FAILURE = (1 << 4),
    HNSA_DOWNLOAD_EVENT_BL_INSTALL_SUCCESS = (1 << 5),
    HNSA_DOWNLOAD_EVENT_BL_INSTALL_FAILURE = (1 << 6),
    HNSA_DOWNLOAD_EVENT_BL_START_WRITING = (1 << 7),
    HNSA_DOWNLOAD_EVENT_BL_STOP_WRITING = (1 << 8),
    HNSA_DOWNLOAD_EVENT_BL_RESOURCES_NEEDED = (1 << 9),
    HNSA_DOWNLOAD_EVENT_BL_RESOURCES_FREED = (1 << 10),
    HNSA_DOWNLOAD_EVENT_BL_COUNTRY_REQUIRED = (1 << 11),
    HNSA_DOWNLOAD_EVENT_TYPE_LAST = (1 << 12)
} hnsa_download_event_type_t;

typedef enum {
    HNSA_DOWNLOAD_TX_TYPE_TS = (1 << 0),
    HNSA_DOWNLOAD_TX_TYPE_GS = (1 << 1),
    HNSA_DOWNLOAD_TX_TYPE_TS_GS = (1 << 2),
    HNSA_DOWNLOAD_TX_TYPE_LAST = (1 << 3)
} hnsa_download_tx_type_t;

typedef enum {
    HNSA_DOWNLOAD_BL_SRC_DVB_S = (1 << 0),
    HNSA_DOWNLOAD_BL_SRC_DVB_T = (1 << 1),
    HNSA_DOWNLOAD_BL_SRC_DVB_C = (1 << 2),
    HNSA_DOWNLOAD_BL_SRC_IP = (1 << 3),
    HNSA_DOWNLOAD_BL_SRC_USB = (1 << 4),
    HNSA_DOWNLOAD_BL_SRC_HDD = (1 << 5),
    HNSA_DOWNLOAD_BL_SRC_SYSTEM = (1 << 6),
    HNSA_DOWNLOAD_BL_SRC_LAST = (1 << 7)
} hnsa_download_bl_source_t;

typedef enum {
    HNSA_DOWNLOAD_WIFI_BAND_2_4_GHZ,
    HNSA_DOWNLOAD_WIFI_BAND_5_GHZ,
    HNSA_DOWNLOAD_WIFI_BAND_LAST
} hnsa_download_wifi_bands_t;

typedef struct
{
    hnsa_download_ssu_bandwidth_t bandwidth;
    hnsa_download_ssu_code_rate_t code_rate_HP;
    hnsa_download_ssu_code_rate_t code_rate_LP;
    hnsa_download_ssu_constellation_t constellation;
    hnsa_download_ssu_transmit_mode_t transmission_mode;
    hnsa_download_ssu_guard_interval_t guard_interval;
    hnsa_download_ssu_hierarchy_t hierarchy_information;
    hnsa_download_ssu_papr_t papr_reduction;
    hnsa_download_ssu_pilot_pattern_t pilot_pattern;
    hnsa_download_ssu_preamble_format_t format;
    hnsa_download_ssu_fecframe_length_t length;
    hnsa_download_ssu_input_mode_t input_mode;
    hnsa_download_ssu_tfs_used_t tfs_used;
    hnsa_download_ssu_adaptation_module_t adaptation_mode;
    hnsa_download_tx_type_t tx_type;
    uint8_t plp_id;
    uint16_t network_id;
    uint16_t t2_system_id;
} hnsa_download_ssu_dvb_t_t;

typedef struct
{
    uint32_t frequency;
    hnsa_download_dvb_type_t hnsa_download_dvb_type;
    hnsa_download_spectral_inversion_t inversion;
    union {
        hnsa_download_ssu_dvb_s_t dvb_s;
        hnsa_download_ssu_dvb_t_t dvb_t;
    } u_tuning_params;
} hnsa_download_ssu_dvb_s_s2_t_t;

typedef struct
{
    unsigned char oui[3];
    uint32_t ssu_hardware_version;
    uint32_t ssu_software_version;
    uint8_t nb_ssu_params;
    uint16_t onid[HNSA_SSU_MAX_PARAMS];
    uint16_t tsid[HNSA_SSU_MAX_PARAMS];
    uint16_t sid[HNSA_SSU_MAX_PARAMS];
    uint16_t pid[HNSA_SSU_MAX_PARAMS];
    hnsa_download_ssu_type_t type[HNSA_SSU_MAX_PARAMS];
    union {
        hnsa_download_ssu_ip_t download_ssu_ip_params;
        hnsa_download_ssu_dvb_s_s2_t_t download_ssu_dvb_s_s2_t_params;
    } u_download_type[HNSA_SSU_MAX_PARAMS];
} hnsa_download_ssu_params_t;

typedef enum {
    HNSA_DOWNLOAD_REQUESTED,
    HNSA_DOWNLOAD_NOT_REQUESTED,
    HNSA_DOWNLOAD_CHECK_REQUEST,
    HNSA_DOWNLOAD_REQUEST_LAST
} hnsa_download_request_t;

typedef enum {
    HNSA_DOWNLOAD_NOMINAL_1_PARAMS,
    HNSA_DOWNLOAD_NOMINAL_2_PARAMS,
    HNSA_DOWNLOAD_RESCUE_PARAMS,
    HNSA_DOWNLOAD_PARAMS_TYPE_LAST
} hnsa_download_params_type_t;

typedef enum {
    HNSA_DOWNLOAD_MODE_ALL,
    HNSA_DOWNLOAD_MODE_SSU,
    HNSA_DOWNLOAD_MODE_LAST
} hnsa_download_mode_t;

typedef struct
{
    hnsa_download_type_t download_type;
    union {
        hnsa_download_tftp_params_t download_tftp_params;
        hnsa_download_ssu_params_t download_ssu_params;
        hnsa_download_http_s_params_t download_http_s_params;
    } u_download_type;
} hnsa_download_protocol_image_t;

typedef struct
{
    char id_image[HNSA_MAX_ID_IMAGE];
    uint8_t nb_protocol_preference;
    hnsa_download_protocol_image_t protocol_image[HNSA_DOWNLOAD_MAX_PREFERENCE];
} hnsa_download_settings_image_t;

typedef struct
{
    hnsa_download_mode_t mode;
    hnsa_download_params_type_t params_type;
    hnsa_download_request_t download_request;
    char ntp_server_address[HNSA_DOWNLOAD_MAX_ADDRESS_SIZE];
    char timeoffset[30];
    char country[2];
    uint8_t nb_images;
    hnsa_download_settings_image_t settings_images[HNSA_DOWNLOAD_MAX_IMAGE];
    char dhcp_option_77[HNSA_DOWNLOAD_MAX_ADDRESS_SIZE];
    uint8_t wifi_state;
    char wifi_ssid[HNSA_DOWNLOAD_MAX_WIFI_SSID];
    char wifi_key[HNSA_DOWNLOAD_MAX_WIFI_KEY];
    hnsa_download_wifi_bands_t frequency_band;
} hnsa_download_params_t;

typedef struct
{
    uint32_t type;
} hnsa_download_capability_t;

typedef struct
{
    char download_device_version[32];
} hnsa_download_version_t;

typedef struct
{
    char id_image[HNSA_MAX_ID_IMAGE];
    char version[HNSA_DOWNLOAD_DEF_IMAGE];
    hnsa_download_bl_source_t bl_src_type;
} hnsa_download_def_image_t;

typedef struct
{
    uint8_t nb_images;
    hnsa_download_def_image_t def_image[HNSA_DOWNLOAD_MAX_IMAGE];
} hnsa_download_id_image_t;

typedef struct
{
    char id_image[HNSA_MAX_ID_IMAGE];
    hnsa_download_type_t last_protocol_used;
    hnsa_download_status_t download_status;
    char extended_status[HNSA_DOWNLOAD_MAX_STATUS];
} hnsa_download_status_image_t;

typedef struct
{
    uint8_t nb_images;
    hnsa_download_status_image_t status_all_images[HNSA_DOWNLOAD_MAX_IMAGE];
} hnsa_download_report_t;

typedef struct
{
    char fe_path[HNSA_DOWNLOAD_MAX_ADDRESS_SIZE];
    char dmx_path[HNSA_DOWNLOAD_MAX_ADDRESS_SIZE];
} hnsa_download_bl_resources_t;

typedef struct
{
    hnsa_download_event_type_t event_type;
    struct timeval timestamp;
    char version_image_on_hdd[64];
    char unt_time[32];
} hnsa_download_event_t;

typedef struct
{
    char country[HNSA_DOWNLOAD_MAX_COUNTRY_SIZE];
} hnsa_download_bl_country_t;

/*******************************************************************************
  IOCTL VALUES
*******************************************************************************/

#define HNSA_DOWNLOAD_READ_PARAMS _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 1, hnsa_download_params_t)
#define HNSA_DOWNLOAD_WRITE_PARAMS _IOWR(HNSA_DOWNLOAD_MAGIC_NUMBER, 2, hnsa_download_params_t)
#define HNSA_DOWNLOAD_READ_PRIV_DATA _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 3, uint32_t)
#define HNSA_DOWNLOAD_WRITE_PRIV_DATA _IOWR(HNSA_DOWNLOAD_MAGIC_NUMBER, 4, uint32_t)
#define HNSA_DOWNLOAD_GET_CAPABILITIES _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 5, hnsa_download_capability_t)
#define HNSA_DOWNLOAD_ACTIVATE_LOADER _IO(HNSA_DOWNLOAD_MAGIC_NUMBER, 6)
#define HNSA_DOWNLOAD_GET_VERSION _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 7, hnsa_download_version_t)
#define HNSA_DOWNLOAD_GET_ID_IMAGE _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 8, hnsa_download_id_image_t)
#define HNSA_DOWNLOAD_GET_REPORT _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 9, hnsa_download_report_t)
#define HNSA_DOWNLOAD_BL_ALLOC_RESOURCES _IOWR(HNSA_DOWNLOAD_MAGIC_NUMBER, 10, hnsa_download_bl_resources_t)
#define HNSA_DOWNLOAD_BL_FREE_RESOURCES _IO(HNSA_DOWNLOAD_MAGIC_NUMBER, 11)
#define HNSA_DOWNLOAD_GET_EVENT _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 12, hnsa_download_event_t)
#define HNSA_DOWNLOAD_BL_SEARCH_AVAILABLE_ID_IMAGE _IO(HNSA_DOWNLOAD_MAGIC_NUMBER, 13)
#define HNSA_DOWNLOAD_BL_SET_AUTOSEARCH_PERIODICITY _IOWR(HNSA_DOWNLOAD_MAGIC_NUMBER, 14, uint32_t)
#define HNSA_DOWNLOAD_BL_GET_AUTOSEARCH_PERIODICITY _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 15, uint32_t)
#define HNSA_DOWNLOAD_GET_AVAILABLE_ID_IMAGE _IOR(HNSA_DOWNLOAD_MAGIC_NUMBER, 16, hnsa_download_id_image_t)
#define HNSA_DOWNLOAD_INSTALL_ID_IMAGE _IOWR(HNSA_DOWNLOAD_MAGIC_NUMBER, 17, hnsa_download_id_image_t)
#define HNSA_DOWNLOAD_RESET_REPORT _IO(HNSA_DOWNLOAD_MAGIC_NUMBER, 18)
#define HNSA_DOWNLOAD_BL_SET_COUNTRY _IOWR(HNSA_DOWNLOAD_MAGIC_NUMBER, 19, hnsa_download_bl_country_t)
#define HNSA_DOWNLOAD_SET_BOOT_COUNTER _IOWR(HNSA_DOWNLOAD_MAGIC_NUMBER, 20, uint32_t)
#endif /*_HNSA_DOWNLOAD_H_*/
