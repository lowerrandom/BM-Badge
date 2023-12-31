/*
This is intended to be a shared header file with common formats used in all the
MAGE() game functions. It contains common constants and structured used
throughout the MAGE() game code, and should be included in all future header
files as a way to consolidate redundant definitions and externs showing up all
over the code base.

Note: Do NOT try to include this in the "common.h" header file used outside of
the MAGE() game, as the use of c++ standard libraries will cause issues with
all of the old code used as the foundation of this badge.
*/

#ifndef _MAGE_DEFINES_H
#define _MAGE_DEFINES_H

#include "common.h"
#include "FrameBuffer.h"
#include <memory>
#include <utility>
#include <string>

//this is the path to the game.dat file on the SD card.
//if an SD card is inserted with game.dat in this location
//and its header hash is different from the one in the ROM chip
//it will automatically be loaded.
#define MAGE_GAME_DAT_PATH "MAGE/game.dat"

//this is the most unique entities that can be in any map.
#define MAX_ENTITIES_PER_MAP 64

//this is the map that will load at the start of the game:
#define DEFAULT_MAP 0

#define DEFAULT_PLAYER_NAME "Bub"

//this is used to note that no player entity was found within the
//entities loaded into the map
#define NO_PLAYER 255

//this is a value used to indicate that an action's calling entity was
//the map, and not actually one of the entities on the map.
#define MAGE_MAP_ENTITY 255

//this is a value used in the entityId in actions that refers to the
//entity the script is running on.
#define MAGE_ENTITY_SELF 254

//this is a value used in the entityId in actions that refers to the
//current playerEntityId for the MageGameControl object.
#define MAGE_ENTITY_PLAYER 253

//this is a value used in the entityId in actions that refers to the
//current playerEntityId for the MageGameControl object.
#define MAGE_ENTITY_PATH 65535

//these are the failover values that the game will use when an invalid hacked entity state is found:
#define MAGE_TILESET_FAILOVER_ID 0
#define MAGE_TILE_FAILOVER_ID 0
#define MAGE_ANIMATION_DURATION_FAILOVER_VALUE 0
#define MAGE_FRAME_COUNT_FAILOVER_VALUE 0
#define MAGE_RENDER_FLAGS_FAILOVER_VALUE 0

//these are used for setting player speed
//speed is in x/y units per update
#define MAGE_RUNNING_SPEED 200
#define MAGE_WALKING_SPEED 100

//these are the agreed-upon indices for entity_type entity animations
//If you import entities that don't use this convention, their animations may
//not work as intended.
#define MAGE_IDLE_ANIMATION_INDEX 0
#define MAGE_WALK_ANIMATION_INDEX 1
#define MAGE_ACTION_ANIMATION_INDEX 2

//this is how many bytes of arguments each script action has.
//all actions will have this many bytes, even if some are not used by a particular action
#define MAGE_NUM_ACTION_ARGS 7

#define MAGE_NUM_MEM_BUTTONS 4

//this is the number of chars that are used in the entity struct as part of the entity name
#define MAGE_ENTITY_NAME_LENGTH 12
#define MAGE_SAVE_FLAG_COUNT 2048
#define MAGE_SAVE_FLAG_BYTE_COUNT (MAGE_SAVE_FLAG_COUNT / 8)
#define MAGE_SCRIPT_VARIABLE_COUNT 256

//these variables are reserved script and action IDs used to indicate when a script or action should not do anything.
#define MAGE_NO_SCRIPT (-1)
#define MAGE_NO_MAP (-1)
#define MAGE_NO_WARP_STATE (-1)
#define MAGE_NULL_SCRIPT 0
#define MAGE_NULL_ACTION 0

//this is how many ms must have passed before the main game loop will run again:
//typical values:
//60fps: ~16ms
//30fps: ~33ms
//24fps: ~41ms
#ifdef DC801_DESKTOP
#define MAGE_MIN_MILLIS_BETWEEN_FRAMES (1000 / 24)
#endif
#ifdef DC801_EMBEDDED
#define MAGE_MIN_MILLIS_BETWEEN_FRAMES 90
#endif

