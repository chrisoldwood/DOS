@ECHO OFF
@ECHO Removing Java intermediate files...

del /s /f *.class
del /s /f *.jar
del /s /f *.cab

@ECHO Removing VC++ intermediate files...

del /s /f *.pch
del /s /f *.sbr
del /s /f *.obj
del /s /f *.res

@ECHO Removing VC++ target files...

del /s /f *.pdb
del /s /f *.idb
del /s /f *.ilk
del /s /f *.exe
del /s /f *.dll
del /s /f *.lib
del /s /f *.plg
del /s /f *.map
del /s /f *.exp

@ECHO Removing VC++ workspace files...

del /s /f *.ncb
del /s /f *.opt
del /s /f *.aps
del /s /f *.bsc
del /s /f *.scc
del /s /f *.$$$
del /s /f *.vcw
del /s /f *.wsp

@ECHO Removing VC++ profiling/instrumentation files...

del /s /f *.pbi
del /s /f *.pbo
del /s /f *.pbt
del /s /f *._xe
del /s /f *.pcc
del /s /f *.stt

@ECHO Removing log files...

del /s /f *.log
del /s /f CrtReports.txt
del /s /f *.sup
del /s /f *.err
