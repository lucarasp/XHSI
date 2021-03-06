/*
 * commands.c
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define XPLM200 1

#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMNavigation.h"
#include "XPLMDisplay.h"
#include "XPLMMenus.h"
//#include "XPWidgets.h"
//#include "XPStandardWidgets.h"


#include "structs.h"
#include "datarefs.h"
#include "datarefs_qpac.h"
#include "datarefs_x737.h"
#include "commands.h"


#define UP -99
#define DOWN -1
#define SHUTTLE -22
#define TOGGLE -333
#define CYCLE -4444
#define ON 1
#define OFF 0

#define B737CL_MAP 73701
#define B737CL_CTR_MAP 73702
#define B737CL_EXP_VOR_ILS 73703
#define B737CL_FULL_VOR_ILS 73704
#define B737CL_PLAN 73705

#define EFIS_PILOT 0
#define EFIS_COPILOT 1

#define MODE_APP 0
#define MODE_VOR 1
#define MODE_MAP 2
#define MODE_NAV 3
#define MODE_PLN 4

#define MODE_CENTERED 0
#define MODE_EXPANDED 1

#define ZOOMIN_OFF 0
#define ZOOMIN_ON 1

#define RANGE_10 0
#define RANGE_20 1
#define RANGE_40 2
#define RANGE_80 3
#define RANGE_160 4
#define RANGE_320 5
#define RANGE_640 6

#define RADIO_ADF 0
#define RADIO_OFF 1
#define RADIO_NAV 2

#define SOURCE_NAV1 0
#define SOURCE_NAV2 1
#define SOURCE_FMC 2

#define MINS_RADIO 0
#define MINS_BARO 1

#define MFD_ARPT 0
#define MFD_FPLN 1
#define MFD_RTU 2
#define MFD_EICAS 3
#define MFD_BLEED 4
#define MFD_CAB_PRESS 5
#define MFD_ELEC 6
#define MFD_HYDR 7
#define MFD_FUEL 8
#define MFD_APU 9
#define MFD_COND 10
#define MFD_DOOR_OXY 11
#define MFD_WHEELS 12
#define MFD_FCTL 13
#define MFD_SYS 14
#define MFD_STATUS 15
#define MFD_DEBUG 31
#define MFD_RESET_FUEL_USED 32


#define CDU_PILOT_CUSTOM 0
#define CDU_PILOT_XFMC 1
#define CDU_COPILOT_CUSTOM 2
#define CDU_COPILOT_XFMC 3

#define RTU_SELECT_NONE 0
#define RTU_SELECT_COM1 1
#define RTU_SELECT_NAV1 2
#define RTU_SELECT_ADF1 3
#define RTU_SELECT_XPDR 4
#define RTU_SELECT_TCAS 5
#define RTU_SELECT_ADF2 6
#define RTU_SELECT_NAV2 7
#define RTU_SELECT_COM2 8
#define RTU_SELECT_NEXT +111
#define RTU_SELECT_PREV -111
#define RTU_COARSE_DOWN -1000
#define RTU_COARSE_UP +1000
#define RTU_FINE_DOWN -999
#define RTU_FINE_UP +999
#define RTU_FLIP_SELECTED 10
#define RTU_SELECT_FLIP_COM1 11
#define RTU_SELECT_FLIP_NAV1 12
#define RTU_SELECT_FLIP_ADF1 13
#define RTU_SELECT_FLIP_XPDR 14
#define RTU_SELECT_FLIP_TCAS 15
#define RTU_SELECT_FLIP_ADF2 16
#define RTU_SELECT_FLIP_NAV2 17
#define RTU_SELECT_FLIP_COM2 18

#define XPDR_MODE_UP +1
#define XPDR_MODE_DOWN -1
#define XPDR_MODE_CYCLE 0
#define XPDR_CODE_HUNDREDS_UP +100
#define XPDR_CODE_HUNDREDS_DOWN -100
#define XPDR_CODE_UNITS_UP +99
#define XPDR_CODE_UNITS_DOWN -99

#define ADF1 10000
#define ADF1_STBY 19000
#define ADF2 20000
#define ADF2_STBY 29000
#define ADF_HUNDREDS_UP 100
#define ADF_HUNDREDS_DOWN 900
#define ADF_UNITS_UP 1
#define ADF_UNITS_DOWN 9

#define WXR_MODE_OFF 0
#define WXR_MODE_ON 1
#define WXR_MODE_TURB 2
#define WXR_MODE_MAP 3
#define WXR_MODE_FORCE_ON 4

#define CLK_START_STOP_RESET 0
#define CLK_START_STOP 1
#define CLK_RESET 2
#define CLK_ET_STOP 3
#define CLK_ET_RUN 4
#define CLK_ET_RESET 5
#define CLK_UTC_GPS 6
#define CLK_UTC_INT 7
#define CLK_UTC_SET 8

XPLMCommandRef mode_app;
XPLMCommandRef mode_vor;
XPLMCommandRef mode_map;
XPLMCommandRef mode_nav;
XPLMCommandRef mode_pln;
XPLMCommandRef mode_down;
XPLMCommandRef mode_up;
XPLMCommandRef mode_cycle;
XPLMCommandRef mode_shuttle;

XPLMCommandRef b737cl_mode_map;
XPLMCommandRef b737cl_mode_ctrmap;
XPLMCommandRef b737cl_mode_expvorils;
XPLMCommandRef b737cl_mode_fullvorils;
XPLMCommandRef b737cl_mode_plan;

XPLMCommandRef ctr_toggle;
XPLMCommandRef ctr_on;
XPLMCommandRef ctr_off;

XPLMCommandRef range_10;
XPLMCommandRef range_20;
XPLMCommandRef range_40;
XPLMCommandRef range_80;
XPLMCommandRef range_160;
XPLMCommandRef range_320;
XPLMCommandRef range_640;
XPLMCommandRef range_down;
XPLMCommandRef range_up;
XPLMCommandRef range_cycle;
XPLMCommandRef range_shuttle;

XPLMCommandRef ext_range_0_10;
XPLMCommandRef ext_range_0_20;
XPLMCommandRef ext_range_0_40;
XPLMCommandRef ext_range_0_80;
XPLMCommandRef ext_range_1_60;
XPLMCommandRef ext_range_3_20;
XPLMCommandRef ext_range_6_40;
XPLMCommandRef ext_range_010;
XPLMCommandRef ext_range_020;
XPLMCommandRef ext_range_040;
XPLMCommandRef ext_range_080;
XPLMCommandRef ext_range_160;
XPLMCommandRef ext_range_320;
XPLMCommandRef ext_range_640;
XPLMCommandRef ext_range_down;
XPLMCommandRef ext_range_up;
XPLMCommandRef ext_range_cycle;
XPLMCommandRef ext_range_shuttle;

XPLMCommandRef zoom_x100_on;
XPLMCommandRef zoom_x100_off;
XPLMCommandRef zoom_x100_toggle;

XPLMCommandRef radio1_adf;
XPLMCommandRef radio1_off;
XPLMCommandRef radio1_nav;
XPLMCommandRef radio1_down;
XPLMCommandRef radio1_up;
//XPLMCommandRef radio1_cycle;
XPLMCommandRef radio1_shuttle;

XPLMCommandRef radio2_adf;
XPLMCommandRef radio2_off;
XPLMCommandRef radio2_nav;
XPLMCommandRef radio2_down;
XPLMCommandRef radio2_up;
//XPLMCommandRef radio2_cycle;
XPLMCommandRef radio2_shuttle;

XPLMCommandRef source_nav1;
XPLMCommandRef source_nav2;
XPLMCommandRef source_fmc;
XPLMCommandRef source_down;
XPLMCommandRef source_up;
XPLMCommandRef source_cycle;

XPLMCommandRef tfc_toggle;
XPLMCommandRef tfc_on;
XPLMCommandRef tfc_off;

XPLMCommandRef arpt_toggle;
XPLMCommandRef arpt_on;
XPLMCommandRef arpt_off;

XPLMCommandRef wpt_toggle;
XPLMCommandRef wpt_on;
XPLMCommandRef wpt_off;

XPLMCommandRef vor_toggle;
XPLMCommandRef vor_on;
XPLMCommandRef vor_off;

XPLMCommandRef ndb_toggle;
XPLMCommandRef ndb_on;
XPLMCommandRef ndb_off;

XPLMCommandRef sta_toggle;
XPLMCommandRef sta_on;
XPLMCommandRef sta_off;
XPLMCommandRef sta_cycle;

XPLMCommandRef declutter_cycle;

XPLMCommandRef data_toggle;
XPLMCommandRef data_on;
XPLMCommandRef data_off;

XPLMCommandRef pos_toggle;
XPLMCommandRef pos_on;
XPLMCommandRef pos_off;

XPLMCommandRef terrain_toggle;
XPLMCommandRef terrain_on;
XPLMCommandRef terrain_off;

XPLMCommandRef vp_toggle;
XPLMCommandRef vp_on;
XPLMCommandRef vp_off;

XPLMCommandRef ils_toggle;
XPLMCommandRef ils_on;
XPLMCommandRef ils_off;

XPLMCommandRef metric_alt_toggle;
XPLMCommandRef metric_alt_on;
XPLMCommandRef metric_alt_off;

XPLMCommandRef track_fpa_toggle;
XPLMCommandRef track_fpa_on;
XPLMCommandRef track_fpa_off;

XPLMCommandRef wxr_toggle;
XPLMCommandRef wxr_on;
XPLMCommandRef wxr_off;

XPLMCommandRef wxr_gain_up;
XPLMCommandRef wxr_gain_down;
XPLMCommandRef wxr_gain_middle;

XPLMCommandRef wxr_tilt_up;
XPLMCommandRef wxr_tilt_down;
XPLMCommandRef wxr_tilt_zero;

XPLMCommandRef wxr_auto_gain_toggle;
XPLMCommandRef wxr_auto_gain_on;
XPLMCommandRef wxr_auto_gain_off;

XPLMCommandRef wxr_auto_tilt_toggle;
XPLMCommandRef wxr_auto_tilt_on;
XPLMCommandRef wxr_auto_tilt_off;

XPLMCommandRef wxr_test_toggle;
XPLMCommandRef wxr_test_on;
XPLMCommandRef wxr_test_off;

XPLMCommandRef wxr_mode_up;
XPLMCommandRef wxr_mode_down;
XPLMCommandRef wxr_mode_off;
XPLMCommandRef wxr_mode_on;
XPLMCommandRef wxr_mode_turb;
XPLMCommandRef wxr_mode_map;
XPLMCommandRef wxr_mode_force_on;

XPLMCommandRef wxr_slave_toggle;
XPLMCommandRef wxr_slave_on;
XPLMCommandRef wxr_slave_off;

XPLMCommandRef wxr_react_toggle;
XPLMCommandRef wxr_react_on;
XPLMCommandRef wxr_react_off;

XPLMCommandRef wxr_narrow_toggle;
XPLMCommandRef wxr_narrow_on;
XPLMCommandRef wxr_narrow_off;

XPLMCommandRef wxr_alert_toggle;
XPLMCommandRef wxr_alert_on;
XPLMCommandRef wxr_alert_off;

XPLMCommandRef wxr_target_toggle;
XPLMCommandRef wxr_target_on;
XPLMCommandRef wxr_target_off;

XPLMCommandRef mins_toggle;
XPLMCommandRef mins_radio;
XPLMCommandRef mins_baro;

XPLMCommandRef mins_reset;
XPLMCommandRef mins_down;
XPLMCommandRef mins_up;


XPLMCommandRef copilot_mode_app;
XPLMCommandRef copilot_mode_vor;
XPLMCommandRef copilot_mode_map;
XPLMCommandRef copilot_mode_nav;
XPLMCommandRef copilot_mode_pln;
XPLMCommandRef copilot_mode_down;
XPLMCommandRef copilot_mode_up;
XPLMCommandRef copilot_mode_cycle;
XPLMCommandRef copilot_mode_shuttle;

XPLMCommandRef b737cl_copilot_mode_map;
XPLMCommandRef b737cl_copilot_mode_ctrmap;
XPLMCommandRef b737cl_copilot_mode_expvorils;
XPLMCommandRef b737cl_copilot_mode_fullvorils;
XPLMCommandRef b737cl_copilot_mode_plan;

XPLMCommandRef copilot_ctr_toggle;
XPLMCommandRef copilot_ctr_on;
XPLMCommandRef copilot_ctr_off;

XPLMCommandRef copilot_range_10;
XPLMCommandRef copilot_range_20;
XPLMCommandRef copilot_range_40;
XPLMCommandRef copilot_range_80;
XPLMCommandRef copilot_range_160;
XPLMCommandRef copilot_range_320;
XPLMCommandRef copilot_range_640;
XPLMCommandRef copilot_range_down;
XPLMCommandRef copilot_range_up;
XPLMCommandRef copilot_range_cycle;
XPLMCommandRef copilot_range_shuttle;

XPLMCommandRef copilot_ext_range_0_10;
XPLMCommandRef copilot_ext_range_0_20;
XPLMCommandRef copilot_ext_range_0_40;
XPLMCommandRef copilot_ext_range_0_80;
XPLMCommandRef copilot_ext_range_1_60;
XPLMCommandRef copilot_ext_range_3_20;
XPLMCommandRef copilot_ext_range_6_40;
XPLMCommandRef copilot_ext_range_010;
XPLMCommandRef copilot_ext_range_020;
XPLMCommandRef copilot_ext_range_040;
XPLMCommandRef copilot_ext_range_080;
XPLMCommandRef copilot_ext_range_160;
XPLMCommandRef copilot_ext_range_320;
XPLMCommandRef copilot_ext_range_640;
XPLMCommandRef copilot_ext_range_down;
XPLMCommandRef copilot_ext_range_up;
XPLMCommandRef copilot_ext_range_cycle;
XPLMCommandRef copilot_ext_range_shuttle;

XPLMCommandRef copilot_zoom_x100_on;
XPLMCommandRef copilot_zoom_x100_off;
XPLMCommandRef copilot_zoom_x100_toggle;

XPLMCommandRef copilot_radio1_adf;
XPLMCommandRef copilot_radio1_off;
XPLMCommandRef copilot_radio1_nav;
XPLMCommandRef copilot_radio1_down;
XPLMCommandRef copilot_radio1_up;
//XPLMCommandRef copilot_radio1_cycle;
XPLMCommandRef copilot_radio1_shuttle;

XPLMCommandRef copilot_radio2_adf;
XPLMCommandRef copilot_radio2_off;
XPLMCommandRef copilot_radio2_nav;
XPLMCommandRef copilot_radio2_down;
XPLMCommandRef copilot_radio2_up;
//XPLMCommandRef copilot_radio2_cycle;
XPLMCommandRef copilot_radio2_shuttle;

XPLMCommandRef copilot_source_nav1;
XPLMCommandRef copilot_source_nav2;
XPLMCommandRef copilot_source_fmc;
XPLMCommandRef copilot_source_down;
XPLMCommandRef copilot_source_up;
XPLMCommandRef copilot_source_cycle;

XPLMCommandRef copilot_tfc_toggle;
XPLMCommandRef copilot_tfc_on;
XPLMCommandRef copilot_tfc_off;

XPLMCommandRef copilot_arpt_toggle;
XPLMCommandRef copilot_arpt_on;
XPLMCommandRef copilot_arpt_off;

XPLMCommandRef copilot_wpt_toggle;
XPLMCommandRef copilot_wpt_on;
XPLMCommandRef copilot_wpt_off;

XPLMCommandRef copilot_vor_toggle;
XPLMCommandRef copilot_vor_on;
XPLMCommandRef copilot_vor_off;

XPLMCommandRef copilot_ndb_toggle;
XPLMCommandRef copilot_ndb_on;
XPLMCommandRef copilot_ndb_off;

XPLMCommandRef copilot_sta_toggle;
XPLMCommandRef copilot_sta_on;
XPLMCommandRef copilot_sta_off;
XPLMCommandRef copilot_sta_cycle;

XPLMCommandRef copilot_declutter_cycle;

XPLMCommandRef copilot_data_toggle;
XPLMCommandRef copilot_data_on;
XPLMCommandRef copilot_data_off;

XPLMCommandRef copilot_pos_toggle;
XPLMCommandRef copilot_pos_on;
XPLMCommandRef copilot_pos_off;

XPLMCommandRef copilot_terrain_toggle;
XPLMCommandRef copilot_terrain_on;
XPLMCommandRef copilot_terrain_off;

XPLMCommandRef copilot_vp_toggle;
XPLMCommandRef copilot_vp_on;
XPLMCommandRef copilot_vp_off;

XPLMCommandRef copilot_ils_toggle;
XPLMCommandRef copilot_ils_on;
XPLMCommandRef copilot_ils_off;

XPLMCommandRef copilot_metric_alt_toggle;
XPLMCommandRef copilot_metric_alt_on;
XPLMCommandRef copilot_metric_alt_off;

XPLMCommandRef copilot_track_fpa_toggle;
XPLMCommandRef copilot_track_fpa_on;
XPLMCommandRef copilot_track_fpa_off;

XPLMCommandRef copilot_wxr_toggle;
XPLMCommandRef copilot_wxr_on;
XPLMCommandRef copilot_wxr_off;

XPLMCommandRef copilot_wxr_gain_up;
XPLMCommandRef copilot_wxr_gain_down;
XPLMCommandRef copilot_wxr_gain_middle;

XPLMCommandRef copilot_wxr_tilt_up;
XPLMCommandRef copilot_wxr_tilt_down;
XPLMCommandRef copilot_wxr_tilt_zero;

XPLMCommandRef copilot_wxr_auto_gain_toggle;
XPLMCommandRef copilot_wxr_auto_gain_on;
XPLMCommandRef copilot_wxr_auto_gain_off;

XPLMCommandRef copilot_wxr_auto_tilt_toggle;
XPLMCommandRef copilot_wxr_auto_tilt_on;
XPLMCommandRef copilot_wxr_auto_tilt_off;

XPLMCommandRef copilot_wxr_test_toggle;
XPLMCommandRef copilot_wxr_test_on;
XPLMCommandRef copilot_wxr_test_off;

XPLMCommandRef copilot_wxr_mode_up;
XPLMCommandRef copilot_wxr_mode_down;
XPLMCommandRef copilot_wxr_mode_off;
XPLMCommandRef copilot_wxr_mode_on;
XPLMCommandRef copilot_wxr_mode_turb;
XPLMCommandRef copilot_wxr_mode_map;
XPLMCommandRef copilot_wxr_mode_force_on;

XPLMCommandRef copilot_wxr_slave_toggle;
XPLMCommandRef copilot_wxr_slave_on;
XPLMCommandRef copilot_wxr_slave_off;

XPLMCommandRef copilot_wxr_react_toggle;
XPLMCommandRef copilot_wxr_react_on;
XPLMCommandRef copilot_wxr_react_off;

XPLMCommandRef copilot_wxr_narrow_toggle;
XPLMCommandRef copilot_wxr_narrow_on;
XPLMCommandRef copilot_wxr_narrow_off;

XPLMCommandRef copilot_wxr_alert_toggle;
XPLMCommandRef copilot_wxr_alert_on;
XPLMCommandRef copilot_wxr_alert_off;

XPLMCommandRef copilot_wxr_target_toggle;
XPLMCommandRef copilot_wxr_target_on;
XPLMCommandRef copilot_wxr_target_off;

XPLMCommandRef copilot_mins_toggle;
XPLMCommandRef copilot_mins_radio;
XPLMCommandRef copilot_mins_baro;

XPLMCommandRef copilot_mins_reset;
XPLMCommandRef copilot_mins_down;
XPLMCommandRef copilot_mins_up;

XPLMCommandRef nav1_sync;
XPLMCommandRef nav2_sync;

XPLMCommandRef auto_range_pilot;
XPLMCommandRef auto_range_copilot;

XPLMCommandRef auto_ext_range_pilot;
XPLMCommandRef auto_ext_range_copilot;

// Clock & Chrono
XPLMCommandRef chr_start_stop_reset;
XPLMCommandRef chr_start_stop;
XPLMCommandRef chr_reset;

XPLMCommandRef timer_start_stop;
XPLMCommandRef timer_reset;

XPLMCommandRef et_reset;
XPLMCommandRef et_stop;
XPLMCommandRef et_run;

XPLMCommandRef clock_utc_gps;
XPLMCommandRef clock_utc_int;
XPLMCommandRef clock_utc_set;

XPLMCommandRef pilot_chrono_stop_reset;
XPLMCommandRef pilot_chrono_start_stop;
XPLMCommandRef pilot_chrono_reset;
XPLMCommandRef copilot_chrono_stop_reset;
XPLMCommandRef copilot_chrono_start_stop;
XPLMCommandRef copilot_chrono_reset;

XPLMCommandRef mfd_mode_arpt;
XPLMCommandRef mfd_mode_fpln;
XPLMCommandRef mfd_mode_rtu;
XPLMCommandRef mfd_mode_eicas;
XPLMCommandRef mfd_mode_bleed;
XPLMCommandRef mfd_mode_cab_press;
XPLMCommandRef mfd_mode_elec;
XPLMCommandRef mfd_mode_hydr;
XPLMCommandRef mfd_mode_fuel;
XPLMCommandRef mfd_mode_apu;
XPLMCommandRef mfd_mode_cond;
XPLMCommandRef mfd_mode_door_oxy;
XPLMCommandRef mfd_mode_wheel;
XPLMCommandRef mfd_mode_fctl;
XPLMCommandRef mfd_mode_sys;
XPLMCommandRef mfd_mode_status;
XPLMCommandRef mfd_mode_debug;
XPLMCommandRef mfd_mode_down;
XPLMCommandRef mfd_mode_up;
XPLMCommandRef mfd_mode_cycle;
XPLMCommandRef mfd_reset_fuel_used;

// CDU
XPLMCommandRef cdu_pilot_custom;
XPLMCommandRef cdu_pilot_xfmc;
XPLMCommandRef cdu_copilot_custom;
XPLMCommandRef cdu_copilot_xfmc;

// for an RTU
XPLMCommandRef nav1_standy_flip;
XPLMCommandRef nav2_standy_flip;
XPLMCommandRef com1_standy_flip;
XPLMCommandRef com2_standy_flip;
XPLMCommandRef adf1_standy_flip;
XPLMCommandRef adf2_standy_flip;
XPLMCommandRef sim_transponder_transponder_ident;
//XPLMCommandRef contact_atc_cmd;
XPLMCommandRef stby_com1_coarse_down;
XPLMCommandRef stby_com1_coarse_up;
XPLMCommandRef stby_com1_fine_down_833;
XPLMCommandRef stby_com1_fine_up_833;
XPLMCommandRef stby_com2_coarse_down;
XPLMCommandRef stby_com2_coarse_up;
XPLMCommandRef stby_com2_fine_down_833;
XPLMCommandRef stby_com2_fine_up_833;
XPLMCommandRef stby_nav1_coarse_down;
XPLMCommandRef stby_nav1_coarse_up;
XPLMCommandRef stby_nav1_fine_down;
XPLMCommandRef stby_nav1_fine_up;
XPLMCommandRef stby_nav2_coarse_down;
XPLMCommandRef stby_nav2_coarse_up;
XPLMCommandRef stby_nav2_fine_down;
XPLMCommandRef stby_nav2_fine_up;

XPLMCommandRef rtu_select_none;
XPLMCommandRef rtu_select_com1;
XPLMCommandRef rtu_select_nav1;
XPLMCommandRef rtu_select_adf1;
XPLMCommandRef rtu_select_xpdr;
XPLMCommandRef rtu_select_tcas;
XPLMCommandRef rtu_select_adf2;
XPLMCommandRef rtu_select_nav2;
XPLMCommandRef rtu_select_com2;
XPLMCommandRef rtu_select_flip_com1;
XPLMCommandRef rtu_select_flip_nav1;
XPLMCommandRef rtu_select_flip_adf1;
XPLMCommandRef rtu_select_flip_xpdr;
XPLMCommandRef rtu_select_flip_tcas;
XPLMCommandRef rtu_select_flip_adf2;
XPLMCommandRef rtu_select_flip_nav2;
XPLMCommandRef rtu_select_flip_com2;
XPLMCommandRef rtu_select_next;
XPLMCommandRef rtu_select_prev;
XPLMCommandRef rtu_flip_selected;
XPLMCommandRef rtu_coarse_down;
XPLMCommandRef rtu_coarse_up;
XPLMCommandRef rtu_fine_down;
XPLMCommandRef rtu_fine_up;

XPLMCommandRef xpdr_mode_up;
XPLMCommandRef xpdr_mode_down;
XPLMCommandRef xpdr_mode_cycle;

XPLMCommandRef xpdr_code_hundreds_up;
XPLMCommandRef xpdr_code_hundreds_down;
XPLMCommandRef xpdr_code_units_up;
XPLMCommandRef xpdr_code_units_down;

XPLMCommandRef adf1_hundreds_up;
XPLMCommandRef adf1_hundreds_down;
XPLMCommandRef adf1_units_up;
XPLMCommandRef adf1_units_down;
XPLMCommandRef adf1_stby_hundreds_up;
XPLMCommandRef adf1_stby_hundreds_down;
XPLMCommandRef adf1_stby_units_up;
XPLMCommandRef adf1_stby_units_down;
XPLMCommandRef adf2_hundreds_up;
XPLMCommandRef adf2_hundreds_down;
XPLMCommandRef adf2_units_up;
XPLMCommandRef adf2_units_down;
XPLMCommandRef adf2_stby_hundreds_up;
XPLMCommandRef adf2_stby_hundreds_down;
XPLMCommandRef adf2_stby_units_up;
XPLMCommandRef adf2_stby_units_down;


// for an MCP
XPLMCommandRef sim_autopilot_fdir_servos_toggle;
XPLMCommandRef sim_autopilot_autothrottle_toggle;
XPLMCommandRef sim_autopilot_level_change;
XPLMCommandRef sim_autopilot_heading;
XPLMCommandRef sim_autopilot_vertical_speed;
XPLMCommandRef sim_autopilot_nav;
XPLMCommandRef sim_autopilot_approach;
XPLMCommandRef sim_autopilot_glide_slope;
XPLMCommandRef sim_autopilot_back_course;
XPLMCommandRef sim_autopilot_altitude_hold;
XPLMCommandRef sim_autopilot_wing_leveler;

// for Clock
XPLMCommandRef sim_instruments_timer_show_date;

// lights
XPLMCommandRef sim_lights_nav_lights_toggle;
XPLMCommandRef sim_lights_beacon_lights_toggle;
XPLMCommandRef sim_lights_taxi_lights_toggle;
XPLMCommandRef sim_lights_strobe_lights_toggle;
XPLMCommandRef sim_lights_landing_lights_toggle;

// flaps, gear and speedbrake
XPLMCommandRef sim_flight_controls_flaps_down;
XPLMCommandRef sim_flight_controls_flaps_up;
XPLMCommandRef sim_flight_controls_landing_gear_toggle;
XPLMCommandRef sim_flight_controls_landing_gear_up;
XPLMCommandRef sim_flight_controls_landing_gear_down;
XPLMCommandRef sim_flight_controls_speed_brakes_down_one;
XPLMCommandRef sim_flight_controls_speed_brakes_up_one;

//x737 mcp toggles
XPLMCommandRef x737_cmda_toggle;
XPLMCommandRef x737_mcpspd_toggle;
XPLMCommandRef x737_lvlchange_toggle;
XPLMCommandRef x737_hdgsel_toggle;
XPLMCommandRef x737_lnav_toggle;
XPLMCommandRef x737_vorloc_toggle;
XPLMCommandRef x737_app_toggle;
XPLMCommandRef x737_althld_toggle;
XPLMCommandRef x737_vs_toggle;

// Master Warnings, Caution, Accept
XPLMCommandRef sim_annunciator_clear_master_warning;
XPLMCommandRef sim_annunciator_clear_master_caution;
XPLMCommandRef sim_annunciator_clear_master_accept;

// FMS Keys
XPLMCommandRef sim_fms[SIM_FMS_KEY_MAX];

char debug_string[80];


// ctr
XPLMCommandCallback_f ctr_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_map_mode);
        XPLMSetDatai(efis_map_mode, i);
    }
    return (XPLMCommandCallback_f)1;
}

// zoomin
XPLMCommandCallback_f zoom_x100_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_map_zoomin);
        XPLMSetDatai(efis_pilot_map_zoomin, i);
    }
    return (XPLMCommandCallback_f)1;
}

// range
XPLMCommandCallback_f range_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;
    int range_max = qpac_ready ? 5 : 6;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_map_range_selector) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_map_range_selector) + 1;
            if (i>range_max) i = range_max;
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(efis_map_range_selector) + 1;
            if (i>range_max) i = 0;
        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_map_range_selector) + (shuttle_up ? +1 : -1);
            if (i>range_max)
            {
                i = range_max-1;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_map_range_selector, i);
        if (qpac_ready) XPLMSetDatai(qpac_capt_efis_nd_range,i);
    }
    return (XPLMCommandCallback_f)1;
}

// ext range
XPLMCommandCallback_f ext_range_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;
    int range_max = qpac_ready ? 5 : 6;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        int z = XPLMGetDatai(efis_pilot_map_zoomin);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_map_range_selector) - 1;
            if (i<0)
            {
                if ( z )
                {
                    i = 0;
                }
                else
                {
                    i = range_max;
                    z = 1;
                }
            }
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_map_range_selector) + 1;
            if (i>range_max)
            {
                if ( ! z )
                {
                    i = range_max;
                }
                else
                {
                    i = 0;
                    z = 0;
                }
            }
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(efis_map_range_selector) + 1;
            if (i>range_max)
            {
                i = 0;
                z = !z;
            }
        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_map_range_selector) + (shuttle_up ? +1 : -1);
            if (i>range_max)
            {
                if ( z )
                {
                    i = 0;
                    z = ! z;
                }
                else
                {
                    i = range_max-1;
                    shuttle_up = 0;
                }
            }
            else if (i<0)
            {
                if ( z )
                {
                    i = 1;
                    shuttle_up = 1;
                }
                else
                {
                    i = range_max;
                    z = ! z;;
                }
            }
        }
        if (i>=1100)
        {
            z = 1;
        }
        else if (i>=1000)
        {
            z = 0;
        }
        XPLMSetDatai(efis_map_range_selector, i % 10);
        XPLMSetDatai(efis_pilot_map_zoomin, z);
        if (qpac_ready) XPLMSetDatai(qpac_capt_efis_nd_range,i % 10);
    }
    return (XPLMCommandCallback_f)1;
}

// mode
XPLMCommandCallback_f mode_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;
    // Qpac override efis_map_submode with forced value = 2
    int nd_mode = qpac_ready ? XPLMGetDatai(qpac_capt_efis_nd_mode) : XPLMGetDatai(efis_map_submode);

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = nd_mode - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = nd_mode + 1;
            if (i>4) i = 4;
        }
        else if ( i == CYCLE )
        {
            i = nd_mode + 1;
            if (i>4) i = 0;
        }
        else if ( i == SHUTTLE )
        {
            i = nd_mode + (shuttle_up ? +1 : -1);
            if (i>4)
            {
                i = 3;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }

        if (qpac_ready) {
        	XPLMSetDatai(qpac_capt_efis_nd_mode,i);
        } else {
        	XPLMSetDatai(efis_map_submode, i);
        }
    }
    return (XPLMCommandCallback_f)1;
}

// B737-Classic modes
XPLMCommandCallback_f b737cl_mode_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);

        if ( i == B737CL_FULL_VOR_ILS )
        {
            XPLMSetDatai(efis_map_submode, MODE_APP);
            XPLMSetDatai(efis_map_mode, MODE_CENTERED);
        }
        else if ( i == B737CL_EXP_VOR_ILS )
        {
            XPLMSetDatai(efis_map_submode, MODE_APP);
            XPLMSetDatai(efis_map_mode, MODE_EXPANDED);
        }
        else if ( i == B737CL_MAP )
        {
            XPLMSetDatai(efis_map_submode, MODE_MAP);
            XPLMSetDatai(efis_map_mode, MODE_EXPANDED);
        }
        else if ( i == B737CL_CTR_MAP )
        {
            XPLMSetDatai(efis_map_submode, MODE_MAP);
            XPLMSetDatai(efis_map_mode, MODE_CENTERED);
        }
        else if ( i == B737CL_PLAN )
        {
            XPLMSetDatai(efis_map_submode, MODE_PLN);
        }
    }
    return (XPLMCommandCallback_f)1;
}

// radio1
XPLMCommandCallback_f radio1_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_dme_1_selector) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_dme_1_selector) + 1;
            if (i>2) i = 2;
        }
//        else if ( i == CYCLE )
//        {
//            i = XPLMGetDatai(efis_dme_1_selector) + 1;
//            if (i>2) i = 0;
//        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_dme_1_selector) + (shuttle_up ? +1 : -1);
            if (i>2)
            {
                i = 1;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_dme_1_selector, i);
    }
    return (XPLMCommandCallback_f)1;
}

// radio2
XPLMCommandCallback_f radio2_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_dme_2_selector) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_dme_2_selector) + 1;
            if (i>2) i = 2;
        }
//        else if ( i == CYCLE )
//        {
//            i = XPLMGetDatai(efis_dme_2_selector) + 1;
//            if (i>2) i = 0;
//        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_dme_2_selector) + (shuttle_up ? +1 : -1);
            if (i>2)
            {
                i = 1;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_dme_2_selector, i);
    }
    return (XPLMCommandCallback_f)1;
}


// source
XPLMCommandCallback_f source_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(hsi_selector) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(hsi_selector) + 1;
            if (i>2) i = 2;
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(hsi_selector) + 1;
            if (i>2) i = 0;
        }
        XPLMSetDatai(hsi_selector, i);
    }
    return (XPLMCommandCallback_f)1;
}

// tfc
XPLMCommandCallback_f tfc_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_shows_tcas);
        XPLMSetDatai(efis_shows_tcas, i);
    }
    return (XPLMCommandCallback_f)1;
}

// arpt
XPLMCommandCallback_f arpt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_shows_airports);
        XPLMSetDatai(efis_shows_airports, i);
    }
    return (XPLMCommandCallback_f)1;
}

// wpt
XPLMCommandCallback_f wpt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_shows_waypoints);
        XPLMSetDatai(efis_shows_waypoints, i);
    }
    return (XPLMCommandCallback_f)1;
}

// vor
XPLMCommandCallback_f vor_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_shows_vors);
        XPLMSetDatai(efis_shows_vors, i);
    }
    return (XPLMCommandCallback_f)1;
}

// ndb
XPLMCommandCallback_f ndb_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_shows_ndbs);
        XPLMSetDatai(efis_shows_ndbs, i);
    }
    return (XPLMCommandCallback_f)1;
}

// sta = vor + ndb
XPLMCommandCallback_f sta_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == CYCLE )
        {
            int vors = XPLMGetDatai(efis_shows_vors);
            int ndbs = XPLMGetDatai(efis_shows_ndbs);
            if ( ! vors && ! ndbs )
            {
                XPLMSetDatai(efis_shows_vors, 1);
            }
            else if ( vors && ! ndbs )
            {
                XPLMSetDatai(efis_shows_ndbs, 1);
            }
            else if ( vors && ndbs )
            {
                XPLMSetDatai(efis_shows_vors, 0);
            }
            else if ( ! vors && ndbs )
            {
                XPLMSetDatai(efis_shows_ndbs, 0);
            }
        }
        else
        {
            if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_shows_stas);
            XPLMSetDatai(efis_pilot_shows_stas, i);
            XPLMSetDatai(efis_shows_vors, i);
            XPLMSetDatai(efis_shows_ndbs, i);
        }
    }
    return (XPLMCommandCallback_f)1;
}

// declutter
XPLMCommandCallback_f declutter_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        //int i = (int)((intptr_t)inRefcon);
        if ( XPLMGetDatai(efis_shows_waypoints) ) XPLMSetDatai(efis_shows_waypoints, 0);
        else if ( XPLMGetDatai(efis_shows_ndbs) ) XPLMSetDatai(efis_shows_ndbs, 0);
        else if ( XPLMGetDatai(efis_shows_vors) ) XPLMSetDatai(efis_shows_vors, 0);
        else if ( XPLMGetDatai(efis_shows_airports) ) XPLMSetDatai(efis_shows_airports, 0);
        else {
            XPLMSetDatai(efis_shows_airports, 1);
            XPLMSetDatai(efis_shows_vors, 1);
            XPLMSetDatai(efis_shows_ndbs, 1);
            XPLMSetDatai(efis_shows_waypoints, 1);
        }
    }
    return (XPLMCommandCallback_f)1;
}


// data = route data
XPLMCommandCallback_f data_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_shows_data);
        XPLMSetDatai(efis_pilot_shows_data, i);
    }
    return (XPLMCommandCallback_f)1;
}

// pos
XPLMCommandCallback_f pos_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_shows_pos);
        XPLMSetDatai(efis_pilot_shows_pos, i);
    }
    return (XPLMCommandCallback_f)1;
}

// terrain on ND
XPLMCommandCallback_f terrain_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_shows_terrain);
        XPLMSetDatai(efis_pilot_shows_terrain, i);
    }
    return (XPLMCommandCallback_f)1;
}

// pilot Vertical Path
XPLMCommandCallback_f vertical_path_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_shows_vp);
        XPLMSetDatai(efis_pilot_shows_vp, i);
    }
    return (XPLMCommandCallback_f)1;
}

// pilot ILS
XPLMCommandCallback_f ils_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_shows_ils);
        XPLMSetDatai(efis_pilot_shows_ils, i);
        if (qpac_ready) XPLMSetDatai(qpac_ils_on_capt, i);
    }
    return (XPLMCommandCallback_f)1;
}

// pilot metric alt
XPLMCommandCallback_f metric_alt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_metric_alt);
        XPLMSetDatai(efis_pilot_metric_alt, i);
        if (qpac_ready) XPLMSetDatai(qpac_fcu_metric_alt, i);
    }
    return (XPLMCommandCallback_f)1;
}

// pilot track FPA / FPV / Bird
XPLMCommandCallback_f track_fpa_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_track_fpa);
        XPLMSetDatai(efis_pilot_track_fpa, i);
        if (qpac_ready) XPLMSetDatai(qpac_fcu_hdg_trk, i);
    }
    return (XPLMCommandCallback_f)1;
}

// weather radar on ND
XPLMCommandCallback_f wxr_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_shows_weather);
        XPLMSetDatai(efis_shows_weather, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_gain_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        float v = XPLMGetDataf(efis_pilot_wxr_gain);
        if ( i == DOWN )
        {
            v -= 0.05f;
            if (v<0) v = 0;
        }
        else if ( i == UP )
        {
        	v += 0.05f;
            if (v>1) v = 1;
        }
        else if ( i == MODE_CENTERED )
        {
        	v = 0.50f;
        }
        XPLMSetDataf(efis_pilot_wxr_gain,v);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_tilt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        float v = XPLMGetDataf(efis_pilot_wxr_tilt);
        if ( i == DOWN )
        {
            v -= 0.5f;
            if (v<-15.0) v = -15.0;
        }
        else if ( i == UP )
        {
        	v += 0.5f;
            if (v>15.0) v = 15.0;
        }
        else if ( i == MODE_CENTERED )
        {
        	v = 0.0f;
        }
        XPLMSetDataf(efis_pilot_wxr_tilt,v);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_auto_gain_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_wxr_auto_gain);
        XPLMSetDatai(efis_pilot_wxr_auto_gain, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_auto_tilt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_wxr_auto_tilt);
        XPLMSetDatai(efis_pilot_wxr_auto_tilt, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_test_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_wxr_test);
        XPLMSetDatai(efis_pilot_wxr_test, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_mode_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_pilot_wxr_mode) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_pilot_wxr_mode) + 1;
            if (i>4) i = 4;
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(efis_pilot_wxr_mode) + 1;
            if (i>4) i = 0;
        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_pilot_wxr_mode) + (shuttle_up ? +1 : -1);
            if (i>4)
            {
                i = 3;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_pilot_wxr_mode, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_slave_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_wxr_slave);
        XPLMSetDatai(efis_pilot_wxr_slave, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_react_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_wxr_react);
        XPLMSetDatai(efis_pilot_wxr_react, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_narrow_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_wxr_narrow);
        XPLMSetDatai(efis_pilot_wxr_narrow, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_alert_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_wxr_alert);
        XPLMSetDatai(efis_pilot_wxr_alert, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f wxr_target_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_wxr_target);
        XPLMSetDatai(efis_pilot_wxr_target, i);
    }
    return (XPLMCommandCallback_f)1;
}



// mins mode
XPLMCommandCallback_f mins_mode_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_pilot_mins_mode);
        XPLMSetDatai(efis_pilot_mins_mode, i);
    }
    return (XPLMCommandCallback_f)1;
}

// mins value
XPLMCommandCallback_f mins_value_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        int m = XPLMGetDatai(efis_pilot_mins_mode);
        if ( m )
        {
            int da = 0;
            if ( i == DOWN )
            {
                da = XPLMGetDatai(efis_pilot_da_bug) - 50;
                if (da<0) da = 0;
            }
            else if ( i == UP )
            {
                da = XPLMGetDatai(efis_pilot_da_bug) + 50;
            }
            else if ( i == OFF )
            {
                da = 0;
            }
            XPLMSetDatai(efis_pilot_da_bug, da);
        }
        else
        {
            float dh = 0.0f;
            if ( i == DOWN )
            {
                dh = XPLMGetDataf(ra_bug_pilot) - 10.0f;
                if (dh<0.0f) dh = 0.0f;
            }
            else if ( i == UP )
            {
                dh = XPLMGetDataf(ra_bug_pilot) + 10.0f;
                if (dh>2500.0f) dh = 2500.0f;
            }
            else if ( i == OFF )
            {
                dh = 0.0f;
            }
            XPLMSetDataf(ra_bug_pilot, dh);
        }
    }
    return (XPLMCommandCallback_f)1;
}


// copilot command handlers
// copilot ctr
XPLMCommandCallback_f copilot_ctr_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_map_mode);
        XPLMSetDatai(efis_copilot_map_mode, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot zoomin
XPLMCommandCallback_f copilot_zoom_x100_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_map_zoomin);
        XPLMSetDatai(efis_copilot_map_zoomin, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot range
XPLMCommandCallback_f copilot_range_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;
    int range_max = qpac_ready ? 5 : 6;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_copilot_map_range_selector) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_copilot_map_range_selector) + 1;
            if (i>range_max) i = range_max;
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(efis_copilot_map_range_selector) + 1;
            if (i>range_max) i = 0;
        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_copilot_map_range_selector) + (shuttle_up ? +1 : -1);
            if (i>range_max)
            {
                i = range_max-1;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_copilot_map_range_selector, i);
        if (qpac_ready) XPLMSetDatai(qpac_co_efis_nd_range,i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot ext range
XPLMCommandCallback_f copilot_ext_range_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;
    int range_max = qpac_ready ? 5 : 6;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        int z = XPLMGetDatai(efis_copilot_map_zoomin);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_copilot_map_range_selector) - 1;
            if (i<0)
            {
                if ( z )
                {
                    i = 0;
                }
                else
                {
                    i = range_max;
                    z = 1;
                }
            }
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_copilot_map_range_selector) + 1;
            if (i>range_max)
            {
                if ( ! z )
                {
                    i = range_max;
                }
                else
                {
                    i = 0;
                    z = 0;
                }
            }
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(efis_copilot_map_range_selector) + 1;
            if (i>range_max)
            {
                i = 0;
                z = !z;
            }
        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_copilot_map_range_selector) + (shuttle_up ? +1 : -1);
            if (i>range_max)
            {
                if ( z )
                {
                    i = 0;
                    z = ! z;
                }
                else
                {
                    i = range_max-1;
                    shuttle_up = 0;
                }
            }
            else if (i<0)
            {
                if ( z )
                {
                    i = 1;
                    shuttle_up = 1;
                }
                else
                {
                    i = range_max;
                    z = ! z;;
                }
            }
        }
        if (i>=1100)
        {
            z = 1;
        }
        else if (i>=1000)
        {
            z = 0;
        }
        XPLMSetDatai(efis_copilot_map_range_selector, i % 10);
        XPLMSetDatai(efis_copilot_map_zoomin, z);
        if (qpac_ready) XPLMSetDatai(qpac_co_efis_nd_range,i % 10);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot mode
XPLMCommandCallback_f copilot_mode_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_copilot_map_submode) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_copilot_map_submode) + 1;
            if (i>4) i = 4;
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(efis_copilot_map_submode) + 1;
            if (i>4) i = 0;
        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_copilot_map_submode) + (shuttle_up ? +1 : -1);
            if (i>4)
            {
                i = 3;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_copilot_map_submode, i);
        if (qpac_ready) XPLMSetDatai(qpac_co_efis_nd_mode,i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot B737-Classic modes
XPLMCommandCallback_f b737cl_copilot_mode_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);

        if ( i == B737CL_FULL_VOR_ILS )
        {
            XPLMSetDatai(efis_copilot_map_submode, MODE_APP);
            XPLMSetDatai(efis_copilot_map_mode, MODE_CENTERED);
        }
        else if ( i == B737CL_EXP_VOR_ILS )
        {
            XPLMSetDatai(efis_copilot_map_submode, MODE_APP);
            XPLMSetDatai(efis_copilot_map_mode, MODE_EXPANDED);
        }
        else if ( i == B737CL_MAP )
        {
            XPLMSetDatai(efis_copilot_map_submode, MODE_MAP);
            XPLMSetDatai(efis_copilot_map_mode, MODE_EXPANDED);
        }
        else if ( i == B737CL_CTR_MAP )
        {
            XPLMSetDatai(efis_copilot_map_submode, MODE_MAP);
            XPLMSetDatai(efis_copilot_map_mode, MODE_CENTERED);
        }
        else if ( i == B737CL_PLAN )
        {
            XPLMSetDatai(efis_copilot_map_submode, MODE_PLN);
        }
    }
    return (XPLMCommandCallback_f)1;
}

// copilot radio1
XPLMCommandCallback_f copilot_radio1_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_copilot_dme_1_selector) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_copilot_dme_1_selector) + 1;
            if (i>2) i = 2;
        }
//        else if ( i == CYCLE )
//        {
//            i = XPLMGetDatai(efis_copilot_dme_1_selector) + 1;
//            if (i>2) i = 0;
//        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_copilot_dme_1_selector) + (shuttle_up ? +1 : -1);
            if (i>2)
            {
                i = 1;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_copilot_dme_1_selector, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot radio2
XPLMCommandCallback_f copilot_radio2_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_copilot_dme_2_selector) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_copilot_dme_2_selector) + 1;
            if (i>2) i = 2;
        }
//        else if ( i == CYCLE )
//        {
//            i = XPLMGetDatai(efis_copilot_dme_2_selector) + 1;
//            if (i>2) i = 0;
//        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_copilot_dme_2_selector) + (shuttle_up ? +1 : -1);
            if (i>2)
            {
                i = 1;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_copilot_dme_2_selector, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot source
XPLMCommandCallback_f copilot_source_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(copilot_hsi_selector) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(copilot_hsi_selector) + 1;
            if (i>2) i = 2;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(copilot_hsi_selector) + 1;
            if (i>2) i = 0;
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(copilot_hsi_selector) + 1;
            if (i>2) i = 0;
        }
        XPLMSetDatai(copilot_hsi_selector, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot tfc
XPLMCommandCallback_f copilot_tfc_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_tcas);
        XPLMSetDatai(efis_copilot_shows_tcas, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot arpt
XPLMCommandCallback_f copilot_arpt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_airports);
        XPLMSetDatai(efis_copilot_shows_airports, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot wpt
XPLMCommandCallback_f copilot_wpt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_waypoints);
        XPLMSetDatai(efis_copilot_shows_waypoints, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot vor
XPLMCommandCallback_f copilot_vor_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_vors);
        XPLMSetDatai(efis_copilot_shows_vors, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot ndb
XPLMCommandCallback_f copilot_ndb_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_ndbs);
        XPLMSetDatai(efis_copilot_shows_ndbs, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot sta = vor + ndb
XPLMCommandCallback_f copilot_sta_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == CYCLE )
        {
            int vors = XPLMGetDatai(efis_copilot_shows_vors);
            int ndbs = XPLMGetDatai(efis_copilot_shows_ndbs);
            if ( ! vors && ! ndbs )
            {
                XPLMSetDatai(efis_copilot_shows_vors, 1);
            }
            else if ( vors && ! ndbs )
            {
                XPLMSetDatai(efis_copilot_shows_ndbs, 1);
            }
            else if ( vors && ndbs )
            {
                XPLMSetDatai(efis_copilot_shows_vors, 0);
            }
            else if ( ! vors && ndbs )
            {
                XPLMSetDatai(efis_copilot_shows_ndbs, 0);
            }
        }
        else
        {
            if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_stas);
            XPLMSetDatai(efis_copilot_shows_stas, i);
            XPLMSetDatai(efis_copilot_shows_vors, i);
            XPLMSetDatai(efis_copilot_shows_ndbs, i);
        }
    }
    return (XPLMCommandCallback_f)1;
}

// copilot declutter
XPLMCommandCallback_f copilot_declutter_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        //int i = (int)((intptr_t)inRefcon);
        if ( XPLMGetDatai(efis_copilot_shows_waypoints) ) XPLMSetDatai(efis_copilot_shows_waypoints, 0);
        else if ( XPLMGetDatai(efis_copilot_shows_ndbs) ) XPLMSetDatai(efis_copilot_shows_ndbs, 0);
        else if ( XPLMGetDatai(efis_copilot_shows_vors) ) XPLMSetDatai(efis_copilot_shows_vors, 0);
        else if ( XPLMGetDatai(efis_copilot_shows_airports) ) XPLMSetDatai(efis_copilot_shows_airports, 0);
        else {
            XPLMSetDatai(efis_copilot_shows_airports, 1);
            XPLMSetDatai(efis_copilot_shows_vors, 1);
            XPLMSetDatai(efis_copilot_shows_ndbs, 1);
            XPLMSetDatai(efis_copilot_shows_waypoints, 1);
        }
    }
    return (XPLMCommandCallback_f)1;
}


// copilot data = route data
XPLMCommandCallback_f copilot_data_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_data);
        XPLMSetDatai(efis_copilot_shows_data, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot pos
XPLMCommandCallback_f copilot_pos_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_pos);
        XPLMSetDatai(efis_copilot_shows_pos, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot terrain
XPLMCommandCallback_f copilot_terrain_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_terrain);
        XPLMSetDatai(efis_copilot_shows_terrain, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot Vertical Path
XPLMCommandCallback_f copilot_vertical_path_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_vp);
        XPLMSetDatai(efis_copilot_shows_vp, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot ILS
XPLMCommandCallback_f copilot_ils_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_shows_ils);
        XPLMSetDatai(efis_copilot_shows_ils, i);
        if (qpac_ready) XPLMSetDatai(qpac_ils_on_fo, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot metric alt
XPLMCommandCallback_f copilot_metric_alt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_metric_alt);
        XPLMSetDatai(efis_copilot_metric_alt, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot track FPA / FPV / Bird
XPLMCommandCallback_f copilot_track_fpa_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_track_fpa);
        XPLMSetDatai(efis_copilot_track_fpa, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot weather radar on ND
XPLMCommandCallback_f copilot_wxr_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_shows_weather);
        XPLMSetDatai(efis_shows_weather, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_gain_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        float v = XPLMGetDataf(efis_copilot_wxr_gain);
        if ( i == DOWN )
        {
            v -= 0.05f;
            if (v<0) v = 0;
        }
        else if ( i == UP )
        {
        	v += 0.05f;
            if (v>1) v = 1;
        }
        else if ( i == MODE_CENTERED )
        {
        	v = 0.50f;
        }
        XPLMSetDataf(efis_copilot_wxr_gain,v);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_tilt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        float v = XPLMGetDataf(efis_copilot_wxr_tilt);
        if ( i == DOWN )
        {
            v -= 0.5f;
            if (v<-15.0) v = -15.0;
        }
        else if ( i == UP )
        {
        	v += 0.5f;
            if (v>15.0) v = 15.0;
        }
        else if ( i == MODE_CENTERED )
        {
        	v = 0.0f;
        }
        XPLMSetDataf(efis_copilot_wxr_tilt,v);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_auto_gain_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_wxr_auto_gain);
        XPLMSetDatai(efis_copilot_wxr_auto_gain, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_auto_tilt_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_wxr_auto_tilt);
        XPLMSetDatai(efis_copilot_wxr_auto_tilt, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_test_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_wxr_test);
        XPLMSetDatai(efis_copilot_wxr_test, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_mode_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    static int shuttle_up = 1;

    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == DOWN )
        {
            i = XPLMGetDatai(efis_copilot_wxr_mode) - 1;
            if (i<0) i = 0;
        }
        else if ( i == UP )
        {
            i = XPLMGetDatai(efis_copilot_wxr_mode) + 1;
            if (i>4) i = 4;
        }
        else if ( i == CYCLE )
        {
            i = XPLMGetDatai(efis_copilot_wxr_mode) + 1;
            if (i>4) i = 0;
        }
        else if ( i == SHUTTLE )
        {
            i = XPLMGetDatai(efis_copilot_wxr_mode) + (shuttle_up ? +1 : -1);
            if (i>4)
            {
                i = 3;
                shuttle_up = 0;
            }
            else if (i<0)
            {
                i = 1;
                shuttle_up = 1;
            }
        }
        XPLMSetDatai(efis_copilot_wxr_mode, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_slave_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_wxr_slave);
        XPLMSetDatai(efis_copilot_wxr_slave, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_react_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_wxr_react);
        XPLMSetDatai(efis_copilot_wxr_react, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_narrow_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_wxr_narrow);
        XPLMSetDatai(efis_copilot_wxr_narrow, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_alert_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_wxr_alert);
        XPLMSetDatai(efis_copilot_wxr_alert, i);
    }
    return (XPLMCommandCallback_f)1;
}

XPLMCommandCallback_f copilot_wxr_target_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_wxr_target);
        XPLMSetDatai(efis_copilot_wxr_target, i);
    }
    return (XPLMCommandCallback_f)1;
}


// copilot mins mode
XPLMCommandCallback_f copilot_mins_mode_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == TOGGLE ) i = ! XPLMGetDatai(efis_copilot_mins_mode);
        XPLMSetDatai(efis_copilot_mins_mode, i);
    }
    return (XPLMCommandCallback_f)1;
}

// copilot mins value
XPLMCommandCallback_f copilot_mins_value_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        int m = XPLMGetDatai(efis_copilot_mins_mode);
        if ( m )
        {
            int da = 0;
            if ( i == DOWN )
            {
                da = XPLMGetDatai(efis_copilot_da_bug) - 50;
                if (da<0) da = 0;
            }
            else if ( i == UP )
            {
                da = XPLMGetDatai(efis_copilot_da_bug) + 50;
            }
            else if ( i == OFF )
            {
                da = 0;
            }
            XPLMSetDatai(efis_copilot_da_bug, da);
        }
        else
        {
            float dh = 0.0f;
            if ( i == DOWN )
            {
                dh = XPLMGetDataf(ra_bug_copilot) - 10.0f;
                if (dh<0.0f) dh = 0.0f;
            }
            else if ( i == UP )
            {
                dh = XPLMGetDataf(ra_bug_copilot) + 10.0f;
                if (dh>2500.0f) dh = 2500.0f;
            }
            else if ( i == OFF )
            {
                dh = 0.0f;
            }
            XPLMSetDataf(ra_bug_copilot, dh);
        }
    }
    return (XPLMCommandCallback_f)1;
}


// MFD
XPLMCommandCallback_f mfd_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
	float zero_tab[8]= { 0,0,0,0,0,0,0,0 };
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        if ( i == MFD_RESET_FUEL_USED ) {
        	XPLMSetDatavf(mfd_fuel_used, zero_tab, 0, 8);
        } else {
        	if ( i == DOWN )
        	{
        		i = XPLMGetDatai(mfd_mode) - 1;
        		if (i<0) i = 0;
        	}
        	else if ( i == UP )
        	{
        		i = XPLMGetDatai(mfd_mode) + 1;
        		if (i>15) i = 15;
        	}
        	else if ( i == CYCLE )
        	{
        		i = XPLMGetDatai(mfd_mode) + 1;
        		if (i>15) i = 0;
        	}
        	XPLMSetDatai(mfd_mode, i);
        }
        if ( qpac_plugin_status && (i>=3) ) {
            switch (i-3) {
		case 0  : XPLMSetDatai(qpac_sd_eng,1); break;
		case 1  : XPLMSetDatai(qpac_sd_bleed,1); break;
		case 2  : XPLMSetDatai(qpac_sd_press,1); break;
		case 3  : XPLMSetDatai(qpac_sd_elec,1); break;
		case 4  : XPLMSetDatai(qpac_sd_hyd,1); break;
		case 5  : XPLMSetDatai(qpac_sd_fuel,1); break;
		case 6  : XPLMSetDatai(qpac_sd_apu,1); break;
		case 7  : XPLMSetDatai(qpac_sd_cond,1); break;
		case 8  : XPLMSetDatai(qpac_sd_door,1); break;
		case 9  : XPLMSetDatai(qpac_sd_wheel,1); break;
		case 10  : XPLMSetDatai(qpac_sd_fctl,1); break;
		case 11  :
			XPLMSetDatai(qpac_sd_eng,0);
			XPLMSetDatai(qpac_sd_bleed,0);
			XPLMSetDatai(qpac_sd_press,0);
			XPLMSetDatai(qpac_sd_elec,0);
			XPLMSetDatai(qpac_sd_hyd,0);
			XPLMSetDatai(qpac_sd_fuel,0);
			XPLMSetDatai(qpac_sd_apu,0);
			XPLMSetDatai(qpac_sd_cond,0);
			XPLMSetDatai(qpac_sd_door,0);
			XPLMSetDatai(qpac_sd_wheel,0);
			XPLMSetDatai(qpac_sd_fctl,0);
			XPLMSetDatai(qpac_sd_status,0);
			break;
		case 12 : XPLMSetDatai(qpac_sd_status,1); break;
            }
        }
    }
    return (XPLMCommandCallback_f)1;
}


// CDU
XPLMCommandCallback_f cdu_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        switch (i) {
        case CDU_PILOT_CUSTOM : XPLMSetDatai(cdu_pilot_source,0); break;
        case CDU_PILOT_XFMC : XPLMSetDatai(cdu_pilot_source,1); break;
        case CDU_COPILOT_CUSTOM : XPLMSetDatai(cdu_copilot_source,0); break;
        case CDU_COPILOT_XFMC : XPLMSetDatai(cdu_copilot_source,1); break;
        }
    }
    return (XPLMCommandCallback_f)1;
}


// Sync NAV*
XPLMCommandCallback_f nav_sync_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandContinue)
    {
	    int navaid[6];
        float heading;
        float bearing;
        float course;
        int i = (int)((intptr_t)inRefcon);
	    XPLMGetDatavi(nav_type_, navaid, 0, 2);
        if ( i == 1 )
        {
			// NAV1
			if ( ( navaid[0] == 4 ) || ( navaid[0] == 5 ) ) {
				// LOC/ILS
				XPLMSetDataf( nav1_obs_degm, XPLMGetDataf( nav1_course_degm ) );
			} else {
				// VOR
					heading = XPLMGetDataf(magpsi);
					bearing = XPLMGetDataf(nav1_dir_degt);
					course = heading + bearing;
					if ( course < 0.0f ) course += 360.0f;
					XPLMSetDataf(nav1_obs_degm, course );
			}
        } else /* i == 2 */ {
			// NAV2
			if ( ( navaid[1] == 4 ) || ( navaid[1] == 5 ) ) {
				XPLMSetDataf( nav2_obs_degm, XPLMGetDataf( nav2_course_degm ) );
			} else {
				heading = XPLMGetDataf(magpsi);
				bearing = XPLMGetDataf(nav2_dir_degt);
				course = heading + bearing;
				if ( course < 0.0f ) course += 360.0f;
				XPLMSetDataf(nav2_obs_degm, course );
			}
        }
    }
    return (XPLMCommandCallback_f)1;
}


