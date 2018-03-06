/* SONY CONFIDENTIAL
 * Copyright (C) 2013 Sony Corporation.
 * All Rights Reserved.
 */

#ifndef _SFACE_H_
#define _SFACE_H_

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#ifndef WIN32
#include <stdbool.h>
#endif

#define S_FACE_OK						(0)
#define S_FACE_ERROR_NO_MEMORY			(-2138374143) /* 0x808B0001 */
#define S_FACE_ERROR_INVALID_PARAM		(-2138374142) /* 0x808B0002 */
#define S_FACE_ERROR_INVALID_DICT		(-2138374141) /* 0x808B0003 */
#define S_FACE_ERROR_IMPERF_PARTS		(-2138374140) /* 0x808B0004 */
#define S_FACE_ERROR_OUT_OF_RANGE		(-2138374139) /* 0x808B0005 */
#define S_FACE_ERROR_IMPERF_SHAPE		(-2138374138) /* 0x808B0006 */

#define S_FACE_ERROR_TIME_EXPIRED		(-2138374137) /* 0x808B0007 */

/**********************************************************************
 Face Detection
**********************************************************************/

typedef void *SFaceDetectionDictPtr;

#if defined(_WIN64) || defined(__x86_64__) || defined(__arm64__) // 64bit
#define S_FACE_DETECT_FRONTAL_DICT				"V01_R20P15Y10_S2R2_DV3_x64.fdd"
#define S_FACE_DETECT_ROLL_DICT					"V03_R30P20Y20_S2R2_JG26GR_x64.fdd"
#define S_FACE_DETECT_YAW_DICT					"V03_R10P20Y60_S2R2_CY50G_x64.fdd"
#define S_FACE_DETECT_PITCH_DICT				"V03_R10P60Y20_S2R2_LB30GR_x64.fdd"
#define S_FACE_DETECT_ROLL_YAW_DICT				"V09_R30P20Y60_S2R2_BM16GR_x64.fdd"
#define S_FACE_DETECT_ROLL_YAW_PITCH_DICT		"V15_R30P60Y60_S2R2_KA26_x64.fdd"
#define S_FACE_DETECT_FRONTAL_DICT_SIZE			(26324)
#define S_FACE_DETECT_ROLL_DICT_SIZE			(50380)
#define S_FACE_DETECT_YAW_DICT_SIZE				(85764)
#define S_FACE_DETECT_PITCH_DICT_SIZE			(75276)
#define S_FACE_DETECT_ROLL_YAW_DICT_SIZE		(123012)
#define S_FACE_DETECT_ROLL_YAW_PITCH_DICT_SIZE	(175340)
#else // 32bit
#define S_FACE_DETECT_FRONTAL_DICT				"V01_R20P15Y10_S2R2_DV3.fdd"
#define S_FACE_DETECT_ROLL_DICT					"V03_R30P20Y20_S2R2_JG26GR.fdd"
#define S_FACE_DETECT_YAW_DICT					"V03_R10P20Y60_S2R2_CY50G.fdd"
#define S_FACE_DETECT_PITCH_DICT				"V03_R10P60Y20_S2R2_LB30GR.fdd"
#define S_FACE_DETECT_ROLL_YAW_DICT				"V09_R30P20Y60_S2R2_BM16GR.fdd"
#define S_FACE_DETECT_ROLL_YAW_PITCH_DICT		"V15_R30P60Y60_S2R2_KA26.fdd"
#define S_FACE_DETECT_FRONTAL_DICT_SIZE			(26216)
#define S_FACE_DETECT_ROLL_DICT_SIZE			(50160)
#define S_FACE_DETECT_YAW_DICT_SIZE				(85544)
#define S_FACE_DETECT_PITCH_DICT_SIZE			(75056)
#define S_FACE_DETECT_ROLL_YAW_DICT_SIZE		(122372)
#define S_FACE_DETECT_ROLL_YAW_PITCH_DICT_SIZE	(174364)
#endif // defined(_WIN64) || defined(__x86_64__)

#define S_FACE_DETECT_RESULT_NORMAL		0
#define S_FACE_DETECT_RESULT_PRECISE	1
#define S_FACE_DETECT_RESULT_FAST		2

#define S_FACE_DETECT_SEARCH_ALL_FACE		0
#define S_FACE_DETECT_SEARCH_FACE_NUM_LIMIT	1

typedef struct SFaceDetectionResult {
	float	faceX;
	float	faceY;
	float	faceW;
	float	faceH;
	float	faceRoll;
	float	facePitch;
	float	faceYaw;
	float	score;
} SFaceDetectionResult;

typedef struct SFaceDetectionParam {
	int     version;
	int     size;
	float	magBegin;
	float	magStep;
	float	magEnd;
	int		xScanStart;
	int		yScanStart;
	int		xScanStep;
	int		yScanStep;
	float	xScanOver; // 0 - 0.5
	float	yScanOver; // 0 - 0.5
	float	thresholdScore;
	int		resultPrecision;
	int		searchType;
} SFaceDetectionParam;

