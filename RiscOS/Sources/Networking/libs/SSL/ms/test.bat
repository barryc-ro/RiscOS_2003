@echo=off

set bin=..\out
set test=.

echo destest
%bin%\destest
if errorlevel 1 goto done

echo ideatest
%bin%\ideatest
if errorlevel 1 goto done

echo shatest
%bin%\shatest
if errorlevel 1 goto done

echo sha1test
%bin%\sha1test
if errorlevel 1 goto done

echo md5test
%bin%\md5test
if errorlevel 1 goto done

echo md2test
%bin%\md2test
if errorlevel 1 goto done

echo rc2test
%bin%\rc2test
if errorlevel 1 goto done

echo rc4test
%bin%\rc4test
if errorlevel 1 goto done

echo randtest
%bin%\randtest
if errorlevel 1 goto done

echo dhtest
%bin%\dhtest
if errorlevel 1 goto done

echo testenc
call %test%\testenc %bin%\ssleay
if errorlevel 1 goto done

echo verify
copy ..\certs\*.pem cert.tmp >nul
%bin%\ssleay verify -CAfile cert.tmp ..\certs\*.pem
del cert.tmp

echo testss
call %test%\testss %bin%\ssleay
if errorlevel 1 goto done

echo passed all tests
goto end
:done
echo problems.....
:end
