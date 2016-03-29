
// Sim value ids ===============================================================

// Aircraft position
#define SIM_FLIGHTMODEL_POSITION_GROUNDSPEED 0
#define SIM_FLIGHTMODEL_POSITION_TRUE_AIRSPEED 1
#define SIM_FLIGHTMODEL_POSITION_MAGPSI 2
#define SIM_FLIGHTMODEL_POSITION_HPATH 3
#define SIM_FLIGHTMODEL_POSITION_LATITUDE 4		// double!
#define SIM_FLIGHTMODEL_POSITION_LONGITUDE 5 	// double!
#define SIM_FLIGHTMODEL_POSITION_PHI 6 			// roll angle
#define SIM_FLIGHTMODEL_POSITION_R 7 			// rotation rate
#define SIM_FLIGHTMODEL_POSITION_MAGVAR 8
#define SIM_FLIGHTMODEL_POSITION_ELEVATION 9
#define SIM_FLIGHTMODEL_POSITION_Y_AGL 10
#define SIM_FLIGHTMODEL_POSITION_THETA 11       // pitch
#define SIM_FLIGHTMODEL_POSITION_VPATH 12       // fpa
#define SIM_FLIGHTMODEL_POSITION_ALPHA 13       // aoa
#define SIM_FLIGHTMODEL_POSITION_BETA 14        // yaw ( = slip or drift ? )
#define SIM_FLIGHTMODEL_FAILURES_ONGROUND_ANY 15 // It was misplaced and is not really a failure, you can use that to indicate when the wheels are on the ground
#define XHSI_FLIGHTMODEL_POSITION_NEAREST_ARPT 10016 // Special case : 4 char
#define SIM_FLIGHTMODEL_FORCES_G_LOAD 17
#define SIM_FLIGHTMODEL_MISC_TURNRATE_NOROLL 18 // I don't know in which units this is, but a standard rate turn seems to be 30.0f


// Instruments
//#define SIM_FLIGHTMODEL_POSITION_VH_IND_FPM 50
//#define SIM_FLIGHTMODEL_MISC_H_IND 51
#define SIM_COCKPIT2_GAUGES_INDICATORS_AIRSPEED_KTS_PILOT 52
#define SIM_COCKPIT2_GAUGES_INDICATORS_AIRSPEED_KTS_COPILOT 53
#define SIM_COCKPIT2_GAUGES_INDICATORS_ALTITUDE_FT_PILOT 54
#define SIM_COCKPIT2_GAUGES_INDICATORS_ALTITUDE_FT_COPILOT 55
#define SIM_COCKPIT2_GAUGES_INDICATORS_VVI_FPM_PILOT 56
#define SIM_COCKPIT2_GAUGES_INDICATORS_VVI_FPM_COPILOT 57
#define SIM_COCKPIT2_GAUGES_INDICATORS_SIDESLIP_DEGREES 58
#define SIM_COCKPIT2_GAUGES_ACTUATORS_RADIO_ALTIMETER_BUG_FT_PILOT 59
#define SIM_COCKPIT2_GAUGES_ACTUATORS_RADIO_ALTIMETER_BUG_FT_COPILOT 60
#define SIM_COCKPIT2_GAUGES_ACTUATORS_BAROMETER_SETTING_IN_HG_PILOT 61
#define SIM_COCKPIT2_GAUGES_ACTUATORS_BAROMETER_SETTING_IN_HG_COPILOT 62
#define SIM_COCKPIT2_GAUGES_INDICATORS_AIRSPEED_ACCELERATION 63
#define XHSI_EFIS_PILOT_DA_BUG 64
#define XHSI_EFIS_PILOT_MINS_MODE 65
#define XHSI_EFIS_COPILOT_DA_BUG 66
#define XHSI_EFIS_COPILOT_MINS_MODE 67
#define SIM_GAUGES_FAILURES_PILOT 68
#define SIM_GAUGES_FAILURES_COPILOT 69

// Electrical
#define SIM_COCKPIT_ELECTRICAL_AVIONICS_ON 80
#define SIM_COCKPIT_ELECTRICAL_BATTERY_ON 81
#define SIM_COCKPIT_ELECTRICAL_COCKPIT_LIGHTS_ON 82
#define APU_N1 83
#define APU_GEN_AMP 84
#define GPU_GEN_AMP 85
#define AUX_GEN_STATUS 86
// Battery array [4]
#define SIM_COCKPIT_ELECTRICAL_BATTERY_VOLT_ 87
#define SIM_COCKPIT_ELECTRICAL_BATTERY_AMP_ 91

