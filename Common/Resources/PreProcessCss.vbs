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
'~ function print(strStream)
'~ function printArray(arr)
'~ function replaceStream(arr, txtStream)
'~ function RisingEdge(Flag)
'~ function FallingEdge(Flag)
'~ function OpenFile(File)
'~ function WriteFile(File, strTxt)
'~ main()
'##################################################################################################

Option Explicit
'On Error Resume Next

'Globals
dim WshShell, fs, RsngEdg

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

'##################################################################################################
'Funktionen
'##################################################################################################

'##################################################################################################
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
'##################################################################################################
function RegExpTest(patrn, strng, count, myMatch, FoundAll, replace_, repStrng)
dim regEx, myMatches

    RegExpTest = false
    set regEx = New RegExp      ' Create a regular expression.
    regEx.Pattern = patrn           ' set pattern.
    regEx.IgnoreCase = true'       ' set case insensitivity.
    regEx.Global = true' false          ' set global applicability. --Alle vorhandenen finden

    set myMatches = regEx.Execute(strng)  ' Execute search.
    'print myMatches.Count

    if (myMatches.Count > 0) then
        '~ For each myMatch in myMatches
            '~ print "FirstIndex: " &  myMatch.FirstIndex
            '~ print "Laenge: " & myMatch.Length
            '~ print "Wert: " & myMatch.Value
        '~ Next

        RegExpTest = true
        if (FoundAll = true) then
            count = myMatches.Count
            myMatch = myMatches(0).Value
        else
            count = myMatches(0).FirstIndex
            myMatch = myMatches(0).Value
        end if

        if (replace_ = true) then
            strng = regEx.replace(strng, repStrng)	'replace
        end if
    end if

end function

'##################################################################################################
' Funktion:  print
' Desc:
'##################################################################################################
function print(strStream)

    wscript.echo (strStream)

end function

'##################################################################################################
' Funktion:  printArray
' Desc:
'##################################################################################################
function printArray(arr)
dim i

    for i = 0 to UBound(arr)
        wscript.echo i & ": " &(arr(i))
    next

end function

'##################################################################################################
' Funktion:  printArray
' Desc:
'##################################################################################################
function replaceStream(arr, txtStream)
dim i, strClr

    for i = 0 to UBound(arr)-1
        strClr = split(arr(i), ":", -1, 1)
        strClr(0) = trim(strClr(0))
        strClr(1) = trim(strClr(1))

        call RegExpTest("(\b"&strClr(0)&"\b)", txtStream, empty, empty, FndAll, Rplc, strClr(1))
    next

    replaceStream = txtStream
end function

'##################################################################################################
' Funktion:  RisingEdge
' Desc:
'##################################################################################################
function RisingEdge(Flag)

    if ((Flag = true) and (RsngEdg = false)) then
        RisingEdge = true
    end if

    RsngEdg = Flag
end function

'##################################################################################################
' Funktion:  RisingEdge
' Desc:
'##################################################################################################
function FallingEdge(Flag)

    if ((Flag = false) and (RsngEdg = true)) then
        FallingEdge = true
    end if

    RsngEdg = Flag
end function

'##################################################################################################
' Funktion:  OpenFile
' Desc:
'##################################################################################################
function OpenFile(File)
dim f, ts, strIn, arrClr(), fndFlg

Redim arrClr(0)
fndFlg = false

On Error Resume Next
	Set f = fs.GetFile(File)
	if (Err.Number <> 0) then
		Err.Clear   ' Clear the error.
		exit Function
	end if
On Error GoTo 0

	Set ts = f.OpenAsTextStream(ForReading, -2)
	Do While ts.AtEndOfStream <> True
        do
            strIn = ts.ReadLine()

            if (FallingEdge(fndFlg)) then
                strIn = ts.ReadAll()
            end if

            if (RegExpTest("<Farben>", strIn, empty, empty, FndAll, empty, "")) then
                fndFlg = true
                exit do
            end if
            if (RegExpTest("</Farben>", strIn, empty, empty, FndAll, empty, "")) then
                fndFlg = false
                exit do
            end if

            if (fndFlg = true) then
                arrClr(UBound(arrClr)) = strIn
                Redim Preserve arrClr(UBound(arrClr) + 1)
                exit do
            end if

        loop until true
	loop
	ts.close

	set f = nothing
	set ts = nothing

    'wscript.echo strIn
    OpenFile = replaceStream(arrClr, strIn)

end function

'##################################################################################################
' Funktion:  WriteFile
' Desc:
'##################################################################################################
function WriteFile(File, strTxt)
dim f, ts

    call fs.CreateTextFile(File, true)

On Error Resume Next
	Set f = fs.GetFile(File)
	if (Err.Number <> 0) then
		Err.Clear   ' Clear the error.
		exit Function
	end if
On Error GoTo 0

	Set ts = f.OpenAsTextStream(ForWriting, -2)
    ts.Write(strTxt)
	ts.close

	set f = nothing
	set ts = nothing

end function
'##################################################################################################
' function:  Main
' Desc:
'##################################################################################################
'Hier kommt die Main rein !!
dim objArgs, File, i, strOut, OutFile

    RsngEdg = false
    do
		set objArgs = Wscript.Arguments
		if (objArgs.Count = 0) then
			wscript.echo "Falsche Parameter"
			exit do
		end if

		for i = 0 to objArgs.Count-1
            strOut = OpenFile(objArgs(i))

            OutFile = fs.GetBaseName(objArgs(i))
            print "konvertiere Style " & OutFile
            call RegExpTest("(css)", OutFile, empty, empty, FndAll, Rplc, ".css")
            OutFile = fs.GetParentFolderName(objArgs(i)) & "\" & OutFile

            call WriteFile(OutFile, strOut)
		next

    loop until true

    set WshShell = nothing
    set fs = nothing