int sFaceDetection(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceDetectionDictPtr		detectDictPtr,
	float							magBegin,
	float							magStep,
	float							magEnd,
	int								xScanStep,
	int								yScanStep,
	float							thresholdScore,
	int								resultPrecision,
	SFaceDetectionResult			resultFaceArray[],
	int								resultFaceArraySize,
	int								*resultFaceNum,
	void							*workMemory,
	int								workMemorySize
);

int sFaceDetectionGetDefaultParam(SFaceDetectionParam *param);

int sFaceDetectionEx(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceDetectionDictPtr		detectDictPtr,
	const SFaceDetectionParam		*detectParam,
	SFaceDetectionResult			resultFaceArray[],
	int								resultFaceArraySize,
	int								*resultFaceNum,
	void							*workMemory,
	int								workMemorySize
);

int sFaceDetectionLocal(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceDetectionDictPtr		detectDictPtr,
	float							magStep,
	float							xExpandRegion,
	float							yExpandRegion,
	int								xScanStep,
	int								yScanStep,
	float							thresholdScore,
	const SFaceDetectionResult		referenceFaceArray[],
	int								referenceFaceArraySize,
	SFaceDetectionResult			resultFaceArray[],
	int								resultFaceArraySize,
	int								*resultFaceNum,
	void							*workMemory,
	int								workMemorySize
);

int sFaceDetectionGetWorkingMemorySize(
	int						width,
	int						height,
	int						rowstride,
	SFaceDetectionDictPtr	detectDictPtr
);

/**********************************************************************
 Parts Detection
**********************************************************************/

typedef void *SFacePartsDictPtr;
typedef void *SFaceShapeDictPtr;
typedef void *SFacePartsCheckDictPtr;

#if defined(_WIN64) || defined(__x86_64__) || defined(__arm64__) // 64bit
#define S_FACE_PARTS_ROLL_DICT				"PD_EYE_NOSE_MOUTH_CENTER_E1_64_x64.pdd"
#define S_FACE_PARTS_ROLL_YAW_DICT			"PD_EYE_NOSE_MOUTH_CENTER_64_5DIV_N1_x64.pdd"
#define S_FACE_PARTS_ROLL_YAW_DICT2			"PD_EYE_NOSE_MOUTH_CENTER_64_5DIV_N5_x64.pdd"
#define S_FACE_ALLPARTS_DICT				"PD_ALL_PARTS_x64.pdd"
#define S_FACE_ALLPARTS_SHAPE_DICT			"ALL_PARTS_SHAPE_DATA_x64.asd"
#define S_FACE_PARTS_CHECK_DICT				"EYE_NOSE_MOUTH_CENTER_CHECK_x64.pcd"
#define S_FACE_PARTS_ROLL_DICT_SIZE			(41060)
#define S_FACE_PARTS_ROLL_YAW_DICT_SIZE		(100468)
#define S_FACE_PARTS_ROLL_YAW_DICT2_SIZE	(496716)
#define S_FACE_ALLPARTS_DICT_SIZE			(1618000)
#define S_FACE_ALLPARTS_SHAPE_DICT_SIZE		(55252)
#define S_FACE_PARTS_CHECK_DICT_SIZE		(1532)
#else // 32bit
#define S_FACE_PARTS_ROLL_DICT				"PD_EYE_NOSE_MOUTH_CENTER_E1_64.pdd"
#define S_FACE_PARTS_ROLL_YAW_DICT			"PD_EYE_NOSE_MOUTH_CENTER_64_5DIV_N1.pdd"
#define S_FACE_PARTS_ROLL_YAW_DICT2			"PD_EYE_NOSE_MOUTH_CENTER_64_5DIV_N5.pdd"
#define S_FACE_ALLPARTS_DICT				"PD_ALL_PARTS.pdd"
#define S_FACE_ALLPARTS_SHAPE_DICT			"ALL_PARTS_SHAPE_DATA.asd"
#define S_FACE_PARTS_CHECK_DICT				"EYE_NOSE_MOUTH_CENTER_CHECK.pcd"
#define S_FACE_PARTS_ROLL_DICT_SIZE			(40832)
#define S_FACE_PARTS_ROLL_YAW_DICT_SIZE		(99408)
#define S_FACE_PARTS_ROLL_YAW_DICT2_SIZE	(495656)
#define S_FACE_ALLPARTS_DICT_SIZE			(1609180)
#define S_FACE_ALLPARTS_SHAPE_DICT_SIZE		(55180)
#define S_FACE_PARTS_CHECK_DICT_SIZE		(1516)
//#define S_FACE_PARTS_CHECK_WIDE_DICT_SIZE	(3168)
#endif // defined(_WIN64) || defined(__x86_64__)

#define S_FACE_PARTS_NUM_MAX			4
#define S_FACE_PARTS_ID_UNDEF			0
#define S_FACE_PARTS_ID_R_EYE_CENTER	1
#define S_FACE_PARTS_ID_L_EYE_CENTER	2
#define S_FACE_PARTS_ID_NOSE_CENTER		3
#define S_FACE_PARTS_ID_MOUTH_CENTER	4

