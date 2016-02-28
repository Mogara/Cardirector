wscript.echo "Copying Headers..."

Set WshShell = CreateObject("Wscript.Shell")

Set fso = CreateObject("Scripting.FileSystemObject")
Set scriptFile = fso.GetFile(WScript.ScriptFullName)

Set folder = scriptFile.ParentFolder

Set rootFolder = folder.ParentFolder
Set srcFolder = Nothing

For Each f In rootFolder.SubFolders
	If LCase(f.Name) = "src" Then
		Set srcFolder = f
		Exit For
	End If
Next 

If srcFolder Is Nothing Then
	WScript.Quit
End If

Dim folders()
ReDim folders(0)

Set folders(0) = srcFolder

i = 0
Do While i <= UBound(folders)
	For Each f In (folders(i)).files
		If (LCase(fso.GetExtensionName(f.Path))) = "h" And LCase(f.Name) <> "cpch.h" Then
			wscript.echo "Now copying " & f.path & " to " & folder.path
			f.Copy getCopyToName(f, folder), True
		End If
	Next
	
	For Each f In (folders(i)).subfolders
		If LCase(f.Name) <> "3rdparty" Then
			ReDim Preserve folders(UBound(folders) + 1)
			Set folders(UBound(folders)) = f
		End If
	Next
	i = i + 1
Loop

wscript.echo "Copy Complete"

Function getCopyToName(file, folder)
	pathStr = folder.Path
	If Right(pathStr, 1) <> "\" Then pathStr = pathstr & "\"
	pathstr = pathstr & file.name
	getCopyToName = pathstr
End Function