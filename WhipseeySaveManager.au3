;*****************************************
;WhipseeySaveManager.au3 by StillGreen-san
;Created with ISN AutoIt Studio v. 1.11
;*****************************************

#include "Forms\GUI.isf"

;VARS
Global $defaultSaveDir = @UserProfileDir &  "\AppData\Local\Whipseey\savedata"
Global $saveFile =  $defaultSaveDir &  "\whipseey.sav"
Global $file1Controls[] = ["file1", $file1Boss, $file1Enemies, $file1Castle, $file1Moon, $file1Snow, $file1Desert, $file1Forest, $file1Beach, $file1Ending, $file1Intro, $file1Lives, $file1Gems]
Global $file2Controls[] = ["file2", $file2Boss, $file2Enemies, $file2Castle, $file2Moon, $file2Snow, $file2Desert, $file2Forest, $file2Beach, $file2Ending, $file2Intro, $file2Lives, $file2Gems]
Global $file3Controls[] = ["file3", $file3Boss, $file3Enemies, $file3Castle, $file3Moon, $file3Snow, $file3Desert, $file3Forest, $file3Beach, $file3Ending, $file3Intro, $file3Lives, $file3Gems]
Global Enum $LEVEL_CASTLE = 5, $LEVEL_MOON = 4, $LEVEL_SNOW = 3, $LEVEL_DESERT = 2, $LEVEL_FOREST = 1, $LEVEL_BEACH = 0
Global Enum $CONTROL_NAME = 0, $CONTROL_BOSS = 1, $CONTROL_ENEMIES = 2, $CONTROL_CASTLE = 3, $CONTROL_MOON = 4, $CONTROL_SNOW = 5, $CONTROL_DESERT = 6, $CONTROL_FOREST = 7, $CONTROL_BEACH = 8, $CONTROL_ENDING = 9, $CONTROL_INTRO = 10, $CONTROL_LIVES = 11, $CONTROL_GEMS = 12
Global Enum $FILE_BOSS = 1, $FILE_ENEMIES = 2, $FILE_CASTLE = 3, $FILE_MOON = 4, $FILE_SNOW = 5, $FILE_DESERT = 6, $FILE_FOREST = 7, $FILE_ENDING = 8, $FILE_INTRO = 9, $FILE_LIVES = 10, $FILE_GEMS = 11
Global Enum $INI_KEY = 0, $INI_VALUE = 1, $INI_CASTLE = 32, $INI_MOON = 16, $INI_SNOW = 8, $INI_DESERT = 4, $INI_FOREST = 2

;GUI INIT
Opt("GUIOnEventMode", 1)

If FileExists($defaultSaveDir) Then
	GUICtrlSetData($path, $defaultSaveDir)
	_LoadSave()
Else
	;deactivate all file controls
EndIf

;GUI HANDLING
GUISetState(@SW_SHOW, $GUI)

While 1
	Sleep(100) ;Idle
WEnd

;GUI FUNCTIONS
Func _LoadFile(ByRef $fileControls)
	Local $fileData = IniReadSection($saveFile, $fileControls[$CONTROL_NAME])
	GUICtrlSetData($fileControls[$CONTROL_BOSS], _IniToInt($fileData[$FILE_BOSS][$INI_VALUE]))
	GUICtrlSetData($fileControls[$CONTROL_ENEMIES], _IniToInt($fileData[$FILE_ENEMIES][$INI_VALUE]))

	Local $levelStates[] =  [$GUI_UNCHECKED, $GUI_UNCHECKED, $GUI_UNCHECKED, $GUI_UNCHECKED, $GUI_UNCHECKED, $GUI_UNCHECKED]
	If $fileData[$FILE_CASTLE][$INI_VALUE] = '"32.000000"' Then
		$levelStates[$LEVEL_CASTLE] = $GUI_CHECKED
	ElseIf $fileData[$FILE_MOON][$INI_VALUE] = '"16.000000"' Then
		$levelStates[$LEVEL_MOON] = $GUI_CHECKED
	ElseIf $fileData[$FILE_SNOW][$INI_VALUE] = '"8.000000"' Then
		$levelStates[$LEVEL_SNOW] = $GUI_CHECKED
	ElseIf $fileData[$FILE_DESERT][$INI_VALUE] = '"4.000000"' Then
		$levelStates[$LEVEL_DESERT] = $GUI_CHECKED
	ElseIf $fileData[$FILE_FOREST][$INI_VALUE] = '"2.000000"' Then
		$levelStates[$LEVEL_FOREST] = $GUI_CHECKED
	Else
		$levelStates[$LEVEL_BEACH] = $GUI_CHECKED
	EndIf
	GUICtrlSetState($fileControls[$CONTROL_CASTLE], $levelStates[$LEVEL_CASTLE])
	GUICtrlSetState($fileControls[$CONTROL_MOON], $levelStates[$LEVEL_MOON])
	GUICtrlSetState($fileControls[$CONTROL_SNOW], $levelStates[$LEVEL_SNOW])
	GUICtrlSetState($fileControls[$CONTROL_DESERT], $levelStates[$LEVEL_DESERT])
	GUICtrlSetState($fileControls[$CONTROL_FOREST], $levelStates[$LEVEL_FOREST])
	GUICtrlSetState($fileControls[$CONTROL_BEACH], $levelStates[$LEVEL_BEACH])

	GUICtrlSetState($fileControls[$CONTROL_ENDING], _IniToCheckState($fileData[$FILE_ENDING][$INI_VALUE]))
	GUICtrlSetState($fileControls[$CONTROL_INTRO], _IniToCheckState($fileData[$FILE_INTRO][$INI_VALUE]))
	GUICtrlSetData($fileControls[$CONTROL_LIVES], _IniToInt($fileData[$FILE_LIVES][$INI_VALUE]))
	GUICtrlSetData($fileControls[$CONTROL_GEMS], _IniToInt($fileData[$FILE_GEMS][$INI_VALUE]))