// Radios
#define SIM_COCKPIT_RADIOS_NAV1_FREQ_HZ 100
#define SIM_COCKPIT_RADIOS_NAV2_FREQ_HZ 101
#define SIM_COCKPIT_RADIOS_ADF1_FREQ_HZ 102
#define SIM_COCKPIT_RADIOS_ADF2_FREQ_HZ 103
#define SIM_COCKPIT_RADIOS_NAV1_DIR_DEGT 104
#define SIM_COCKPIT_RADIOS_NAV2_DIR_DEGT 105
#define SIM_COCKPIT_RADIOS_ADF1_DIR_DEGT 106
#define SIM_COCKPIT_RADIOS_ADF2_DIR_DEGT 107
#define SIM_COCKPIT_RADIOS_NAV1_DME_DIST_M 108
#define SIM_COCKPIT_RADIOS_NAV2_DME_DIST_M 109
#define SIM_COCKPIT_RADIOS_ADF1_DME_DIST_M 110
#define SIM_COCKPIT_RADIOS_ADF2_DME_DIST_M 111
#define SIM_COCKPIT_RADIOS_NAV1_OBS_DEGM 112
#define SIM_COCKPIT_RADIOS_NAV2_OBS_DEGM 113
#define SIM_COCKPIT_RADIOS_NAV1_COURSE_DEGM 114
#define SIM_COCKPIT_RADIOS_NAV2_COURSE_DEGM 115
#define SIM_COCKPIT_RADIOS_NAV1_CDI 116
#define SIM_COCKPIT_RADIOS_NAV2_CDI 117
#define SIM_COCKPIT_RADIOS_NAV1_HDEF_DOT 118
#define SIM_COCKPIT_RADIOS_NAV2_HDEF_DOT 119
#define SIM_COCKPIT_RADIOS_NAV1_FROMTO 120
#define SIM_COCKPIT_RADIOS_NAV2_FROMTO 121
#define SIM_COCKPIT_RADIOS_NAV1_VDEF_DOT 122
#define SIM_COCKPIT_RADIOS_NAV2_VDEF_DOT 123

#define SIM_COCKPIT_RADIOS_GPS_DIR_DEGT 124
#define SIM_COCKPIT_RADIOS_GPS_DME_DIST_M 125
#define SIM_COCKPIT_RADIOS_GPS_COURSE_DEGTM 126
#define SIM_COCKPIT_RADIOS_GPS_HDEF_DOT 127
#define SIM_COCKPIT_RADIOS_GPS_FROMTO 128
#define SIM_COCKPIT_RADIOS_GPS_VDEF_DOT 129

#define SIM_COCKPIT_RADIOS_NAV1_DME_TIME_SECS 130
#define SIM_COCKPIT_RADIOS_NAV2_DME_TIME_SECS 131
#define SIM_COCKPIT_RADIOS_GPS_DME_TIME_SECS 132

#define SIM_COCKPIT2_RADIOS_INDICATORS_OUTER_MARKER_LIT 133
#define SIM_COCKPIT2_RADIOS_INDICATORS_MIDDLE_MARKER_LIT 134
#define SIM_COCKPIT2_RADIOS_INDICATORS_INNER_MARKER_LIT 135

#define SIM_COCKPIT_RADIOS_NAV1_STDBY_FREQ_HZ 136
#define SIM_COCKPIT_RADIOS_NAV2_STDBY_FREQ_HZ 137
#define SIM_COCKPIT_RADIOS_ADF1_STDBY_FREQ_HZ 138
#define SIM_COCKPIT_RADIOS_ADF2_STDBY_FREQ_HZ 139

#define SIM_COCKPIT2_RADIOS_INDICATORS_NAV1_NAV_ID 10140
#define SIM_COCKPIT2_RADIOS_INDICATORS_NAV2_NAV_ID 10141
#define SIM_COCKPIT2_RADIOS_INDICATORS_ADF1_NAV_ID 10142
#define SIM_COCKPIT2_RADIOS_INDICATORS_ADF2_NAV_ID 10143

#define SIM_COCKPIT_RADIOS_COM1_FREQ_HZ 144
#define SIM_COCKPIT_RADIOS_COM1_STDBY_FREQ_HZ 145
#define SIM_COCKPIT_RADIOS_COM2_FREQ_HZ 146
#define SIM_COCKPIT_RADIOS_COM2_STDBY_FREQ_HZ 147
#define SIM_COCKPIT_RADIOS_STDBY_FLIP 148 // for incoming commands only

#define SIM_COCKPIT_RADIOS_GPS_HAS_GLIDESLOPE 149

// for the 8.33KHz spacing, see 194-197 below