// Auto Range
XPLMCommandCallback_f auto_range_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandContinue)
    {
        float dist = 0.0f;
        int i = (int)((intptr_t)inRefcon);
		XPLMDataRef nd_range = NULL;
		int mode = 0;
		int ctr = 0;
		switch ( i )
		{
		case EFIS_PILOT :
			dist = XPLMGetDataf(hsi_dme_nm_pilot);
			mode = XPLMGetDatai(efis_map_submode);
			ctr = XPLMGetDatai(efis_map_mode);
			// the copilot dataref that we will modify
			nd_range = efis_map_range_selector;
			break;
		case EFIS_COPILOT :
			dist = XPLMGetDataf(hsi_dme_nm_copilot);
			mode = XPLMGetDatai(efis_copilot_map_submode);
			ctr = XPLMGetDatai(efis_copilot_map_mode);
			// the copilot dataref that we will modify
			nd_range = efis_copilot_map_range_selector;
			break;
		}

		// half the range when centered
		// actually, we pretend that the distance doubles, otherwise we would have to change all the range values below
		if ( ( !ctr && ((mode==0)||(mode==1)||(mode==2)) ) || ( ctr && (mode==3) ) || (mode==4) )
			dist *= 2.0f;

        if ( ( dist < 999.9f ) && ( dist >= 640.0f * 0.4f ) )
			XPLMSetDatai(nd_range, RANGE_640);
		else if ( ( dist < 320.0f * 0.8f ) && ( dist >= 320.0f * 0.4f ) )
			XPLMSetDatai(nd_range, RANGE_320);
		else if ( ( dist < 160.0f * 0.8f ) && ( dist >= 160.0f * 0.4f ) )
			XPLMSetDatai(nd_range, RANGE_160);
		else if ( ( dist < 80.0f * 0.8f ) && ( dist >= 80.0f * 0.4f ) )
			XPLMSetDatai(nd_range, RANGE_80);
		else if ( ( dist < 40.0f * 0.8f ) && ( dist >= 40.0f * 0.4f ) )
			XPLMSetDatai(nd_range, RANGE_40);
		else if ( ( dist < 20.0f * 0.8f ) && ( dist >= 20.0f * 0.4f ) )
			XPLMSetDatai(nd_range, RANGE_20);
		else if ( ( dist < 10.0f * 0.8f ) && ( dist > 0.0f ) )
			XPLMSetDatai(nd_range, RANGE_10);
    }
    return (XPLMCommandCallback_f)1;
}