// color palette corruption detection - requires much ram, can only be run on desktop
#ifdef DC801_DESKTOP
#define LOG_COLOR_PALETTE_CORRUPTION(value) MageGame->verifyAllColorPalettes((value));
#endif //DC801_DESKTOP
#ifdef DC801_EMBEDDED
#define LOG_COLOR_PALETTE_CORRUPTION(value) //(value)
#endif //DC801_EMBEDDED


//these are the types of scripts that can be on a map or entity:
typedef enum : uint8_t {
	ON_LOAD = 0,
	ON_TICK = 1,
	ON_INTERACT = 2,
	NUM_SCRIPT_TYPES
} MageScriptType;

typedef enum : uint8_t {
	SET = 0,
	ADD,
	SUB,
	DIV,
	MUL,
	MOD,
	RNG
} MageMutateOperation;

typedef enum : uint8_t {
	LT = 0,
	LTEQ,
	EQ,
	GTEQ,
	GT
} MageCheckComparison;

typedef enum : uint8_t {
	x = 12,
	y = 14,
	onInteractScriptId = 16,
	onTickScriptId = 18,
	primaryId = 20,
	secondaryId = 22,
	primaryIdType = 24,
	currentAnimation = 25,
	currentFrame = 26,
	direction = 27,
	hackableStateA = 28,
	hackableStateB = 29,
	hackableStateC = 30,
	hackableStateD = 31
} MageEntityField;

//this contains the possible options for an entity PrimaryIdType value.
typedef enum : uint8_t {
	TILESET = 0,
	ANIMATION = 1,
	ENTITY_TYPE = 2,
	NUM_PRIMARY_ID_TYPES
} MageEntityPrimaryIdType;

//this is the numerical translation for entity direction.
typedef enum : uint8_t{
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3,
	NUM_DIRECTIONS,
} MageEntityAnimationDirection;

#define RENDER_FLAGS_IS_GLITCHED_MASK		0b01111111
#define RENDER_FLAGS_IS_GLITCHED			0b10000000
#define RENDER_FLAGS_IS_DEBUG				0b01000000
#define RENDER_FLAGS_FLIP_X					0b00000100
#define RENDER_FLAGS_FLIP_Y					0b00000010
#define RENDER_FLAGS_FLIP_DIAG				0b00000001
#define RENDER_FLAGS_FLIP_MASK				0b00000111
#define RENDER_FLAGS_DIRECTION_MASK			0b00000011

