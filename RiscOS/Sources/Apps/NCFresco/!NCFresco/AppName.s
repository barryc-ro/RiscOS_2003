; > asm.AppName
;
; Called from asm.ModuleWrap
;

	GET	VersionASM
        GBLS    ApplicationName
ApplicationName SETS    "NCFresco"
        GBLS    ApplicationVersion
ApplicationVersion SETS Module_MajorVersion :CC: " (" :CC: Module_Date :CC: ") " :CC: Module_MinorVersion
	; ApplicationVersion SETS "1.33 (11 Sep 1997)"

        END