// AP
#define SIM_COCKPIT_AUTOPILOT_AUTOPILOT_STATE 150
#define SIM_COCKPIT_AUTOPILOT_VERTICAL_VELOCITY 151
#define SIM_COCKPIT_AUTOPILOT_ALTITUDE 152
#define SIM_COCKPIT_AUTOPILOT_APPROACH_SELECTOR 153
#define SIM_COCKPIT_AUTOPILOT_HEADING_MAG 154
#define SIM_COCKPIT_AUTOPILOT_AIRSPEED 155
#define SIM_COCKPIT_AUTOPILOT_AIRSPEED_IS_MACH 156
#define SIM_COCKPIT_AUTOPILOT_FD_PITCH 157
#define SIM_COCKPIT_AUTOPILOT_FD_ROLL 158
#define SIM_COCKPIT_AUTOPILOT_MODE 159
#define SIM_COCKPIT2_AUTOPILOT_AUTOTHROTTLE_ENABLED 160
#define SIM_COCKPIT2_AUTOPILOT_AUTOTHROTTLE_ON 161
#define SIM_COCKPIT2_AUTOPILOT_HEADING_STATUS 162
#define SIM_COCKPIT2_AUTOPILOT_NAV_STATUS 163
#define SIM_COCKPIT2_AUTOPILOT_VVI_STATUS 164
#define SIM_COCKPIT2_AUTOPILOT_SPEED_STATUS 165
#define SIM_COCKPIT2_AUTOPILOT_ALTITUDE_HOLD_STATUS 166
#define SIM_COCKPIT2_AUTOPILOT_GLIDESLOPE_STATUS 167
#define SIM_COCKPIT2_AUTOPILOT_VNAV_STATUS 168
#define SIM_COCKPIT2_AUTOPILOT_TOGA_STATUS 169
#define SIM_COCKPIT2_AUTOPILOT_TOGA_LATERAL_STATUS 170
#define SIM_COCKPIT2_AUTOPILOT_ROLL_STATUS 171
#define SIM_COCKPIT2_AUTOPILOT_PITCH_STATUS 172
#define SIM_COCKPIT2_AUTOPILOT_BACKCOURSE_STATUS 173
#define SIM_COCKPIT_AUTOPILOT_KEY_PRESS 174 // for incoming commands only
#define SIM_COCKPIT_AUTOPILOT_HEADING_ROLL_MODE 175

// Transponder
#define SIM_COCKPIT_RADIOS_TRANSPONDER_MODE 180
#define SIM_COCKPIT_RADIOS_TRANSPONDER_CODE 181
#define SIM_COCKPIT_RADIOS_TRANSPONDER_ID 182


// RTU
#define XHSI_RTU_CONTACT_ATC 185 // also works for PilotEdge
#define XHSI_RTU_SELECTED_RADIO 186 // 0:none, 1:COM1, 2:NAV1, 3:ADF1, 4:XPDR_CODE, 5:XPDR_MODE, 6:ADF2, 7:NAV2, 8:COM2
#define XHSI_RTU_PILOTEDGE_RX 187 // works only for PilotEdge


// Clock mode
#define SIM_COCKPIT2_CLOCK_TIMER_MODE 190
#define XHSI_CHRONOGRAPH_CONTROL 191 // for incoming commands only


// CDU source
#define XHSI_CDU_SOURCE 192


// COM1 & 2 using 8.33kHz spacing
#define SIM_COCKPIT2_RADIOS_ACTUATORS_COM1_FREQUENCY_HZ_833 194
#define SIM_COCKPIT2_RADIOS_ACTUATORS_COM1_STANDBY_FREQUENCY_HZ_833 195
#define SIM_COCKPIT2_RADIOS_ACTUATORS_COM2_FREQUENCY_HZ_833 196
#define SIM_COCKPIT2_RADIOS_ACTUATORS_COM2_STANDBY_FREQUENCY_HZ_833 197


// GPS next waypoint ID
#define SIM_COCKPIT2_RADIOS_INDICATORS_GPS_NAV_ID_0_3 10198 // the 4 first characters
#define SIM_COCKPIT2_RADIOS_INDICATORS_GPS_NAV_ID_4_7 10199 // the next 4 characters


// EFIS
#define SIM_COCKPIT_SWITCHES_HSI_SELECTOR 200
#define SIM_COCKPIT_SWITCHES_EFIS_MAP_RANGE_SELECTOR 201
#define SIM_COCKPIT_SWITCHES_EFIS_DME_1_SELECTOR 202
#define SIM_COCKPIT_SWITCHES_EFIS_DME_2_SELECTOR 203
#define SIM_COCKPIT_SWITCHES_EFIS_SHOWS_WEATHER 204
#define SIM_COCKPIT_SWITCHES_EFIS_SHOWS_TCAS 205
#define SIM_COCKPIT_SWITCHES_EFIS_SHOWS_AIRPORTS 206
#define SIM_COCKPIT_SWITCHES_EFIS_SHOWS_WAYPOINTS 207
#define SIM_COCKPIT_SWITCHES_EFIS_SHOWS_VORS 208
#define SIM_COCKPIT_SWITCHES_EFIS_SHOWS_NDBS 209
#define SIM_COCKPIT_SWITCHES_EFIS_MAP_MODE 210
#define SIM_COCKPIT_SWITCHES_EFIS_MAP_SUBMODE 211
#define XHSI_EFIS_PILOT_STA 212
#define XHSI_EFIS_PILOT_DATA 213
#define XHSI_EFIS_PILOT_POS 214
#define XHSI_EFIS_PILOT_MAP_ZOOMIN 215


