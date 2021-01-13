'##################################################################################################
' function:	ScriptTemplate
' Desc:
'
' Author:
' Created:
' ScriptVersion:	2.00
' Historie
'##################################################################################################
'~ Prototypen
'~ main()
'##################################################################################################

Option Explicit
'On Error Resume Next

'Globals
dim WshShell, fs
dim cVS, cQTBin, cQT

'const
const ForReading = 1
const ForWriting = 2
const ForAppending = 8
const FndFrst = False
const FndAll = True
const Rplc = True
const Srch = False

'Initialisierung
set WshShell = CreateObject("WScript.Shell")
set fs = CreateObject("Scripting.FileSystemObject")

cVS = WshShell.ExpandEnvironmentStrings("%ProgramFiles%") & "\Microsoft Visual Studio 12.0\Team Tools\Performance Tools"
cQT = "D:\Qt\5.4.0\5.4\msvc2013"
cQTBin = cQT & "\bin"

'*********************************************************************************************************
' Funktion:     RegExpTest()
' Parameter:    patrn, strng
' Return-Wert:  true oder false
' Beschreibung:
' patrn: SuchString
' strng: zu durchsuchender String
' count: Variable uebergeben oder "empty"
' FoundAll: nur den ersten oder alle
' replace: true oder false
' repStrng: ersetzender String
'*********************************************************************************************************
function RegExpTest(patrn, strng, count, FoundAll, Ersetze, repStrng)
dim regEx, myMatches, myMatch, retString

    RegExpTest = false
    set regEx = New RegExp          ' Create a regular expression.
    regEx.Pattern = patrn   					   ' set pattern.
    regEx.IgnoreCase = true   				  ' set case insensitivity.
    regEx.Global = true' false   			' set global applicability. --Alle vorhandenen finden

    set myMatches = regEx.Execute(strng)  ' Execute search.
    'wscript.echo myMatches.Item(0).FirstIndex
    'wscript.echo myMatches.Count
    'wscript.echo myMatches(0).FirstIndex

    if (myMatches.Count > 0) then
        For each myMatch in myMatches
            'wscript.echo myMatch.FirstIndex
            'wscript.echo myMatch.Length
            retString = retString & myMatch.Value & vbCRLF
        Next

        RegExpTest = retString
        if (FoundAll = true) then
            count = myMatches.Count
        else
            count = myMatches(0).FirstIndex
        end if
        if (Ersetze = true) then
            strng = regEx.replace(strng, repStrng)	'replace
        end if
    end if

end function

'##################################################################################################
' function:  GetDllForProject
' Desc:
'##################################################################################################
function GetDllForProject()
dim f, objFolder, colFiles, objFile, ReadAllTextFile, count, strDll, arrHlp, i, hlpArr

    wscript.echo "*.vcxproj sollten in " & WshShell.CurrentDirectory & " liegen"
    set objFolder= fs.GetFolder(WshShell.CurrentDirectory)
    Set colFiles = objFolder.Files
    For Each objFile in colFiles
        if (fs.GetExtensionName(objFile) = "vcxproj")  then
            wscript.echo "Verwende " & objFile
            exit for
        end if
    Next

    Set f = fs.OpenTextFile(objFile, ForReading)
    ReadAllTextFile = f.ReadAll

    strDll = RegExpTest("(<AdditionalDependencies>)(.*)(</AdditionalDependencies>)", ReadAllTextFile, count, FndAll, Srch, "")
    if (count <= 0) then
        exit function
    end if

    strDll = replace(strDll, "AdditionalDependencies", "")
    strDll = replace(strDll, "<>", "")
    strDll = replace(strDll, "</>", "")
    strDll = replace(strDll, "%()", "")
    strDll = replace(strDll, ".lib", "")
    strDll = replace(strDll, "dwmapi", "")

    arrHlp = split(strDll, vbCRLF)
    'jetzt sind noch Debug und Release aufzudröseln
    for i = 0 to UBound(arrHlp)-1
        hlpArr = split(arrHlp(i), ";")

        if (LCase(Right(hlpArr(0), 1) <> "d")) then
            strDll = arrHlp(i)
            exit for
        end if
    next

    strDll = replace(strDll, "qtmain;", "")
    GetDllForProject = split(strDll, ";")

end function

