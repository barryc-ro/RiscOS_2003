; > asm.AppName
;
; Called from asm.ModuleWrap
;

	GET	VersionASM
	
        GBLS    ApplicationName

ApplicationName SETS    "ICAClient"

        GBLS    ApplicationVersion

ApplicationVersion SETS Module_MajorVersion :CC: " (" :CC: Module_Date :CC: ") " :CC: Module_MinorVersion

        END