// Copilot EFIS
#define XHSI_EFIS_COPILOT_HSI_SOURCE 250
#define XHSI_EFIS_COPILOT_MAP_RANGE 251
#define XHSI_EFIS_COPILOT_RADIO1 252
#define XHSI_EFIS_COPILOT_RADIO2 253
#define XHSI_EFIS_COPILOT_WXR 254
#define XHSI_EFIS_COPILOT_TFC 255
#define XHSI_EFIS_COPILOT_ARPT 256
#define XHSI_EFIS_COPILOT_WPT 257
#define XHSI_EFIS_COPILOT_VOR 258
#define XHSI_EFIS_COPILOT_NDB 259
#define XHSI_EFIS_COPILOT_MAP_CTR 260
#define XHSI_EFIS_COPILOT_MAP_MODE 261
#define XHSI_EFIS_COPILOT_STA 262
#define XHSI_EFIS_COPILOT_DATA 263
#define XHSI_EFIS_COPILOT_POS 264
#define XHSI_EFIS_COPILOT_MAP_ZOOMIN 265

// EICAS
#define XHSI_ENGINE_TYPE 280
#define XHSI_EICAS_TRQ_SCALE 281
#define XHSI_FUEL_UNITS 282

// MFD
#define XHSI_MFD_MODE 290
#define XHSI_CREW_OXY_PSI 291


// Environment
#define SIM_WEATHER_WIND_SPEED_KT 300
#define SIM_WEATHER_WIND_DIRECTION_DEGT 301
#define SIM_TIME_ZULU_TIME_SEC 302
#define SIM_TIME_LOCAL_TIME_SEC 303
#define SIM_WEATHER_TEMPERATURE_AMBIENT_C 304
#define SIM_WEATHER_SPEED_SOUND_MS 305
#define SIM_WEATHER_TEMPERATURE_SEALEVEL_C 309
#define SIM_WEATHER_TEMPERATURE_LE_C 310
// Timers
#define SIM_TIME_TIMER_IS_RUNNING_SEC 306
#define SIM_TIME_TIMER_ELAPSED_TIME_SEC 307
#define SIM_TIME_TOTAL_FLIGHT_TIME_SEC 308


// Aircraft constants
#define SIM_AIRCRAFT_VIEW_ACF_VSO 320
#define SIM_AIRCRAFT_VIEW_ACF_VS 321
#define SIM_AIRCRAFT_VIEW_ACF_VFE 322
#define SIM_AIRCRAFT_VIEW_ACF_VNO 323
#define SIM_AIRCRAFT_VIEW_ACF_VNE 324
#define SIM_AIRCRAFT_VIEW_ACF_MMO 325
#define SIM_AIRCRAFT_OVERFLOW_ACF_VLE 326
#define SIM_AIRCRAFT_PARTS_ACF_SBRKEQ 327
#define SIM_AIRCRAFT_GEAR_ACF_GEAR_RETRACT 328
#define SIM_AIRCRAFT_OVERFLOW_ACF_VMCA 329
#define SIM_AIRCRAFT_OVERFLOW_ACF_VYSE 330
#define SIM_AIRCRAFT_VIEW_ACF_TAILNUM_0_3 10331
#define SIM_AIRCRAFT_VIEW_ACF_TAILNUM_4_7 10332

#define SIM_AIRCRAFT_CONTROLS_ACL_ELEV_DN 333
#define SIM_AIRCRAFT_CONTROLS_ACL_ELEV_UP 334
#define SIM_AIRCRAFT_CONTROLS_ACL_AIL_UP 335
#define SIM_AIRCRAFT_CONTROLS_ACL_AIL_DN 336
#define SIM_AIRCRAFT_CONTROLS_ACL_RUDDER_LR 337

#define SIM_AIRCRAFT_ELECTRICAL_NUM_BATTERIES 338
#define SIM_AIRCRAFT_ELECTRICAL_NUM_BUSES 339
#define SIM_AIRCRAFT_ELECTRICAL_NUM_GENERATORS 340
#define SIM_AIRCRAFT_ELECTRICAL_NUM_INVERTERS 341

#define SIM_AIRCRAFT_ENGINE_RED_OIL_P 342
#define SIM_AIRCRAFT_ENGINE_RED_OIL_T 343
#define SIM_AIRCRAFT_ENGINE_RED_EPR 344