EndFunc
Func _LoadSave()
	_LoadFile($file3Controls)
	_LoadFile($file2Controls)
	_LoadFile($file1Controls)
EndFunc

Func _SaveFile(ByRef $fileControls)
	Local $fileData = IniReadSection($saveFile, $fileControls[0])
	Local $levelStates[] = ['"0.000000"', '"2.000000"', '"4.000000"', '"8.000000"', '"16.000000"', '"32.000000"']
	If GUICtrlRead($fileControls[$CONTROL_CASTLE]) = $GUI_UNCHECKED Then
		$levelStates[$LEVEL_CASTLE] = $levelStates[0]
		If GUICtrlRead($fileControls[$CONTROL_MOON]) = $GUI_UNCHECKED Then
			$levelStates[$LEVEL_MOON] = $levelStates[0]
			If GUICtrlRead($fileControls[$CONTROL_SNOW]) = $GUI_UNCHECKED Then
				$levelStates[$LEVEL_SNOW] = $levelStates[0]
				If GUICtrlRead($fileControls[$CONTROL_DESERT]) = $GUI_UNCHECKED Then
					$levelStates[$LEVEL_DESERT] = $levelStates[0]
					If GUICtrlRead($fileControls[$CONTROL_FOREST]) = $GUI_UNCHECKED Then
						$levelStates[$LEVEL_FOREST] = $levelStates[0]
					EndIf
				EndIf
			EndIf
		EndIf
	EndIf
	$fileData[$FILE_CASTLE][$INI_VALUE] = $levelStates[$LEVEL_CASTLE]
	$fileData[$FILE_MOON][$INI_VALUE] = $levelStates[$LEVEL_MOON]
	$fileData[$FILE_SNOW][$INI_VALUE] = $levelStates[$LEVEL_SNOW]
	$fileData[$FILE_DESERT][$INI_VALUE] = $levelStates[$LEVEL_DESERT]
	$fileData[$FILE_FOREST][$INI_VALUE] = $levelStates[$LEVEL_FOREST]
	
	$fileData[$FILE_ENDING][$INI_VALUE] = _CheckStateToIni(GUICtrlRead($fileControls[$CONTROL_ENDING]))
	$fileData[$FILE_INTRO][$INI_VALUE] = _CheckStateToIni(GUICtrlRead($fileControls[$CONTROL_INTRO]))
	$fileData[$FILE_LIVES][$INI_VALUE] = _IntToIni(GUICtrlRead($fileControls[$CONTROL_LIVES]))
	$fileData[$FILE_GEMS][$INI_VALUE] = _IntToIni(GUICtrlRead($fileControls[$CONTROL_GEMS]))
	
	IniWriteSection($saveFile, $fileControls[$CONTROL_NAME], $fileData)
EndFunc

Func _File1Changed()
	_SaveFile($file1Controls)
EndFunc
Func _File2Changed()
	_SaveFile($file2Controls)
EndFunc
Func _File3Changed()
	_SaveFile($file3Controls)
EndFunc

;HELPER FUNCTIONS
Func _IniToCheckState(ByRef $data)
	If $data = '"1.000000"' Then
		return $GUI_CHECKED
	EndIf
	return $GUI_UNCHECKED
EndFunc
Func _CheckStateToIni($state)
	If $state = $GUI_CHECKED Then
		return '"1.000000"'
	EndIf
	return '"0.000000"'
EndFunc

Func _IniToRadioState(ByRef $data)
	If $data = "1.000000" Then
		return $GUI_CHECKED
	EndIf
	return $GUI_UNCHECKED
EndFunc

Func _IniToInt($data)
	$data = StringReplace($data, '"', "")
	return Int($data)
EndFunc
Func _IntToIni($int)
	return String('"' & $int & '.000000"')
EndFunc

Func _Exit()
	Exit
EndFunc
