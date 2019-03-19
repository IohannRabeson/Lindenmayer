QT_SDK_URL="http://download.qt.io/official_releases/qt/5.12/5.12.2/qt-opensource-windows-x86-5.12.2.exe"
powershell -Command "Invoke-WebRequest $QT_SDK_URL -OutFile package.zip"