// Controls & annunciators
#define SIM_COCKPIT2_ANNUNCIATORS_MASTER_CAUTION 350
#define SIM_COCKPIT2_ANNUNCIATORS_MASTER_WARNING 351
#define SIM_COCKPIT2_CONTROLS_GEAR_HANDLE_DOWN 352
#define SIM_COCKPIT2_ANNUNCIATORS_GEAR_UNSAFE 353
#define XHSI_AIRCRAFT_GEAR_COUNT 354 // Calculated from sim/aircraft/parts/acf_gear_type int[10]
#define SIM_COCKPIT2_CONTROLS_PARKING_BRAKE_RATIO 355
#define SIM_COCKPIT_LIGHTS 356 // bit-fields: taxi_light_on/strobe_lights_on/nav_lights_on/landing_lights_on/beacon_lights_on
#define SIM_COCKPIT2_CONTROLS_FLAP_RATIO 357 // this is supposed to be the handle location
#define SIM_COCKPIT2_CONTROLS_FLAP_HANDLE_DEPLOY_RATIO 358 // this is supposed to be the indicator
#define SIM_AIRCRAFT_CONTROLS_ACF_FLAP_DETENTS 359
#define SIM_COCKPIT2_ANNUNCIATORS_AUTOPILOT_DISCONNECT 360
#define SIM_COCKPIT2_ANNUNCIATORS_FUEL_QUANTITY 361
#define SIM_COCKPIT2_ANNUNCIATORS_GPWS 362
#define SIM_COCKPIT2_ANNUNCIATORS_ICE 363
#define SIM_COCKPIT2_ANNUNCIATORS_PITOT_HEAT 364
#define SIM_COCKPIT2_ANNUNCIATORS_STALL_WARNING 365
#define SIM_COCKPIT2_ANNUNCIATORS_GEAR_WARNING 366
#define SIM_COCKPIT2_SWITCHES_AUTO_BRAKE_LEVEL 367
#define SIM_COCKPIT2_CONTROLS_SPEEDBRAKE_RATIO 368
#define SIM_FLIGHTMODEL2_CONTROLS_SPEEDBRAKE_RATIO 369
#define SIM_FLIGHTMODEL2_GEAR_DEPLOY_RATIO_ 370 // array of 10 floats
#define XHSI_AIRCRAFT_GEAR_DOOR_DEPLOY_RATIO_ 380 // array of 10 floats
#define SIM_COCKPIT2_CONTROLS_YOKE_PITCH_RATIO 390
#define SIM_COCKPIT2_CONTROLS_YOKE_ROLL_RATIO 391
#define SIM_COCKPIT2_CONTROLS_YOKE_HDG_RATIO 396
#define SIM_COCKPIT2_CONTROLS_ELEVATOR_TRIM 392
#define SIM_COCKPIT2_CONTROLS_AILERON_TRIM 393
#define SIM_COCKPIT2_CONTROLS_RUDDER_TRIM 394
#define SIM_FLIGHTMODEL_CONTROLS_SLATRAT 395
#define SIM_COCKPIT2_CONTROLS_LEFT_BRK_RATIO 397
#define SIM_COCKPIT2_CONTROLS_RIGHT_BRK_RATIO 398
#define XHSI_AIRCRAFT_GEAR_DOOR_COUNT 399 // Calculated from sim/aircraft/gear/acf_gear_door_type int[20]

// Fuel, engines, etc...
// ids that end with an underscore represent arrays of up to 8 floats
#define SIM_AIRCRAFT_OVERFLOW_ACF_NUM_TANKS 400
#define SIM_AIRCRAFT_ENGINE_ACF_NUM_ENGINES 401
#define SIM_COCKPIT2_ANNUNCIATORS_REVERSER_DEPLOYED 402
#define SIM_COCKPIT2_ANNUNCIATORS_OIL_PRESSURE 403
#define SIM_COCKPIT2_ANNUNCIATORS_OIL_TEMPERATURE 404
#define SIM_COCKPIT2_ANNUNCIATORS_FUEL_PRESSURE 405
#define SIM_FLIGHTMODEL_WEIGHT_M_FUEL_TOTAL 406
#define SIM_AIRCRAFT_WEIGHT_ACF_M_FUEL_TOT 407
#define SIM_FLIGHTMODEL_WEIGHT_M_TOTAL 408
#define SIM_COCKPIT2_ENGINE_FIRE_EXTINGUISHER 409
#define SIM_COCKPIT2_FUEL_QUANTITY_ 410
#define SIM_FLIGHTMODEL_ENGINE_ENGN_N1_ 420
#define SIM_FLIGHTMODEL_ENGINE_ENGN_EGT_ 430
#define SIM_FLIGHTMODEL_ENGINE_ENGN_EGT_C_ 440
#define SIM_FLIGHTMODEL2_ENGINES_THRUST_REVERSER_DEPLOY_RATIO_ 450
#define SIM_AIRCRAFT_OVERFLOW_ACF_TANK_RATIO_ 460
#define SIM_FLIGHTMODEL_ENGINE_ENGN_N2_ 470
#define SIM_FLIGHTMODEL_ENGINE_ENGN_FF_ 480
#define SIM_FLIGHTMODEL_ENGINE_ENGN_OIL_PRESS_ 490
#define SIM_FLIGHTMODEL_ENGINE_ENGN_OIL_TEMP_ 500
#define SIM_COCKPIT2_ENGINE_INDICATORS_OIL_QUANTITY_RATIO_ 510
#define SIM_FLIGHTMODEL_ENGINE_ENGN_OIL_TEMP_C_ 520
#define SIM_FLIGHTMODEL_ENGINE_ENGN_OIL_PRESS_PSI_ 530
#define XHSI_FLIGHTMODEL_ENGINE_VIB_ 540
#define SIM_OPERATION_FAILURES_HYDRAULIC_PRESSURE_RATIO1 551
#define SIM_OPERATION_FAILURES_HYDRAULIC_PRESSURE_RATIO2 552
#define SIM_COCKPIT2_HYDRAULICS_INDICATORS_HYDRAULIC_FLUID_RATIO_1 553
#define SIM_COCKPIT2_HYDRAULICS_INDICATORS_HYDRAULIC_FLUID_RATIO_2 554
#define SIM_COCKPIT_FUEL_PUMPS 555
#define SIM_AIRCRAFT_ENGINE_MAX_EGT 556
// 557 : free
#define XHSI_EICAS_OVERRIDE_TRQ_MAX 558
#define SIM_AIRCRAFT_CONTROLS_ACF_TRQ_MAX_ENG 559
#define SIM_FLIGHTMODEL_ENGINE_ENGN_TRQ_ 560
#define SIM_FLIGHTMODEL_ENGINE_ENGN_ITT_ 570
#define SIM_FLIGHTMODEL_ENGINE_ENGN_ITT_C_ 580
#define SIM_AIRCRAFT_CONTROLS_ACF_RSC_REDLINE_PRP 589
#define SIM_COCKPIT2_ENGINE_INDICATORS_PROP_SPEED_RPM_ 590
#define SIM_FLIGHTMODEL_ENGINE_ENGN_PROPMODE_ 600
#define SIM_FLIGHTMODEL_ENGINE_ENGN_MPR_ 610
#define SIM_FLIGHTMODEL_ENGINE_ENGN_EPR_ 620
#define SIM_COCKPIT2_ENGINE_ACTUATORS_THROTTLE_RATIO_ 630
#define XHSI_FUEL_USED_ 640
// Control surface positions : rudder, elevators, ailerons, spoilers
#define SIM_FLIGHTMODEL_CONTROLS_RUDDER 650
#define SIM_FLIGHTMODEL_CONTROLS_LEFT_ELEV 651
#define SIM_FLIGHTMODEL_CONTROLS_RIGHT_ELEV 652
#define SIM_FLIGHTMODEL_CONTROLS_LEFT_AIL 653
#define SIM_FLIGHTMODEL_CONTROLS_RIGHT_AIL 654
// Cabin Pressure
#define SIM_COCKPIT2_PRESSURIZATION_CABIN_DELTA_P 655
#define SIM_COCKPIT2_PRESSURIZATION_CABIN_ALT 656
#define SIM_COCKPIT2_PRESSURIZATION_CABIN_VVI 657
// Brakes, wheels, steering
#define WHEEL_STATUS 658
#define TIRE_STATUS 659
// Generators and elec buses
#define SIM_COCKPIT_ELECTRICAL_GENERATOR_AMPS_ 660
#define SIM_COCKPIT_ELECTRICAL_GENERATOR_STATUS 668
#define SIM_COCKPIT_ELECTRICAL_INV_BUS_STATUS 669
#define SIM_COCKPIT_ELECTRICAL_BUS_LOAD_ 670

