wscript.echo "Generating Headers..."

Set WshShell = CreateObject("Wscript.Shell")

Set fso = CreateObject("Scripting.FileSystemObject")
Set scriptFile = fso.GetFile(WScript.ScriptFullName)

Set folder = scriptFile.ParentFolder

For Each file In folder.Files
	If fso.GetExtensionName(file.Path) = "h" Then
		Set fs = fso.OpenTextFile(file.Path)
		Do While Not fs.AtEndOfStream
			line = fs.ReadLine
			If Left(line, 17) = "class MCD_EXPORT " Then
				truncatedLine = Mid(line,17)
				colonPos = InStr(truncatedLine, ":")
				Dim generatedFileName
				If colonPos = 0 Then generatedFileName = Trim(truncatedline) Else generatedFileName = Trim(Left(truncatedLine, colonPos - 1))
				Dim path
				path = folder.Path
				If Right(path, 1) <> "\" Then path = path & "\"
				path = path & generatedFileName
				wscript.echo "Generating Header for " & generatedFileName
				Set generatedFile = fso.CreateTextFile(path, True, False)
				generatedLine = "#include """ & fso.GetFileName(file.Path) & """"
				generatedFile.WriteLine generatedLine
				generatedFile.Close
			End If
		Loop
		fs.close
	End If
Next

wscript.echo "Header Generation Complete"