#define S_FACE_PARTS_R_EYE_CENTER_INDEX	0
#define S_FACE_PARTS_L_EYE_CENTER_INDEX	1
#define S_FACE_PARTS_NOSE_CENTER_INDEX	2
#define S_FACE_PARTS_MOUTH_CENTER_INDEX	3

#define S_FACE_ALLPARTS_NUM_MAX				55
#define S_FACE_PARTS_ID_ALL_BASE			100
#define S_FACE_PARTS_FACE_ID_ALL_00			(S_FACE_PARTS_ID_ALL_BASE+0)
#define S_FACE_PARTS_FACE_ID_ALL_01			(S_FACE_PARTS_ID_ALL_BASE+1)
#define S_FACE_PARTS_FACE_ID_ALL_02			(S_FACE_PARTS_ID_ALL_BASE+2)
#define S_FACE_PARTS_FACE_ID_ALL_03			(S_FACE_PARTS_ID_ALL_BASE+3)
#define S_FACE_PARTS_FACE_ID_ALL_04			(S_FACE_PARTS_ID_ALL_BASE+4)
#define S_FACE_PARTS_FACE_ID_ALL_05			(S_FACE_PARTS_ID_ALL_BASE+5)
#define S_FACE_PARTS_FACE_ID_ALL_06			(S_FACE_PARTS_ID_ALL_BASE+6)
#define S_FACE_PARTS_FACE_ID_ALL_07			(S_FACE_PARTS_ID_ALL_BASE+7)
#define S_FACE_PARTS_FACE_ID_ALL_08			(S_FACE_PARTS_ID_ALL_BASE+8)
#define S_FACE_PARTS_FACE_ID_ALL_09			(S_FACE_PARTS_ID_ALL_BASE+9)
#define S_FACE_PARTS_FACE_ID_ALL_10			(S_FACE_PARTS_ID_ALL_BASE+10)
#define S_FACE_PARTS_FACE_ID_ALL_11			(S_FACE_PARTS_ID_ALL_BASE+11)
#define S_FACE_PARTS_R_EYEBROW_ID_ALL_12	(S_FACE_PARTS_ID_ALL_BASE+12)
#define S_FACE_PARTS_R_EYEBROW_ID_ALL_13	(S_FACE_PARTS_ID_ALL_BASE+13)
#define S_FACE_PARTS_R_EYEBROW_ID_ALL_14	(S_FACE_PARTS_ID_ALL_BASE+14)
#define S_FACE_PARTS_R_EYE_ID_ALL_15		(S_FACE_PARTS_ID_ALL_BASE+15)
#define S_FACE_PARTS_R_EYE_ID_ALL_16		(S_FACE_PARTS_ID_ALL_BASE+16)
#define S_FACE_PARTS_R_EYE_ID_ALL_17		(S_FACE_PARTS_ID_ALL_BASE+17)
#define S_FACE_PARTS_R_EYE_ID_ALL_18		(S_FACE_PARTS_ID_ALL_BASE+18)
#define S_FACE_PARTS_R_EYE_ID_ALL_19		(S_FACE_PARTS_ID_ALL_BASE+19)
#define S_FACE_PARTS_R_EYE_ID_ALL_20		(S_FACE_PARTS_ID_ALL_BASE+20)
#define S_FACE_PARTS_R_EYE_ID_ALL_21		(S_FACE_PARTS_ID_ALL_BASE+21)
#define S_FACE_PARTS_R_EYE_ID_ALL_22		(S_FACE_PARTS_ID_ALL_BASE+22)
#define S_FACE_PARTS_R_PUPIL_ID_ALL_23		(S_FACE_PARTS_ID_ALL_BASE+23)
#define S_FACE_PARTS_L_EYEBROW_ID_ALL_24	(S_FACE_PARTS_ID_ALL_BASE+24)
#define S_FACE_PARTS_L_EYEBROW_ID_ALL_25	(S_FACE_PARTS_ID_ALL_BASE+25)
#define S_FACE_PARTS_L_EYEBROW_ID_ALL_26	(S_FACE_PARTS_ID_ALL_BASE+26)
#define S_FACE_PARTS_L_EYE_ID_ALL_27		(S_FACE_PARTS_ID_ALL_BASE+27)
#define S_FACE_PARTS_L_EYE_ID_ALL_28		(S_FACE_PARTS_ID_ALL_BASE+28)
#define S_FACE_PARTS_L_EYE_ID_ALL_29		(S_FACE_PARTS_ID_ALL_BASE+29)
#define S_FACE_PARTS_L_EYE_ID_ALL_30		(S_FACE_PARTS_ID_ALL_BASE+30)
#define S_FACE_PARTS_L_EYE_ID_ALL_31		(S_FACE_PARTS_ID_ALL_BASE+31)
#define S_FACE_PARTS_L_EYE_ID_ALL_32		(S_FACE_PARTS_ID_ALL_BASE+32)
#define S_FACE_PARTS_L_EYE_ID_ALL_33		(S_FACE_PARTS_ID_ALL_BASE+33)
#define S_FACE_PARTS_L_EYE_ID_ALL_34		(S_FACE_PARTS_ID_ALL_BASE+34)
#define S_FACE_PARTS_L_PUPIL_ID_ALL_35		(S_FACE_PARTS_ID_ALL_BASE+35)
#define S_FACE_PARTS_NOSE_ID_ALL_36			(S_FACE_PARTS_ID_ALL_BASE+36)
#define S_FACE_PARTS_NOSE_ID_ALL_37			(S_FACE_PARTS_ID_ALL_BASE+37)
/*#define S_FACE_PARTS_NOSE_ID_ALL_38		(S_FACE_PARTS_ID_ALL_BASE+38)	unsupported */
/*#define S_FACE_PARTS_NOSE_ID_ALL_39		(S_FACE_PARTS_ID_ALL_BASE+39)	unsupported */
#define S_FACE_PARTS_NOSTRIL_ID_ALL_40		(S_FACE_PARTS_ID_ALL_BASE+40)
#define S_FACE_PARTS_NOSTRIL_ID_ALL_41		(S_FACE_PARTS_ID_ALL_BASE+41)
#define S_FACE_PARTS_NOSTRIL_ID_ALL_42		(S_FACE_PARTS_ID_ALL_BASE+42)
/*#define S_FACE_PARTS_NOSE_ID_ALL_43		(S_FACE_PARTS_ID_ALL_BASE+43)	unsupported */
/*#define S_FACE_PARTS_NOSE_ID_ALL_44		(S_FACE_PARTS_ID_ALL_BASE+44)	unsupported */
#define S_FACE_PARTS_R_LIP_ID_ALL_45		(S_FACE_PARTS_ID_ALL_BASE+45)
#define S_FACE_PARTS_U_LIP_ID_ALL_46		(S_FACE_PARTS_ID_ALL_BASE+46)
#define S_FACE_PARTS_U_LIP_ID_ALL_47		(S_FACE_PARTS_ID_ALL_BASE+47)
#define S_FACE_PARTS_U_LIP_ID_ALL_48		(S_FACE_PARTS_ID_ALL_BASE+48)
#define S_FACE_PARTS_L_LIP_ID_ALL_49		(S_FACE_PARTS_ID_ALL_BASE+49)
#define S_FACE_PARTS_D_LIP_ID_ALL_50		(S_FACE_PARTS_ID_ALL_BASE+50)
#define S_FACE_PARTS_D_LIP_ID_ALL_51		(S_FACE_PARTS_ID_ALL_BASE+51)
#define S_FACE_PARTS_D_LIP_ID_ALL_52		(S_FACE_PARTS_ID_ALL_BASE+52)
#define S_FACE_PARTS_UC_LIP_ID_ALL_53		(S_FACE_PARTS_ID_ALL_BASE+53)
#define S_FACE_PARTS_UC_LIP_ID_ALL_54		(S_FACE_PARTS_ID_ALL_BASE+54)
#define S_FACE_PARTS_UC_LIP_ID_ALL_55		(S_FACE_PARTS_ID_ALL_BASE+55)
#define S_FACE_PARTS_DC_LIP_ID_ALL_56		(S_FACE_PARTS_ID_ALL_BASE+56)
#define S_FACE_PARTS_DC_LIP_ID_ALL_57		(S_FACE_PARTS_ID_ALL_BASE+57)
#define S_FACE_PARTS_DC_LIP_ID_ALL_58		(S_FACE_PARTS_ID_ALL_BASE+58)