// UFMC
#define UFMC_STATUS 700
// UFMC REF
#define UFMC_V1 701
#define UFMC_VR 702
#define UFMC_V2 703
#define UFMC_VREF 704 // superceded by UFMC_F30 and UFMC_F40
#define UFMC_VF30 705
#define UFMC_VF40 706
// UFMC N1
#define UFMC_N1_1 707
#define UFMC_N1_2 708
#define UFMC_N1_3 709
#define UFMC_N1_4 710


//XFMC
#define XFMC_KEYPATH 750


// X737
#define X737_STATUS 800
// X737 AP
#define X737_AFDS_FD_A 801
#define X737_AFDS_FD_B 802
#define X737_AFDS_CMD_A 803
#define X737_AFDS_CMD_B 804
#define X737_PFD_MCPSPD 805
#define X737_PFD_FMCSPD 806
#define X737_PFD_RETARD 807
#define X737_PFD_THRHLD 808
#define X737_PFD_LNAVARMED 809
#define X737_PFD_VORLOCARMED 810
#define X737_PFD_PITCHSPD 811
#define X737_PFD_ALTHLD 812
#define X737_PFD_VSARMED 813
#define X737_PFD_VS 814
#define X737_PFD_VNAVALT 815
#define X737_PFD_VNAVPATH 816
#define X737_PFD_VNAVSPD 817
#define X737_PFD_GSARMED 818
#define X737_PFD_GS 819
#define X737_PFD_FLAREARMED 820
#define X737_PFD_FLARE 821
#define X737_PFD_TOGA 822
#define X737_PFD_LNAV 823
#define X737_PFD_HDG 824
#define X737_PFD_VORLOC 825
#define X737_PFD_N1 826
#define X737_AFDS_A_PITCH 827
#define X737_AFDS_B_PITCH 828
#define X737_AFDS_A_ROLL 829
#define X737_AFDS_B_ROLL 830
#define X737_ATHR_ARMED 831
// X737 N1
#define X737_N1_PHASE 832
#define X737_N1_LIMIT_ENG1 833
#define X737_N1_LIMIT_ENG2 834
#define X737_STBY_PWR 835
#define X737_PFD_PWR 836
#define X737_LVLCHANGE 837


// CL30
#define CL30_STATUS 850
// CL30 REF
#define CL30_V1 851
#define CL30_VR 852
#define CL30_V2 853
#define CL30_VT 854
#define CL30_VGA 855
#define CL30_VREF 856
#define CL30_REFSPDS 857
// CL30 ANNUN
#define CL30_MAST_WARN 858
#define CL30_MAST_CAUT 859
// CL30 Thrust Mode
#define CL30_CARETS 860
#define CL30_TO_N1 861
#define CL30_CLB_N1 862


