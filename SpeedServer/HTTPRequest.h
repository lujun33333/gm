#include <winsock2.h>
#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

#include <ws2tcpip.h>

#include <sstream>
#include <Wbemidl.h>
#include <comdef.h> // 必须包含此头文件以使用 _bstr_t
#pragma comment(lib, "wbemuuid.lib")

#pragma comment(lib, "Ws2_32.lib")

#pragma comment(lib, "winhttp.lib")


int VerMyIP();
std::wstring Utf8ToWString(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string GetSystemUUID() {
    HRESULT hres;
    std::string uuid;

    // Step 1: Initialize COM library
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        return "Failed to initialize COM library";
    }

    // Step 2: Set general COM security levels
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(hres)) {
        CoUninitialize();
        return "Failed to initialize security";
    }

    // Step 3: Obtain initial locator to WMI
    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc
    );

    if (FAILED(hres)) {
        CoUninitialize();
        return "Failed to create IWbemLocator object";
    }

    // Step 4: Connect to WMI
    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );

    if (FAILED(hres)) {
        pLoc->Release();
        CoUninitialize();
        return "Could not connect to WMI";
    }

    // Step 5: Set security levels on the proxy
    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "Could not set proxy blanket";
    }

    // Step 6: Query for system UUID
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_ComputerSystemProduct"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator
    );

    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "Query for UUID failed";
    }

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;
    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;
        hr = pclsObj->Get(L"UUID", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            uuid = _bstr_t(vtProp.bstrVal);
            VariantClear(&vtProp);
        }
        pclsObj->Release();
    }

    // Cleanup
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

    return uuid;
}


int SendJsonRequest(const std::string& jsonData) {
    HINTERNET hSession = WinHttpOpen(L"A WinHTTP JSON Client",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    if (hSession) {
        LPCWSTR host = L"127.0.0.1"; // Host from URL
        INTERNET_PORT port = 8095; // Port from URL

        HINTERNET hConnect = WinHttpConnect(hSession, host, port, 0);

        if (hConnect) {
            HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST",
                L"/ver", // Path from URL
                NULL,
                WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                0);

            if (hRequest) {
                LPCWSTR szHeaders = L"Content-Type: application/json\r\n";
                WinHttpAddRequestHeaders(hRequest, szHeaders, -1L, WINHTTP_ADDREQ_FLAG_ADD);

                BOOL bResults = WinHttpSendRequest(hRequest,
                    WINHTTP_NO_ADDITIONAL_HEADERS,
                    0,
                    (LPVOID)jsonData.c_str(),
                    jsonData.length(),
                    jsonData.length(),
                    0);

                if (bResults) {
                    WinHttpReceiveResponse(hRequest, NULL);

                    DWORD dwSize = 0;
                    DWORD dwDownloaded = 0;
                    LPSTR pszOutBuffer;
                    BOOL  bResults = FALSE;

                    std::string strResponse;

                    do {
                        dwSize = 0;
                        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                            std::cerr << "WinHttpQueryDataAvailable failed." << std::endl;
                            return -1;
                        }

                        pszOutBuffer = new char[dwSize + 1];
                        if (!pszOutBuffer) {
                            std::cerr << "Out of memory" << std::endl;
                            dwSize = 0;
                        }
                        else {
                            ZeroMemory(pszOutBuffer, dwSize + 1);
                            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                                std::cerr << "WinHttpReadData failed." << std::endl;
                            }
                            else {
                                strResponse.append(pszOutBuffer, dwDownloaded);
                            }

                            delete[] pszOutBuffer;
                        }

                    } while (dwSize > 0);

                    if (strResponse.empty()) {
                        return 1;
                    }
                    else {
                        system("chcp 65001");
                        std::cout << jsonData << std::endl;
                        std::cout << strResponse << std::endl;
                    }
                }
                else {
                    DWORD dwError = GetLastError();
                    std::cerr << "WinHttpSendRequest failed with error: " << dwError << std::endl;
                }

                WinHttpCloseHandle(hRequest);
            }
            else {
                DWORD dwError = GetLastError();
                std::cerr << "WinHttpOpenRequest failed with error: " << dwError << std::endl;
            }

            WinHttpCloseHandle(hConnect);
        }
        else {
            DWORD dwError = GetLastError();
            std::cerr << "WinHttpConnect failed with error: " << dwError << std::endl;
        }

        WinHttpCloseHandle(hSession);
    }
    else {
        DWORD dwError = GetLastError();
        std::cerr << "WinHttpOpen failed with error: " << dwError << std::endl;
    }
    return -1;
}

std::string getLocalIP() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return "";
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        std::cerr << "Failed to get hostname" << std::endl;
        WSACleanup();
        return "";
    }

    struct addrinfo* result = nullptr, * ptr = nullptr, hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
        std::cerr << "Failed to get address info" << std::endl;
        WSACleanup();
        return "";
    }

    std::string ipAddress;
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(ptr->ai_family, &((struct sockaddr_in*)ptr->ai_addr)->sin_addr, ip, sizeof(ip));
        ipAddress = ip;
        break;  // Use the first available IP address
    }

    freeaddrinfo(result);
    WSACleanup();

    return ipAddress;
}

std::string createJson(const std::string& ip) {
    std::ostringstream jsonStream;
    jsonStream << "{"
        << "\"uin\":\"" << ip << "\","
        << "\"code\":null"
        << "}";
    return jsonStream.str();
}

std::wstring StringToWString(const std::string& str) {
    std::wstring wstr(str.length(), L'\0');
    std::mbstowcs(&wstr[0], str.c_str(), str.size());
    return wstr;
}

int VerMyIP() {
    return 1;
    std::string ip = getLocalIP();
    ip = ip +  "==" + GetSystemUUID();
    if (!ip.empty()) {
        std::string json = createJson(ip);
       
        if (SendJsonRequest(json) > 0) {
            return 1;
        }
    }
    else {
        std::cerr << "Failed to retrieve local IP address" << std::endl;
        return -1;
    }

    //注意：URI地址必须以 http:// 开头，否则不符合头文件校验规则
    
    return -1;
}