/* array index of all parts */
#define S_FACE_PARTS_FACE_ALL_00_INDEX		0
#define S_FACE_PARTS_FACE_ALL_01_INDEX		1
#define S_FACE_PARTS_FACE_ALL_02_INDEX		2
#define S_FACE_PARTS_FACE_ALL_03_INDEX		3
#define S_FACE_PARTS_FACE_ALL_04_INDEX		4
#define S_FACE_PARTS_FACE_ALL_05_INDEX		5
#define S_FACE_PARTS_FACE_ALL_06_INDEX		6
#define S_FACE_PARTS_FACE_ALL_07_INDEX		7
#define S_FACE_PARTS_FACE_ALL_08_INDEX		8
#define S_FACE_PARTS_FACE_ALL_09_INDEX		9
#define S_FACE_PARTS_FACE_ALL_10_INDEX		10
#define S_FACE_PARTS_FACE_ALL_11_INDEX		11
#define S_FACE_PARTS_R_EYEBROW_ALL_12_INDEX	12
#define S_FACE_PARTS_R_EYEBROW_ALL_13_INDEX	13
#define S_FACE_PARTS_R_EYEBROW_ALL_14_INDEX	14
#define S_FACE_PARTS_R_EYE_ALL_15_INDEX		15
#define S_FACE_PARTS_R_EYE_ALL_16_INDEX		16
#define S_FACE_PARTS_R_EYE_ALL_17_INDEX		17
#define S_FACE_PARTS_R_EYE_ALL_18_INDEX		18
#define S_FACE_PARTS_R_EYE_ALL_19_INDEX		19
#define S_FACE_PARTS_R_EYE_ALL_20_INDEX		20
#define S_FACE_PARTS_R_EYE_ALL_21_INDEX		21
#define S_FACE_PARTS_R_EYE_ALL_22_INDEX		22
#define S_FACE_PARTS_R_PUPIL_ALL_23_INDEX	23
#define S_FACE_PARTS_L_EYEBROW_ALL_24_INDEX	24
#define S_FACE_PARTS_L_EYEBROW_ALL_25_INDEX	25
#define S_FACE_PARTS_L_EYEBROW_ALL_26_INDEX	26
#define S_FACE_PARTS_L_EYE_ALL_27_INDEX		27
#define S_FACE_PARTS_L_EYE_ALL_28_INDEX		28
#define S_FACE_PARTS_L_EYE_ALL_29_INDEX		29
#define S_FACE_PARTS_L_EYE_ALL_30_INDEX		30
#define S_FACE_PARTS_L_EYE_ALL_31_INDEX		31
#define S_FACE_PARTS_L_EYE_ALL_32_INDEX		32
#define S_FACE_PARTS_L_EYE_ALL_33_INDEX		33
#define S_FACE_PARTS_L_EYE_ALL_34_INDEX		34
#define S_FACE_PARTS_L_PUPIL_ALL_35_INDEX	35
#define S_FACE_PARTS_NOSE_ALL_36_INDEX		36
#define S_FACE_PARTS_NOSE_ALL_37_INDEX		37
/*#define S_FACE_PARTS_NOSE_ALL_38_INDEX	unsupported */
/*#define S_FACE_PARTS_NOSE_ALL_39_INDEX	unsupported */
#define S_FACE_PARTS_NOSTRIL_ALL_40_INDEX	38
#define S_FACE_PARTS_NOSTRIL_ALL_41_INDEX	39
#define S_FACE_PARTS_NOSTRIL_ALL_42_INDEX	40
/*#define S_FACE_PARTS_NOSE_ALL_43_INDEX	unsupported */
/*#define S_FACE_PARTS_NOSE_ALL_44_INDEX	unsupported */
#define S_FACE_PARTS_R_LIP_ALL_45_INDEX		41
#define S_FACE_PARTS_U_LIP_ALL_46_INDEX		42
#define S_FACE_PARTS_U_LIP_ALL_47_INDEX		43
#define S_FACE_PARTS_U_LIP_ALL_48_INDEX		44
#define S_FACE_PARTS_L_LIP_ALL_49_INDEX		45
#define S_FACE_PARTS_D_LIP_ALL_50_INDEX		46
#define S_FACE_PARTS_D_LIP_ALL_51_INDEX		47
#define S_FACE_PARTS_D_LIP_ALL_52_INDEX		48
#define S_FACE_PARTS_UC_LIP_ALL_53_INDEX	49
#define S_FACE_PARTS_UC_LIP_ALL_54_INDEX	50
#define S_FACE_PARTS_UC_LIP_ALL_55_INDEX	51
#define S_FACE_PARTS_DC_LIP_ALL_56_INDEX	52
#define S_FACE_PARTS_DC_LIP_ALL_57_INDEX	53
#define S_FACE_PARTS_DC_LIP_ALL_58_INDEX	54

