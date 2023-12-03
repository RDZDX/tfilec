"C:\Program Files\MRE SDK V3.0.00\tools\DllPackage.exe" "D:\MyGitHub\Tfilec\Tfilec.vcproj"
if %errorlevel% == 0 (
 echo postbuild OK.
  copy Tfilec.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\Tfilec.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

