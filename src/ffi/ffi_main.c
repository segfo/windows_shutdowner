#include <windows.h>
#include <stdio.h>

BOOL EnablePrivileges(LPTSTR lpPrivilegeName, BOOL bEnable)
{
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tokenPrivileges;
    BOOL bRet;


    //1.OpenProcessToken関数で、プロセストークンを取得する
    bRet = OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
        &hToken);
    if (!bRet) {
        return FALSE;
    }

    //2.LookupPrivilegeValue関数で、特権に対応するLUID(ローカル一意識別子)を取得する
    bRet = LookupPrivilegeValue(NULL, lpPrivilegeName, &luid);
    if (bRet) {

        //3.TOKEN_PRIVILEGES型のオブジェクトに、LUID(ローカル一意識別子)と特権の属性(有効にするか無効にするか)を指定する
        tokenPrivileges.PrivilegeCount = 1;
        tokenPrivileges.Privileges[0].Luid = luid;
        tokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

        //4.AdjustTokenPrivileges関数で、特権を有効にする
        AdjustTokenPrivileges(hToken,
            FALSE,
            &tokenPrivileges, 0, 0, 0);

        bRet = GetLastError() == ERROR_SUCCESS;

    }

    CloseHandle(hToken);

    return bRet;
}

int try_shutdown()
{
    //SE_SHUTDOWN_NAME(シャットダウン特権) を有効にする
    if ( !EnablePrivileges(SE_SHUTDOWN_NAME, TRUE) ) {
        return 1;
    }

    if ( !ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0) ) {
        return 2;
    }
    return 0;
}