typedef struct SFacePartsResult {
	unsigned int	partsId;
	float			partsX;
	float			partsY;
	float			score;
} SFacePartsResult;

int sFaceParts(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFacePartsDictPtr			partsDictPtr,
	int								xScanStep,
	int								yScanStep,
	const SFaceDetectionResult		*detectedFace,
	SFacePartsResult				resultPartsArray[],
	int								resultPartsArraySize,
	int								*resultPartsNum,
	void							*workMemory,
	int								workMemorySize
);

int sFacePartsEx(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFacePartsDictPtr			partsDictPtr,
	const SFacePartsCheckDictPtr	partsCheckerDictPtr,
	int								xScanStep,
	int								yScanStep,
	const SFaceDetectionResult		*detectedFace,
	SFacePartsResult				resultPartsArray[],
	int								resultPartsArraySize,
	int								*resultPartsNum,
	void							*workMemory,
	int								workMemorySize
);

int sFaceAllParts(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFacePartsDictPtr			partsDictPtr,
	const SFaceShapeDictPtr			shapeDictPtr,
	int								xScanStep,
	int								yScanStep,
	const SFaceDetectionResult		*detectedFace,
	SFacePartsResult				resultPartsArray[],
	int								resultPartsArraySize,
	int								*resultPartsNum,
	void							*workMemory,
	int								workMemorySize
);

typedef struct SFacePose {
	float	faceRoll;
	float	facePitch;
	float	faceYaw;
} SFacePose;

typedef struct SFaceRegion {
	float	faceRegionX[4];
	float	faceRegionY[4];
} SFaceRegion;

int sFaceEstimatePoseRegion(
	int								width,
	int								height,
	const SFaceDetectionResult		*detectedFace,
	const SFacePartsResult			detectedPartsArray[],
	int								detectedPartsNum,
	SFacePose						*resultFacePose,
	SFaceRegion						*resultFaceRegion
);

