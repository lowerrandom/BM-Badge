/*
This class contains all the code related to the hex editor hacking interface.
*/
#ifndef _MAGE_SCRIPT_CONTROL_H
#define _MAGE_SCRIPT_CONTROL_H

#include "mage_defines.h"
#include "mage.h"
#include "mage_game_control.h"
#include "mage_hex.h"

#define SCRIPT_NAME_LENGTH 32

//this is a class designed to handle all the scripting for the MAGE() game
//it is designed to work in tandem with a MageGameControl object and a
//MageHex object to effect that state of the game.

class MageScriptControl
{
	private:
		//the mapLocalJumpScript variable is used by some actions to indicate that a script should
		//end and immediately begin running a new script.
		//it should be set to MAGE_NO_SCRIPT unless a new script should be run immediately.
		int32_t mapLocalJumpScript;

		//this is a variable that tracks which entity called an action. 
		//If the action was called by the map, the value will be MAGE_MAP_ENTITY.
		//most actions will not do anything if an action that uses MAGE_ENTITY_SELF is called from the map's scripts.
		uint8_t currentEntityId;

		//this tracks which type of script called processScript() so that when a call script
		//calls a new script, the original entity can be updated to match.
		MageScriptType currentScriptType;

		//variables for tracking suspended script states:
		MageScriptState mapLoadResumeState;
		MageScriptState mapTickResumeState;
		MageScriptState entityInteractResumeStates[MAX_ENTITIES_PER_MAP];
		MageScriptState entityTickResumeStates[MAX_ENTITIES_PER_MAP];

		//typedef for the array of function pointers to script action functions:
		typedef void(MageScriptControl::*ActionFunctionPointer)(uint8_t * args, MageScriptState * resumeStateStruct);

		//the actual array of action functions:
		ActionFunctionPointer actionFunctions[MageScriptActionTypeId::NUM_ACTIONS];

		//this will process a script based on the state of the resumeStateStruct passed to it.
		//it should only be called from the 
		void processScript(MageScriptState * resumeStateStruct, uint8_t mapLocalEntityId, MageScriptType scriptType);

		//this will run through the actions in a script from the state stores in resumeState
		//if a mapLocalJumpScript is called by an action, it will return without processing any further actions.
		void processActionQueue(MageScriptState * resumeStateStruct);

		//this will get action arguments from ROM memory and call
		//a function based on the ActionTypeId 
		void runAction(uint32_t argumentMemoryAddress, MageScriptState * resumeStateStruct);

		//this allows an I+C action to set the calling map or entity script to match the new script.
		void setEntityScript(uint16_t mapLocalScriptId, uint8_t entityId, uint8_t scriptType);

	uint16_t getUsefulGeometryIndexFromActionGeometryId(uint16_t geometryId, MageEntity *entity);

		//the functions below here are the action functions. These are going to be
		//called directly by scripts, and preform their actions based on arguments read from ROM

		//each action has an action logic type, depending on how it will need to interact with the rest of the game loop:
		//I   = instant, will execute and immediately proceed to the next action
		//NB  = non-blocking, will use loopsToNextAction and totalLoopsToNextAction to run the action until it is completed
		//NBC = non-blocking continuous, will never proceed to another action, and will begin the same action again forever until the mapLocalScriptId is changed
		//B   = blocking, will pause all game actions until complete.
		//I+C = scripts that may call another mapLocalScriptId, discarding any actions that occur after them in the current script
		//I've noted the blocking state of actions below on the line above the action:

		//Action Logic Type: I
		void nullAction(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityName(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityX(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityY(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityInteractScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityTickScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityType(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityPrimaryId(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntitySecondaryId(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityPrimaryIdType(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityCurrentAnimation(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityCurrentFrame(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityDirection(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityGlitched(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityHackableStateA(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityHackableStateB(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityHackableStateC(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityHackableStateD(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityHackableStateAU2(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityHackableStateCU2(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityHackableStateAU4(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkEntityPath(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkSaveFlag(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkIfEntityIsInGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkForButtonPress(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkForButtonState(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkWarpState(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void runScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: B
		void blockingDelay(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void nonBlockingDelay(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityName(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityX(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityY(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityInteractScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityTickScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityType(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityPrimaryId(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntitySecondaryId(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityPrimaryIdType(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityCurrentAnimation(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityCurrentFrame(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityDirection(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityDirectionRelative(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityDirectionTargetEntity(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityDirectionTargetGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityGlitched(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityHackableStateA(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityHackableStateB(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityHackableStateC(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityHackableStateD(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityHackableStateAU2(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityHackableStateCU2(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityHackableStateAU4(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setEntityPath(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setSaveFlag(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setPlayerControl(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setMapTickScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setHexCursorLocation(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setWarpState(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setHexEditorState(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setHexEditorDialogMode(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setHexEditorControl(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setHexEditorControlClipboard(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I (loadMap will stop all other scripts immediately, loading a new map with new scripts)
		void loadMap(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB (note showDialog will render over the main game loop and not return player control until the dialog is concluded)
		void showDialog(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void playEntityAnimation(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void teleportEntityToGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void walkEntityToGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void walkEntityAlongGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NBC
		void loopEntityAlongGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void setCameraToFollowEntity(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void teleportCameraToGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void panCameraToEntity(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void panCameraToGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void panCameraAlongGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NBC
		void loopCameraAlongGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void setScreenShake(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void screenFadeOut(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void screenFadeIn(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void mutateVariable(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void mutateVariables(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: NB
		void copyVariable(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkVariable(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I+C
		void checkVariables(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void slotSave(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void slotLoad(uint8_t * args, MageScriptState * resumeStateStruct);
		//Action Logic Type: I
		void slotErase(uint8_t * args, MageScriptState * resumeStateStruct);
	public:
		//this is a global that holds the amount of millis that a blocking delay will
		//prevent the main loop from continuing for. It is set by the blockingDelay() action.
		uint32_t blockingDelayTime;

		//this is used by the loadMap action to indicate when a new map needs to be loaded.
		//when set to a value other than MAGE_NO_MAP, it will cause all scripts to stop and 
		//the new map will be loaded at the beginning of the next tick
		int32_t mapLoadId;

		MageScriptControl();

		//returns size in RAM of all reserved class variables.
		uint32_t size() const;

		//this resets the values of a MageScriptState struct to default values.
		//you need to provide a mapLocalScriptId, and the state of the scriptIsRunning variable
		//the actionId, and duration variables are always reset to 0 on an init.
		void initScriptState(
			MageScriptState * resumeStateStruct,
			uint16_t mapLocalScriptId,
			bool scriptIsRunning
		);

		//these functions return the specified MageScriptState struct:
		MageScriptState* getMapLoadResumeState();
		MageScriptState* getMapTickResumeState();
		MageScriptState* getEntityInteractResumeState(uint8_t index);
		MageScriptState* getEntityTickResumeState(uint8_t index);
		
		Point offsetPointRelativeToEntityCenter(
			const MageEntityRenderableData *renderable,
			const MageEntity *entity,
			const Point *geometryPoint
		) const;
		MageEntityAnimationDirection getRelativeDirection(
			const Point &pointA,
			const Point &pointB
		) const;
		void setEntityPositionToPoint(
			MageEntity *entity,
			const Point &point
		) const;
		float getProgressOfAction(const MageScriptState *resumeStateStruct) const;
		float manageProgressOfAction(
			MageScriptState *resumeStateStruct,
			uint32_t duration
		) const;
		uint16_t getLoopableGeometryPointIndex(
			MageGeometry *geometry,
			uint8_t index
		);
		uint16_t getLoopableGeometrySegmentIndex(
			MageGeometry *geometry,
			uint8_t segmentIndex
		);
		void setResumeStatePointsAndEntityDirection(
			MageScriptState *resumeStateStruct,
			MageEntityRenderableData *renderable,
			MageEntity *entity,
			MageGeometry *geometry,
			uint16_t pointAIndex,
			uint16_t pointBIndex
		) const;
		void initializeEntityGeometryPath(
			MageScriptState *resumeStateStruct,
			MageEntityRenderableData *renderable,
			MageEntity *entity,
			MageGeometry *geometry
		);

		//these functions will call the appropriate script processing for their script type:
		void handleMapOnLoadScript(bool isFirstRun);
		void handleMapOnTickScript();
		void handleEntityOnTickScript(uint8_t filteredEntityId);
		void handleEntityOnInteractScript(uint8_t filteredEntityId);

		int16_t getUsefulEntityIndexFromActionEntityId(
			uint8_t entityId,
			int16_t callingEntityId
		);

		void mutate(
			MageMutateOperation operation,
			uint16_t *destination,
			uint16_t value
		) const;

		bool compare(
			MageCheckComparison comparison,
			uint16_t a,
			uint16_t b
		) const;

		static bool getButtonStateFromButtonArray(
			uint8_t buttonId,
			ButtonStates *buttonStates
		) ;
}; //MageScriptControl

#endif //_MAGE_SCRIPT_CONTROL_H