//this contains all the possible script actions by actionTypeId value.
//these enum values match the data generated in the binary,
//so don't change any numbering unless you fix the binary generation as well.
//don't add more than 255 actions, or it will break the binary file.
//!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!
//IF YOU WANT TO ADD ACTIONS, YOU MUST UPDATE THE FOLLOWING FILES AS WELL!!
//	add enum for the MageScriptActionTypeId, right here in this file
//	add struct for the action arguments, lower in this file
//	add entry in `actionFunctions` the constructor of `Software/src/games/mage/mage_script_control.cpp`
//	add action handler function in `Software/src/games/mage/mage_script_control.(cpp/h)`
//	add action encoder function in `SD_Card/MAGE/editor/js/scripts.js`
//	add entry in actionNames array in `SD_Card/MAGE/editor/js/scripts.js`
//	add action_type enum in `SD_Card/MAGE/mage_dat.ksy`
typedef enum : uint8_t {
	NULL_ACTION = 0,
	CHECK_ENTITY_NAME,
	CHECK_ENTITY_X,
	CHECK_ENTITY_Y,
	CHECK_ENTITY_INTERACT_SCRIPT,
	CHECK_ENTITY_TICK_SCRIPT,
	CHECK_ENTITY_TYPE,
	CHECK_ENTITY_PRIMARY_ID,
	CHECK_ENTITY_SECONDARY_ID,
	CHECK_ENTITY_PRIMARY_ID_TYPE,
	CHECK_ENTITY_CURRENT_ANIMATION,
	CHECK_ENTITY_CURRENT_FRAME,
	CHECK_ENTITY_DIRECTION,
	CHECK_ENTITY_GLITCHED,
	CHECK_ENTITY_HACKABLE_STATE_A,
	CHECK_ENTITY_HACKABLE_STATE_B,
	CHECK_ENTITY_HACKABLE_STATE_C,
	CHECK_ENTITY_HACKABLE_STATE_D,
	CHECK_ENTITY_HACKABLE_STATE_A_U2,
	CHECK_ENTITY_HACKABLE_STATE_C_U2,
	CHECK_ENTITY_HACKABLE_STATE_A_U4,
	CHECK_ENTITY_PATH,
	CHECK_SAVE_FLAG,
	CHECK_IF_ENTITY_IS_IN_GEOMETRY,
	CHECK_FOR_BUTTON_PRESS,
	CHECK_FOR_BUTTON_STATE,
	CHECK_WARP_STATE,
	RUN_SCRIPT,
	BLOCKING_DELAY,
	NON_BLOCKING_DELAY,
	SET_ENTITY_NAME,
	SET_ENTITY_X,
	SET_ENTITY_Y,
	SET_ENTITY_INTERACT_SCRIPT,
	SET_ENTITY_TICK_SCRIPT,
	SET_ENTITY_TYPE,
	SET_ENTITY_PRIMARY_ID,
	SET_ENTITY_SECONDARY_ID,
	SET_ENTITY_PRIMARY_ID_TYPE,
	SET_ENTITY_CURRENT_ANIMATION,
	SET_ENTITY_CURRENT_FRAME,
	SET_ENTITY_DIRECTION,
	SET_ENTITY_DIRECTION_RELATIVE,
	SET_ENTITY_DIRECTION_TARGET_ENTITY,
	SET_ENTITY_DIRECTION_TARGET_GEOMETRY,
	SET_ENTITY_GLITCHED,
	SET_ENTITY_HACKABLE_STATE_A,
	SET_ENTITY_HACKABLE_STATE_B,
	SET_ENTITY_HACKABLE_STATE_C,
	SET_ENTITY_HACKABLE_STATE_D,
	SET_ENTITY_HACKABLE_STATE_A_U2,
	SET_ENTITY_HACKABLE_STATE_C_U2,
	SET_ENTITY_HACKABLE_STATE_A_U4,
	SET_ENTITY_PATH,
	SET_SAVE_FLAG,
	SET_PLAYER_CONTROL,
	SET_MAP_TICK_SCRIPT,
	SET_HEX_CURSOR_LOCATION,
	SET_WARP_STATE,
	SET_HEX_EDITOR_STATE,
	SET_HEX_EDITOR_DIALOG_MODE,
	SET_HEX_EDITOR_CONTROL,
	SET_HEX_EDITOR_CONTROL_CLIPBOARD,
	LOAD_MAP,
	SHOW_DIALOG,
	PLAY_ENTITY_ANIMATION,
	TELEPORT_ENTITY_TO_GEOMETRY,
	WALK_ENTITY_TO_GEOMETRY,
	WALK_ENTITY_ALONG_GEOMETRY,
	LOOP_ENTITY_ALONG_GEOMETRY,
	SET_CAMERA_TO_FOLLOW_ENTITY,
	TELEPORT_CAMERA_TO_GEOMETRY,
	PAN_CAMERA_TO_ENTITY,
	PAN_CAMERA_TO_GEOMETRY,
	PAN_CAMERA_ALONG_GEOMETRY,
	LOOP_CAMERA_ALONG_GEOMETRY,
	SET_SCREEN_SHAKE,
	SCREEN_FADE_OUT,
	SCREEN_FADE_IN,
	MUTATE_VARIABLE,
	MUTATE_VARIABLES,
	COPY_VARIABLE,
	CHECK_VARIABLE,
	CHECK_VARIABLES,
	SLOT_SAVE,
	SLOT_LOAD,
	SLOT_ERASE,
	//this tracks the number of actions we're at:
	NUM_ACTIONS
} MageScriptActionTypeId;

//these are the types of geometries that can be passed from the geometry data in ROM:
typedef enum : uint8_t{
	POINT = 0,
	POLYLINE = 1,
	POLYGON = 2,
	//this tracks how many different geometry types there are:
	NUM_GEOMETRIES
} MageGeometryTypeId;