int sFacePartsResultCheck(
	const SFaceDetectionResult		*detectedFace,
	const SFacePartsResult			detectedPartsArray[],
	int								detectedPartsNum,
	const SFacePartsCheckDictPtr	partsCheckerDictPtr,
	bool							*good
);

int sFacePartsGetWorkingMemorySize(
	int								width,
	int								height,
	int								rowstride,
	const SFacePartsDictPtr			partsDictPtr
);

int sFaceAllPartsGetWorkingMemorySize(
	int								width,
	int								height,
	int								rowstride,
	const SFacePartsDictPtr			partsDictPtr
);

/**********************************************************************
 Attribute Classify
**********************************************************************/

typedef void *SFaceAttribDictPtr;
typedef void *SFaceAgeDictPtr;

#if defined(_WIN64) || defined(__x86_64__) || defined(__arm64__) // 64bit
#define S_FACE_ATTRIB_SMILE_DICT		"ATTR_RSMILE_64_x64.atd"
#define S_FACE_ATTRIB_GAZE_DICT			"ATTR_GAZE_x64.atd"
#define S_FACE_ATTRIB_DICT				"ATTR_ALL01_x64.atd"
#define S_FACE_AGE_DICT					"ATTR_AGE_TREE_SAM1A_x64.agd"
#define S_FACE_ATTRIB_SMILE_DICT_SIZE	(25276)
#define S_FACE_ATTRIB_GAZE_DICT_SIZE	(20964)
#define S_FACE_ATTRIB_DICT_SIZE			(485332)
#define S_FACE_AGE_DICT_SIZE			(923028)
#else // 32bit
#define S_FACE_ATTRIB_SMILE_DICT		"ATTR_RSMILE_64.atd"
#define S_FACE_ATTRIB_GAZE_DICT			"ATTR_GAZE.atd"
#define S_FACE_ATTRIB_DICT				"ATTR_ALL01.atd"
#define S_FACE_AGE_DICT					"ATTR_AGE_TREE_SAM1A.agd"
#define S_FACE_ATTRIB_SMILE_DICT_SIZE	(25076)
#define S_FACE_ATTRIB_GAZE_DICT_SIZE	(20828)
#define S_FACE_ATTRIB_DICT_SIZE			(483788)
#define S_FACE_AGE_DICT_SIZE			(921324)
#endif // defined(_WIN64) || defined(__x86_64__)

#define S_FACE_ATTRIB_NUM_MAX			8
#define S_FACE_ATTRIB_ID_UNDEF			0
#define S_FACE_ATTRIB_ID_SMILE			1
#define S_FACE_ATTRIB_ID_REYEOPEN		2
#define S_FACE_ATTRIB_ID_LEYEOPEN		3
#define S_FACE_ATTRIB_ID_GENDER			4
#define S_FACE_ATTRIB_ID_ADULT			5
#define S_FACE_ATTRIB_ID_BABY			6
#define S_FACE_ATTRIB_ID_ELDER			7
#define S_FACE_ATTRIB_ID_GLASS			8

#define S_FACE_ATTRIB_ID_GAZE_YAW		9
#define S_FACE_ATTRIB_ID_GAZE_PITCH		10

#define S_FACE_ATTRIB_SMILE_INDEX		0
#define S_FACE_ATTRIB_REYEOPEN_INDEX	1
#define S_FACE_ATTRIB_LEYEOPEN_INDEX	2
#define S_FACE_ATTRIB_GENDER_INDEX		3
#define S_FACE_ATTRIB_ADULT_INDEX		4
#define S_FACE_ATTRIB_BABY_INDEX		5
#define S_FACE_ATTRIB_ELDER_INDEX		6
#define S_FACE_ATTRIB_GLASS_INDEX		7

#define S_FACE_ATTRIB_GAZE_YAW_INDEX	0
#define S_FACE_ATTRIB_GAZE_PITCH_INDEX	1

typedef struct SFaceAttribResult {
	unsigned int	attribId;
	float			score;
} SFaceAttribResult;

typedef struct SFaceAgeRangeResult {
	unsigned char maxAge;
	unsigned char minAge;
} SFaceAgeRangeResult;

#define S_FACE_AGE_BIN_SIZE	(81)
typedef struct SFaceAgeDistrData {
	float		score[S_FACE_AGE_BIN_SIZE];
	int			numIntegrated;
} SFaceAgeDistrData;

int sFaceAttribute(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceAttribDictPtr		attribDictPtr,
	const SFaceDetectionResult		*detectedFace,
	const SFacePartsResult			detectedPartsArray[],
	int								detectedPartsNum,
	SFaceAttribResult				resultAttribArray[],
	int								resultAttribArraySize,
	int								*resultAttribNum,
	void							*workMemory,
	int								workMemorySize
);