// Auto Extended Range
XPLMCommandCallback_f auto_ext_range_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandContinue)
    {
        float dist = 0.0f;
        int i = (int)((intptr_t)inRefcon);
		XPLMDataRef nd_range = NULL;
		XPLMDataRef nd_zoomin = NULL;
		int mode = 0;
		int ctr = 0;
		switch ( i )
		{
		case EFIS_PILOT :
			dist = XPLMGetDataf(hsi_dme_nm_pilot);
			mode = XPLMGetDatai(efis_map_submode);
			ctr = XPLMGetDatai(efis_map_mode);
			// the copilot datarefs that we will modify
			nd_range = efis_map_range_selector;
			nd_zoomin = efis_pilot_map_zoomin;
			break;
		case EFIS_COPILOT :
			dist = XPLMGetDataf(hsi_dme_nm_copilot);
			mode = XPLMGetDatai(efis_copilot_map_submode);
			ctr = XPLMGetDatai(efis_copilot_map_mode);
			// the copilot datarefs that we will modify
			nd_range = efis_copilot_map_range_selector;
			nd_zoomin = efis_copilot_map_zoomin;
			break;
		}

		// half the range when centered
		// actually, we pretend that the distance doubles, otherwise we would have to change all the range values below
		if ( ( !ctr && ((mode==0)||(mode==1)||(mode==2)) ) || ( ctr && (mode==3) ) || (mode==4) )
			dist *= 2.0f;

        if ( ( dist < 999.9f ) && ( dist >= 640.0f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_640);
			XPLMSetDatai(nd_zoomin, ZOOMIN_OFF);
		} else if ( ( dist < 320.0f * 0.8f ) && ( dist >= 320.0f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_320);
			XPLMSetDatai(nd_zoomin, ZOOMIN_OFF);
		} else if ( ( dist < 160.0f * 0.8f ) && ( dist >= 160.0f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_160);
			XPLMSetDatai(nd_zoomin, ZOOMIN_OFF);
		} else if ( ( dist < 80.0f * 0.8f ) && ( dist >= 80.0f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_80);
			XPLMSetDatai(nd_zoomin, ZOOMIN_OFF);
		} else if ( ( dist < 40.0f * 0.8f ) && ( dist >= 40.0f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_40);
			XPLMSetDatai(nd_zoomin, ZOOMIN_OFF);
		} else if ( ( dist < 20.0f * 0.8f ) && ( dist >= 20.0f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_20);
			XPLMSetDatai(nd_zoomin, ZOOMIN_OFF);
		} else if ( ( dist < 10.0f * 0.8f ) && ( dist >= 5.0f ) ) {
			XPLMSetDatai(nd_range, RANGE_10);
			XPLMSetDatai(nd_zoomin, ZOOMIN_OFF);
		} else if ( ( dist < 5.0f ) && ( dist >= 6.40f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_640);
			XPLMSetDatai(nd_zoomin, ZOOMIN_ON);
		} else if ( ( dist < 3.20f * 0.8f ) && ( dist >= 3.20f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_320);
			XPLMSetDatai(nd_zoomin, ZOOMIN_ON);
		} else if ( ( dist < 1.60f * 0.8f ) && ( dist >= 1.60f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_160);
			XPLMSetDatai(nd_zoomin, ZOOMIN_ON);
		} else if ( ( dist < 0.80f * 0.8f ) && ( dist >= 0.80f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_80);
			XPLMSetDatai(nd_zoomin, ZOOMIN_ON);
		} else if ( ( dist < 0.40f * 0.8f ) && ( dist >= 0.40f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_40);
			XPLMSetDatai(nd_zoomin, ZOOMIN_ON);
		} else if ( ( dist < 0.20f * 0.8f ) && ( dist >= 0.20f * 0.4f ) ) {
			XPLMSetDatai(nd_range, RANGE_20);
			XPLMSetDatai(nd_zoomin, ZOOMIN_ON);
		} else if ( ( dist < 0.10f * 0.8f ) && ( dist > 0.0f ) ) {
			XPLMSetDatai(nd_range, RANGE_10);
			XPLMSetDatai(nd_zoomin, ZOOMIN_ON);
		}
    }
    return (XPLMCommandCallback_f)1;
}


/** clock_handler
 * 0 CLK_START_STOP_RESET Legacy X-Plane Chrono Start / Stop / Reset
 * 1 CLK_START_STOP Legacy X-Plane Chrono Start / Stop
 * 2 CLK_RESET Legacy X-Plane Chrono Reset
 * 3 CLK_ET_STOP Elapsed Flight Timer : Stop
 * 4 CLK_ET_RUN Elapsed Flight Timer : Run
 * 5 CLK_ET_RESET Elapsed Flight Timer : Rest
 * 6 CLK_UTC_GPS Clock reference set to GPS (X-Plane Clock)
 * 7 CLK_UTC_INT Clock reference set to GPS (XHSI Clock)
 * 8 CLK_UTC_SET Clock set mode (XHSI Clock)
 */
XPLMCommandCallback_f clock_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
    	int i = (int)((intptr_t)inRefcon);
    	switch ( i ) {
    	case CLK_START_STOP_RESET:
    		if ( ( XPLMGetDataf(elapsed_time_sec) == 0.0f ) || XPLMGetDatai(timer_is_running) )
    			XPLMCommandOnce(timer_start_stop);
    		else
    			XPLMCommandOnce(timer_reset);
    		break;
    	case CLK_START_STOP:
    		XPLMCommandOnce(timer_start_stop);
    		break;
    	case CLK_RESET:
    		XPLMCommandOnce(timer_reset);
    		break;
    	case CLK_ET_STOP:
    		XPLMSetDatai(xhsi_et_running,0);
    		break;
    	case CLK_ET_RUN:
    		XPLMSetDatai(xhsi_et_running,1);
    		break;
    	case CLK_ET_RESET:
    		XPLMSetDataf(flight_time_sec,0.0);
    		XPLMSetDatai(xhsi_et_running,0);
    		break;
    	case CLK_UTC_GPS:
    		XPLMSetDatai(xhsi_utc_selector,0);
    		break;
    	case CLK_UTC_INT:
    		XPLMSetDatai(xhsi_utc_selector,1);
    		break;
    	case CLK_UTC_SET:
    		XPLMSetDatai(xhsi_utc_selector,2);
    		break;
    	}
    }
    return (XPLMCommandCallback_f)1;
}

// Chrono
XPLMCommandCallback_f chrono_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int i = (int)((intptr_t)inRefcon);
        switch ( i )
        {
        	case 0 : // START-STOP-RESET
        		if ( ( XPLMGetDataf(efis_pilot_chrono) == 0.0f ) || XPLMGetDatai(efis_pilot_chrono_running) ) {
        			XPLMSetDatai(efis_pilot_chrono_running, XPLMGetDatai(efis_pilot_chrono_running) != 0 ? 0 : 1);
        		} else
        		    XPLMSetDataf(efis_pilot_chrono, 0.0f);
        		break;
        	case 1 : // START-STOP
        		XPLMSetDatai(efis_pilot_chrono_running, XPLMGetDatai(efis_pilot_chrono_running) != 0 ? 0 : 1);
        		break;
        	case 2 : // RESET
        		XPLMSetDataf(efis_pilot_chrono, 0.0f);
        		break;
        	case 3 : // START-STOP-RESET
        		if ( ( XPLMGetDataf(efis_copilot_chrono) == 0.0f ) || XPLMGetDatai(efis_copilot_chrono_running) ) {
        			XPLMSetDatai(efis_copilot_chrono_running, XPLMGetDatai(efis_copilot_chrono_running) != 0 ? 0 : 1);
        		} else
        		    XPLMSetDataf(efis_copilot_chrono, 0.0f);
        		break;
        	case 4 : // START-STOP
        		XPLMSetDatai(efis_copilot_chrono_running, XPLMGetDatai(efis_copilot_chrono_running) != 0 ? 0 : 1);
        		break;
        	case 5 : // RESET
        		XPLMSetDataf(efis_copilot_chrono, 0.0f);
        		break;
        }
    }
    return (XPLMCommandCallback_f)1;
}


//// contact_atc
//XPLMCommandCallback_f contact_atc_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon)
//{
//
//    switch (inPhase) {
//        case xplm_CommandBegin:
//        case xplm_CommandContinue:
//            XPLMSetDatai(xhsi_rtu_contact_atc, 1);
//            break;
//        case xplm_CommandEnd:
//            XPLMSetDatai(xhsi_rtu_contact_atc, 0);
//            break;
//        default:
//            break;
//    }
//    // return 1 , so that other plugins like PilotEdge or X-Plane itself can handle this command
//    return (XPLMCommandCallback_f)1;
//}


// Transponder
XPLMCommandCallback_f xpdr_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int mode = XPLMGetDatai(transponder_mode);
        int code = XPLMGetDatai(transponder_code);
        int code_units;
        int code_tens;
        int code_hundreds;
        int code_thousands;
        int i;
        code_units = code % 10;
        code /= 10;
        code_tens = code % 10;
        code /= 10;
        code_hundreds = code % 10;
        code /= 10;
        code_thousands = code % 10;
        i = (int)((intptr_t)inRefcon);
        switch ( i )
        {
            case XPDR_MODE_UP :
                mode += 1;
                if (mode > 4) mode = 4;
                break;
            case XPDR_MODE_DOWN :
                mode -= 1;
                if (mode < 0) mode = 0;
                break;
            case XPDR_MODE_CYCLE :
                mode += 1;
                if (mode > 4) mode = 0;
                break;
            case XPDR_CODE_HUNDREDS_UP :
                code_hundreds += 1;
                if (code_hundreds > 7)
                {
                    code_hundreds = 0;
                    code_thousands += 1;
                    if (code_thousands > 7)
                        code_thousands = 0;
                }
                break;
            case XPDR_CODE_HUNDREDS_DOWN :
                code_hundreds -= 1;
                if (code_hundreds < 0)
                {
                    code_hundreds = 7;
                    code_thousands -= 1;
                    if (code_thousands < 0)
                        code_thousands = 7;
                }
                break;
            case XPDR_CODE_UNITS_UP :
                code_units += 1;
                if (code_units > 7)
                {
                    code_units = 0;
                    code_tens += 1;
                    if (code_tens > 7)
                        code_tens = 0;
                }
                break;
            case XPDR_CODE_UNITS_DOWN :
                code_units -= 1;
                if (code_units < 0)
                {
                    code_units = 7;
                    code_tens -= 1;
                    if (code_tens < 0)
                        code_tens = 7;
                }
                break;
        }
        XPLMSetDatai(transponder_mode, mode);
        XPLMSetDatai(transponder_code, code_thousands * 1000 + code_hundreds * 100 + code_tens * 10 + code_units);
    }
    return (XPLMCommandCallback_f)1;
}


// ADF tuning
XPLMCommandCallback_f adf_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        XPLMDataRef radioref;
        int i = (int)((intptr_t)inRefcon);
        int action = i % 1000;
        int radio = i - action;
        int freq;
        int freq_units;
        int freq_tens;
        int freq_hundreds;
        int freq_thousands;
        switch ( radio )
        {
            case ADF1 :
                radioref = adf1_freq_hz;
                break;
            case ADF1_STBY :
                radioref = adf1_stdby_freq_hz;
                break;
            case ADF2 :
                radioref = adf2_freq_hz;
                break;
            case ADF2_STBY :
                radioref = adf2_stdby_freq_hz;
                break;
            default :
                // this should never happen, it's just to make the compiler happy
                radioref = adf2_stdby_freq_hz;
                break;
        }
        freq = XPLMGetDatai(radioref);
        freq_units = freq % 10;
        freq /= 10;
        freq_tens = freq % 10;
        freq /= 10;
        freq_hundreds = freq % 10;
        freq /= 10;
        freq_thousands = freq % 10;
        switch ( action )
        {
            case ADF_HUNDREDS_UP :
                if ( freq_thousands != 1 || freq_hundreds != 9 )
                {
                    freq_hundreds += 1;
                    if (freq_hundreds > 9)
                    {
                        freq_hundreds = 0;
                        freq_thousands += 1;
                        if (freq_thousands > 1)
                            freq_thousands = 1;
                    }
                }
                break;
            case ADF_HUNDREDS_DOWN :
                if ( freq_thousands != 0 || freq_hundreds != 1)
                {
                    freq_hundreds -= 1;
                    if (freq_hundreds < 0)
                    {
                        freq_hundreds = 9;
                        freq_thousands -= 1;
                        if (freq_thousands < 0)
                            freq_thousands = 0;
                    }
                }
                break;
            case ADF_UNITS_UP :
                freq_units += 1;
                if (freq_units > 9)
                {
                    freq_units = 0;
                    freq_tens += 1;
                    if (freq_tens > 9)
                        freq_tens = 0;
                }
                break;
            case ADF_UNITS_DOWN :
                freq_units -= 1;
                if (freq_units < 0)
                {
                    freq_units = 9;
                    freq_tens -= 1;
                    if (freq_tens < 0)
                        freq_tens = 9;
                }
                break;
        }
        XPLMSetDatai(radioref, freq_thousands * 1000 + freq_hundreds * 100 + freq_tens * 10 + freq_units);
    }
    return (XPLMCommandCallback_f)1;
}


// RTU
XPLMCommandCallback_f rtu_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon)
{
    if (inPhase == xplm_CommandBegin)
    {
        int sel;
        int i = (int)((intptr_t)inRefcon);
        switch ( i )
        {
            case RTU_SELECT_NONE :
            case RTU_SELECT_COM1 :
            case RTU_SELECT_NAV1 :
            case RTU_SELECT_ADF1 :
            case RTU_SELECT_XPDR :
            case RTU_SELECT_TCAS :
            case RTU_SELECT_ADF2 :
            case RTU_SELECT_NAV2 :
            case RTU_SELECT_COM2 :
                XPLMSetDatai(xhsi_rtu_selected_radio, i);
                break;
                
            case RTU_SELECT_FLIP_COM1 :
                if (XPLMGetDatai(xhsi_rtu_selected_radio) == RTU_SELECT_COM1) XPLMCommandOnce(com1_standy_flip);
                else XPLMSetDatai(xhsi_rtu_selected_radio, RTU_SELECT_COM1);
                break;
                
            case RTU_SELECT_FLIP_NAV1 :
                if (XPLMGetDatai(xhsi_rtu_selected_radio) == RTU_SELECT_NAV1) XPLMCommandOnce(nav1_standy_flip);
                else XPLMSetDatai(xhsi_rtu_selected_radio, RTU_SELECT_NAV1);
                break;
                
            case RTU_SELECT_FLIP_ADF1 :
                if (XPLMGetDatai(xhsi_rtu_selected_radio) == RTU_SELECT_ADF1) XPLMCommandOnce(adf1_standy_flip);
                else XPLMSetDatai(xhsi_rtu_selected_radio, RTU_SELECT_ADF1);
                break;
                
            case RTU_SELECT_FLIP_XPDR :
                if (XPLMGetDatai(xhsi_rtu_selected_radio) == RTU_SELECT_XPDR) XPLMCommandOnce(sim_transponder_transponder_ident);
                else XPLMSetDatai(xhsi_rtu_selected_radio, RTU_SELECT_XPDR);
                break;
                
            case RTU_SELECT_FLIP_TCAS :
                if (XPLMGetDatai(xhsi_rtu_selected_radio) == RTU_SELECT_TCAS) XPLMCommandOnce(xpdr_mode_cycle);
                else XPLMSetDatai(xhsi_rtu_selected_radio, RTU_SELECT_TCAS);
                break;
                
            case RTU_SELECT_FLIP_ADF2 :
                if (XPLMGetDatai(xhsi_rtu_selected_radio) == RTU_SELECT_ADF2) XPLMCommandOnce(adf2_standy_flip);
                else XPLMSetDatai(xhsi_rtu_selected_radio, RTU_SELECT_ADF2);
                break;
                
            case RTU_SELECT_FLIP_NAV2 :
                if (XPLMGetDatai(xhsi_rtu_selected_radio) == RTU_SELECT_NAV2) XPLMCommandOnce(nav2_standy_flip);
                else XPLMSetDatai(xhsi_rtu_selected_radio, RTU_SELECT_NAV2);
                break;
                
            case RTU_SELECT_FLIP_COM2 :
                if (XPLMGetDatai(xhsi_rtu_selected_radio) == RTU_SELECT_COM2) XPLMCommandOnce(com2_standy_flip);
                else XPLMSetDatai(xhsi_rtu_selected_radio, RTU_SELECT_COM2);
                break;
                
            case RTU_SELECT_NEXT :
                sel = XPLMGetDatai(xhsi_rtu_selected_radio);
                sel += 1;
                if (sel > 8) sel = 0;
                XPLMSetDatai(xhsi_rtu_selected_radio, sel);
                break;
                
            case RTU_SELECT_PREV :
                sel = XPLMGetDatai(xhsi_rtu_selected_radio);
                sel -= 1;
                if (sel < 0) sel = 8;
                XPLMSetDatai(xhsi_rtu_selected_radio, sel);
                break;
                
            case RTU_FLIP_SELECTED :
                sel = XPLMGetDatai(xhsi_rtu_selected_radio);
                switch ( sel )
                {
                    case RTU_SELECT_COM1 :
                        XPLMCommandOnce(com1_standy_flip);
                        break;
                    case RTU_SELECT_NAV1 :
                        XPLMCommandOnce(nav1_standy_flip);
                        break;
                    case RTU_SELECT_ADF1 :
                        XPLMCommandOnce(adf1_standy_flip);
                        break;
                    case RTU_SELECT_XPDR :
                        // not realistic... XPLMCommandOnce(sim_transponder_transponder_ident);
                        break;
                    case RTU_SELECT_TCAS :
                        XPLMCommandOnce(xpdr_mode_cycle);
                        break;
                    case RTU_SELECT_ADF2 :
                        XPLMCommandOnce(adf2_standy_flip);
                        break;
                    case RTU_SELECT_NAV2 :
                        XPLMCommandOnce(nav2_standy_flip);
                        break;
                    case RTU_SELECT_COM2 :
                        XPLMCommandOnce(com2_standy_flip);
                        break;
                }
                break;
                
            case RTU_COARSE_DOWN :
                sel = XPLMGetDatai(xhsi_rtu_selected_radio);
                switch ( sel )
                {
                    case RTU_SELECT_COM1 :
                        XPLMCommandOnce(stby_com1_coarse_down);
                        break;
                    case RTU_SELECT_NAV1 :
                        XPLMCommandOnce(stby_nav1_coarse_down);
                        break;
                    case RTU_SELECT_ADF1 :
                        XPLMCommandOnce(adf1_stby_hundreds_down);
                        break;
                    case RTU_SELECT_XPDR :
                        XPLMCommandOnce(xpdr_code_hundreds_down);
                        break;
                    case RTU_SELECT_TCAS :
                        XPLMCommandOnce(xpdr_mode_down);
                        break;
                    case RTU_SELECT_ADF2 :
                        XPLMCommandOnce(adf2_stby_hundreds_down);
                        break;
                    case RTU_SELECT_NAV2 :
                        XPLMCommandOnce(stby_nav2_coarse_down);
                        break;
                    case RTU_SELECT_COM2 :
                        XPLMCommandOnce(stby_com2_coarse_down);
                        break;
                }
                break;
                
            case RTU_COARSE_UP :
                sel = XPLMGetDatai(xhsi_rtu_selected_radio);
                switch ( sel )
                {
                    case RTU_SELECT_COM1 :
                        XPLMCommandOnce(stby_com1_coarse_up);
                        break;
                    case RTU_SELECT_NAV1 :
                        XPLMCommandOnce(stby_nav1_coarse_up);
                        break;
                    case RTU_SELECT_ADF1 :
                        XPLMCommandOnce(adf1_stby_hundreds_up);
                        break;
                    case RTU_SELECT_XPDR :
                        XPLMCommandOnce(xpdr_code_hundreds_up);
                        break;
                    case RTU_SELECT_TCAS :
                        XPLMCommandOnce(xpdr_mode_up);
                        break;
                    case RTU_SELECT_ADF2 :
                        XPLMCommandOnce(adf2_stby_hundreds_up);
                        break;
                    case RTU_SELECT_NAV2 :
                        XPLMCommandOnce(stby_nav2_coarse_up);
                        break;
                    case RTU_SELECT_COM2 :
                        XPLMCommandOnce(stby_com2_coarse_up);
                        break;
                }
                break;
                
            case RTU_FINE_DOWN :
                sel = XPLMGetDatai(xhsi_rtu_selected_radio);
                switch ( sel )
                {
                    case RTU_SELECT_COM1 :
                        XPLMCommandOnce(stby_com1_fine_down_833);
                        break;
                    case RTU_SELECT_NAV1 :
                        XPLMCommandOnce(stby_nav1_fine_down);
                        break;
                    case RTU_SELECT_ADF1 :
                        XPLMCommandOnce(adf1_stby_units_down);
                        break;
                    case RTU_SELECT_XPDR :
                        XPLMCommandOnce(xpdr_code_units_down);
                        break;
                    case RTU_SELECT_TCAS :
                        XPLMCommandOnce(xpdr_mode_down);
                        break;
                    case RTU_SELECT_ADF2 :
                        XPLMCommandOnce(adf2_stby_units_down);
                        break;
                    case RTU_SELECT_NAV2 :
                        XPLMCommandOnce(stby_nav2_fine_down);
                        break;
                    case RTU_SELECT_COM2 :
                        XPLMCommandOnce(stby_com2_fine_down_833);
                        break;
                }
                break;
                
            case RTU_FINE_UP :
                sel = XPLMGetDatai(xhsi_rtu_selected_radio);
                switch ( sel )
                {
                    case RTU_SELECT_COM1 :
                        XPLMCommandOnce(stby_com1_fine_up_833);
                        break;
                    case RTU_SELECT_NAV1 :
                        XPLMCommandOnce(stby_nav1_fine_up);
                        break;
                    case RTU_SELECT_ADF1 :
                        XPLMCommandOnce(adf1_stby_units_up);
                        break;
                    case RTU_SELECT_XPDR :
                        XPLMCommandOnce(xpdr_code_units_up);
                        break;
                    case RTU_SELECT_TCAS :
                        XPLMCommandOnce(xpdr_mode_up);
                        break;
                    case RTU_SELECT_ADF2 :
                        XPLMCommandOnce(adf2_stby_units_up);
                        break;
                    case RTU_SELECT_NAV2 :
                        XPLMCommandOnce(stby_nav2_fine_up);
                        break;
                    case RTU_SELECT_COM2 :
                        XPLMCommandOnce(stby_com2_fine_up_833);
                        break;
                }
                break;
                
        }
    }
    return (XPLMCommandCallback_f)1;
}



