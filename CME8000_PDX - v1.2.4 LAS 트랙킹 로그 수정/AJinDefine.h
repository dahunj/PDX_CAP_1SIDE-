///////////////////////////////////////////////////////////////////////////////
// AJinDefine.h : AJin Structure
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Motion Axis
const int AXIS_COUNT = 35;		// Motion Module

const int AX_TRAY_PICKER_X		= 0;
const int AX_TRAY_PICKER_Z		= 1;
const int AX_TRAY_PICKER_R		= 2;
const int AX_NO_AXIS			= 3;
const int AX_LOAD_STAGE1_X		= 4;
const int AX_LOAD_STAGE1_Z		= 5;
const int AX_LOAD_STAGE2_X		= 6;
const int AX_LOAD_STAGE2_Z		= 7;
const int AX_LOAD_PICKER_Y		= 8;
const int AX_LOAD_PICKER_Z		= 9;
const int AX_LOAD_PICKER_P		= 10;
const int AX_INDEX_R			= 11;
const int AX_VISION_CM_ALIGN_X	= 12;
const int AX_CAP_STAGE1_X		= 13;
const int AX_CAP_STAGE1_Z		= 14;
const int AX_CAP_STAGE2_X		= 15;
const int AX_CAP_STAGE2_Z		= 16;
const int AX_CAP_PICKER_Y		= 17;
const int AX_CAP_PICKER_Z		= 18;
const int AX_CAP_PICKER_P		= 19;
const int AX_CAP_BUFFER_STAGE_Y	= 20;
const int AX_VISION_CAP_ALIGN_Y	= 21;
const int AX_ASSY_PICKER_X		= 22;
const int AX_ASSY_PICKER_Y		= 23;
const int AX_ASSY_PICKER_Z		= 24;
const int AX_TRANS_STAGE_X		= 25;
const int AX_TRANS_STAGE_Z		= 26;
const int AX_TRANS_STAGE_T		= 27;
const int AX_UNLOAD_PICKER_X	= 28;
const int AX_UNLOAD_PICKER_Z	= 29;
const int AX_UNLOAD_PICKER_P	= 30;
const int AX_UNLOAD_STAGE1_Y	= 31;
const int AX_UNLOAD_STAGE1_Z	= 32;
const int AX_UNLOAD_STAGE2_Y	= 33;
const int AX_UNLOAD_STAGE2_Z	= 34;
///////////////////////////////////////////////////////////////////////////////
// Motion Status

typedef struct tag_AXIS_STATUS {
	double dPos;		// Motor Current Position
	double dVel;		// Motor Current Velocity
	BOOL bSOn;			// Servo On Status
	BOOL bOrg;			// Origin Sensor
	BOOL bELP;			// End Limit Positive
	BOOL bELN;			// End Limit Negative
	BOOL bALM;			// Driver Alarm
	BOOL bInP;			// In Position
	BOOL bRun;			// Running
	BOOL bHom;			// Home Complete
} AXIS_STATUS;

///////////////////////////////////////////////////////////////////////////////
// Motion Parameter

typedef struct tag_AXIS_PARAM {
	double dSpeedM;	// Move Speed
	double dSpeedJ;	// Jog Speed
	double dAccel;	// Move Accel
} AXIS_PARAM;

///////////////////////////////////////////////////////////////////////////////
// DIO Count

const long DIO_MODULE_COUNT = 14;		// IO Module (PCI-DI64R 2EA, PCI-DO64R 2EA)

///////////////////////////////////////////////////////////////////////////////
// 변환 타입

typedef union tag_DXY_DATA {
	DWORD nValue;
	struct {
		DWORD b0000: 1;		// 0000
		DWORD b0001: 1;		// 0001
		DWORD b0002: 1;		// 0002
		DWORD b0003: 1;		// 0003
		DWORD b0004: 1;		// 0004
		DWORD b0005: 1;		// 0005
		DWORD b0006: 1;		// 0006
		DWORD b0007: 1;		// 0007
		DWORD b0008: 1;		// 0008
		DWORD b0009: 1;		// 0009
		DWORD b0010: 1;		// 0010
		DWORD b0011: 1;		// 0011
		DWORD b0012: 1;		// 0012
		DWORD b0013: 1;		// 0013
		DWORD b0014: 1;		// 0014
		DWORD b0015: 1;		// 0015
		DWORD b0016: 1;		// 0016
		DWORD b0017: 1;		// 0017
		DWORD b0018: 1;		// 0018
		DWORD b0019: 1;		// 0019
		DWORD b0020: 1;		// 0020
		DWORD b0021: 1;		// 0021
		DWORD b0022: 1;		// 0022
		DWORD b0023: 1;		// 0023
		DWORD b0024: 1;		// 0024
		DWORD b0025: 1;		// 0025
		DWORD b0026: 1;		// 0026
		DWORD b0027: 1;		// 0027
		DWORD b0028: 1;		// 0028
		DWORD b0029: 1;		// 0029
		DWORD b0030: 1;		// 0030
		DWORD b0031: 1;		// 0031
	};
} DXY_DATA;	// Digital Input Output

///////////////////////////////////////////////////////////////////////////////
// Input (X0000 - X1031)
typedef union tag_DX_DATA_00 {
	DWORD nValue;
	struct {

		DWORD iTrayPickerGrip1Close	: 1;	// X0000
		DWORD iTrayPickerGrip1Open	: 1;	// X0001
		DWORD iTrayPickerGrip2Close	: 1;	// X0002
		DWORD iTrayPickerGrip2Open	: 1;	// X0003
		DWORD iTrayPickerGrip3Close	: 1;	// X0004
		DWORD iTrayPickerGrip3Open	: 1;	// X0005
		DWORD iTrayPickerGrip4Close	: 1;	// X0006
		DWORD iTrayPickerGrip4Open	: 1;	// X0007
		DWORD iTrayPickerExist		: 1;	// X0008
		DWORD i0009	: 1;					// X0009
		DWORD i0010	: 1;					// X0010
		DWORD i0011	: 1;					// X0011
		DWORD i0012	: 1;					// X0012
		DWORD i0013	: 1;					// X0013
		DWORD i0014	: 1;					// X0014
		DWORD i0015 : 1;					// X0015
		DWORD iLoadPort1SlideLock	: 1;	// X0016
		DWORD iLoadPort1SlideUnlock : 1;	// X0017
		DWORD iLoadPort1SlideClose	: 1;	// X0018
		DWORD iLoadPort1SlideOpen	: 1;	// X0019
		DWORD iLoadPort1LowCheck	: 1;	// X0020
		DWORD i0021 : 1;					// X0021
		DWORD i0022 : 1;					// X0022
		DWORD i0023 : 1;					// X0023
		DWORD iLoadPort1Support1In	: 1;	// X0024
		DWORD iLoadPort1Support1Out : 1;	// X0025
		DWORD iLoadPort1Support2In	: 1;	// X0026
		DWORD iLoadPort1Support2Out : 1;	// X0027
		DWORD i0028 : 1;					// X0028
		DWORD i0029 : 1;					// X0029
		DWORD i0030 : 1;					// X0030
		DWORD i0031 : 1;					// X0031
	};
} DX_DATA_00;	// Digital Input 00

