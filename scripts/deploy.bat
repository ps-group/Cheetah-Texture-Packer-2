@echo off
:: Deploys QT application
:: Usage: deploy.cmd $TargetName $AppBinariesDir $QtSdkDir

SET TargetName=%1%
SET AppBinPath=%2%
SET QtSdkBinPath=%3%\bin
SET PackagePath=%4%

IF "%TargetName%"=="" GOTO :invalid_arguments
IF "%AppBinPath%"=="" GOTO :invalid_arguments
IF "%QtSdkBinPath%"=="" GOTO :invalid_arguments
IF "%PackagePath%"=="" GOTO :invalid_arguments

echo Packaging %TargetName%...

IF EXIST "%PackagePath%" (
    RD /S /Q "%PackagePath%"
)
MKDIR "%PackagePath%"
PUSHD %PackagePath%
COPY /Y %AppBinPath%\%TargetName%.exe %PackagePath%\%TargetName%.exe
:: COPY /Y "%QtSdkBinPath%\libgcc_s_dw2-1.dll" "%PackagePath%\libgcc_s_dw2-1.dll"
:: COPY /Y "%QtSdkBinPath%\libstdc++-6.dll" "%PackagePath%\libstdc++-6.dll"
:: COPY /Y "%QtSdkBinPath%\libwinpthread-1.dll" "%PackagePath%\libwinpthread-1.dll"
:: COPY /Y "%QtSdkBinPath%\Qt5MultimediaQuick_p.dll" "%PackagePath%\Qt5MultimediaQuick_p.dll"
:: COPY /Y "%QtSdkBinPath%\Qt5MultimediaWidgets.dll" "%PackagePath%\Qt5MultimediaWidgets.dll"
:: COPY /Y "%QtSdkBinPath%\Qt5OpenGL.dll" "%PackagePath%\Qt5OpenGL.dll"
:: COPY /Y "smack-win\libs\win32\libeay32.dll" "%PackagePath%\libeay32.dll"
:: COPY /Y "smack-win\libs\win32\libssl32.dll" "%PackagePath%\libssl32.dll"
:: COPY /Y "smack-win\libs\win32\ssleay32.dll" "%PackagePath%\ssleay32.dll"
:: COPY /Y "smack-win\libs\win32\msvcm90.dll" "%PackagePath%\msvcm90.dll"
:: COPY /Y "smack-win\libs\win32\msvcp90.dll" "%PackagePath%\msvcp90.dll"
:: COPY /Y "smack-win\libs\win32\msvcr90.dll" "%PackagePath%\msvcr90.dll"
PATH=%PATH%;%QtSdkBinPath%
windeployqt --release %PackagePath%\%TargetName%.exe
POPD

exit /b
goto :eof

::@echo Removing debug and unused libraries...
::@echo off

DEL /Q "%PackagePath%\accessible\qtaccessiblequickd.dll"
DEL /Q "%PackagePath%\accessible\qtaccessiblewidgetsd.dll"

DEL /Q "%PackagePath%\bearer\qgenericbearerd.dll"
DEL /Q "%PackagePath%\bearer\qnativewifibearerd.dll"

DEL /Q "%PackagePath%\iconengines\qsvgicond.dll"

DEL /Q "%PackagePath%\imageformats\qgifd.dll"
DEL /Q "%PackagePath%\imageformats\qicod.dll"
DEL /Q "%PackagePath%\imageformats\qjpegd.dll"
DEL /Q "%PackagePath%\imageformats\qmngd.dll"
DEL /Q "%PackagePath%\imageformats\qsvgd.dll"
DEL /Q "%PackagePath%\imageformats\qtgad.dll"
DEL /Q "%PackagePath%\imageformats\qtiffd.dll"
DEL /Q "%PackagePath%\imageformats\qwbmpd.dll"

DEL /Q "%PackagePath%\mediaservice\dsengined.dll"
DEL /Q "%PackagePath%\mediaservice\qtmedia_audioengined.dll"

DEL /Q "%PackagePath%\playlistformats\qtmultimedia_m3ud.dll"

DEL /Q "%PackagePath%\qmltooling\qmldbg_qtquick2d.dll"
DEL /Q "%PackagePath%\qmltooling\qmldbg_tcpd.dll"

DEL /Q "%PackagePath%\Qt\labs\folderlistmodel\qmlfolderlistmodelplugind.dll"
DEL /Q "%PackagePath%\QtMultimedia\declarative_multimediad.dll"
DEL /Q "%PackagePath%\QtQuick.2\qtquick2plugind.dll"
DEL /Q "%PackagePath%\QtQuick\Controls\qtquickcontrolsplugind.dll"
DEL /Q "%PackagePath%\QtQuick\Dialogs\dialogplugind.dll"
DEL /Q "%PackagePath%\QtQuick\Dialogs\Private\dialogsprivateplugind.dll"
DEL /Q "%PackagePath%\QtQuick\Layouts\qquicklayoutsplugind.dll"
DEL /Q "%PackagePath%\QtQuick\PrivateWidgets\widgetsplugind.dll"
DEL /Q "%PackagePath%\QtQuick\Window.2\windowplugind.dll"

goto :eof

:invalid_arguments
echo "Invalid arguments passed!"
exit /b 1