void registerCommands(void) {

    XPLMDebugString("XHSI: creating custom commands and registering custom command handlers\n");


    // xhsi/nd_pilot/...

    // source
    source_nav1 = XPLMCreateCommand("xhsi/nd_pilot/source_nav1", "NAV source NAV1");
    XPLMRegisterCommandHandler(source_nav1, (XPLMCommandCallback_f)source_handler, 1, (void *)SOURCE_NAV1);
    source_nav2 = XPLMCreateCommand("xhsi/nd_pilot/source_nav2", "NAV source NAV2");
    XPLMRegisterCommandHandler(source_nav2, (XPLMCommandCallback_f)source_handler, 1, (void *)SOURCE_NAV2);
    source_fmc = XPLMCreateCommand("xhsi/nd_pilot/source_fmc", "NAV source FMC");
    XPLMRegisterCommandHandler(source_fmc, (XPLMCommandCallback_f)source_handler, 1, (void *)SOURCE_FMC);
    source_down = XPLMCreateCommand("xhsi/nd_pilot/source_down", "Previous NAV source");
    XPLMRegisterCommandHandler(source_down, (XPLMCommandCallback_f)source_handler, 1, (void *)DOWN);
    source_up = XPLMCreateCommand("xhsi/nd_pilot/source_up", "Next NAV source");
    XPLMRegisterCommandHandler(source_up, (XPLMCommandCallback_f)source_handler, 1, (void *)UP);
    source_cycle = XPLMCreateCommand("xhsi/nd_pilot/source_cycle", "Cycle through NAV sources");
    XPLMRegisterCommandHandler(source_cycle, (XPLMCommandCallback_f)source_handler, 1, (void *)CYCLE);

    // mode
    mode_app = XPLMCreateCommand("xhsi/nd_pilot/mode_app", "ND mode APP");
    XPLMRegisterCommandHandler(mode_app, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_APP);
    mode_vor = XPLMCreateCommand("xhsi/nd_pilot/mode_vor", "ND mode VOR");
    XPLMRegisterCommandHandler(mode_vor, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_VOR);
    mode_map = XPLMCreateCommand("xhsi/nd_pilot/mode_map", "ND mode MAP");
    XPLMRegisterCommandHandler(mode_map, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_MAP);
    mode_nav = XPLMCreateCommand("xhsi/nd_pilot/mode_nav", "ND mode NAV");
    XPLMRegisterCommandHandler(mode_nav, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_NAV);
    mode_pln = XPLMCreateCommand("xhsi/nd_pilot/mode_pln", "ND mode PLN");
    XPLMRegisterCommandHandler(mode_pln, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_PLN);
    mode_down = XPLMCreateCommand("xhsi/nd_pilot/mode_down", "Previous ND mode");
    XPLMRegisterCommandHandler(mode_down, (XPLMCommandCallback_f)mode_handler, 1, (void *)DOWN);
    mode_up = XPLMCreateCommand("xhsi/nd_pilot/mode_up", "Next ND mode");
    XPLMRegisterCommandHandler(mode_up, (XPLMCommandCallback_f)mode_handler, 1, (void *)UP);
    mode_cycle = XPLMCreateCommand("xhsi/nd_pilot/mode_cycle", "Cycle through ND modes");
    XPLMRegisterCommandHandler(mode_cycle, (XPLMCommandCallback_f)mode_handler, 1, (void *)CYCLE);
    mode_shuttle = XPLMCreateCommand("xhsi/nd_pilot/mode_shuttle", "Shuttle back and forth through ND modes");
    XPLMRegisterCommandHandler(mode_shuttle, (XPLMCommandCallback_f)mode_handler, 1, (void *)SHUTTLE);

    // ctr
    ctr_toggle = XPLMCreateCommand("xhsi/nd_pilot/mode_ctr_toggle", "Toggle ND map CTR");
    XPLMRegisterCommandHandler(ctr_toggle, (XPLMCommandCallback_f)ctr_handler, 1, (void *)TOGGLE);
    ctr_on = XPLMCreateCommand("xhsi/nd_pilot/mode_ctr_on", "ND map CTR on");
    XPLMRegisterCommandHandler(ctr_on, (XPLMCommandCallback_f)ctr_handler, 1, (void *)MODE_CENTERED);
    ctr_off = XPLMCreateCommand("xhsi/nd_pilot/mode_ctr_off", "ND map CTR off");
    XPLMRegisterCommandHandler(ctr_off, (XPLMCommandCallback_f)ctr_handler, 1, (void *)MODE_EXPANDED);

    // range
    range_10 = XPLMCreateCommand("xhsi/nd_pilot/range_10", "ND map range 10");
    XPLMRegisterCommandHandler(range_10, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_10);
    range_20 = XPLMCreateCommand("xhsi/nd_pilot/range_20", "ND map range 20");
    XPLMRegisterCommandHandler(range_20, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_20);
    range_40 = XPLMCreateCommand("xhsi/nd_pilot/range_40", "ND map range 40");
    XPLMRegisterCommandHandler(range_40, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_40);
    range_80 = XPLMCreateCommand("xhsi/nd_pilot/range_80", "ND map range 80");
    XPLMRegisterCommandHandler(range_80, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_80);
    range_160 = XPLMCreateCommand("xhsi/nd_pilot/range_160", "ND map range 160");
    XPLMRegisterCommandHandler(range_160, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_160);
    range_320 = XPLMCreateCommand("xhsi/nd_pilot/range_320", "ND map range 320");
    XPLMRegisterCommandHandler(range_320, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_320);
    range_640 = XPLMCreateCommand("xhsi/nd_pilot/range_640", "ND map range 640");
    XPLMRegisterCommandHandler(range_640, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_640);
    range_down = XPLMCreateCommand("xhsi/nd_pilot/range_down", "Decrease ND map range");
    XPLMRegisterCommandHandler(range_down, (XPLMCommandCallback_f)range_handler, 1, (void *)DOWN);
    range_up = XPLMCreateCommand("xhsi/nd_pilot/range_up", "Increase ND map range");
    XPLMRegisterCommandHandler(range_up, (XPLMCommandCallback_f)range_handler, 1, (void *)UP);
    range_cycle = XPLMCreateCommand("xhsi/nd_pilot/range_cycle", "Cycle through ND map ranges");
    XPLMRegisterCommandHandler(range_cycle, (XPLMCommandCallback_f)range_handler, 1, (void *)CYCLE);
    range_shuttle = XPLMCreateCommand("xhsi/nd_pilot/range_shuttle", "Shuttle back and forth through ND map ranges");
    XPLMRegisterCommandHandler(range_shuttle, (XPLMCommandCallback_f)range_handler, 1, (void *)SHUTTLE);

    // radio1
    radio1_adf = XPLMCreateCommand("xhsi/nd_pilot/radio1_adf", "ND radio1 ADF1");
    XPLMRegisterCommandHandler(radio1_adf, (XPLMCommandCallback_f)radio1_handler, 1, (void *)RADIO_ADF);
    radio1_off = XPLMCreateCommand("xhsi/nd_pilot/radio1_off", "ND radio1 OFF");
    XPLMRegisterCommandHandler(radio1_off, (XPLMCommandCallback_f)radio1_handler, 1, (void *)RADIO_OFF);
    radio1_nav = XPLMCreateCommand("xhsi/nd_pilot/radio1_nav", "ND radio1 NAV1");
    XPLMRegisterCommandHandler(radio1_nav, (XPLMCommandCallback_f)radio1_handler, 1, (void *)RADIO_NAV);
    radio1_down = XPLMCreateCommand("xhsi/nd_pilot/radio1_down", "Previous ND radio1");
    XPLMRegisterCommandHandler(radio1_down, (XPLMCommandCallback_f)radio1_handler, 1, (void *)DOWN);
    radio1_up = XPLMCreateCommand("xhsi/nd_pilot/radio1_up", "Next ND radio1");
    XPLMRegisterCommandHandler(radio1_up, (XPLMCommandCallback_f)radio1_handler, 1, (void *)UP);
//    radio1_cycle = XPLMCreateCommand("xhsi/nd_pilot/radio1_cycle", "Cycle ND radio1");
//    XPLMRegisterCommandHandler(radio1_cycle, (XPLMCommandCallback_f)radio1_handler, 1, (void *)CYCLE);
    radio1_shuttle = XPLMCreateCommand("xhsi/nd_pilot/radio1_shuttle", "Shuttle ND radio1");
    XPLMRegisterCommandHandler(radio1_shuttle, (XPLMCommandCallback_f)radio1_handler, 1, (void *)SHUTTLE);

    // radio2
    radio2_adf = XPLMCreateCommand("xhsi/nd_pilot/radio2_adf", "ND radio2 ADF");
    XPLMRegisterCommandHandler(radio2_adf, (XPLMCommandCallback_f)radio2_handler, 1, (void *)RADIO_ADF);
    radio2_off = XPLMCreateCommand("xhsi/nd_pilot/radio2_off", "ND radio2 OFF");
    XPLMRegisterCommandHandler(radio2_off, (XPLMCommandCallback_f)radio2_handler, 1, (void *)RADIO_OFF);
    radio2_nav = XPLMCreateCommand("xhsi/nd_pilot/radio2_nav", "ND radio2 NAV");
    XPLMRegisterCommandHandler(radio2_nav, (XPLMCommandCallback_f)radio2_handler, 1, (void *)RADIO_NAV);
    radio2_down = XPLMCreateCommand("xhsi/nd_pilot/radio2_down", "Previous ND radio2");
    XPLMRegisterCommandHandler(radio2_down, (XPLMCommandCallback_f)radio2_handler, 1, (void *)DOWN);
    radio2_up = XPLMCreateCommand("xhsi/nd_pilot/radio2_up", "Next ND radio2");
    XPLMRegisterCommandHandler(radio2_up, (XPLMCommandCallback_f)radio2_handler, 1, (void *)UP);
//    radio2_cycle = XPLMCreateCommand("xhsi/nd_pilot/radio2_cycle", "Cycle ND radio2");
//    XPLMRegisterCommandHandler(radio2_cycle, (XPLMCommandCallback_f)radio2_handler, 1, (void *)CYCLE);
    radio2_shuttle = XPLMCreateCommand("xhsi/nd_pilot/radio2_shuttle", "Shuttle ND radio2");
    XPLMRegisterCommandHandler(radio2_shuttle, (XPLMCommandCallback_f)radio2_handler, 1, (void *)SHUTTLE);

    // tfc
    tfc_toggle = XPLMCreateCommand("xhsi/nd_pilot/tfc_toggle", "Toggle ND symbols TFC");
    XPLMRegisterCommandHandler(tfc_toggle, (XPLMCommandCallback_f)tfc_handler, 1, (void *)TOGGLE);
    tfc_on = XPLMCreateCommand("xhsi/nd_pilot/tfc_on", "ND symbols TFC on");
    XPLMRegisterCommandHandler(tfc_on, (XPLMCommandCallback_f)tfc_handler, 1, (void *)ON);
    tfc_off = XPLMCreateCommand("xhsi/nd_pilot/tfc_off", "ND symbols TFC off");
    XPLMRegisterCommandHandler(tfc_off, (XPLMCommandCallback_f)tfc_handler, 1, (void *)OFF);

    // arpt
    arpt_toggle = XPLMCreateCommand("xhsi/nd_pilot/arpt_toggle", "Toggle ND symbols ARPT");
    XPLMRegisterCommandHandler(arpt_toggle, (XPLMCommandCallback_f)arpt_handler, 1, (void *)TOGGLE);
    arpt_on = XPLMCreateCommand("xhsi/nd_pilot/arpt_on", "ND symbols ARPT on");
    XPLMRegisterCommandHandler(arpt_on, (XPLMCommandCallback_f)arpt_handler, 1, (void *)ON);
    arpt_off = XPLMCreateCommand("xhsi/nd_pilot/arpt_off", "ND symbols ARPT off");
    XPLMRegisterCommandHandler(arpt_off, (XPLMCommandCallback_f)arpt_handler, 1, (void *)OFF);

    // wpt
    wpt_toggle = XPLMCreateCommand("xhsi/nd_pilot/wpt_toggle", "Toggle ND symbols WPT");
    XPLMRegisterCommandHandler(wpt_toggle, (XPLMCommandCallback_f)wpt_handler, 1, (void *)TOGGLE);
    wpt_on = XPLMCreateCommand("xhsi/nd_pilot/wpt_on", "ND symbols WPT on");
    XPLMRegisterCommandHandler(wpt_on, (XPLMCommandCallback_f)wpt_handler, 1, (void *)ON);
    wpt_off = XPLMCreateCommand("xhsi/nd_pilot/wpt_off", "ND symbols WPT off");
    XPLMRegisterCommandHandler(wpt_off, (XPLMCommandCallback_f)wpt_handler, 1, (void *)OFF);

    // vor
    vor_toggle = XPLMCreateCommand("xhsi/nd_pilot/vor_toggle", "Toggle ND symbols VOR");
    XPLMRegisterCommandHandler(vor_toggle, (XPLMCommandCallback_f)vor_handler, 1, (void *)TOGGLE);
    vor_on = XPLMCreateCommand("xhsi/nd_pilot/vor_on", "ND symbols VOR on");
    XPLMRegisterCommandHandler(vor_on, (XPLMCommandCallback_f)vor_handler, 1, (void *)ON);
    vor_off = XPLMCreateCommand("xhsi/nd_pilot/vor_off", "ND symbols VOR off");
    XPLMRegisterCommandHandler(vor_off, (XPLMCommandCallback_f)vor_handler, 1, (void *)OFF);

    // ndb
    ndb_toggle = XPLMCreateCommand("xhsi/nd_pilot/ndb_toggle", "Toggle ND symbols NDB");
    XPLMRegisterCommandHandler(ndb_toggle, (XPLMCommandCallback_f)ndb_handler, 1, (void *)TOGGLE);
    ndb_on = XPLMCreateCommand("xhsi/nd_pilot/ndb_on", "ND symbols NDB on");
    XPLMRegisterCommandHandler(ndb_on, (XPLMCommandCallback_f)ndb_handler, 1, (void *)ON);
    ndb_off = XPLMCreateCommand("xhsi/nd_pilot/ndb_off", "ND symbols NDB off");
    XPLMRegisterCommandHandler(ndb_off, (XPLMCommandCallback_f)ndb_handler, 1, (void *)OFF);

    // sta = vor + ndb
    sta_toggle = XPLMCreateCommand("xhsi/nd_pilot/sta_toggle", "Toggle ND symbols STA");
    XPLMRegisterCommandHandler(sta_toggle, (XPLMCommandCallback_f)sta_handler, 1, (void *)TOGGLE);
    sta_on = XPLMCreateCommand("xhsi/nd_pilot/sta_on", "ND symbols STA on");
    XPLMRegisterCommandHandler(sta_on, (XPLMCommandCallback_f)sta_handler, 1, (void *)ON);
    sta_off = XPLMCreateCommand("xhsi/nd_pilot/sta_off", "ND symbols STA off");
    XPLMRegisterCommandHandler(sta_off, (XPLMCommandCallback_f)sta_handler, 1, (void *)OFF);
    sta_cycle = XPLMCreateCommand("xhsi/nd_pilot/sta_cycle", "Cycle ND symbols STA");
    XPLMRegisterCommandHandler(sta_cycle, (XPLMCommandCallback_f)sta_handler, 1, (void *)CYCLE);

    // declutter : progressively hide map symbols
    declutter_cycle = XPLMCreateCommand("xhsi/nd_pilot/declutter", "ND declutter");
    XPLMRegisterCommandHandler(declutter_cycle, (XPLMCommandCallback_f)declutter_handler, 1, (void *)CYCLE);

    // data = route data
    data_toggle = XPLMCreateCommand("xhsi/nd_pilot/data_toggle", "Toggle ND symbols DATA");
    XPLMRegisterCommandHandler(data_toggle, (XPLMCommandCallback_f)data_handler, 1, (void *)TOGGLE);
    data_on = XPLMCreateCommand("xhsi/nd_pilot/data_on", "ND symbols DATA on");
    XPLMRegisterCommandHandler(data_on, (XPLMCommandCallback_f)data_handler, 1, (void *)ON);
    data_off = XPLMCreateCommand("xhsi/nd_pilot/data_off", "ND symbols DATA off");
    XPLMRegisterCommandHandler(data_off, (XPLMCommandCallback_f)data_handler, 1, (void *)OFF);

    // pos
    pos_toggle = XPLMCreateCommand("xhsi/nd_pilot/pos_toggle", "Toggle ND symbols POS");
    XPLMRegisterCommandHandler(pos_toggle, (XPLMCommandCallback_f)pos_handler, 1, (void *)TOGGLE);
    pos_on = XPLMCreateCommand("xhsi/nd_pilot/pos_on", "ND symbols POS on");
    XPLMRegisterCommandHandler(pos_on, (XPLMCommandCallback_f)pos_handler, 1, (void *)ON);
    pos_off = XPLMCreateCommand("xhsi/nd_pilot/pos_off", "ND symbols POS off");
    XPLMRegisterCommandHandler(pos_off, (XPLMCommandCallback_f)pos_handler, 1, (void *)OFF);

    // terrain on ND
    terrain_toggle = XPLMCreateCommand("xhsi/nd_pilot/terrain_toggle", "Toggle Terrain on ND");
    XPLMRegisterCommandHandler(terrain_toggle, (XPLMCommandCallback_f)terrain_handler, 1, (void *)TOGGLE);
    terrain_on = XPLMCreateCommand("xhsi/nd_pilot/terrain_on", "Terrain on ND on");
    XPLMRegisterCommandHandler(terrain_on, (XPLMCommandCallback_f)terrain_handler, 1, (void *)ON);
    terrain_off = XPLMCreateCommand("xhsi/nd_pilot/terrain_off", "Terrain on ND off");
    XPLMRegisterCommandHandler(terrain_off, (XPLMCommandCallback_f)terrain_handler, 1, (void *)OFF);

    // Vertical Path on ND
    vp_toggle = XPLMCreateCommand("xhsi/nd_pilot/vertical_path_toggle", "Toggle Vertical Path on ND");
    XPLMRegisterCommandHandler(vp_toggle, (XPLMCommandCallback_f)vertical_path_handler, 1, (void *)TOGGLE);
    vp_on = XPLMCreateCommand("xhsi/nd_pilot/vertical_path_on", "Vertical Path on ND on");
    XPLMRegisterCommandHandler(vp_on, (XPLMCommandCallback_f)vertical_path_handler, 1, (void *)ON);
    vp_off = XPLMCreateCommand("xhsi/nd_pilot/vertical_path_off", "Vertical Path on ND off");
    XPLMRegisterCommandHandler(vp_off, (XPLMCommandCallback_f)vertical_path_handler, 1, (void *)OFF);

    // Weather Radar
    wxr_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_toggle", "Toggle weather radar on ND");
    XPLMRegisterCommandHandler(wxr_toggle, (XPLMCommandCallback_f)wxr_handler, 1, (void *)TOGGLE);
    wxr_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_on", "Weather radar on ND");
    XPLMRegisterCommandHandler(wxr_on, (XPLMCommandCallback_f)wxr_handler, 1, (void *)ON);
    wxr_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_off", "Weather radar off ND");
    XPLMRegisterCommandHandler(wxr_off, (XPLMCommandCallback_f)wxr_handler, 1, (void *)OFF);

    wxr_gain_up = XPLMCreateCommand("xhsi/nd_pilot/wxr_gain_up", "Weather radar gain up +0.05");
    XPLMRegisterCommandHandler(wxr_gain_up, (XPLMCommandCallback_f)wxr_gain_handler, 1, (void *)UP);
    wxr_gain_down = XPLMCreateCommand("xhsi/nd_pilot/wxr_gain_down", "Weather radar gain down -0.05");
    XPLMRegisterCommandHandler(wxr_gain_down, (XPLMCommandCallback_f)wxr_gain_handler, 1, (void *)DOWN);
    wxr_gain_middle = XPLMCreateCommand("xhsi/nd_pilot/wxr_gain_middle", "Set weather radar gain to 0.50");
    XPLMRegisterCommandHandler(wxr_gain_middle, (XPLMCommandCallback_f)wxr_gain_handler, 1, (void *)MODE_CENTERED);

    wxr_tilt_up = XPLMCreateCommand("xhsi/nd_pilot/wxr_tilt_up", "Weather radar tilt up 1 degree");
    XPLMRegisterCommandHandler(wxr_tilt_up, (XPLMCommandCallback_f)wxr_tilt_handler, 1, (void *)UP);
    wxr_tilt_down = XPLMCreateCommand("xhsi/nd_pilot/wxr_tilt_down", "Weather radar tilt down 1 degree");
    XPLMRegisterCommandHandler(wxr_tilt_down, (XPLMCommandCallback_f)wxr_tilt_handler, 1, (void *)DOWN);
    wxr_tilt_zero = XPLMCreateCommand("xhsi/nd_pilot/wxr_tilt_zero", "Set weather radar tilt to 0");
    XPLMRegisterCommandHandler(wxr_tilt_zero, (XPLMCommandCallback_f)wxr_tilt_handler, 1, (void *)MODE_CENTERED);

    wxr_auto_gain_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_auto_gain_toggle", "Toggle weather radar auto gain");
    XPLMRegisterCommandHandler(wxr_auto_gain_toggle, (XPLMCommandCallback_f)wxr_auto_gain_handler, 1, (void *)TOGGLE);
    wxr_auto_gain_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_auto_gain_on", "Weather radar auto gain on");
    XPLMRegisterCommandHandler(wxr_auto_gain_on, (XPLMCommandCallback_f)wxr_auto_gain_handler, 1, (void *)ON);
    wxr_auto_gain_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_auto_gain_off", "Weather radar auto gain off");
    XPLMRegisterCommandHandler(wxr_auto_gain_off, (XPLMCommandCallback_f)wxr_auto_gain_handler, 1, (void *)OFF);

    wxr_auto_tilt_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_auto_tilt_toggle", "Toggle weather radar auto tilt");
    XPLMRegisterCommandHandler(wxr_auto_tilt_toggle, (XPLMCommandCallback_f)wxr_auto_tilt_handler, 1, (void *)TOGGLE);
    wxr_auto_tilt_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_auto_tilt_on", "Weather radar auto tilt on");
    XPLMRegisterCommandHandler(wxr_auto_tilt_on, (XPLMCommandCallback_f)wxr_auto_tilt_handler, 1, (void *)ON);
    wxr_auto_tilt_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_auto_tilt_off", "Weather radar auto tilt off");
    XPLMRegisterCommandHandler(wxr_auto_tilt_off, (XPLMCommandCallback_f)wxr_auto_tilt_handler, 1, (void *)OFF);

    wxr_test_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_test_toggle", "Toggle weather radar test");
    XPLMRegisterCommandHandler(wxr_test_toggle, (XPLMCommandCallback_f)wxr_test_handler, 1, (void *)TOGGLE);
    wxr_test_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_test_on", "Weather radar auto tilt on");
    XPLMRegisterCommandHandler(wxr_test_on, (XPLMCommandCallback_f)wxr_test_handler, 1, (void *)ON);
    wxr_test_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_test_off", "Weather radar auto tilt off");
    XPLMRegisterCommandHandler(wxr_test_off, (XPLMCommandCallback_f)wxr_test_handler, 1, (void *)OFF);

    wxr_mode_up = XPLMCreateCommand("xhsi/nd_pilot/wxr_mode_up", "Weather radar mode up");
    XPLMRegisterCommandHandler(wxr_mode_up, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)UP);
    wxr_mode_down = XPLMCreateCommand("xhsi/nd_pilot/wxr_mode_down", "Weather radar mode down");
    XPLMRegisterCommandHandler(wxr_mode_down, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)DOWN);
    wxr_mode_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_mode_off", "Weather radar mode OFF");
    XPLMRegisterCommandHandler(wxr_mode_off, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_OFF);
    wxr_mode_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_mode_on", "Weather radar mode ON");
    XPLMRegisterCommandHandler(wxr_mode_on, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_ON);
    wxr_mode_turb = XPLMCreateCommand("xhsi/nd_pilot/wxr_mode_turb", "Weather radar mode TURB");
    XPLMRegisterCommandHandler(wxr_mode_turb, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_TURB);
    wxr_mode_map = XPLMCreateCommand("xhsi/nd_pilot/wxr_mode_map", "Weather radar mode MAP");
    XPLMRegisterCommandHandler(wxr_mode_map, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_MAP);
    wxr_mode_force_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_mode_force_on", "Weather radar mode FORCE ON");
    XPLMRegisterCommandHandler(wxr_mode_force_on, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_FORCE_ON);

    wxr_slave_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_slave_toggle", "Toggle weather salve mode");
    XPLMRegisterCommandHandler(wxr_slave_toggle, (XPLMCommandCallback_f)wxr_slave_handler, 1, (void *)TOGGLE);
    wxr_slave_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_slave_on", "Weather radar slave on [F/O side]");
    XPLMRegisterCommandHandler(wxr_slave_on, (XPLMCommandCallback_f)wxr_slave_handler, 1, (void *)ON);
    wxr_slave_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_slave_off", "Weather radar slave off");
    XPLMRegisterCommandHandler(wxr_slave_off, (XPLMCommandCallback_f)wxr_slave_handler, 1, (void *)OFF);

    wxr_react_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_react_toggle", "Toggle weather REACT mode");
    XPLMRegisterCommandHandler(wxr_react_toggle, (XPLMCommandCallback_f)wxr_react_handler, 1, (void *)TOGGLE);
    wxr_react_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_react_on", "Weather radar REACT on");
    XPLMRegisterCommandHandler(wxr_react_on, (XPLMCommandCallback_f)wxr_react_handler, 1, (void *)ON);
    wxr_react_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_react_off", "Weather radar REACT off");
    XPLMRegisterCommandHandler(wxr_react_off, (XPLMCommandCallback_f)wxr_react_handler, 1, (void *)OFF);

    wxr_narrow_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_narrow_toggle", "Toggle weather narrow mode");
    XPLMRegisterCommandHandler(wxr_narrow_toggle, (XPLMCommandCallback_f)wxr_narrow_handler, 1, (void *)TOGGLE);
    wxr_narrow_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_narrow_on", "Weather radar narrow mode on");
    XPLMRegisterCommandHandler(wxr_narrow_on, (XPLMCommandCallback_f)wxr_narrow_handler, 1, (void *)ON);
    wxr_narrow_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_narrow_off", "Weather radar narrow mode off");
    XPLMRegisterCommandHandler(wxr_narrow_off, (XPLMCommandCallback_f)wxr_narrow_handler, 1, (void *)OFF);

    wxr_alert_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_alert_toggle", "Toggle weather alert mode");
    XPLMRegisterCommandHandler(wxr_alert_toggle, (XPLMCommandCallback_f)wxr_alert_handler, 1, (void *)TOGGLE);
    wxr_alert_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_alert_on", "Weather radar narrow alert mode on");
    XPLMRegisterCommandHandler(wxr_alert_on, (XPLMCommandCallback_f)wxr_alert_handler, 1, (void *)ON);
    wxr_alert_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_alert_off", "Weather radar alert mode off");
    XPLMRegisterCommandHandler(wxr_alert_off, (XPLMCommandCallback_f)wxr_alert_handler, 1, (void *)OFF);

    wxr_target_toggle = XPLMCreateCommand("xhsi/nd_pilot/wxr_target_toggle", "Toggle weather target mode");
    XPLMRegisterCommandHandler(wxr_target_toggle, (XPLMCommandCallback_f)wxr_target_handler, 1, (void *)TOGGLE);
    wxr_target_on = XPLMCreateCommand("xhsi/nd_pilot/wxr_target_on", "Weather radar narrow target mode on");
    XPLMRegisterCommandHandler(wxr_target_on, (XPLMCommandCallback_f)wxr_target_handler, 1, (void *)ON);
    wxr_target_off = XPLMCreateCommand("xhsi/nd_pilot/wxr_target_off", "Weather radar target mode off");
    XPLMRegisterCommandHandler(wxr_target_off, (XPLMCommandCallback_f)wxr_target_handler, 1, (void *)OFF);

    // xhsi/nd_ext_range_pilot/...

    // zoomin
    zoom_x100_toggle = XPLMCreateCommand("xhsi/nd_ext_range_pilot/zoom_x100_toggle", "Toggle ND map zoom x100");
    XPLMRegisterCommandHandler(zoom_x100_toggle, (XPLMCommandCallback_f)zoom_x100_handler, 1, (void *)TOGGLE);
    zoom_x100_on = XPLMCreateCommand("xhsi/nd_ext_range_pilot/zoom_x100_on", "ND map range zoom in x100");
    XPLMRegisterCommandHandler(zoom_x100_on, (XPLMCommandCallback_f)zoom_x100_handler, 1, (void *)ON);
    zoom_x100_off = XPLMCreateCommand("xhsi/nd_ext_range_pilot/zoom_x100_off", "ND map range normal");
    XPLMRegisterCommandHandler(zoom_x100_off, (XPLMCommandCallback_f)zoom_x100_handler, 1, (void *)OFF);

    // ext range
    ext_range_0_10 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_0.10", "ND extended map range 0.10");
    XPLMRegisterCommandHandler(ext_range_0_10, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_10 + 1100));
    ext_range_0_20 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_0.20", "ND extended map range 0.20");
    XPLMRegisterCommandHandler(ext_range_0_20, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_20 + 1100));
    ext_range_0_40 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_0.40", "ND extended map range 0.40");
    XPLMRegisterCommandHandler(ext_range_0_40, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_40 + 1100));
    ext_range_0_80 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_0.80", "ND extended map range 0.80");
    XPLMRegisterCommandHandler(ext_range_0_80, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_80 + 1100));
    ext_range_1_60 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_1.60", "ND extended map range 1.60");
    XPLMRegisterCommandHandler(ext_range_1_60, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_160 + 1100));
    ext_range_3_20 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_3.20", "ND extended map range 3.20");
    XPLMRegisterCommandHandler(ext_range_3_20, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_320 + 1100));
    ext_range_6_40 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_6.40", "ND extended map range 6.40");
    XPLMRegisterCommandHandler(ext_range_6_40, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_640 + 1100));
    ext_range_010 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_010", "ND extended map range 10");
    XPLMRegisterCommandHandler(ext_range_010, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_10 + 1000));
    ext_range_020 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_020", "ND extended map range 20");
    XPLMRegisterCommandHandler(ext_range_020, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_20 + 1000));
    ext_range_040 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_040", "ND extended map range 40");
    XPLMRegisterCommandHandler(ext_range_040, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_40 + 1000));
    ext_range_080 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_080", "ND extended map range 80");
    XPLMRegisterCommandHandler(ext_range_080, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_80 + 1000));
    ext_range_160 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_160", "ND extended map range 160");
    XPLMRegisterCommandHandler(ext_range_160, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_160 + 1000));
    ext_range_320 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_320", "ND extended map range 320");
    XPLMRegisterCommandHandler(ext_range_320, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_320 + 1000));
    ext_range_640 = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_640", "ND extended map range 640");
    XPLMRegisterCommandHandler(ext_range_640, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_640 + 1000));
    ext_range_down = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_down", "Decrease ND extended map range");
    XPLMRegisterCommandHandler(ext_range_down, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)DOWN);
    ext_range_up = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_up", "Increase ND extended map range");
    XPLMRegisterCommandHandler(ext_range_up, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)UP);
    ext_range_cycle = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_cycle", "Cycle through ND extended map ranges");
    XPLMRegisterCommandHandler(ext_range_cycle, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)CYCLE);
    ext_range_shuttle = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_shuttle", "Shuttle back and forth through ND extended map ranges");
    XPLMRegisterCommandHandler(ext_range_shuttle, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)SHUTTLE);



    // xhsi/nd_b737classic_pilot/...

    // B737-Classic modes
    b737cl_mode_fullvorils = XPLMCreateCommand("xhsi/nd_b737classic_pilot/mode_fullvorils", "B737-Classic mode FULL VOR/ILS");
    XPLMRegisterCommandHandler(b737cl_mode_fullvorils, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_FULL_VOR_ILS);
    b737cl_mode_expvorils = XPLMCreateCommand("xhsi/nd_b737classic_pilot/mode_expvorils", "B737-Classic mode EXP VOR/ILS");
    XPLMRegisterCommandHandler(b737cl_mode_expvorils, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_EXP_VOR_ILS);
    b737cl_mode_map = XPLMCreateCommand("xhsi/nd_b737classic_pilot/mode_map", "B737-Classic mode MAP");
    XPLMRegisterCommandHandler(b737cl_mode_map, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_MAP);
    b737cl_mode_ctrmap = XPLMCreateCommand("xhsi/nd_b737classic_pilot/mode_ctrmap", "B737-Classic mode CTR MAP");
    XPLMRegisterCommandHandler(b737cl_mode_ctrmap, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_CTR_MAP);
    b737cl_mode_plan = XPLMCreateCommand("xhsi/nd_b737classic_pilot/mode_plan", "B737-Classic mode PLAN");
    XPLMRegisterCommandHandler(b737cl_mode_plan, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_PLAN);



    // xhsi/pfd_pilot/...

    // mins mode
    mins_toggle = XPLMCreateCommand("xhsi/pfd_pilot/mins_toggle", "Toggle MINS selector");
    XPLMRegisterCommandHandler(mins_toggle, (XPLMCommandCallback_f)mins_mode_handler, 1, (void *)TOGGLE);
    mins_radio = XPLMCreateCommand("xhsi/pfd_pilot/mins_radio", "Select RADIO MINS");
    XPLMRegisterCommandHandler(mins_radio, (XPLMCommandCallback_f)mins_mode_handler, 1, (void *)MINS_RADIO);
    mins_baro = XPLMCreateCommand("xhsi/pfd_pilot/mins_baro", "Select BARO MINS");
    XPLMRegisterCommandHandler(mins_baro, (XPLMCommandCallback_f)mins_mode_handler, 1, (void *)MINS_BARO);

    // mins value
    mins_reset = XPLMCreateCommand("xhsi/pfd_pilot/mins_reset", "Reset MINS value");
    XPLMRegisterCommandHandler(mins_reset, (XPLMCommandCallback_f)mins_value_handler, 1, (void *)OFF);
    mins_down = XPLMCreateCommand("xhsi/pfd_pilot/mins_down", "Decrease MINS value");
    XPLMRegisterCommandHandler(mins_down, (XPLMCommandCallback_f)mins_value_handler, 1, (void *)DOWN);
    mins_up = XPLMCreateCommand("xhsi/pfd_pilot/mins_up", "Increase MINS value");
    XPLMRegisterCommandHandler(mins_up, (XPLMCommandCallback_f)mins_value_handler, 1, (void *)UP);

    // Instrument Landing System (PFD & ND)
    ils_toggle = XPLMCreateCommand("xhsi/pfd_pilot/ils_toggle", "Toggle ILS on PFD/ND");
    XPLMRegisterCommandHandler(ils_toggle, (XPLMCommandCallback_f)ils_handler, 1, (void *)TOGGLE);
    ils_on = XPLMCreateCommand("xhsi/pfd_pilot/ils_on", "ILS on PFD/ND on");
    XPLMRegisterCommandHandler(ils_on, (XPLMCommandCallback_f)ils_handler, 1, (void *)ON);
    ils_off = XPLMCreateCommand("xhsi/pfd_pilot/ils_off", "ILS on PFD/ND off");
    XPLMRegisterCommandHandler(ils_off, (XPLMCommandCallback_f)ils_handler, 1, (void *)OFF);

    // Metric altitude on PFD/ND
    metric_alt_toggle = XPLMCreateCommand("xhsi/pfd_pilot/metric_alt_toggle", "Toggle metric altitude on PFD/ND");
    XPLMRegisterCommandHandler(metric_alt_toggle, (XPLMCommandCallback_f)metric_alt_handler, 1, (void *)TOGGLE);
    metric_alt_on = XPLMCreateCommand("xhsi/pfd_pilot/metric_alt_on", "metric altitude on PFD/ND on");
    XPLMRegisterCommandHandler(metric_alt_on, (XPLMCommandCallback_f)metric_alt_handler, 1, (void *)ON);
    metric_alt_off = XPLMCreateCommand("xhsi/pfd_pilot/metric_alt_off", "metric altitude on PFD/ND off");
    XPLMRegisterCommandHandler(metric_alt_off, (XPLMCommandCallback_f)metric_alt_handler, 1, (void *)OFF);

    // Track FPA / FPV / Bird : on display FPA/FPV/Bird , off display flight director
    track_fpa_toggle = XPLMCreateCommand("xhsi/pfd_pilot/track_fpa_toggle", "Toggle Track FPA on PFD");
    XPLMRegisterCommandHandler(track_fpa_toggle, (XPLMCommandCallback_f)track_fpa_handler, 1, (void *)TOGGLE);
    track_fpa_on = XPLMCreateCommand("xhsi/pfd_pilot/track_fpa_on", "Track FPA on PFD on");
    XPLMRegisterCommandHandler(track_fpa_on, (XPLMCommandCallback_f)track_fpa_handler, 1, (void *)ON);
    track_fpa_off = XPLMCreateCommand("xhsi/pfd_pilot/track_fpa_off", "Track FPA on PFD off");
    XPLMRegisterCommandHandler(track_fpa_off, (XPLMCommandCallback_f)track_fpa_handler, 1, (void *)OFF);


    // copilot commands

    // xhsi/nd_copilot/...

    // copilot source
    copilot_source_nav1 = XPLMCreateCommand("xhsi/nd_copilot/source_nav1", "NAV source NAV1 - copilot");
    XPLMRegisterCommandHandler(copilot_source_nav1, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)SOURCE_NAV1);
    copilot_source_nav2 = XPLMCreateCommand("xhsi/nd_copilot/source_nav2", "NAV source NAV2 - copilot");
    XPLMRegisterCommandHandler(copilot_source_nav2, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)SOURCE_NAV2);
    copilot_source_fmc = XPLMCreateCommand("xhsi/nd_copilot/source_fmc", "NAV source FMC - copilot");
    XPLMRegisterCommandHandler(copilot_source_fmc, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)SOURCE_FMC);
    copilot_source_down = XPLMCreateCommand("xhsi/nd_copilot/source_down", "Previous NAV source - copilot");
    XPLMRegisterCommandHandler(copilot_source_down, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)DOWN);
    copilot_source_up = XPLMCreateCommand("xhsi/nd_copilot/source_up", "Next NAV source - copilot");
    XPLMRegisterCommandHandler(copilot_source_up, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)UP);
    copilot_source_cycle = XPLMCreateCommand("xhsi/nd_copilot/source_cycle", "Cycle through NAV sources - copilot");
    XPLMRegisterCommandHandler(copilot_source_cycle, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)CYCLE);

    // copilot mode
    copilot_mode_app = XPLMCreateCommand("xhsi/nd_copilot/mode_app", "ND mode APP - copilot");
    XPLMRegisterCommandHandler(copilot_mode_app, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_APP);
    copilot_mode_vor = XPLMCreateCommand("xhsi/nd_copilot/mode_vor", "ND mode VOR - copilot");
    XPLMRegisterCommandHandler(copilot_mode_vor, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_VOR);
    copilot_mode_map = XPLMCreateCommand("xhsi/nd_copilot/mode_map", "ND mode MAP - copilot");
    XPLMRegisterCommandHandler(copilot_mode_map, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_MAP);
    copilot_mode_nav = XPLMCreateCommand("xhsi/nd_copilot/mode_nav", "ND mode NAV - copilot");
    XPLMRegisterCommandHandler(copilot_mode_nav, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_NAV);
    copilot_mode_pln = XPLMCreateCommand("xhsi/nd_copilot/mode_pln", "ND mode PLN - copilot");
    XPLMRegisterCommandHandler(copilot_mode_pln, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_PLN);
    copilot_mode_down = XPLMCreateCommand("xhsi/nd_copilot/mode_down", "Previous ND mode - copilot");
    XPLMRegisterCommandHandler(copilot_mode_down, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)DOWN);
    copilot_mode_up = XPLMCreateCommand("xhsi/nd_copilot/mode_up", "Next ND mode - copilot");
    XPLMRegisterCommandHandler(copilot_mode_up, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)UP);
    copilot_mode_cycle = XPLMCreateCommand("xhsi/nd_copilot/mode_cycle", "Cycle through ND modes - copilot");
    XPLMRegisterCommandHandler(copilot_mode_cycle, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)CYCLE);
    copilot_mode_shuttle = XPLMCreateCommand("xhsi/nd_copilot/mode_shuttle", "Shuttle back and forth through ND modes - copilot");
    XPLMRegisterCommandHandler(copilot_mode_shuttle, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)SHUTTLE);

    // copilot ctr
    copilot_ctr_toggle = XPLMCreateCommand("xhsi/nd_copilot/mode_ctr_toggle", "Toggle ND map CTR - copilot");
    XPLMRegisterCommandHandler(copilot_ctr_toggle, (XPLMCommandCallback_f)copilot_ctr_handler, 1, (void *)TOGGLE);
    copilot_ctr_on = XPLMCreateCommand("xhsi/nd_copilot/mode_ctr_on", "ND map CTR on - copilot");
    XPLMRegisterCommandHandler(copilot_ctr_on, (XPLMCommandCallback_f)copilot_ctr_handler, 1, (void *)MODE_CENTERED);
    copilot_ctr_off = XPLMCreateCommand("xhsi/nd_copilot/mode_ctr_off", "ND map CTR off - copilot");
    XPLMRegisterCommandHandler(copilot_ctr_off, (XPLMCommandCallback_f)copilot_ctr_handler, 1, (void *)MODE_EXPANDED);

    // copilot range
    copilot_range_10 = XPLMCreateCommand("xhsi/nd_copilot/range_10", "ND map range 10 - copilot");
    XPLMRegisterCommandHandler(copilot_range_10, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_10);
    copilot_range_20 = XPLMCreateCommand("xhsi/nd_copilot/range_20", "ND map range 20 - copilot");
    XPLMRegisterCommandHandler(copilot_range_20, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_20);
    copilot_range_40 = XPLMCreateCommand("xhsi/nd_copilot/range_40", "ND map range 40 - copilot");
    XPLMRegisterCommandHandler(copilot_range_40, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_40);
    copilot_range_80 = XPLMCreateCommand("xhsi/nd_copilot/range_80", "ND map range 80 - copilot");
    XPLMRegisterCommandHandler(copilot_range_80, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_80);
    copilot_range_160 = XPLMCreateCommand("xhsi/nd_copilot/range_160", "ND map range 160 - copilot");
    XPLMRegisterCommandHandler(copilot_range_160, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_160);
    copilot_range_320 = XPLMCreateCommand("xhsi/nd_copilot/range_320", "ND map range 320 - copilot");
    XPLMRegisterCommandHandler(copilot_range_320, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_320);
    copilot_range_640 = XPLMCreateCommand("xhsi/nd_copilot/range_640", "ND map range 640 - copilot");
    XPLMRegisterCommandHandler(copilot_range_640, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_640);
    copilot_range_down = XPLMCreateCommand("xhsi/nd_copilot/range_down", "Decrease ND map range - copilot");
    XPLMRegisterCommandHandler(copilot_range_down, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)DOWN);
    copilot_range_up = XPLMCreateCommand("xhsi/nd_copilot/range_up", "Increase ND map range - copilot");
    XPLMRegisterCommandHandler(copilot_range_up, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)UP);
    copilot_range_cycle = XPLMCreateCommand("xhsi/nd_copilot/range_cycle", "Cycle through ND map ranges - copilot");
    XPLMRegisterCommandHandler(copilot_range_cycle, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)CYCLE);
    copilot_range_shuttle = XPLMCreateCommand("xhsi/nd_copilot/range_shuttle", "Shuttle back and forth through ND map ranges - copilot");
    XPLMRegisterCommandHandler(copilot_range_shuttle, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)SHUTTLE);

    // copilot radio1
    copilot_radio1_adf = XPLMCreateCommand("xhsi/nd_copilot/radio1_adf", "ND radio1 ADF1 - copilot");
    XPLMRegisterCommandHandler(copilot_radio1_adf, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)RADIO_ADF);
    copilot_radio1_off = XPLMCreateCommand("xhsi/nd_copilot/radio1_off", "ND radio1 OFF - copilot");
    XPLMRegisterCommandHandler(copilot_radio1_off, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)RADIO_OFF);
    copilot_radio1_nav = XPLMCreateCommand("xhsi/nd_copilot/radio1_nav", "ND radio1 NAV1 - copilot");
    XPLMRegisterCommandHandler(copilot_radio1_nav, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)RADIO_NAV);
    copilot_radio1_down = XPLMCreateCommand("xhsi/nd_copilot/radio1_down", "Previous ND radio1 - copilot");
    XPLMRegisterCommandHandler(copilot_radio1_down, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)DOWN);
    copilot_radio1_up = XPLMCreateCommand("xhsi/nd_copilot/radio1_up", "Next ND radio1 - copilot");
    XPLMRegisterCommandHandler(copilot_radio1_up, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)UP);