typedef union tag_DX_DATA_01 {
	DWORD nValue;
	struct {
		DWORD iLoadPort2SlideLock	: 1;	// X0100
		DWORD iLoadPort2SlideUnlock	: 1;	// X0101
		DWORD iLoadPort2SlideClose	: 1;	// X0102
		DWORD iLoadPort2SlideOpen	: 1;	// X0103
		DWORD iLoadPort2LowCheck	: 1;	// X0104
		DWORD i0105 : 1;					// X0105
		DWORD i0106 : 1;					// X0106
		DWORD i0107 : 1;					// X0107
		DWORD iLoadPort2Support1In	: 1;	// X0108
		DWORD iLoadPort2Support1Out : 1;	// X0109
		DWORD iLoadPort2Support2In	: 1;	// X0110
		DWORD iLoadPort2Support2Out : 1;	// X0111
		DWORD i0112 : 1;					// X0112
		DWORD i0113 : 1;					// X0113
		DWORD i0114 : 1;					// X0114
		DWORD iLoadPortAreaCheck	: 1;	// X0115
		DWORD iLoadPort3SlideLock	: 1;	// X0116
		DWORD iLoadPort3SlideUnlock : 1;	// X0117
		DWORD iLoadPort3SlideClose	: 1;	// X0118
		DWORD iLoadPort3SlideOpen	: 1;	// X0119
		DWORD iLoadPort3LowCheck	: 1;	// X0120
		DWORD i0121 : 1;					// X0121
		DWORD iLoadPort3HighCheck	: 1;	// X0122
		DWORD i0123 : 1;					// X0123
		DWORD i0124 : 1;					// X0124
		DWORD i0125 : 1;					// X0125
		DWORD i0126 : 1;					// X0126
		DWORD i0127 : 1;					// X0127
		DWORD i0128 : 1;					// X0128
		DWORD i0129 : 1;					// X0129
		DWORD i0130 : 1;					// X0130
		DWORD iLoadPort3AreaCheck : 1;		// X0131
	};
} DX_DATA_01;	// Digital Input 01

typedef union tag_DX_DATA_02 {
	DWORD nValue;
	struct {
		DWORD iCapPort1SlideLock	: 1;	// X0200
		DWORD iCapPort1SlideUnlock	: 1;	// X0201
		DWORD iCapPort1SlideClose	: 1;	// X0202
		DWORD iCapPort1SlideOpen	: 1;	// X0203
		DWORD iCapPort1LowCheck		: 1;	// X0204
		DWORD i0205 : 1;					// X0205
		DWORD iCapPort1HighCheck : 1;		// X0206
		DWORD i0207 : 1;					// X0207
		DWORD iCapPort1Support1In	: 1;	// X0208
		DWORD iCapPort1Support1Out	: 1;	// X0209
		DWORD iCapPort1Support2In	: 1;	// X0210
		DWORD iCapPort1Support2Out	: 1;	// X0211
		DWORD i0212 : 1;					// X0212
		DWORD i0213 : 1;					// X0213
		DWORD i0214 : 1;					// X0214
		DWORD i0215 : 1;					// X0215
		DWORD iCapPort2SlideLock	: 1;	// X0216
		DWORD iCapPort2SlideUnlock	: 1;	// X0217
		DWORD iCapPort2SlideClose	: 1;	// X0218
		DWORD iCapPort2SlideOpen	: 1;	// X0219
		DWORD iCapPort2LowCheck		: 1;	// X0220
		DWORD i0221 : 1;					// X0221
		DWORD i0222 : 1;					// X0222
		DWORD i0223 : 1;					// X0223
		DWORD iCapPort2Support1In	: 1;	// X0224
		DWORD iCapPort2Support1Out	: 1;	// X0225
		DWORD iCapPort2Support2In	: 1;	// X0226
		DWORD iCapPort2Support2Out	: 1;	// X0227
		DWORD i0228 : 1;					// X0228
		DWORD i0229 : 1;					// X0229
		DWORD i0230 : 1;					// X0230
		DWORD iCapPortAreaCheck : 1;		// X0231
	};
} DX_DATA_02;	// Digital Input 02

typedef union tag_DX_DATA_03 {
	DWORD nValue;
	struct {
		DWORD iUnloadPort1SlideLock		: 1;	// X0300
		DWORD iUnloadPort1SlideUnlock	: 1;	// X0301
		DWORD iUnloadPort1SlideClose	: 1;	// X0302
		DWORD iUnloadPort1SlideOpen		: 1;	// X0303
		DWORD iUnlaodPort1LowCheck		: 1;	// X0304
		DWORD i0305 : 1;						// X0305
		DWORD i0306 : 1;						// X0306
		DWORD i0307 : 1;						// X0307
		DWORD iUnloadPort1Support1In	: 1;	// X0308
		DWORD iUnloadPort1Support1Out	: 1;	// X0309
		DWORD iUnloadPort1Support2In	: 1;	// X0310
		DWORD iUnloadPort1Support2Out	: 1;	// X0311
		DWORD i0312 : 1;						// X0312
		DWORD i0313 : 1;						// X0313
		DWORD i0314 : 1;						// X0314
		DWORD iUnloadPort1AreaCheck		: 1;	// X0315
		DWORD iUnloadPort2SlideLock		: 1;	// X0316
		DWORD iUnloadPort2SlideUnlock	: 1;	// X0317
		DWORD iUnloadPort2SlideClose	: 1;	// X0318
		DWORD iUnloadPort2SlideOpen		: 1;	// X0319
		DWORD iUnlaodPort2LowCheck		: 1;	// X0320
		DWORD i0321 : 1;						// X0321
		DWORD i0322	: 1;						// X0322
		DWORD i0323 : 1;						// X0323
		DWORD iUnloadPort2Support1In	: 1;	// X0324
		DWORD iUnloadPort2Support1Out	: 1;	// X0325
		DWORD iUnloadPort2Support2In	: 1;	// X0326
		DWORD iUnloadPort2Support2Out	: 1;	// X0327
		DWORD iUnloadPort2HighCheck		: 1;	// X0328
		DWORD i0329 : 1;						// X0329
		DWORD i0330	: 1;						// X0330
		DWORD iUnloadPort2AreaCheck		: 1;	// X0331
	};
} DX_DATA_03;	// Digital Input 03

typedef union tag_DX_DATA_04 {
	DWORD nValue;
	struct {
		DWORD iLoadStage1MasterIn	: 1;	// X0400
		DWORD iLoadStage1MasterOut	: 1;	// X0401
		DWORD iLoadStage1SlaveIn	: 1;	// X0402
		DWORD iLoadStage1SlaveOut	: 1;	// X0403
		DWORD iLoadStage1Exist		: 1;	// X0404
		DWORD i0405 : 1;					// X0405
		DWORD i0406	: 1;					// X0406
		DWORD i0407 : 1;					// X0407
		DWORD iLoadStage2MasterIn	: 1;	// X0408
		DWORD iLoadStage2MasterOut	: 1;	// X0409
		DWORD iLoadStage2SlaveIn	: 1;	// X0410
		DWORD iLoadStage2SlaveOut	: 1;	// X0411
		DWORD iLoadStage2Exist		: 1;	// X0412
		DWORD i0413 : 1;					// X0413
		DWORD i0414	: 1;					// X0414
		DWORD i0415 : 1;					// X0415
		DWORD iCapStage1MasterIn	: 1;	// X0416
		DWORD iCapStage1MasterOut	: 1;	// X0417
		DWORD iCapStage1SlaveIn		: 1;	// X0418
		DWORD iCapStage1SlaveOut	: 1;	// X0419
		DWORD iCapStage1Exist		: 1;	// X0420
		DWORD i0421 : 1;					// X0421
		DWORD i0422	: 1;					// X0422
		DWORD i0423 : 1;					// X0423
		DWORD iCapStage2MasterIn	: 1;	// X0424
		DWORD iCapStage2MasterOut	: 1;	// X0425
		DWORD iCapStage2SlaveIn		: 1;	// X0426
		DWORD iCapStage2SlaveOut	: 1;	// X0427
		DWORD iCapStage2Exist		: 1;	// X0428
		DWORD i0429 : 1;					// X0429
		DWORD i0430	: 1;					// X0430
		DWORD i0431 : 1;					// X0431
	};
} DX_DATA_04;	// Digital Input 04

