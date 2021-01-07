;*****************************************
;WhipseeySaveManager.au3 by StillGreen-san
;Created with ISN AutoIt Studio v. 1.11
;*****************************************

#include "Forms\GUI.isf"

;VARS
$defaultSaveDir = @UserProfileDir &  "\AppData\Local\Whipseey\savedata"
$saveFile =  $defaultSaveDir &  "\whipseey.sav"

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
Func _LoadSave()
	Local $fileData = IniReadSection($saveFile, "file1")
	GUICtrlSetData($file1Boss, _IniToInt($fileData[1][1]))
	GUICtrlSetData($file1Enemies, _IniToInt($fileData[2][1]))

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
	GUICtrlSetState($file1Castle, $levelStates[5])
	GUICtrlSetState($file1Moon, $levelStates[4])
	GUICtrlSetState($file1Snow, $levelStates[3])
	GUICtrlSetState($file1Desert, $levelStates[2])
	GUICtrlSetState($file1Forest, $levelStates[1])
	GUICtrlSetState($file1Beach, $levelStates[0])

	GUICtrlSetState($file1Ending, _IniToCheckState($fileData[8][1]))
	GUICtrlSetState($file1Intro, _IniToCheckState($fileData[9][1]))
	GUICtrlSetData($file1Lives, _IniToInt($fileData[10][1]))
	GUICtrlSetData($file1Gems, _IniToInt($fileData[11][1]))
EndFunc

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
	return StringReplace($data, ".000000", "")
EndFunc

Func _Exit()
	Exit
EndFunc