// Settings & Preferences
// General instrument style
#define XHSI_STYLE 900 // 0:Boeing 1:Airbus
// Minimum runway length
#define XHSI_RWY_LENGTH_MIN 901 // Override with this value if it is > 0
#define XHSI_RWY_UNITS 902 // 0:Meters 1:Feet

	
	
// QPAC AirbusFBW
#define QPAC_STATUS 1000
// Autopilot
#define QPAC_AP_FD 1001
#define QPAC_AP_PHASE 1002
#define QPAC_PRESEL_CRZ 1003
#define QPAC_PRESEL_CLB 1004
#define QPAC_PRESEL_MACH 1005
#define QPAC_AP_VERTICAL_MODE 1006
#define QPAC_AP_VERTICAL_ARMED 1007
#define QPAC_AP_LATERAL_MODE 1008
#define QPAC_AP_LATERAL_ARMED 1009
#define QPAC_NPA_VALID 1010
#define QPAC_NPA_NO_POINTS 1011
#define QPAC_AP_APPR 1012
#define QPAC_APPR_TYPE 1013
#define QPAC_APPR_MDA 1014
#define QPAC_ALT_IS_CSTR 1015
#define QPAC_CONSTRAINT_ALT 1016
// FCU (bit field)
#define QPAC_FCU 1017
#define QPAC_FCU_BARO 1018
// Auto-Thrust
#define QPAC_ATHR_MODE 1019
#define QPAC_ATHR_MODE2 1020
#define QPAC_ATHR_LIMITED 1021
#define QPAC_THR_LEVER_MODE 1022
#define QPAC_FMA_THR_WARNING 1023
#define QPAC_FLEX_TEMP 1024
#define QPAC_THR_RATING_TYPE 1059
#define QPAC_THR_RATING_N1 1060
#define QPAC_THROTTLE_INPUT 1061
// ILS Sig and Deviation Capt. and FO
#define QPAC_ILS_FLAGS 1025
#define QPAC_LOC_VAL_CAPT 1026
#define QPAC_GS_VAL_CAPT 1027
#define QPAC_LOC_VAL_FO 1028
#define QPAC_GS_VAL_FO 1029
#define QPAC_ILS_CRS 1030
#define QPAC_ILS_FREQ 1031
#define QPAC_ILS_ID 11032
#define QPAC_ILS_DME 1033
// FD
#define QPAC_FD1_VER_BAR 1034
#define QPAC_FD1_HOR_BAR 1035
#define QPAC_FD1_YAW_BAR 1046
#define QPAC_FD2_VER_BAR 1037
#define QPAC_FD2_HOR_BAR 1038
#define QPAC_FD2_YAW_BAR 1039
// V Speeds
#define QPAC_V1_VALUE 1040
#define QPAC_V1 1041
#define QPAC_VR 1042
#define QPAC_VMO 1043
#define QPAC_VLS 1044
#define QPAC_VF 1045
#define QPAC_VS 1046
#define QPAC_V_GREEN_DOT 1047
#define QPAC_ALPHA_PROT 1048
#define QPAC_ALPHA_MAX 1049
//EFIS
#define QPAC_EFIS_ND_MODE_CAPT 1050
#define QPAC_EFIS_ND_RANGE_CAPT 1051
#define QPAC_EFIS_ND_MODE_FO 1052
#define QPAC_EFIS_ND_RANGE_FO 1053
// FAILURES
#define QPAC_FAILURES 1055
// BRAKES
#define QPAC_AUTO_BRAKE_LEVEL 1056
// FLAPS and SLATS
#define QPAC_FLAPS_REQ_POS 1057
#define QPAC_SLATS_REQ_POS 1058
// next value : 1062
// SPOILERS
#define QPAC_SPOILERS_LEFT 1062
#define QPAC_SPOILERS_RIGHT 1063
// ELAC and SEC FCC
#define QPAC_FCC 1064
// Rudder limit
#define QPAC_RUDDER_LIMIT 1065
// Hydraulic
#define QPAC_HYD_B_PRESS 1066
#define QPAC_HYD_G_PRESS 1067
#define QPAC_HYD_Y_PRESS 1068
#define QPAC_HYD_B_QTY 1069
#define QPAC_HYD_G_QTY 1070
#define QPAC_HYD_Y_QTY 1071
#define QPAC_HYD_PUMPS 1077
// Cabin Pressure
#define QPAC_CABIN_DELTA_P 1072
#define QPAC_CABIN_ALT 1073
#define QPAC_CABIN_VS 1074
#define QPAC_AIR_VALVES 1075
#define QPAC_OUTFLOW_VALVE 1090
// ECAM System Display page
#define QPAC_SD_PAGE 1076
// COND
#define QPAC_COND_COCKPIT_TRIM 1078
#define QPAC_COND_ZONE1_TRIM 1079
#define QPAC_COND_ZONE2_TRIM 1080
#define QPAC_COND_COCKPIT_TEMP 1100
#define QPAC_COND_AFT_CABIN_TEMP 1101
#define QPAC_COND_FWD_CABIN_TEMP 1102
#define QPAC_COND_AFT_CARGO_TEMP 1103
#define QPAC_COND_FWD_CARGO_TEMP 1104
// BLEED AIR
#define QPAC_BLEED_VALVES 1081
#define QPAC_PACK1_FLOW 1105
#define QPAC_PACK2_FLOW 1106
#define QPAC_PACK1_TEMP 1107
#define QPAC_PACK2_TEMP 1108
// APU
#define QPAC_APU_EGT 1082
// FUEL
#define QPAC_FUEL_PUMPS 1083
#define QPAC_FUEL_VALVES 1084
// ELEC
#define QPAC_ELEC_AC_CROSS 1085
#define QPAC_ELEC_CX_LEFT 1086
#define QPAC_ELEC_CX_CENTER 1087
#define QPAC_ELEC_CX_RIGHT 1088
#define QPAC_ELEC_BUTTONS 1099
// Keypress
#define QPAC_KEY_PRESS 1089
// next is 1091
// Triple pressure indicator
#define QPAC_TPI_LEFT 1091
#define QPAC_TPI_RIGHT 1092
#define QPAC_TPI_ACCU 1093
//
#define QPAC_NACELLE_TEMP_ 1094 // Array [4]
// Doors
#define QPAC_DOOR_STATUS 1098
// Next is 1109
// IDs reserved for QPAC up to 1199