//    copilot_radio1_cycle = XPLMCreateCommand("xhsi/nd_copilot/radio1_cycle", "Cycle ND radio1 - copilot");
//    XPLMRegisterCommandHandler(copilot_radio1_cycle, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)CYCLE);
    copilot_radio1_shuttle = XPLMCreateCommand("xhsi/nd_copilot/radio1_shuttle", "Shuttle ND radio1 - copilot");
    XPLMRegisterCommandHandler(copilot_radio1_shuttle, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)SHUTTLE);

    // copilot radio2
    copilot_radio2_adf = XPLMCreateCommand("xhsi/nd_copilot/radio2_adf", "ND radio2 ADF2 - copilot");
    XPLMRegisterCommandHandler(copilot_radio2_adf, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)RADIO_ADF);
    copilot_radio2_off = XPLMCreateCommand("xhsi/nd_copilot/radio2_off", "ND radio2 OFF - copilot");
    XPLMRegisterCommandHandler(copilot_radio2_off, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)RADIO_OFF);
    copilot_radio2_nav = XPLMCreateCommand("xhsi/nd_copilot/radio2_nav", "ND radio2 NAV2 - copilot");
    XPLMRegisterCommandHandler(copilot_radio2_nav, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)RADIO_NAV);
    copilot_radio2_down = XPLMCreateCommand("xhsi/nd_copilot/radio2_down", "Previous ND radio2 - copilot");
    XPLMRegisterCommandHandler(copilot_radio2_down, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)DOWN);
    copilot_radio2_up = XPLMCreateCommand("xhsi/nd_copilot/radio2_up", "Next ND radio2 - copilot");
    XPLMRegisterCommandHandler(copilot_radio2_up, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)UP);
//    copilot_radio2_cycle = XPLMCreateCommand("xhsi/nd_copilot/radio2_cycle", "Cycle ND radio2 - copilot");
//    XPLMRegisterCommandHandler(copilot_radio2_cycle, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)CYCLE);
    copilot_radio2_shuttle = XPLMCreateCommand("xhsi/nd_copilot/radio2_shuttle", "Shuttle ND radio2 - copilot");
    XPLMRegisterCommandHandler(copilot_radio2_shuttle, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)SHUTTLE);

    // copilot tfc
    copilot_tfc_toggle = XPLMCreateCommand("xhsi/nd_copilot/tfc_toggle", "Toggle ND symbols TFC - copilot");
    XPLMRegisterCommandHandler(copilot_tfc_toggle, (XPLMCommandCallback_f)copilot_tfc_handler, 1, (void *)TOGGLE);
    copilot_tfc_on = XPLMCreateCommand("xhsi/nd_copilot/tfc_on", "ND symbols TFC on - copilot");
    XPLMRegisterCommandHandler(copilot_tfc_on, (XPLMCommandCallback_f)copilot_tfc_handler, 1, (void *)ON);
    copilot_tfc_off = XPLMCreateCommand("xhsi/nd_copilot/tfc_off", "ND symbols TFC off - copilot");
    XPLMRegisterCommandHandler(copilot_tfc_off, (XPLMCommandCallback_f)copilot_tfc_handler, 1, (void *)OFF);

    // copilot arpt
    copilot_arpt_toggle = XPLMCreateCommand("xhsi/nd_copilot/arpt_toggle", "Toggle ND symbols ARPT - copilot");
    XPLMRegisterCommandHandler(copilot_arpt_toggle, (XPLMCommandCallback_f)copilot_arpt_handler, 1, (void *)TOGGLE);
    copilot_arpt_on = XPLMCreateCommand("xhsi/nd_copilot/arpt_on", "ND symbols ARPT on - copilot");
    XPLMRegisterCommandHandler(copilot_arpt_on, (XPLMCommandCallback_f)copilot_arpt_handler, 1, (void *)ON);
    copilot_arpt_off = XPLMCreateCommand("xhsi/nd_copilot/arpt_off", "ND symbols ARPT off - copilot");
    XPLMRegisterCommandHandler(copilot_arpt_off, (XPLMCommandCallback_f)copilot_arpt_handler, 1, (void *)OFF);

    // copilot wpt
    copilot_wpt_toggle = XPLMCreateCommand("xhsi/nd_copilot/wpt_toggle", "Toggle ND symbols WPT - copilot");
    XPLMRegisterCommandHandler(copilot_wpt_toggle, (XPLMCommandCallback_f)copilot_wpt_handler, 1, (void *)TOGGLE);
    copilot_wpt_on = XPLMCreateCommand("xhsi/nd_copilot/wpt_on", "ND symbols WPT on - copilot");
    XPLMRegisterCommandHandler(copilot_wpt_on, (XPLMCommandCallback_f)copilot_wpt_handler, 1, (void *)ON);
    copilot_wpt_off = XPLMCreateCommand("xhsi/nd_copilot/wpt_off", "ND symbols WPT off - copilot");
    XPLMRegisterCommandHandler(copilot_wpt_off, (XPLMCommandCallback_f)copilot_wpt_handler, 1, (void *)OFF);

    // copilot vor
    copilot_vor_toggle = XPLMCreateCommand("xhsi/nd_copilot/vor_toggle", "Toggle ND symbols VOR - copilot");
    XPLMRegisterCommandHandler(copilot_vor_toggle, (XPLMCommandCallback_f)copilot_vor_handler, 1, (void *)TOGGLE);
    copilot_vor_on = XPLMCreateCommand("xhsi/nd_copilot/vor_on", "ND symbols VOR on - copilot");
    XPLMRegisterCommandHandler(copilot_vor_on, (XPLMCommandCallback_f)copilot_vor_handler, 1, (void *)ON);
    copilot_vor_off = XPLMCreateCommand("xhsi/nd_copilot/vor_off", "ND symbols VOR off - copilot");
    XPLMRegisterCommandHandler(copilot_vor_off, (XPLMCommandCallback_f)copilot_vor_handler, 1, (void *)OFF);

    // copilot ndb
    copilot_ndb_toggle = XPLMCreateCommand("xhsi/nd_copilot/ndb_toggle", "Toggle ND symbols NDB - copilot");
    XPLMRegisterCommandHandler(copilot_ndb_toggle, (XPLMCommandCallback_f)copilot_ndb_handler, 1, (void *)TOGGLE);
    copilot_ndb_on = XPLMCreateCommand("xhsi/nd_copilot/ndb_on", "ND symbols NDB on - copilot");
    XPLMRegisterCommandHandler(copilot_ndb_on, (XPLMCommandCallback_f)copilot_ndb_handler, 1, (void *)ON);
    copilot_ndb_off = XPLMCreateCommand("xhsi/nd_copilot/ndb_off", "ND symbols NDB off - copilot");
    XPLMRegisterCommandHandler(copilot_ndb_off, (XPLMCommandCallback_f)copilot_ndb_handler, 1, (void *)OFF);

    // copilot sta = vor + ndb
    copilot_sta_toggle = XPLMCreateCommand("xhsi/nd_copilot/sta_toggle", "Toggle ND symbols STA - copilot");
    XPLMRegisterCommandHandler(copilot_sta_toggle, (XPLMCommandCallback_f)copilot_sta_handler, 1, (void *)TOGGLE);
    copilot_sta_on = XPLMCreateCommand("xhsi/nd_copilot/sta_on", "ND symbols STA on - copilot");
    XPLMRegisterCommandHandler(copilot_sta_on, (XPLMCommandCallback_f)copilot_sta_handler, 1, (void *)ON);
    copilot_sta_off = XPLMCreateCommand("xhsi/nd_copilot/sta_off", "ND symbols STA off - copilot");
    XPLMRegisterCommandHandler(copilot_sta_off, (XPLMCommandCallback_f)copilot_sta_handler, 1, (void *)OFF);
    copilot_sta_cycle = XPLMCreateCommand("xhsi/nd_copilot/sta_cycle", "Cycle ND symbols STA - copilot");
    XPLMRegisterCommandHandler(copilot_sta_cycle, (XPLMCommandCallback_f)copilot_sta_handler, 1, (void *)CYCLE);

    // copilot declutter : progressively hide map symbols
    copilot_declutter_cycle = XPLMCreateCommand("xhsi/nd_copilot/declutter", "ND declutter - copilot");
    XPLMRegisterCommandHandler(copilot_declutter_cycle, (XPLMCommandCallback_f)copilot_declutter_handler, 1, (void *)CYCLE);

    // copilot data = route data
    copilot_data_toggle = XPLMCreateCommand("xhsi/nd_copilot/data_toggle", "Toggle ND symbols DATA - copilot");
    XPLMRegisterCommandHandler(copilot_data_toggle, (XPLMCommandCallback_f)copilot_data_handler, 1, (void *)TOGGLE);
    copilot_data_on = XPLMCreateCommand("xhsi/nd_copilot/data_on", "ND symbols DATA on - copilot");
    XPLMRegisterCommandHandler(copilot_data_on, (XPLMCommandCallback_f)copilot_data_handler, 1, (void *)ON);
    copilot_data_off = XPLMCreateCommand("xhsi/nd_copilot/data_off", "ND symbols DATA off - copilot");
    XPLMRegisterCommandHandler(copilot_data_off, (XPLMCommandCallback_f)copilot_data_handler, 1, (void *)OFF);

    // copilot pos
    copilot_pos_toggle = XPLMCreateCommand("xhsi/nd_copilot/pos_toggle", "Toggle ND symbols POS - copilot");
    XPLMRegisterCommandHandler(copilot_pos_toggle, (XPLMCommandCallback_f)copilot_pos_handler, 1, (void *)TOGGLE);
    copilot_pos_on = XPLMCreateCommand("xhsi/nd_copilot/pos_on", "ND symbols POS on - copilot");
    XPLMRegisterCommandHandler(copilot_pos_on, (XPLMCommandCallback_f)copilot_pos_handler, 1, (void *)ON);
    copilot_pos_off = XPLMCreateCommand("xhsi/nd_copilot/pos_off", "ND symbols POS off - copilot");
    XPLMRegisterCommandHandler(copilot_pos_off, (XPLMCommandCallback_f)copilot_pos_handler, 1, (void *)OFF);

    // copilot terrain
    copilot_terrain_toggle = XPLMCreateCommand("xhsi/nd_copilot/terrain_toggle", "Toggle Terrain on ND - copilot");
    XPLMRegisterCommandHandler(copilot_terrain_toggle, (XPLMCommandCallback_f)copilot_terrain_handler, 1, (void *)TOGGLE);
    copilot_terrain_on = XPLMCreateCommand("xhsi/nd_copilot/terrain_on", "Terrain on ND on - copilot");
    XPLMRegisterCommandHandler(copilot_terrain_on, (XPLMCommandCallback_f)copilot_terrain_handler, 1, (void *)ON);
    copilot_terrain_off = XPLMCreateCommand("xhsi/nd_copilot/terrain_off", "Terrain on ND off - copilot");
    XPLMRegisterCommandHandler(copilot_terrain_off, (XPLMCommandCallback_f)copilot_terrain_handler, 1, (void *)OFF);

    // copilot Vertical Path on ND
    copilot_vp_toggle = XPLMCreateCommand("xhsi/nd_copilot/vertical_path_toggle", "Toggle Vertical Path on ND");
    XPLMRegisterCommandHandler(copilot_vp_toggle, (XPLMCommandCallback_f)copilot_vertical_path_handler, 1, (void *)TOGGLE);
    copilot_vp_on = XPLMCreateCommand("xhsi/nd_copilot/vertical_path_on", "Vertical Path on ND on");
    XPLMRegisterCommandHandler(copilot_vp_on, (XPLMCommandCallback_f)copilot_vertical_path_handler, 1, (void *)ON);
    copilot_vp_off = XPLMCreateCommand("xhsi/nd_copilot/vertical_path_off", "Vertical Path on ND off");
    XPLMRegisterCommandHandler(copilot_vp_off, (XPLMCommandCallback_f)copilot_vertical_path_handler, 1, (void *)OFF);

    // Weather Radar
    copilot_wxr_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_toggle", "Toggle weather radar on ND");
    XPLMRegisterCommandHandler(copilot_wxr_toggle, (XPLMCommandCallback_f)copilot_wxr_handler, 1, (void *)TOGGLE);
    copilot_wxr_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_on", "Weather radar on ND");
    XPLMRegisterCommandHandler(copilot_wxr_on, (XPLMCommandCallback_f)copilot_wxr_handler, 1, (void *)ON);
    copilot_wxr_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_off", "Weather radar off ND");
    XPLMRegisterCommandHandler(copilot_wxr_off, (XPLMCommandCallback_f)copilot_wxr_handler, 1, (void *)OFF);

    copilot_wxr_gain_up = XPLMCreateCommand("xhsi/nd_copilot/wxr_gain_up", "Weather radar gain up +0.05");
    XPLMRegisterCommandHandler(copilot_wxr_gain_up, (XPLMCommandCallback_f)copilot_wxr_gain_handler, 1, (void *)UP);
    copilot_wxr_gain_down = XPLMCreateCommand("xhsi/nd_copilot/wxr_gain_down", "Weather radar gain down -0.05");
    XPLMRegisterCommandHandler(copilot_wxr_gain_down, (XPLMCommandCallback_f)copilot_wxr_gain_handler, 1, (void *)DOWN);
    copilot_wxr_gain_middle = XPLMCreateCommand("xhsi/nd_copilot/wxr_gain_middle", "Set weather radar gain to 0.50");
    XPLMRegisterCommandHandler(copilot_wxr_gain_middle, (XPLMCommandCallback_f)copilot_wxr_gain_handler, 1, (void *)MODE_CENTERED);

    copilot_wxr_tilt_up = XPLMCreateCommand("xhsi/nd_copilot/wxr_tilt_up", "Weather radar tilt up 1 degree");
    XPLMRegisterCommandHandler(copilot_wxr_tilt_up, (XPLMCommandCallback_f)copilot_wxr_tilt_handler, 1, (void *)UP);
    copilot_wxr_tilt_down = XPLMCreateCommand("xhsi/nd_copilot/wxr_tilt_down", "Weather radar tilt down 1 degree");
    XPLMRegisterCommandHandler(copilot_wxr_tilt_down, (XPLMCommandCallback_f)copilot_wxr_tilt_handler, 1, (void *)DOWN);
    copilot_wxr_tilt_zero = XPLMCreateCommand("xhsi/nd_copilot/wxr_tilt_zero", "Set weather radar tilt to 0");
    XPLMRegisterCommandHandler(copilot_wxr_tilt_zero, (XPLMCommandCallback_f)copilot_wxr_tilt_handler, 1, (void *)MODE_CENTERED);

    copilot_wxr_auto_gain_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_auto_gain_toggle", "Toggle weather radar auto gain");
    XPLMRegisterCommandHandler(copilot_wxr_auto_gain_toggle, (XPLMCommandCallback_f)copilot_wxr_auto_gain_handler, 1, (void *)TOGGLE);
    copilot_wxr_auto_gain_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_auto_gain_on", "Weather radar auto gain on");
    XPLMRegisterCommandHandler(copilot_wxr_auto_gain_on, (XPLMCommandCallback_f)copilot_wxr_auto_gain_handler, 1, (void *)ON);
    copilot_wxr_auto_gain_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_auto_gain_off", "Weather radar auto gain off");
    XPLMRegisterCommandHandler(copilot_wxr_auto_gain_off, (XPLMCommandCallback_f)copilot_wxr_auto_gain_handler, 1, (void *)OFF);

    copilot_wxr_auto_tilt_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_auto_tilt_toggle", "Toggle weather radar auto tilt");
    XPLMRegisterCommandHandler(copilot_wxr_auto_tilt_toggle, (XPLMCommandCallback_f)copilot_wxr_auto_tilt_handler, 1, (void *)TOGGLE);
    copilot_wxr_auto_tilt_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_auto_tilt_on", "Weather radar auto tilt on");
    XPLMRegisterCommandHandler(copilot_wxr_auto_tilt_on, (XPLMCommandCallback_f)copilot_wxr_auto_tilt_handler, 1, (void *)ON);
    copilot_wxr_auto_tilt_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_auto_tilt_off", "Weather radar auto tilt off");
    XPLMRegisterCommandHandler(copilot_wxr_auto_tilt_off, (XPLMCommandCallback_f)copilot_wxr_auto_tilt_handler, 1, (void *)OFF);

    copilot_wxr_test_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_test_toggle", "Toggle weather radar test");
    XPLMRegisterCommandHandler(copilot_wxr_test_toggle, (XPLMCommandCallback_f)copilot_wxr_test_handler, 1, (void *)TOGGLE);
    copilot_wxr_test_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_test_on", "Weather radar auto tilt on");
    XPLMRegisterCommandHandler(copilot_wxr_test_on, (XPLMCommandCallback_f)copilot_wxr_test_handler, 1, (void *)ON);
    copilot_wxr_test_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_test_off", "Weather radar auto tilt off");
    XPLMRegisterCommandHandler(copilot_wxr_test_off, (XPLMCommandCallback_f)copilot_wxr_test_handler, 1, (void *)OFF);

    copilot_wxr_mode_up = XPLMCreateCommand("xhsi/nd_copilot/wxr_mode_up", "Weather radar mode up");
    XPLMRegisterCommandHandler(copilot_wxr_mode_up, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)UP);
    copilot_wxr_mode_down = XPLMCreateCommand("xhsi/nd_copilot/wxr_mode_down", "Weather radar mode down");
    XPLMRegisterCommandHandler(copilot_wxr_mode_down, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)DOWN);
    copilot_wxr_mode_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_mode_off", "Weather radar mode OFF");
    XPLMRegisterCommandHandler(copilot_wxr_mode_off, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_OFF);
    copilot_wxr_mode_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_mode_on", "Weather radar mode ON");
    XPLMRegisterCommandHandler(copilot_wxr_mode_on, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_ON);
    copilot_wxr_mode_turb = XPLMCreateCommand("xhsi/nd_copilot/wxr_mode_turb", "Weather radar mode TURB");
    XPLMRegisterCommandHandler(copilot_wxr_mode_turb, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_TURB);
    copilot_wxr_mode_map = XPLMCreateCommand("xhsi/nd_copilot/wxr_mode_map", "Weather radar mode MAP");
    XPLMRegisterCommandHandler(copilot_wxr_mode_map, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_MAP);
    copilot_wxr_mode_force_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_mode_force_on", "Weather radar mode FORCE ON");
    XPLMRegisterCommandHandler(copilot_wxr_mode_force_on, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_FORCE_ON);

    copilot_wxr_slave_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_slave_toggle", "Toggle weather salve mode");
    XPLMRegisterCommandHandler(copilot_wxr_slave_toggle, (XPLMCommandCallback_f)copilot_wxr_slave_handler, 1, (void *)TOGGLE);
    copilot_wxr_slave_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_slave_on", "Weather radar slave on [F/O side]");
    XPLMRegisterCommandHandler(copilot_wxr_slave_on, (XPLMCommandCallback_f)copilot_wxr_slave_handler, 1, (void *)ON);
    copilot_wxr_slave_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_slave_off", "Weather radar slave off");
    XPLMRegisterCommandHandler(copilot_wxr_slave_off, (XPLMCommandCallback_f)copilot_wxr_slave_handler, 1, (void *)OFF);

    copilot_wxr_react_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_react_toggle", "Toggle weather REACT mode");
    XPLMRegisterCommandHandler(copilot_wxr_react_toggle, (XPLMCommandCallback_f)copilot_wxr_react_handler, 1, (void *)TOGGLE);
    copilot_wxr_react_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_react_on", "Weather radar REACT on");
    XPLMRegisterCommandHandler(copilot_wxr_react_on, (XPLMCommandCallback_f)copilot_wxr_react_handler, 1, (void *)ON);
    copilot_wxr_react_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_react_off", "Weather radar REACT off");
    XPLMRegisterCommandHandler(copilot_wxr_react_off, (XPLMCommandCallback_f)copilot_wxr_react_handler, 1, (void *)OFF);

    copilot_wxr_narrow_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_narrow_toggle", "Toggle weather narrow mode");
    XPLMRegisterCommandHandler(copilot_wxr_narrow_toggle, (XPLMCommandCallback_f)copilot_wxr_narrow_handler, 1, (void *)TOGGLE);
    copilot_wxr_narrow_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_narrow_on", "Weather radar narrow mode on");
    XPLMRegisterCommandHandler(copilot_wxr_narrow_on, (XPLMCommandCallback_f)copilot_wxr_narrow_handler, 1, (void *)ON);
    copilot_wxr_narrow_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_narrow_off", "Weather radar narrow mode off");
    XPLMRegisterCommandHandler(copilot_wxr_narrow_off, (XPLMCommandCallback_f)copilot_wxr_narrow_handler, 1, (void *)OFF);

    copilot_wxr_alert_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_alert_toggle", "Toggle weather alert mode");
    XPLMRegisterCommandHandler(copilot_wxr_alert_toggle, (XPLMCommandCallback_f)copilot_wxr_alert_handler, 1, (void *)TOGGLE);
    copilot_wxr_alert_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_alert_on", "Weather radar narrow alert mode on");
    XPLMRegisterCommandHandler(copilot_wxr_alert_on, (XPLMCommandCallback_f)copilot_wxr_alert_handler, 1, (void *)ON);
    copilot_wxr_alert_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_alert_off", "Weather radar alert mode off");
    XPLMRegisterCommandHandler(copilot_wxr_alert_off, (XPLMCommandCallback_f)copilot_wxr_alert_handler, 1, (void *)OFF);

    copilot_wxr_target_toggle = XPLMCreateCommand("xhsi/nd_copilot/wxr_target_toggle", "Toggle weather target mode");
    XPLMRegisterCommandHandler(copilot_wxr_target_toggle, (XPLMCommandCallback_f)copilot_wxr_target_handler, 1, (void *)TOGGLE);
    copilot_wxr_target_on = XPLMCreateCommand("xhsi/nd_copilot/wxr_target_on", "Weather radar narrow target mode on");
    XPLMRegisterCommandHandler(copilot_wxr_target_on, (XPLMCommandCallback_f)copilot_wxr_target_handler, 1, (void *)ON);
    copilot_wxr_target_off = XPLMCreateCommand("xhsi/nd_copilot/wxr_target_off", "Weather radar target mode off");
    XPLMRegisterCommandHandler(copilot_wxr_target_off, (XPLMCommandCallback_f)copilot_wxr_target_handler, 1, (void *)OFF);


    // xhsi/nd_ext_range_copilot/...

    // copilot zoomin
    copilot_zoom_x100_toggle = XPLMCreateCommand("xhsi/nd_ext_range_copilot/zoom_x100_toggle", "Toggle ND map zoom x100 - copilot");
    XPLMRegisterCommandHandler(copilot_zoom_x100_toggle, (XPLMCommandCallback_f)copilot_zoom_x100_handler, 1, (void *)TOGGLE);
    copilot_zoom_x100_on = XPLMCreateCommand("xhsi/nd_ext_range_copilot/zoom_x100_on", "ND map range zoom in x100 - copilot");
    XPLMRegisterCommandHandler(copilot_zoom_x100_on, (XPLMCommandCallback_f)copilot_zoom_x100_handler, 1, (void *)ON);
    copilot_zoom_x100_off = XPLMCreateCommand("xhsi/nd_ext_range_copilot/zoom_x100_off", "ND map range normal - copilot");
    XPLMRegisterCommandHandler(copilot_zoom_x100_off, (XPLMCommandCallback_f)copilot_zoom_x100_handler, 1, (void *)OFF);

    // copilot ext range
    copilot_ext_range_0_10 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_0.10", "ND extended map range 0.10 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_0_10, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_10 + 1100));
    copilot_ext_range_0_20 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_0.20", "ND extended map range 0.20 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_0_20, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_20 + 1100));
    copilot_ext_range_0_40 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_0.40", "ND extended map range 0.40 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_0_40, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_40 + 1100));
    copilot_ext_range_0_80 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_0.80", "ND extended map range 0.80 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_0_80, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_80 + 1100));
    copilot_ext_range_1_60 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_1.60", "ND extended map range 1.60 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_1_60, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_160 + 1100));
    copilot_ext_range_3_20 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_3.20", "ND extended map range 3.20 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_3_20, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_320 + 1100));
    copilot_ext_range_6_40 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_6.40", "ND extended map range 6.40 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_6_40, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_640 + 1100));
    copilot_ext_range_010 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_010", "ND extended map range 10 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_010, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_10 + 1000));
    copilot_ext_range_020 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_020", "ND extended map range 20 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_020, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_20 + 1000));
    copilot_ext_range_040 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_040", "ND extended map range 40 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_040, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_40 + 1000));
    copilot_ext_range_080 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_080", "ND extended map range 80 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_080, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_80 + 1000));
    copilot_ext_range_160 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_160", "ND extended map range 160 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_160, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_160 + 1000));
    copilot_ext_range_320 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_320", "ND extended map range 320 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_320, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_320 + 1000));
    copilot_ext_range_640 = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_640", "ND extended map range 640 - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_640, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_640 + 1000));
    copilot_ext_range_down = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_down", "Decrease ND extended map range - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_down, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)DOWN);
    copilot_ext_range_up = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_up", "Increase ND extended map range - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_up, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)UP);
    copilot_ext_range_cycle = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_cycle", "Cycle through ND extended map ranges - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_cycle, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)CYCLE);
    copilot_ext_range_shuttle = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_shuttle", "Shuttle back and forth through ND extended map ranges - copilot");
    XPLMRegisterCommandHandler(copilot_ext_range_shuttle, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)SHUTTLE);


    // xhsi/nd_b737classic_copilot/...

    // copilot B737-Classic modes
    b737cl_copilot_mode_fullvorils = XPLMCreateCommand("xhsi/nd_b737classic_copilot/mode_fullvorils", "B737-Classic mode FULL VOR/ILS - copilot");
    XPLMRegisterCommandHandler(b737cl_copilot_mode_fullvorils, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_FULL_VOR_ILS);
    b737cl_copilot_mode_expvorils = XPLMCreateCommand("xhsi/nd_b737classic_copilot/mode_expvorils", "B737-Classic mode EXP VOR/ILS - copilot");
    XPLMRegisterCommandHandler(b737cl_copilot_mode_expvorils, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_EXP_VOR_ILS);
    b737cl_copilot_mode_map = XPLMCreateCommand("xhsi/nd_b737classic_copilot/mode_map", "B737-Classic mode MAP - copilot");
    XPLMRegisterCommandHandler(b737cl_copilot_mode_map, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_MAP);
    b737cl_copilot_mode_ctrmap = XPLMCreateCommand("xhsi/nd_b737classic_copilot/mode_ctrmap", "B737-Classic mode CTR MAP - copilot");
    XPLMRegisterCommandHandler(b737cl_copilot_mode_ctrmap, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_CTR_MAP);
    b737cl_copilot_mode_plan = XPLMCreateCommand("xhsi/nd_b737classic_copilot/mode_plan", "B737-Classic mode PLAN - copilot");
    XPLMRegisterCommandHandler(b737cl_copilot_mode_plan, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_PLAN);


    // xhsi/pfd_copilot/...

    // copilot mins mode
    copilot_mins_toggle = XPLMCreateCommand("xhsi/pfd_copilot/mins_toggle", "Toggle MINS selector - copilot");
    XPLMRegisterCommandHandler(copilot_mins_toggle, (XPLMCommandCallback_f)copilot_mins_mode_handler, 1, (void *)TOGGLE);
    copilot_mins_radio = XPLMCreateCommand("xhsi/pfd_copilot/mins_radio", "Select RADIO MINS - copilot");
    XPLMRegisterCommandHandler(copilot_mins_radio, (XPLMCommandCallback_f)copilot_mins_mode_handler, 1, (void *)MINS_RADIO);
    copilot_mins_baro = XPLMCreateCommand("xhsi/pfd_copilot/mins_baro", "Select BARO MINS - copilot");
    XPLMRegisterCommandHandler(copilot_mins_baro, (XPLMCommandCallback_f)copilot_mins_mode_handler, 1, (void *)MINS_BARO);

    // copilot mins value
    copilot_mins_reset = XPLMCreateCommand("xhsi/pfd_copilot/mins_reset", "Reset MINS value - copilot");
    XPLMRegisterCommandHandler(copilot_mins_reset, (XPLMCommandCallback_f)copilot_mins_value_handler, 1, (void *)OFF);
    copilot_mins_down = XPLMCreateCommand("xhsi/pfd_copilot/mins_down", "Decrease MINS value - copilot");
    XPLMRegisterCommandHandler(copilot_mins_down, (XPLMCommandCallback_f)copilot_mins_value_handler, 1, (void *)DOWN);
    copilot_mins_up = XPLMCreateCommand("xhsi/pfd_copilot/mins_up", "Increase MINS value - copilot");
    XPLMRegisterCommandHandler(copilot_mins_up, (XPLMCommandCallback_f)copilot_mins_value_handler, 1, (void *)UP);

    // copilot Instrument Landing System (PFD & ND)
    copilot_ils_toggle = XPLMCreateCommand("xhsi/pfd_copilot/ils_toggle", "Toggle ILS on PFD/ND");
    XPLMRegisterCommandHandler(copilot_ils_toggle, (XPLMCommandCallback_f)copilot_ils_handler, 1, (void *)TOGGLE);
    copilot_ils_on = XPLMCreateCommand("xhsi/pfd_copilot/ils_on", "ILS on PFD/ND on");
    XPLMRegisterCommandHandler(copilot_ils_on, (XPLMCommandCallback_f)copilot_ils_handler, 1, (void *)ON);
    copilot_ils_off = XPLMCreateCommand("xhsi/pfd_copilot/ils_off", "ILS on PFD/ND off");
    XPLMRegisterCommandHandler(copilot_ils_off, (XPLMCommandCallback_f)copilot_ils_handler, 1, (void *)OFF);

    // copilot Metric altitude on PFD/ND
    copilot_metric_alt_toggle = XPLMCreateCommand("xhsi/pfd_copilot/metric_alt_toggle", "Toggle metric altitude on PFD/ND");
    XPLMRegisterCommandHandler(copilot_metric_alt_toggle, (XPLMCommandCallback_f)copilot_metric_alt_handler, 1, (void *)TOGGLE);
    copilot_metric_alt_on = XPLMCreateCommand("xhsi/pfd_copilot/metric_alt_on", "metric altitude on PFD/ND on");
    XPLMRegisterCommandHandler(copilot_metric_alt_on, (XPLMCommandCallback_f)copilot_metric_alt_handler, 1, (void *)ON);
    copilot_metric_alt_off = XPLMCreateCommand("xhsi/pfd_copilot/metric_alt_off", "metric altitude on PFD/ND off");
    XPLMRegisterCommandHandler(copilot_metric_alt_off, (XPLMCommandCallback_f)copilot_metric_alt_handler, 1, (void *)OFF);

    // copilot Track FPA / FPV / Bird : on display FPA/FPV/Bird , off display flight director
    copilot_track_fpa_toggle = XPLMCreateCommand("xhsi/pfd_copilot/track_fpa_toggle", "Toggle Track FPA on PFD");
    XPLMRegisterCommandHandler(track_fpa_toggle, (XPLMCommandCallback_f)copilot_track_fpa_handler, 1, (void *)TOGGLE);
    copilot_track_fpa_on = XPLMCreateCommand("xhsi/pfd_copilot/track_fpa_on", "Track FPA on PFD on");
    XPLMRegisterCommandHandler(track_fpa_on, (XPLMCommandCallback_f)copilot_track_fpa_handler, 1, (void *)ON);
    copilot_track_fpa_off = XPLMCreateCommand("xhsi/pfd_copilot/track_fpa_off", "Track FPA on PFD off");
    XPLMRegisterCommandHandler(track_fpa_off, (XPLMCommandCallback_f)copilot_track_fpa_handler, 1, (void *)OFF);

    // xhsi/mfd/...

    // MFD mode
    mfd_mode_arpt = XPLMCreateCommand("xhsi/mfd/mode_arpt", "MFD mode Airport Chart");
    XPLMRegisterCommandHandler(mfd_mode_arpt, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_ARPT);
    mfd_mode_fpln = XPLMCreateCommand("xhsi/mfd/mode_fpln", "MFD mode Flight Plan");
    XPLMRegisterCommandHandler(mfd_mode_fpln, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_FPLN);
    mfd_mode_eicas = XPLMCreateCommand("xhsi/mfd/mode_rtu", "MFD mode Radio Tuning Unit");
    XPLMRegisterCommandHandler(mfd_mode_eicas, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_RTU);
    mfd_mode_eicas = XPLMCreateCommand("xhsi/mfd/mode_eicas", "MFD mode Lower EICAS");
    XPLMRegisterCommandHandler(mfd_mode_eicas, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_EICAS);

    mfd_mode_bleed = XPLMCreateCommand("xhsi/mfd/mode_bleed", "MFD mode BLEED");
    XPLMRegisterCommandHandler(mfd_mode_bleed, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_BLEED);

    mfd_mode_cab_press = XPLMCreateCommand("xhsi/mfd/mode_cab_press", "MFD mode CAB_PRESS");
    XPLMRegisterCommandHandler(mfd_mode_cab_press, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_CAB_PRESS);

    mfd_mode_elec = XPLMCreateCommand("xhsi/mfd/mode_elec", "MFD mode ELEC");
    XPLMRegisterCommandHandler(mfd_mode_elec, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_ELEC);

    mfd_mode_hydr = XPLMCreateCommand("xhsi/mfd/mode_hydr", "MFD mode HYDR");
    XPLMRegisterCommandHandler(mfd_mode_hydr, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_HYDR);

    mfd_mode_fuel = XPLMCreateCommand("xhsi/mfd/mode_fuel", "MFD mode FUEL");
    XPLMRegisterCommandHandler(mfd_mode_fuel, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_FUEL);

    mfd_mode_apu = XPLMCreateCommand("xhsi/mfd/mode_apu", "MFD mode APU");
    XPLMRegisterCommandHandler(mfd_mode_apu, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_APU);

    mfd_mode_cond = XPLMCreateCommand("xhsi/mfd/mode_cond", "MFD mode COND");
    XPLMRegisterCommandHandler(mfd_mode_cond, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_COND);

    mfd_mode_door_oxy = XPLMCreateCommand("xhsi/mfd/mode_door_oxy", "MFD mode DOOR/OXY");
    XPLMRegisterCommandHandler(mfd_mode_door_oxy, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_DOOR_OXY);

    mfd_mode_wheel = XPLMCreateCommand("xhsi/mfd/mode_wheel", "MFD mode WHEEL");
    XPLMRegisterCommandHandler(mfd_mode_wheel, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_WHEELS);

    mfd_mode_fctl = XPLMCreateCommand("xhsi/mfd/mode_fctl", "MFD mode F/CTL");
    XPLMRegisterCommandHandler(mfd_mode_fctl, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_FCTL);

    mfd_mode_sys = XPLMCreateCommand("xhsi/mfd/mode_sys", "MFD mode CRUISE");
    XPLMRegisterCommandHandler(mfd_mode_sys, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_SYS);

    mfd_mode_status = XPLMCreateCommand("xhsi/mfd/mode_status", "MFD mode STATUS");
    XPLMRegisterCommandHandler(mfd_mode_status, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_STATUS);

    mfd_mode_debug = XPLMCreateCommand("xhsi/mfd/mode_debug", "MFD mode DEBUG");
    XPLMRegisterCommandHandler(mfd_mode_debug, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_DEBUG);

    mfd_mode_down = XPLMCreateCommand("xhsi/mfd/mode_down", "Previous MFD mode");
    XPLMRegisterCommandHandler(mfd_mode_down, (XPLMCommandCallback_f)mfd_handler, 1, (void *)DOWN);

    mfd_mode_up = XPLMCreateCommand("xhsi/mfd/mode_up", "Next MFD mode");
    XPLMRegisterCommandHandler(mfd_mode_up, (XPLMCommandCallback_f)mfd_handler, 1, (void *)UP);

    mfd_mode_cycle = XPLMCreateCommand("xhsi/mfd/mode_cycle", "Cycle MFD modes");
    XPLMRegisterCommandHandler(mfd_mode_cycle, (XPLMCommandCallback_f)mfd_handler, 1, (void *)CYCLE);

    mfd_reset_fuel_used = XPLMCreateCommand("xhsi/mfd/reset_fuel_used", "Reset Fuel Used");
    XPLMRegisterCommandHandler(mfd_reset_fuel_used, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_RESET_FUEL_USED);


    // CDU
    cdu_pilot_custom = XPLMCreateCommand("xhsi/cdu_pilot/custom", "Aircraft custom CDU");
    XPLMRegisterCommandHandler(cdu_pilot_custom, (XPLMCommandCallback_f)cdu_handler, 1, (void *)CDU_PILOT_CUSTOM);
    cdu_pilot_xfmc = XPLMCreateCommand("xhsi/cdu_pilot/xfmc", "X-FMC CDU");
    XPLMRegisterCommandHandler(cdu_pilot_xfmc, (XPLMCommandCallback_f)cdu_handler, 1, (void *)CDU_PILOT_XFMC);
    cdu_copilot_custom = XPLMCreateCommand("xhsi/cdu_copilot/custom", "Aircraft custom CDU");
    XPLMRegisterCommandHandler(cdu_copilot_custom, (XPLMCommandCallback_f)cdu_handler, 1, (void *)CDU_COPILOT_CUSTOM);
    cdu_copilot_xfmc = XPLMCreateCommand("xhsi/cdu_copilot/xfmc", "X-FMC CDU");
    XPLMRegisterCommandHandler(cdu_copilot_xfmc, (XPLMCommandCallback_f)cdu_handler, 1, (void *)CDU_COPILOT_XFMC);

    // xhsi/radios/...

    // nav1_sync
    nav1_sync = XPLMCreateCommand("xhsi/radios/nav1_sync", "Sync NAV1 to LOC1/ILS1 or Direct-To VOR1");
    XPLMRegisterCommandHandler(nav1_sync, (XPLMCommandCallback_f)nav_sync_handler, 1, (void *)1);

    // nav2_sync
    nav2_sync = XPLMCreateCommand("xhsi/radios/nav2_sync", "Sync NAV2 to LOC2/ILS2 or Direct-To VOR2");
    XPLMRegisterCommandHandler(nav2_sync, (XPLMCommandCallback_f)nav_sync_handler, 1, (void *)2);


    // xhsi/nd_.../range_auto

    // xhsi/nd_pilot/range_auto
    auto_range_pilot = XPLMCreateCommand("xhsi/nd_pilot/range_auto", "Auto ND map range");
    XPLMRegisterCommandHandler(auto_range_pilot, (XPLMCommandCallback_f)auto_range_handler, 1, (void *)EFIS_PILOT);
    // xhsi/nd_copilot/range_auto
    auto_range_copilot = XPLMCreateCommand("xhsi/nd_copilot/range_auto", "Auto ND map range - copilot");
    XPLMRegisterCommandHandler(auto_range_copilot, (XPLMCommandCallback_f)auto_range_handler, 1, (void *)EFIS_COPILOT);


    // xhsi/nd_ext_range_.../ext_range_auto

    // xhsi/nd_ext_range_pilot/ext_range_auto
    auto_ext_range_pilot = XPLMCreateCommand("xhsi/nd_ext_range_pilot/ext_range_auto", "Auto ND extended map range");
    XPLMRegisterCommandHandler(auto_ext_range_pilot, (XPLMCommandCallback_f)auto_ext_range_handler, 1, (void *)EFIS_PILOT);
    // xhsi/nd_ext_range_copilot/ext_range_auto
    auto_ext_range_copilot = XPLMCreateCommand("xhsi/nd_ext_range_copilot/ext_range_auto", "Auto ND extended map range - copilot");
    XPLMRegisterCommandHandler(auto_ext_range_copilot, (XPLMCommandCallback_f)auto_ext_range_handler, 1, (void *)EFIS_COPILOT);


    // xhsi/clock/...

    // clock
    chr_start_stop_reset = XPLMCreateCommand("xhsi/clock/chr_start_stop_reset", "Chronograph start/stop/reset");
    XPLMRegisterCommandHandler(chr_start_stop_reset, (XPLMCommandCallback_f)clock_handler, 1, (void *)0);
    chr_start_stop = XPLMCreateCommand("xhsi/clock/chr_start_stop", "Chronograph start/stop");
    XPLMRegisterCommandHandler(chr_start_stop, (XPLMCommandCallback_f)clock_handler, 1, (void *)1);
    chr_reset = XPLMCreateCommand("xhsi/clock/chr_reset", "Chronograph reset");
    XPLMRegisterCommandHandler(chr_reset, (XPLMCommandCallback_f)clock_handler, 1, (void *)2);

    // Elapsed Flight Time
    et_reset = XPLMCreateCommand("xhsi/clock/et_reset", "Elapsed flight time reset");
    XPLMRegisterCommandHandler(et_reset, (XPLMCommandCallback_f)clock_handler, 1, (void *)CLK_ET_RESET);
    et_stop = XPLMCreateCommand("xhsi/clock/et_stop", "Elapsed flight time stop");
    XPLMRegisterCommandHandler(et_stop, (XPLMCommandCallback_f)clock_handler, 1, (void *)CLK_ET_STOP);
    et_run = XPLMCreateCommand("xhsi/clock/et_run", "Elapsed flight time run");
    XPLMRegisterCommandHandler(et_run, (XPLMCommandCallback_f)clock_handler, 1, (void *)CLK_ET_RUN);

    // Clock Source
    clock_utc_gps = XPLMCreateCommand("xhsi/clock/utc_gps", "Set clock reference to GPS");
    XPLMRegisterCommandHandler(clock_utc_gps, (XPLMCommandCallback_f)clock_handler, 1, (void *)CLK_UTC_GPS);
    clock_utc_int = XPLMCreateCommand("xhsi/clock/utc_int", "Set clock reference to Internal");
    XPLMRegisterCommandHandler(clock_utc_int, (XPLMCommandCallback_f)clock_handler, 1, (void *)CLK_UTC_INT);
    clock_utc_set = XPLMCreateCommand("xhsi/clock/utc_set", "Set internal clock mode");
    XPLMRegisterCommandHandler(clock_utc_set, (XPLMCommandCallback_f)clock_handler, 1, (void *)CLK_UTC_SET);
    
    // special case: use these existing commands to control the chronometer
    timer_start_stop = XPLMFindCommand("sim/instruments/timer_start_stop");
    timer_reset = XPLMFindCommand("sim/instruments/timer_reset");

    // Chrono
    pilot_chrono_stop_reset = XPLMCreateCommand("xhsi/nd_pilot/chr_start_stop_reset", "ND Pilot Chronograph start/stop/reset");
    XPLMRegisterCommandHandler(pilot_chrono_stop_reset, (XPLMCommandCallback_f)chrono_handler, 1, (void *)0);
    pilot_chrono_start_stop = XPLMCreateCommand("xhsi/nd_pilot/chr_start_stop", "ND Pilot Chronograph start/stop");
    XPLMRegisterCommandHandler(pilot_chrono_start_stop, (XPLMCommandCallback_f)chrono_handler, 1, (void *)1);
    pilot_chrono_reset = XPLMCreateCommand("xhsi/nd_pilot/chr_reset", "ND Pilot Chronograph reset");
    XPLMRegisterCommandHandler(pilot_chrono_reset, (XPLMCommandCallback_f)chrono_handler, 1, (void *)2);
    copilot_chrono_stop_reset = XPLMCreateCommand("xhsi/nd_copilot/chr_start_stop_reset", "ND Copilot Chronograph start/stop/reset");
    XPLMRegisterCommandHandler(copilot_chrono_stop_reset, (XPLMCommandCallback_f)chrono_handler, 1, (void *)3);
    copilot_chrono_start_stop = XPLMCreateCommand("xhsi/nd_copilot/chr_start_stop", "ND Copilot Chronograph start/stop");
    XPLMRegisterCommandHandler(copilot_chrono_start_stop, (XPLMCommandCallback_f)chrono_handler, 1, (void *)4);
    copilot_chrono_reset = XPLMCreateCommand("xhsi/nd_copilot/chr_reset", "ND Copilot Chronograph reset");
    XPLMRegisterCommandHandler(copilot_chrono_reset, (XPLMCommandCallback_f)chrono_handler, 1, (void *)5);


    // special case: use these existing commands to flip active/standby radios
    // (typos are Laminar Research's, not ours!)
    nav1_standy_flip = XPLMFindCommand("sim/radios/nav1_standy_flip");
    nav2_standy_flip = XPLMFindCommand("sim/radios/nav2_standy_flip");
    com1_standy_flip = XPLMFindCommand("sim/radios/com1_standy_flip");
    com2_standy_flip = XPLMFindCommand("sim/radios/com2_standy_flip");
    adf1_standy_flip = XPLMFindCommand("sim/radios/adf1_standy_flip");
    adf2_standy_flip = XPLMFindCommand("sim/radios/adf2_standy_flip");
    sim_transponder_transponder_ident = XPLMFindCommand("sim/transponder/transponder_ident");
    

    // special case: use these existing commands for the RTU
    stby_com1_coarse_down = XPLMFindCommand("sim/radios/stby_com1_coarse_down");
    stby_com1_coarse_up = XPLMFindCommand("sim/radios/stby_com1_coarse_up");
    stby_com1_fine_down_833 = XPLMFindCommand("sim/radios/stby_com1_fine_down_833");
    stby_com1_fine_up_833 = XPLMFindCommand("sim/radios/stby_com1_fine_up_833");
    stby_com2_coarse_down = XPLMFindCommand("sim/radios/stby_com2_coarse_down");
    stby_com2_coarse_up = XPLMFindCommand("sim/radios/stby_com2_coarse_up");
    stby_com2_fine_down_833 = XPLMFindCommand("sim/radios/stby_com2_fine_down_833");
    stby_com2_fine_up_833 = XPLMFindCommand("sim/radios/stby_com2_fine_up_833");
    stby_nav1_coarse_down = XPLMFindCommand("sim/radios/stby_nav1_coarse_down");
    stby_nav1_coarse_up = XPLMFindCommand("sim/radios/stby_nav1_coarse_up");
    stby_nav1_fine_down = XPLMFindCommand("sim/radios/stby_nav1_fine_down");
    stby_nav1_fine_up = XPLMFindCommand("sim/radios/stby_nav1_fine_up");
    stby_nav2_coarse_down = XPLMFindCommand("sim/radios/stby_nav2_coarse_down");
    stby_nav2_coarse_up = XPLMFindCommand("sim/radios/stby_nav2_coarse_up");
    stby_nav2_fine_down = XPLMFindCommand("sim/radios/stby_nav2_fine_down");
    stby_nav2_fine_up = XPLMFindCommand("sim/radios/stby_nav2_fine_up");
    
    