typedef union tag_DX_DATA_05 {
	DWORD nValue;
	struct {
		DWORD iUnloadStage1MasterIn		: 1;	// X0500
		DWORD iUnloadStage1MasterOut	: 1;	// X0501
		DWORD iUnloadStage1SlaveIn		: 1;	// X0502
		DWORD iUnloadStage1SlaveOut		: 1;	// X0503
		DWORD iUnloadStage1Exist		: 1;	// X0504
		DWORD i0505 : 1;						// X0505
		DWORD i0506 : 1;						// X0506
		DWORD i0507 : 1;						// X0507
		DWORD iUnloadStage2MasterIn		: 1;	// X0508
		DWORD iUnloadStage2MasterOut	: 1;	// X0509
		DWORD iUnloadStage2SlaveIn		: 1;	// X0510
		DWORD iUnloadStage2SlaveOut		: 1;	// X0511
		DWORD iUnloadStage2Exist		: 1;	// X0512
		DWORD i0513 : 1;						// X0513
		DWORD i0514 : 1;						// X0514
		DWORD i0515 : 1;						// X0515
		DWORD iTransStageClamp1On		: 1;	// X0516
		DWORD iTransStageClamp1Off		: 1;	// X0517
		DWORD iTransStageClamp2On		: 1;	// X0518
		DWORD iTransStageClamp2Off		: 1;	// X0519
		DWORD i0520 : 1;						// X0520
		DWORD i0521 : 1;						// X0521
		DWORD i0522 : 1;						// X0522
		DWORD i0523 : 1;						// X0523
		DWORD iTransStageVac01			: 1;	// X0524
		DWORD iTransStageVac02			: 1;	// X0525
		DWORD iTransStageVac03			: 1;	// X0526
		DWORD iTransStageVac04			: 1;	// X0527
		DWORD iTransStageVac05			: 1;	// X0528
		DWORD iTransStageVac06			: 1;	// X0529
		DWORD i0530 : 1;						// X0530
		DWORD i0531 : 1;						// X0531
	};
} DX_DATA_05;	// Digital Input 05

typedef union tag_DX_DATA_06 {
	DWORD nValue;
	struct {
		DWORD iLoadPickerUp01		: 1;		// X0600
		DWORD iLoadPickerDown01		: 1;		// X0601
		DWORD iLoadPickerUp02		: 1;		// X0602
		DWORD iLoadPickerDown02		: 1;		// X0603
		DWORD iLoadPickerUp03		: 1;		// X0604
		DWORD iLoadPickerDown03		: 1;		// X0605
		DWORD iLoadPickerUp04		: 1;		// X0606
		DWORD iLoadPickerDown04		: 1;		// X0607
		DWORD iLoadPickerUp05		: 1;		// X0608
		DWORD iLoadPickerDown05		: 1;		// X0609
		DWORD iLoadPickerUp06		: 1;		// X0610
		DWORD iLoadPickerDown06		: 1;		// X0611
		DWORD i0612 : 1;						// X0612
		DWORD i0613 : 1;						// X0613
		DWORD i0614 : 1;						// X0614
		DWORD i0615 : 1;						// X0615
		DWORD iLoadPickerOpen01		: 1;		// X0616
		DWORD iLoadPickerOpen02		: 1;		// X0617
		DWORD iLoadPickerOpen03		: 1;		// X0618
		DWORD iLoadPickerOpen04		: 1;		// X0619
		DWORD iLoadPickerOpen05		: 1;		// X0620
		DWORD iLoadPickerOpen06		: 1;		// X0621
		DWORD i0622 : 1;						// X0622
		DWORD i0623 : 1;						// X0623
		DWORD iLoadPickerCMCheck01	: 1;		// X0624
		DWORD iLoadPickerCMCheck02	: 1;		// X0625
		DWORD iLoadPickerCMCheck03	: 1;		// X0626
		DWORD iLoadPickerCMCheck04	: 1;		// X0627
		DWORD iLoadPickerCMCheck05	: 1;		// X0628
		DWORD iLoadPickerCMCheck06	: 1;		// X0629
		DWORD i0630	: 1;						// X0630
		DWORD i0631 : 1;						// X0631
	};
} DX_DATA_06;	// Digital Input 06

typedef union tag_DX_DATA_07 {
	DWORD nValue;
	struct {
		DWORD iCapPickerUp01	: 1;	// X0700
		DWORD iCapPickerDown01	: 1;	// X0701
		DWORD iCapPickerTurn01	: 1;	// X0702
		DWORD iCapPickerUp02	: 1;	// X0703
		DWORD iCapPickerDown02	: 1;	// X0704
		DWORD iCapPickerTurn02	: 1;	// X0705
		DWORD iCapPickerUp03	: 1;	// X0706
		DWORD iCapPickerDown03	: 1;	// X0707
		DWORD iCapPickerTurn03	: 1;	// X0708
		DWORD iCapPickerUp04	: 1;	// X0709
		DWORD iCapPickerDown04	: 1;	// X0710
		DWORD iCapPickerTurn04	: 1;	// X0711
		DWORD iCapPickerUp05	: 1;	// X0712
		DWORD iCapPickerDown05	: 1;	// X0713
		DWORD iCapPickerTurn05	: 1;	// X0714
		DWORD iCapPickerUp06	: 1;	// X0715
		DWORD iCapPickerDown06	: 1;	// X0716
		DWORD iCapPickerTurn06	: 1;	// X0717
		DWORD i0718 : 1;				// X0718
		DWORD i0719 : 1;				// X0719
		DWORD iCapPickerVac01	: 1;	// X0720
		DWORD i0721 : 1;				// X0721
		DWORD iCapPickerVac02	: 1;	// X0722
		DWORD i0723 : 1;				// X0723
		DWORD iCapPickerVac03	: 1;	// X0724
		DWORD i0725	: 1;				// X0725
		DWORD iCapPickerVac04	: 1;	// X0726
		DWORD i0727 : 1;				// X0727
		DWORD iCapPickerVac05	: 1;	// X0728
		DWORD i0729: 1;					// X0729
		DWORD iCapPickerVac06	: 1;	// X0730
		DWORD i0731 : 1;				// X0731
	};
} DX_DATA_07;	// Digital Input 07

typedef union tag_DX_DATA_08 {
	DWORD nValue;
	struct {
		DWORD iAssyPickerUp01		: 1;	// X0800
		DWORD iAssyPickerDown01		: 1;	// X0801
		DWORD iAssyPickerUp02		: 1;	// X0802
		DWORD iAssyPickerDown02		: 1;	// X0803
		DWORD iAssyPickerUp03		: 1;	// X0804
		DWORD iAssyPickerDown03		: 1;	// X0805
		DWORD iAssyPickerUp04		: 1;	// X0806
		DWORD iAssyPickerDown04		: 1;	// X0807
		DWORD iAssyPickerUp05		: 1;	// X0808
		DWORD iAssyPickerDown05		: 1;	// X0809
		DWORD iAssyPickerUp06		: 1;	// X0810
		DWORD iAssyPickerDown06		: 1;	// X0811
		DWORD iAssyPickerTiltUp		: 1;	// X0812
		DWORD iAssyPickerTiltDown	: 1;	// X0813
		DWORD i0814	: 1;					// X0814
		DWORD i0815 : 1;					// X0815
		DWORD i0816	: 1;					// X0816
		DWORD i0817 : 1;					// X0817
		DWORD i0818	: 1;					// X0818
		DWORD i0819 : 1;					// X0819
		DWORD iAssyPickerVac01		: 1;	// X0820
		DWORD i0821 : 1;					// X0821
		DWORD iAssyPickerVac02		: 1;	// X0822
		DWORD i0823	: 1;					// X0823
		DWORD iAssyPickerVac03		: 1;	// X0824
		DWORD i0825 : 1;					// X0825
		DWORD iAssyPickerVac04		: 1;	// X0826
		DWORD i0827 : 1;					// X0827
		DWORD iAssyPickerVac05		: 1;	// X0828
		DWORD i0829 : 1;					// X0829
		DWORD iAssyPickerVac06		: 1;	// X0830
		DWORD i0831 : 1;					// X0831
	};
} DX_DATA_08;	// Digital Input 08

