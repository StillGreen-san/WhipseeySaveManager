#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Res_ProductVersion=0.6
#AutoIt3Wrapper_Res_ProductName=Whipseey Save Manager
#AutoIt3Wrapper_Res_Comment=Intended for (Speedrun) practice
#AutoIt3Wrapper_Res_Description=GUI for modifying some Values in Whipseey Savegames
#AutoIt3Wrapper_Res_Language=1033
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****
;*****************************************
;WhipseeySaveManager.au3 by StillGreen-san
;Created with ISN AutoIt Studio v. 1.11
;*****************************************

#include "Forms\GUI.isf"

;VARS
Global $defaultSaveDir = @UserProfileDir &  "\AppData\Local\Whipseey\savedata"
Global $saveFile =  $defaultSaveDir &  "\whipseey.sav"
Global $gameFile = ""
Global $file1Controls[] = ["file1", $file1Castle, $file1Moon, $file1Snow, $file1Desert, $file1Forest, $file1Beach, $file1Ending, $file1Intro, $file1Lives, $file1Gems, $file1Group]
Global $file2Controls[] = ["file2", $file2Castle, $file2Moon, $file2Snow, $file2Desert, $file2Forest, $file2Beach, $file2Ending, $file2Intro, $file2Lives, $file2Gems, $file2Group]
Global $file3Controls[] = ["file3", $file3Castle, $file3Moon, $file3Snow, $file3Desert, $file3Forest, $file3Beach, $file3Ending, $file3Intro, $file3Lives, $file3Gems, $file3Group]
Global Enum $LEVEL_CASTLE = 5, $LEVEL_MOON = 4, $LEVEL_SNOW = 3, $LEVEL_DESERT = 2, $LEVEL_FOREST = 1, $LEVEL_BEACH = 0
Global Enum $CONTROL_NAME = 0, $CONTROL_CASTLE = 1, $CONTROL_MOON = 2, $CONTROL_SNOW = 3, $CONTROL_DESERT = 4, $CONTROL_FOREST = 5, $CONTROL_BEACH = 6, $CONTROL_ENDING = 7, $CONTROL_INTRO = 8, $CONTROL_LIVES = 9, $CONTROL_GEMS = 10,  $CONTROL_GROUP = 11
Global Enum $FILE_BOSS = 1, $FILE_ENEMIES = 2, $FILE_CASTLE = 3, $FILE_MOON = 4, $FILE_SNOW = 5, $FILE_DESERT = 6, $FILE_FOREST = 7, $FILE_ENDING = 8, $FILE_INTRO = 9, $FILE_LIVES = 10, $FILE_GEMS = 11
Global Enum $INI_KEY = 0, $INI_VALUE = 1, $INI_CASTLE = 32, $INI_MOON = 16, $INI_SNOW = 8, $INI_DESERT = 4, $INI_FOREST = 2

;GUI INIT
Opt("GUIOnEventMode", 1)

If FileExists($defaultSaveDir) Then
	GUICtrlSetData($pathSave, $saveFile)
	_LoadSave()
	_FindGame()
	GUICtrlSetData($pathGame, $gameFile)
	_LoadGame()
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
	Local $fileData = _ReadFile($saveFile, $fileControls[$CONTROL_NAME])
	Local $fileName = " " & StringReplace($fileControls[$CONTROL_NAME], "f", "F", 1)
	$fileName = $fileName & "  [ " & _IniToInt($fileData[$FILE_BOSS][$INI_VALUE]) & "-" & _IniToInt($fileData[$FILE_ENEMIES][$INI_VALUE]) & " ] "
	GuiCtrlSetData($fileControls[$CONTROL_GROUP], $fileName)

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

Func _LoadGame()
	$cheatState = IniRead($gameFile, "Cheats", "cheats_enabled", "0")
	If $cheatState = 1 Then
		GUICtrlSetState($cheats, $GUI_CHECKED)
	Else
		GUICtrlSetState($cheats, $GUI_UNCHECKED)
	EndIf
EndFunc

