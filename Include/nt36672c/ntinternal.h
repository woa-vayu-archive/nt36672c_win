/*++
	Copyright (c) Microsoft Corporation. All Rights Reserved.
	Sample code. Dealpoint ID #843729.

	Module Name:

		ftinternal.h

	Abstract:

		Contains common types and defintions used internally
		by the multi touch screen driver.

	Environment:

		Kernel mode

	Revision History:

--*/

#pragma once

#include <wdm.h>
#include <wdf.h>
#include <controller.h>
#include <resolutions.h>
#include <Cross Platform Shim/bitops.h>
#include <Cross Platform Shim/hweight.h>
#include <report.h>

// Ignore warning C4152: nonstandard extension, function/data pointer conversion in expression
#pragma warning (disable : 4152)

// Ignore warning C4201: nonstandard extension used : nameless struct/union
#pragma warning (disable : 4201)

// Ignore warning C4201: nonstandard extension used : bit field types other than in
#pragma warning (disable : 4214)

// Ignore warning C4324: 'xxx' : structure was padded due to __declspec(align())
#pragma warning (disable : 4324)


typedef enum _NVT_GESTURE_ID
{
      NVT_GESTURE_NONE = 0x00,
      NVT_GESTURE_MOVE_UP = 0x10,
      NVT_GESTURE_MOVE_RIGHT = 0x14,
      NVT_GESTURE_MOVE_DOWN = 0x18,
      NVT_GESTURE_MOVE_LEFT = 0x1C,
      NVT_GESTURE_ZOOM_IN = 0x48,
      NVT_GESTURE_ZOOM_OUT = 0x49
} NVT_GESTURE_ID;

typedef enum _NVT_DEVICE_MODE
{
      NVT_MODE_WORKING = 0,
      NVT_MODE_TEST = 4
} NVT_DEVICE_MODE;

typedef enum _NVT_EVENT_FLAG
{
      NVT_EVENT_PRESS_DOWN = 0,
      NVT_EVENT_LIFT_UP = 1,
      NVT_EVENT_CONTACT = 2,
      NVT_EVENT_NONE = 3
} NVT_EVENT_FLAG;

typedef struct _NVT_TOUCH_DATA
{
	BYTE PositionX_High : 4;
	BYTE Reserved0 : 2;
	BYTE EventFlag : 2;

	BYTE PositionX_Low;

	BYTE PositionY_High : 4;
	BYTE TouchId : 4;

	BYTE PositionY_Low;

	BYTE TouchWeight;

	BYTE Reserved1 : 4;
	BYTE TouchArea : 4;
} NVT_TOUCH_DATA, * PNVT_TOUCH_DATA;

typedef struct _NVT_EVENT_DATA
{
	BYTE Reserved0 : 4;
	BYTE DeviceMode : 3;
	BYTE Reserved1 : 1;

	BYTE GestureId;

	BYTE NumberOfTouchPoints : 4;
	BYTE Reserved2 : 4;

	NVT_TOUCH_DATA TouchData[6];
} NVT_EVENT_DATA, * PNVT_EVENT_DATA;

#define TOUCH_POOL_TAG_F12              (ULONG)'21oT'

//
// Logical structure for getting registry config settings
//
typedef struct _RM4_F01_CTRL_REGISTERS_LOGICAL
{
	UINT32 SleepMode;
	UINT32 NoSleep;
	UINT32 ReportRate;
	UINT32 Configured;
	UINT32 InterruptEnable;
	UINT32 DozeInterval;
	UINT32 DozeThreshold;
	UINT32 DozeHoldoff;
} NT36X_F01_CTRL_REGISTERS_LOGICAL;

#define NT36X_MILLISECONDS_TO_TENTH_MILLISECONDS(n) n/10
#define NT36X_SECONDS_TO_HALF_SECONDS(n) 2*n

//
// Function $11 - 2-D Touch Sensor
//