typedef union tag_DX_DATA_09 {
	DWORD nValue;
	struct {
		DWORD iAssyPickerTilt01			: 1;	// X0900
		DWORD iAssyPickerTilt02			: 1;	// X0901
		DWORD iAssyPickerTilt03			: 1;	// X0902
		DWORD iAssyPickerTilt04			: 1;	// X0903
		DWORD iAssyPickerTilt05			: 1;	// X0904
		DWORD iAssyPickerTilt06			: 1;	// X0905
		DWORD iAssyPickerCrossTilt		: 1;	// X0906
		DWORD i0907 : 1;						// X0907
		DWORD i0908 : 1;						// X0908
		DWORD i0909 : 1;						// X0909
		DWORD i0910 : 1;						// X0910
		DWORD i0911 : 1;						// X0911
		DWORD iCapBufferStageAlignIn	: 1;	// X0912
		DWORD iCapBufferStageAlignOut	: 1;	// X0913
		DWORD i0914 : 1;						// X0914
		DWORD i0915 : 1;						// X0915
		DWORD iCapCleanUp				: 1;	// X0916
		DWORD iCapCleanDown				: 1;	// X0917
		DWORD iCapCoverIn				: 1;	// X0918
		DWORD iCapCoverOut				: 1;	// X0919
		DWORD iCapCoverDown				: 1;	// X0920
		DWORD iCapCoverUp				: 1;	// X0921
		DWORD i0922 : 1;						// X0922
		DWORD i0923 : 1;						// X0923
		DWORD i0924	: 1;						// X0924
		DWORD i0925	: 1;						// X0925
		DWORD i0926	: 1;						// X0926
		DWORD i0927 : 1;						// X0927
		DWORD i0928 : 1;						// X0928
		DWORD i0929 : 1;						// X0929
		DWORD i0930	: 1;						// X0930
		DWORD i0931 : 1;						// X0931
	};
} DX_DATA_09;	// Digital Input 09

typedef union tag_DX_DATA_10 {
	DWORD nValue;
	struct {
		DWORD iUnloadPickerUp01		: 1;	// X1000
		DWORD iUnloadPickerDown01	: 1;	// X1001
		DWORD iUnloadPickerUp02		: 1;	// X1002
		DWORD iUnloadPickerDown02	: 1;	// X1003
		DWORD iUnloadPickerUp03		: 1;	// X1004
		DWORD iUnloadPickerDown03	: 1;	// X1005
		DWORD iUnloadPickerUp04		: 1;	// X1006
		DWORD iUnloadPickerDown04	: 1;	// X1007
		DWORD iUnloadPickerUp05		: 1;	// X1008
		DWORD iUnloadPickerDown05	: 1;	// X1009
		DWORD iUnloadPickerUp06		: 1;	// X1010
		DWORD iUnloadPickerDown06	: 1;	// X1011
		DWORD iUnloadPickerCapChk1	: 1;	// X1012
		DWORD iUnloadPickerCapChk2	: 1;	// X1013
		DWORD iUnloadPickerCapChk3	: 1;	// X1014
		DWORD iUnloadPickerCapChk4	: 1;	// X1015
		DWORD iUnloadPickerCapChk5	: 1;	// X1016
		DWORD iUnloadPickerCapChk6	: 1;	// X1017
		DWORD i1018 : 1;					// X1018
		DWORD i1019 : 1;					// X1019
		DWORD iUnloadPickerVac01 : 1;		// X1020
		DWORD i1021 : 1;					// X1021
		DWORD iUnloadPickerVac02 : 1;		// X1022
		DWORD i1023 : 1;					// X1023
		DWORD iUnloadPickerVac03 : 1;		// X1024
		DWORD i1025 : 1;					// X1025
		DWORD iUnloadPickerVac04 : 1;		// X1026
		DWORD i1027 : 1;					// X1027
		DWORD iUnloadPickerVac05 : 1;		// X1028
		DWORD i1029 : 1;					// X1029
		DWORD iUnloadPickerVac06 : 1;		// X1030
		DWORD i1031 : 1;					// X1031
	};
} DX_DATA_10;	// Digital Input 10

typedef union tag_DX_DATA_11 {
	DWORD nValue;
	struct {
		DWORD iIndexPosition0			: 1;	// X1100
		DWORD iIndexPosition1			: 1;	// X1101
		DWORD iIndexLoadAlignOut		: 1;	// X1102
		DWORD iIndexLoadAlignIn			: 1;	// X1103
		DWORD iIndexLoadVacUp			: 1;	// X1104
		DWORD iIndexLoadVacDown			: 1;	// X1105
		DWORD i1106	: 1;						// X1106
		DWORD i1107	: 1;						// X1107
		DWORD iIndexAssyAlignOut		: 1;	// X1108
		DWORD iIndexAssyAlignIn			: 1;	// X1109
		DWORD iIndexAssyVacUp			: 1;	// X1110
		DWORD iIndexAssyVacDown			: 1;	// X1111
		DWORD iIndexTransAlignOut		: 1;	// X1112
		DWORD iIndexTransAlignIn		: 1;	// X1113
		DWORD i1114 : 1;						// X1114
		DWORD i1115 : 1;						// X1115
		DWORD iIndexLoadVac01			: 1;	// X1116
		DWORD iIndexLoadVac02			: 1;	// X1117
		DWORD iIndexLoadVac03			: 1;	// X1118
		DWORD iIndexLoadVac04			: 1;	// X1119
		DWORD iIndexLoadVac05			: 1;	// X1120
		DWORD iIndexLoadVac06			: 1;	// X1121
		DWORD i1122	: 1;						// X1122
		DWORD i1123 : 1;						// X1123
		DWORD iIndexAssyVac01			: 1;	// X1124
		DWORD iIndexAssyVac02			: 1;	// X1125
		DWORD iIndexAssyVac03			: 1;	// X1126
		DWORD iIndexAssyVac04			: 1;	// X1127
		DWORD iIndexAssyVac05			: 1;	// X1128
		DWORD iIndexAssyVac06			: 1;	// X1129
		DWORD i1130 : 1;						// X1130
		DWORD i1131 : 1;						// X1131
	};
} DX_DATA_11;	// Digital Input 11