int sFaceAgeRangeEstimate(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceAgeDictPtr			ageDictPtr,
	const SFaceDetectionResult		*detectedFace,
	const SFacePartsResult			detectedPartsArray[],
	int								detectedPartsNum,
	SFaceAgeRangeResult				*resultAge,
	void							*workMemory,
	int								workMemorySize
);

int sFaceAgeRangeIntegrate(
	const SFaceAgeRangeResult		*ageRange,
	SFaceAgeDistrData				*ageDistrData,
	int								*ageResult
);

int sFaceAttributeGetWorkingMemorySize(
	int								width,
	int								height,
	int								rowstride,
	const SFaceAttribDictPtr		attribDictPtr
);

int sFaceAgeGetWorkingMemorySize(
	int								width,
	int								height,
	int								rowstride,
	const SFaceAgeDictPtr			ageDictPtr
);

/**********************************************************************
 Face Identify
**********************************************************************/

typedef void *SFaceIdentifyDictPtr;
typedef void *SFaceIdentifyLiteDictPtr;
typedef void *SFaceIdentifyExDictPtr;

#if defined(_WIN64) || defined(__x86_64__) || defined(__arm64__) // 64bit
#define S_FACE_IDENTIFY_DICT			"FI_CRECT_D4_S3_G2_x64.rid"
#define S_FACE_IDENTIFY_LITE_DICT		"FI_ECOC_M255_MA1_EA1_x64.eid"
#define S_FACE_IDENTIFY_EX_DICT			"FI_ECOCSVM_D1_EA6_EA3_x64.sid"
#define S_FACE_IDENTIFY_EX_DICT2		"FI_ECOCSVM_D1_EA6_2_x64.sid"
#define S_FACE_IDENTIFY_EX_DICT3		"FI_ECOCSVM_ID1_D1_x64.sid"
#define S_FACE_IDENTIFY_DICT_SIZE		(73844)
#define S_FACE_IDENTIFY_LITE_DICT_SIZE	(83376)
#define S_FACE_IDENTIFY_EX_DICT_SIZE	(206324)
#define S_FACE_IDENTIFY_EX_DICT2_SIZE	(190572)
#define S_FACE_IDENTIFY_EX_DICT3_SIZE	(703452)
#else // 32bit
#define S_FACE_IDENTIFY_DICT			"FI_CRECT_D4_S3_G2.rid"
#define S_FACE_IDENTIFY_LITE_DICT		"FI_ECOC_M255_MA1_EA1.eid"
#define S_FACE_IDENTIFY_EX_DICT			"FI_ECOCSVM_D1_EA6_EA3.sid"
#define S_FACE_IDENTIFY_EX_DICT2		"FI_ECOCSVM_D1_EA6_2.sid"
#define S_FACE_IDENTIFY_EX_DICT3		"FI_ECOCSVM_ID1_D1.sid"
#define S_FACE_IDENTIFY_DICT_SIZE		(73824)
#define S_FACE_IDENTIFY_LITE_DICT_SIZE	(83252)
#define S_FACE_IDENTIFY_EX_DICT_SIZE	(205912)
#define S_FACE_IDENTIFY_EX_DICT2_SIZE	(190272)
#define S_FACE_IDENTIFY_EX_DICT3_SIZE	(703152)
#endif // defined(_WIN64) || defined(__x86_64__)

#define S_FACE_IDENTIFY_FEATURE_SIZE	(4096)
typedef struct SFaceIdentifyFeature {
	unsigned char	data[S_FACE_IDENTIFY_FEATURE_SIZE];
} SFaceIdentifyFeature;

#define S_FACE_IDENTIFY_LITE_FEATURE_SIZE	(256)
typedef struct SFaceIdentifyLiteFeature {
	unsigned char	data[S_FACE_IDENTIFY_LITE_FEATURE_SIZE];
} SFaceIdentifyLiteFeature;

//#define S_FACE_IDENTIFY_EX_FEATURE_SIZE	(512)
#define S_FACE_IDENTIFY_EX_FEATURE_SIZE	(1024)
typedef struct SFaceIdentifyExFeature {
	unsigned char	data[S_FACE_IDENTIFY_EX_FEATURE_SIZE];
} SFaceIdentifyExFeature;

int sFaceIdentifyGetFeature(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceIdentifyDictPtr		identifyDictPtr,
	const SFaceDetectionResult		*detectedFace,
	const SFacePartsResult			detectedPartsArray[],
	int								detectedPartsNum,
	SFaceIdentifyFeature			*resultFeature,
	void							*workMemory,
	int								workMemorySize
);

int sFaceIdentifySimilarity(
	const SFaceIdentifyFeature		*extractedFeature,
	const SFaceIdentifyFeature		registeredFeatureArray[],
	int								registeredFeatureNum,
	const SFaceIdentifyDictPtr		identifyDictPtr,
	float							*maxScore,
	int								*maxScoreId,
	float							resultScoreArray[]
);

int sFaceIdentifyGetWorkingMemorySize(
	int								width,
	int								height,
	int								rowstride,
	const SFaceIdentifyDictPtr		identifyDictPtr
);

