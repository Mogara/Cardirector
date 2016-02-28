wscript.echo "Generating cgui.qrc..."

Set WshShell = WScript.CreateObject("Wscript.Shell")
Set fso = WScript.CreateObject("Scripting.Filesystemobject")
Set scriptFile = fso.GetFile(WScript.ScriptFullName)
Set folder = scriptFile.ParentFolder
Set cardirectorFolder = folder.ParentFolder
Set guiFolder = Nothing
Set subFolders = cardirectorFolder.SubFolders

For Each f In subfolders
	If f.Name = "Gui" Then
		Set guiFolder = f
		Exit For
	End If
Next
If guiFolder Is Nothing Then
	WScript.Echo "There is no folder which name is GUI in Cardirector"
	WScript.Quit
End If

Set guiPrivateFolder = Nothing
For Each f In guiFolder.SubFolders
	If LCase(f.Name) = "private" Then
		Set guiPrivateFolder = f
		Exit For
	End If
Next

For Each f In folder.Files
	If LCase(f.name) = "cgui.qrc" Then
		f.attributes = 0
		f.delete
	End If
Next

Dim path
path = folder.Path
If Right(path, 1) <> "\" Then path = path & "\"
path = path & "cgui.qrc"

Set generatedFile = fso.CreateTextFile(path, True, False)
generatedfile.WriteLine "<RCC>"
generatedfile.WriteLine "    <qresource prefix=""/Cardirector/Gui"">"

For Each f In guiFolder.Files
	If LCase(fso.GetExtensionName(f.path)) = "qml" Or f.name = "qmldir" Then
		generatedFile.WriteLine "        <file alias=""" & f.name & """>../Gui/" & f.name & "</file>"
	End If
Next

If Not guiPrivateFolder Is Nothing Then
	For Each f In guiPrivateFolder.Files
		If LCase(fso.GetExtensionName(f.path)) = "qml" Then
			generatedFile.WriteLine "        <file alias=""" & f.name & """>../Gui/Private/" & f.name & "</file>"
		End If
	Next
End If

generatedFile.WriteLine "    </qresource>"
generatedFile.WriteLine "</RCC>"
generatedFile.Close

wscript.echo "cgui.qrc Generation Complete"