typedef union tag_DX_DATA_12 {
	DWORD nValue;
	struct {
		DWORD iEmgSw1			: 1;	// X1200
		DWORD iEmgSw2			: 1;	// X1201
		DWORD iEmgSw3			: 1;	// X1202
		DWORD i1203	: 1;				// X1203
		DWORD i1204	: 1;				// X1204
		DWORD i1205	: 1;				// X1205
		DWORD iMainAir1			: 1;	// X1206
		DWORD iMainAir2			: 1;	// X1207
		DWORD i1208 : 1;				// X1208
		DWORD i1209	: 1;				// X1209
		DWORD i1210 : 1;				// X1210
		DWORD i1211 : 1;				// X1211
		DWORD iStartSw			: 1;	// X1212
		DWORD i1213 : 1;				// X1213
		DWORD i1214	: 1;				// X1214
		DWORD iStopSw			: 1;	// X1215
		DWORD i1216 : 1;				// X1216
		DWORD i1217	: 1;				// X1217
		DWORD iResetSw			: 1;	// X1218
		DWORD i1219 : 1;				// X1219
		DWORD i1220	: 1;				// X1220
		DWORD i1221 : 1;				// X1221
		DWORD i1222 : 1;				// X1222
		DWORD i1223	: 1;				// X1223
		DWORD iLoad1Sw			: 1;	// X1224
		DWORD iLoad2Sw			: 1;	// X1225
		DWORD iLoad3Sw			: 1;	// X1226
		DWORD iCap1Sw			: 1;	// X1227
		DWORD iCap2Sw			: 1;	// X1228
		DWORD iUnload1Sw		: 1;	// X1229
		DWORD iUnload2Sw		: 1;	// X1230
		DWORD i1231 : 1;				// X1231
	};
} DX_DATA_12;	// Digital Input 12

typedef union tag_DX_DATA_13 {
	DWORD nValue;
	struct {
		DWORD iDoor01Unlock	: 1;	// X1300
		DWORD iDoor02Unlock	: 1;	// X1301
		DWORD iDoor03Unlock	: 1;	// X1302
		DWORD iDoor04Unlock	: 1;	// X1303
		DWORD iDoor05Unlock	: 1;	// X1304
		DWORD iDoor06Unlock	: 1;	// X1305
		DWORD iDoor07Unlock	: 1;	// X1306
		DWORD iDoor08Unlock	: 1;	// X1307
		DWORD iDoor09Unlock	: 1;	// X1308
		DWORD iDoor10Unlock	: 1;	// X1309
		DWORD iDoor11Unlock	: 1;	// X1310
		DWORD iDoor12Unlock	: 1;	// X1311
		DWORD iDoor13Unlock	: 1;	// X1312
		DWORD i1313	: 1;			// X1313
		DWORD i1314	: 1;			// X1314
		DWORD i1315 : 1;			// X1315
		DWORD i1316 : 1;			// X1316
		DWORD i1317 : 1;			// X1317
		DWORD i1318 : 1;			// X1318
		DWORD i1319 : 1;			// X1319
		DWORD i1320 : 1;			// X1320
		DWORD i1321 : 1;			// X1321
		DWORD i1322 : 1;			// X1322
		DWORD i1323 : 1;			// X1323
		DWORD i1324 : 1;			// X1324
		DWORD i1325 : 1;			// X1325
		DWORD i1326 : 1;			// X1326
		DWORD i1327 : 1;			// X1327
		DWORD i1328 : 1;			// X1328
		DWORD i1329 : 1;			// X1329
		DWORD i1330 : 1;			// X1330
		DWORD i1331 : 1;			// X1331
	};
} DX_DATA_13;	// Digital Input 13

///////////////////////////////////////////////////////////////////////////////
// Output (Y0000 - Y1331)

typedef union tag_DY_DATA_00 {
	DWORD nValue;
	struct {
		DWORD oTrayPickerGrip12Close: 1;	// Y0000
		DWORD oTrayPickerGrip12Open	: 1;	// Y0001
		DWORD o0002 : 1;					// Y0002
		DWORD o0003 : 1;					// Y0003
		DWORD oTrayPickerGrip34Close: 1;	// Y0004
		DWORD oTrayPickerGrip34Open	: 1;	// Y0005
		DWORD o0006 : 1;					// Y0006
		DWORD o0007 : 1;					// Y0007
		DWORD o0008	: 1;					// Y0008
		DWORD o0009 : 1;					// Y0009
		DWORD o0010 : 1;					// Y0010
		DWORD o0011 : 1;					// Y0011
		DWORD o0012	: 1;					// Y0012
		DWORD o0013 : 1;					// Y0013
		DWORD o0014 : 1;					// Y0014
		DWORD o0015 : 1;					// Y0015
		DWORD oLoadPort1SlideLock	: 1;	// Y0016
		DWORD oLoadPort1SlideUnlock : 1;	// Y0017
		DWORD o0018 : 1;					// Y0018
		DWORD o0019 : 1;					// Y0019
		DWORD o0020 : 1;					// Y0020
		DWORD o0021 : 1;					// Y0021
		DWORD o0022 : 1;					// Y0022
		DWORD o0023 : 1;					// Y0023
		DWORD oLoadPort1SupportIn	: 1;	// Y0024
		DWORD oLoadPort1SupportOut	: 1;	// Y0025
		DWORD o0026 : 1;					// Y0026
		DWORD o0027 : 1;					// Y0027
		DWORD o0028	: 1;					// Y0028
		DWORD o0029 : 1;					// Y0029
		DWORD o0030 : 1;					// Y0030
		DWORD o0031 : 1;					// Y0031
	};
} DY_DATA_00;		// Digital Output 00

typedef union tag_DY_DATA_01 {
	DWORD nValue;
	struct {
		DWORD oLoadPort2SlideLock : 1;		// Y0100
		DWORD oLoadPort2SlideUnlock : 1;	// Y0101
		DWORD o0102 : 1;					// Y0102
		DWORD o0103 : 1;					// Y0103
		DWORD o0104 : 1;					// Y0104
		DWORD o0105 : 1;					// Y0105
		DWORD o0106 : 1;					// Y0106
		DWORD o0107 : 1;					// Y0107
		DWORD oLoadPort2SupportIn : 1;		// Y0108
		DWORD oLoadPort2SupportOut : 1;		// Y0109
		DWORD o0110 : 1;					// Y0110
		DWORD o0111 : 1;					// Y0111
		DWORD o0112 : 1;					// Y0112
		DWORD o0113 : 1;					// Y0113
		DWORD o0114 : 1;					// Y0114
		DWORD o0115 : 1;					// Y0115
		DWORD oLoadPort3SlideLock : 1;		// Y0116
		DWORD oLoadPort3SlideUnlock : 1;	// Y0117
		DWORD o0118 : 1;					// Y0118
		DWORD o0119 : 1;					// Y0119
		DWORD o0120 : 1;					// Y0120
		DWORD o0121 : 1;					// Y0121
		DWORD o0122 : 1;					// Y0122
		DWORD o0123 : 1;					// Y0123
		DWORD o0124 : 1;					// Y0124
		DWORD o0125 : 1;					// Y0125
		DWORD o0126 : 1;					// Y0126
		DWORD o0127 : 1;					// Y0127
		DWORD o0128 : 1;					// Y0128
		DWORD o0129 : 1;					// Y0129
		DWORD o0130 : 1;					// Y0130
		DWORD o0131 : 1;					// Y0131
	};
} DY_DATA_01;		// Digital Output 01