//
// Logical structure for getting registry config settings
//
typedef struct _NT36X_F11_CTRL_REGISTERS_LOGICAL
{
	UINT32 ReportingMode;
	UINT32 AbsPosFilt;
	UINT32 RelPosFilt;
	UINT32 RelBallistics;
	UINT32 Dribble;
	UINT32 PalmDetectThreshold;
	UINT32 MotionSensitivity;
	UINT32 ManTrackEn;
	UINT32 ManTrackedFinger;
	UINT32 DeltaXPosThreshold;
	UINT32 DeltaYPosThreshold;
	UINT32 Velocity;
	UINT32 Acceleration;
	UINT32 SensorMaxXPos;
	UINT32 SensorMaxYPos;
	UINT32 ZTouchThreshold;
	UINT32 ZHysteresis;
	UINT32 SmallZThreshold;
	UINT32 SmallZScaleFactor;
	UINT32 LargeZScaleFactor;
	UINT32 AlgorithmSelection;
	UINT32 WxScaleFactor;
	UINT32 WxOffset;
	UINT32 WyScaleFactor;
	UINT32 WyOffset;
	UINT32 XPitch;
	UINT32 YPitch;
	UINT32 FingerWidthX;
	UINT32 FingerWidthY;
	UINT32 ReportMeasuredSize;
	UINT32 SegmentationSensitivity;
	UINT32 XClipLo;
	UINT32 XClipHi;
	UINT32 YClipLo;
	UINT32 YClipHi;
	UINT32 MinFingerSeparation;
	UINT32 MaxFingerMovement;
} NT36X_F11_CTRL_REGISTERS_LOGICAL;

//
// Driver structures
//

typedef struct _NT36X_CONFIGURATION
{
	NT36X_F01_CTRL_REGISTERS_LOGICAL DeviceSettings;
	NT36X_F11_CTRL_REGISTERS_LOGICAL TouchSettings;
	UINT32 PepRemovesVoltageInD3;
} NT36X_CONFIGURATION;

typedef struct _NT36X_CONTROLLER_CONTEXT
{
	WDFDEVICE FxDevice;
	WDFWAITLOCK ControllerLock;

	//
	// Power state
	//
	DEVICE_POWER_STATE DevicePowerState;

	//
	// Register configuration programmed to chip
	//
	TOUCH_SCREEN_SETTINGS TouchSettings;
	NT36X_CONFIGURATION Config;

	UCHAR Data1Offset;

	BYTE MaxFingers;

    int HidQueueCount;
} NT36X_CONTROLLER_CONTEXT;

NTSTATUS
NT36XBuildFunctionsTable(
	IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
	IN SPB_CONTEXT* SpbContext
);

NTSTATUS
NT36XChangePage(
	IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
	IN SPB_CONTEXT* SpbContext,
	IN int DesiredPage
);

NTSTATUS
NT36XConfigureFunctions(
	IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
	IN SPB_CONTEXT* SpbContext
);

NTSTATUS
NT36XServiceInterrupts(
	IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
	IN SPB_CONTEXT* SpbContext,
	IN PREPORT_CONTEXT ReportContext
);

#define NT36X_F01_DEVICE_CONTROL_SLEEP_MODE_OPERATING  0
#define NT36X_F01_DEVICE_CONTROL_SLEEP_MODE_SLEEPING   1

#pragma pack(push)
#pragma pack(1)
typedef enum _NT36X_F12_REPORTING_FLAGS
{
	NT36X_F12_REPORTING_CONTINUOUS_MODE = 0,
	NT36X_F12_REPORTING_REDUCED_MODE = 1,
	NT36X_F12_REPORTING_WAKEUP_GESTURE_MODE = 2,
} NT36X_F12_REPORTING_FLAGS;
#pragma pack(pop)

NTSTATUS
NT36XSetReportingFlagsF12(
    IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR NewMode,
    OUT UCHAR* OldMode
);

NTSTATUS
NT36XChangeChargerConnectedState(
    IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR ChargerConnectedState
);

NTSTATUS
NT36XChangeSleepState(
    IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR SleepState
);

NTSTATUS
NT36XGetFirmwareVersion(
    IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext
);

NTSTATUS
NT36XCheckInterrupts(
    IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN ULONG* InterruptStatus
);

NTSTATUS
NT36XConfigureInterruptEnable(
    IN NT36X_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext
);

