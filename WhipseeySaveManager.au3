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
	Local $fileData = IniReadSection($saveFile, $fileControls[0])
	GUICtrlSetData($fileControls[1], _IniToInt($fileData[1][1]))
	GUICtrlSetData($fileControls[2], _IniToInt($fileData[2][1]))

	Local $levelStates[] =  [$GUI_UNCHECKED, $GUI_UNCHECKED, $GUI_UNCHECKED, $GUI_UNCHECKED, $GUI_UNCHECKED, $GUI_UNCHECKED]
	If $fileData[3][1] = '"32.000000"' Then
		$levelStates[5] = $GUI_CHECKED
	ElseIf $fileData[4][1] = '"16.000000"' Then
		$levelStates[4] = $GUI_CHECKED
	ElseIf $fileData[5][1] = '"8.000000"' Then
		$levelStates[3] = $GUI_CHECKED
	ElseIf $fileData[6][1] = '"4.000000"' Then
		$levelStates[2] = $GUI_CHECKED
	ElseIf $fileData[7][1] = '"2.000000"' Then
		$levelStates[1] = $GUI_CHECKED
	Else
		$levelStates[0] = $GUI_CHECKED
	EndIf
	GUICtrlSetState($fileControls[3], $levelStates[5])
	GUICtrlSetState($fileControls[4], $levelStates[4])
	GUICtrlSetState($fileControls[5], $levelStates[3])
	GUICtrlSetState($fileControls[6], $levelStates[2])
	GUICtrlSetState($fileControls[7], $levelStates[1])
	GUICtrlSetState($fileControls[8], $levelStates[0])

	GUICtrlSetState($fileControls[9], _IniToCheckState($fileData[8][1]))
	GUICtrlSetState($fileControls[10], _IniToCheckState($fileData[9][1]))
	GUICtrlSetData($fileControls[11], _IniToInt($fileData[10][1]))
	GUICtrlSetData($fileControls[12], _IniToInt($fileData[11][1]))
EndFunc
Func _LoadSave()
	_LoadFile($file3Controls)
	_LoadFile($file2Controls)
	_LoadFile($file1Controls)
EndFunc

;HELPER FUNCTIONS
Func _IniToCheckState(ByRef $data)
	If $data = '"1.000000"' Then
		return $GUI_CHECKED
	EndIf
	return $GUI_UNCHECKED
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

Func _Exit()
	Exit
EndFunc