typedef union tag_DY_DATA_02 {
	DWORD nValue;
	struct {
		DWORD oCapPort1SlideLock	: 1;	// Y0200
		DWORD oCapPort1SlideUnlock	: 1;	// Y0201
		DWORD o0202 : 1;					// Y0202
		DWORD o0203 : 1;					// Y0203
		DWORD o0204 : 1;					// Y0204
		DWORD o0205 : 1;					// Y0205
		DWORD o0206 : 1;					// Y0206
		DWORD o0207 : 1;					// Y0207
		DWORD oCapPort1SupportIn	: 1;	// Y0208
		DWORD oCapPort1SupportOut	: 1;	// Y0209
		DWORD o0210 : 1;					// Y0210
		DWORD o0211 : 1;					// Y0211
		DWORD o0212 : 1;					// Y0212
		DWORD o0213 : 1;					// Y0213
		DWORD o0214 : 1;					// Y0214
		DWORD o0215 : 1;					// Y0215
		DWORD oCapPort2SlideLock	: 1;	// Y0216
		DWORD oCapPort2SlideUnlock	: 1;	// Y0217
		DWORD o0218 : 1;					// Y0218
		DWORD o0219 : 1;					// Y0219
		DWORD o0220 : 1;					// Y0220
		DWORD o0221 : 1;					// Y0221
		DWORD o0222 : 1;					// Y0222
		DWORD o0223 : 1;					// Y0223
		DWORD oCapPort2SupportIn : 1;		// Y0224
		DWORD oCapPort2SupportOut : 1;		// Y0225
		DWORD o0226 : 1;					// Y0226
		DWORD o0227 : 1;					// Y0227
		DWORD o0228 : 1;					// Y0228
		DWORD o0229 : 1;					// Y0229
		DWORD o0230 : 1;					// Y0230
		DWORD o0231 : 1;					// Y0231
	};
} DY_DATA_02;		// Digital Output 02

typedef union tag_DY_DATA_03 {
	DWORD nValue;
	struct {
		DWORD oUnloadPort1SlideLock		: 1;	// Y0300
		DWORD oUnloadPort1SlideUnlock	: 1;	// Y0301
		DWORD o0302 : 1;						// Y0302
		DWORD o0303 : 1;						// Y0303
		DWORD o0304 : 1;						// Y0304
		DWORD o0305 : 1;						// Y0305
		DWORD o0306 : 1;						// Y0306
		DWORD o0307 : 1;						// Y0307
		DWORD oUnloadPort1SupportIn		: 1;	// Y0308
		DWORD oUnloadPort1SupportOut	: 1;	// Y0309
		DWORD o0310 : 1;						// Y0310
		DWORD o0311 : 1;						// Y0311
		DWORD o0312 : 1;						// Y0312
		DWORD o0313 : 1;						// Y0313
		DWORD o0314 : 1;						// Y0314
		DWORD o0315 : 1;						// Y0315
		DWORD oUnloadPort2SlideLock		: 1;	// Y0316
		DWORD oUnloadPort2SlideUnlock	: 1;	// Y0317
		DWORD o0318 : 1;						// Y0318
		DWORD o0319 : 1;						// Y0319
		DWORD o0320 : 1;						// Y0320
		DWORD o0321 : 1;						// Y0321
		DWORD o0322 : 1;						// Y0322
		DWORD o0323 : 1;						// Y0323
		DWORD oUnloadPort2SupportIn		: 1;	// Y0324
		DWORD oUnloadPort2SupportOut	: 1;	// Y0325
		DWORD o0326 : 1;						// Y0326
		DWORD o0327 : 1;						// Y0327
		DWORD o0328 : 1;						// Y0328
		DWORD o0329 : 1;						// Y0329
		DWORD o0330 : 1;						// Y0330
		DWORD o0331 : 1;						// Y0331
	};
} DY_DATA_03;		// Digital Output 03

typedef union tag_DY_DATA_04 {
	DWORD nValue;
	struct {
		DWORD oLoadStage1MasterIn	: 1;	// Y0400
		DWORD o0401 : 1;					// Y0401
		DWORD oLoadStage1SlaveIn	: 1;	// Y0402
		DWORD o0403 : 1;					// Y0403
		DWORD o0404 : 1;					// Y0404
		DWORD o0405 : 1;					// Y0405
		DWORD o0406 : 1;					// Y0406
		DWORD o0407 : 1;					// Y0407
		DWORD oLoadStage2MasterIn	: 1;	// Y0408
		DWORD o0409 : 1;					// Y0409
		DWORD oLoadStage2SlaveIn	: 1;	// Y0410
		DWORD o0411 : 1;					// Y0411
		DWORD o0412 : 1;					// Y0412
		DWORD o0413 : 1;					// Y0413
		DWORD o0414 : 1;					// Y0414
		DWORD o0415 : 1;					// Y0415
		DWORD oCapStage1MasterIn	: 1;	// Y0416
		DWORD o0417 : 1;					// Y0417
		DWORD oCapStage1SlaveIn		: 1;	// Y0418
		DWORD o0419 : 1;					// Y0419
		DWORD o0420 : 1;					// Y0420
		DWORD o0421 : 1;					// Y0421
		DWORD o0422 : 1;					// Y0422
		DWORD o0423 : 1;					// Y0423
		DWORD oCapStage2MasterIn	: 1;	// Y0424
		DWORD o0425 : 1;					// Y0425
		DWORD oCapStage2SlaveIn		: 1;	// Y0426
		DWORD o0427 : 1;					// Y0427
		DWORD o0428 : 1;					// Y0428
		DWORD o0429 : 1;					// Y0429
		DWORD o0430 : 1;					// Y0430
		DWORD o0431 : 1;					// Y0431
	};
} DY_DATA_04;		// Digital Output 04

typedef union tag_DY_DATA_05 {
	DWORD nValue;
	struct {
		DWORD oUnloadStage1MasterIn	: 1;	// Y0500
		DWORD o0502 : 1;					// Y0501
		DWORD oUnloadStage1SlaveIn	: 1;	// Y0502
		DWORD o0503 : 1;					// Y0503
		DWORD o0504 : 1;					// Y0504
		DWORD o0505 : 1;					// Y0505
		DWORD o0506 : 1;					// Y0506
		DWORD o0507 : 1;					// Y0507
		DWORD oUnloadStage2MasterIn : 1;	// Y0508
		DWORD o0509	: 1;					// Y0509
		DWORD oUnloadStage2SlaveIn	: 1;	// Y0510
		DWORD o0511	: 1;					// Y0511
		DWORD o0512	: 1;					// Y0512
		DWORD o0513	: 1;					// Y0513
		DWORD o0514	: 1;					// Y0514
		DWORD o0515	: 1;					// Y0515
		DWORD oTransStageClamp1On	: 1;	// Y0516
		DWORD oTransStageClamp1Off	: 1;	// Y0517
		DWORD oTransStageClamp2On	: 1;	// Y0518
		DWORD oTransStageClamp2Off	: 1;	// Y0519
		DWORD oTransStageVac01		: 1;	// Y0520
		DWORD oTransStageAir01		: 1;	// Y0521
		DWORD oTransStageVac02		: 1;	// Y0522
		DWORD oTransStageAir02		: 1;	// Y0523
		DWORD oTransStageVac03		: 1;	// Y0524
		DWORD oTransStageAir03		: 1;	// Y0525
		DWORD oTransStageVac04		: 1;	// Y0526
		DWORD oTransStageAir04		: 1;	// Y0527
		DWORD oTransStageVac05		: 1;	// Y0528
		DWORD oTransStageAir05		: 1;	// Y0529
		DWORD oTransStageVac06		: 1;	// Y0530
		DWORD oTransStageAir06		: 1;	// Y0531
	};
} DY_DATA_05;		// Digital Output 05