Func _ReadFile($file, $section)
	Local $fileData = IniReadSection($file, $section)
	If $fileData[$FILE_BOSS][$INI_KEY] = "boss_no_damage_progress" Then
		Return $fileData
	EndIf
	Dim $fullData[12][2]
	$fullData[0][0] = 11
	$fullData[$FILE_BOSS][$INI_KEY] = "boss_no_damage_progress"
	$fullData[$FILE_BOSS][$INI_VALUE] = "0.000000"
	For $i = $FILE_ENEMIES To $FILE_GEMS
		$fullData[$i][$INI_KEY] = $fileData[$i-1][$INI_KEY]
		$fullData[$i][$INI_VALUE] = $fileData[$i-1][$INI_VALUE]
	Next
	Return $fullData
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
Func _SaveSave()
	_SaveFile($file3Controls)
	_SaveFile($file2Controls)
	_SaveFile($file1Controls)
EndFunc
Func _SaveGame()
	If GUICtrlRead($cheats) = $GUI_CHECKED Then
		IniWrite($gameFile, "Cheats", "cheats_enabled", 1)
	Else
		IniWrite($gameFile, "Cheats", "cheats_enabled", 0)
	EndIf
EndFunc

Func _File1Gems0()
	GUICtrlSetData($file1Gems, 0)
EndFunc
Func _File1Gems9()
	GUICtrlSetData($file1Gems, 99)
EndFunc
Func _File1Lives5()
	GUICtrlSetData($file1Lives, 5)
EndFunc
Func _File1Lives9()
	If GUICtrlRead($file1Lives) = 99 Then
		GUICtrlSetData($file1Lives, 9999)
	Else 
		GUICtrlSetData($file1Lives, 99)
	EndIf
EndFunc

Func _File2Gems0()
	GUICtrlSetData($file2Gems, 0)
EndFunc
Func _File2Gems9()
	GUICtrlSetData($file2Gems, 99)
EndFunc
Func _File2Lives5()
	GUICtrlSetData($file2Lives, 5)
EndFunc
Func _File2Lives9()
	If GUICtrlRead($file2Lives) = 99 Then
		GUICtrlSetData($file2Lives, 9999)
	Else 
		GUICtrlSetData($file2Lives, 99)
	EndIf
EndFunc

Func _File3Gems0()
	GUICtrlSetData($file3Gems, 0)
EndFunc
Func _File3Gems9()
	GUICtrlSetData($file3Gems, 99)
EndFunc
Func _File3Lives5()
	GUICtrlSetData($file3Lives, 5)
EndFunc
Func _File3Lives9()
	If GUICtrlRead($file3Lives) = 99 Then
		GUICtrlSetData($file3Lives, 9999)
	Else 
		GUICtrlSetData($file3Lives, 99)
	EndIf
EndFunc

Func _File1Max()
	GUICtrlSetData($file1Gems, 99)
	GUICtrlSetData($file1Lives, 9999)
	GUICtrlSetState($file1Intro, $GUI_CHECKED)
	GUICtrlSetState($file1Ending, $GUI_CHECKED)
EndFunc
Func _File1Delete()
	GUICtrlSetData($file1Gems, 0)
	GUICtrlSetData($file1Lives, 5)
	GUICtrlSetState($file1Intro, $GUI_UNCHECKED)
	GUICtrlSetState($file1Ending, $GUI_UNCHECKED)
	GUICtrlSetState($file1Beach, $GUI_CHECKED)
EndFunc

Func _File2Max()
	GUICtrlSetData($file2Gems, 99)
	GUICtrlSetData($file2Lives, 9999)
	GUICtrlSetState($file2Intro, $GUI_CHECKED)
	GUICtrlSetState($file2Ending, $GUI_CHECKED)
EndFunc
Func _File2Delete()
	GUICtrlSetData($file2Gems, 0)
	GUICtrlSetData($file2Lives, 5)
	GUICtrlSetState($file2Intro, $GUI_UNCHECKED)
	GUICtrlSetState($file2Ending, $GUI_UNCHECKED)
	GUICtrlSetState($file2Beach, $GUI_CHECKED)
EndFunc

Func _File3Max()
	GUICtrlSetData($file3Gems, 99)
	GUICtrlSetData($file3Lives, 9999)
	GUICtrlSetState($file3Intro, $GUI_CHECKED)
	GUICtrlSetState($file3Ending, $GUI_CHECKED)
EndFunc
Func _File3Delete()
	GUICtrlSetData($file3Gems, 0)
	GUICtrlSetData($file3Lives, 5)
	GUICtrlSetState($file3Intro, $GUI_UNCHECKED)
	GUICtrlSetState($file3Ending, $GUI_UNCHECKED)
	GUICtrlSetState($file3Beach, $GUI_CHECKED)