'##################################################################################################
' function:  Main
' Desc:
'##################################################################################################
'Hier kommt die Main rein !!
dim objArgs, ReleasePath, FileCopy, objFolder, colFiles, objFile, i, arrDll

    do
		set objArgs = Wscript.Arguments
		if (objArgs.Count = 0) then
			wscript.echo "Falsche Parameter"
			exit do
		end if
        ReleasePath = objArgs(0)
        wscript.echo vbNewLine & "Kopiere Resourcen"

        'files from Qt

        'FileCopy = cQTBin & "\d3dcompiler_46.dll"
        'wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        'fs.CopyFile FileCopy , ReleasePath

        'fs.CopyFile cQTBin & "\libEGL.dll" , ReleasePath
        'fs.CopyFile cQTBin & "\libGLESv2.dll" , ReleasePath
        'fs.CopyFile cQTBin & "\Qt5Network.dll" , ReleasePath
        'fs.CopyFile cQTBin & "\Qt5OpenGL.dll" , ReleasePath
        'fs.CopyFile cQTBin & "\Qt5Qml.dll" , ReleasePath
        'fs.CopyFile cQTBin & "\Qt5Quick.dll" , ReleasePath
        'fs.CopyFile cQTBin & "\Qt5QuickParticles.dll" , ReleasePath
        'fs.CopyFile cQTBin & "\Qt5Sql.dll" , ReleasePath

        FileCopy = cQTBin & "\icudt53.dll"
        wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        fs.CopyFile FileCopy , ReleasePath

        FileCopy = cQTBin & "\icuin53.dll"
        wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        fs.CopyFile FileCopy , ReleasePath

        FileCopy = cQTBin & "\icuuc53.dll"
        wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        fs.CopyFile FileCopy , ReleasePath

        'Dlls abhängig vom Projekt. Dazu die QtProjekt.vcxproj öffnen und nach dem AdditionalDependencies Tag parsen
        arrDll = GetDllForProject()
        for i = 0 to UBound(arrDll) -1
            if (arrDll(i) <> "") then
                FileCopy = cQTBin & "\" & arrDll(i) & ".dll"
                wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
                fs.CopyFile FileCopy , ReleasePath
            end if
        next

        'Visual Studio
        FileCopy =  cVS & "\msvcp120.dll"
        wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        fs.CopyFile FileCopy , ReleasePath

        FileCopy =  cVS & "\msvcr120.dll"
        wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        fs.CopyFile FileCopy , ReleasePath

        'DIRs from Qt
        'can be fount in Qt plugins dir
        FileCopy =  cQT & "\plugins\platforms"
        if (not fs.FolderExists(ReleasePath & "platforms")) then
            fs.CreateFolder(ReleasePath & "platforms")
        end if
        Set objFolder = fs.GetFolder(FileCopy)
        Set colFiles = objFolder.Files
        For Each objFile in colFiles
			if (fs.GetExtensionName(objFile) = "dll") and (LCase(Right(fs.GetBaseName(objFile), 1) <> "d"))  then
                wscript.echo "Datei: " & objFile & " ===> " & ReleasePath & "platforms"
                fs.CopyFile objFile , ReleasePath & "platforms\" & fs.GetFileName(objFile)
            end if
        Next

        FileCopy =  cQT & "\plugins\imageformats"
        if (not fs.FolderExists(ReleasePath & "imageformats")) then
            fs.CreateFolder(ReleasePath & "imageformats")
        end if
        Set objFolder = fs.GetFolder(FileCopy)
        Set colFiles = objFolder.Files
        For Each objFile in colFiles
			if (fs.GetExtensionName(objFile) = "dll") and (LCase(Right(fs.GetBaseName(objFile), 1) <> "d"))  then
                wscript.echo "Datei: " & objFile & " ===> " & ReleasePath & "imageformats"
                fs.CopyFile objFile , ReleasePath & "imageformats\" & fs.GetFileName(objFile)
            end if
        Next

        FileCopy =  cQT & "\plugins\printsupport"
        if (not fs.FolderExists(ReleasePath & "printsupport")) then
            fs.CreateFolder(ReleasePath & "printsupport")
        end if
        Set objFolder = fs.GetFolder(FileCopy)
        Set colFiles = objFolder.Files
        For Each objFile in colFiles
			if (fs.GetExtensionName(objFile) = "dll") and (LCase(Right(fs.GetBaseName(objFile), 1) <> "d"))  then
                wscript.echo "Datei: " & objFile & " ===> " & ReleasePath & "printsupport"
                fs.CopyFile objFile , ReleasePath & "printsupport\" & fs.GetFileName(objFile)
            end if
        Next

        'can be found in Qt qml dir
        'fs.CopyFolder  cQT & "\qml\Qt"
        'fs.CopyFolder  cQT & "\qml\QtGraphicalEffects" , ReleasePath
        'fs.CopyFolder  cQT & "\qml\QtQuick" , ReleasePath
        'fs.CopyFolder  cQT & "\qml\QtQuick2" , ReleasePath

        'Extending functionality will require more files to include in application DIR.

        Set objFolder = fs.GetFolder(ReleasePath)
        Set objFolder = objFolder.ParentFolder
        FileCopy = objFolder.ParentFolder & "\PcZeit\cUrl\curl.exe"
        wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        fs.CopyFile FileCopy , ReleasePath

        FileCopy = objFolder.ParentFolder & "\PcZeit\cUrl\libcurl.dll"
        wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        fs.CopyFile FileCopy , ReleasePath

        FileCopy = objFolder.ParentFolder & "\PcZeit\cUrl\curl-ca-bundle.crt"
        wscript.echo "Datei: " & FileCopy & " ===> " & ReleasePath
        fs.CopyFile FileCopy , ReleasePath

    loop until true

    set WshShell = nothing
    set fs = nothing