typedef union tag_DY_DATA_06 {
	DWORD nValue;
	struct {
		DWORD o0600 : 1;				// Y0600
		DWORD oLoadPickerDown01 : 1;	// Y0601
		DWORD o0602 : 1;				// Y0602
		DWORD oLoadPickerDown02 : 1;	// Y0603
		DWORD o0604 : 1;				// Y0604
		DWORD oLoadPickerDown03 : 1;	// Y0605
		DWORD o0606 : 1;				// Y0606
		DWORD oLoadPickerDown04 : 1;	// Y0607
		DWORD o0608 : 1;				// Y0608
		DWORD oLoadPickerDown05 : 1;	// Y0609
		DWORD o0610 : 1;				// Y0610
		DWORD oLoadPickerDown06 : 1;	// Y0611
		DWORD o0612 : 1;				// Y0612
		DWORD o0613 : 1;				// Y0613
		DWORD o0614 : 1;				// Y0614
		DWORD o0615 : 1;				// Y0615
		DWORD oLoadPickerGrip01 : 1;	// Y0616
		DWORD oLoadPickerGrip02 : 1;	// Y0617
		DWORD oLoadPickerGrip03 : 1;	// Y0618
		DWORD oLoadPickerGrip04 : 1;	// Y0619
		DWORD oLoadPickerGrip05 : 1;	// Y0620
		DWORD oLoadPickerGrip06 : 1;	// Y0621
		DWORD o0622 : 1;				// Y0622
		DWORD o0623 : 1;				// Y0623
		DWORD o0624 : 1;				// Y0624
		DWORD o0625 : 1;				// Y0625
		DWORD o0626 : 1;				// Y0626
		DWORD o0627 : 1;				// Y0627
		DWORD o0628 : 1;				// Y0628
		DWORD o0629 : 1;				// Y0629
		DWORD o0630 : 1;				// Y0630
		DWORD o0631 : 1;				// Y0631
	};
} DY_DATA_06;		// Digital Output 06

typedef union tag_DY_DATA_07 {
	DWORD nValue;
	struct {
		DWORD o0702 : 1;				// Y0700
		DWORD oCapPickerDown01 : 1;		// Y0701
		DWORD oCapPickerTurn01 : 1;		// Y0702
		DWORD o0703 : 1;				// Y0703
		DWORD oCapPickerDown02 : 1;		// Y0704
		DWORD oCapPickerTurn02 : 1;		// Y0705
		DWORD o0706 : 1;				// Y0706
		DWORD oCapPickerDown03 : 1;		// Y0707
		DWORD oCapPickerTurn03 : 1;		// Y0708
		DWORD o0709 : 1;				// Y0709
		DWORD oCapPickerDown04 : 1;		// Y0710
		DWORD oCapPickerTurn04 : 1;		// Y0711
		DWORD o0712 : 1;				// Y0712
		DWORD oCapPickerDown05 : 1;		// Y0713
		DWORD oCapPickerTurn05 : 1;		// Y0714
		DWORD o0715 : 1;				// Y0715
		DWORD oCapPickerDown06 : 1;		// Y0716
		DWORD oCapPickerTurn06 : 1;		// Y0717
		DWORD o0718 : 1;				// Y0718
		DWORD o0719 : 1;				// Y0719
		DWORD oCapPickerVac01 : 1;		// Y0720
		DWORD oCapPickerAir01 : 1;		// Y0721
		DWORD oCapPickerVac02 : 1;		// Y0722
		DWORD oCapPickerAir02 : 1;		// Y0723
		DWORD oCapPickerVac03 : 1;		// Y0724
		DWORD oCapPickerAir03 : 1;		// Y0725
		DWORD oCapPickerVac04 : 1;		// Y0726
		DWORD oCapPickerAir04 : 1;		// Y0727
		DWORD oCapPickerVac05 : 1;		// Y0728
		DWORD oCapPickerAir05 : 1;		// Y0729
		DWORD oCapPickerVac06 : 1;		// Y0730
		DWORD oCapPickerAir06 : 1;		// Y0731
	};
} DY_DATA_07;		// Digital Output 07

typedef union tag_DY_DATA_08 {
	DWORD nValue;
	struct {
		DWORD o0800 : 1;				// Y0800
		DWORD oAssyPickerDown01 : 1;	// Y0801
		DWORD o0802 : 1;				// Y0802
		DWORD oAssyPickerDown02 : 1;	// Y0803
		DWORD o0804 : 1;				// Y0804
		DWORD oAssyPickerDown03 : 1;	// Y0805
		DWORD o0806 : 1;				// Y0806
		DWORD oAssyPickerDown04 : 1;	// Y0807
		DWORD o0808 : 1;				// Y0808
		DWORD oAssyPickerDown05 : 1;	// Y0809
		DWORD o0810 : 1;				// Y0810
		DWORD oAssyPickerDown06 : 1;	// Y0811
		DWORD o0812 : 1;				// Y0812
		DWORD oAssyPickerTiltDown : 1;	// Y0813
		DWORD o0814 : 1;				// Y0814
		DWORD o0815 : 1;				// Y0815
		DWORD o0816 : 1;				// Y0816
		DWORD o0817 : 1;				// Y0817
		DWORD o0818 : 1;				// Y0818
		DWORD o0819 : 1;				// Y0819
		DWORD oAssyPickerVac01 : 1;		// Y0820
		DWORD oAssyPickerAir01 : 1;		// Y0821
		DWORD oAssyPickerVac02 : 1;		// Y0822
		DWORD oAssyPickerAir02 : 1;		// Y0823
		DWORD oAssyPickerVac03 : 1;		// Y0824
		DWORD oAssyPickerAir03 : 1;		// Y0825
		DWORD oAssyPickerVac04 : 1;		// Y0826
		DWORD oAssyPickerAir04 : 1;		// Y0827
		DWORD oAssyPickerVac05 : 1;		// Y0828
		DWORD oAssyPickerAir05 : 1;		// Y0829
		DWORD oAssyPickerVac06 : 1;		// Y0830
		DWORD oAssyPickerAir06 : 1;		// Y0831
	};
} DY_DATA_08;		// Digital Output 08

typedef union tag_DY_DATA_09 {
	DWORD nValue;
	struct {
		DWORD o0900 : 1;					// Y0900
		DWORD o0901 : 1;					// Y0901
		DWORD o0902 : 1;					// Y0902
		DWORD o0903 : 1;					// Y0903
		DWORD o0904 : 1;					// Y0904
		DWORD o0905 : 1;					// Y0905
		DWORD o0906 : 1;					// Y0906
		DWORD o0907 : 1;					// Y0907
		DWORD o0908 : 1;					// Y0908
		DWORD o0909 : 1;					// Y0909
		DWORD o0910 : 1;					// Y0910
		DWORD o0911 : 1;					// Y0911
		DWORD oCapBufferAlignIn		: 1;	// Y0912
		DWORD o0913 : 1;					// Y0913
		DWORD o0914 : 1;					// Y0914
		DWORD o0915 : 1;					// Y0915
		DWORD oCapCleanUp			: 1;	// Y0916
		DWORD o0917 : 1;					// Y0917
		DWORD oCapCoverIn		: 1;	// Y0918
		DWORD o0919 : 1;					// Y0919
		DWORD oCapCoverDown	: 1;	// Y0920
		DWORD o0921 : 1;					// Y0921
		DWORD o0922 : 1;					// Y0922
		DWORD o0923 : 1;					// Y0923
		DWORD oCapCleanAir01		: 1;	// Y0924
		DWORD oCapCleanAir02		: 1;	// Y0925
		DWORD oCapCleanAir03		: 1;	// Y0926
		DWORD oCapCleanAir04		: 1;	// Y0927
		DWORD oCapCleanAir05		: 1;	// Y0928
		DWORD oCapCleanAir06		: 1;	// Y0929
		DWORD o0930 : 1;					// Y0930
		DWORD oCapCleanSuction		: 1;	// Y0931
	};
} DY_DATA_09;		// Digital Output 09