//    // special case: intercept a standard X-Plane command
//    contact_atc_cmd = XPLMCreateCommand("sim/operation/contact_atc", "Contact ATC");
//    XPLMRegisterCommandHandler(contact_atc_cmd, (XPLMCommandCallback_f)contact_atc_handler, 1, (void *)0);


    // RTU select
    rtu_select_none = XPLMCreateCommand("xhsi/rtu/select_none", "RTU select none");
    XPLMRegisterCommandHandler(rtu_select_none, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_NONE);
    rtu_select_com1 = XPLMCreateCommand("xhsi/rtu/select_com1", "RTU select COM1");
    XPLMRegisterCommandHandler(rtu_select_com1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_COM1);
    rtu_select_nav1 = XPLMCreateCommand("xhsi/rtu/select_nav1", "RTU select NAV1");
    XPLMRegisterCommandHandler(rtu_select_nav1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_NAV1);
    rtu_select_adf1 = XPLMCreateCommand("xhsi/rtu/select_adf1", "RTU select ADF1");
    XPLMRegisterCommandHandler(rtu_select_adf1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_ADF1);
    rtu_select_xpdr = XPLMCreateCommand("xhsi/rtu/select_xpdr", "RTU select XPDR");
    XPLMRegisterCommandHandler(rtu_select_xpdr, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_XPDR);
    rtu_select_tcas = XPLMCreateCommand("xhsi/rtu/select_tcas", "RTU select TCAS");
    XPLMRegisterCommandHandler(rtu_select_tcas, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_TCAS);
    rtu_select_adf2 = XPLMCreateCommand("xhsi/rtu/select_adf2", "RTU select ADF2");
    XPLMRegisterCommandHandler(rtu_select_adf2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_ADF2);
    rtu_select_nav2 = XPLMCreateCommand("xhsi/rtu/select_nav2", "RTU select NAV2");
    XPLMRegisterCommandHandler(rtu_select_nav2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_NAV2);
    rtu_select_com2 = XPLMCreateCommand("xhsi/rtu/select_com2", "RTU select COM2");
    XPLMRegisterCommandHandler(rtu_select_com2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_COM2);
    // RTU select/flip
    rtu_select_flip_com1 = XPLMCreateCommand("xhsi/rtu/select_flip_com1", "RTU select/flip COM1");
    XPLMRegisterCommandHandler(rtu_select_flip_com1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_COM1);
    rtu_select_flip_nav1 = XPLMCreateCommand("xhsi/rtu/select_flip_nav1", "RTU select/flip NAV1");
    XPLMRegisterCommandHandler(rtu_select_flip_nav1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_NAV1);
    rtu_select_flip_adf1 = XPLMCreateCommand("xhsi/rtu/select_flip_adf1", "RTU select/flip ADF1");
    XPLMRegisterCommandHandler(rtu_select_flip_adf1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_ADF1);
    rtu_select_flip_xpdr = XPLMCreateCommand("xhsi/rtu/select_flip_xpdr", "RTU select/ident XPDR");
    XPLMRegisterCommandHandler(rtu_select_flip_xpdr, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_XPDR);
    rtu_select_flip_tcas = XPLMCreateCommand("xhsi/rtu/select_flip_tcas", "RTU select/cycle TCAS");
    XPLMRegisterCommandHandler(rtu_select_flip_tcas, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_TCAS);
    rtu_select_flip_adf2 = XPLMCreateCommand("xhsi/rtu/select_flip_adf2", "RTU select/flip ADF2");
    XPLMRegisterCommandHandler(rtu_select_flip_adf2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_ADF2);
    rtu_select_flip_nav2 = XPLMCreateCommand("xhsi/rtu/select_flip_nav2", "RTU select/flip NAV2");
    XPLMRegisterCommandHandler(rtu_select_flip_nav2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_NAV2);
    rtu_select_flip_com2 = XPLMCreateCommand("xhsi/rtu/select_flip_com2", "RTU select/flip COM2");
    XPLMRegisterCommandHandler(rtu_select_flip_com2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_COM2);
    // RTU next selection RTU_SELECT_NEXT
    rtu_select_next = XPLMCreateCommand("xhsi/rtu/select_next", "RTU select next radio");
    XPLMRegisterCommandHandler(rtu_select_next, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_NEXT);
    // RTU previous selection RTU_SELECT_PREV
    rtu_select_prev = XPLMCreateCommand("xhsi/rtu/select_prev", "RTU select previous radio");
    XPLMRegisterCommandHandler(rtu_select_prev, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_PREV);
    // RTU flip selected
    rtu_flip_selected = XPLMCreateCommand("xhsi/rtu/flip_selected", "RTU flip selected radio");
    XPLMRegisterCommandHandler(rtu_flip_selected, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_FLIP_SELECTED);
    // RTU tuning
    rtu_coarse_down = XPLMCreateCommand("xhsi/rtu/coarse_down", "RTU coarse down");
    XPLMRegisterCommandHandler(rtu_coarse_down, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_COARSE_DOWN);
    rtu_coarse_up = XPLMCreateCommand("xhsi/rtu/coarse_up", "RTU coarse up");
    XPLMRegisterCommandHandler(rtu_coarse_up, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_COARSE_UP);
    rtu_fine_down = XPLMCreateCommand("xhsi/rtu/fine_down", "RTU fine down");
    XPLMRegisterCommandHandler(rtu_fine_down, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_FINE_DOWN);
    rtu_fine_up = XPLMCreateCommand("xhsi/rtu/fine_up", "RTU fine up");
    XPLMRegisterCommandHandler(rtu_fine_up, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_FINE_UP);
    
    // XPDR mode
    xpdr_mode_up = XPLMCreateCommand("xhsi/xpdr/mode_up", "XPDR mode up");
    XPLMRegisterCommandHandler(xpdr_mode_up, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_MODE_UP);
    xpdr_mode_down = XPLMCreateCommand("xhsi/xpdr/mode_down", "XPDR mode down");
    XPLMRegisterCommandHandler(xpdr_mode_down, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_MODE_DOWN);
    xpdr_mode_cycle = XPLMCreateCommand("xhsi/xpdr/mode_cycle", "XPDR mode cycle");
    XPLMRegisterCommandHandler(xpdr_mode_cycle, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_MODE_CYCLE);
    // XPDR code
    xpdr_code_hundreds_up = XPLMCreateCommand("xhsi/xpdr/code_hundreds_up", "XPDR code hundreds(thousands) up");
    XPLMRegisterCommandHandler(xpdr_code_hundreds_up, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_CODE_HUNDREDS_UP);
    xpdr_code_hundreds_down = XPLMCreateCommand("xhsi/xpdr/code_hundreds_down", "XPDR code hundreds(thousands) down");
    XPLMRegisterCommandHandler(xpdr_code_hundreds_down, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_CODE_HUNDREDS_DOWN);
    xpdr_code_units_up = XPLMCreateCommand("xhsi/xpdr/code_units_up", "XPDR code units(tens) up");
    XPLMRegisterCommandHandler(xpdr_code_units_up, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_CODE_UNITS_UP);
    xpdr_code_units_down = XPLMCreateCommand("xhsi/xpdr/code_units_down", "XPDR code units(tens) down");
    XPLMRegisterCommandHandler(xpdr_code_units_down, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_CODE_UNITS_DOWN);
    
    // ADF tuning
    // ADF1
    adf1_hundreds_up = XPLMCreateCommand("xhsi/adf/adf1_hundreds_up", "ADF1 hundreds(thousands) up");
    XPLMRegisterCommandHandler(adf1_hundreds_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1 + ADF_HUNDREDS_UP));
    adf1_hundreds_down = XPLMCreateCommand("xhsi/adf/adf1_hundreds_down", "ADF1 hundreds(thousands) down");
    XPLMRegisterCommandHandler(adf1_hundreds_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1 + ADF_HUNDREDS_DOWN));
    adf1_units_up = XPLMCreateCommand("xhsi/adf/adf1_units_up", "ADF1 units(tens) up");
    XPLMRegisterCommandHandler(adf1_units_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1 + ADF_UNITS_UP));
    adf1_units_down = XPLMCreateCommand("xhsi/adf/adf1_units_down", "ADF1 units(tens) down");
    XPLMRegisterCommandHandler(adf1_units_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1 + ADF_UNITS_DOWN));
    // ADF1 standby
    adf1_stby_hundreds_up = XPLMCreateCommand("xhsi/adf/adf1_stby_hundreds_up", "Standby ADF1 hundreds(thousands) up");
    XPLMRegisterCommandHandler(adf1_stby_hundreds_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1_STBY + ADF_HUNDREDS_UP));
    adf1_stby_hundreds_down = XPLMCreateCommand("xhsi/adf/adf1_stby_hundreds_down", "Standby ADF1 hundreds(thousands) down");
    XPLMRegisterCommandHandler(adf1_stby_hundreds_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1_STBY + ADF_HUNDREDS_DOWN));
    adf1_stby_units_up = XPLMCreateCommand("xhsi/adf/adf1_stby_units_up", "Standby ADF1 units(tens) up");
    XPLMRegisterCommandHandler(adf1_stby_units_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1_STBY + ADF_UNITS_UP));
    adf1_stby_units_down = XPLMCreateCommand("xhsi/adf/adf1_stby_units_down", "Standby ADF1 units(tens) down");
    XPLMRegisterCommandHandler(adf1_stby_units_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1_STBY + ADF_UNITS_DOWN));
    // ADF2
    adf2_hundreds_up = XPLMCreateCommand("xhsi/adf/adf2_hundreds_up", "ADF2 hundreds(thousands) up");
    XPLMRegisterCommandHandler(adf2_hundreds_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2 + ADF_HUNDREDS_UP));
    adf2_hundreds_down = XPLMCreateCommand("xhsi/adf/adf2_hundreds_down", "ADF2 hundreds(thousands) down");
    XPLMRegisterCommandHandler(adf2_hundreds_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2 + ADF_HUNDREDS_DOWN));
    adf2_units_up = XPLMCreateCommand("xhsi/adf/adf2_units_up", "ADF2 units(tens) up");
    XPLMRegisterCommandHandler(adf2_units_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2 + ADF_UNITS_UP));
    adf2_units_down = XPLMCreateCommand("xhsi/adf/adf2_units_down", "ADF2 units(tens) down");
    XPLMRegisterCommandHandler(adf2_units_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2 + ADF_UNITS_DOWN));
    // ADF2 standby
    adf2_stby_hundreds_up = XPLMCreateCommand("xhsi/adf/adf2_stby_hundreds_up", "Standby ADF2 hundreds(thousands) up");
    XPLMRegisterCommandHandler(adf2_stby_hundreds_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2_STBY + ADF_HUNDREDS_UP));
    adf2_stby_hundreds_down = XPLMCreateCommand("xhsi/adf/adf2_stby_hundreds_down", "Standby ADF2 hundreds(thousands) down");
    XPLMRegisterCommandHandler(adf2_stby_hundreds_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2_STBY + ADF_HUNDREDS_DOWN));
    adf2_stby_units_up = XPLMCreateCommand("xhsi/adf/adf2_stby_units_up", "Standby ADF2 units(tens) up");
    XPLMRegisterCommandHandler(adf2_stby_units_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2_STBY + ADF_UNITS_UP));
    adf2_stby_units_down = XPLMCreateCommand("xhsi/adf/adf2_stby_units_down", "Standby ADF2 units(tens) down");
    XPLMRegisterCommandHandler(adf2_stby_units_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2_STBY + ADF_UNITS_DOWN));
    
    
    // special case: use these existing commands for an MCP
    sim_autopilot_fdir_servos_toggle = XPLMFindCommand("sim/autopilot/fdir_servos_toggle");
    sim_autopilot_autothrottle_toggle = XPLMFindCommand("sim/autopilot/autothrottle_toggle");
    sim_autopilot_level_change = XPLMFindCommand("sim/autopilot/level_change");
    sim_autopilot_heading = XPLMFindCommand("sim/autopilot/heading");
    sim_autopilot_vertical_speed = XPLMFindCommand("sim/autopilot/vertical_speed");
    sim_autopilot_nav = XPLMFindCommand("sim/autopilot/NAV");
    sim_autopilot_approach = XPLMFindCommand("sim/autopilot/approach");
    sim_autopilot_glide_slope = XPLMFindCommand("sim/autopilot/glide_slope");
    sim_autopilot_back_course = XPLMFindCommand("sim/autopilot/back_course");
    sim_autopilot_altitude_hold = XPLMFindCommand("sim/autopilot/altitude_hold");
    sim_autopilot_wing_leveler = XPLMFindCommand("sim/autopilot/wing_leveler");

    // clock
    sim_instruments_timer_show_date = XPLMFindCommand("sim/instruments/timer_show_date");

    // special case: use these existing commands for lights
    sim_lights_nav_lights_toggle = XPLMFindCommand("sim/lights/nav_lights_toggle");
    sim_lights_beacon_lights_toggle = XPLMFindCommand("sim/lights/beacon_lights_toggle");
    sim_lights_taxi_lights_toggle = XPLMFindCommand("sim/lights/taxi_lights_toggle");
    sim_lights_strobe_lights_toggle = XPLMFindCommand("sim/lights/strobe_lights_toggle");
    sim_lights_landing_lights_toggle = XPLMFindCommand("sim/lights/landing_lights_toggle");

    // special case: use these existing commands for flaps, gear and speedbrake
    sim_flight_controls_flaps_down = XPLMFindCommand("sim/flight_controls/flaps_down");
    sim_flight_controls_flaps_up = XPLMFindCommand("sim/flight_controls/flaps_up");
    sim_flight_controls_landing_gear_toggle = XPLMFindCommand("sim/flight_controls/landing_gear_toggle");
    sim_flight_controls_landing_gear_up = XPLMFindCommand("sim/flight_controls/landing_gear_up");
    sim_flight_controls_landing_gear_down = XPLMFindCommand("sim/flight_controls/landing_gear_down");
    sim_flight_controls_speed_brakes_down_one = XPLMFindCommand("sim/flight_controls/speed_brakes_down_one");
    sim_flight_controls_speed_brakes_up_one = XPLMFindCommand("sim/flight_controls/speed_brakes_up_one");

    // master caution, warning, accept commands
    sim_annunciator_clear_master_warning = XPLMFindCommand("sim/annunciator/clear_master_warning");
    sim_annunciator_clear_master_caution = XPLMFindCommand("sim/annunciator/clear_master_caution");
    sim_annunciator_clear_master_accept  = XPLMFindCommand("sim/annunciator/clear_master_accept");

    // CDU Keyboard

    /*
     * CDU 1 buttons
     */

    sim_fms[SIM_FMS1_LS_1L] = XPLMFindCommand("sim/FMS/ls_1l");
    sim_fms[SIM_FMS1_LS_2L] = XPLMFindCommand("sim/FMS/ls_2l");
    sim_fms[SIM_FMS1_LS_3L] = XPLMFindCommand("sim/FMS/ls_3l");
    sim_fms[SIM_FMS1_LS_4L] = XPLMFindCommand("sim/FMS/ls_4l");
    sim_fms[SIM_FMS1_LS_5L] = XPLMFindCommand("sim/FMS/ls_5l");
    sim_fms[SIM_FMS1_LS_6L] = XPLMFindCommand("sim/FMS/ls_6l");
    sim_fms[SIM_FMS1_LS_1R] = XPLMFindCommand("sim/FMS/ls_1r");
    sim_fms[SIM_FMS1_LS_2R] = XPLMFindCommand("sim/FMS/ls_2r");
    sim_fms[SIM_FMS1_LS_3R] = XPLMFindCommand("sim/FMS/ls_3r");
    sim_fms[SIM_FMS1_LS_4R] = XPLMFindCommand("sim/FMS/ls_4r");
    sim_fms[SIM_FMS1_LS_5R] = XPLMFindCommand("sim/FMS/ls_5r");
    sim_fms[SIM_FMS1_LS_6R] = XPLMFindCommand("sim/FMS/ls_6r");

    // CDU Pages
    sim_fms[SIM_FMS1_INIT] = XPLMFindCommand("sim/FMS/index");
    sim_fms[SIM_FMS1_RTE] = XPLMFindCommand("sim/FMS/fpln");
    sim_fms[SIM_FMS1_CLB] = XPLMFindCommand("sim/FMS/clb");
    sim_fms[SIM_FMS1_CRZ] = XPLMFindCommand("sim/FMS/crz");
    sim_fms[SIM_FMS1_DES] = XPLMFindCommand("sim/FMS/des");
    sim_fms[SIM_FMS1_DIR_INTC] = XPLMFindCommand("sim/FMS/dir_intc");
    sim_fms[SIM_FMS1_LEGS] = XPLMFindCommand("sim/FMS/legs");
    sim_fms[SIM_FMS1_DEP_ARR] = XPLMFindCommand("sim/FMS/dep_arr");
    sim_fms[SIM_FMS1_HOLD] = XPLMFindCommand("sim/FMS/hold");
    sim_fms[SIM_FMS1_PROG] = XPLMFindCommand("sim/FMS/prog");
    sim_fms[SIM_FMS1_EXEC] = XPLMFindCommand("sim/FMS/exec");
    sim_fms[SIM_FMS1_FIX] = XPLMFindCommand("sim/FMS/fix");
    sim_fms[SIM_FMS1_NAVRAD] = XPLMFindCommand("sim/FMS/navrad");
    sim_fms[SIM_FMS1_PREV_PAGE] = XPLMFindCommand("sim/FMS/prev");
    sim_fms[SIM_FMS1_NEXT_PAGE] = XPLMFindCommand("sim/FMS/next");
    // sim_fms[SIM_FMS1_PERF] = XPLMFindCommand("sim/FMS/fmc1_n1_lim");


    sim_fms[SIM_FMS1_BACK] = XPLMFindCommand("sim/FMS/key_back");
    sim_fms[SIM_FMS1_DEL] = XPLMFindCommand("sim/FMS/key_delete");
    sim_fms[SIM_FMS1_KEY_CLR] = XPLMFindCommand("sim/FMS/key_clear");

    sim_fms[SIM_FMS1_SPACE] = XPLMFindCommand("sim/FMS/key_space");
    sim_fms[SIM_FMS1_PLUS_M] = XPLMFindCommand("sim/FMS/key_minus");
    sim_fms[SIM_FMS1_DOT] = XPLMFindCommand("sim/FMS/key_period");
    sim_fms[SIM_FMS1_SLASH] = XPLMFindCommand("sim/FMS/key_slash");

    sim_fms[SIM_FMS1_0] = XPLMFindCommand("sim/FMS/key_0");
    sim_fms[SIM_FMS1_1] = XPLMFindCommand("sim/FMS/key_1");
    sim_fms[SIM_FMS1_2] = XPLMFindCommand("sim/FMS/key_2");
    sim_fms[SIM_FMS1_3] = XPLMFindCommand("sim/FMS/key_3");
    sim_fms[SIM_FMS1_4] = XPLMFindCommand("sim/FMS/key_4");
    sim_fms[SIM_FMS1_5] = XPLMFindCommand("sim/FMS/key_5");
    sim_fms[SIM_FMS1_6] = XPLMFindCommand("sim/FMS/key_6");
    sim_fms[SIM_FMS1_7] = XPLMFindCommand("sim/FMS/key_7");
    sim_fms[SIM_FMS1_8] = XPLMFindCommand("sim/FMS/key_8");
    sim_fms[SIM_FMS1_9] = XPLMFindCommand("sim/FMS/key_9");
    sim_fms[SIM_FMS1_A] = XPLMFindCommand("sim/FMS/key_A");
    sim_fms[SIM_FMS1_B] = XPLMFindCommand("sim/FMS/key_B");
    sim_fms[SIM_FMS1_C] = XPLMFindCommand("sim/FMS/key_C");
    sim_fms[SIM_FMS1_D] = XPLMFindCommand("sim/FMS/key_D");
    sim_fms[SIM_FMS1_E] = XPLMFindCommand("sim/FMS/key_E");
    sim_fms[SIM_FMS1_F] = XPLMFindCommand("sim/FMS/key_F");
    sim_fms[SIM_FMS1_G] = XPLMFindCommand("sim/FMS/key_G");
    sim_fms[SIM_FMS1_H] = XPLMFindCommand("sim/FMS/key_H");
    sim_fms[SIM_FMS1_I] = XPLMFindCommand("sim/FMS/key_I");
    sim_fms[SIM_FMS1_J] = XPLMFindCommand("sim/FMS/key_J");
    sim_fms[SIM_FMS1_K] = XPLMFindCommand("sim/FMS/key_K");
    sim_fms[SIM_FMS1_L] = XPLMFindCommand("sim/FMS/key_L");
    sim_fms[SIM_FMS1_M] = XPLMFindCommand("sim/FMS/key_M");
    sim_fms[SIM_FMS1_N] = XPLMFindCommand("sim/FMS/key_N");
    sim_fms[SIM_FMS1_O] = XPLMFindCommand("sim/FMS/key_O");
    sim_fms[SIM_FMS1_P] = XPLMFindCommand("sim/FMS/key_P");
    sim_fms[SIM_FMS1_Q] = XPLMFindCommand("sim/FMS/key_Q");
    sim_fms[SIM_FMS1_R] = XPLMFindCommand("sim/FMS/key_R");
    sim_fms[SIM_FMS1_S] = XPLMFindCommand("sim/FMS/key_S");
    sim_fms[SIM_FMS1_T] = XPLMFindCommand("sim/FMS/key_T");
    sim_fms[SIM_FMS1_U] = XPLMFindCommand("sim/FMS/key_U");
    sim_fms[SIM_FMS1_V] = XPLMFindCommand("sim/FMS/key_V");
    sim_fms[SIM_FMS1_W] = XPLMFindCommand("sim/FMS/key_W");
    sim_fms[SIM_FMS1_X] = XPLMFindCommand("sim/FMS/key_X");
    sim_fms[SIM_FMS1_Y] = XPLMFindCommand("sim/FMS/key_Y");
    sim_fms[SIM_FMS1_Z] = XPLMFindCommand("sim/FMS/key_Z");

    sim_fms[SIM_FMS1_CDU_POPUP] = XPLMFindCommand("sim/FMS/CDU_popup");
    sim_fms[SIM_FMS1_CDU_POPOUT] = XPLMFindCommand("sim/FMS/CDU_popout");

    // Exists only for FMS1
    sim_fms[SIM_FMS1_CLEAR] = XPLMFindCommand("sim/FMS/clear");
    sim_fms[SIM_FMS1_DIRECT] = XPLMFindCommand("sim/FMS/direct");
    sim_fms[SIM_FMS1_SIGN] = XPLMFindCommand("sim/FMS/sign");
    sim_fms[SIM_FMS1_TYPE_APT] = XPLMFindCommand("sim/FMS/type_apt");
    sim_fms[SIM_FMS1_TYPE_VOR] = XPLMFindCommand("sim/FMS/type_vor");
    sim_fms[SIM_FMS1_TYPE_NDB] = XPLMFindCommand("sim/FMS/type_ndb");
    sim_fms[SIM_FMS1_TYPE_FIX] = XPLMFindCommand("sim/FMS/type_fix");
    sim_fms[SIM_FMS1_TYPE_LATLON] = XPLMFindCommand("sim/FMS/type_latlon");
    sim_fms[SIM_FMS1_FIX_NEXT] = XPLMFindCommand("sim/FMS/fix_next");
    sim_fms[SIM_FMS1_FIX_PREV] = XPLMFindCommand("sim/FMS/fix_prev");
    sim_fms[SIM_FMS1_KEY_LOAD] = XPLMFindCommand("sim/FMS/key_load");
    sim_fms[SIM_FMS1_KEY_SAVE] = XPLMFindCommand("sim/FMS/key_save");

    /*
     * CDU 2 buttons
     */

    sim_fms[SIM_FMS2_LS_1L] = XPLMFindCommand("sim/FMS2/ls_1l");
    sim_fms[SIM_FMS2_LS_2L] = XPLMFindCommand("sim/FMS2/ls_2l");
    sim_fms[SIM_FMS2_LS_3L] = XPLMFindCommand("sim/FMS2/ls_3l");
    sim_fms[SIM_FMS2_LS_4L] = XPLMFindCommand("sim/FMS2/ls_4l");
    sim_fms[SIM_FMS2_LS_5L] = XPLMFindCommand("sim/FMS2/ls_5l");
    sim_fms[SIM_FMS2_LS_6L] = XPLMFindCommand("sim/FMS2/ls_6l");
    sim_fms[SIM_FMS2_LS_1R] = XPLMFindCommand("sim/FMS2/ls_1r");
    sim_fms[SIM_FMS2_LS_2R] = XPLMFindCommand("sim/FMS2/ls_2r");
    sim_fms[SIM_FMS2_LS_3R] = XPLMFindCommand("sim/FMS2/ls_3r");
    sim_fms[SIM_FMS2_LS_4R] = XPLMFindCommand("sim/FMS2/ls_4r");
    sim_fms[SIM_FMS2_LS_5R] = XPLMFindCommand("sim/FMS2/ls_5r");
    sim_fms[SIM_FMS2_LS_6R] = XPLMFindCommand("sim/FMS2/ls_6r");

    sim_fms[SIM_FMS2_INIT] = XPLMFindCommand("sim/FMS2/index");
    sim_fms[SIM_FMS2_RTE] = XPLMFindCommand("sim/FMS2/fpln");
    sim_fms[SIM_FMS2_CLB] = XPLMFindCommand("sim/FMS2/clb");
    sim_fms[SIM_FMS2_CRZ] = XPLMFindCommand("sim/FMS2/crz");
    sim_fms[SIM_FMS2_DES] = XPLMFindCommand("sim/FMS2/des");

    sim_fms[SIM_FMS2_DIR_INTC] = XPLMFindCommand("sim/FMS2/dir_intc");
    sim_fms[SIM_FMS2_LEGS] = XPLMFindCommand("sim/FMS2/legs");
    sim_fms[SIM_FMS2_DEP_ARR] = XPLMFindCommand("sim/FMS2/dep_arr");
    sim_fms[SIM_FMS2_HOLD] = XPLMFindCommand("sim/FMS2/hold");
    sim_fms[SIM_FMS2_PROG] = XPLMFindCommand("sim/FMS2/prog");
    sim_fms[SIM_FMS2_EXEC] = XPLMFindCommand("sim/FMS2/exec");

    sim_fms[SIM_FMS2_PERF] = XPLMFindCommand("sim/FMS2/fmc2_n1_lim");
    sim_fms[SIM_FMS2_FIX] = XPLMFindCommand("sim/FMS2/fix");
    sim_fms[SIM_FMS2_NAVRAD] = XPLMFindCommand("sim/FMS2/navrad");
    sim_fms[SIM_FMS2_PREV_PAGE] = XPLMFindCommand("sim/FMS2/prev");
    sim_fms[SIM_FMS2_NEXT_PAGE] = XPLMFindCommand("sim/FMS2/next");

    sim_fms[SIM_FMS2_BACK] = XPLMFindCommand("sim/FMS2/key_back");
    sim_fms[SIM_FMS2_DEL] = XPLMFindCommand("sim/FMS2/key_delete");
    sim_fms[SIM_FMS2_CLR] = XPLMFindCommand("sim/FMS2/key_clear");
    sim_fms[SIM_FMS2_SPACE] = XPLMFindCommand("sim/FMS2/key_space");
    sim_fms[SIM_FMS2_PLUS_M] = XPLMFindCommand("sim/FMS2/key_minus");
    sim_fms[SIM_FMS2_DOT] = XPLMFindCommand("sim/FMS2/key_period");
    sim_fms[SIM_FMS2_SLASH] = XPLMFindCommand("sim/FMS2/key_slash");

    sim_fms[SIM_FMS2_0] = XPLMFindCommand("sim/FMS2/key_0");
    sim_fms[SIM_FMS2_1] = XPLMFindCommand("sim/FMS2/key_1");
    sim_fms[SIM_FMS2_2] = XPLMFindCommand("sim/FMS2/key_2");
    sim_fms[SIM_FMS2_3] = XPLMFindCommand("sim/FMS2/key_3");
    sim_fms[SIM_FMS2_4] = XPLMFindCommand("sim/FMS2/key_4");
    sim_fms[SIM_FMS2_5] = XPLMFindCommand("sim/FMS2/key_5");
    sim_fms[SIM_FMS2_6] = XPLMFindCommand("sim/FMS2/key_6");
    sim_fms[SIM_FMS2_7] = XPLMFindCommand("sim/FMS2/key_7");
    sim_fms[SIM_FMS2_8] = XPLMFindCommand("sim/FMS2/key_8");
    sim_fms[SIM_FMS2_9] = XPLMFindCommand("sim/FMS2/key_9");
    sim_fms[SIM_FMS2_A] = XPLMFindCommand("sim/FMS2/key_A");
    sim_fms[SIM_FMS2_B] = XPLMFindCommand("sim/FMS2/key_B");
    sim_fms[SIM_FMS2_C] = XPLMFindCommand("sim/FMS2/key_C");
    sim_fms[SIM_FMS2_D] = XPLMFindCommand("sim/FMS2/key_D");
    sim_fms[SIM_FMS2_E] = XPLMFindCommand("sim/FMS2/key_E");
    sim_fms[SIM_FMS2_F] = XPLMFindCommand("sim/FMS2/key_F");
    sim_fms[SIM_FMS2_G] = XPLMFindCommand("sim/FMS2/key_G");
    sim_fms[SIM_FMS2_H] = XPLMFindCommand("sim/FMS2/key_H");
    sim_fms[SIM_FMS2_I] = XPLMFindCommand("sim/FMS2/key_I");
    sim_fms[SIM_FMS2_J] = XPLMFindCommand("sim/FMS2/key_J");
    sim_fms[SIM_FMS2_K] = XPLMFindCommand("sim/FMS2/key_K");
    sim_fms[SIM_FMS2_L] = XPLMFindCommand("sim/FMS2/key_L");
    sim_fms[SIM_FMS2_M] = XPLMFindCommand("sim/FMS2/key_M");
    sim_fms[SIM_FMS2_N] = XPLMFindCommand("sim/FMS2/key_N");
    sim_fms[SIM_FMS2_O] = XPLMFindCommand("sim/FMS2/key_O");
    sim_fms[SIM_FMS2_P] = XPLMFindCommand("sim/FMS2/key_P");
    sim_fms[SIM_FMS2_Q] = XPLMFindCommand("sim/FMS2/key_Q");
    sim_fms[SIM_FMS2_R] = XPLMFindCommand("sim/FMS2/key_R");
    sim_fms[SIM_FMS2_S] = XPLMFindCommand("sim/FMS2/key_S");
    sim_fms[SIM_FMS2_T] = XPLMFindCommand("sim/FMS2/key_T");
    sim_fms[SIM_FMS2_U] = XPLMFindCommand("sim/FMS2/key_U");
    sim_fms[SIM_FMS2_V] = XPLMFindCommand("sim/FMS2/key_V");
    sim_fms[SIM_FMS2_W] = XPLMFindCommand("sim/FMS2/key_W");
    sim_fms[SIM_FMS2_X] = XPLMFindCommand("sim/FMS2/key_X");
    sim_fms[SIM_FMS2_Y] = XPLMFindCommand("sim/FMS2/key_Y");
    sim_fms[SIM_FMS2_Z] = XPLMFindCommand("sim/FMS2/key_Z");

    sim_fms[SIM_FMS2_CDU_POPUP] = XPLMFindCommand("sim/FMS2/CDU_popup");
    sim_fms[SIM_FMS2_CDU_POPOUT] = XPLMFindCommand("sim/FMS2/CDU_popout");

    XPLMDebugString("XHSI: custom commands created and custom command handlers registered\n");

}


