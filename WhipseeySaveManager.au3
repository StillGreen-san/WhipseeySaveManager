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
;~ 	boss_no_damage_progress="2.000000"
	GUICtrlSetData($file1Boss, _IniToInt($fileData[1][1]))
;~ 	enemies_defeated="195.000000"
	GUICtrlSetData($file1Enemies, _IniToInt($fileData[2][1]))

;~ 	castle="32.000000"
;~ 	GUICtrlSetData($file1Boss, $fileData[3][1])
;~ 	moon="16.000000"
;~ 	GUICtrlSetData($file1Boss, $fileData[4][1])
;~ 	snow="8.000000"
;~ 	GUICtrlSetData($file1Boss, $fileData[5][1])
;~ 	desert="4.000000"
;~ 	GUICtrlSetData($file1Boss, $fileData[6][1])
;~ 	forest="2.000000"
;~ 	GUICtrlSetData($file1Boss, $fileData[7][1])

;~ 	ending="1.000000"
	GUICtrlSetState($file1Ending, _IniToCheckState($fileData[8][1]))
;~ 	intro="1.000000"
	GUICtrlSetState($file1Intro, _IniToCheckState($fileData[9][1]))
;~ 	lives="5.000000"
	GUICtrlSetData($file1Lives, _IniToInt($fileData[10][1]))
;~ 	gems="83.000000"
	GUICtrlSetData($file1Gems, _IniToInt($fileData[11][1]))
EndFunc

Func _IniToCheckState(ByRef $data)
	If $data = "1.000000" Then
		return $GUI_CHECKED
	ElseIf $data = "0.000000" Then
		return $GUI_UNCHECKED
	EndIf
	return $GUI_INDETERMINATE
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