typedef union tag_DY_DATA_10 {
	DWORD nValue;
	struct {
		DWORD o1000 : 1;					// Y1000
		DWORD oUnloadPickerDown01 : 1;		// Y1001
		DWORD o1002 : 1;					// Y1002
		DWORD oUnloadPickerDown02 : 1;		// Y1003
		DWORD o1004 : 1;					// Y1004
		DWORD oUnloadPickerDown03 : 1;		// Y1005
		DWORD o1006 : 1;					// Y1006
		DWORD oUnloadPickerDown04 : 1;		// Y1007
		DWORD o1008 : 1;					// Y1008
		DWORD oUnloadPickerDown05 : 1;		// Y1009
		DWORD o1010 : 1;					// Y1010
		DWORD oUnloadPickerDown06 : 1;		// Y1011
		DWORD o1012 : 1;					// Y1012
		DWORD o1013 : 1;					// Y1013
		DWORD o1014 : 1;					// Y1014
		DWORD o1015 : 1;					// Y1015
		DWORD o1016 : 1;					// Y1016
		DWORD o1017 : 1;					// Y1017
		DWORD o1018 : 1;					// Y1018
		DWORD o1019 : 1;					// Y1019
		DWORD oUnloadPickerVac01 : 1;		// Y1020
		DWORD oUnloadPickerAir01 : 1;		// Y1021
		DWORD oUnloadPickerVac02 : 1;		// Y1022
		DWORD oUnloadPickerAir02 : 1;		// Y1023
		DWORD oUnloadPickerVac03 : 1;		// Y1024
		DWORD oUnloadPickerAir03 : 1;		// Y1025
		DWORD oUnloadPickerVac04 : 1;		// Y1026
		DWORD oUnloadPickerAir04 : 1;		// Y1027
		DWORD oUnloadPickerVac05 : 1;		// Y1028
		DWORD oUnloadPickerAir05 : 1;		// Y1029
		DWORD oUnloadPickerVac06 : 1;		// Y1030
		DWORD oUnloadPickerAir06 : 1;		// Y1031
	};
} DY_DATA_10;		// Digital Output 10

typedef union tag_DY_DATA_11 {
	DWORD nValue;
	struct {
		DWORD o1100 : 1;					// Y1100
		DWORD o1101 : 1;					// Y1101
		DWORD oIndexLoadAlignOut	: 1;	// Y1102
		DWORD o1103 : 1;					// Y1103
		DWORD oIndexLoadVacUp		: 1;	// Y1104
		DWORD o1105 : 1;					// Y1105
		DWORD o1106	: 1;					// Y1106
		DWORD o1107 : 1;					// Y1107
		DWORD oIndexAssyAlignOut	: 1;	// Y1108
		DWORD o1110 : 1;					// Y1109
		DWORD oIndexAssyVacUp		: 1;	// Y1110
		DWORD o1111 : 1;					// Y1111
		DWORD oIndexTransAlignOut	: 1;	// Y1112
		DWORD o1113 : 1;					// Y1113
		DWORD o1114 : 1;					// Y1114
		DWORD o1115 : 1;					// Y1115
		DWORD oIndexLoadVac01 : 1;			// Y1116
		DWORD oIndexLoadVac02 : 1;			// Y1117
		DWORD oIndexLoadVac03 : 1;			// Y1118
		DWORD oIndexLoadVac04 : 1;			// Y1119
		DWORD oIndexLoadVac05 : 1;			// Y1120
		DWORD oIndexLoadVac06 : 1;			// Y1121
		DWORD o1122 : 1;					// Y1122
		DWORD o1123 : 1;					// Y1123
		DWORD oIndexAssyVac01 : 1;			// Y1124
		DWORD oIndexAssyVac02 : 1;			// Y1125
		DWORD oIndexAssyVac03 : 1;			// Y1126
		DWORD oIndexAssyVac04 : 1;			// Y1127
		DWORD oIndexAssyVac05 : 1;			// Y1128
		DWORD oIndexAssyVac06 : 1;			// Y1129
		DWORD o1130 : 1;					// Y1130
		DWORD o1131 : 1;					// Y1131
	};
} DY_DATA_11;	// Digital Output 11

typedef union tag_DY_DATA_12 {
	DWORD nValue;
	struct {
		DWORD oTowerGreen			: 1;	// Y1200
		DWORD oTowerYellow			: 1;	// Y1201
		DWORD oTowerRed				: 1;	// Y1202
		DWORD oBuzzerBit0			: 1;	// Y1203
		DWORD oBuzzerBit1			: 1;	// Y1204
		DWORD oBuzzerBit2			: 1;	// Y1205
		DWORD oBuzzerBit3			: 1;	// Y1206
		DWORD oBuzzerBit4			: 1;	// Y1207
		DWORD o1208 : 1;					// Y1208
		DWORD o1209 : 1;					// Y1209
		DWORD o1210 : 1;					// Y1210
		DWORD o1211 : 1;					// Y1211
		DWORD oStartLamp1			: 1;	// Y1212
		DWORD oStartLamp2			: 1;	// Y1213
		DWORD o1214	: 1;					// Y1214
		DWORD oStopLamp1			: 1;	// Y1215
		DWORD oStopLamp2			: 1;	// Y1216
		DWORD o1217	: 1;					// Y1217
		DWORD oResetLamp1			: 1;	// Y1218
		DWORD oResetLamp2			: 1;	// Y1219
		DWORD o1220	: 1;					// Y1220
		DWORD o1221 : 1;					// Y1221
		DWORD o1222 : 1;					// Y1222
		DWORD o1223 : 1;					// Y1223
		DWORD oLoad1Lamp			: 1;	// Y1224
		DWORD oLoad2Lamp			: 1;	// Y1225
		DWORD oLoad3Lamp			: 1;	// Y1226
		DWORD oCap1Lamp				: 1;	// Y1227
		DWORD oCap2Lamp				: 1;	// Y1228
		DWORD oUnload1Lamp			: 1;	// Y1229
		DWORD oUnload2Lamp			: 1;	// Y1230
		DWORD o1231 : 1;					// Y1231
	};
} DY_DATA_12;	// Digital Output 12

typedef union tag_DY_DATA_13 {
	DWORD nValue;
	struct {
		DWORD oDoor01Unlock : 1;	// Y1300
		DWORD oDoor02Unlock : 1;	// Y1301
		DWORD oDoor03Unlock : 1;	// Y1302
		DWORD oDoor04Unlock : 1;	// Y1303
		DWORD oDoor05Unlock : 1;	// Y1304
		DWORD oDoor06Unlock : 1;	// Y1305
		DWORD oDoor07Unlock : 1;	// Y1306
		DWORD oDoor08Unlock : 1;	// Y1307
		DWORD oDoor09Unlock : 1;	// Y1308
		DWORD oDoor10Unlock : 1;	// Y1309
		DWORD oDoor11Unlock : 1;	// Y1310
		DWORD oDoor12Unlock : 1;	// Y1311
		DWORD oDoor13Unlock : 1;	// Y1312
		DWORD o1313 : 1;			// Y1313
		DWORD o1314 : 1;			// Y1314
		DWORD o1315 : 1;			// Y1315
		DWORD o1316 : 1;			// Y1316
		DWORD o1317 : 1;			// Y1317
		DWORD o1318 : 1;			// Y1318
		DWORD o1319 : 1;			// Y1319
		DWORD o1320 : 1;			// Y1320
		DWORD o1321 : 1;			// Y1321
		DWORD o1322 : 1;			// Y1322
		DWORD o1323 : 1;			// Y1323
		DWORD oInsideLight : 1;		// Y1324
		DWORD oSafetyReset : 1;		// Y1325
		DWORD oModeSelect  : 1;		// Y1326
		DWORD o1327 : 1;			// Y1327
		DWORD o1328 : 1;			// Y1328
		DWORD o1329 : 1;			// Y1329
		DWORD o1330 : 1;			// Y1330
		DWORD o1331 : 1;			// Y1331
	};
} DY_DATA_13;	// Digital Output 13

///////////////////////////////////////////////////////////////////////////////