void unregisterCommands(void) {

    XPLMDebugString("XHSI: unregistering custom command handlers\n");

    // pilot commands
    // ctr
    XPLMUnregisterCommandHandler(ctr_toggle, (XPLMCommandCallback_f)ctr_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(ctr_on, (XPLMCommandCallback_f)ctr_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(ctr_off, (XPLMCommandCallback_f)ctr_handler, 1, (void *)OFF);

    // zoomin
    XPLMUnregisterCommandHandler(zoom_x100_toggle, (XPLMCommandCallback_f)zoom_x100_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(zoom_x100_on, (XPLMCommandCallback_f)zoom_x100_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(zoom_x100_off, (XPLMCommandCallback_f)zoom_x100_handler, 1, (void *)OFF);

    // range
    XPLMUnregisterCommandHandler(range_10, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_10);
    XPLMUnregisterCommandHandler(range_20, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_20);
    XPLMUnregisterCommandHandler(range_40, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_40);
    XPLMUnregisterCommandHandler(range_80, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_80);
    XPLMUnregisterCommandHandler(range_160, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_160);
    XPLMUnregisterCommandHandler(range_320, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_320);
    XPLMUnregisterCommandHandler(range_640, (XPLMCommandCallback_f)range_handler, 1, (void *)RANGE_640);
    XPLMUnregisterCommandHandler(range_down, (XPLMCommandCallback_f)range_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(range_up, (XPLMCommandCallback_f)range_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(range_cycle, (XPLMCommandCallback_f)range_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(range_shuttle, (XPLMCommandCallback_f)range_handler, 1, (void *)SHUTTLE);

    // ext range
    XPLMUnregisterCommandHandler(ext_range_0_10, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_10 + 1100));
    XPLMUnregisterCommandHandler(ext_range_0_20, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_20 + 1100));
    XPLMUnregisterCommandHandler(ext_range_0_40, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_40 + 1100));
    XPLMUnregisterCommandHandler(ext_range_0_80, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_80 + 1100));
    XPLMUnregisterCommandHandler(ext_range_1_60, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_160 + 1100));
    XPLMUnregisterCommandHandler(ext_range_3_20, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_320 + 1100));
    XPLMUnregisterCommandHandler(ext_range_6_40, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_640 + 1100));
    XPLMUnregisterCommandHandler(ext_range_010, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_10 + 1000));
    XPLMUnregisterCommandHandler(ext_range_020, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_20 + 1000));
    XPLMUnregisterCommandHandler(ext_range_040, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_40 + 1000));
    XPLMUnregisterCommandHandler(ext_range_080, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_80 + 1000));
    XPLMUnregisterCommandHandler(ext_range_160, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_160 + 1000));
    XPLMUnregisterCommandHandler(ext_range_320, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_320 + 1000));
    XPLMUnregisterCommandHandler(ext_range_640, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)(RANGE_640 + 1000));
    XPLMUnregisterCommandHandler(ext_range_down, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(ext_range_up, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(ext_range_cycle, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(ext_range_shuttle, (XPLMCommandCallback_f)ext_range_handler, 1, (void *)SHUTTLE);

    // mode
    XPLMUnregisterCommandHandler(mode_app, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_APP);
    XPLMUnregisterCommandHandler(mode_vor, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_VOR);
    XPLMUnregisterCommandHandler(mode_map, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_MAP);
    XPLMUnregisterCommandHandler(mode_nav, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_NAV);
    XPLMUnregisterCommandHandler(mode_pln, (XPLMCommandCallback_f)mode_handler, 1, (void *)MODE_PLN);
    XPLMUnregisterCommandHandler(mode_down, (XPLMCommandCallback_f)mode_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(mode_up, (XPLMCommandCallback_f)mode_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(mode_cycle, (XPLMCommandCallback_f)mode_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(mode_shuttle, (XPLMCommandCallback_f)mode_handler, 1, (void *)SHUTTLE);

    // B737-Classic modes
    XPLMUnregisterCommandHandler(b737cl_mode_fullvorils, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_FULL_VOR_ILS);
    XPLMUnregisterCommandHandler(b737cl_mode_expvorils, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_EXP_VOR_ILS);
    XPLMUnregisterCommandHandler(b737cl_mode_map, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_MAP);
    XPLMUnregisterCommandHandler(b737cl_mode_ctrmap, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_CTR_MAP);
    XPLMUnregisterCommandHandler(b737cl_mode_plan, (XPLMCommandCallback_f)b737cl_mode_handler, 1, (void *)B737CL_PLAN);

    // radio1
    XPLMUnregisterCommandHandler(radio1_adf, (XPLMCommandCallback_f)radio1_handler, 1, (void *)RADIO_ADF);
    XPLMUnregisterCommandHandler(radio1_off, (XPLMCommandCallback_f)radio1_handler, 1, (void *)RADIO_OFF);
    XPLMUnregisterCommandHandler(radio1_nav, (XPLMCommandCallback_f)radio1_handler, 1, (void *)RADIO_NAV);
    XPLMUnregisterCommandHandler(radio1_down, (XPLMCommandCallback_f)radio1_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(radio1_up, (XPLMCommandCallback_f)radio1_handler, 1, (void *)UP);
//    XPLMUnregisterCommandHandler(radio1_cycle, (XPLMCommandCallback_f)radio1_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(radio1_shuttle, (XPLMCommandCallback_f)radio1_handler, 1, (void *)SHUTTLE);

    // radio2
    XPLMUnregisterCommandHandler(radio2_adf, (XPLMCommandCallback_f)radio2_handler, 1, (void *)RADIO_ADF);
    XPLMUnregisterCommandHandler(radio2_off, (XPLMCommandCallback_f)radio2_handler, 1, (void *)RADIO_OFF);
    XPLMUnregisterCommandHandler(radio2_nav, (XPLMCommandCallback_f)radio2_handler, 1, (void *)RADIO_NAV);
    XPLMUnregisterCommandHandler(radio2_down, (XPLMCommandCallback_f)radio2_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(radio2_up, (XPLMCommandCallback_f)radio2_handler, 1, (void *)UP);
//    XPLMUnregisterCommandHandler(radio2_cycle, (XPLMCommandCallback_f)radio2_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(radio2_shuttle, (XPLMCommandCallback_f)radio2_handler, 1, (void *)SHUTTLE);

    // source
    XPLMUnregisterCommandHandler(source_nav1, (XPLMCommandCallback_f)source_handler, 1, (void *)SOURCE_NAV1);
    XPLMUnregisterCommandHandler(source_nav2, (XPLMCommandCallback_f)source_handler, 1, (void *)SOURCE_NAV2);
    XPLMUnregisterCommandHandler(source_fmc, (XPLMCommandCallback_f)source_handler, 1, (void *)SOURCE_FMC);
    XPLMUnregisterCommandHandler(source_down, (XPLMCommandCallback_f)source_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(source_up, (XPLMCommandCallback_f)source_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(source_cycle, (XPLMCommandCallback_f)source_handler, 1, (void *)CYCLE);

    // tfc
    XPLMUnregisterCommandHandler(tfc_toggle, (XPLMCommandCallback_f)tfc_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(tfc_on, (XPLMCommandCallback_f)tfc_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(tfc_off, (XPLMCommandCallback_f)tfc_handler, 1, (void *)OFF);

    // arpt
    XPLMUnregisterCommandHandler(arpt_toggle, (XPLMCommandCallback_f)arpt_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(arpt_on, (XPLMCommandCallback_f)arpt_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(arpt_off, (XPLMCommandCallback_f)arpt_handler, 1, (void *)OFF);

    // wpt
    XPLMUnregisterCommandHandler(wpt_toggle, (XPLMCommandCallback_f)wpt_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wpt_on, (XPLMCommandCallback_f)wpt_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wpt_off, (XPLMCommandCallback_f)wpt_handler, 1, (void *)OFF);

    // vor
    XPLMUnregisterCommandHandler(vor_toggle, (XPLMCommandCallback_f)vor_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(vor_on, (XPLMCommandCallback_f)vor_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(vor_off, (XPLMCommandCallback_f)vor_handler, 1, (void *)OFF);

    // ndb
    XPLMUnregisterCommandHandler(ndb_toggle, (XPLMCommandCallback_f)ndb_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(ndb_on, (XPLMCommandCallback_f)ndb_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(ndb_off, (XPLMCommandCallback_f)ndb_handler, 1, (void *)OFF);

    // sta = vor + ndb
    XPLMUnregisterCommandHandler(sta_toggle, (XPLMCommandCallback_f)sta_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(sta_on, (XPLMCommandCallback_f)sta_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(sta_off, (XPLMCommandCallback_f)sta_handler, 1, (void *)OFF);
    XPLMUnregisterCommandHandler(sta_cycle, (XPLMCommandCallback_f)sta_handler, 1, (void *)CYCLE);

    // declutter
    XPLMUnregisterCommandHandler(declutter_cycle, (XPLMCommandCallback_f)declutter_handler, 1, (void *)CYCLE);

    // data = route data
    XPLMUnregisterCommandHandler(data_toggle, (XPLMCommandCallback_f)data_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(data_on, (XPLMCommandCallback_f)data_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(data_off, (XPLMCommandCallback_f)data_handler, 1, (void *)OFF);

    // pos
    XPLMUnregisterCommandHandler(pos_toggle, (XPLMCommandCallback_f)pos_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(pos_on, (XPLMCommandCallback_f)pos_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(pos_off, (XPLMCommandCallback_f)pos_handler, 1, (void *)OFF);

    // terrain on ND
    XPLMUnregisterCommandHandler(terrain_toggle, (XPLMCommandCallback_f)terrain_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(terrain_on, (XPLMCommandCallback_f)terrain_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(terrain_off, (XPLMCommandCallback_f)terrain_handler, 1, (void *)OFF);

    // Vertical Path on ND
    XPLMUnregisterCommandHandler(vp_toggle, (XPLMCommandCallback_f)vertical_path_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(vp_on, (XPLMCommandCallback_f)vertical_path_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(vp_off, (XPLMCommandCallback_f)vertical_path_handler, 1, (void *)OFF);

    // Weather Radar
    XPLMUnregisterCommandHandler(wxr_toggle, (XPLMCommandCallback_f)wxr_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_on, (XPLMCommandCallback_f)wxr_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_off, (XPLMCommandCallback_f)wxr_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(wxr_gain_up, (XPLMCommandCallback_f)wxr_gain_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(wxr_gain_down, (XPLMCommandCallback_f)wxr_gain_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(wxr_gain_middle, (XPLMCommandCallback_f)wxr_gain_handler, 1, (void *)MODE_CENTERED);

    XPLMUnregisterCommandHandler(wxr_tilt_up, (XPLMCommandCallback_f)wxr_tilt_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(wxr_tilt_down, (XPLMCommandCallback_f)wxr_tilt_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(wxr_tilt_zero, (XPLMCommandCallback_f)wxr_tilt_handler, 1, (void *)MODE_CENTERED);

    XPLMUnregisterCommandHandler(wxr_auto_gain_toggle, (XPLMCommandCallback_f)wxr_auto_gain_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_auto_gain_on, (XPLMCommandCallback_f)wxr_auto_gain_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_auto_gain_off, (XPLMCommandCallback_f)wxr_auto_gain_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(wxr_auto_gain_toggle, (XPLMCommandCallback_f)wxr_auto_tilt_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_auto_gain_on, (XPLMCommandCallback_f)wxr_auto_tilt_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_auto_gain_off, (XPLMCommandCallback_f)wxr_auto_tilt_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(wxr_test_toggle, (XPLMCommandCallback_f)wxr_test_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_test_on, (XPLMCommandCallback_f)wxr_test_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_test_off, (XPLMCommandCallback_f)wxr_test_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(wxr_mode_up, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(wxr_mode_down, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(wxr_mode_off, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_OFF);
    XPLMUnregisterCommandHandler(wxr_mode_on, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_ON);
    XPLMUnregisterCommandHandler(wxr_mode_turb, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_TURB);
    XPLMUnregisterCommandHandler(wxr_mode_map, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_MAP);
    XPLMUnregisterCommandHandler(wxr_mode_force_on, (XPLMCommandCallback_f)wxr_mode_handler, 1, (void *)WXR_MODE_FORCE_ON);

    XPLMUnregisterCommandHandler(wxr_slave_toggle, (XPLMCommandCallback_f)wxr_slave_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_slave_on, (XPLMCommandCallback_f)wxr_slave_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_slave_off, (XPLMCommandCallback_f)wxr_slave_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(wxr_react_toggle, (XPLMCommandCallback_f)wxr_react_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_react_on, (XPLMCommandCallback_f)wxr_react_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_react_off, (XPLMCommandCallback_f)wxr_react_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(wxr_narrow_toggle, (XPLMCommandCallback_f)wxr_narrow_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_narrow_on, (XPLMCommandCallback_f)wxr_narrow_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_narrow_off, (XPLMCommandCallback_f)wxr_narrow_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(wxr_alert_toggle, (XPLMCommandCallback_f)wxr_alert_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_alert_on, (XPLMCommandCallback_f)wxr_alert_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_alert_off, (XPLMCommandCallback_f)wxr_alert_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(wxr_target_toggle, (XPLMCommandCallback_f)wxr_target_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(wxr_target_on, (XPLMCommandCallback_f)wxr_target_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(wxr_target_off, (XPLMCommandCallback_f)wxr_target_handler, 1, (void *)OFF);


    // xhsi/pfd_pilot/...
    // mins mode
    XPLMUnregisterCommandHandler(mins_toggle, (XPLMCommandCallback_f)mins_mode_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(mins_radio, (XPLMCommandCallback_f)mins_mode_handler, 1, (void *)MINS_RADIO);
    XPLMUnregisterCommandHandler(mins_baro, (XPLMCommandCallback_f)mins_mode_handler, 1, (void *)MINS_BARO);
    // mins value
    XPLMUnregisterCommandHandler(mins_reset, (XPLMCommandCallback_f)mins_value_handler, 1, (void *)OFF);
    XPLMUnregisterCommandHandler(mins_down, (XPLMCommandCallback_f)mins_value_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(mins_up, (XPLMCommandCallback_f)mins_value_handler, 1, (void *)UP);

    // Instrument Landing System (PFD & ND)
    XPLMUnregisterCommandHandler(ils_toggle, (XPLMCommandCallback_f)ils_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(ils_on, (XPLMCommandCallback_f)ils_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(ils_off, (XPLMCommandCallback_f)ils_handler, 1, (void *)OFF);

    // Metric altitude on PFD/ND
    XPLMUnregisterCommandHandler(metric_alt_toggle, (XPLMCommandCallback_f)metric_alt_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(metric_alt_on, (XPLMCommandCallback_f)metric_alt_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(metric_alt_off, (XPLMCommandCallback_f)metric_alt_handler, 1, (void *)OFF);

    // Track FPA / FPV / Bird : on display FPA/FPV/Bird , off display flight director
    XPLMUnregisterCommandHandler(track_fpa_toggle, (XPLMCommandCallback_f)track_fpa_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(track_fpa_on, (XPLMCommandCallback_f)track_fpa_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(track_fpa_off, (XPLMCommandCallback_f)track_fpa_handler, 1, (void *)OFF);

    // copilot commands
    // copilot ctr
    XPLMUnregisterCommandHandler(copilot_ctr_toggle, (XPLMCommandCallback_f)copilot_ctr_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_ctr_on, (XPLMCommandCallback_f)copilot_ctr_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_ctr_off, (XPLMCommandCallback_f)copilot_ctr_handler, 1, (void *)OFF);

    // copilot zoomin
    XPLMUnregisterCommandHandler(copilot_zoom_x100_toggle, (XPLMCommandCallback_f)copilot_zoom_x100_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_zoom_x100_on, (XPLMCommandCallback_f)copilot_zoom_x100_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_zoom_x100_off, (XPLMCommandCallback_f)copilot_zoom_x100_handler, 1, (void *)OFF);

    // copilot range
    XPLMUnregisterCommandHandler(copilot_range_10, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_10);
    XPLMUnregisterCommandHandler(copilot_range_20, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_20);
    XPLMUnregisterCommandHandler(copilot_range_40, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_40);
    XPLMUnregisterCommandHandler(copilot_range_80, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_80);
    XPLMUnregisterCommandHandler(copilot_range_160, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_160);
    XPLMUnregisterCommandHandler(copilot_range_320, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_320);
    XPLMUnregisterCommandHandler(copilot_range_640, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)RANGE_640);
    XPLMUnregisterCommandHandler(copilot_range_down, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_range_up, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(copilot_range_cycle, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(copilot_range_shuttle, (XPLMCommandCallback_f)copilot_range_handler, 1, (void *)SHUTTLE);

    // copilot ext range
    XPLMUnregisterCommandHandler(copilot_ext_range_0_10, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_10 + 1100));
    XPLMUnregisterCommandHandler(copilot_ext_range_0_20, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_20 + 1100));
    XPLMUnregisterCommandHandler(copilot_ext_range_0_40, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_40 + 1100));
    XPLMUnregisterCommandHandler(copilot_ext_range_0_80, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_80 + 1100));
    XPLMUnregisterCommandHandler(copilot_ext_range_1_60, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_160 + 1100));
    XPLMUnregisterCommandHandler(copilot_ext_range_3_20, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_320 + 1100));
    XPLMUnregisterCommandHandler(copilot_ext_range_6_40, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_640 + 1100));
    XPLMUnregisterCommandHandler(copilot_ext_range_010, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_10 + 1000));
    XPLMUnregisterCommandHandler(copilot_ext_range_020, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_20 + 1000));
    XPLMUnregisterCommandHandler(copilot_ext_range_040, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_40 + 1000));
    XPLMUnregisterCommandHandler(copilot_ext_range_080, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_80 + 1000));
    XPLMUnregisterCommandHandler(copilot_ext_range_160, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_160 + 1000));
    XPLMUnregisterCommandHandler(copilot_ext_range_320, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_320 + 1000));
    XPLMUnregisterCommandHandler(copilot_ext_range_640, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)(RANGE_640 + 1000));
    XPLMUnregisterCommandHandler(copilot_ext_range_down, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_ext_range_up, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(copilot_ext_range_cycle, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(copilot_ext_range_shuttle, (XPLMCommandCallback_f)copilot_ext_range_handler, 1, (void *)SHUTTLE);

    // copilot mode
    XPLMUnregisterCommandHandler(copilot_mode_app, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_APP);
    XPLMUnregisterCommandHandler(copilot_mode_vor, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_VOR);
    XPLMUnregisterCommandHandler(copilot_mode_map, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_MAP);
    XPLMUnregisterCommandHandler(copilot_mode_nav, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_NAV);
    XPLMUnregisterCommandHandler(copilot_mode_pln, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)MODE_PLN);
    XPLMUnregisterCommandHandler(copilot_mode_down, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_mode_up, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(copilot_mode_cycle, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(copilot_mode_shuttle, (XPLMCommandCallback_f)copilot_mode_handler, 1, (void *)SHUTTLE);

    // copilot B737-Classic modes
    XPLMUnregisterCommandHandler(b737cl_copilot_mode_fullvorils, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_FULL_VOR_ILS);
    XPLMUnregisterCommandHandler(b737cl_copilot_mode_expvorils, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_EXP_VOR_ILS);
    XPLMUnregisterCommandHandler(b737cl_copilot_mode_map, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_MAP);
    XPLMUnregisterCommandHandler(b737cl_copilot_mode_ctrmap, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_CTR_MAP);
    XPLMUnregisterCommandHandler(b737cl_copilot_mode_plan, (XPLMCommandCallback_f)b737cl_copilot_mode_handler, 1, (void *)B737CL_PLAN);

    // copilot radio1
    XPLMUnregisterCommandHandler(copilot_radio1_adf, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)RADIO_ADF);
    XPLMUnregisterCommandHandler(copilot_radio1_off, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)RADIO_OFF);
    XPLMUnregisterCommandHandler(copilot_radio1_nav, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)RADIO_NAV);
    XPLMUnregisterCommandHandler(copilot_radio1_down, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_radio1_up, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)UP);
//    XPLMUnregisterCommandHandler(copilot_radio1_cycle, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(copilot_radio1_shuttle, (XPLMCommandCallback_f)copilot_radio1_handler, 1, (void *)SHUTTLE);

    // copilot radio2
    XPLMUnregisterCommandHandler(copilot_radio2_adf, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)RADIO_ADF);
    XPLMUnregisterCommandHandler(copilot_radio2_off, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)RADIO_OFF);
    XPLMUnregisterCommandHandler(copilot_radio2_nav, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)RADIO_NAV);
    XPLMUnregisterCommandHandler(copilot_radio2_down, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_radio2_up, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)UP);
//    XPLMUnregisterCommandHandler(copilot_radio2_cycle, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(copilot_radio2_shuttle, (XPLMCommandCallback_f)copilot_radio2_handler, 1, (void *)SHUTTLE);

    // copilot source
    XPLMUnregisterCommandHandler(copilot_source_nav1, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)SOURCE_NAV1);
    XPLMUnregisterCommandHandler(copilot_source_nav2, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)SOURCE_NAV2);
    XPLMUnregisterCommandHandler(copilot_source_fmc, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)SOURCE_FMC);
    XPLMUnregisterCommandHandler(copilot_source_down, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_source_up, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(copilot_source_cycle, (XPLMCommandCallback_f)copilot_source_handler, 1, (void *)CYCLE);

    // copilot tfc
    XPLMUnregisterCommandHandler(copilot_tfc_toggle, (XPLMCommandCallback_f)copilot_tfc_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_tfc_on, (XPLMCommandCallback_f)copilot_tfc_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_tfc_off, (XPLMCommandCallback_f)copilot_tfc_handler, 1, (void *)OFF);

    // copilot arpt
    XPLMUnregisterCommandHandler(copilot_arpt_toggle, (XPLMCommandCallback_f)copilot_arpt_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_arpt_on, (XPLMCommandCallback_f)copilot_arpt_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_arpt_off, (XPLMCommandCallback_f)copilot_arpt_handler, 1, (void *)OFF);

    // copilot wpt
    XPLMUnregisterCommandHandler(copilot_wpt_toggle, (XPLMCommandCallback_f)copilot_wpt_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wpt_on, (XPLMCommandCallback_f)copilot_wpt_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wpt_off, (XPLMCommandCallback_f)copilot_wpt_handler, 1, (void *)OFF);

    // copilot vor
    XPLMUnregisterCommandHandler(copilot_vor_toggle, (XPLMCommandCallback_f)copilot_vor_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_vor_on, (XPLMCommandCallback_f)copilot_vor_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_vor_off, (XPLMCommandCallback_f)copilot_vor_handler, 1, (void *)OFF);

    // copilot ndb
    XPLMUnregisterCommandHandler(copilot_ndb_toggle, (XPLMCommandCallback_f)copilot_ndb_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_ndb_on, (XPLMCommandCallback_f)copilot_ndb_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_ndb_off, (XPLMCommandCallback_f)copilot_ndb_handler, 1, (void *)OFF);

    // copilot sta = vor + ndb
    XPLMUnregisterCommandHandler(copilot_sta_toggle, (XPLMCommandCallback_f)copilot_sta_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_sta_on, (XPLMCommandCallback_f)copilot_sta_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_sta_off, (XPLMCommandCallback_f)copilot_sta_handler, 1, (void *)OFF);
    XPLMUnregisterCommandHandler(copilot_sta_cycle, (XPLMCommandCallback_f)copilot_sta_handler, 1, (void *)CYCLE);

    // copilot declutter
    XPLMUnregisterCommandHandler(copilot_declutter_cycle, (XPLMCommandCallback_f)copilot_declutter_handler, 1, (void *)CYCLE);

    // copilot data
    XPLMUnregisterCommandHandler(copilot_data_toggle, (XPLMCommandCallback_f)copilot_data_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_data_on, (XPLMCommandCallback_f)copilot_data_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_data_off, (XPLMCommandCallback_f)copilot_data_handler, 1, (void *)OFF);

    // copilot pos
    XPLMUnregisterCommandHandler(copilot_pos_toggle, (XPLMCommandCallback_f)copilot_pos_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_pos_on, (XPLMCommandCallback_f)copilot_pos_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_pos_off, (XPLMCommandCallback_f)copilot_pos_handler, 1, (void *)OFF);

    // copilot terrain
    XPLMUnregisterCommandHandler(copilot_terrain_toggle, (XPLMCommandCallback_f)copilot_terrain_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_terrain_on, (XPLMCommandCallback_f)copilot_terrain_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_terrain_off, (XPLMCommandCallback_f)copilot_terrain_handler, 1, (void *)OFF);

    // copilot Vertical Path on ND
    XPLMUnregisterCommandHandler(copilot_vp_toggle, (XPLMCommandCallback_f)copilot_vertical_path_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_vp_on, (XPLMCommandCallback_f)copilot_vertical_path_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_vp_off, (XPLMCommandCallback_f)copilot_vertical_path_handler, 1, (void *)OFF);

    // Weather Radar
    XPLMUnregisterCommandHandler(copilot_wxr_toggle, (XPLMCommandCallback_f)copilot_wxr_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_on, (XPLMCommandCallback_f)copilot_wxr_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_off, (XPLMCommandCallback_f)copilot_wxr_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(copilot_wxr_gain_up, (XPLMCommandCallback_f)copilot_wxr_gain_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(copilot_wxr_gain_down, (XPLMCommandCallback_f)copilot_wxr_gain_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_wxr_gain_middle, (XPLMCommandCallback_f)copilot_wxr_gain_handler, 1, (void *)MODE_CENTERED);

    XPLMUnregisterCommandHandler(copilot_wxr_tilt_up, (XPLMCommandCallback_f)copilot_wxr_tilt_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(copilot_wxr_tilt_down, (XPLMCommandCallback_f)copilot_wxr_tilt_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_wxr_tilt_zero, (XPLMCommandCallback_f)copilot_wxr_tilt_handler, 1, (void *)MODE_CENTERED);

    XPLMUnregisterCommandHandler(copilot_wxr_auto_gain_toggle, (XPLMCommandCallback_f)copilot_wxr_auto_gain_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_auto_gain_on, (XPLMCommandCallback_f)copilot_wxr_auto_gain_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_auto_gain_off, (XPLMCommandCallback_f)copilot_wxr_auto_gain_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(copilot_wxr_auto_tilt_toggle, (XPLMCommandCallback_f)copilot_wxr_auto_tilt_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_auto_tilt_on, (XPLMCommandCallback_f)copilot_wxr_auto_tilt_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_auto_tilt_off, (XPLMCommandCallback_f)copilot_wxr_auto_tilt_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(copilot_wxr_test_toggle, (XPLMCommandCallback_f)copilot_wxr_test_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_test_on, (XPLMCommandCallback_f)copilot_wxr_test_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_test_off, (XPLMCommandCallback_f)copilot_wxr_test_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(copilot_wxr_mode_up, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(copilot_wxr_mode_down, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_wxr_mode_off, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_OFF);
    XPLMUnregisterCommandHandler(copilot_wxr_mode_on, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_ON);
    XPLMUnregisterCommandHandler(copilot_wxr_mode_turb, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_TURB);
    XPLMUnregisterCommandHandler(copilot_wxr_mode_map, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_MAP);
    XPLMUnregisterCommandHandler(copilot_wxr_mode_force_on, (XPLMCommandCallback_f)copilot_wxr_mode_handler, 1, (void *)WXR_MODE_FORCE_ON);

    XPLMUnregisterCommandHandler(copilot_wxr_slave_toggle, (XPLMCommandCallback_f)copilot_wxr_slave_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_slave_on, (XPLMCommandCallback_f)copilot_wxr_slave_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_slave_off, (XPLMCommandCallback_f)copilot_wxr_slave_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(copilot_wxr_react_toggle, (XPLMCommandCallback_f)copilot_wxr_react_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_react_on, (XPLMCommandCallback_f)copilot_wxr_react_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_react_off, (XPLMCommandCallback_f)copilot_wxr_react_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(copilot_wxr_narrow_toggle, (XPLMCommandCallback_f)copilot_wxr_narrow_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_narrow_on, (XPLMCommandCallback_f)copilot_wxr_narrow_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_narrow_off, (XPLMCommandCallback_f)copilot_wxr_narrow_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(copilot_wxr_alert_toggle, (XPLMCommandCallback_f)copilot_wxr_alert_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_alert_on, (XPLMCommandCallback_f)copilot_wxr_alert_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_alert_off, (XPLMCommandCallback_f)copilot_wxr_alert_handler, 1, (void *)OFF);

    XPLMUnregisterCommandHandler(copilot_wxr_target_toggle, (XPLMCommandCallback_f)copilot_wxr_target_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_wxr_target_on, (XPLMCommandCallback_f)copilot_wxr_target_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_wxr_target_off, (XPLMCommandCallback_f)copilot_wxr_target_handler, 1, (void *)OFF);


    // xhsi/pfd_copilot/...
    // copilot mins mode
    XPLMUnregisterCommandHandler(copilot_mins_toggle, (XPLMCommandCallback_f)copilot_mins_mode_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_mins_radio, (XPLMCommandCallback_f)copilot_mins_mode_handler, 1, (void *)MINS_RADIO);
    XPLMUnregisterCommandHandler(copilot_mins_baro, (XPLMCommandCallback_f)copilot_mins_mode_handler, 1, (void *)MINS_BARO);
    // copilot mins value
    XPLMUnregisterCommandHandler(copilot_mins_reset, (XPLMCommandCallback_f)copilot_mins_value_handler, 1, (void *)OFF);
    XPLMUnregisterCommandHandler(copilot_mins_down, (XPLMCommandCallback_f)copilot_mins_value_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(copilot_mins_up, (XPLMCommandCallback_f)copilot_mins_value_handler, 1, (void *)UP);

    // copilot Instrument Landing System (PFD & ND)
    XPLMUnregisterCommandHandler(copilot_ils_toggle, (XPLMCommandCallback_f)copilot_ils_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_ils_on, (XPLMCommandCallback_f)copilot_ils_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_ils_off, (XPLMCommandCallback_f)copilot_ils_handler, 1, (void *)OFF);

    // copilot Metric altitude on PFD/ND
    XPLMUnregisterCommandHandler(copilot_metric_alt_toggle, (XPLMCommandCallback_f)copilot_metric_alt_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(copilot_metric_alt_on, (XPLMCommandCallback_f)copilot_metric_alt_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(copilot_metric_alt_off, (XPLMCommandCallback_f)copilot_metric_alt_handler, 1, (void *)OFF);

    // copilot Track FPA / FPV / Bird : on display FPA/FPV/Bird , off display flight director
    XPLMUnregisterCommandHandler(track_fpa_toggle, (XPLMCommandCallback_f)copilot_track_fpa_handler, 1, (void *)TOGGLE);
    XPLMUnregisterCommandHandler(track_fpa_on, (XPLMCommandCallback_f)copilot_track_fpa_handler, 1, (void *)ON);
    XPLMUnregisterCommandHandler(track_fpa_off, (XPLMCommandCallback_f)copilot_track_fpa_handler, 1, (void *)OFF);

    // MFD mode
    XPLMUnregisterCommandHandler(mfd_mode_arpt, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_ARPT);
    XPLMUnregisterCommandHandler(mfd_mode_fpln, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_FPLN);
    XPLMUnregisterCommandHandler(mfd_mode_rtu, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_RTU);
    XPLMUnregisterCommandHandler(mfd_mode_eicas, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_EICAS);
    XPLMUnregisterCommandHandler(mfd_mode_bleed, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_BLEED);
    XPLMUnregisterCommandHandler(mfd_mode_cab_press, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_CAB_PRESS);
    XPLMUnregisterCommandHandler(mfd_mode_elec, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_ELEC);
    XPLMUnregisterCommandHandler(mfd_mode_hydr, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_HYDR);
    XPLMUnregisterCommandHandler(mfd_mode_fuel, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_FUEL);
    XPLMUnregisterCommandHandler(mfd_mode_apu, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_APU);
    XPLMUnregisterCommandHandler(mfd_mode_cond, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_COND);
    XPLMUnregisterCommandHandler(mfd_mode_door_oxy, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_DOOR_OXY);
    XPLMUnregisterCommandHandler(mfd_mode_wheel, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_WHEELS);
    XPLMUnregisterCommandHandler(mfd_mode_fctl, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_FCTL);
    XPLMUnregisterCommandHandler(mfd_mode_sys, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_SYS);
    XPLMUnregisterCommandHandler(mfd_mode_status, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_STATUS);
    XPLMUnregisterCommandHandler(mfd_mode_debug, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_DEBUG);
    XPLMUnregisterCommandHandler(mfd_mode_down, (XPLMCommandCallback_f)mfd_handler, 1, (void *)DOWN);
    XPLMUnregisterCommandHandler(mfd_mode_up, (XPLMCommandCallback_f)mfd_handler, 1, (void *)UP);
    XPLMUnregisterCommandHandler(mfd_mode_cycle, (XPLMCommandCallback_f)mfd_handler, 1, (void *)CYCLE);
    XPLMUnregisterCommandHandler(mfd_reset_fuel_used, (XPLMCommandCallback_f)mfd_handler, 1, (void *)MFD_RESET_FUEL_USED);

    // CDU Mode
    XPLMUnregisterCommandHandler(cdu_pilot_custom, (XPLMCommandCallback_f)cdu_handler, 1, (void *)CDU_PILOT_CUSTOM);
    XPLMUnregisterCommandHandler(cdu_pilot_xfmc, (XPLMCommandCallback_f)cdu_handler, 1, (void *)CDU_PILOT_XFMC);
    XPLMUnregisterCommandHandler(cdu_copilot_custom, (XPLMCommandCallback_f)cdu_handler, 1, (void *)CDU_COPILOT_CUSTOM);
    XPLMUnregisterCommandHandler(cdu_copilot_xfmc, (XPLMCommandCallback_f)cdu_handler, 1, (void *)CDU_COPILOT_XFMC);

    // nav1_sync
    XPLMUnregisterCommandHandler(nav1_sync, (XPLMCommandCallback_f)nav_sync_handler, 1, (void *)1);
    // nav2_sync
    XPLMUnregisterCommandHandler(nav2_sync, (XPLMCommandCallback_f)nav_sync_handler, 1, (void *)2);


    // range_auto
    XPLMUnregisterCommandHandler(auto_range_pilot, (XPLMCommandCallback_f)auto_range_handler, 1, (void *)EFIS_PILOT);
    XPLMUnregisterCommandHandler(auto_range_copilot, (XPLMCommandCallback_f)auto_range_handler, 1, (void *)EFIS_COPILOT);

    // ext_range_auto
    XPLMUnregisterCommandHandler(auto_ext_range_pilot, (XPLMCommandCallback_f)auto_ext_range_handler, 1, (void *)EFIS_PILOT);
    XPLMUnregisterCommandHandler(auto_ext_range_copilot, (XPLMCommandCallback_f)auto_ext_range_handler, 1, (void *)EFIS_COPILOT);


    // Main chronometer
    XPLMUnregisterCommandHandler(chr_start_stop_reset, (XPLMCommandCallback_f)clock_handler, 1, (void *)0);
    XPLMUnregisterCommandHandler(chr_start_stop, (XPLMCommandCallback_f)clock_handler, 1, (void *)1);
    XPLMUnregisterCommandHandler(chr_reset, (XPLMCommandCallback_f)clock_handler, 1, (void *)2);

    // ND Chronometers
    XPLMUnregisterCommandHandler(pilot_chrono_stop_reset, (XPLMCommandCallback_f)chrono_handler, 1, (void *)0);
    XPLMUnregisterCommandHandler(pilot_chrono_start_stop, (XPLMCommandCallback_f)chrono_handler, 1, (void *)1);
    XPLMUnregisterCommandHandler(pilot_chrono_reset, (XPLMCommandCallback_f)clock_handler, 1, (void *)2);
    XPLMUnregisterCommandHandler(copilot_chrono_stop_reset, (XPLMCommandCallback_f)chrono_handler, 1, (void *)3);
    XPLMUnregisterCommandHandler(copilot_chrono_start_stop, (XPLMCommandCallback_f)chrono_handler, 1, (void *)4);
    XPLMUnregisterCommandHandler(copilot_chrono_reset, (XPLMCommandCallback_f)clock_handler, 1, (void *)5);

//    // cancel the interception of a standard X-Plane command
//    XPLMUnregisterCommandHandler(contact_atc_cmd, (XPLMCommandCallback_f)contact_atc_handler, 1, (void *)0);

    // RTU select
    XPLMUnregisterCommandHandler(rtu_select_none, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_NONE);
    XPLMUnregisterCommandHandler(rtu_select_com1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_COM1);
    XPLMUnregisterCommandHandler(rtu_select_nav1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_NAV1);
    XPLMUnregisterCommandHandler(rtu_select_adf1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_ADF1);
    XPLMUnregisterCommandHandler(rtu_select_xpdr, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_XPDR);
    XPLMUnregisterCommandHandler(rtu_select_tcas, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_TCAS);
    XPLMUnregisterCommandHandler(rtu_select_adf2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_ADF2);
    XPLMUnregisterCommandHandler(rtu_select_nav2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_NAV2);
    XPLMUnregisterCommandHandler(rtu_select_com2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_COM2);
    // RTU select/flip
    XPLMUnregisterCommandHandler(rtu_select_flip_com1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_COM1);
    XPLMUnregisterCommandHandler(rtu_select_flip_nav1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_NAV1);
    XPLMUnregisterCommandHandler(rtu_select_flip_adf1, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_ADF1);
    XPLMUnregisterCommandHandler(rtu_select_flip_xpdr, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_XPDR);
    XPLMUnregisterCommandHandler(rtu_select_flip_tcas, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_TCAS);
    XPLMUnregisterCommandHandler(rtu_select_flip_adf2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_ADF2);
    XPLMUnregisterCommandHandler(rtu_select_flip_nav2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_NAV2);
    XPLMUnregisterCommandHandler(rtu_select_flip_com2, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_FLIP_COM2);
    // RTU next selection RTU_SELECT_NEXT
    XPLMUnregisterCommandHandler(rtu_select_next, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_NEXT);
    // RTU previous selection RTU_SELECT_PREV
    XPLMUnregisterCommandHandler(rtu_select_prev, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_SELECT_PREV);
    // RTU flip selected
    XPLMUnregisterCommandHandler(rtu_flip_selected, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_FLIP_SELECTED);
    // RTU tuning
    XPLMUnregisterCommandHandler(rtu_coarse_down, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_COARSE_DOWN);
    XPLMUnregisterCommandHandler(rtu_coarse_up, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_COARSE_UP);
    XPLMUnregisterCommandHandler(rtu_fine_down, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_FINE_DOWN);
    XPLMUnregisterCommandHandler(rtu_fine_up, (XPLMCommandCallback_f)rtu_handler, 1, (void *)RTU_FINE_UP);
    
    // XPDR mode
    XPLMUnregisterCommandHandler(xpdr_mode_up, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_MODE_UP);
    XPLMUnregisterCommandHandler(xpdr_mode_down, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_MODE_DOWN);
    XPLMUnregisterCommandHandler(xpdr_mode_cycle, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_MODE_CYCLE);
    // XPDR code
    XPLMUnregisterCommandHandler(xpdr_code_hundreds_up, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_CODE_HUNDREDS_UP);
    XPLMUnregisterCommandHandler(xpdr_code_hundreds_down, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_CODE_HUNDREDS_DOWN);
    XPLMUnregisterCommandHandler(xpdr_code_units_up, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_CODE_UNITS_UP);
    XPLMUnregisterCommandHandler(xpdr_code_units_down, (XPLMCommandCallback_f)xpdr_handler, 1, (void *)XPDR_CODE_UNITS_DOWN);
    
    // ADF tuning
    // ADF1
    XPLMUnregisterCommandHandler(adf1_hundreds_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1 + ADF_HUNDREDS_UP));
    XPLMUnregisterCommandHandler(adf1_hundreds_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1 + ADF_HUNDREDS_DOWN));
    XPLMUnregisterCommandHandler(adf1_units_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1 + ADF_UNITS_UP));
    XPLMUnregisterCommandHandler(adf1_units_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1 + ADF_UNITS_DOWN));
    // ADF1 standby
    XPLMUnregisterCommandHandler(adf1_stby_hundreds_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1_STBY + ADF_HUNDREDS_UP));
    XPLMUnregisterCommandHandler(adf1_stby_hundreds_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1_STBY + ADF_HUNDREDS_DOWN));
    XPLMUnregisterCommandHandler(adf1_stby_units_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1_STBY + ADF_UNITS_UP));
    XPLMUnregisterCommandHandler(adf1_stby_units_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF1_STBY + ADF_UNITS_DOWN));
    // ADF2
    XPLMUnregisterCommandHandler(adf2_hundreds_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2 + ADF_HUNDREDS_UP));
    XPLMUnregisterCommandHandler(adf2_hundreds_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2 + ADF_HUNDREDS_DOWN));
    XPLMUnregisterCommandHandler(adf2_units_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2 + ADF_UNITS_UP));
    XPLMUnregisterCommandHandler(adf2_units_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2 + ADF_UNITS_DOWN));
    // ADF2 standby
    XPLMUnregisterCommandHandler(adf2_stby_hundreds_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2_STBY + ADF_HUNDREDS_UP));
    XPLMUnregisterCommandHandler(adf2_stby_hundreds_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2_STBY + ADF_HUNDREDS_DOWN));
    XPLMUnregisterCommandHandler(adf2_stby_units_up, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2_STBY + ADF_UNITS_UP));
    XPLMUnregisterCommandHandler(adf2_stby_units_down, (XPLMCommandCallback_f)adf_handler, 1, (void *)(ADF2_STBY + ADF_UNITS_DOWN));
    

    XPLMDebugString("XHSI: custom command handlers unregistered\n");

}