//this is a structure to hold information about the currently executing scripts so they can resume
typedef struct{
	//indicated whether or not an active script is running on this MageScriptState
	bool scriptIsRunning;
	//the script Id to resume - this is a global mapLocalScriptId number value
	uint16_t mapLocalScriptId;
	//the action index to resume from - this is the action index for the script above, NOT a global actionTypeId.
	uint16_t actionOffset;
	//the number of loops until the next action in the script is to run
	uint16_t loopsToNextAction;
	//the total number of loops from the start of the action until the next action
	uint16_t totalLoopsToNextAction;
	//used to store state various geometry things
	Point pointA;
	Point pointB;
	float length;
	float lengthOfPreviousSegments;
	uint8_t currentSegmentIndex;
} MageScriptState;


//this is the structure of a MageEntity. All hackable info is contained within.
//the complete current entity state can be determined with only this info and
//the MageGame class interpreting the ROM data.
typedef struct {
	char name[MAGE_ENTITY_NAME_LENGTH]; // bob's club
	uint16_t x; // put the sheep back in the pen, rake in the lake
	uint16_t y;
	uint16_t onInteractScriptId;
	uint16_t onTickScriptId;
	uint16_t primaryId;
	uint16_t secondaryId;
	MageEntityPrimaryIdType primaryIdType;
	uint8_t currentAnimation;
	uint8_t currentFrame;
	MageEntityAnimationDirection direction;
	uint8_t hackableStateA;
	uint8_t hackableStateB;
	uint8_t hackableStateC;
	uint8_t hackableStateD;
} MageEntity;

typedef struct MageSaveGame {
	char identifier[ENGINE_ROM_IDENTIFIER_STRING_LENGTH] = ENGINE_ROM_SAVE_IDENTIFIER_STRING;
	uint32_t scenarioDataCRC32;
	uint32_t saveDataLength = sizeof(MageSaveGame);
	char name[MAGE_ENTITY_NAME_LENGTH] = DEFAULT_PLAYER_NAME;
	//this stores the byte offsets for the hex memory buttons:
	uint8_t memOffsets[MAGE_NUM_MEM_BUTTONS] = {
		MageEntityField::x,
		MageEntityField::y,
		MageEntityField::primaryId, // entityType
		MageEntityField::direction,
	};
	uint16_t currentMapId = DEFAULT_MAP;
	uint16_t warpState = MAGE_NO_WARP_STATE;
	uint8_t clipboard[sizeof(MageEntity)] = {0};
	uint8_t clipboardLength = 1;
	uint8_t paddingA;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t saveFlags[MAGE_SAVE_FLAG_BYTE_COUNT] = {0};
	uint16_t scriptVariables[MAGE_SCRIPT_VARIABLE_COUNT] = {0};
} MageSaveGame;

typedef struct {
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
} Rect;

//this is info needed to render entities that can be determined
//at run time from the MageEntity class info.
typedef struct {
	Rect hitBox;
	Rect interactBox;
	Point center;
	uint16_t currentFrameTicks;
	uint16_t tilesetId;
	uint16_t lastTilesetId;
	uint16_t tileId;
	uint32_t duration;
	uint16_t frameCount;
	uint8_t renderFlags;
	bool isInteracting;
} MageEntityRenderableData;

//below here are all the structures for interpreting the MageScriptAction binary data.
//Each action on the binary has 7 bytes of argument data, and these structs allow you
//to read all 7 bytes into the struct and get valid named arguments.
//for arguments larger than uint8_t, endian conversion will be needed.