int sFaceIdentifyLiteGetFeature(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceIdentifyLiteDictPtr	identifyDictPtr,
	const SFaceDetectionResult		*detectedFace,
	const SFacePartsResult			detectedPartsArray[],
	int								detectedPartsNum,
	SFaceIdentifyLiteFeature		*resultFeature,
	void							*workMemory,
	int								workMemorySize
);

int sFaceIdentifyLiteSimilarity(
	const SFaceIdentifyLiteFeature	*extractedFeature,
	const SFaceIdentifyLiteFeature	registeredFeatureArray[],
	int								registeredFeatureNum,
	const SFaceIdentifyLiteDictPtr	identifyDictPtr,
	float							*maxScore,
	int								*maxScoreId,
	float							resultScoreArray[]
);

int sFaceIdentifyLiteGetWorkingMemorySize(
	int								width,
	int								height,
	int								rowstride,
	const SFaceIdentifyLiteDictPtr	identifyDictPtr
);

int sFaceIdentifyExGetFeature(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceIdentifyExDictPtr	identifyDictPtr,
	const SFaceDetectionResult		*detectedFace,
	const SFacePartsResult			detectedPartsArray[],
	int								detectedPartsNum,
	const SFaceAttribResult			detectedAttribArray[],
	int								detectedAttribNum,
	SFaceIdentifyExFeature			*resultFeature,
	void							*workMemory,
	int								workMemorySize
);

int sFaceIdentifyExSimilarity(
	const SFaceIdentifyExFeature	*extractedFeature,
	const SFaceIdentifyExFeature	registeredFeatureArray[],
	int								registeredFeatureNum,
	const SFaceIdentifyExDictPtr	identifyDictPtr,
	float							*maxScore,
	int								*maxScoreId,
	float							resultScoreArray[]
);

int sFaceIdentifyExGetWorkingMemorySize(
	int								width,
	int								height,
	int								rowstride,
	const SFaceIdentifyExDictPtr	identifyDictPtr
);

/**********************************************************************
 Face Shape Fitting/Tracking
**********************************************************************/
#if defined(_WIN64) || defined(__x86_64__) || defined(__arm64__) // 64bit
#define S_FACE_SHAPE_DICT_FRONTAL "face_shape_frontal_x64.shp"
#define S_FACE_SHAPE_DICT_FRONTAL_SIZE (20028)
#else // 32bit
#define S_FACE_SHAPE_DICT_FRONTAL "face_shape_frontal.shp"
#define S_FACE_SHAPE_DICT_FRONTAL_SIZE (19904)
#endif // defined(_WIN64) || defined(__x86_64__)

#define S_FACE_SHAPE_POINT_NUM_MAX (46)
#define S_FACE_SHAPE_POINT_NUM_FRONTAL (46)

#define S_FACE_SHAPE_MODEL_NUM_MAX (1)
#define S_FACE_SHAPE_MODEL_ID_FRONTAL (0)

#define S_FACE_SHAPE_SCORE_LOST_THRES_MIN (40)
#define S_FACE_SHAPE_SCORE_LOST_THRES_MAX (100)
#define S_FACE_SHAPE_SCORE_LOST_THRES_DEFAULT (55)

//Data
//----------------------------------
typedef void *SFaceShapeModelDictPtr;

typedef struct SFaceShapeResult {
	int modelID;

	int pointNum;
	float pointX[S_FACE_SHAPE_POINT_NUM_MAX];
	float pointY[S_FACE_SHAPE_POINT_NUM_MAX];
	int isLost[S_FACE_SHAPE_POINT_NUM_MAX];

	float fourPointX[S_FACE_PARTS_NUM_MAX];
	float fourPointY[S_FACE_PARTS_NUM_MAX];

	float rectCenterX;
	float rectCenterY;

	float rectWidth;
	float rectHeight;

	float faceRoll;
	float facePitch;
	float faceYaw;

	float score;

	unsigned char data[4096];
} SFaceShapeResult;

//Function
//----------------------------------
int sFaceShapeGetWorkingMemorySize(
	int								width,
	int								height,
	int								rowstride,
	const SFaceShapeModelDictPtr	shapeDictPtr,
	int								maxFaceWidth,
	int								maxFaceHeight,
	bool							isVideoInput
);

int sFaceShapeFit(
	const unsigned char				*imgPtr,
	int								width,
	int								height,
	int								rowstride,
	const SFaceShapeModelDictPtr	shapeDictPtr,
	SFaceShapeResult				*shape,
	float							lostThreshold,
	const SFaceDetectionResult		*detectedFace,
	const SFacePartsResult			detectedPartsArray[],
	int								detectedPartsNum,
	void							*workMemory,
	int								workMemorySize
);

int sFaceShapeTrack(
	const unsigned char				*imgPtrCur,
	const unsigned char				*imgPtrPrv,
	int								width,
	int								height,
	int								rowstride,
	const SFaceShapeModelDictPtr	shapeDictPtr,
	SFaceShapeResult				*shape,
	float							lostThreshold,
	void							*workMemory,
	int								workMemorySize
);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif	/* _SFACE_H_ */