EndFunc

Func _File1Save()
	_SaveFile($file1Controls)
EndFunc
Func _File1Reload()
	_LoadFile($file1Controls)
EndFunc

Func _File2Save()
	_SaveFile($file2Controls)
EndFunc
Func _File2Reload()
	_LoadFile($file2Controls)
EndFunc

Func _File3Save()
	_SaveFile($file3Controls)
EndFunc
Func _File3Reload()
	_LoadFile($file3Controls)
EndFunc

Func _OpenFileSave()
	$result =  FileOpenDialog("Select Savefile", $defaultSaveDir, "Save files (*.sav)|All (*.*)",  $FD_FILEMUSTEXIST + $FD_PATHMUSTEXIST, "whipseey.sav")
	If Not @error Then
		GUICtrlSetData($pathSave, $result)
		$saveFile = $result
		_LoadSave()
	EndIf
EndFunc
Func _OpenFileGame()
	$result =  FileOpenDialog("Select Settingsfile", StringReplace($gameFile, "bfs_settings.ini", "", 1), "Settings files (bfs_settings.ini)|All (*.*)",  $FD_FILEMUSTEXIST + $FD_PATHMUSTEXIST, "bfs_settings.ini")
	If Not @error Then
		GUICtrlSetData($pathGame, $result)
		$gameFile = $result
		_LoadGame()
	EndIf
EndFunc

Func _ReloadSave()
	_LoadSave()
EndFunc
Func _ReloadGame()
	_LoadGame()
EndFunc

Func _ShowCheats()
	MsgBox($MB_ICONINFORMATION+$MB_SETFOREGROUND, "Cheats", "checking Cheats will enable some hotkeys in game" & _
		@CRLF & "R  : restart room" & _
		@CRLF & "N  : next room" & _
		@CRLF & "P  : toggle fullscreen" & _
		@CRLF & ", . , .  : infinite flight" & _
		@CRLF & ", . , ,  : unlock all levels" & _
		@CRLF & ", , , .  : disable hud" & _
		@CRLF & ", , , ,  : invincibility", 0, $GUI)
EndFunc

;HELPER FUNCTIONS
Func _IniToCheckState(ByRef $data)
	If $data = '"1.000000"' Then
		Return $GUI_CHECKED
	EndIf
	Return $GUI_UNCHECKED
EndFunc
Func _CheckStateToIni($state)
	If $state = $GUI_CHECKED Then
		return '"1.000000"'
	EndIf
	Return '"0.000000"'
EndFunc

Func _IniToRadioState(ByRef $data)
	If $data = "1.000000" Then
		Return $GUI_CHECKED
	EndIf
	Return $GUI_UNCHECKED
EndFunc

Func _IniToInt($data)
	$data = StringReplace($data, '"', "")
	Return Int($data)
EndFunc
Func _IntToIni($int)
	Return String('"' & $int & '.000000"')
EndFunc

Func _FindGame()
	Local $steamPath = RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Valve\Steam", "InstallPath")
	If @error Then
		$steamPath = RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\Valve\Steam", "InstallPath")
		If @error Then Return
	EndIf
	
	Local $settingsPath = "\steamapps\common\Whipseey and the Lost Atlas\bfs_settings.ini"
	Local $librariesPath = "\steamapps\libraryfolders.vdf"
	Local Enum $LIB_NUM = 0, $LIB_PATH = 1
	
	If FileExists($steamPath & $settingsPath) Then
		$gameFile = $steamPath & $settingsPath
		Return
	EndIf

	Local $file = FileOpen($steamPath & $librariesPath, $FO_READ)
	If $file = -1 Then
		$gameFile = ""
		Return
	EndIf

	While True 
		Local $line = FileReadLine($file)
		If @error = -1 Then ExitLoop

		Local $library = StringRegExp($line, '.*"(\d+)".*"(.*)"', $STR_REGEXPARRAYMATCH)
		If @error Then ContinueLoop

		If Number($library[$LIB_NUM]) = 0 Then ContinueLoop
		$library[$LIB_PATH] = StringReplace($library[$LIB_PATH], "\\", "\", 1)

		If FileExists($library[$LIB_PATH] & $settingsPath) Then
			$gameFile = $library[$LIB_PATH] & $settingsPath
			Return
		EndIf
	WEnd

	$gameFile = ""
EndFunc

Func _Exit()
	Exit
EndFunc