// JarDesign Airbus A320neo (1200 - 1299)
#define JAR_A320NEO_STATUS 1200
// FMS
#define JAR_A320NEO_PRESEL_CRZ 1201
#define JAR_A320NEO_PRESEL_CLB 1202
#define JAR_A320NEO_PRESEL_MACH 1203
#define JAR_A320NEO_TRANS_ALT 1204
// Approach
#define JAR_A320NEO_APPR 1205
#define JAR_A320NEO_APPR_TYPE 1206
#define JAR_A320NEO_APPR_DH 1207
#define JAR_A320NEO_APPR_MDA 1208
#define JAR_A320NEO_FMA_CAT 1209
#define JAR_A320NEO_FMA_DUAL 1210
#define JAR_A320NEO_FMA_DH 1211
// FCU
#define JAR_A320NEO_FCU 1212
#define JAR_A320NEO_FCU_BARO 1213
// Auto-pilot
#define JAR_A320NEO_AP_PHASE 1214
#define JAR_A320NEO_AP_VERTICAL_MODE 1215
#define JAR_A320NEO_AP_VERTICAL_ARMED 1216
#define JAR_A320NEO_AP_LATERAL_MODE 1217
#define JAR_A320NEO_AP_LATERAL_ARMED 1218
#define JAR_A320NEO_AP_COMMON_MODE 1219
#define JAR_A320NEO_ALT_IS_CSTR 1220
#define JAR_A320NEO_CONSTRAINT_ALT 1221
// Auto-Thrust
#define JAR_A320NEO_ATHR_MODE 1222
#define JAR_A320NEO_THR_MODE 1223
#define JAR_A320NEO_ATHR_LIMITED 1224
#define JAR_A320NEO_THR_LEVER_MODE 1225
#define JAR_A320NEO_FMA_THR_WARNING 1226
#define JAR_A320NEO_FLEX_TEMP 1227
// ILS (1230 - 1239)
// V Speeds
#define JAR_A320NEO_V1 1240
#define JAR_A320NEO_VR 1242
#define JAR_A320NEO_VMO 1242
#define JAR_A320NEO_VLS 1243
#define JAR_A320NEO_VF 1244
#define JAR_A320NEO_VS 1245
#define JAR_A320NEO_V_GREEN_DOT 1246
#define JAR_A320NEO_ALPHA_PROT 1247
#define JAR_A320NEO_ALPHA_MAX 1248
// EFIS (1250-1254)
#define JAR_A320NEO_ND_MODE 1250
// FAILURES
#define JAR_A320NEO_FAILURES 1255
// BRAKES
#define JAR_A320NEO_AUTO_BRAKE_LEVEL 1256
#define JAR_A320NEO_BRAKE_ACCU_PSI 1257
#define JAR_A320NEO_BRAKE_LEFT_PSI 1258
#define JAR_A320NEO_BRAKE_RIGHT_PSI 1259
#define JAR_A320NEO_BRAKE_TEMP_ 1260
#define JAR_A320NEO_BRAKE_STATUS 1265
// MCDU Clics
#define JAR_A320NEO_MCDU_CLICK 1270
// Hydraulic
#define JAR_A320NEO_HYD_B_PRESS 1271
#define JAR_A320NEO_HYD_G_PRESS 1272
#define JAR_A320NEO_HYD_Y_PRESS 1273
#define JAR_A320NEO_HYD_B_QTY 1274
#define JAR_A320NEO_HYD_G_QTY 1275
#define JAR_A320NEO_HYD_Y_QTY 1276
#define JAR_A320NEO_HYD_PUMPS 1277
#define JAR_A320NEO_HYD_PTU   1278
// Doors
#define JAR_A320NEO_DOOR_STATUS 1279
// Plugin Version
#define PLUGIN_VERSION_ID 999