typedef struct {
	uint8_t paddingA;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionNullAction;

typedef struct {
	uint16_t successScriptId;
	uint16_t stringId;
	uint8_t entityId;
	uint8_t expectedBoolValue;
	uint8_t paddingG;
} ActionCheckEntityName;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedValue;
	uint8_t entityId;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityX;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedValue;
	uint8_t entityId;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityY;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedScript;
	uint8_t entityId;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityInteractScript;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedScript;
	uint8_t entityId;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityTickScript;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedValue;
	uint8_t entityId;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityPrimaryId;

typedef struct {
	uint16_t successScriptId;
	uint16_t entityTypeId;
	uint8_t entityId;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityType;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedValue;
	uint8_t entityId;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntitySecondaryId;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedValue;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityPrimaryIdType;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedValue;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityCurrentAnimation;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedValue;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityCurrentFrame;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedValue;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityDirection;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedBool;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionCheckEntityGlitched;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedValue;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityHackableStateA;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedValue;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityHackableStateB;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedValue;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityHackableStateC;

typedef struct {
	uint16_t successScriptId;
	uint8_t entityId;
	uint8_t expectedValue;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckEntityHackableStateD;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedValue;
	uint8_t entityId;
	uint8_t expectedBool;
} ActionCheckEntityHackableStateAU2;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedValue;
	uint8_t entityId;
	uint8_t expectedBool;
} ActionCheckEntityHackableStateCU2;

typedef struct {
	uint32_t expectedValue;
	uint16_t successScriptId;
	uint8_t entityId;
} ActionCheckEntityHackableStateAU4;

typedef struct {
	uint16_t successScriptId;
	uint16_t expectedValue;
	uint8_t entityId;
	uint8_t expectedBool;
} ActionCheckEntityPath;

typedef struct {
	uint16_t successScriptId;
	uint16_t saveFlagOffset;
	uint8_t expectedBoolValue;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionCheckSaveFlag;

typedef struct {
	uint16_t successScriptId;
	uint16_t geometryId;
	uint8_t entityId;
	uint8_t expectedBoolValue;
	uint8_t paddingG;
} ActionCheckifEntityIsInGeometry;

typedef struct {
	uint16_t successScriptId;
	uint8_t buttonId; //KEYBOARD_KEY enum value
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionCheckForButtonPress;

typedef struct {
	uint16_t successScriptId;
	uint8_t buttonId; //KEYBOARD_KEY enum value
	uint8_t expectedBoolValue;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionCheckForButtonState;

typedef struct {
	uint16_t successScriptId;
	uint16_t stringId;
	uint8_t expectedBoolValue;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionCheckWarpState;

typedef struct {
	uint16_t scriptId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionRunScript;

typedef struct {
	uint32_t duration; //in ms
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionBlockingDelay;

typedef struct {
	uint32_t duration; //in ms
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionNonBlockingDelay;

typedef struct {
	uint16_t stringId;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityName;

typedef struct {
	uint16_t newValue;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityX;

typedef struct {
	uint16_t newValue;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityY;

typedef struct {
	uint16_t scriptId;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityInteractScript;

typedef struct {
	uint16_t scriptId;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityTickScript;

typedef struct {
	uint16_t entityTypeId;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityType;

typedef struct {
	uint16_t newValue;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityPrimaryId;

typedef struct {
	uint16_t newValue;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntitySecondaryId;

typedef struct {
	MageEntityPrimaryIdType newValue;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityPrimaryIdType;

typedef struct {
	uint8_t newValue;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityCurrentAnimation;

typedef struct {
	uint8_t newValue;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityCurrentFrame;

typedef struct {
	MageEntityAnimationDirection direction;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityDirection;

typedef struct {
	int8_t relativeDirection;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityDirectionRelative;

typedef struct {
	uint8_t targetEntityId;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityDirectionTargetEntity;

typedef struct {
	uint16_t targetGeometryId;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityDirectionTargetGeometry;

typedef struct {
	uint8_t entityId;
	uint8_t isGlitched;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityGlitched;

typedef struct {
	uint8_t newValue;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityHackableStateA;

typedef struct {
	uint8_t newValue;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityHackableStateB;

typedef struct {
	uint8_t newValue;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityHackableStateC;

typedef struct {
	uint8_t newValue;
	uint8_t entityId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityHackableStateD;

typedef struct {
	uint16_t newValue;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityHackableStateAU2;

typedef struct {
	uint16_t newValue;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityHackableStateCU2;

typedef struct {
	uint32_t newValue;
	uint8_t entityId;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityHackableStateAU4;

typedef struct {
	uint16_t newValue;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetEntityPath;

typedef struct {
	uint16_t saveFlagOffset;
	uint8_t newBoolValue;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetSaveFlag;

typedef struct {
	uint8_t playerHasControl;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetPlayerControl;

typedef struct {
	uint16_t scriptId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetMapTickScript;

typedef struct {
	uint16_t byteAddress;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetHexCursorLocation;

typedef struct {
	uint16_t stringId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetWarpState;

typedef struct {
	uint8_t state;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetHexEditorState;

typedef struct {
	uint8_t state;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetHexEditorDialogMode;

typedef struct {
	uint8_t playerHasHexEditorControl;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetHexEditorControl;

typedef struct {
	uint8_t playerHasHexEditorControlClipboard;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetHexEditorControlClipboard;

typedef struct {
	uint16_t mapId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionLoadMap;

typedef struct {
	uint16_t dialogId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionShowDialog;

typedef struct {
	uint8_t entityId;
	uint8_t animationId;
	uint8_t playCount;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionPlayEntityAnimation;

typedef struct {
	uint16_t geometryId;
	uint8_t entityId;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionTeleportEntityToGeometry;

typedef struct {
	uint32_t duration; //in ms
	uint16_t geometryId;
	uint8_t entityId;
} ActionWalkEntityToGeometry;

typedef struct {
	uint32_t duration; //in ms
	uint16_t geometryId;
	uint8_t entityId;
} ActionWalkEntityAlongGeometry;

typedef struct {
	uint32_t duration; //in ms
	uint16_t geometryId;
	uint8_t entityId;
} ActionLoopEntityAlongGeometry;

typedef struct {
	uint8_t entityId;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetCameraToFollowEntity;

typedef struct {
	uint16_t geometryId;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionTeleportCameraToGeometry;

typedef struct {
	uint32_t duration; //in ms
	uint8_t entityId;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionPanCameraToEntity;

typedef struct {
	uint32_t duration; //in ms
	uint16_t geometryId;
	uint8_t paddingG;
} ActionPanCameraToGeometry;

typedef struct {
	uint32_t duration; //in ms
	uint16_t geometryId;
	uint8_t paddingG;
} ActionPanCameraAlongGeometry;

typedef struct {
	uint32_t duration; //in ms
	uint16_t geometryId;
	uint8_t paddingG;
} ActionLoopCameraAlongGeometry;

typedef struct {
	uint16_t duration; //in ms
	uint16_t frequency;
	uint8_t amplitude;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSetScreenShake;

typedef struct {
	uint32_t duration; //in ms
	uint16_t color;
	uint8_t paddingG;
} ActionScreenFadeOut;

typedef struct {
	uint32_t duration; //in ms
	uint16_t color;
	uint8_t paddingG;
} ActionScreenFadeIn;

typedef struct {
	uint16_t value;
	uint8_t variableId;
	MageMutateOperation operation;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionMutateVariable;

typedef struct {
	uint8_t variableId;
	uint8_t sourceId;
	MageMutateOperation operation;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionMutateVariables;

typedef struct {
	uint8_t variableId;
	uint8_t entityId;
	MageEntityField field;
	uint8_t inbound;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionCopyVariable;

typedef struct {
	uint16_t successScriptId;
	uint16_t value;
	uint8_t variableId;
	MageCheckComparison comparison;
	uint8_t expectedBool;
} ActionCheckVariable;

typedef struct {
	uint16_t successScriptId;
	uint8_t variableId;
	uint8_t sourceId;
	MageCheckComparison comparison;
	uint8_t expectedBool;
	uint8_t paddingG;
} ActionCheckVariables;

typedef struct {
	uint8_t paddingA;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSlotSave;

typedef struct {
	uint8_t slotIndex;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSlotLoad;

typedef struct {
	uint8_t slotIndex;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t paddingD;
	uint8_t paddingE;
	uint8_t paddingF;
	uint8_t paddingG;
} ActionSlotErase;

#endif //_MAGE_DEFINES_H
