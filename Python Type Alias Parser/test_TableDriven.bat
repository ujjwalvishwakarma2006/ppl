@echo off
setlocal enabledelayedexpansion

:: --- Configuration ---
set "PARSER_EXECUTABLE=TableDriven.exe"
set "TEST_DELAY=1"

:: --- Test Cases ---
set test_count=0

:: Valid Cases
call :test_case "type MyInt = int"
call :test_case "type MyStr = str"
call :test_case "type UseFloat = float"
call :test_case "type CheckBool = bool"
call :test_case "type IntList = list[int]"
call :test_case "type StringList = List[str]"
call :test_case "type FloatMatrix = list[list[float]]"
call :test_case "type NameMap = dict{str:str}"
call :test_case "type MixedMap = Dict{int:List[str]}"
call :test_case "type Point = tuple(int, int)"
call :test_case "type DataRecord = Tuple(str, int, bool)"
call :test_case "type NestedTuple = tuple(int, list[str], dict{int:bool})"
call :test_case "type AliasUse = IntList"
call :test_case "type GenericList[T] = list[T]"
call :test_case "type GenericMap[K, V] = dict{K:V}"
call :test_case "type Pair[A, B] = tuple(A, B)"
call :test_case "type Constrained[T] = list[int]"

:: Invalid Cases
call :test_case "type MyInt = integer"
call :test_case "MyType = int"
call :test_case "type MissingEquals int"
call :test_case "type BadList = list[int"
call :test_case "type BadDict = dict{str:str"
call :test_case "type BadTuple = tuple(int, str"
call :test_case "type EmptyList = list[]"
call :test_case "type EmptyDict = dict{}"
call :test_case "type EmptyTuple = tuple()"
call :test_case "type EmptyTupleImplicit = ()"
call :test_case "type DictNoColon = dict{int str}"
call :test_case "type DictTooMany = dict{int:str:bool}"
call :test_case "type ListComma = list[int,]"
call :test_case "type TupleComma = tuple(int,)"
call :test_case "type BadParam = type Gen[T = list[T]"
call :test_case "type = int"
call :test_case "type 1Type = int"
call :test_case "type List = list[list]"
call :test_case "type T = TYPE"
call :test_case "type M = dict{list[int]: str}"

:: Edge Cases
call :test_case "type"
call :test_case "="
call :test_case "type Name = [int]"
call :test_case "type Name = {int:str}"
call :test_case "type Name = (int)"
call :test_case "typo Name = int"

echo.
echo =======================
echo Parser Tests Finished.
exit /b 0

:test_case
set /a test_count+=1
set "tempfile=test_!test_count!.tmp"

(
  echo %~1
  echo band kar
) > "!tempfile!"

echo.
echo --- Test Case !test_count! ---
echo Input: %~1
echo Output:

if not exist "%PARSER_EXECUTABLE%" (
  echo Error: Parser missing
  exit /b 1
)

"%PARSER_EXECUTABLE%" < "!tempfile!" || (
  echo PARSER CRASHED
  del "!tempfile!" 2>nul
  exit /b 1
)

timeout /t %TEST_DELAY% >nul
del "!tempfile!" 2>nul
echo ----------------------
exit /b